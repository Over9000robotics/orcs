/**
 * @file motion.c
 * @todo what if receive command 'F'
 * @todo motion forward - which character?
 */
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "motion.h"
#include "motioncmd.h"
#include "uart.h"
#include "packet.h"
#include "color.h"
#include "config.h"


static uint8_t command = 0;
static t_packet  rx_pkt;
static t_packet* rx_pkt_ptr = &rx_pkt;
static t_motionState motion_state;
unsigned int time_start_millis = 0;
unsigned int time_end_millis = 0;

void motion_set_position(int16_t x, int16_t y, int16_t o)
{
	packet_prepare(MOTION_SET_POSITION_AND_ORIENTATION);
	packet_put_word(x);
	packet_put_word(y);
	packet_put_word(o);
	packet_end();
}

uint8_t motion_check(void)
{
	packet_prepare(MOTION_GET_STATUS_AND_POSITION);
	packet_end();
	
	time_start_millis = millis();
	while(millis() - time_start_millis < MOTION_RESPONSE_MS && command != 'P')
	{
		if(motion_msg_status() == 1)
		{
			print_green();
			printf("Motion: ");
			print_blue();
			printf("communication OK! \n");
			print_yellow();
			printf("Communication time: %d \n", millis() - time_start_millis);
			print_reset();
			if(motion_state.x == MOTION_START_X && motion_state.y == MOTION_START_Y && motion_state.angle == MOTION_START_O)
				return 1;
			else
			{
				print_red();
				printf("Start position init failed \n");
				print_reset();
				return 0;
			}
		}
	}
	print_red();
	printf("Uart: no communication! \n");
	print_red();
	printf("Uart: Check power supply (total-stop), uart cable, or baud-rate \n");
	print_reset();
	return 0;
}

uint8_t motion_msg_status(void)
{
	rx_pkt_ptr = try_read_packet();
	if (rx_pkt_ptr != 0)
	{
		print_rx_packet(rx_pkt_ptr);
		command = rx_pkt_ptr -> type;
		printf("command: %c \n", command);
		switch(command)
		{
			case MOTION_GET_STATUS_AND_POSITION:	//'P'
			{
				if(rx_pkt_ptr -> size == 9)
				{
					set_status_and_position();
					return 1;
				}
				break;
			}
			
			default: 
			{
				print_red();
				printf("Command '%c' is unknown! \n", command);
				print_reset();
				break;
			}
		}
	}
	return 0;		
}

void set_status_and_position(void)
{	
	motion_state.state = rx_pkt_ptr -> data[0];
	
	motion_state.x = rx_pkt_ptr -> data[1];
	motion_state.x = (motion_state.x << 8) | (rx_pkt_ptr -> data[2]);

	motion_state.y = rx_pkt_ptr -> data[3];
	motion_state.y = (motion_state.y << 8) | (rx_pkt_ptr -> data[4]);
	
	motion_state.angle = rx_pkt_ptr -> data[5];
	motion_state.angle = (motion_state.angle << 8) | (rx_pkt_ptr -> data[6]);
	
	motion_state.current_speed = rx_pkt_ptr -> data[7];
	motion_state.current_speed = (motion_state.current_speed << 8) | (rx_pkt_ptr -> data[8]);

	print_status_and_position();
}

void print_status_and_position(void)
{
	print_green();
	printf("Motion: ");
	
	print_blue();
	printf("Position ");
	
	print_reset();
	printf("(%d, %d) ", motion_state.x, motion_state.y);
	
	print_blue();
	printf("Angle ");
	print_reset();
	printf("%d \n",motion_state.angle);
	
	print_blue();
	printf("\tCurrent speed: ");
	print_reset();
	printf("%d \n",motion_state.current_speed);
	
	motion_print_state();
}

void motion_print_state(void)
{
	print_blue();
	printf("\tStatus: ");
	print_reset();
	switch(motion_state.state)
	{
		case STATUS_IDLE:
		{
			printf("IDLE \n");
			break;
		}
		case STATUS_MOVING:
		{
			printf("MOVING \n");
			break;
		}
		case STATUS_ROTATING:
		{
			printf("ROTATING \n");
			break;
		}
		case STATUS_STUCK:
		{
			printf("STUCK \n");
			break;
		}
		case STATUS_ERROR:
		{
			print_red();
			printf("STATE ERROR \n");
			print_reset();
			break;
		}
		default:
		{
			print_red();
			printf("Unknown state character \n");
			print_reset();
		}
	}
}
