/**
 * @file task.h
 * @brief
 */

#ifndef task_H_
#define task_H_

#include <stdint.h>

#include "mission.h"

#define EJECT_TIME 500

enum TaskStatus
{
	not_active,
	active,
	task_interrupted
};

typedef struct t_task
{
	unsigned int time;
	enum TaskStatus status;
}t_task;

void define_missions(void);
void define_obstacle_handling(void);

void init_task(uint8_t option);

void task(void);

t_mission* task_get_mission_ptr(void);

void task_mission_check(void);

void obstacle1(void);
void obstacle2(void);
void obstacle3(void);
void obstacle_end(void);

void define_sensor_obstacle_handling(void);
void sens_obstacle1(void);
void sens_obstacle_end(void);

void mission1(void);
void mission2(void);
void mission3(void);
void mission4(void);
void mission5(void);
void mission6(void);
void mission7(void);
void mission8(void);
void mission9(void);
void mission10(void);
void mission11(void);
void mission12(void);
void mission13(void);
void mission14(void);
void mission15(void);
void mission16(void);
void mission17(void);
void mission18(void);
void mission19(void);
void mission_19a(void);
void mission20(void);
void mission21(void);
void mission22(void);
void mission23(void);
void mission21(void);
void mission22(void);
void mission23(void);
void mission24(void);
void mission25(void);
void mission26(void);
void mission27(void);

void last_mission(void);

void task1(void);

void task2(void);

void task3(void);

void task4(void);

void task5(void);

#endif
