/*
 * serial_listener.c
 *
 * Created: 08.04.2017 13:43:10
 *  Author: Max Brand
 */ 

#include "serial_listener.h"

uint8_t serial_listener_fnct() {
	tm_clear();
	uint8_t uart_input[SERIAL_BUFFER_SIZE];
	uint8_t uart_input_position = 0;
	for (uint8_t i = 0; i < SERIAL_BUFFER_SIZE; i++) {
		uart_input[i] = 0x00;
	}
	while (1) {
		while (!(UCSR0A & (1 << RXC0)));
		uint8_t ch = UDR0;
		if (ch == 0x0A) {
			tm_print(uart_input);
			tm_print("\n");
			uart_input_position = 0;
			for (uint8_t i = 0; i < SERIAL_BUFFER_SIZE; i++) {
				uart_input[i] = 0x00;
			}
		} else {
			uart_input[uart_input_position++] = ch;
		}
	};
	return 0;
}