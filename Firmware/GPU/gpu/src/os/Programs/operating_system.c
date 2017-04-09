/*
 * operating_system.c
 *
 * Created: 09.04.2017 02:44:32
 *  Author: Max Brand
 */ 

 #include "operating_system.h"
 
 uint8_t operating_system_fnct() {
	 Button serialListenerButton = { "Serial-Listener", 0, 1 };
	 kernel_touch_bind(process_touch);
	 disp_driver_set_graphics_mode();
	 gfx_clear();
	 button_draw(serialListenerButton, 0);
	 return 0;
 }
 
 void process_touch(vector_2 pos) {
	 if (button_get_status(serialListenerButton, pos)) {
		 kernel_run("serial_listener");
	 }
 }