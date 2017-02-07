/*
 * Kernel.c
 *
 * Created: 29.01.2016 23:38:06
 *  Author: Max
 */ 
#include "Kernel.h"

void kernel_run() {
	auto_run = "gfx_test";
	tm_print(uint_to_string(program_list_size));
	tm_print(uint_to_string(20));
	tm_print(" programs loaded\n");
	tm_print("listening to RS232\n");
	uint8_t uart_input[20];
	uint8_t uart_input_position = 0;
	for (uint8_t i = 0; i < 20; i++) {
		uart_input[i] = 0x00;
	}
	if (auto_run != NULL) {
		kernel_execute(auto_run);
	}
	while (1) {
		while (!(UCSR0A & (1 << RXC0)));
		uint8_t ch = UDR0;
		if (ch == 0x0A) {
			kernel_execute(uart_input);
			uart_input_position = 0;
			for (uint8_t i = 0; i < 20; i++) {
				uart_input[i] = 0x00;
			}
		} else {
			uart_input[uart_input_position++] = ch;
		}
	};
}

Program kernel_add_program(uint8_t* name, int_fnct_void fnct) {
	Program p;
	p.name = name;
	p.run = fnct;
}

void kernel_initialize() {
	last_call_status = 0;
	program_list_size = 4;
	program_list[0].name = "clear";
	program_list[0].run = clear_fnct;
	program_list[1].name = "hello_world";
	program_list[1].run = hello_world_fnct;
	program_list[2].name = "count_down";
	program_list[2].run = count_down_fnct;
	program_list[3].name = "gfx_test";
	program_list[3].run = gfx_test_fnct;
}

void kernel_execute(uint8_t* command) {
	tm_print("kernel execution\n");
	tm_print(" >");
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

// ISR(USART0_RX_vect)
// {
// 	cli();
// 	KernelFunction(KF_UART);
// 	sei();
// }