#include <stdio.h>
#include <stdint.h>
#include "math.h"
void LCD_WriteData(char data)
{
	printf("%c", data);
}
void Set_CursorPosition(int line, int position)
{
}
void DisplayDecimal(double num, int8_t line, int8_t position, uint8_t from, uint8_t digits)
{
	__disable_irq();
	int maxDigits = digits - 1;
	int logOf = (int)log10(num);

	if (logOf > maxDigits)
		logOf = maxDigits;
	if ((line != -1) && (position != -1))
		Set_CursorPosition(line, from ? position - maxDigits : position);
	if (num < 0)
	{
		LCD_WriteData('-');
		--logOf;
		num = -num;
	}
	int decimalplaces = fmin(-(maxDigits - logOf - 1), 0);
	printf("%d\n", maxDigits);
	printf("%d\n", logOf);
	printf("%d\n", decimalplaces);
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

int main()
{
	DisplayDecimal(1123.123f, 0, 0, 0, 5);
}