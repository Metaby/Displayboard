/*
 * HardwareInterface.c
 *
 * Created: 23.11.2012 20:54:42
 *  Author: Max
 */
#include "HardwareInterface.h"

void display_initialize()
{
	DDRA = (1 << PORTA0) | (1 << PORTA1) | (1 << PORTA4);
	DDRB = (1 << PORTB4) | (1 << PORTB5) | (1 << PORTB7);
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);
	display_enable(0);
	display_send_byte(0xF1);
	display_send_byte(0x67);
	display_send_byte(0xC0);
	display_send_byte(0x40);
	display_send_byte(0x50);
	display_send_byte(0x2B);
	display_send_byte(0xEB);
	display_send_byte(0x81);
	display_send_byte(0x69);
	display_send_byte(0x89);
	display_send_byte(0xAF);
	display_send_byte(0xA4);
}

void display_send_byte(uint8_t Data)
{
	SPDR = Data;
	while(!(SPSR & (1 << SPIF)));
}

void display_enable(uint8_t CD)
{
	PORTA = (0 << PORTA0) | (CD << PORTA1);
}

void display_disable()
{
	PORTA = (1 << PORTA0);
}

void display_select_column(uint8_t Column)
{
	display_enable(0);
	display_send_byte(0x0 | (Column & 0x0F));
	Column = (Column >> 4);
	display_send_byte(0x10 | (Column & 0x0F));
}

void display_select_page(uint8_t Page)
{
	display_enable(0);
	display_send_byte(0x60 | (Page & 0x1F));
}

void display_clear()
{
	display_select_column(0);
	display_select_page(0);
	display_enable(1);
	for (uint8_t i = 0; i < 200; i++)
	{
		for (uint8_t j = 0; j < 32; j++)
		{
			display_send_byte(0x00);
		}
	}
}

void display_send_buffer()
{
	display_select_column(0);
	display_select_page(0);
	display_enable(1);
	for (uint8_t y = 0; y < 26; y++)
	{
		if (display_row_flag[y] == 1)
		{
			for (uint8_t x = 0; x < 160; x++)
			{
				display_send_byte(display_buffer[x][y]);
			}
			display_row_flag[y] = 0;
		}
		else
		{
			display_enable(0);
			display_select_page(y + 1);
			display_enable(1);
		}
	}
}

vector_2 display_get_touch()
{	
	i2c_start_wait(0x0C + TW_WRITE);
	i2c_write(0x00);
	i2c_stop();
	i2c_start_wait(0x0C + TW_READ);
	vector_2 position = {i2c_readAck(), i2c_readNak()};
	i2c_stop();
	return position;
}