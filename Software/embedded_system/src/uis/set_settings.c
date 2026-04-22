#include "../lcd_ui.h"
uint8_t positionsSetSettings[2] = {5, 13};
void checkCursorSetSettings() // for writing a 4 digit float 1 of those being a decimal place
{
	cursor = (cursor + 1) % 2;
	Set_CursorPosition(1, positionsSetSettings[cursor]);
}
void showSetSettingsMessageCallback(void *aux)
{
	char *value = (char *)aux;
	WriteStringAt("Set Settings:", 0, 0);
	WriteStringAt("PWM: ", 1, 0);
	LCD_WriteData(value[0]);
	Write_String_LCD(" CTRL: ");
	LCD_WriteData(value[1]);
}
void keyPressSetSettings(char key)
{
	uint8_t pos = cacheLCD.position;
	uint8_t line = cacheLCD.line;
	Set_CursorPosition(0, 15);
	LCD_WriteData(key);
	Set_CursorPosition(line, pos);

	if (key >= '0' && key <= '9')
	{
		if (key >= '1')
			LCD_WriteData('1');
		else
			LCD_WriteData(key);
		checkCursorSetSettings();
	}
	else if (key == 'D')
	{
		checkCursorSetSettings();
	}
	else if (key == '#')
	{
		char enablePWMChar = cacheLCD.string[16 + positionsSetSettings[0]];
		char enableControlChar = cacheLCD.string[16 + positionsSetSettings[1]];
		bool enablePWM = enablePWMChar == '1' ? true : false;
		bool enableControlForm = enableControlChar == '1' ? true : false;
		char bufferString[2] = {enablePWMChar, enableControlChar};
		controllerData.control = enableControlForm;
		showMessageForTime(showSetSettingsMessageCallback, 2000, SystemVitals, bufferString);
		if (!controllerData.control)
		{
			clearRun_PID();
		}

		if (enablePWM)
		{

			TIM1_8_Enable();
			if (!controllerData.control)
			{

				updateTIM1_8_PowerLevelWithStart(controllerData.frequency, controllerData.powerLevel);
				// updateTIM1_8_PowerLevel(controllerData.frequency, controllerData.powerLevel);
			}
			else
			{
				run_PID();
			}
		}
		else
			TIM1_8_Disable();
	}
}

void setupSetSettings()
{
	cursor = 0;
	WriteStringAt("Settings:", 0, 0);
	WriteStringAt("PWM: ", 1, 0);
	LCD_WriteData(isPWMEnabled ? '1' : '0');
	Write_String_LCD(" CTRL: ");
	LCD_WriteData(controllerData.control ? '1' : '0');
	Set_CursorPosition(1, positionsSetSettings[0]);
}
void modeStartStopSettings(bool startNotStop)
{
	if (startNotStop)
	{
		setupSetSettings();
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOn | DisplayOnOffControlCursorBlinkOn, 1);
		subscribeKeyPress(keyPressSetSettings);
	}
	else
	{
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOff | DisplayOnOffControlCursorBlinkOff, 1);

		unsubscribeKeyPress(keyPressSetSettings);
	}
}