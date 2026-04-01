

#include "lcd_ui.h"
#include "./uis/set_pid.h"
#include "./uis/set_number.h"
#include "./uis/set_settings.h"
#include "./uis/system_vitals.h"
enum DisplayMode displayMode = NoModeSet;

bool displayLocked = false;
unsigned cursor = 0;

int setOffset = 7;
char *setUnit = "";
char *setMessage = "";

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

void showErrorMessageCallback(void *aux)
{
	char *message = (char *)aux;
	WriteStringAt(message, 0, 0);
}

void showErrorMessage(char *message)
{
	showMessageForTime(showErrorMessageCallback, 2000, SystemVitals, message);
}

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
		modeStartStopSystemVitals(startNotStop);
		break;
	}
	case SetPowerLevel:
	{
		modeStartStopPowerLevel(startNotStop);
		break;
	}
	case SetDesiredTemperature:
	{
		modeStartStopDesiredTemperature(startNotStop);
		break;
	}
	case SetFrequency:
	{
		modeStartStopFrequency(startNotStop);
		break;
	}
	case SetSettings:
	{
		modeStartStopSettings(startNotStop);
		break;
	}
	case SetPID:
	{
		modeStartStopPID(startNotStop);
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
	WriteStringAt("Induction Heater", 0, 0);
	WriteStringAt("Controller", 1, 0);
}
void startTimer()
{
	startTimerDef();
}
void LCDstartUI()
{
	showMessageForTime(displayTitleCallback, 2000, SystemVitals, NULL);
}
