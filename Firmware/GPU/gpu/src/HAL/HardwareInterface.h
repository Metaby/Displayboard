/*
 * HardwareInterface.h
 *
 * Created: 23.11.2012 20:54:56
 *  Author: Max
 */ 
#ifndef _HARDWAREINTERFACE_H_
#define _HARDWAREINTERFACE_H_
#include <avr/io.h>
#include <util/twi.h>
#include "../Extern/i2cmaster.h"
#include "../GraphicsDriver/Display.h"
#include "../Tools/Tools.h"

// Display
void display_initialize();
void display_select_column(uint8_t Column);
void display_select_page(uint8_t Page);
void display_clear();
void display_send_byte(uint8_t Data);
void display_enable(uint8_t CD);
void display_disable();
void display_send_buffer();

// Touch-Input
vector_2 touch_position;
void display_get_touch();

#endif