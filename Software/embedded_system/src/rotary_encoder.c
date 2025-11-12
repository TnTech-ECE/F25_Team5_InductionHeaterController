#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay.h"
#include "main.h"
static int16_t last = 0;
void onRotate(int16_t cnt, uint32_t counting_down)
{
	// int16_t delta = (int16_t)(cnt - last);
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
	DisplayNumber(cnt, 1, 4, 0, 3);

	DisplayNumber(delta, 1, 8, 0, 3);
	DisplayNumber(counting_down, 1, 12, 0, 3);
}
void onWrap(uint32_t counting_down)
{
}
uint8_t buttonState = 0;
bool pressTimeOut = false;
void onRotaryPressTimeoutCallback(void)
{
	buttonState ^= 1;
	DisplayNumber(buttonState, 0, 4, 0, 1);
	pressTimeOut = false;
}
void onRotaryPress()
{
	if (pressTimeOut)
		return;
	pressTimeOut = true;
	runTimeout(onRotaryPressTimeoutCallback, 20);

	// buttonState ^=1;
	// char *buttonString = (buttonState) ? "Button State, 1" : "Button State, 0";
	// //		Write_String_Sector_LCD(4, clearSector);
	// Write_String_Sector_LCD(4, buttonString);
}
