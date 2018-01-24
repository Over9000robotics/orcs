/**
 * @file motion.c
 * @todo what if receive command 'F'
 * @todo motion forward - which character?
 */
#include <stdio.h>
#include <stdint.h>

#include "motion.h"
#include "motioncmd.h"
#include "uart.h"
#include "packet.h"
#include "color.h"


static uint8_t command = 0;
static t_packet  rx_pkt;
static t_packet* rx_pkt_ptr = &rx_pkt;
static t_motionState motion_state;

void motion_msg_status(void)
{
	rx_pkt_ptr = try_read_packet();
	if (rx_pkt_ptr != 0)
	{
		command = rx_pkt_ptr -> type;
		switch(command)
		{
			case MOTION_GET_STATUS_AND_POSITION:	// 'P'
			{
				set_status_and_position();
				break;
			}
			
			default: 
			{
/*				print_red();
				printf("Command '%c' is unknown! \n", command);
				print_reset();
*/
				break;
			}
		}
	}
	else 
		return;
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
	printf("Motion: \n");
	
	motion_print_state();
	
	print_blue();
	printf("\tX: ");
	print_reset();
	printf("int %d, hex 0x%x \n",motion_state.x, motion_state.x);

	print_blue();
	printf("\tY: ");
	print_reset();
	printf("int %d, hex 0x%x \n",motion_state.y, motion_state.y);
	
	print_blue();
	printf("\tAngle: ");
	print_reset();
	printf("int %d, hex 0x%x \n",motion_state.angle, motion_state.angle);
	
	print_blue();
	printf("\tCurrent speed: ");
	print_reset();
	printf("int %d, hex 0x%x \n",motion_state.current_speed, motion_state.current_speed);
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
