/*
 * TPC.c
 *
 * Created: 09.10.2012 11:06:12
 *  Author: Max
 */
#define EnableRapidSending 0
#define F_CPU 20000000UL
#define PORTA0 0
#define PORTA1 1
#define PORTA2 2
#define PORTA3 3
#define PORTA4 4
#define PORTA5 5
#define PORTA6 6
#define PORTA7 7
#define SDA_PORT        PA6
#define SCL_PORT        PA4
#define SDA_PIN         PINA6
#define SCL_PIN         PINA4
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usiTwiSlave.h"

volatile uint8_t lastX = 0;
volatile uint8_t lastY = 0;

uint16_t readADC(uint8_t Channel, uint8_t Resolution)
{
	uint8_t i;
	uint32_t result = 0;
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	ADMUX = Channel;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	for(i=0; i < Resolution; i++) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		result += ADCW;
	}
	ADCSRA &= ~(1<<ADEN);
	result /= Resolution;
	return result;
}

void Sleep (uint16_t ms)
{
	for(; ms > 0; ms--) _delay_ms(1);
}

uint8_t SampleY()
{
	DDRA |= (1 << PORTA1) | (1 << PORTA0);
	PORTA |= (1 << PORTA0) | (1 << PORTA3);
	PORTA &= ~(1 << PORTA1);
	uint16_t minY = 400;//360;
	uint16_t maxY = 815;//815;
	return (uint8_t)((readADC(2, 10) - minY) * (104.0 / (maxY - minY)));
}

uint8_t SampleX()
{
	DDRA |= (1 << PORTA1);
	PORTA |= (1 << PORTA1) | (1 << PORTA2);
	PORTA &= ~(1 << PORTA0);
	uint16_t minX = 215;//175;
	uint16_t maxX = 830;//830;
	return (uint8_t)((readADC(3, 10) - minX) * (160.0 / (maxX - minX)));
}

void Initialize()
{
	usiTwiSlaveInit(0x0C);
	TCCR1B = (1 << CS11);
	TIMSK1 = (1 << TOIE1);
	TCNT1H = 0xAF;
	TCNT1L = 0xFF;
	Sleep(1000);
	sei();
}

int main(void)
{
	Initialize();
    while(1);
}

ISR(TIM1_OVF_vect)
{
	TIMSK1 = 0x00;
	TCNT1H = 0xAF;
	TCNT1L = 0xFF;
	uint8_t x = 160 - SampleX();
	uint8_t y = 104 - SampleY();
	uint8_t send = 0;
	if (lastX != x)
	{
		lastX = x;
		send = 1;
	}		
	if (lastY != y)
	{
		lastY = y;
		send = 1;
	}		
	txbuffer[0] = lastX;
	txbuffer[1] = lastY;
	if (send == 1 || EnableRapidSending)
	{
		DDRA |= (1 << PORTA7);
		PORTA &= ~(1 << PORTA7);
		PORTA |= (1 << PORTA7);
	}
	TIMSK1 = (1 << TOIE1);
}