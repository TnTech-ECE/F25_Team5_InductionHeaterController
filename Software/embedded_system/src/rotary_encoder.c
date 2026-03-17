#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay.h"
#include "main.h"
#include "save.h"
#include "run.h"
#include "lcd_ui.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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

void onRotateTimeoutCallback(void *aux)
{

	if (TIM3->CNT != timeoutLast)
	{
		long delta = ((long)TIM3->CNT - (long)timeoutLast);
		long deltaABS = abs((long)TIM3->CNT - (long)timeoutLast);
		int direction = sign(delta);
		delta = direction * fmin(deltaABS, TIM3->ARR - deltaABS);

		timeoutLast = TIM3->CNT;

		char numberString[6];
		memcpy(numberString, cacheLCD.string + sizeof(char) * (16 + setOffset), sizeof(numberString));
		numberString[5] = '\0';
		char *endptr;
		errno = 0; // Reset for error checking
		float value = strtof(numberString, &endptr);
		int intValue = strtoul(numberString, &endptr, 10);
		uint8_t pos = cacheLCD.position;
		uint8_t line = cacheLCD.line;
		switch (displayMode)
		{
		case SetPowerLevel:
		{
			value += (float)(delta) / 10.0f;
			if (value > 100.0f)
				value = 0.0f;
			else if (value < 0.0f)
				value = 100.0f;
			DisplayDecimalWithPlaces(value, 1, setOffset, 0, 5, 1);
			break;
		}
		case SetDesiredTemperature:
		{
			value += (float)(delta) / 10.0f;
			if (value > 999.9f)
				value = 0.0f;
			else if (value < 0.0f)
				value = 999.9f;
			DisplayDecimalWithPlaces(value, 1, setOffset, 0, 5, 1);
			break;
		}
		case SetFrequency:
		{
			intValue += delta * 100;
			if (intValue > 99999)
				intValue = 0;
			else if (intValue < 0)
				intValue = 99999;
			DisplayNumber(intValue, 1, setOffset, 0, 5);
			break;
		}
		default:
		{
			break;
		}
		}
		Set_CursorPosition(line, pos);
		// DisplayDecimal(controllerData.desiredTemperature, 1, 8, 0, 5);
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
void onRotaryPressTimeoutCallback(void *aux)
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
