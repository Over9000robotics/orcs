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
#include "sensor.h"

void init_main(void)
{
	int flush_success = 0;
	
	tx_packets_init();
	uart_pkt_en(TRUE);
	set_side("GREEN");
	uart0_init(B57600);
	uart1_init(B57600); //using uart1 is alternate option
	
	flush_success = uart0_input_flush();
	flush_success = uart1_input_flush();
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
	
	//motion_soft_stop();

	delay(100);

	define_missions();
	define_obstacle_handling();
	define_sensor_obstacle_handling();
	
	while(1)
	{	
		
		if(millis() > last_motion_check + MOTION_REFRESH_INTERVAL)
		{
			last_motion_check = millis();
			motion_get_status_and_position();
			sensor_ask_for_status();
		}
		update_sensor_status();
		motion_msg_status();
		
		task();	

	}
	return 0;
}
