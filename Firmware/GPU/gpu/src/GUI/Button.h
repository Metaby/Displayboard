/*
 * Button.h
 *
 * Created: 28.10.2012 14:26:19
 *  Author: Max
 */
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <avr/io.h>
#include "../Tools/Tools.h"
#include "../GraphicsDriver/Display.h"
 
#define EOB { "", -1 ,1 }
 
struct Button
{
	uint8_t* text;
	uint8_t x;
	uint8_t y;
} typedef Button;

void button_draw(Button B, uint8_t Inverted);
uint8_t button_get_status(Button B, vector_2 Position);

#endif