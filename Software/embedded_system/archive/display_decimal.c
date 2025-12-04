#include "stdint.h"
#include "stdio.h"
#include "math.h"
void __disable_irq()
{
}
void Set_CursorPosition(uint8_t line, uint8_t position)
{
}
void __enable_irq()
{
}
void LCD_WriteData(char data)
{
	printf("%c", data);
}
void DisplayDecimal(double num, int8_t line, int8_t position, uint8_t from, uint8_t digits)
{
	__disable_irq();
	int maxDigits = digits - 1;
	int logOf = (num <= 1) ? 0 : (int)log10(num);
	printf("%d\n", logOf);
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
	DisplayDecimal(0.01, 0, 0, 0, 4);
	return 1;
}