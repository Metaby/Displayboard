/*
 * Kernel.c
 *
 * Created: 29.01.2016 23:38:06
 *  Author: Max
 */ 
#include "Kernel.h"

void kernel_run() {
	while (1) {
		kernel_mode = 1;
		text_test_button.text = "Text-Test";
		text_test_button.x = 2;
		text_test_button.y = 23;
		serial_listener_button.text = "Serial-Listener";
		serial_listener_button.x = 2;
		serial_listener_button.y = 1;
		gfx_test_button.text = "GFX-Test";
		gfx_test_button.x = 60;
		gfx_test_button.y = 23;
		disp_driver_set_graphics_mode();
		gfx_clear();
		button_draw(text_test_button, 0);
		button_draw(serial_listener_button, 0);
		button_draw(gfx_test_button, 0);
		while (kernel_mode) {};
		kernel_execute(cmd);
		cmd = "";
	};
}

void kernel_initialize() {
	last_call_status = 0;
	program_list_size = 6;
	program_list[0].name = "text_test";
	program_list[0].run = text_test_fnct;
	program_list[1].name = "count_down";
	program_list[1].run = count_down_fnct;
	program_list[2].name = "gfx_test";
	program_list[2].run = gfx_test_fnct;
	program_list[3].name = "serial_listener";
	program_list[3].run = serial_listener_fnct;
	program_list[4].name = "clear";
	program_list[4].run = clear_fnct;
	program_list[5].name = "robot_control";
	program_list[5].run = robot_control_fnct;
}

void kernel_execute(uint8_t* command) {
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
	TCNT1H = 0xF6;
	TCNT1L = 0x3B;
	display_get_touch();
	if (kernel_mode) {
		if (button_get_status(text_test_button, touch_position)) {
			cmd = "text_test";
			kernel_mode = 0;
		}
		if (button_get_status(serial_listener_button, touch_position)) {
			cmd = "serial_listener";
			kernel_mode = 0;
		}
		if (button_get_status(gfx_test_button, touch_position)) {
			cmd = "gfx_test";
			kernel_mode = 0;
		}
	} else {
		if (kernel_touch_fnct != NULL) {
			kernel_touch_fnct(touch_position);
		}
	}
	sei();
}