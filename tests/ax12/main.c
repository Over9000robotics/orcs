#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <wiringPi.h> //for delay and millis

#include "uart.h"
#include "color.h"
//#include "motioncmd.h"
//#include "motion.h"
//#include "packet.h"
#include "config.h"
//#include "task.h"
//#include "sensor.h"
#include "actuator.h"

static unsigned long int start_time = 0;

void init_main(void)
{
	int flush_success = 0;
	
//	tx_packets_init();
//	uart_pkt_en(TRUE);
	set_side("GREEN");
	uart0_init(B57600);
	uart1_init(B115200); //using uart1 is alternate option
	
	flush_success = uart0_input_flush();
	flush_success = uart1_input_flush();
	
//	brushless_set_speed(BR_HEAD, 0);
//	brushless_set_speed(BR_SECONDARY, 0);
}

int main(int argc, char* argv[])
{
	
	while(1)
	{	
		axpacket_prepare(1, INST_WRITE, P_GOAL_POSITION_L);
		axpacket_put_word(1000);
		axpacket_end();
		
		delay(2000);
		
		axpacket_prepare(1, INST_WRITE, P_GOAL_POSITION_L);
		axpacket_put_word(100);
		axpacket_end();
		
		delay(2000);
	}
	return 0;
}
