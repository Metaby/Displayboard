/*
 * count_down.c
 *
 * Created: 01.02.2016 07:10:33
 *  Author: Max
 */ 
#include "count_down.h"

uint8_t count_down_fnct() {
	count = 20;
	while (count > 0) {
		tm_print_byte(count);
		tm_print("\n");
		count--;
		system_sleep_ms(1000);
	}
	return 0;
}