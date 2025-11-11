#include <stdint.h>
#include "lcd.h"
#include "main.h"
static int16_t last = 0;
void onRotate(int16_t cnt, uint32_t counting_down)
{
	// int16_t delta = (int16_t)(cnt - last);
	// if (counting_down && delta > 0)
	// {
	// 	delta -= (getTimer3Instance().ARR + 1);
	// }
	// else if (!counting_down && delta < 0)
	// {
	// 	delta += (getTimer3Instance().ARR + 1);
	// }
	// last = cnt;

	// // Example: update display only on change
	// char buf[8];
	// snprintf(buf, sizeof buf, "%03d", (int)cnt);
	// Write_String_Sector_LCD(0, buf);
}
void onWrap(uint32_t counting_down)
{
}
uint8_t buttonState = 0;
void onRotaryPress(uint8_t state)
{
	// buttonState ^=1;
	// char *buttonString = (buttonState) ? "Button State, 1" : "Button State, 0";
	// //		Write_String_Sector_LCD(4, clearSector);
	// Write_String_Sector_LCD(4, buttonString);
}
