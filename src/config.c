/**
 * @file config.c
 */
#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include "string.h"
#include "color.h"

static uint8_t side = GREEN;

uint8_t set_side(char* s)
{
	if(!strcmp(s, "ORANGE") || !strcmp(s, "orange"))
	{
		side = ORANGE;
		return ORANGE;
	}
	
	else if(!strcmp(s, "GREEN") || !strcmp(s, "green"))
	{
		side = GREEN;
		return GREEN;
	}
	
	else
	{
		print_red();
		printf("Side input is incorrect! \n");
		print_reset();
		return 0;
	}
}

uint8_t use_input_arguments(int argc, char* argv[])
{
	int i;
	
	for(i=1; i < argc; i++) 
	{
		if(!strcmp(argv[i], "--side")) 
		{	
			if(set_side(argv[i+1]) == 0)
			{
				return 0;
			}
			i++;
		}
	}
	return 1;
}

uint8_t get_side(void)
{
	return side;
}

void print_side(void)
{
	print_yellow();
	printf("Side: ");
	if(side == GREEN)
	{
		print_green();
		printf("GREEN \n");
	}
	else
	{
		print_orange();
		printf("ORANGE \n");
	}
	print_reset();
}

