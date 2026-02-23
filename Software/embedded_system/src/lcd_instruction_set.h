/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef LCD_INSTRUCTION_SET_H_
#define LCD_INSTRUCTION_SET_H_
enum LCD1602aInstructionSet
{
	ClearDisplay = 0b1,
	ReturnHome = 0b10,
	EntryModeSetCursorMoveDirectIncrement = 0b110,
	EntryModeSetCursorMoveDirectDecrement = 0b100,
	EntryModeSetShiftOnWrite = 0b101,
	DisplayOnOffControlDisplayOn = 0b1100,
	DisplayOnOffControlDisplayOff = 0b1000,
	DisplayOnOffControlCursorOn = 0b1010,
	DisplayOnOffControlCursorOff = 0b1000,
	DisplayOnOffControlCursorBlinkOn = 0b1001,
	DisplayOnOffControlCursorBlinkOff = 0b1000,
	CursorOrDisplayShiftDisplayShift = 0b10000,
	CursorOrDisplayShiftCursorMove = 0b11000,
	CursorOrDisplayShiftCursorShiftRight = 0b10100,
	CursorOrDisplayShiftCursorShiftLeft = 0b10000,
	FunctionSet8Bit = 0b110000,
	FunctionSet4Bit = 0b100000,
	FunctionSet2Lines = 0b100000,
	FunctionSet1Lines = 0b101000,
	FunctionSetChar5x10Dots = 0b100100,
	FunctionSetChar5x7Dots = 0b100000,
	SetCGRamAddress = 0b1000000,
	SetDDRamAddress = 0b10000000,
	SetDDRamAddressLine0 = 0b10000000,
	SetDDRamAddressLine1 = 0b11000000,
	ReadBusyFlagAndAddress = 0b100000000,
	WriteDataToCGorDDRam = 0b1000000000,
	ReadDataFromCGorDDRam = 0b1100000000
};
#endif /* LCD_INSTRUCTION_SET_H_ */
