/**
 * @file mission.c
 */
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "mission.h"
#include "motion.h"
#include "task.h"
#include "color.h"
#include "config.h"
#include "sensor.h"
#include "actuator.h"

static t_motionState* motion_state;
static uint8_t* sensor;
static uint8_t started_moving_flag = 0;
static unsigned int start_time = 0;
t_mission* mission_ptr;

//void wait_in_mission(uint32_t time)
//{
	
//}

void missions_init(t_mission* m)
{
	int i = 0;

	for(i=0; i<MAX_MISSIONS; i++)
	{
		(m+i)->status = mission_never_activated;

	}
}

void missions_print(t_mission* m)
{
	int i = 0;

	for(i=0; i<MAX_MISSIONS; i++)
	{
		printf("Mission[%d]_status: %d \n",i, m->status);
	}
}

void mission_collect_cubes(void)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission collect cubes \n");
			print_reset();
			
			//da li iskljuciti motore
			
			mission_ptr->status = mission_in_progress;
			break;
		}
		
		default:
		{
			break;
		}
	}
}

void mission_robot_stop(void)
{
	
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission robot soft stop \n");
			print_reset();

			motion_soft_stop();
			mission_ptr->status = mission_in_progress;
			break;
		}
		default:
		{
			break;
		}
	//mission_ptr->status = mission_done;
	}
}

void mission_wait(unsigned int time_ms)
{

	if (mission_ptr->status == mission_never_activated)
	{
		start_time = millis();

		print_yellow();
		printf("Mission wait %d ms \n", time_ms);
		//printf("Start time: %d \n", start_time);

		mission_ptr->status = mission_in_progress;
	}
	else if(mission_ptr->status == mission_in_progress)
	{
		unsigned int tdiff = 0;
		tdiff = millis() - start_time;
/*
		if((tdiff % 1000) == 0)
		{
			printf("\t%d seconds past \n", (tdiff / 1000));
		}
*/
		if (tdiff > time_ms)
		{
			print_yellow();
			printf("Mission wait %d ms ", time_ms);
			print_green();
			printf("done \n");
			print_reset();

			mission_ptr->status = mission_done;
		}
	}
}

void mission_switch_activate(void)
{
	uint8_t stuck_flag = 0;
	motion_state = get_motion_state();
	
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			started_moving_flag = 0;
			if(motion_state->state == STATUS_IDLE)
			{
				mission_ptr->status = mission_in_progress;
				motion_forward(-MOTION_SWITCH_DISTANCE, 0x32);
				print_yellow();
				printf("Mission switch activate: \n");
				print_reset();
				printf("Started");
			}
			break;
		}
		case mission_in_progress:
		{
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 0)
			{
				started_moving_flag = 1;
			}
			if(motion_state->state == STATUS_STUCK && started_moving_flag == 1)
			{
				started_moving_flag = 0;
				motion_soft_stop();
				stuck_flag = 1;
				//@todo implement wait
			}
			if(stuck_flag == 1)
			{
				stuck_flag = 0;
				motion_forward(MOTION_SWITCH_DISTANCE, 0x20);
			}
			if(started_moving_flag == 1 && motion_state->state == STATUS_IDLE)
			{
				started_moving_flag = 0;
				mission_ptr->status = mission_done;
			}
			break;
		}
		case mission_done:
		{
			print_yellow();
			printf("Mission switch activate ");
			print_blue();
			printf("DONE");
			print_reset();
			break;
		}
		default:
		{
			printf("Mission switch default");
			break;
		}
	}
}

/**
 * 	@todo LATER MAKE SPEED AS INT_16, FOR DEBUGGING NEGATIVE INPUT NUMBERS
 * 	@todo object check - at start of task program (actuator-board sensor check)
 */
