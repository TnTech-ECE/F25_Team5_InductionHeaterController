
/*
 * lcd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef PWM_H_
#define PWM_H_
extern bool isPWMStarted;
extern bool isPWMEnabled;
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param phaseDegrees 0-360
 */
void TIM1_8_Update(float frequency, float dutyCycle, float deadTime, float phaseDegrees);

void TIM1_8_start();
void TIM1_8_stop();
void updateTIM1_8_PowerLevel(float frequency, float powerLevel);
void updateTIM1_8_PowerLevelWithStart(float frequency, float powerLevel);
void TIM1_8_Disable();
void TIM1_8_Enable();
#endif // PWM_H_