#include <stdio.h>
#include <stdint.h>

#include "mission.h"
#include "motion.h"
#include "task.h"

static t_mission* mission_ptr;
static t_motionState* motion_state;
static uint8_t started_moving_flag = 0;

void missions_init(t_mission* m)
{
	int i = 0;
	
	for(i=0; i<MAX_MISSIONS; i++)
	{
		m->status = mission_never_activated;
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

/**
 * 	@todo LATER MAKE SPEED AS INT_16, FOR DEBUGGING NEGATIVE INPUT NUMBERS
 * 	@todo object check - at start of task program (actuator-board sensor check)
 */
void mission_go(int x, int y, int speed)
{
	mission_ptr = task_get_mission_ptr();
	switch(mission_ptr->status)
	{
		//ako je ova funkcija ometena, skace se u drugu state masinu koja npr zaustavlja robota
		//ukoliko se robot oslobodi, opet se poziva ova funkcija, koja ce da proradi
		//i u slucaju da je stanje bilo 'interrupted' i stanje prelazi u 'in_progress'
		case mission_never_activated:
		{
			if(motion_check_speed(speed))
			{
				motion_set_speed(speed);
			}
			motion_move_to(x, y, 0, 0);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_interrupted:
		{
			if(motion_check_speed(speed))
			{
				motion_set_speed(speed);
			}
			motion_move_to(x, y, 0, 0);
			mission_ptr->status = mission_in_progress;
			break;
		}
		case mission_in_progress:
		{
			motion_state = get_motion_state();
			if(motion_state->state == STATUS_MOVING && started_moving_flag == 0)
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
			break;
		}
		default:
		{	
			printf("Mission_go: unknown state \n");
			break;
		}
	}
}

