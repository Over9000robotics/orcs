/**
 * @file task.c
 * @todo Input cinch option
 */

#include <stdio.h>
#include <stdint.h>

#include "motion.h"
#include "config.h"
#include "color.h"

void init_task(uint16_t option)
{
	char start_char = 0;
	char entter=0xa;
	
	switch(option)
	{
		case ENTER:
		{
			do
			{
				printf("PRESS ENTER TO START THE PROGRAM: %c \n",entter);
				start_char = getchar();
				printf("0x%x ", start_char);
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
