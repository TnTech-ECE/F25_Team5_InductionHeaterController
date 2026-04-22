#ifndef SET_PID_H_
#define SET_PID_H_
void modeStartStopPID(bool startNotStop);
extern uint8_t positionsSetPID[9];
void Set_CursorPositionPID(uint8_t index);
#endif
