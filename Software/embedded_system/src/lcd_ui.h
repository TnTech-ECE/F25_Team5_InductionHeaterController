#ifndef LCD_UI_H
#define LCD_UI_H
enum DisplayMode
{
	NoModeSet = -1,
	SystemVitals = 0,
	SetPowerMode = 1,
	SetDesiredTemperature = 2,
	DisplayFault = 3,
	DisplayMessage = 4,
};
typedef void MessageCallback(void *aux);

extern enum DisplayMode displayMode;
void showMessageForTime(MessageCallback callbackMessage, unsigned time, enum DisplayMode displayMode, void *aux);
void setDisplayMode(enum DisplayMode mode);
void LCDstartUI();
#endif