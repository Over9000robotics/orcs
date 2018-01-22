#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <termios.h>
#include <wiringPi.h> //for delay

#include "uart.h"
#include "color.h"
#include "motioncmd.h"
#include "motion.h"
#include "packet.h"

//TRUE  = 1
//FALSE = 0

void init_main(void)
{
	tx_packets_init();
	uart_pkt_en(TRUE);
}

int main(void)
{
	init_main();
	uart0_init(B57600);
	
	packet_prepare(MOTION_MOVE_TO);
	packet_put_word(500);
	packet_put_word(500);
	packet_put_byte(0);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
/*	
	packet_prepare(MOTION_UNSTUCK);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
	
	packet_prepare(MOTION_GET_STATUS_AND_POSITION);
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/
/*
	packet_prepare(MOTION_MOVE_FORWARD);
	packet_put_word(150); //length
	packet_put_byte(0x0);	 //end speed
	packet_end();
	print_packet(get_selected_tx_packet(0));
	delay(100);
*/

	while(1)
	{
		motion_msg_status();		
		
	}
	return 0;
}
