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
#include "actuator.h"

#ifdef TASK_ZOKI_ORANGE

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
		//Karakteri!
		//#define COLLECT_CUBES '6'
		//#define EJECT_CUBES '7'
		
		uint8_t index = 0;
		
		missions[index++].job = mission_nuliraj_hv;
		missions[index++].job = mission_podigni_sis;
		missions[index++].job = mission1;
		missions[index++].job = mission2;
		missions[index++].job = mission3;
		missions[index++].job = mission4;
		missions[index++].job = mission5;	//kupi 1. kockice
		missions[index++].job = mission6; 
		missions[index++].job = mission7;
		missions[index++].job = mission8; //istovari 1. kockice
		missions[index++].job = mission9;
		
		missions[index++].job = mission10; //backward
		missions[index++].job = mission10a; //nuliraj
		missions[index++].job = mission10b; //sacekaj
		missions[index++].job = mission10c;//podigni sisaljke
		
		missions[index++].job = mission11;
		missions[index++].job = mission12;
		missions[index++].job = mission12a;
		missions[index++].job = mission13; //pokupi 2. kockice
		missions[index++].job = mission14;
		missions[index++].job = mission15; 
		missions[index++].job = mission16; //istovari 2. kockice
		missions[index++].job = mission17;
		
		missions[index++].job = mission18; //bacward
		missions[index++].job = mission18a; //nuliraj
		missions[index++].job = mission18b; //podigni hvataljke
		
		missions[index++].job = mission19;
		missions[index++].job = mission_19a; 
		missions[index++].job = mission20; 
		missions[index++].job = mission21; //pokupi 3. kocice
		missions[index++].job = mission22;  
		missions[index++].job = mission23;
		missions[index++].job = mission24; //istovari 3. kockice
		missions[index++].job = mission25;  
		//missions[index++].job = mission26;
		//missions[index++].job = mission27;
	
		missions[index++].job = last_mission;
	}

	/**
	 * @note Last member always has to be obstacle_end!
	 */
	void define_obstacle_handling(void)
	{
		uint8_t index = 0;
		
		obstacles[index++].job = obstacle1;
		obstacles[index++].job = obstacle2;
		obstacles[index++].job = obstacle3;
		obstacles[index++].job = obstacle_end;
	}

	void define_sensor_obstacle_handling(void)
	{
		sensor_obstacles[0].job = sens_obstacle1;
		sensor_obstacles[1].job = sens_obstacle_end;
	}
	
	void mission_nuliraj_hv(void)
	{
		//uvek prva misija
		mission_nuliraj_hvataljke();
	}
	
	void mission_podigni_sis(void)
	{
		mission_podigni_sisaljke();
	}
	
	void mission1(void)
	{
		//mission_robot_stop();
		mission_go(-683, -467,0x60,FORWARD);
	}

	void mission2(void)
	{
		//mission_rotate_abs(0);
		//mission_go(1050, 0, 0x32, FORWARD);
		mission_wait(200);
		//servo_set_angle(SERVO1, 180);
	}

	void mission3(void)
	{
		mission_rotate_abs(90);
	}
	void mission4(void)
	{
		//mission_go(-675, -289, 0x32, FORWARD);
		mission_forward(345, 0x32);
	}

	void mission5(void)
	{
		/*
		 * Ovde treba da pokupi 1. kockice
		 * */
		 mission_collect_cubes();
	}
	void mission6(void)
	{
		//COLLECT_TIME definisan u task.h
		mission_wait(COLLECT_TIME); //koliko vremena skuplja
	}

	void mission7(void)
	{
		//Istovar 1 - tacka
		mission_go(-610, -460, 0x50, FORWARD);
	}
	void mission8(void)
	{
		//istovari 1. kocke
		mission_eject_cubes();
	}
	void mission9(void)
	{
		mission_wait(EJECT_TIME);
	}
	void mission10(void)
	{
		mission_forward(-200, 0x50);
	}
	
	void mission10a(void)
	{
		mission_nuliraj_hvataljke();
	}
	
	void mission10b(void)
	{
		mission_wait(5000);
	}
	
	void mission10c(void)
	{
		mission_podigni_sisaljke();
	}
	
	void mission11(void)
	{
		//Idi na druge - dalji prilaz
		mission_go(-1220, 190, 0x50, FORWARD);
	}
	void mission12(void)
	{
		mission_rotate_abs(90);
	}
	void mission12a(void)
	{
		//pridji skroz kockama 2
		mission_forward(170, 0x32);
	}
	void mission13(void)
	{
			//pokupi kocke 2
		mission_collect_cubes();
	}
	void mission14(void)
	{
		mission_wait(COLLECT_TIME);
	}
	void mission15(void)
	{
		//pridji na istovar kocki 2
		mission_go(-1100, -460, 0x50, FORWARD);
	}
	void mission16(void)
	{
		//istovari kocke 2
		mission_eject_cubes();
	}
	void mission17(void)
	{
		//sacekaj da se istovare
		mission_wait(EJECT_TIME);
	}
	void mission18(void)
	{
		//odmakni se
		mission_forward(-100, 0x50);
	}
	
	void mission18a(void)
	{
		mission_nuliraj_hvataljke();
	}
	void mission18b(void)
	{
		mission_podigni_sisaljke();
	}
	
	void mission19(void)
	{
		//kockice 3, prilazna
		mission_go(-431, 396, 0x70, FORWARD);
	}
	
	void mission_19a(void)
	{
		mission_rotate_abs(90);
	}
	
	void mission20(void)
	{
		//pridji skroz - 3.
		mission_forward(300, 0x50);
	}
	void mission21(void)
	{
		//pokupi 3. kockice
		mission_collect_cubes();
	}
	void mission22(void)
	{
		mission_wait(COLLECT_TIME);
	}
	void mission23(void)
	{
		//Istovar 3. - tacka
		mission_go(-800, -425, 0x50, FORWARD);
	}
	void mission24(void)
	{
		mission_eject_cubes();
	}
	void mission25(void)
	{
		mission_wait(EJECT_TIME);
	}
	void mission26(void)
	{
	
	}
	void mission27(void)
	{
		//ZAKOMENTARISANO U define_missions
	}
	
	
	void last_mission(void)
	{
		mission_robot_stop();
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
			case CINCH:
			{
				print_blue();
				printf("Pin0: %d \n", digitalRead(0));
				print_reset();
				printf("PULL CHINCH CABLE \n");
				while(digitalRead(0));
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
#endif
