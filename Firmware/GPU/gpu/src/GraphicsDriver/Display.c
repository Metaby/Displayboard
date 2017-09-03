/*
 * Display.c
 *
 * Created: 09.10.2012 10:50:51
 *  Author: Max
 */
#include "Display.h"
#include "../Tools/Unicode.h"
#include "../Tools/Tools.h"

const uint8_t GFX_TEXT_MODE = 0;
const uint8_t GFX_GRAPHICS_MODE = 1;

void disp_driver_initialize() {
	display_driver_mode = GFX_TEXT_MODE;
	for (uint8_t i = 0; i < LINES; i++) {
		for (uint8_t j = 0; j < LINE_LENGTH; j++) {
			gfx_text_mode_content[i][j] = 0x00;
		}
	}
	gfx_text_mode_content_position = 0;
	gfx_text_mode_content_line = 0;
	display_initialize();
	display_clear();
	buffer_clear();
}

void disp_driver_set_text_mode() {
	display_driver_mode = GFX_TEXT_MODE;
	TIMSK1 = (0 << TOIE1);
}

void disp_driver_set_graphics_mode() {
	display_driver_mode = GFX_GRAPHICS_MODE;
	TIMSK1 = (1 << TOIE1);
}

void tm_clear() {
	if (display_driver_mode == GFX_TEXT_MODE) {
		for (uint8_t i = 0; i < LINES; i++) {
			for (uint8_t j = 0; j < LINE_LENGTH; j++) {
				gfx_text_mode_content[i][j] = 0x00;
			}
		}
		gfx_text_mode_content_position = 0;
		gfx_text_mode_content_line = 0;
		cli();
		buffer_clear();
		display_send_buffer();
		sei();
	}
}

void tm_print_byte(uint8_t val) {	
	uint8_t str[10];
	sprintf(str, "%i", val);
	tm_print(str);
}

void tm_print(uint8_t* str) {
	if (display_driver_mode == GFX_TEXT_MODE) {
		uint8_t i = 0;
		uint8_t ch = str[i++];
		while (ch != 0x00) {
			if (ch == 0x0A || gfx_text_mode_content_position == LINE_LENGTH) {
				gfx_text_mode_content_position = 0;
				gfx_text_mode_content_line++;
				if (gfx_text_mode_content_line == LINES) {
					gfx_text_mode_content_line--;
					for (uint8_t k = 0; k < LINES - 1; k++) {
						for (uint8_t j = 0; j < LINE_LENGTH; j++) {
							gfx_text_mode_content[k][j] = gfx_text_mode_content[k + 1][j];
						}
					}
					for (uint8_t k = 0; k < LINE_LENGTH; k++) {
						gfx_text_mode_content[gfx_text_mode_content_line][k] = 0x00;
					}
					if (ch != 0x0A) {
						gfx_text_mode_content[gfx_text_mode_content_line][gfx_text_mode_content_position++] = ch;
					}
				}
			} else {
				gfx_text_mode_content[gfx_text_mode_content_line][gfx_text_mode_content_position++] = ch;
			}
			ch = str[i++];
		}
		buffer_clear();
		for (uint8_t k = 0; k < LINES; k++) {
			buffer_write_text(gfx_text_mode_content[k], 0, k * 2);
		}
		cli();
		display_send_buffer();
		sei();
	}
}

void gfx_fill(vector_2 pos, uint8_t color) {
	if (display_driver_mode == GFX_GRAPHICS_MODE)	 {
		buffer_flood_fill(pos.x, pos.y, color);
	}
}

void gfx_clear() {
	if (display_driver_mode == GFX_GRAPHICS_MODE) {
		buffer_clear();
	}
}

void gfx_draw_circle(vector_2 c, uint8_t r, uint8_t color) {
	if (display_driver_mode == GFX_GRAPHICS_MODE) {
		buffer_draw_circle(c.x, c.y, r, color);
	}
}

void gfx_draw_line(vector_2 p1, vector_2 p2, uint8_t color) {
	if (display_driver_mode == GFX_GRAPHICS_MODE) {
		buffer_draw_line(p1.x, p1.y, p2.x, p2.y, color);
	}
}

void gfx_draw_triangle(vector_2 p1, vector_2 p2, vector_2 p3, uint8_t color) {
	if (display_driver_mode == GFX_GRAPHICS_MODE) {
		buffer_draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
	}
}

