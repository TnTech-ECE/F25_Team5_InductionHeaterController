/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "lcd.h"
#include "max6675.h"
#include "main.h"
#include "delay.h"
#include "save.h"
#include "run.h"
#include "flow_sensor.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "sd_functions.h"
#include "pwm.h"
#include "keypad.h"
#include "max31856.h"
#include "lcd_ui.h"
#include "ff.h"
#include "adc.h"
volatile bool need_save = false;
volatile bool need_log = false;
void saveSD()
{
	need_save = true;
}
void logSD()
{
	need_log = true;
}
unsigned value_adc;
max31856_t thermoSPI2 = {&hspi2, {spi_cn2_GPIO_Port, spi_cn2_Pin}};
max31856_t thermoSPI3 = {&hspi3, {spi_cn3_GPIO_Port, spi_cn3_Pin}};
// max6675_tc *thermoSPI3;
struct ControllerData controllerData;
int tick = 1;

void keyStateChangeCallback(char key, enum KeyState keyState)
{
	Set_CursorPosition(1, 6);
	if (keyState == KeyPressed)
	{
		LCD_WriteData(key);
	}
	else
	{
		LCD_WriteData(' ');
	}
}

void unsubscribeTest()
{
	unsubscribeKeyStateChange(keyStateChangeCallback);
}
void setupTempAmp(max31856_t *amp)
{
	max31856_init(amp);
	max31856_set_noise_filter(amp, CR0_FILTER_OUT_60Hz);
	max31856_set_cold_junction_enable(amp, CR0_CJ_ENABLED);
	max31856_set_thermocouple_type(amp, CR1_TC_TYPE_K);
	max31856_set_average_samples(amp, CR1_AVG_TC_SAMPLES_2);
	max31856_set_open_circuit_fault_detection(amp, CR0_OC_DETECT_ENABLED_TC_LESS_2ms);
	max31856_set_conversion_mode(amp, CR0_CONV_CONTINUOUS);
}
void run()
{
	printf("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");
	// Initial wait for I2C and LCD to be ready

	HAL_Delay(100);
	// initFlowSensor();
	// HAL_ADC_Start_DMA(&hadc3, (unsigned *)&value_adc, 1);
	HAL_Delay(20);
	initKeypad();
	//
	//	// Initialize LCD once
	lcd_init();
	HAL_Delay(20); // Wait after init
	startADC();
	// sd_send_initial_dummy_clocks();
	setupTempAmp(&thermoSPI2);
	setupTempAmp(&thermoSPI3);
	max31856_setHighFaultTemp(&thermoSPI2, 98);
	max31856_setHighFaultTemp(&thermoSPI3, 98);
	// thermoSPI3 = tc_init(&hspi3, spi_cn3_GPIO_Port, spi_cn3_Pin);
	//	int fr = sd_mount();
	//	printf("sd_mount -> %d\r\n", fr);
	//	if (fr != FR_OK)
	//	{
	//		printf("Mount failed\n");
	//		return;
	//	}
	// subscribeKeyStateChange(keyStateChangeCallback);
	FRESULT mountStatus = sd_mount();
	HAL_Delay(100);
	if (mountStatus != FR_OK)
	{
		showErrorMessage("mount failed: ram only");
	}
	else
	{
		LCDstartUI();
	}

	startTimer();
	loadControllerDataSD(CONTROLLER_DATA_PATH, &controllerData);
	//
	//	// Clear display and home cursor
	//	HAL_Delay(2);		// Clear needs > 1.5ms
	// LCD_WriteCommand(0xF, 1);

	// runTimeout(unsubscribeTest, 10000);
	while (1)
	{
		if (need_save)
		{

			bool result = saveControllerDataSD(CONTROLLER_DATA_PATH, &controllerData);
			// WriteStringAt(result ? "true " : "false", 0, 6);
			need_save = false;
		}
		if (need_log)
		{
			bool result = appendControllerDataSD(CONTROLLER_LOG_PATH, &controllerData);
			// WriteStringAt(result ? "true " : "false", 0, 6);
			need_log = false;
		}
	}
}
