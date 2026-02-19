#include "main.h"
#include "stdlib.h"
#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "lcd.h"
void TIM_2_Callback() {

}
void LCD_WriteNibble(uint8_t nibble)
{
	// Set D4-D7 according to the nibble value
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (nibble & 0x01));
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (nibble & 0x02) >> 1);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (nibble & 0x04) >> 2);
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (nibble & 0x08) >> 3);
	// Toggle EN pin to latch the nibble
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
	// HAL_Delay(1); // Small delay to ensure the LCD latches the data
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
	HAL_Delay(1); // Small delay to ensure the LCD processes the nibble
}
 void LCD_WriteCommand(uint8_t command)
{
	// Set RS to 0 for command mode
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
	// Send the higher nibble
	LCD_WriteNibble(command >> 4);
	// Send the lower nibble
	LCD_WriteNibble(command & 0x0F);
	HAL_Delay(1); // Delay for command execution
}
 void LCD_WriteData(uint8_t data)
{
	// Set RS to 1 for data mode
	HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
	// Send the higher nibble
	LCD_WriteNibble(data >> 4);
	// Send the lower nibble
	LCD_WriteNibble(data & 0x0F);
	HAL_Delay(1); // Delay for data processing
}
void setCursor(int a, int b)
{
	int i = 0;
	switch (b)
	{
	case 0:
		LCD_WriteCommand(0x80);
		break;
	case 1:
		LCD_WriteCommand(0xC0);
		break;
	case 2:
		LCD_WriteCommand(0x94);
		break;
	case 3:
		LCD_WriteCommand(0xd4);
		break;
	}
	for (i = 0; i < a; i++)
		LCD_WriteCommand(0x14);
}
void lcd_send_string(char *str)
{
	while (*str)
		LCD_WriteData(*str++);
}
void lcd_init(void)
{
	// 4 bit initialisation
	HAL_Delay(50); // wait for >40ms
	LCD_WriteCommand(0x3);
	HAL_Delay(5); // wait for >4.1ms
	LCD_WriteCommand(0x3);
	HAL_Delay(1); // wait for >100us
	LCD_WriteCommand(0x3);
	HAL_Delay(10);
	LCD_WriteCommand(0x2); // 4bit mode
	HAL_Delay(10);
	// dislay initialisation
	LCD_WriteCommand(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0(5x8 characters)
	HAL_Delay(1);
	LCD_WriteCommand(0x08); // Display on/off control --> D=0,C=0, B=0 ---> display off

	HAL_Delay(1);
	LCD_WriteCommand(0x01); // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_WriteCommand(0x06); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_WriteCommand(0x0C); // Display on/off control --> D = 1, C and B =0.(Cursor and blink, last two bits)
}
#define LCD_CLEARDISPLAY 0x01
void lcd_clear(void)
{
	LCD_WriteCommand(LCD_CLEARDISPLAY);
	HAL_Delay(100);
}
void create_custom_char(uint8_t loc, char *data)
{
	switch (loc)
	{
	case 0:
		LCD_WriteCommand(0x40);
		break;
	case 1:
		LCD_WriteCommand(0x40 + 8);
		break;
	case 2:
		LCD_WriteCommand(0x40 + 16);
		break;
	case 3:
		LCD_WriteCommand(0x40 + 24);
		break;
	case 4:
		LCD_WriteCommand(0x40 + 32);
		break;
	case 5:
		LCD_WriteCommand(0x40 + 40);
		break;
	case 6:
		LCD_WriteCommand(0x40 + 48);
		break;
	case 7:
		LCD_WriteCommand(0x40 + 56);
		break;
	default:
		break;
	}
	uint8_t cc_data[8];
	for (int i = 0; i < 8; i++)
	{
		cc_data[i] = *data++;
	}
	for (int i = 0; i < 8; i++)
	{
		LCD_WriteData(cc_data[i]);
	}
}

void display_custom_char(uint8_t loc)
{
	switch (loc)
	{
	case 0:
		LCD_WriteData(0);
		break;
	case 1:
		LCD_WriteData(1);
		break;
	case 2:
		LCD_WriteData(2);
		break;
	case 3:
		LCD_WriteData(3);
		break;
	case 4:
		LCD_WriteData(4);
		break;
	case 5:
		LCD_WriteData(5);
		break;
	case 6:
		LCD_WriteData(6);
		break;
	case 7:
		LCD_WriteData(7);
		break;
	default:
		break;
	}
}
