
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "main.h"
#include "keypad.h"
#include "delay.h"
#include "lcd.h"
void setColPin(int8_t rowNumber, GPIO_PinState PinState);
uint16_t pinsRow[4] = {ROW0_Pin, ROW1_Pin, ROW2_Pin, ROW3_Pin};
unsigned long portsRow[4] = {ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port};
enum RowMode
{
	Interrupt = 0,
	Input = 1
};
void setRowMode(enum RowMode rowMode)
{
	GPIO_InitTypeDef GPIO_InitStructRows = {0};
	switch (rowMode)
	{
	case Input:
	{
		GPIO_InitStructRows.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructRows.Pull = GPIO_NOPULL;
		GPIO_InitStructRows.Speed = GPIO_SPEED_FREQ_LOW;
		for (int i = 0; i < 4; i++)
		{
			GPIO_InitStructRows.Pin = pinsRow[i];
			HAL_GPIO_Init(portsRow[i], &GPIO_InitStructRows);
		}
		break;
	}
	case Interrupt:
	{
		GPIO_InitStructRows.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStructRows.Pull = GPIO_PULLDOWN;
		for (int i = 0; i < 4; i++)
		{
			GPIO_InitStructRows.Pin = pinsRow[i];
			HAL_GPIO_Init(portsRow[i], &GPIO_InitStructRows);
		}
		break;
	}
	}
}
void setColPin(int8_t rowNumber, GPIO_PinState PinState)
{
	if (rowNumber == -1 || rowNumber == 0)
		HAL_GPIO_WritePin(COL0_GPIO_Port, COL0_Pin, PinState);
	if (rowNumber == -1 || rowNumber == 1)
		HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, PinState);
	if (rowNumber == -1 || rowNumber == 2)
		HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, PinState);
	if (rowNumber == -1 || rowNumber == 3)
		HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, PinState);
}
GPIO_PinState readRowPin(int8_t colNumber)
{
	switch (colNumber)
	{
	case 0:
		return HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW0_Pin);
	case 1:
		return HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin);

	case 2:
		return HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin);

	case 3:
		return HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin);
	}
}
enum ButtonState
{
	Unpressed = 0,
	Pressed = 1,
	Depressed = 2
};
enum ButtonState pressedRows[4];

enum ButtonState pressedMatrix[4][4];
static char keyMatrix[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};
void initKeypad()
{
	setColPin(-1, SET);
	setRowMode(Interrupt);
	for (uint8_t i = 0; i < 4; i++)
	{
		pressedRows[i] = Unpressed;
		for (uint8_t j = 0; j < 4; j++)
		{
			pressedMatrix[i][j] = Unpressed;
		}
	}
}
void scanAllToLCD()
{
	setRowMode(Input);
	//Set_CursorPosition(0, 0);
	for (uint8_t row = 0; row < 4; row++)
	{
		setColPin(-1, RESET);
		setColPin(row, SET);
		for (uint8_t col = 0; col < 4; col++)
		{
			GPIO_PinState pinState = readRowPin(col);
			// Set_CursorPosition(0, row *4 + col % 4);

			if (pinState)
			{
				LCD_WriteData(keyMatrix[row][col]);
			}
		}
	}
	setColPin(-1, SET);
	setRowMode(Interrupt);
}
uint8_t scanCols(uint8_t row)
{
	setColPin(-1, RESET);
	setColPin(row, SET);
	for (uint8_t col = 0; col < 4; col++)
	{
		GPIO_PinState pinState = readRowPin(col);
		enum ButtonState buttonState = pressedMatrix[row][col];
		if (buttonState == Unpressed)
		{
			pressedMatrix[row][col] = Pressed;
		}
	}
}
void rowEvent(uint8_t rowNumber)
{
	scanAllToLCD();
}
