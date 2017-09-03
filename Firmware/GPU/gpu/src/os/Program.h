/*
 * Program.h
 *
 * Created: 29.01.2016 23:30:47
 *  Author: Max
 */ 
#ifndef PROGRAM_H_
#define PROGRAM_H_
#include "../Tools/Tools.h"
#include "Programs/clear.h"
#include "Programs/text_test.h"
#include "Programs/count_down.h"
#include "Programs/gfx_test.h"
#include "Programs/serial_listener.h"
#include "Programs/robot_control.h"

typedef struct Program {
	uint8_t* name;
	int_fnct_void run;
} Program;

#endif /* PROGRAM_H_ */