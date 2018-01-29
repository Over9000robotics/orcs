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


void init_main(void)
{
	tx_packets_init();
	uart_pkt_en(TRUE);
	set_side("GREEN");
	uart0_init(B57600);
	motion_set_position(MOTION_START_X, MOTION_START_Y, MOTION_START_O);
}

uint8_t use_input_arguments(int argc, char* argv[])
{
	int i;
	
	for(i=1; i < argc; i++) 
	{
		if(!strcmp(argv[i], "--side")) 
		{	
			if(set_side(argv[i+1]) == 0)
			{
				return 0;
			}
			i++;
		}
	}
	return 1;
}
int main(int argc, char* argv[])
{

	init_main();
	if(!use_input_arguments(argc, argv))
	{
		return 0;
	}
	print_side();

/*
	packet_prepare(MOTION_MOVE_TO);
	packet_put_word(-50);
	packet_put_word(0);
	packet_put_byte(-1);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/

/*
	packet_prepare(MOTION_UNSTUCK);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/
/*
	packet_prepare(MOTION_GET_STATUS_AND_POSITION);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/
/*
	packet_prepare(MOTION_FORWARD);
	packet_put_word(300); //length
	packet_put_byte(0x0);	 //end speed
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/
	//if communication doesn't work
	if(!motion_check())
		return 0;
		
	while(1)
	{
		motion_msg_status();		
	}
	return 0;
}
