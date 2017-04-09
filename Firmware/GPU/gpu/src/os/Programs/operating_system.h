/*
 * operating_system.h
 *
 * Created: 09.04.2017 02:44:40
 *  Author: Max Brand
 */ 

#include "../../GraphicsDriver/Display.h"
#include "../../Tools/Tools.h"
#include "../../GUI/Button.h"
#include "../Kernel.h"

#ifndef OPERATING_SYSTEM_H_
#define OPERATING_SYSTEM_H_

uint8_t operating_system_fnct();
void process_touch();

Button serialListenerButton;

#endif /* HELLO_WORLD_H_ */