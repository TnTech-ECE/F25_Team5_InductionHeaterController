
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

struct Run
{
	uint32_t delay;
	uint32_t _delayLeft;
	OnTimeCallback *callback;
	UntilCheckCallback *UntilCheckCallback;
};
/**
 * @param callback
 * @param delay in milliseconds
 */
uint8_t runInterval(OnTimeCallback callback, uint32_t delay);
uint8_t runTimeout(OnTimeCallback callback, uint32_t delay);
uint8_t runIntervalUntil(UntilCheckCallback UntilCheckCallback, OnTimeCallback callback, uint32_t delay);
#endif /* DELAY_H_ */
