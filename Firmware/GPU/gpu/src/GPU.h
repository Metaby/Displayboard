/*
 * GPU.h
 *
 * Created: 28.10.2012 14:46:31
 *  Author: Max
 */ 
#ifndef _GPU_H_
#define _GPU_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/twi.h>
#include "Extern/i2cmaster.h"
#include "GraphicsDriver/Display.h"
#include "Tools/Tools.h"
#include "GUI/Button.h"
#include "HAL/HardwareInterface.h"
#include "OS/Kernel.h"
#include "OS/Program.h"

volatile uint8_t brightness = 31;

void hardware_initialize();

#endif