
/*
 * lcd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef PWM_H_
#define PWM_H_
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 */
void TIM8_Update(float frequency, float dutyCycle, float deadTime);
/**
 * @param frequency
 * @param dutyCycle 0-100%
 * @param deadTime 0-100%
 * @param start
 */
void TIM8_UpdateStart(float frequency, float dutyCycle, float deadTime, bool start);
void TIM8_start();
void TIM8_stop();
#endif // PWM_H_