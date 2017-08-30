/*
 * robot_control.h
 *
 * Created: 30.08.2017 23:07:30
 *  Author: Max
 */ 


#ifndef ROBOT_CONTROL_H_
#define ROBOT_CONTROL_H_

#include "../../GraphicsDriver/Display.h"

uint8_t bbat_level;
uint8_t mbat_level;

uint8_t robot_control_fnct();
uint8_t read_bbat_level();
uint8_t read_mbat_level();

#endif /* ROBOT_CONTROL_H_ */