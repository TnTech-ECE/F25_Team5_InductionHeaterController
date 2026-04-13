#ifndef ADC_H_
#define ADC_H_
extern volatile float rmsCurrent;
extern volatile float maxCurrent;
void startADC(void);
float getRMSCurrent();
float getAVGCurrent();
#endif
