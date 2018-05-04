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
static t_motionState motion_old_state;
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

void motion_get_status_and_position(void)
{
	packet_prepare(MOTION_GET_STATUS_AND_POSITION);
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
		command = rx_pkt_ptr -> type;

#ifdef DEBUG
		printf("command: %c \n", command);
		print_rx_packet(rx_pkt_ptr);
#endif

		switch(command)
		{
			case MOTION_GET_STATUS_AND_POSITION:	//'P'
			{
				if(rx_pkt_ptr -> size == 7)
				{
					set_status_and_position();
					return 1;
				}
				break;
			}

			case 'A':
			{
				if(rx_pkt_ptr -> size == 1)
				{
					packet_verify(rx_pkt_ptr);
				}
				break;
			}

			//ignore 'F' messages, that is motion debug character
			case 'F':
			{
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

void motion_set_speed(uint8_t speed)
{
	if(speed < 0 || speed > 255)
	{
		print_red();
		printf("\nIncorrect speed parameter\n");
		print_reset();
		return;
	}
	packet_prepare(MOTION_SET_SPEED);
	packet_put_byte(speed);
	packet_end();

	motion_state.max_speed = speed;
}

void motion_set_rotating_speed(uint8_t speed)
{
	if(speed < 0 || speed > 255)
	{
		print_red();
		printf("\nIncorrect speed parameter\n");
		print_reset();
		return;
	}
	packet_prepare(MOTION_SET_ROTATION_SPEED);
	packet_put_byte(speed);
	packet_put_byte(0);
	packet_end();

	motion_state.max_rot_speed = speed;
}

void set_status_and_position(void)
{
	motion_old_state = motion_state;

	motion_state.state = rx_pkt_ptr -> data[0];

	motion_state.x = rx_pkt_ptr -> data[1];
	motion_state.x = (motion_state.x << 8) | (rx_pkt_ptr -> data[2]);

	motion_state.y = rx_pkt_ptr -> data[3];
	motion_state.y = (motion_state.y << 8) | (rx_pkt_ptr -> data[4]);

	motion_state.angle = rx_pkt_ptr -> data[5];
	motion_state.angle = (motion_state.angle << 8) | (rx_pkt_ptr -> data[6]);

	motion_state.current_speed = rx_pkt_ptr -> data[7];
	motion_state.current_speed = (motion_state.current_speed << 8) | (rx_pkt_ptr -> data[8]);


	if(motion_old_state.state != motion_state.state ||
		motion_old_state.x != motion_state.x ||
		motion_old_state.y != motion_state.y ||
		motion_old_state.angle != motion_state.angle ||
		motion_old_state.current_speed != motion_state.current_speed)
	{
			print_status_and_position();
	}
}
		/*
		direction:
				0 - pick smallest rotation (do not use)
				1 - forward
			   -1 - backward
		*/
void motion_move_to(int16_t x, int16_t y, int8_t direction, int16_t radius)
{
	packet_prepare(MOTION_MOVE_TO);
	packet_put_word(x);
	packet_put_word(y);
	packet_put_byte(direction);
	if(radius != 0x7fff)
	{
		packet_put_word(radius);
	}
	packet_end();
	
	motion_state.moving_direction = direction;
}

void motion_turn_and_go(int16_t x, int16_t y, uint8_t end_speed, int8_t direction)
{
	packet_prepare(MOTION_TURN_AND_GO);
	packet_put_word(x);
	packet_put_word(y);
	packet_put_byte(end_speed);
	packet_put_byte(direction);
	packet_end();
	
	motion_state.moving_direction = direction;
}

void motion_forward(int16_t length, uint8_t end_speed)
{
	packet_prepare(MOTION_FORWARD);
	packet_put_word(length);
	packet_put_byte(end_speed);
	packet_end();
	
	if(length > 0)
		motion_state.moving_direction = FORWARD;
	
	else if(length < 0)
		motion_state.moving_direction = BACKWARD;
}

void motion_absolute_rotate(int16_t degrees)
{
	packet_prepare(MOTION_ABSOLUTE_ROTATE);
	packet_put_word(degrees);
	packet_end();
}

void motion_relative_rotate(int16_t degrees)
{
	packet_prepare(MOTION_RELATIVE_ROTATE);
	packet_put_word(degrees);
	packet_end();
}

void motion_hard_stop(void)
{
	packet_prepare(MOTION_HARD_STOP);
	packet_end();
}

void motion_soft_stop(void)
{
	packet_prepare(MOTION_SOFT_STOP);
	packet_end();
}

void motion_reset_driver(void)
{
	packet_prepare(MOTION_RESET_DRIVER);
	packet_end();
}

void motion_unstuck(void)
{
	packet_prepare(MOTION_UNSTUCK);
	packet_end();
}

void motion_speed_check_set(int speed)
{
	if(speed == 0)
	{
		motion_set_speed(MOTION_NORMAL_SPEED);
	}

	else if(speed > 0 && speed < 255)
	{
		motion_set_speed(speed);
	}

	else if (speed < 0 || speed > 255)
	{
		motion_set_speed(MOTION_NORMAL_SPEED);
		print_red();
		printf("Motion: robot speed is MIN %d and MAX %d \n", MOTION_MIN_SPEED, MOTION_MAX_SPEED);
		printf("\tYou typed %d ", speed);
		print_reset();
		return;
	}

}

void print_status_and_position(void)
{
	print_green();
	printf("Motion: ");

	print_blue();
	printf("Position: ");

	print_reset();
	printf("(%d, %d) ", motion_state.x, motion_state.y);

	print_blue();
	printf("Angle: ");
	print_reset();
	printf("%d ",motion_state.angle);
/*
	print_blue();
	printf("\tCurrent speed: ");
	print_reset();
	printf("%d \n",motion_state.current_speed);
*/
	motion_print_state();
}

t_motionState* get_motion_state(void)
{
	return &motion_state;
}

void motion_print_state(void)
{
	print_blue();
	printf(" Status: ");
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
