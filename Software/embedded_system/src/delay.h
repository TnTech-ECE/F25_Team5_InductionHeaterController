
/*
 * delay.h
 *
 *  Created on: Nov 11, 2025
 *      Author: mrpat
 */

#ifndef DELAY_H_
#define DELAY_H_
#include <stdint.h>
#include <stdbool.h>

void Delay_TIM_2_Callback();

typedef void OnTimeCallback(void);
typedef bool UntilCheckCallback(void);
uint32_t getDelayScale();
struct Run
{
	int id;
	uint32_t delay;
	long _delayLeft;
	OnTimeCallback *callback;
	UntilCheckCallback *UntilCheckCallback;
};
/**
 * @param callback
 * @param delay in milliseconds
 */
int runInterval(OnTimeCallback callback, uint32_t delay);
int runTimeout(OnTimeCallback callback, uint32_t delay);
int runIntervalUntil(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, uint32_t delay);
bool clearRun(int runId);
#endif /* DELAY_H_ */
