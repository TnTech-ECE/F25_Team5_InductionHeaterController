#include "delay.h"
#include "stdbool.h"
#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "main.h"
volatile uint32_t flowCount = 0;
volatile uint32_t lastFlowCount = 0;
float getFlowRateGPS()
{
	return (float)(lastFlowCount) / 28.39;
}
void flowSensorOnTimeCallback()
{
	lastFlowCount = flowCount;
	flowCount = 0;
	printf("flowCount: %d\n\r", lastFlowCount);
}
static bool doneInit = false;
void initFlowSensor()
{
	if (doneInit)
		return;
	doneInit = true;
	runInterval(flowSensorOnTimeCallback, 1000);
}

void onFlowPulse()
{
	flowCount++;
}
