/*
 * lcd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef LCD_H_
#define LCD_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>
struct LCDCache
{
	char string[33]; // 32 characters + null terminator, 16x2 LCD can hold 32 chars in total (16 chars per line * 2 lines)
	uint8_t line;
	uint8_t position;
};
extern struct LCDCache cacheLCD;
void LCD_TIM_2_Callback();
uint8_t LCD_WriteData(uint8_t data);
uint8_t LCD_WriteCommand(uint8_t data, uint8_t delay);

uint8_t Write_String_LCD(char *str);
void lcd_init(void);
void Shift_Cursor_LCD(uint8_t value);
void Clear_Display();

char *Get_String_LCD(int8_t from, int8_t to);
void Set_LCD(char *string);
uint8_t CursorPositionToCode(uint8_t line, uint8_t position);
void Set_CursorPosition(uint8_t line, uint8_t position);
void Write_String_Sector_LCD(uint8_t sector, char *string);
void WriteStringAt(char *string, uint8_t line, uint8_t position);
void DisplayNumber(long num, int8_t line, int8_t position, uint8_t from, uint8_t minDigits);
void DisplayDecimal(double num, int8_t line, int8_t position, uint8_t from, uint8_t digits);

#endif /* LCD_H_ */
