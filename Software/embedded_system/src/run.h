/*
 * run.c
 *
 *  Created on: Nov 1, 2025
 *      Author: mrpat
 */

#ifndef RUN_H_
#define RUN_H_
#include "save.h"
#include "max31856.h"
void saveSD();
void run();
extern struct ControllerData controllerData;
extern max31856_t thermoSPI2, thermoSPI3;
#define CONTROLLER_DATA_PATH "controller_data.save"
#define CONTROLLER_LOG_PATH "controller_data.log"
#endif /* RUN_C_ */