void gfx_draw_rectangle(vector_2 lb, vector_2 wh, uint8_t color) {
	if (display_driver_mode == GFX_GRAPHICS_MODE) {
		buffer_draw_rectangle(lb.x, lb.y, wh.x, wh.y, color);
	}
}


static void buffer_write_char(uint8_t Char, uint8_t X, uint8_t Y)
{
	if (Char < 128 && Char > 32) {
		Char = (Char - 32);
		display_row_flag[Y] = 1;
		display_row_flag[Y + 1] = 1;
		for (uint8_t i = 0; i < 6; i++)
		{
			display_buffer[X + i][Y] = pgm_read_byte(&charset[Char][i]);
			display_buffer[X + i][Y + 1] = pgm_read_byte(&charset[Char][i + 6]);
		}
	}
}

static void buffer_write_text(uint8_t Text[], uint8_t X, uint8_t Y)
{
	uint8_t strLn = strlen(Text);
	for (uint8_t i = 0; i < strLn; i++)
	{
		buffer_write_char(Text[i], X, Y);
		X += 6;	
	}
}

static void buffer_fill(uint8_t color)
{
	for (uint8_t y = 0; y < 26; y++)
	{
		display_row_flag[y] = 1;
		for (uint8_t x = 0; x < 160; x++)
		{
			display_buffer[x][y] = 0xFF;
		}
	}
}

static void buffer_clear()
{
	for (uint8_t y = 0; y < 26; y++)
	{
		display_row_flag[y] = 1;
		for (uint8_t x = 0; x < 160; x++)
		{
			display_buffer[x][y] = 0x00;
		}
	}
}

static void buffer_set_pixel(uint8_t X, uint8_t Y, uint8_t Color)
{
	uint8_t row = Y / 4;
	uint8_t page;
	page = display_buffer[X][row];
 	page = page & (0xFF ^ (Color << (Y % 4) * 2));
 	page = page | (Color << (Y % 4) * 2);
	display_buffer[X][row] = page;
	display_row_flag[row] = 1;
}

static void buffer_draw_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Color)
{
	int16_t x;
	int16_t y;
	int16_t t;
	int16_t dx;
	int16_t dy;
	int16_t incx;
	int16_t incy;
	int16_t pdx;
	int16_t pdy;
	int16_t ddx;
	int16_t ddy;
	int16_t es;
	int16_t el;
	int16_t err;
   	dx = X1 - X0;
   	dy = Y1 - Y0;
   	incx = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
  	incy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
  	if(dx < 0)
  	 	dx = -dx;
 	if(dy < 0)
   		dy = -dy;
	if (dx > dy)
	{
		pdx = incx;
		pdy = 0;
		ddx = incx;
		ddy = incy;
		es = dy;
		el = dx;
	}
	else
	{
		pdx = 0;
		pdy = incy;
		ddx = incx;
		ddy = incy;
		es = dx;
		el = dy;
	}
	x = X0;
	y = Y0;
	err = el / 2;
	buffer_set_pixel(x, y, Color);
	for(t = 0; t < el; ++t)
	{
		err -= es; 
		if(err < 0)
		{
		    err += el;
		    x += ddx;
		    y += ddy;
		}
		else
		{
		    x += pdx;
		    y += pdy;
		}
		buffer_set_pixel(x, y, Color);
	}
}

static void buffer_draw_circle(uint8_t X0, uint8_t Y0, uint8_t Radius, uint8_t Color)
{	
	int8_t f = 1 - Radius;
	int8_t ddDogXL_x = 0;
	int8_t ddDogXL_y = -2 * Radius;
	int8_t x = 0;
	int8_t y = Radius;	 
	buffer_set_pixel(X0, Y0 + Radius, Color);
	buffer_set_pixel(X0, Y0 - Radius, Color);
	buffer_set_pixel(X0 + Radius, Y0, Color);
	buffer_set_pixel(X0 - Radius, Y0, Color);  
	while(x < y) 
	{
		if(f >= 0) 
		{
			y--;
			ddDogXL_y += 2;
			f += ddDogXL_y;
		}
		x++;
		ddDogXL_x += 2;
		f += ddDogXL_x + 1;	
		buffer_set_pixel(X0 + x, Y0 + y, Color);
		buffer_set_pixel(X0 - x, Y0 + y, Color);
		buffer_set_pixel(X0 + x, Y0 - y, Color);
		buffer_set_pixel(X0 - x, Y0 - y, Color);
		buffer_set_pixel(X0 + y, Y0 + x, Color);
		buffer_set_pixel(X0 - y, Y0 + x, Color);
		buffer_set_pixel(X0 + y, Y0 - x, Color);
		buffer_set_pixel(X0 - y, Y0 - x, Color);
	}
}	
   
 static void buffer_draw_rectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t Color)
 {
 	buffer_draw_line(X, Y, X + Width, Y, Color);
	buffer_draw_line(X, Y + Height, X + Width, Y + Height, Color);	
	buffer_draw_line(X, Y, X, Y + Height, Color);
	buffer_draw_line(X + Width, Y, X + Width, Y + Height, Color);
 }	
 
 static void buffer_draw_triangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t X3, uint8_t Y3, uint8_t Color)
 {	   
 	buffer_draw_line(X1, Y1, X2, Y2, Color);
 	buffer_draw_line(X2, Y2, X3, Y3, Color);
 	buffer_draw_line(X3, Y3, X1, Y1, Color);
 }	  

