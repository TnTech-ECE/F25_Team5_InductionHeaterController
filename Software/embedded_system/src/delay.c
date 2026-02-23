#include "delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
int queueRun(OnTimeCallback callback, unsigned delay, UntilCheckCallback UntilCheckCallback, void *aux);
void removeRunFromList(int runId, int index);
int runListLength = 0;
int currentRunId = 0;
#define MAX_RUN_LIST 16
struct Run RunList[MAX_RUN_LIST];
uint64_t getDelayScale()
{
	// 80 MHz core -> derive how many TIM2 updates make 1ms
	uint64_t psc = (uint64_t)TIM2->PSC + 1U;
	uint64_t arr = (uint64_t)TIM2->ARR + 1U;
	return 80000000UL / (1000UL * psc * arr);
}
bool timeoutCallback()
{
	return true;
}
unsigned time = 0;
void Delay_TIM_2_Callback()
{
	if (++time < getDelayScale())
		return;
	// LCD_WriteData('a');
	time = 0;
	for (int i = 0; i < runListLength;)
	{
		struct Run *run = &RunList[i];
		if (--run->_delayLeft <= 0)
		{
			run->_delayLeft = run->delay;
			if (run->callback != NULL)
				run->callback(run->aux);
			if (run->UntilCheckCallback != NULL && run->UntilCheckCallback(run->aux))
			{
				removeRunFromList(run->id, i);
				continue;
			}
		}
		++i;
	}
}
int runInterval(OnTimeCallback callback, unsigned delay)
{
	return queueRun(callback, delay, NULL, NULL);
}
int runTimeout(OnTimeCallback callback, unsigned delay)
{
	return queueRun(callback, delay, timeoutCallback, NULL);
}
int runIntervalUntil(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, unsigned delay)
{
	return queueRun(callback, delay, UntilCheckCallback, NULL);
}

int runIntervalAux(OnTimeCallback callback, unsigned delay, void *aux)
{
	return queueRun(callback, delay, NULL, aux);
}
int runTimeoutAux(OnTimeCallback callback, unsigned delay, void *aux)
{
	return queueRun(callback, delay, timeoutCallback, aux);
}
int runIntervalUntilAux(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, unsigned delay, void *aux)
{
	return queueRun(callback, delay, UntilCheckCallback, aux);
}
struct Run getNullRun()
{
	struct Run run;
	run.callback = NULL;
	run.UntilCheckCallback = NULL;
	run.delay = 0;
	run._delayLeft = 0;
	run.id = -1;
	return run;
}
bool clearRun(int runId)
{
	int index = -1;
	for (int i = 0; i < runListLength; i++)
	{

		if (RunList[i].id == runId)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return false;
	removeRunFromList(runId, index);
	return true;
}
void removeRunFromList(int runIdFound, int index)
{
	if (runIdFound == -1)
		return;
	if (index < 0 || index >= runListLength)
		return;
	if (RunList[index].id != runIdFound)
		return;
	__disable_irq();
	for (int i = index + 1; i < runListLength; i++)
	{
		RunList[i - 1] = RunList[i];
	}
	runListLength--;
	RunList[runListLength] = getNullRun();
	__enable_irq();
}

int queueRun(OnTimeCallback callback, unsigned delay, UntilCheckCallback UntilCheckCallback, void *aux)
{
	struct Run run;
	run.callback = callback;
	run.delay = delay;
	run._delayLeft = delay;
	run.aux = aux;
	if (currentRunId >= INT32_MAX)
	{
		currentRunId = 0;
	}
	run.id = currentRunId++;
	run.UntilCheckCallback = UntilCheckCallback;
	__disable_irq();
	if (runListLength >= MAX_RUN_LIST)
	{
		__enable_irq();
		return -1;
	}
	RunList[runListLength++] = run;
	__enable_irq();
	return run.id;
}