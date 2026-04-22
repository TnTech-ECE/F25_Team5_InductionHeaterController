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
	KeyUnpressed = 0,
	KeyPressed = 1
};
typedef void (*KeyPressCallback)(char key);
typedef void (*KeyReleaseCallback)(char key);
typedef void (*KeyStateChangeCallback)(char key, enum KeyState keyState);

bool subscribeKeyPress(KeyPressCallback callback);
bool subscribeKeyRelease(KeyReleaseCallback callback);
bool subscribeKeyStateChange(KeyStateChangeCallback callback);

bool unsubscribeKeyPress(KeyPressCallback callback);
bool unsubscribeKeyRelease(KeyReleaseCallback callback);
bool unsubscribeKeyStateChange(KeyStateChangeCallback callback);

#endif /* KEYPAD_H_ */
