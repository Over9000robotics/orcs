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
	unsigned int last_motion_check = 0;
	
	if(!use_input_arguments(argc, argv))
	{
		return 0;
	}
	print_side();
	
	init_main();
	
	delay(100);
	
	//if communication doesn't work, end program
	if(!motion_check())
		return 0;
	delay(10);
	
	/** 
	 * @todo this has to be moved to init_task
	 */
	char start_char = 0;
	printf("PRESS ENTER TO START THE PROGRAM");
	do{
		scanf("%c", &start_char);
		if(start_char != '\n');
		printf("ENTER WAS NOT PRESSED %c\n", start_char);
	}while(start_char != '\n');
	printf("\n");
	
	motion_forward(300, 0);

	motion_get_status_and_position();
	delay(100);
	
	//motion_relative_rotate(-30);
	//motion_absolute_rotate(360);
	
	while(1)
	{
		if(millis() > last_motion_check + MOTION_REFRESH_INTERVAL)
		{
			last_motion_check = millis();
			motion_get_status_and_position();
		}
		motion_msg_status();		
	}
	return 0;
}
