/*
 * gfx_test.c
 *
 * Created: 05.02.2016 00:27:41
 *  Author: Max
 */ 

#include "gfx_test.h"

vector_2 center;
uint8_t dir;
uint8_t r;

#define CV2(X,Y) vector_2 v = {X, Y}

void timer_fnct() {
	r += dir;
	if (r == 50) {
		dir = -1;
	}
	if (r == 0) {
		dir = 1;
	}
	gfx_clear();
	gfx_draw_circle(center, r, COLOR_BLACK);
	vector_2 v1 = {0, 0};
	vector_2 v2 = {20, 20};
	vector_2 vc = {11, 11};
	gfx_draw_rectangle(v1, v2, COLOR_BLACK);
	gfx_fill(vc, COLOR_WHITE);
	v1.x += 20;
	vc.x += 20;
	gfx_draw_rectangle(v1, v2, COLOR_BLACK);
	gfx_fill(vc, COLOR_LIGHT_GRAY);
	v1.x += 20;
	vc.x += 20;
	gfx_draw_rectangle(v1, v2, COLOR_BLACK);
	gfx_fill(vc, COLOR_DARK_GRAY);
	v1.x += 20;
	vc.x += 20;
	gfx_draw_rectangle(v1, v2, COLOR_BLACK);
	gfx_fill(vc, COLOR_BLACK);
}

uint8_t gfx_test_fnct() {
	disp_driver_set_graphics_mode();
	center.x = 80;
	center.y = 52;
	dir = 1;
	r = 1;
	kernel_timer_set(150);
	kernel_timer_bind(timer_fnct);
	kernel_timer_enable();
	return 0;
}