void mission_forward(int distance, int speed)
{
	///sensor = get_sensors();
	motion_state = get_motion_state();
	
	switch(mission_ptr->status)
	{
		//ako je ova funkcija ometena, skace se u drugu state masinu koja npr zaustavlja robota
		//ukoliko se robot oslobodi, opet se poziva ova funkcija, koja ce da proradi
		//i u slucaju da je stanje bilo 'interrupted', prelazi u 'in_progress'
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission forward: ");
			print_reset();
			printf("Distance: %d, speed: %d \n", distance, speed);

			motion_speed_check_set(speed);

			motion_forward(distance, 0);
			mission_ptr->status = mission_in_progress;
			break;
		}

		case mission_from_interrupted:
		{
			sensor = get_sensors();
			
			printf("sensor front:    %x\n", *(sensor+FRONT));
			printf("sensor backward: %x\n", *(sensor+REAR));
			
			if((*(sensor+FRONT) == 0x00 && motion_state->moving_direction == FORWARD) || (*(sensor+REAR) == 0x00 && motion_state->moving_direction == BACKWARD))
			{
				print_yellow();
				printf("Mission from interrupted forward: ");
				print_reset();
				printf("%d mm, speed: %d \n", distance, speed);
				
				motion_speed_check_set(MOTION_SAFE_SPEED);
				motion_forward(distance, 0);
				mission_ptr->status = mission_in_progress;
			}
			break;
		}

		case mission_in_progress:
		{
			sensor = get_sensors();
			motion_state = get_motion_state();
			
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 0)
			{
				started_moving_flag = 1;
			}
			else if(motion_state->state == STATUS_IDLE && started_moving_flag == 1)
			{
				mission_ptr->status = mission_done;

				//prepare flag for next moving command
				started_moving_flag = 0;
			}

			else if(motion_state->state == STATUS_STUCK && started_moving_flag == 1)
			{
				started_moving_flag = 0; //get ready for next moving
				print_yellow();
				printf("Mission: ");
				print_reset();
				printf("Mission forward %d", distance);
				print_red();
				printf(" interrupted \n");
				mission_ptr->status = mission_interrupted;
			}
	
			//Front sensor handling
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 1 && *(sensor+FRONT) == 0xFF 
				&& motion_state->moving_direction == FORWARD)
			{
				print_yellow();
				printf("Mission forward: ");
				print_reset();
				printf("front sensor active \n");
				
				/** @todo analiza pozicije prepreke -> ako je dinamicka, zakoci! */
				motion_hard_stop();
				started_moving_flag = 0;
				mission_ptr->status = mission_sens_interrupted;
				
			}
			
			//Back sensor handling
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 1 && *(sensor+REAR) == 0xFF 
				&& motion_state->moving_direction == BACKWARD)
			{
				print_yellow();
				printf("Mission forward: ");
				print_reset();
				printf("rear sensor active \n");
				
				/** @todo analiza pozicije prepreke -> ako je dinamicka, zakoci! */
				motion_hard_stop();
				started_moving_flag = 0;
				mission_ptr->status = mission_sens_interrupted;			
			}
			
			break;
		}
		case mission_done:
		{
			print_yellow();
			printf("Mission forward DONE: ");
			print_reset();
			printf("%d mm, speed: %d \n", distance, speed);
			print_reset();
			break;
		}
		default:
		{
			printf("Mission_forward: unknown state \n");
			break;
		}
	}
}

void mission_brushless(uint8_t brushless_num, uint8_t procent)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission brushless: ");
			print_reset();
			printf("%d procent \n", procent);
			
			
			brushless_set_speed(brushless_num, procent);
			
			mission_ptr->status = mission_done;
			break;
		}
		
		default:
		{
			break;
		}
	}
}

/** 
 * AX-12a operating angle from 0 to 300 degrees 
 */
void mission_ax_angle(uint8_t id, uint32_t degrees)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission AX: ");
			print_reset();
			printf("%d degrees \n", degrees);
			
			ax_set_angle(id, degrees);
			mission_ptr->status = mission_done;
		}
		
		default:
		{
			break;
		}
	}
}

void mission_servo(uint8_t servo_num, uint8_t angle)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission servo: ");
			print_reset();
			printf("%d degrees \n", angle);
			
			if(angle > 180 || angle < 0)
			{
				print_red();
				printf("Servo: ");
				print_yellow();
				printf("invalid angle parameter! (%d) \n", angle);
				return;
			}

			servo_set_angle(servo_num, angle);
			mission_ptr->status = mission_done;
			break;
		}

		default:
		{
			break;
		}
	}
}


