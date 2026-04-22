#include "../lcd_ui.h"

uint8_t positionsSetPID[9] =
	{
		8, 10, 11,
		16 + 3, 16 + 5, 16 + 6,
		16 + 11, 16 + 13, 16 + 14};
//"PID: P: 00.0"
//"I: 00.0 D: 00.0";
void Set_CursorPositionPID(uint8_t index)
{
	if (index > 8)
		return;
	uint8_t positionPlus16TimesRow = positionsSetPID[index];
	uint8_t position = positionPlus16TimesRow % 16;
	uint8_t line = positionPlus16TimesRow / 16;
	Set_CursorPosition(line, position);
}
void checkCursorSetPID() // for writing a 4 digit float 1 of those being a decimal place
{
	cursor = (cursor + 1) % 9;
	Set_CursorPositionPID(cursor);
}
void setupSetPID()
{
	cursor = 0;
	WriteStringAt("PID: ", 0, 0);
	Write_String_LCD("P: ");
	DisplayDecimalWithPlaces(controllerData.kp, -1, -1, 0, 4, 2);

	WriteStringAt("I: ", 1, 0);
	DisplayDecimalWithPlaces(controllerData.ki, -1, -1, 0, 4, 2);
	Write_String_LCD(" D: ");
	DisplayDecimalWithPlaces(controllerData.kd, -1, -1, 0, 4, 2);
	Set_CursorPositionPID(cursor);
}
void showSetPIDMessageCallback(void *aux)
{
	cursor = 0;
	WriteStringAt("Set PID: ", 0, 0);
	Write_String_LCD("P: ");
	DisplayDecimalWithPlaces(controllerData.kp, -1, -1, 0, 4, 2);

	WriteStringAt("I: ", 1, 0);
	DisplayDecimalWithPlaces(controllerData.ki, -1, -1, 0, 4, 2);
	Write_String_LCD(" D: ");
	DisplayDecimalWithPlaces(controllerData.kd, -1, -1, 0, 4, 2);
}
void keyPressSetPID(char key)
{
	// uint8_t pos = cacheLCD.position;
	// uint8_t line = cacheLCD.line;
	// DisplayNumber(displayMode, 0, 15, 1, 2);

	// Set_CursorPosition(line, pos);

	if (key >= '0' && key <= '9')
	{
		LCD_WriteData(key);
		checkCursorSetPID();
	}
	else if (key == 'D')
	{
		checkCursorSetPID();
	}
	else if (key == '#')
	{
		char numberStringP[5];
		char numberStringI[5];
		char numberStringD[5];
		memcpy(numberStringP, cacheLCD.string + sizeof(char) * (positionsSetPID[0]), sizeof(numberStringP));
		numberStringP[5] = '\0';
		memcpy(numberStringI, cacheLCD.string + sizeof(char) * (positionsSetPID[3]), sizeof(numberStringI));
		numberStringI[5] = '\0';

		memcpy(numberStringD, cacheLCD.string + sizeof(char) * (positionsSetPID[6]), sizeof(numberStringD));
		numberStringD[5] = '\0';
		char *endptr;
		errno = 0; // Reset for error checking
		float valueKp = strtof(numberStringP, &endptr);
		if (numberStringP == endptr)
		{
			showErrorMessage("ERROR: numberStringP == endptr");
			return;
		}
		else if (errno == ERANGE)
		{
			showErrorMessage("ERROR: ERANGE");
			return;
		}
		errno = 0;
		float valueKi = strtof(numberStringI, &endptr);
		if (numberStringI == endptr)
		{
			showErrorMessage("ERROR: numberStringI == endptr");
			return;
		}
		else if (errno == ERANGE)
		{
			showErrorMessage("ERROR: ERANGE");
			return;
		}
		errno = 0;
		float valueKd = strtof(numberStringD, &endptr);
		if (numberStringD == endptr)
		{
			showErrorMessage("ERROR: numberStrinD == endptr");
			return;
		}
		else if (errno == ERANGE)
		{
			showErrorMessage("ERROR: ERANGE");
			return;
		}
		controllerData.kp = valueKp;
		controllerData.ki = valueKi;
		controllerData.kd = valueKd;

		showMessageForTime(showSetPIDMessageCallback, 2000, SystemVitals, NULL);
		saveSD();
	}
}

void modeStartStopPID(bool startNotStop)
{
	if (startNotStop)
	{
		setupSetPID();
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOn | DisplayOnOffControlCursorBlinkOn, 1);
		subscribeKeyPress(keyPressSetPID);
	}
	else
	{
		LCD_WriteCommand(DisplayOnOffControlDisplayOn | DisplayOnOffControlCursorOff | DisplayOnOffControlCursorBlinkOff, 1);

		unsubscribeKeyPress(keyPressSetPID);
	}
}