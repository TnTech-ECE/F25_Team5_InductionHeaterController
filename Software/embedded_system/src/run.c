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
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
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
	max6675_tc *thermoSPI2 = tc_init(&hspi2, spi_cn2_GPIO_Port, spi_cn2_Pin);
	max6675_tc *thermoSPI3 = tc_init(&hspi3, spi_cn3_GPIO_Port, spi_cn3_Pin);

	//
	//	// Clear display and home cursor
	//	HAL_Delay(2);		// Clear needs > 1.5ms
	int tick = 1;
	// LCD_WriteCommand(0xF, 1);
	Write_String_Sector_LCD(0, "Tempature");
	Write_String_Sector_LCD(4, "Sensors");
	HAL_Delay(2000);
	Clear_Display();
	while (1)
	{
		float tempSPI2 = tc_readTemp(thermoSPI2);
		float tempSPI3 = tc_readTemp(thermoSPI3);
		DisplayNumber((int)tempSPI2, 0, 0, 0, 3);
		DisplayNumber((int)tempSPI3, 1, 0, 0, 3);
		DisplayNumber(tick++, 0, 15, 1, 12);
		HAL_Delay(300);
		// if (charCurrent > charStartEnd)
		// {
		// 	charCurrent = charStart;
		// }
		// if (cacheLCD.string[31] == charCurrent)
		// 	charCurrent++;
		// LCD_WriteData(charCurrent);
		// HAL_Delay(1);
	}
}
