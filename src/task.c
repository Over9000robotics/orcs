/**
 * @file task.c
 * @todo Input cinch option
 * @todo object check - at start of task program (actuator-board sensor check)
 * 		 if something found, go into second state machine
 */

#include <stdio.h>
#include <stdint.h>

#include "task.h"
#include "motion.h"
#include "config.h"
#include "color.h"
#include "mission.h"

static t_task robot_task;
static t_mission missions[MAX_MISSIONS];

/** this pointer needs to be incremented after every done mission */
static t_mission* mission_ptr = &missions[0];

void define_missions(void)
{
		missions[0].job = mission_go;
}

void task(void)
{
	missions[0].job(200, 200, 0);
}


void init_task(uint8_t option)
{
	char start_char = 0;
	
	switch(option)
	{
		case ENTER:
		{
			do
			{
				printf("PRESS ENTER TO START THE PROGRAM: ");
				start_char = getchar();
				if(start_char != '\n')
				{
					printf("ENTER WAS NOT PRESSED %c\n", start_char);
				}
			}while(start_char != '\n');
			break;
		}
		default:
		{
			print_red();
			printf("No start command \n");
			print_reset();
			break;
		}
	}
	missions_init(mission_ptr);
}

t_mission* task_get_mission_ptr(void)
{
	return mission_ptr;
}

void task3(void)
{
	printf("Task3 \n");
}

void task4(void)
{
	printf("Task4 \n");
}

void task5(void)
{
	printf("Task5 \n");
}

