/*
 * Display.h
 *
 * Created: 09.10.2012 10:51:19
 *  Author: Max
 */
 #ifndef _DISPLAY_H_
 #define _DISPLAY_H_
 #include <avr/interrupt.h>
 #include <avr/io.h>
 #include <string.h>
 #include "../HAL/HardwareInterface.h"
 #include "../Tools/Tools.h"
 
 #define LINES 13
 #define LINE_LENGTH 22
 #define COLOR_WHITE 0
 #define COLOR_LIGHT_GRAY 1
 #define COLOR_DARK_GRAY 2
 #define COLOR_BLACK 3

 extern const uint8_t GFX_TEXT_MODE;
 extern const uint8_t GFX_GRAPHICS_MODE;

 uint8_t display_buffer[160][26];
 uint8_t display_row_flag[26];
 
 uint8_t gfx_text_mode_content[LINES][LINE_LENGTH];
 uint8_t gfx_text_mode_content_position;
 uint8_t gfx_text_mode_content_line;
 uint8_t display_driver_mode;
 
 // GFX
 void disp_driver_initialize();
 void disp_driver_set_text_mode();
 void disp_driver_set_graphics_mode();
 void tm_print_byte(uint8_t val);
 void tm_print(uint8_t* str);
 void tm_clear();
 void gfx_clear();
 void gfx_fill(vector_2 pos, uint8_t color);
 void gfx_draw_circle(vector_2 c, uint8_t r, uint8_t color);
 void gfx_draw_line(vector_2 p1, vector_2 p2, uint8_t color);
 void gfx_draw_triangle(vector_2 p1, vector_2 p2, vector_2 p3, uint8_t color);
 void gfx_draw_rectangle(vector_2 lb, vector_2 wh, uint8_t color);
 
 // Buffer
 static void buffer_write_text(uint8_t Text[], uint8_t X, uint8_t Y);
 static void buffer_write_char(uint8_t Char, uint8_t X, uint8_t Y);
 static void buffer_fill(uint8_t Color);
 static void buffer_clear();
 static void buffer_set_pixel(uint8_t X, uint8_t Y, uint8_t Color);
 static void buffer_draw_line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Color);
 static void buffer_draw_circle(uint8_t X0, uint8_t Y0, uint8_t Radius, uint8_t Color);
 static void buffer_draw_rectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t Color); 
 static void buffer_draw_triangle(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t X3, uint8_t Y3, uint8_t Color);
 static uint8_t buffer_get_pixel(uint8_t X, uint8_t Y);
 static void buffer_flood_fill(uint8_t X, uint8_t Y, uint8_t Color);
 static void buffer_write_int(uint16_t Number, uint8_t X, uint8_t Y);
 static void buffer_invert(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
 static void buffer_draw_button(uint8_t* Text, uint8_t X, uint8_t Y, uint8_t Inverted);
 
 #endif