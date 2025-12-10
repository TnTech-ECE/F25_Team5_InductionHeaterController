/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef SAVE_H_
#define SAVE_H_
struct ControllerData
{
	float desiredTemperature;
	float t1;
	float t2;
};
#include "ff.h"
#include "user_diskio.h"
#include "stdbool.h"
#include "save.h"

void test();

bool saveControllerDataSD(const char *path, const struct ControllerData *data);

// Binary load of a single struct
bool loadControllerDataSD(const char *path, struct ControllerData *out);

// Append another record (for logging multiple samples)
bool appendControllerDataSD(const char *path, const struct ControllerData *data);
#endif /* RUN_C_ */
