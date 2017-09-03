/*
 * Button.c
 *
 * Created: 28.10.2012 14:24:23
 *  Author: Max
 */ 
#include "Button.h"

void button_draw(Button B, uint8_t Inverted)
{
	buffer_draw_button(B.text, B.x, B.y, Inverted);
}

uint8_t button_get_status(Button B, vector_2 Position)
{
	if (Position.x >=  B.x && Position.x <= B.x + strlen(B.text) * 6 && Position.y >= B.y * 4 && Position.y <= B.y * 4 + 8) {
		return 1;
	}
	return 0;
}