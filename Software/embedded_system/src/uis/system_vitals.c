#include "../lcd_ui.h"

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
	controllerData.t1 = tempSPI2;
	controllerData.t2 = tempSPI3;
	// tempSPI2 /= 128.0f;
	if (displayMode != SystemVitals)
		return;
	DisplayDecimal(tempSPI2, 0, 0, 0, 5);
	DisplayDecimal(tempSPI3, 0, 6, 0, 5);
	DisplayDecimalWithPlaces(getRMSCurrent(), 0, 12, 0, 4, 1);
	DisplayDecimalWithPlaces(getAVGCurrent(), 1, 12, 0, 4, 1);
	// DisplayDecimalWithPlaces(controllerData.phaseDegrees, 1, 11, 0, 5, 1);
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
int temperatureRunId = -1;
int timerRunId = -1;
void modeStartStopSystemVitals(bool startNotStop)
{
	if (startNotStop)
	{
		reWriteTimer = true;
		displayTimer = true;

		subscribeKeyPress(keyPressSwitchModeCallback);
	}
	else
	{
		// if (temperatureRunId >= 0)
		// 	clearRun(temperatureRunId);
		// temperatureRunId = -1;
		// timerRunId = -1;
		displayTimer = false;
		unsubscribeKeyPress(keyPressSwitchModeCallback);
	}
}
void startTimerDef()
{
	temperatureRunId = runInterval(readThermocouples, 100);
	timerRunId = runInterval(timerCallback, 1000);
}