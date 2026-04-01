#ifndef LCD_UI_H
#define LCD_UI_H
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "lcd.h"

#include "delay.h"
#include "run.h"
#include "save.h"
#include "keypad.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "max31856.h"
#include "pwm.h"
#include "lcd_instruction_set.h"
#include "PID.h"
extern unsigned cursor;
extern int setOffset;
extern char *setUnit;
extern char *setMessage;
enum DisplayMode
{
	NoModeSet = -1,
	SystemVitals = 0,
	SetPowerLevel = 1,
	SetDesiredTemperature = 2,
	SetFrequency = 3,
	SetSettings = 4,
	SetPID = 5,
	DisplayFault = 6,
	DisplayMessage = 7,
};
typedef void MessageCallback(void *aux);

extern enum DisplayMode displayMode;
extern int setOffset;
void showMessageForTime(MessageCallback callbackMessage, unsigned time, enum DisplayMode displayMode, void *aux);
void setDisplayMode(enum DisplayMode mode);
void LCDstartUI();
void showErrorMessage(char *message);
void startTimer();
#endif