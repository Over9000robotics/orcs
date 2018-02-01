/**
 * @file task.c
 * @todo Input cinch option
 */

#include <stdio.h>
#include <stdint.h>

#include "task.h"
#include "motion.h"
#include "config.h"
#include "color.h"


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

