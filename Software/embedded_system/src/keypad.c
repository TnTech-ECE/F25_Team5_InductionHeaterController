
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
GPIO_TypeDef *portsRow[4] = {ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port};
enum RowMode
{
	InterruptFalling = 0,
	InterruptRising = 1,
	Input = 2
};

void setRowMode(int8_t row, enum RowMode rowMode)
{
	GPIO_InitTypeDef GPIO_InitStructRows = {0};
	GPIO_InitStructRows.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructRows.Pull = GPIO_PULLDOWN;
	switch (rowMode)
	{
	case Input:
	{
		GPIO_InitStructRows.Mode = GPIO_MODE_INPUT;
		break;
	}
	case InterruptFalling:
	{
		GPIO_InitStructRows.Mode = GPIO_MODE_IT_FALLING;
		break;
	}
	case InterruptRising:
	{
		GPIO_InitStructRows.Mode = GPIO_MODE_IT_RISING;
		break;
	}
	}
	if (row == -1)
		for (int i = 0; i < 4; i++)
		{
			GPIO_InitStructRows.Pin |= pinsRow[i];
			HAL_GPIO_Init(portsRow[i], &GPIO_InitStructRows);
		}
	else if (row >= 0 && row <= 3)
	{
		GPIO_InitStructRows.Pin = pinsRow[row];
		HAL_GPIO_Init(portsRow[row], &GPIO_InitStructRows);
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
GPIO_PinState readRowPin(int8_t rowNumber)
{
	switch (rowNumber)
	{
	case 0:
		return HAL_GPIO_ReadPin(ROW0_GPIO_Port, ROW0_Pin);
	case 1:
		return HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin);

	case 2:
		return HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin);

	case 3:
		return HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin);

	default:
		return GPIO_PIN_RESET;
	}
}
enum ButtonState
{
	Unpressed = 0,
	Pressing = 1,
	Pressed = 2,
	Depressing = 3
};

enum ButtonState pressedMatrix[4][4];

struct RowState
{
	int8_t row;
	int8_t col;
	int8_t ticks;
	enum ButtonState state;
};

struct RowState rowsStates[4];

static char keyMatrix[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
};

int8_t charToKeyNumber(char key)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			if (keyMatrix[i][j] == key)
			{
				return i * 4 + j;
			}
		}
	}
	return -1;
}

void initKeypad()
{

	setColPin(-1, SET);
	setRowMode(-1, InterruptRising);
	for (uint8_t i = 0; i < 4; i++)
	{
		rowsStates[i].row = i;
		rowsStates[i].col = -1;
		rowsStates[i].state = Unpressed;
		// for (uint8_t j = 0; j < 4; j++)
		// {
		// 	pressedMatrix[i][j] = Unpressed;
		// }
	}
}

void keyPressCallback(char key)
{
	Set_CursorPosition(0, charToKeyNumber(key));
	LCD_WriteData(key);
}
void unkeyPressCallback(char key)
{
	Set_CursorPosition(0, charToKeyNumber(key));
	LCD_WriteData(' ');
}
bool checkCallback(void *aux)
{
	struct RowState *rowState = (struct RowState *)aux;
	if (rowState == NULL)
		return true;
	setRowMode(rowState->row, Input);

	if (rowState->col == -1)
	{
		for (uint8_t col = 0; col < 4; col++)
		{
			setColPin(-1, RESET);
			setColPin(col, SET);
			GPIO_PinState pinState = readRowPin(rowState->row);
			if (pinState)
			{
				rowState->col = col;
				break;
			}
		}
		setColPin(-1, SET);
	}
	setColPin(-1, RESET);
	setColPin(rowState->col, SET);
	GPIO_PinState pinState = readRowPin(rowState->row);
	setColPin(-1, SET);
	if (rowState->state == Pressing && pinState == GPIO_PIN_SET)
	{
		if (++rowState->ticks > 20)
		{
			rowState->state = Pressed;
			keyPressCallback(keyMatrix[rowState->row][rowState->col]);
		}
		return false;
	}
	else
		rowState->ticks = 0;
	if (rowState->state && pinState == GPIO_PIN_RESET)
	{
		unkeyPressCallback(keyMatrix[rowState->row][rowState->col]);
		rowState->col = -1;
		rowState->state = Unpressed;
		setRowMode(rowState->row, InterruptRising);
		return true;
	}
	return false;
}
uint8_t scanCols(uint8_t row)
{
	struct RowState *rowState = &rowsStates[row];
	if (rowState->state == Unpressed)
	{
		rowState->state = Pressing;
		runIntervalUntilAux(checkCallback, NULL, 1, rowState);
	}
}
enum KeyEventType
{
	KeyPress = 0,
	KeyRelease = 1,
	KeyStateChange = 2
};

typedef void *KeyPressCallback(char key);
typedef void *KeyReleaseCallback(char key);
typedef void *KeyStateChangeCallback(char key, enum KeyState keyState);

struct KeyEvent
{
	union
	{
		KeyPressCallback press;
		KeyReleaseCallback release;
		KeyStateChangeCallback stateChange;
	} callback;
	enum KeyEventType keyEventType;
};
struct KeyEvent keySubscriptions[20];
keySubscriptionsLength = 0;
keySubscriptionsLength = 0;

void rowEvent(uint8_t rowNumber)
{
	printf("%d\n", rowNumber);
	scanCols(rowNumber);
}

void subscribeKeyPress(KeyPressCallback callback)
{
}
void subscribeKeyRelease(KeyReleaseCallback callback)
{
}
void subscribeKeyStateChange(KeyStateChangeCallback callback)
{
}

void unsubscribeKeyPress(KeyPressCallback callback)
{
}
void unsubscribeKeyRelease(KeyReleaseCallback callback)
{
}
void unsubscribeKeyStateChange(KeyStateChangeCallback callback)
{
}

void runKeyEvent(char key, enum KeyState keyState)
{
	for (int i = 0; i < keySubscriptionsLength; i++)
	{
		struct KeyEvent *keyEvent = &keySubscriptions[i];
		switch (keyEvent->keyEventType)
		{
		case KeyPress:
		{
			if (keyState == KeyPress)
			{
				if (keyEvent->callback.press != NULL)
					keyEvent->callback.press(key);
			}
			break;
		}
		case KeyRelease:
		{
			if (keyState == KeyRelease)
			{
				if (keyEvent->callback.release != NULL)
					keyEvent->callback.release(key);
			}
		}
		case KeyStateChange:
		{
			if (keyEvent->callback.stateChange != NULL)
				keyEvent->callback.stateChange(key, keyState);
		}
		}
	}
}