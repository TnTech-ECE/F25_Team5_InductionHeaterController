/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#include <stdint.h>
void onRotate(int16_t cnt, uint32_t counting_down);
void onWrap(uint32_t counting_down);
void onRotaryPress();

#endif /* ROTARY_ENCODER_H_ */
