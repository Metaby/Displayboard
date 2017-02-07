/*
 * Tools.c
 *
 * Created: 09.10.2012 10:54:03
 *  Author: Max
 */
#include "Tools.h"

uint16_t brightness_table[32] =
{
	0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11, 13, 16, 19, 23,
	27, 32, 38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215, 255
};

void system_sleep_ms (uint16_t ms)
{
	for(; ms > 0; ms--) _delay_ms(1);
}

void system_sleep_us (uint16_t ms)
{
	for(; ms > 0; ms--) _delay_us(1);
}

uint8_t* uint_to_string(uint16_t val) {
	uint8_t str[10];	
// 	uint8_t limit = 10000;
// 	uint8_t i = 0;
// 	while (limit != 0) {
// 		if (val / limit != 0) {
// 			str[i++] = (val / limit) + 48;
// 			val = val % limit;
// 			limit = limit / 10;
// 		} else {
// 			limit = limit / 10;
// 		}
// 	}
// 	str[i] = 0x00;	
	sprintf(str, "%i", val);	
// 	itoa(val, str, 10);	
	return str;
}

uint8_t string_starts_with(uint8_t* str1, uint8_t* str2)
{
// 	for (uint8_t i = 0; i < strlen(Text); i++)
// 	if (uartBuffer[i] != Text[i])
// 		return 0;
	return 1;
}

uint8_t string_equals(uint8_t* str1, uint8_t* str2) {
	uint8_t i;
	for (i = 0; str1[i] != 0x00 && str2[i] != 0x00; i++) {
		if (str1[i] != str2[i]) {
			return 0;
		}
	}
	if (str1[i] != str2[i]) {
		return 0;
	}
	return 1;
}

double system_read_temperature()
{
	cli();
	dallas_skip_rom();
	dallas_write_byte(0x44);
	while (dallas_read() == 0);
	dallas_skip_rom();
	dallas_write_byte(0xBE);
	uint8_t l = dallas_read_byte();
	uint8_t h = dallas_read_byte();
	double temp = 64 * ((h & 4) > 0 ? 1 : 0) + 32 * ((h & 2) > 0 ? 1 : 0) + 16 * ((h & 1) > 0 ? 1 : 0)
	+ 8 * ((l & 128) > 0 ? 1 : 0) + 4 * ((l & 64) > 0 ? 1 : 0) + 2 * ((l & 32) > 0 ? 1 : 0) + 1 * ((l & 16) > 0 ? 1 : 0)
	+ 0.5 *((l & 8) > 0 ? 1 : 0) + 0.25 * ((l & 4) > 0 ? 1 : 0)+ 0.125 * ((l & 2) > 0 ? 1 : 0) + 0.0625 * ((l & 1) > 0 ? 1 : 0);
	dallas_reset();
	sei();
	return temp;
}