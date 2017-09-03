/*
 * Kernel.h
 *
 * Created: 29.01.2016 23:37:58
 *  Author: Max
 */ 
#ifndef KERNEL_H_
#define KERNEL_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "HAL/HardwareInterface.h"
#include "GraphicsDriver/Display.h"
#include "GUI/Button.h"
#include "Program.h"

#define KF_UART 0

volatile void_fnct_void kernel_timer_fnct;
volatile void_fnct_vector_2 kernel_touch_fnct;
volatile uint16_t kernel_timer_count;

volatile uint8_t* auto_run;

volatile Program program_list[6];
volatile uint8_t input[8][20];
volatile uint8_t inputPos;
volatile uint8_t inputLength;

volatile uint8_t program_list_size;
volatile uint8_t linePos;
volatile uint8_t lines;

volatile uint8_t last_call_status;

volatile uint8_t* cmd;
volatile uint8_t kernel_mode;
volatile Button serial_listener_button;
volatile Button text_test_button;
volatile Button gfx_test_button;

void kernel_initialize();
void kernel_run();
void kernel_handle_uart();
void kernel_execute(uint8_t* command);
void kernel_timer_set(uint8_t time_in_ms);
void kernel_timer_enable();
void kernel_timer_disable();
void kernel_timer_bind(void_fnct_void fnct);
void kernel_touch_bind(void_fnct_void fnct);

#endif /* KERNEL_H_ */