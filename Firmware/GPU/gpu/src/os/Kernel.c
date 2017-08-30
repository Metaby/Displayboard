/*
 * Kernel.c
 *
 * Created: 29.01.2016 23:38:06
 *  Author: Max
 */ 
#include "Kernel.h"

void kernel_run() {
	auto_run = "roco";
	tm_print_byte(program_list_size);
	tm_print(" programs loaded\n");
	if (auto_run != NULL) {
		kernel_execute(auto_run);
	}
	while (1);
}

void kernel_initialize() {
	last_call_status = 0;
	program_list_size = 7;
	program_list[0].name = "operating_system";
	program_list[0].run = operating_system_fnct;
	program_list[1].name = "hello_world";
	program_list[1].run = hello_world_fnct;
	program_list[2].name = "count_down";
	program_list[2].run = count_down_fnct;
	program_list[3].name = "gfx_test";
	program_list[3].run = gfx_test_fnct;
	program_list[4].name = "serial_listener";
	program_list[4].run = serial_listener_fnct;
	program_list[5].name = "clear";
	program_list[5].run = clear_fnct;
	program_list[6].name = "roco";
	program_list[6].run = robot_control_fnct;
}

void kernel_execute(uint8_t* command) {
	tm_print("exec> ");
	tm_print(command);
	tm_print("\n");
	for (uint8_t i = 0; i < program_list_size; i++) {
		if (string_equals(command, program_list[i].name)) {
			last_call_status = program_list[i].run();
		}	
	}
}

void kernel_timer_set(uint8_t time_in_ms) {
	kernel_timer_count = UINT16_MAX - (uint16_t)(((float)time_in_ms / 1000.0f) * (20000000.0f / 1024.0f));
	TCNT3L = kernel_timer_count;
	TCNT3H = (kernel_timer_count >> 8);
}

void kernel_timer_enable() {
	TIMSK3 = (1 << TOIE3);
}

void kernel_timer_disable() {
	TIMSK3 = (0 << TOIE3);
}

void kernel_timer_bind(void_fnct_void fnct) {
	kernel_timer_fnct = fnct;
}

void kernel_touch_bind(void_fnct_vector_2 fnct) {
	kernel_touch_fnct = fnct;
}

ISR(TIMER3_OVF_vect)
{
	cli();
	TCNT3L = kernel_timer_count;
	TCNT3H = (kernel_timer_count >> 8);
	if (kernel_timer_fnct != NULL) {
		kernel_timer_fnct();
	}
	sei();
}

ISR(INT2_vect)
{
	cli();
	//TIMSK1 = 0x00;
	TCNT1H = 0xF6;
	TCNT1L = 0x3B;
	vector_2 position = display_get_touch();
	if (kernel_touch_fnct != NULL) {
		kernel_touch_fnct(position);
	}
	//TIMSK1 = (1 << TOIE1);
	sei();
}