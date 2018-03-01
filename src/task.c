/**
 * @file task.c
 * @todo Input cinch option
 * @todo object check - at start of task program (actuator-board sensor check)
 * 		 if something found, go into second state machine
 * @todo end_function
 * @todo at the end of obstacles, init their status to -> mission_free_to_use;
 */

#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "task.h"
#include "motion.h"
#include "config.h"
#include "color.h"
#include "mission.h"
#include "sensor.h"

static t_mission missions[MAX_MISSIONS];
static t_mission obstacles[MAX_MISSIONS];
static t_mission sensor_obstacles[MAX_MISSIONS];

/** this pointer needs to be incremented after every done mission */
t_mission* mission_ptr = &missions[0];
t_mission* obstacles_ptr = &obstacles[0];
t_mission* sensor_obstacle_ptr = &sensor_obstacles[0];
t_mission* mission_break_ptr = &missions[0];

void task(void)
{
	task_mission_check();
	mission_ptr->job();
}

void define_missions(void)
{
	missions[0].job = mission1;
	missions[1].job = mission2;
	missions[2].job = mission3;
}

/**
 * @note Last member always has to be obstacle_end!
 */
void define_obstacle_handling(void)
{
	obstacles[0].job = obstacle1;
	obstacles[1].job = obstacle2;
	obstacles[2].job = obstacle3;
	obstacles[3].job = obstacle_end;
}

void define_sensor_obstacle_handling(void)
{
	sensor_obstacles[0].job = sens_obstacle1;
	sensor_obstacles[1].job = sens_obstacle_end;
}

void sens_obstacle1(void)
{
	mission_wait(1000);
}

void sens_obstacle_end(void)
{
	print_yellow();
	printf("Mission sensor obstacle_end \n");
	print_reset();
	
	missions_init(&sensor_obstacles[0]);
	mission_ptr = mission_break_ptr;
}

void obstacle1(void)
{
	mission_wait(1000);
}

void obstacle2(void)
{
	mission_robot_stop();
}

void obstacle3(void)
{
	mission_wait(2000);
}

void obstacle_end(void)
{	
	print_yellow();
	printf("Mission obstacle_end \n");
	print_reset();
	
	missions_init(&obstacles[0]);
	mission_ptr = mission_break_ptr;
}

void mission1(void)
{
	mission_go(300, 300, 0, FORWARD);
}

void mission2(void)
{
	mission_go(1000, 0, 0, FORWARD);
}

void mission3(void)
{
	mission_go(1200, 0, 0, FORWARD);
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
	missions_init(&obstacles[0]);
}

t_mission* task_get_mission_ptr(void)
{
	return mission_ptr;
}

void task_mission_check(void)
{
	
	//motion_state = get_motion_state();
	
	if(mission_ptr->status == mission_done)
	{
		printf("mission_ptr++ \n");
		mission_ptr++;
	}

	if(mission_ptr->status == mission_interrupted)
	{
		print_yellow();
		printf("\nGo to missions obstacle \n");
		
		mission_ptr->status = mission_from_interrupted;
		mission_break_ptr = mission_ptr;
		mission_ptr = obstacles_ptr;
	}
	
	if(mission_ptr->status == mission_sens_interrupted)
	{
		print_yellow();
		printf("\nGo to sensors obstacle \n");
		
		mission_ptr->status = mission_from_interrupted;
		mission_break_ptr = mission_ptr;
		mission_ptr = sensor_obstacle_ptr;
	}
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


