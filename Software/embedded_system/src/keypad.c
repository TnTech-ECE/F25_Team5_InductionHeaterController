
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "main.h"
#include "keypad.h"
#include "delay.h"
void setColPin(int8_t rowNumber, GPIO_PinState PinState);

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
		return HAL_GPIO_ReadPin(COL0_GPIO_Port, COL0_Pin);
	case 1:
		return HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin);

	case 2:
		return HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin);

	case 3:
		return HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin);
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
initKeypad()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		pressedRows[i] = Unpressed;
		for (uint8_t j = 0; j < 4; j++)
		{
			pressedMatrix[i][j] = Unpressed;
		}
	}
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
			runTimeout()
		}
	}
}
void rowEvent(uint8_t rowNumber)
{
}