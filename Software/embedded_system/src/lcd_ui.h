#ifndef LCD_UI_H
#define LCD_UI_H
enum DisplayMode
{
	NoModeSet = -1,
	SystemVitals = 0,
	SetPowerLevel = 1,
	SetDesiredTemperature = 2,
	SetFrequency = 3,
	SetSettings = 4,
	DisplayFault = 5,
	DisplayMessage = 6,
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