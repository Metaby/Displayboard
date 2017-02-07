/*
 * Displayboard_Funktionstest.c
 *
 * Created: 09.10.2012 00:08:43
 *  Author: Max
 */ 
#include "GPU.h"

int main(void)
{
	hardware_initialize();
	disp_driver_initialize();
	kernel_initialize();
	tm_print("initializing done\n");
	tm_print("starting kernel\n");
	kernel_run();
}

ISR(INT2_vect)
{
	TIMSK1 = 0x00;
	TCNT1H = 0xF6;
	TCNT1L = 0x3B;
	vector_2 position = display_get_touch();
	// Touch erkannt
	TIMSK1 = (1 << TOIE1);
}

void hardware_initialize()
{
	// Draw-Timer
	TCCR1B = (1 << CS01);
	TIMSK1 = (1 << TOIE1);
	TCNT1H = 0xF6;
	TCNT1L = 0x3B;
	// 1s Timer
	TCCR3B = (1 << CS30) | (1 << CS32);
	//	TIMSK3 = (1 << TOIE3);
	// 	TCNT3H = 0xEA;//E2;
	// 	TCNT3L = 0x60;//B3;
	// Touch-Interrupt
	EICRA = 0x10;
	EIMSK = 0x04;
	EIFR = 0x04;
	// Brightness-PWM-Timer
	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << WGM02) | (1 << COM0B1);
	TCCR0B = (1 << CS02) | (1 << CS00);
	OCR0B = brightness_table[brightness];
	// UART
	DDRD = (1 << PORTD0);
	UBRR0 = 0x81;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	//UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	// i2c
	i2c_init();
	sei();
}