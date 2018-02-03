/**
 * @file task.h
 * @brief
 */


#ifndef task_H_
#define task_H_

#include <stdint.h>

#include "mission.h"

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

void init_task(uint8_t option);

void task(void);

t_mission* task_get_mission_ptr(void);

void task_mission_check(void);

void mission1(void);
void mission2(void);

void task1(void);

void task2(void);

void task3(void);

void task4(void);

void task5(void);

#endif
