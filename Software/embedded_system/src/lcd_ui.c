
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "lcd.h"
#include "lcd_ui.h"
#include "delay.h"
#include "run.h"
#include "save.h"
#include "keypad.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "max31856.h"
#include "pwm.h"
#include "lcd_instruction_set.h"
#include "PID.h"
enum DisplayMode displayMode = NoModeSet;

bool displayLocked = false;
unsigned cursor = 0;
void keyPressSwitchModeCallback(char key)
{
	switch (key)
	{
	case '9':
	{
		setDisplayMode(SetPID);
		break;
	}
	case 'A':
	{
		setDisplayMode(SetPowerLevel);
		break;
	}
	case 'B':
	{
		setDisplayMode(SetDesiredTemperature);
		break;
	}
	case 'C':
	{
		setDisplayMode(SetFrequency);
		break;
	}
	case 'D':
	{
		setDisplayMode(SetSettings);
		break;
	}
	}
}

int setOffset = 7;
static char *setUnit = "";
static char *setMessage = "";

void runTimeoutMessageCallback(void *aux)
{
	enum DisplayMode *displayMode = (enum DisplayMode *)aux;
	displayLocked = false;
	setDisplayMode(*displayMode);
}

void showMessageForTime(MessageCallback callbackMessage, unsigned time, enum DisplayMode displayMode, void *aux)
{
	if (displayLocked)
		return;
	setDisplayMode(DisplayMessage);
	static enum DisplayMode pendingReturnMode;
	pendingReturnMode = displayMode;
	if (callbackMessage != NULL)
		callbackMessage(aux);
	runTimeoutAux(runTimeoutMessageCallback, time, &pendingReturnMode);
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
void showSetSettingsMessageCallback(void *aux)
{
	char *value = (char *)aux;
	WriteStringAt("Set Settings:", 0, 0);
	WriteStringAt("PWM: ", 1, 0);
	LCD_WriteData(value[0]);
	Write_String_LCD(" CTRL: ");
	LCD_WriteData(value[1]);
}
void showErrorMessageCallback(void *aux)
{
	char *message = (char *)aux;
	WriteStringAt(message, 0, 0);
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

	Write_String_LCD(setUnit);
}
void showErrorMessage(char *message)
{
	showMessageForTime(showErrorMessageCallback, 2000, SystemVitals, message);
}
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
			updateTIM1_8_PowerLevel(controllerData.frequency, controllerData.powerLevel);
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
uint8_t positionsSetSettings[2] = {5, 13};
void checkCursorSetSettings() // for writing a 4 digit float 1 of those being a decimal place
{
	cursor = (cursor + 1) % 2;
	Set_CursorPosition(1, positionsSetSettings[cursor]);
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
				TIM1_8_start();
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
void setupSetSettings()
{
	cursor = 0;
	WriteStringAt("Settings:", 0, 0);
	WriteStringAt("PWM: ", 1, 0);
	LCD_WriteData(isPWMStarted ? '1' : '0');
	Write_String_LCD(" CTRL: ");
	LCD_WriteData(controllerData.control ? '1' : '0');
	Set_CursorPosition(1, positionsSetSettings[0]);
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

void readThermocouples(void *aux)
{
	float tempSPI2 = max31856_read_TC_temp(&thermoSPI2);
	if (thermoSPI2.sr.val)
	{
		return;
	}
	float tempSPI3 = max31856_read_TC_temp(&thermoSPI3);
	if (thermoSPI3.sr.val)
	{
		return;
	}
	// tempSPI2 /= 128.0f;
	DisplayDecimal(tempSPI2, 0, 0, 0, 5);
	DisplayDecimal(tempSPI3, 0, 6, 0, 5);
	// DisplayNumberBase(tempSPI2, 0, 0, 0, 8, 16);
	// float tempSPI2 = tc_readTemp(thermoSPI2);
	// float tempSPI3 = tc_readTemp(thermoSPI3);
	// controllerData.t1 = tempSPI2;
	// controllerData.t2 = tempSPI3;
	// // int voltsADC3 = 5.0f * (float)(value_adc) / 4096.0f;
	// DisplayDecimal(tempSPI2, 0, 0, 0, 4);
	// DisplayDecimal(tempSPI3, 1, 0, 0, 4);
	// float flowRate = getFlowRateGPS();
	// DisplayDecimal(flowRate, 0, 5, 0, 6);
	// DisplayDecimal(controllerData.desiredTemperature, 0, 12, 0, 4);
	// log();
}
unsigned i = 0;
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
bool reWriteTimer = false;
bool displayTimer = false;

void timerCallback(void *aux)
{
	int seconds = i % SECONDS_PER_MINUTE;
	int minutes = (i / SECONDS_PER_MINUTE) % SECONDS_PER_MINUTE;
	int hours = i / SECONDS_PER_HOUR;
	if (displayTimer)
	{
		if (reWriteTimer || !(i % SECONDS_PER_HOUR))
			DisplayNumber(hours, 1, 0, 0, 2);
		if (reWriteTimer || !i)
			WriteStringAt(":", 1, 2);
		if (reWriteTimer || !(i % SECONDS_PER_MINUTE))
			DisplayNumber(minutes, 1, 3, 0, 2);
		DisplayNumber(seconds, 1, 6, 0, 2);
		WriteStringAt(":", 1, 5);
		if (reWriteTimer)
			reWriteTimer = false;
	}
	i++;
}
int temperatureRunId = -1;
int timerRunId = -1;
void modeStartStop(enum DisplayMode displayMode, bool startNotStop)
{
	if (displayLocked)
		return;
	if (displayMode == NoModeSet)
		return;
	Clear_Display();
	switch (displayMode)
	{
	case SystemVitals:
	{
		if (startNotStop)
		{
			reWriteTimer = true;
			displayTimer = true;
			temperatureRunId = runInterval(readThermocouples, 50);
			subscribeKeyPress(keyPressSwitchModeCallback);
		}
		else
		{
			if (temperatureRunId >= 0)
				clearRun(temperatureRunId);
			temperatureRunId = -1;
			timerRunId = -1;
			displayTimer = false;
			unsubscribeKeyPress(keyPressSwitchModeCallback);
		}
		break;
	}
	case SetPowerLevel:
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
		break;
	}
	case SetDesiredTemperature:
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
		break;
	}
	case SetFrequency:
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
		break;
	}
	case SetSettings:
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
		break;
	}
	case SetPID:
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
		break;
	}
	case DisplayMessage:
	{
		if (startNotStop)
		{
			displayLocked = true;
		}
		break;
	}
	default:
		break;
	}
}

void setDisplayMode(enum DisplayMode mode)
{
	if (displayLocked)
		return;
	if (displayMode == mode)
		return;
	modeStartStop(displayMode, false);
	modeStartStop(mode, true);
	displayMode = mode;
}

void displayTitleCallback(void *aux)
{
	WriteStringAt("Induction HeaterController", 0, 0);
}
void startTimer()
{
	timerRunId = runInterval(timerCallback, 1000);
}
void LCDstartUI()
{
	showMessageForTime(displayTitleCallback, 2000, SystemVitals, NULL);
}
