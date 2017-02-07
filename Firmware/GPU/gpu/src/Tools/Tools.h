/*
 * Tools.h
 *
 * Created: 09.10.2012 10:54:12
 *  Author: Max
 */
#ifndef F_CPU
#define F_CPU 20000000UL
#endif
#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Extern/dallas_one_wire.h"

typedef void (*void_fnct_void) (void);
typedef int (*int_fnct_void) (void);

// #define CV2(X,Y)	vector_2 tmp; \
// 					tmp.x = X;	\
// 					tmp.y = Y;	\
// 					tmp;

struct vector_2
{
	uint8_t x;
	uint8_t y;
} typedef vector_2;

struct vector_3
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
} typedef vector_3;

extern uint16_t brightness_table[32];

uint8_t* uint_to_string(uint16_t val);
uint8_t string_starts_with(uint8_t* str1, uint8_t* str2);
uint8_t string_equals(uint8_t* str1, uint8_t* str2);
void system_sleep_ms (uint16_t ms);
void system_sleep_us (uint16_t ms);
double system_read_temperature();

#endif