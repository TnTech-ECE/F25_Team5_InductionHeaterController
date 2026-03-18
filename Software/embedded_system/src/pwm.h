
/*
 * lcd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef PWM_H_
#define PWM_H_
extern bool isPWMEnabled;
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param phaseDegrees 0-360
 */
void TIM8_Update(float frequency, float dutyCycle, float deadTime, float phaseDegrees);

void TIM8_start();
void TIM8_stop();
void updateTIM8PowerLevel(float frequency, float powerLevel);
#endif // PWM_H_