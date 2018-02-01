#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <wiringPi.h> //for delay

#include "uart.h"
#include "color.h"
#include "motioncmd.h"
#include "motion.h"
#include "packet.h"
#include "config.h"
#include "task.h"

void init_main(void)
{
	tx_packets_init();
	uart_pkt_en(TRUE);
	set_side("GREEN");
	uart0_init(B57600);
}

int main(int argc, char* argv[])
{
	unsigned int last_motion_check = 0;
	
	init_main();
	if(!use_input_arguments(argc, argv))
	{
		return 0;
	}
	print_side();
	
	
	delay(100);
	
	//if communication doesn't work, end program
	motion_set_position(MOTION_START_X, MOTION_START_Y, MOTION_START_O);
	if(!motion_check())
		return 0;

	init_task(ENTER); /** @note Start options defined in config.h */
	
	//motion_forward(300, 0);
	delay(100);
	//motion_relative_rotate(-30);
	//motion_absolute_rotate(360);
	define_missions();
	while(1)
	{	

		if(millis() > last_motion_check + MOTION_REFRESH_INTERVAL)
		{
			last_motion_check = millis();
			motion_get_status_and_position();
		}
		motion_msg_status();
		
		task();
				
	}
	return 0;
}
