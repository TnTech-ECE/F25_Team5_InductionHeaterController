/*
 * lcd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "lcd.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"

struct LCDCache cacheLCD;
int LenStr(char *str)
{
	int i = 0;
	while (*str++)
	{
		i++;
	}
	return i;
}
uint8_t testPoint = 0;
enum CurrentTaskIndex
{
	Upper = 0,
	LatchSetUpper = 1,
	LatchResetUpper = 2,
	Lower = 3,
	LatchSetLower = 4,
	LatchResetLower = 5,
	Wait = 6
};

// uint8_t CommandsQueued[200];
// int commandsQueuedLength = 0;
// uint8_t DataQueued[200];
// int dataQueuedLength = 0;
uint8_t LCD_WriteByteDirect(uint16_t byte, enum CurrentTaskIndex task);
static inline int queue_length(void);
static inline int queue_left(void);
static inline int queue_empty(void);
static inline int queue_full(void);
static inline int queue_full_or_emptying(void);
void CacheChar(uint8_t code);

void Clear_Cache();
void CacheCursor(uint8_t code);
#define BytesQueuedSize 1024
volatile uint16_t BytesQueued[BytesQueuedSize];
volatile int bytesQueuedStart = 0;
volatile int bytesQueuedEnd = 0;

uint8_t QueueHalfEmpting = 0;
uint16_t QueueNextIndex(uint16_t i)
{
	return (i + 1) % BytesQueuedSize;
}
volatile enum CurrentTaskIndex currentTaskIndex = Upper;
volatile int wait = 0;
volatile uint16_t forcedByte = UINT16_MAX;
void LCD_TIM_2_Callback()
{
	if (queue_empty())
	{
		return;
	}
	if (queue_full())
	{
		QueueHalfEmpting = 1;
	}
	else if (queue_length() < (BytesQueuedSize / 2))
	{
		QueueHalfEmpting = 0;
	}
	enum CurrentTaskIndex task = currentTaskIndex;
	uint16_t currentByte = (forcedByte != UINT16_MAX) ? forcedByte : BytesQueued[bytesQueuedStart];
	if (task == Upper)
	{
		wait = (int)((currentByte >> 9) & 0x7F) * getDelayScale();
		if (!wait)
			wait = 1;
	}

	uint8_t shouldReturn = LCD_WriteByteDirect(currentByte, currentTaskIndex);
	if (shouldReturn)
		return;
	if (task == Wait && wait > 0)
	{
		--wait;
		return; // Stay in Wait state
	}
	++currentTaskIndex;
	if (currentTaskIndex > Wait)
	{
		currentTaskIndex = Upper;
		if (forcedByte != UINT16_MAX)
		{
			forcedByte = UINT16_MAX;
			return;
		}

		BytesQueued[bytesQueuedStart] = 0;
		bytesQueuedStart = QueueNextIndex(bytesQueuedStart);
	}
}
uint16_t OveriteCurrentByte(uint8_t byte, uint8_t DataNotCommand, uint8_t delay)
{
	uint16_t forcedByte = byte | (DataNotCommand << 8) | (delay << 9);
	BytesQueued[bytesQueuedStart] = forcedByte;
	return forcedByte;
}
void forceWriteByte(uint8_t byte, uint8_t DataNotCommand, uint8_t delay)
{
	currentTaskIndex = Upper;

	forcedByte = byte | (DataNotCommand << 8) | (delay << 9);
}
void LCD_WriteNibbleDirect(uint8_t nibble, enum CurrentTaskIndex task)
{
	switch (task)
	{
	case Upper:
	case Lower:
	{
		HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (nibble & 0x01));
		HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (nibble & 0x02) >> 1);
		HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (nibble & 0x04) >> 2);
		HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (nibble & 0x08) >> 3);
		break;
	}
	case LatchSetLower:
	case LatchSetUpper:
	{
		HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
		break;
	}
	case LatchResetLower:
	case LatchResetUpper:
	{
		HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
		break;
	}
	default:
	{
		break;
	}
	}
	// Set D4-D7 according to the nibble value
	// Toggle EN pin to latch the nibble
	// HAL_Delay(1); // Small delay to ensure the LCD latches the data
	// HAL_Delay(1); // Small delay to ensure the LCD processes the nibble
}

const uint8_t SHIFT_RIGHT = 0x10 | ((int8_t)(1) << 2);
const uint8_t SHIFT_LEFT = 0x10 | ((int8_t)(0) << 2);
uint8_t LCD_WriteByteDirect(uint16_t byte, enum CurrentTaskIndex task)
{
	uint8_t processedByte = (uint8_t)(byte & 0xFF);

	uint8_t dataNotCommand = !!(byte & 0x100); // 9th bit controlls if command or data
	switch (task)
	{
	case Upper:
	{
		//		if (!dataNotCommand && (processedByte == SHIFT_RIGHT || processedByte == SHIFT_LEFT))
		//		{
		//
		//			if (cacheLCD.position > 250)
		//			{
		//				// overwrites the current code so that the lcd shift command doesn't run.
		//				uint8_t code = CursorPositionToCode(!cacheLCD.line, 15);
		//				processedByte = OveriteCurrentByte(code, 0, 1);
		//			}
		//			else if (cacheLCD.position > 15)
		//			{
		//				// overwrites the current code so that the lcd shift command doesn't run.
		//				uint8_t code = CursorPositionToCode(!cacheLCD.line, 0);
		//				processedByte = OveriteCurrentByte(code, 0, 1);
		//			}
		//		}
		if (dataNotCommand && cacheLCD.position >= 16)
		{
			// the byte is force written here as I want it to shift then rerun the char write.
			testPoint = 1;
			uint8_t code = CursorPositionToCode(cacheLCD.line ? 0 : 1, 0);
			forceWriteByte(code, 0, 1);

			return 1;
		}
		HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin,
						  dataNotCommand ? GPIO_PIN_SET : GPIO_PIN_RESET);

		LCD_WriteNibbleDirect(processedByte >> 4, task);
		break;
	}
	case Lower:
	{

		LCD_WriteNibbleDirect(processedByte & 0x0F, task);
		break;
	}
	case LatchResetLower:
	{
		if (dataNotCommand)
			CacheChar(processedByte);

		if (!dataNotCommand)
		{
			if (processedByte == 0x1)
			{
				Clear_Cache();
			}
			else if (processedByte == SHIFT_RIGHT || processedByte == SHIFT_LEFT)
			{
				cacheLCD.position += processedByte == SHIFT_RIGHT ? 1 : -1;
			}
			else
			{
				CacheCursor(processedByte);
			}
		}
	}
	default:
	{
		LCD_WriteNibbleDirect(0, task);
		break;
	}
	}
	return 0;
}
static inline int queue_length(void)
{
	if (bytesQueuedEnd >= bytesQueuedStart)
		return bytesQueuedEnd - bytesQueuedStart;
	else
		return BytesQueuedSize - bytesQueuedStart + bytesQueuedEnd;
}
static inline int queue_left(void)
{
	return BytesQueuedSize - queue_length();
}
static inline int queue_empty(void)
{
	return bytesQueuedStart == bytesQueuedEnd;
}

static inline int queue_full(void)
{
	return QueueNextIndex(bytesQueuedEnd) == bytesQueuedStart;
}
static inline int queue_full_or_emptying(void)
{
	return queue_full() || QueueHalfEmpting;
}

uint8_t queueByte(uint8_t byte, uint8_t DataNotCommand, uint8_t delay)
{
	// printf("byteQueteLength: %d", queue_length());
	__disable_irq();
	if (queue_full_or_emptying())
	{
		__enable_irq();
		return 0;
	}
	if (!delay)
		delay = 1;
	BytesQueued[bytesQueuedEnd] = byte | (DataNotCommand << 8) | (delay << 9);
	bytesQueuedEnd = QueueNextIndex(bytesQueuedEnd);
	__enable_irq();
	return 1;
}
uint8_t LCD_WriteData(uint8_t data)
{
	__disable_irq();
	return queueByte(data, 1, 0);
	__enable_irq();
}
uint8_t LCD_WriteCommand(uint8_t data, uint8_t delay)
{
	__disable_irq();
	return queueByte(data, 0, delay);
	__enable_irq();
}

uint8_t Write_String_LCD(char *str)
{
	__disable_irq();
	if (queue_full_or_emptying() || LenStr(str) >= queue_left())
	{
		__enable_irq();
		return 0;
	}
	while (*str)
	{
		uint8_t queueAvailable = LCD_WriteData(*str++);
		if (!queueAvailable)
		{
			__enable_irq();
			return queueAvailable;
		}
	}
	__enable_irq();
	return 1;
}
void lcd_init(void)
{
	__disable_irq();
	// 4 bit initialisation
	// HAL_Delay(50); // wait for >40ms
	LCD_WriteCommand(0x3, 5);
	// HAL_Delay(5); // wait for >4.1ms
	LCD_WriteCommand(0x3, 1);
	// HAL_Delay(1); // wait for >100us
	LCD_WriteCommand(0x3, 10);
	// HAL_Delay(10);
	LCD_WriteCommand(0x2, 10); // 4bit mode
							   // HAL_Delay(10);
							   //  dislay initialisation
	LCD_WriteCommand(0x28, 1); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
							   // HAL_Delay(1);
	LCD_WriteCommand(0x08, 1); // Display on/off control --> D=0,C=0, B=0 ---> display off
							   // HAL_Delay(1);
	LCD_WriteCommand(0x01, 1); // clear display
							   // HAL_Delay(1);
							   // HAL_Delay(1);
	LCD_WriteCommand(0x06, 1); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
							   // HAL_Delay(1);
	LCD_WriteCommand(0x0C, 1); // Display on/off control --> D = 1, C and B =0. (Cursor and blink, last two bits)
	__enable_irq();
}

#define min(a, b) ((a) < (b) ? (a) : (b))

void Shift_Cursor_LCD(uint8_t value)
{
	__disable_irq();
	LCD_WriteCommand(0x10 | (value << 2), 1);
	__enable_irq();
}

void CacheChar(uint8_t code)
{
	uint8_t index = 16 * cacheLCD.line + cacheLCD.position;
	if (index < 32)
		cacheLCD.string[index] = code; // cache the string for later use in Set_LCD
	cacheLCD.position++;
}

void Clear_Cache()
{
	for (uint8_t i = 0; i < 32; i++)
	{
		cacheLCD.string[i] = 0;
	}
	cacheLCD.line = 0;
	cacheLCD.position = 0;
}
void CacheCursor(uint8_t code)
{
	if (!((code >= 0x80 && code <= 0x8F) || (code >= 0xC0 && code <= 0xCF)))
		return;
	uint8_t line = ((code >> 4) & 0xF) == 0xC ? 1 : 0;
	uint8_t position = (code & 0xF);
	cacheLCD.line = line;
	cacheLCD.position = position;
}
void Clear_Display()
{
	__disable_irq();
	LCD_WriteCommand(0x01, 2);
	__enable_irq();
}

char *Get_String_LCD(int8_t from, int8_t to)
{
	if (from > 31)
		from = 31;
	if (to > 31)
		to = 31;
	if (from < 0)
		from = min(32 + from, 0);
	if (to < 0)
		to = min(32 + to, 0);
	char *output = malloc(sizeof(char) * ((abs(to - from) + 1)));
	int8_t j = 0;
	if (from > to)
		for (int8_t i = to; i >= from; i--)
			output[j++] = cacheLCD.string[i];
	else
		for (int8_t i = from; i <= to; i++)
			output[j++] = cacheLCD.string[i];
	output[j] = '\0';
	return output;
}
void Set_LCD(char *string)
{
	__disable_irq();
	Clear_Display();
	Write_String_LCD(string);
	__enable_irq();
}

void WriteStringAt(char *string, uint8_t line, uint8_t position)
{
	__disable_irq();
	Set_CursorPosition(line, position);
	Write_String_LCD(string);
	__enable_irq();
}
uint8_t CursorPositionToCode(uint8_t line, uint8_t position)
{
	__disable_irq();
	uint8_t location = line ? 0xC0 : 0x80;
	location |= position;
	__enable_irq();
	return location;
}
void Set_CursorPosition(uint8_t line, uint8_t position)
{
	__disable_irq();
	uint8_t location = CursorPositionToCode(line, position);
	LCD_WriteCommand(location, 0);
	__enable_irq();
}

void Write_String_Sector_LCD(uint8_t sector, char *string)
{
	__disable_irq();
	// uint8_t lastPosition = cacheLCD.position;
	// uint8_t lastLine = cacheLCD.line;
	Set_CursorPosition(sector / 4, (sector % 4) * 4);
	Write_String_LCD(string);
	// Set_CursorPosition(lastLine, lastPosition);
	__enable_irq();
}

void DisplayNumber(long num, int8_t line, int8_t position, uint8_t from, uint8_t digits)
{
	__disable_irq();
	int logOf = digits - 1;
	if ((line != -1) && (position != -1))
		Set_CursorPosition(line, from ? position - logOf : position);
	if (num < 0)
	{
		LCD_WriteData('-');
		--logOf;
		num = -num;
	}

	for (int i = logOf; i >= 0; i--)
	{
		uint8_t place = (int)num / (int)pow(10, i);
		uint8_t digit = place % 10;
		LCD_WriteData(digit + '0');
	}
	__enable_irq();
}

void DisplayDecimal(double num, int8_t line, int8_t position, uint8_t from, uint8_t digits)
{
	__disable_irq();
	int maxDigits = digits - 1;
	int logOf = (abs(num) <= 1) ? 0 : (int)log10(abs(num));
	uint8_t negitive = 0;

	if (num < 0)
	{
		negitive = 1;
		--maxDigits;
		num = -num;
	}
	if (logOf > maxDigits)
		logOf = maxDigits;
	if ((line != -1) && (position != -1))
		Set_CursorPosition(line, from ? position - maxDigits : position);
	int decimalplaces = fmin(-(maxDigits - logOf - 1), 0);
	if (negitive)
		LCD_WriteData('-');
	int j = 0;
	for (int i = logOf; i >= decimalplaces; i--)
	{
		int place = (int)(num / pow(10, i));
		int digit = place % 10;
		LCD_WriteData(digit + '0');
		if (j++ < maxDigits && i == 0)
			LCD_WriteData('.');
	}
	__enable_irq();
}
