/*
 * text_test.c
 *
 * Created: 01.02.2016 00:05:57
 *  Author: Max
 */

 #include "text_test.h"
 
 uint8_t text_test_fnct() {
	 disp_driver_set_text_mode();
	 tm_clear();
	 tm_print("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	 tm_print("abcdefghijklmnopqrstuvwxyz\n");
	 tm_print("0123456789.,;:_-+*/\\%$?\n");
	 system_sleep_ms(5000);
	 return 0;
 }