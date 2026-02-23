#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay.h"
#include "main.h"
#include "save.h"
#include "run.h"
int sign(int x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}

static unsigned last = 0;
static unsigned timeoutLast = UINT32_MAX;
bool onRotateTimeout = false;
#define ON_ROTATE_TIMEOUT_DELAY 100
void runRotateTimeout(bool force);

void onRotateTimeoutCallback(void)
{

	if (TIM3->CNT != timeoutLast)
	{
		long delta = ((long)TIM3->CNT - (long)timeoutLast);
		long deltaABS = abs((long)TIM3->CNT - (long)timeoutLast);
		int direction = sign(delta);
		delta = direction * fmin(deltaABS, TIM3->ARR - deltaABS);
		//		DisplayNumber(delta, 1, 7, 0, 4);

		timeoutLast = TIM3->CNT;
		controllerData.desiredTemperature += (float)(delta) / 10.0f;
		save();
		runRotateTimeout(true);
		onRotateTimeout = true;
	}
	else
	{
		onRotateTimeout = false;
		//		DisplayNumber(0, 1, 7, 0, 4);
	}
}
void runRotateTimeout(bool force)
{
	if (!force && onRotateTimeout)
		return;
	onRotateTimeout = true;
	timeoutLast = TIM3->CNT;
	runTimeout(onRotateTimeoutCallback, ON_ROTATE_TIMEOUT_DELAY);
}
void onRotate(int16_t cnt, unsigned counting_down)
{
	int16_t delta = (int16_t)(cnt - last);
	if (counting_down && delta > 0)
	{
		delta -= (getTimer3Instance().ARR + 1);
	}
	else if (!counting_down && delta < 0)
	{
		delta += (getTimer3Instance().ARR + 1);
	}
	last = cnt;
	// DisplayNumber((int)cnt, 1, 12, 0, 4);
	runRotateTimeout(false);
}
void onWrap(unsigned counting_down)
{
}
uint8_t buttonState = 0;
bool pressTimeOut = false;
void onRotaryPressTimeoutCallback(void)
{
	buttonState ^= 1;
	//	DisplayNumber(buttonState, 0, 4, 0, 1);
	pressTimeOut = false;
}
void onRotaryPress()
{
	if (pressTimeOut)
		return;
	pressTimeOut = true;
	runTimeout(onRotaryPressTimeoutCallback, 1000);

	// buttonState ^=1;
	// char *buttonString = (buttonState) ? "Button State, 1" : "Button State, 0";
	// //		Write_String_Sector_LCD(4, clearSector);
	// Write_String_Sector_LCD(4, buttonString);
}
