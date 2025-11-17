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
};
#include "ff.h"
#include "user_diskio.h"
#include "stdbool.h"
#include "save.h"

// chat gpt wrote this
bool mountSD(void);
bool saveControllerDataSD(const char *path, const struct ControllerData *data);
bool loadControllerDataSD(const char *path, struct ControllerData *out);
bool appendControllerDataSD(const char *path, const struct ControllerData *data);
bool unmountSD(void);

#endif /* RUN_C_ */