static uint8_t buffer_get_pixel(uint8_t X, uint8_t Y)
{
//	uint8_t flag;
	uint8_t row = Y / 4;
	uint8_t page = display_buffer[X][row];
	return (page & (3 << (Y % 4) * 2)) >> (Y % 4);
// 	if (Y % 4 == 0)
// 		flag = ((page & 0x03) == 0x00) ? 0 : 1;
// 	else if (Y % 4 == 1)
// 		flag = ((page & 0x0C) == 0x00) ? 0 : 1;
// 	else if (Y % 4 == 2)
// 		flag = ((page & 0x30) == 0x00) ? 0 : 1;
// 	else
// 		flag = ((page & 0xC0) == 0x00) ? 0 : 1;
//	return flag;
}

static float* get_possible_pixels(uint8_t cx, uint8_t cy, uint8_t X, uint8_t Y, uint8_t old_color) {	
	float possible_pixel[8];
	if (buffer_get_pixel(X + 1, Y) == old_color) {
		possible_pixel[0] = sqrt((cx - (X + 1)) * (cx - (X + 1)) + (cy - Y) * (cy - Y));
		} else {
		possible_pixel[0] = -1;
	}
	if (buffer_get_pixel(X + 1, Y + 1) == old_color) {
		possible_pixel[1] = sqrt((cx - (X + 1)) * (cx - (X + 1)) + (cy - (Y + 1)) * (cy - (Y + 1)));
		} else {
		possible_pixel[1] = -1;
	}
	if (buffer_get_pixel(X, Y + 1) == old_color) {
		possible_pixel[2] = sqrt((cx - X) * (cx - X) + (cy - (Y + 1)) * (cy - (Y + 1)));
		} else {
		possible_pixel[2] = -1;
	}
	if (buffer_get_pixel(X - 1, Y + 1) == old_color) {
		possible_pixel[3] = sqrt((cx - (X - 1)) * (cx - (X - 1)) + (cy - (Y + 1)) * (cy - (Y + 1)));
		} else {
		possible_pixel[3] = -1;
	}
	if (buffer_get_pixel(X - 1, Y) == old_color) {
		possible_pixel[4] = sqrt((cx - (X - 1)) * (cx - (X - 1)) + (cy - Y) * (cy - Y));
		} else {
		possible_pixel[4] = -1;
	}
	if (buffer_get_pixel(X - 1, Y - 1) == old_color) {
		possible_pixel[5] = sqrt((cx - (X - 1)) * (cx - (X - 1)) + (cy - (Y - 1)) * (cy - (Y - 1)));
		} else {
		possible_pixel[5] = -1;
	}
	if (buffer_get_pixel(X, Y - 1) == old_color) {
		possible_pixel[6] = sqrt((cx - X) * (cx - X) + (cy - (Y - 1)) * (cy - (Y - 1)));
		} else {
		possible_pixel[6] = -1;
	}
	if (buffer_get_pixel(X + 1, Y - 1) == old_color) {
		possible_pixel[7] = sqrt((cx - (X + 1)) * (cx - (X + 1)) + (cy - (Y - 1)) * (cy - (Y - 1)));
		} else {
		possible_pixel[7] = 0;
	}
	return possible_pixel;
}

 static void buffer_flood_fill(uint8_t X, uint8_t Y, uint8_t Color)
 {
	// Path Fill
	float cx = X;
	float cy = Y;
	uint8_t old_color = buffer_get_pixel(X, Y);
	while (X <= 160 && buffer_get_pixel(X++, Y) == old_color);
	X--;
	buffer_set_pixel(X, Y, Color);
	float* possible_pixel = get_possible_pixels(cx, cy, X, Y, old_color);
	float minVal = 1000;
	uint8_t minIndex = -1;
	for (uint8_t i = 0; i < 8; i++) {
		if (possible_pixel[i] < minVal) {
			minVal = possible_pixel[i];
			minIndex = i;
		}
	}
	while (minIndex != -1) {
		if (minIndex == 0) {
			X++;
		} else if (minIndex == 1) {
			X++;
			Y++;
		} else if (minIndex == 2) {
			Y++;
		} else if (minIndex == 3) {
			X--;
			Y++;
		} else if (minIndex == 4) {
			X--;
		} else if (minIndex == 5) {
			X--;
			Y--;
		} else if (minIndex == 6) {
			Y--;
		} else if (minIndex == 7) {
			X++;
			Y--;
	}
	buffer_set_pixel(X, Y, Color);
	possible_pixel = get_possible_pixels(cx, cy, X, Y, old_color);
	minVal = 1000;
	minIndex = -1;
	for (uint8_t i = 0; i < 8; i++) {
		if (possible_pixel[i] < minVal) {
			minVal = possible_pixel[i];
			minIndex = i;
		}
	}
	}
// Quad Fill
// 	uint8_t x = X;
// 	uint8_t y = Y;
// 	uint8_t c = buffer_get_pixel(x, y);
// 	while (buffer_get_pixel(X, y) == c) {
// 		while (buffer_get_pixel(x, y) == c && x <= 160) buffer_set_pixel(x++, y, Color);
// 		y++;
// 		x = X;
// 	}
// 	y = Y - 1;
// 	while (buffer_get_pixel(X, y) == c) {
// 		while (buffer_get_pixel(x, y) == c && x <= 160) buffer_set_pixel(x++, y, Color);
// 		y--;
// 		x = X;
// 	}
// 	y = Y;
// 	x = X - 1;
// 	while (buffer_get_pixel(X - 1, y) == c) {
// 		while (buffer_get_pixel(x, y) == c && x > 0) buffer_set_pixel(x--, y, Color);
// 		y++;
// 		x = X - 1;
// 	}
// 	y = Y - 1;
// 	x = X - 1;
// 	while (buffer_get_pixel(X - 1, y) == c) {
// 		while (buffer_get_pixel(x, y) == c && x > 0) buffer_set_pixel(x--, y, Color);
// 		y--;
// 		x = X - 1;
// 	}
// Flood Fill
//  if (buffer_get_pixel(X, Y) == 0)
// 	{
// 		buffer_set_pixel(X, Y, Color);
// 		buffer_flood_fill(X + 1, Y, Color);
// 		buffer_flood_fill(X, Y + 1, Color);
// 		buffer_flood_fill(X - 1, Y, Color);
// 		buffer_flood_fill(X, Y - 1, Color);
// 	}
// 	return;
 }

