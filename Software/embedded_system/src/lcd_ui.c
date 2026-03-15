
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "lcd.h"
#include "lcd_ui.h"
#include "delay.h"
#include "run.h"
#include "keypad.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "max31856.h"
enum DisplayMode lastDisplayMode = NoModeSet;
enum DisplayMode displayMode;

bool displayLocked = false;
unsigned cursor = 0;
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
void checkCursorSet() // for writing a 4 digit float 1 of those being a decimal place
{
	cursor++;
	if (cursor == 3)
	{
		cursor++;
		Set_CursorPosition(1, cursor);
	}
	else if (cursor >= 5)
	{
		cursor = 0;
		Set_CursorPosition(1, cursor);
	}
}
void showSetPowerLevelMessageCallback(void *aux)
{
	float *value = (float *)aux;
	WriteStringAt("Set Power Level:", 1, 0);
	DisplayDecimal(*value, 1, 0, 0, 5);
}
void keyPressSetPowerLevel(char key)
{
	if (key >= '0' || key <= '9')
	{
		if (!cursor)
		{
			if (key >= '1')
			{
				WriteStringAt("100", 1, 0);
				WriteStringAt("0", 1, 4);
			}
		}
		else if (cacheLCD.string[16] == '1')
		{
			return;
		}
		LCD_WriteData(key);
		checkCursorSet();
	}
	else if (key == '#')
	{
		char numberString[5];
		memcpy(numberString, cacheLCD.string + 16, sizeof(numberString));
		char *endptr;
		errno = 0; // Reset for error checking
		float value = strtof(numberString, &endptr);

		if (numberString == endptr)
		{
			return;
		}
		else if (errno == ERANGE)
		{
			return;
		}

		controllerData.powerLevel = value;
		showMessageForTime(showSetPowerLevelMessageCallback, 2000, SystemVitals, &controllerData.powerLevel);
	}
}
void setupPowerLevel()
{
	WriteStringAt("Enter %% Power:", 0, 0);
	DisplayDecimal(controllerData.powerLevel, 1, 0, 0, 5);
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
	int minutes = i / SECONDS_PER_MINUTE;
	int hours = i / SECONDS_PER_HOUR;
	if (displayTimer)
	{
		if (reWriteTimer || !i)
			WriteStringAt(":", 1, 2);
		if (reWriteTimer || !(i % SECONDS_PER_MINUTE))
			DisplayNumber(minutes, 1, 3, 0, 2);
		if (reWriteTimer || !(i % SECONDS_PER_HOUR))
			DisplayNumber(hours, 1, 0, 0, 2);
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
			timerRunId = runInterval(timerCallback, 1000);
		}
		else
		{

			if (temperatureRunId >= 0)
				clearRun(temperatureRunId);
			if (timerRunId)
				clearRun(timerRunId);
			temperatureRunId = -1;
			timerRunId = -1;
			displayTimer = false;
		}
		break;
	}
	case SetPowerMode:
	{
		if (startNotStop)
		{
			setupPowerLevel();
			subscribeKeyPress(keyPressSetPowerLevel);
		}
		else
		{
			unsubscribeKeyPress(keyPressSetPowerLevel);
		}
	}
	case DisplayMessage:
	{
		if (startNotStop)
		{
			displayLocked = true;
		}
	}
	default:
		break;
	}
}

void setDisplayMode(enum DisplayMode mode)
{
	if (displayLocked)
		return;
	modeStartStop(lastDisplayMode, false);
	modeStartStop(mode, true);
	lastDisplayMode = mode;
}

void displayTitleCallback(void *aux)
{
	WriteStringAt("Induction Heater Controller", 0, 0);
}
void LCDstartUI()
{
	showMessageForTime(displayTitleCallback, 2000, SystemVitals, NULL);
}