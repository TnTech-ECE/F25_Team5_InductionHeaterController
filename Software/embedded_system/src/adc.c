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

#define ADC_BUF_LEN 256
#define RMS_HISTORY_LEN 30
volatile uint8_t dataReady = 0;
uint16_t adcBuffer[ADC_BUF_LEN];
volatile float rmsCurrent = 0;
volatile float avgCurrent = 0;
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
#define VOLTAGE_TO_CURRENT 0.02777777777 // ((5.0f / 3.0f) / 60.0f)
float getRMSCurrent()
{
	const int offset = 1845;
	float sumSquares = 0.0f;
	uint8_t ready;

	// __disable_irq();
	// ready = dataReady;
	// dataReady = 0;
	// __enable_irq();

	// if (!ready)
	// 	return rmsCurrent;

	unsigned n = ADC_BUF_LEN / 2;
	const uint16_t *p = (ready == 2) ? &adcBuffer[ADC_BUF_LEN / 2] : &adcBuffer[0];
	for (unsigned i = 0; i < n; i++)
	{
		int32_t x = (int32_t)p[i] - offset;
		sumSquares += (float)(x * x);
	}

	float rms = sqrtf(sumSquares / (float)n);

	rmsCurrent = (float)(rms)*VOLTAGE_TO_CURRENT;
	return rmsCurrent;
}
float getAVGCurrent()
{
	const int offset = 1845;
	float sum = 0.0f;
	uint8_t ready;

	// __disable_irq();
	// ready = dataReady;
	// dataReady = 0;
	// __enable_irq();

	// if (!ready)
	// 	return avgCurrent;

	unsigned n = ADC_BUF_LEN / 2;
	const uint16_t *p = (ready == 2) ? &adcBuffer[ADC_BUF_LEN / 2] : &adcBuffer[0];
	for (unsigned i = 0; i < n; i++)
	{
		int32_t x = (int32_t)p[i] - offset;
		sum += (float)(x);
	}

	float avg = sum / (float)n;

	avgCurrent = (float)(avg)*VOLTAGE_TO_CURRENT;
	return avgCurrent;
}
void startADC()
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcBuffer, ADC_BUF_LEN);
}
