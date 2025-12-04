
/*
 * delay.h
 *
 *  Created on: Nov 11, 2025
 *      Author: mrpat
 */
#include <stdint.h>
#include <stdbool.h>

#ifndef DELAY_H_
#define DELAY_H_

void Delay_TIM_2_Callback();

typedef void OnTimeCallback(void);
typedef bool UntilCheckCallback(void);
uint64_t getDelayScale();
struct Run
{
	int id;
	unsigned delay;
	long _delayLeft;
	OnTimeCallback *callback;
	UntilCheckCallback *UntilCheckCallback;
};
/**
 * @param callback
 * @param delay in milliseconds
 */
int runInterval(OnTimeCallback callback, unsigned delay);
int runTimeout(OnTimeCallback callback, unsigned delay);
int runIntervalUntil(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, unsigned delay);
bool clearRun(int runId);
#endif /* DELAY_H_ */