void mission_go(int x, int y, int speed, int direction)
{
	sensor = get_sensors();
	motion_state = get_motion_state();
	
	switch(mission_ptr->status)
	{
		//ako je ova funkcija ometena, skace se u drugu state masinu koja npr zaustavlja robota
		//ukoliko se robot oslobodi, opet se poziva ova funkcija, koja ce da proradi
		//i u slucaju da je stanje bilo 'interrupted', prelazi u 'in_progress'
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission go: ");
			print_reset();
			printf("(%d, %d), speed: %d \n", x, y, speed);

			motion_speed_check_set(speed);

			//motion_move_to(x, y, direction, 0);
			motion_turn_and_go(x, y, 0, direction);
			
			mission_ptr->status = mission_in_progress;
			break;
		}

		case mission_from_interrupted:
		{
			sensor = get_sensors();
			
			printf("sensor front: %x\n", *(sensor+FRONT));
			printf("sensor back:  %x\n", *(sensor+REAR));
			printf("direction : %d \n", direction);
			//delay(500);
			
			if((*(sensor+FRONT) == 0x00 && direction == FORWARD) || (*(sensor+REAR) == 0x00 && direction == BACKWARD))
			{
				print_yellow();
				printf("Mission from interrupted go: ");
				print_reset();
				printf("(%d, %d), speed: %d \n", x, y, speed);
				
				motion_speed_check_set(MOTION_SAFE_SPEED);
				motion_move_to(x, y, direction, 0);
				mission_ptr->status = mission_in_progress;
			}
			break;
		}

		case mission_in_progress:
		{
			//sensor = get_sensors();
			motion_state = get_motion_state();
			
			//printf("Mission go in progress \n");
			
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 0)
			{
				started_moving_flag = 1;
			}
			else if(motion_state->state == STATUS_IDLE && started_moving_flag == 1)
			{
				mission_ptr->status = mission_done;

				//prepare flag for next moving command
				started_moving_flag = 0;
			}

			else if(motion_state->state == STATUS_STUCK && started_moving_flag == 1)
			{
				started_moving_flag = 0; //get ready for next moving
				print_yellow();
				printf("Mission: ");
				print_reset();
				printf("Mission go (%d, %d)", x, y);
				print_red();
				printf(" interrupted \n");
				mission_ptr->status = mission_interrupted;
			}
	
			//Front sensor handling
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 1 && *(sensor+FRONT) == 0xFF 
				&& motion_state->moving_direction == FORWARD)
			{
				print_yellow();
				printf("Mission go: ");
				print_reset();
				printf("front sensor active \n");
				
				// @todo analiza pozicije prepreke -> ako je dinamicka, zakoci! 
				motion_hard_stop();
				started_moving_flag = 0;
				mission_ptr->status = mission_sens_interrupted;
				
			}
			
			//Back sensor handling
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 1 && *(sensor+REAR) == 0xFF 
				&& motion_state->moving_direction == BACKWARD)
			{
				print_yellow();
				printf("Mission go: ");
				print_reset();
				printf("rear sensor active \n");
				
				//  @todo analiza pozicije prepreke -> ako je dinamicka, zakoci!
				motion_hard_stop();
				started_moving_flag = 0;
				mission_ptr->status = mission_sens_interrupted;			
			}
			
			break;
		}
		case mission_done:
		{
			print_yellow();
			printf("Mission go DONE: ");
			print_reset();
			printf("(%d, %d), speed: %d \n", x, y, speed);
			print_reset();
			break;
		}
		default:
		{
			printf("Mission_go: unknown state \n");
			//while(1);
			break;
		}
	}
}

void mission_rotate_abs(int angle)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission absolute rotate: ");
			print_reset();
			printf("Angle: %d \n", angle);

			motion_absolute_rotate(angle);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_interrupted:
		{
			print_yellow();
			printf("Mission absolute rotate from interrupted: ");
			print_reset();
			printf("%d \n", angle);

			motion_absolute_rotate(angle);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_in_progress:
		{
			motion_state = get_motion_state();

			if(motion_state->state == STATUS_ROTATING && started_moving_flag == 0)
			{
				started_moving_flag = 1;
			}

			if(motion_state->state == STATUS_IDLE && started_moving_flag == 1)
			{
				mission_ptr->status = mission_done;

				//prepare flag for next moving command
				started_moving_flag = 0;
			}
			break;
		}
		case mission_done:
		{
			print_yellow();
			printf("Mission rotate absolute DONE: ");
			print_reset();
			printf("Angle: %d \n", angle);
			print_reset();
			break;
		}
		default:
		{
			printf("Mission rotate abs: unknown state \n");
			break;
		}
	}
}

void mission_rotate_rel(int angle)
{
	switch(mission_ptr->status)
	{
		case mission_never_activated:
		{
			print_yellow();
			printf("Mission relative rotate: ");
			print_reset();
			printf("Angle: %d \n", angle);

			motion_relative_rotate(angle);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_interrupted:
		{
			print_yellow();
			printf("Mission absolute rotate from interrupted: ");
			print_reset();
			printf("%d \n", angle);
			motion_relative_rotate(angle);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_in_progress:
		{
			motion_state = get_motion_state();
			if(motion_state->state == STATUS_ROTATING && started_moving_flag == 0)
			{
				started_moving_flag = 1;
			}
			if(motion_state->state == STATUS_IDLE && started_moving_flag == 1)
			{
				mission_ptr->status = mission_done;

				//prepare flag for next moving command
				started_moving_flag = 0;
			}
			break;
		}
		case mission_done:
		{
			print_yellow();
			printf("Mission rotate absolute DONE: ");
			print_reset();
			printf("Angle: %d \n", angle);
			print_reset();
			break;
		}
		default:
		{
			printf("Mission rotate abs: unknown state \n");
			break;
		}
	}
}
