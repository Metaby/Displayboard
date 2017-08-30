/*
 * robot_control.c
 *
 * Created: 30.08.2017 23:07:15
 *  Author: Max
 */ 
#include "robot_control.h"

uint8_t robot_control_fnct() {
	while (1) {
		bbat_level = read_bbat_level();
		mbat_level = read_mbat_level();
		tm_clear();
		tm_print("Akkustand\n");
		tm_print("  Steuerung: ");
		tm_print_byte(bbat_level);
		tm_print("%\n");
		tm_print("  Antrieb:   ");
		tm_print_byte(mbat_level);
		tm_print("%\n");
		system_sleep_ms(1000);
	}
	return 0;
}

uint8_t read_bbat_level() {
	return 70;
}

uint8_t read_mbat_level() {
	return 40;
}