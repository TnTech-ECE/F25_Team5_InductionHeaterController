#include "../lcd_ui.h"

void keyPressDisplayPress(char key)
{
	Set_CursorPosition(0, 0);
	LCD_WriteData(key);
}

void modeStartStopTestKeypad(bool startNotStop)
{
	if (startNotStop)
	{
		subscribeKeyPress(keyPressDisplayPress);
	}
	else
	{
		unsubscribeKeyPress(keyPressDisplayPress);
	}
}