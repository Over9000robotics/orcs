/**
 * @file color.c
 * @brief Functions for setting the default printf() color
 */

#include <stdio.h>
#include "color.h"

void print_red(void) 
{
	printf(RED_TEXT);
}

void print_yellow(void) 
{
  printf(YELLOW_TEXT);
}

void print_blue(void)
{
	printf(BLUE_TEXT);
}

void print_green(void)
{
	printf(GREEN_TEXT);
}

void print_orange(void)
{
	printf(ORANGE_TEXT);
}

void print_reset(void) 
{
  printf("\033[0m");
}