static void buffer_write_int(uint16_t Number, uint8_t X, uint8_t Y)
{
	uint16_t tmp = Number;
	if (tmp > 9999)
	{
		buffer_write_char(Number / 10000 + 48, X, Y);
		Number = Number % 10000;
		X += 8;
	}	
	if (tmp > 999)
	{
		buffer_write_char(Number / 1000 + 48, X, Y);
		Number = Number % 1000;
		X += 8;
	}
	if (tmp > 99)
	{
		buffer_write_char(Number / 100 + 48, X, Y);
		Number = Number % 100;
		X += 8;
	}
	if (tmp > 9)
	{
		buffer_write_char(Number / 10 + 48, X, Y);
		Number = Number % 10;
		X += 8;
	}
	buffer_write_char(Number + 48, X, Y);
}

static void buffer_invert(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	for (int x = 0; x < Width; x++)
		for (int y = 0; y < Height; y++)
			buffer_set_pixel(x + X, y + Y, (buffer_get_pixel(x + X, y + Y) ? 0 : 1));
}

void buffer_draw_button(uint8_t* Text, uint8_t X, uint8_t Y, uint8_t Inverted)
{
	buffer_write_text(Text, X + 2, Y);
	buffer_draw_rectangle(X, (Y * 4) - 2, strlen(Text) * 6 + 2, 10, COLOR_BLACK);
	if (Inverted)
	{
		buffer_invert(X, (Y * 4) - 2, strlen(Text) * 6 + 2, 10);
	}
}

ISR(TIMER1_OVF_vect)
{
	cli();
	display_send_buffer();
	sei();
}