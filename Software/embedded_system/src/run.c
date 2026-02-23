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
volatile bool need_save = false;
volatile bool need_log = false;
void save()
{
	need_save = true;
}
void log()
{
	need_log = true;
}
unsigned value_adc;
max31856_t thermoSPI2 = {&hspi2, {spi_cn2_GPIO_Port, spi_cn2_Pin}};
// max6675_tc *thermoSPI3;
struct ControllerData controllerData;
int tick = 1;

void threeTenthSeconds(void)
{
	float tempSPI2 = max31856_read_TC_temp(&thermoSPI2);
	if (thermoSPI2.sr.val)
	{
		return;
	}
	// tempSPI2 /= 128.0f;
	DisplayDecimal(tempSPI2, 0, 0, 0, 6);
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
int kjdwkjdwjdwkj = 0;
bool testUntilCallback(void *aux)
{

	DisplayNumber(kjdwkjdwjdwkj, 1, 0, 0, 3);
	kjdwkjdwjdwkj++;
	// if (kjdwkjdwjdwkj > 500)
	// {
	// 	return true;
	// }
	return false;
}
void keyStateChangeCallback(char key, enum KeyState keyState)
{
	Set_CursorPosition(0, charToKeyNumber(key));
	if (keyState == KeyPressed)
	{
		LCD_WriteData(key);
	}
	else
	{
		LCD_WriteData(' ');
	}
}
void startTim8()
{
	TIM8_UpdateStart(10000, 15, 1, true);
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
	// sd_send_initial_dummy_clocks();
	setupTempAmp(&thermoSPI2);
	max31856_setHighFaultTemp(&thermoSPI2, 25);
	// thermoSPI3 = tc_init(&hspi3, spi_cn3_GPIO_Port, spi_cn3_Pin);
	//	int fr = sd_mount();
	//	printf("sd_mount -> %d\r\n", fr);
	//	if (fr != FR_OK)
	//	{
	//		printf("Mount failed\n");
	//		return;
	//	}
	subscribeKeyStateChange(keyStateChangeCallback);
	bool success = false; // loadControllerDataSD(CONTROLLER_DATA_PATH, &controllerData);
	if (!success)

		controllerData.desiredTemperature = 48.8f;

	//
	//	// Clear display and home cursor
	//	HAL_Delay(2);		// Clear needs > 1.5ms
	// LCD_WriteCommand(0xF, 1);

	Write_String_Sector_LCD(0, "Tempature");
	Write_String_Sector_LCD(4, "Sensors");
	HAL_Delay(2000);
	Clear_Display();
	// WriteStringAt(success ? "true " : "false", 0, 6);

	// test();
	runInterval(threeTenthSeconds, 50);
	runIntervalUntil(testUntilCallback, NULL, 1000);
	runTimeout(startTim8, 5000);
	// runTimeout(unsubscribeTest, 10000);
	while (1)
	{
		//		scanAllToLCD();
		// if (need_save)
		// {

		// 	bool result = saveControllerDataSD(CONTROLLER_DATA_PATH, &controllerData);
		// 	// WriteStringAt(result ? "true " : "false", 0, 6);
		// 	need_save = false;
		// }
		// if (need_log)
		// {
		// 	bool result = appendControllerDataSD(CONTROLLER_LOG_PATH, &controllerData);
		// 	// WriteStringAt(result ? "true " : "false", 0, 6);
		// 	need_log = false;
		// }
		// if (!i)
		// 	Clear_Display();
		//		threeTenthSeconds();
		//		i = (i + 1) % 10;
		//		HAL_Delay(300);

		// if (charCurrent > charStartEnd)
		// {
		// 	charCurrent = charStart;
		// }
		// if (cacheLCD.string[31] == charCurrent)
		// 	charCurrent++;
		// LCD_WriteData(charCurrent);
	}
}
