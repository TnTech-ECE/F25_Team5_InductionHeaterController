#include "main.h"
#include "adc.h"
#include <stdint.h>
#include "delay.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal_adc.h"
#include <math.h>

#define ADC_BUF_LEN 4096
#define RMS_HISTORY_LEN 30
volatile uint8_t dataReady = 0;
uint16_t adcBuffer[ADC_BUF_LEN];
volatile float maxCurrent = 0;
static float rmsHistory[RMS_HISTORY_LEN];
static unsigned rmsHistoryIndex = 0;
static unsigned rmsHistoryCount = 0;
// Start once at init

// Called by HAL when DMA reaches middle
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{
		dataReady = 1;
	}
}

// Called by HAL when DMA reaches end (then wraps to start)
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc->Instance == ADC1)
	{

		dataReady = 2;
	}
}

float getRMSCurrent()
{
	const int offset = 0;
	float sumSquares = 0.0f;
	uint8_t ready;

	__disable_irq();
	ready = dataReady;
	dataReady = 0;
	__enable_irq();

	if (!ready)
		return maxCurrent;

	unsigned n = ADC_BUF_LEN / 2;
	const uint16_t *p = (ready == 2) ? &adcBuffer[ADC_BUF_LEN / 2] : &adcBuffer[0];
	for (unsigned i = 0; i < n; i++)
	{
		int32_t x = (int32_t)p[i] - offset;
		sumSquares += (float)(x * x);
	}

	float rms = sqrtf(sumSquares / (float)n);

	rmsHistory[rmsHistoryIndex] = rms;
	rmsHistoryIndex = (rmsHistoryIndex + 1U) % RMS_HISTORY_LEN;
	if (rmsHistoryCount < RMS_HISTORY_LEN)
	{
		rmsHistoryCount++;
	}

	float rollingRms = 0.0f;
	for (unsigned i = 0; i < rmsHistoryCount; i++)
	{
		rollingRms += rmsHistory[i];
	}
	rollingRms /= (float)rmsHistoryCount;

	maxCurrent = (float)(rollingRms);
	return maxCurrent;
}

void startADC()
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcBuffer, ADC_BUF_LEN);
}
