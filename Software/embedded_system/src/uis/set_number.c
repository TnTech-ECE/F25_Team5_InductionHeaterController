#include "../lcd_ui.h"
void showSetFrequencyMessageCallback(void *aux)
{
	WriteStringAt(setMessage, 0, 0);
	unsigned int *value = (unsigned int *)aux;
	DisplayNumber(*value, 1, 0, 0, 5);

	Write_String_LCD(setUnit);
}
void showSetMessageCallback(void *aux)
{
	WriteStringAt(setMessage, 0, 0);
	float *value = (float *)aux;
	DisplayDecimalWithPlaces(*value, 1, 0, 0, 5, 1);
	Write_String_LCD(setUnit);
}

void checkCursorSet(bool move) // for writing a 4 digit float 1 of those being a decimal place
{
	cursor++;
	if ((displayMode == SetPowerLevel || displayMode == SetDesiredTemperature) && cursor == 3)
	{
		cursor++;
		Set_CursorPosition(1, setOffset + cursor);
	}
	else if (cursor >= 5)
	{
		cursor = 0;
		Set_CursorPosition(1, setOffset + cursor);
	}
	else if (move)
	{
		Set_CursorPosition(1, setOffset + cursor);
	}
}
void setupSetPowerLevel()
{
	cursor = 0;
	WriteStringAt("Enter Power", 0, 0);
	WriteStringAt("Level:", 1, 0);
	setOffset = 7;
	setUnit = "%";
	setMessage = "Set Power Level:";
	DisplayDecimalWithPlaces(controllerData.powerLevel, 1, setOffset, 0, 5, 1);
	Write_String_LCD(setUnit);
	Set_CursorPosition(1, setOffset);
}
void setupSetDesiredTemperature()
{
	cursor = 0;
	WriteStringAt("Enter Desired", 0, 0);
	WriteStringAt("Temp:", 1, 0);
	setOffset = 6;
	setUnit = "\x80"
			  "C";
	setMessage = "Set Desired Temp:";

	DisplayDecimalWithPlaces(controllerData.desiredTemperature, 1, setOffset, 0, 5, 1);
	Write_String_LCD(setUnit);
	Set_CursorPosition(1, setOffset);
}
void setupSetFrequency()
{
	cursor = 0;
	WriteStringAt("Enter Frequency:", 0, 0);
	setOffset = 0;
	setUnit = " Hz";
	setMessage = "Set Frequency:";
	DisplayNumber(controllerData.frequency, 1, setOffset, 0, 5);
	Write_String_LCD(setUnit);
	Set_CursorPosition(1, setOffset);
}
void keyPressSetNumber(char key)
{
	// uint8_t pos = cacheLCD.position;
	// uint8_t line = cacheLCD.line;
	// DisplayNumber(displayMode, 0, 15, 1, 2);

	// Set_CursorPosition(line, pos);

	if (key >= '0' && key <= '9')
	{
		if (displayMode == SetPowerLevel)
		{
			if (!cursor)
			{
				if (key >= '1')
				{
					DisplayDecimalWithPlaces(100.0, 1, setOffset, 0, 5, 1);
					Set_CursorPosition(1, setOffset);
					return;
				}
			}
			else if (cacheLCD.string[16 + setOffset] == '1')
			{
				return;
			}
		}
		LCD_WriteData(key);
		checkCursorSet(false);
	}
	else if (key == 'D')
	{
		checkCursorSet(true);
	}
	else if (key == '#')
	{
		char numberString[6];
		memcpy(numberString, cacheLCD.string + sizeof(char) * (16 + setOffset), sizeof(numberString));
		numberString[5] = '\0';
		char *endptr;
		errno = 0; // Reset for error checking
		float value = strtof(numberString, &endptr);
		unsigned int intValue = strtoul(numberString, &endptr, 10);
		if (numberString == endptr)
		{
			showErrorMessage("ERROR: numberString == endptr");
			return;
		}
		else if (errno == ERANGE)
		{
			showErrorMessage("ERROR: ERANGE");
			return;
		}
		switch (displayMode)
		{
		case SetPowerLevel:
		{
			controllerData.powerLevel = value;
			showMessageForTime(showSetMessageCallback, 2000, SystemVitals, &controllerData.powerLevel);
			updateTIM1_8_PowerLevelWithStart(controllerData.frequency, controllerData.powerLevel);
			break;
		}
		case SetDesiredTemperature:
		{
			controllerData.desiredTemperature = value;
			showMessageForTime(showSetMessageCallback, 2000, SystemVitals, &controllerData.desiredTemperature);
			break;
		}
		case SetFrequency:
		{
			controllerData.frequency = intValue;
			showMessageForTime(showSetFrequencyMessageCallback, 2000, SystemVitals, &controllerData.frequency);
			updateTIM1_8_PowerLevel(controllerData.frequency, controllerData.powerLevel);
			break;
		}
		default:
			break;
		}
		saveSD();
	}
}
void modeStartStopDesiredTemperature(bool startNotStop)
{
	if (startNotStop)
	{
		setupSetDesiredTemperature();
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOn | DisplayOnOffControlCursorBlinkOn, 1);
		subscribeKeyPress(keyPressSetNumber);
	}
	else
	{
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOff | DisplayOnOffControlCursorBlinkOff, 1);

		unsubscribeKeyPress(keyPressSetNumber);
	}
}
void modeStartStopPowerLevel(bool startNotStop)
{
	if (startNotStop)
	{
		setupSetPowerLevel();
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOn | DisplayOnOffControlCursorBlinkOn, 1);
		subscribeKeyPress(keyPressSetNumber);
	}
	else
	{
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOff | DisplayOnOffControlCursorBlinkOff, 1);

		unsubscribeKeyPress(keyPressSetNumber);
	}
}
void modeStartStopFrequency(bool startNotStop)
{
	if (startNotStop)
	{
		setupSetFrequency();
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOn | DisplayOnOffControlCursorBlinkOn, 1);
		subscribeKeyPress(keyPressSetNumber);
	}
	else
	{
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOff | DisplayOnOffControlCursorBlinkOff, 1);

		unsubscribeKeyPress(keyPressSetNumber);
	}
}