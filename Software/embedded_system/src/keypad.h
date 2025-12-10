/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
void rowEvent(uint8_t rowNumber);
void initKeypad();

enum KeyState
{
	Unpressed = 0,
	Pressed = 1
};

void subscribeKeyPress(KeyPressCallback callback);
void subscribeKeyRelease(KeyReleaseCallback callback);
void subscribeKeyStateChange(KeyReleaseCallback callback);

void unsubscribeKeyPress(KeyPressCallback callback);
void unsubscribeKeyRelease(KeyReleaseCallback callback);
void unsubscribeKeyStateChange(KeyReleaseCallback callback);

#endif /* KEYPAD_H_ */
