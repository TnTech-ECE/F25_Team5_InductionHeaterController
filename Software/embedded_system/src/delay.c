#include "delay.h"
#include <stdint.h>
#include <stdbool.h>
void removeRunFromList(int runId, int index);
int runListLength = 0;
int currentRunId = 0;
#define MAX_RUN_LIST 16
struct Run RunList[MAX_RUN_LIST];

bool timeoutCallback()
{
	return true;
}
void Delay_TIM_2_Callback()
{
	for (int i = 0; i < runListLength; i++)
	{
		struct Run run = RunList[i];
		if (--run._delayLeft <= 0)
		{
			run._delayLeft = run.delay;
			if (run.callback != NULL)
				run.callback();
			if (run.UntilCheckCallback != NULL && run.UntilCheckCallback())
			{
				removeRunFromList(run.id, i);
			}
		}
	}
}
int runInterval(OnTimeCallback callback, uint32_t delay)
{
	struct Run run;
	run.callback = &callback;
	run.delay = delay;
	run._delayLeft = delay;
	run.id = currentRunId++;
	run.UntilCheckCallback = NULL;
	if (runListLength >= MAX_RUN_LIST)
		return -1;
	RunList[runListLength] = run;
	++runListLength;
}
int runTimeout(OnTimeCallback callback, uint32_t delay)
{
	struct Run run;
	run.callback = &callback;
	run.delay = delay;
	run._delayLeft = delay;
	run.id = currentRunId++;
	run.UntilCheckCallback = &timeoutCallback;
	if (runListLength >= MAX_RUN_LIST)
		return -1;
	RunList[runListLength] = run;
	++runListLength;
}
int runIntervalUntil(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, uint32_t delay)
{
	struct Run run;
	run.callback = &callback;
	run.delay = delay;
	run._delayLeft = delay;
	run.id = currentRunId++;
	run.UntilCheckCallback = &UntilCheckCallback;
	if (runListLength >= MAX_RUN_LIST)
		return -1;
	RunList[runListLength] = run;
	++runListLength;
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
	int runIdFound = -1;
	for (int i = 0; i < runListLength; i++)
	{
		struct Run run = RunList[i];

		if (run.id == runId)
		{
			index = i;
			runIdFound = runId;
			break;
		}
	}
	if (runIdFound == -1)
		return false;
	removeRunFromList(runIdFound, index);
}
void removeRunFromList(int runIdFound, int index)
{
	if (runIdFound == -1)
		return;
	if (index == -1)
		return;
	if (RunList[index].id != runIdFound)
		return;
	for (int i = index; i < runListLength; i++)
	{
		struct Run run = RunList[i];

		RunList[i - 1] = run;
	}
	runListLength--;
	RunList[runListLength] = getNullRun();
}