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
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
max6675_tc *thermoSPI2;
max6675_tc *thermoSPI3;
struct ControllerData controllerData;
int tick = 1;
void threeTenthSeconds(void)
{
	float tempSPI2 = tc_readTemp(thermoSPI2);
	float tempSPI3 = tc_readTemp(thermoSPI3);
	DisplayNumber((int)tempSPI2, 0, 0, 0, 3);
	DisplayNumber((int)tempSPI3, 1, 0, 0, 3);
	DisplayDecimal(controllerData.desiredTemperature, 0, 12, 0, 4);
}

void run()
{
	char clearSector[17];
	int i = 0;
	for (; i < 16; i++)
	{
		clearSector[i] = ' ';
	}
	clearSector[i] = '\0';
	// Initial wait for I2C and LCD to be ready
	HAL_Delay(100);
	//
	//	// Initialize LCD once
	lcd_init();
	HAL_Delay(20); // Wait after init
	thermoSPI2 = tc_init(&hspi2, spi_cn2_GPIO_Port, spi_cn2_Pin);
	thermoSPI3 = tc_init(&hspi3, spi_cn3_GPIO_Port, spi_cn3_Pin);

	bool success = loadControllerDataSD(CONTROLLER_DATA_PATH, &controllerData);
	if (!success)
		controllerData.desiredTemperature = 48.8889f;
	//
	//	// Clear display and home cursor
	//	HAL_Delay(2);		// Clear needs > 1.5ms
	// LCD_WriteCommand(0xF, 1);
	Write_String_Sector_LCD(0, "Tempature");
	Write_String_Sector_LCD(4, "Sensors");
	HAL_Delay(2000);
	Clear_Display();
	runInterval(threeTenthSeconds, 300);
	i = 0;
	while (1)
	{
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
