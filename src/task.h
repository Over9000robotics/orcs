/**
 * @file task.h
 * @brief
 */

#include <stdint.h>

#ifndef task_H_
#define task_H_

typedef void (*task) (void);

void init_task(uint8_t option);


void task1(void);

void task2(void);

void task3(void);

void task4(void);

void task5(void);

#endif
