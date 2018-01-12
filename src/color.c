#include "color.h"
#include <stdio.h>

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

void print_reset(void) 
{
  printf("\033[0m");
}
