#include <stdio.h>
#include <stdint.h>

#include "mission.h"
#include "motion.h"



void missions_init(t_mission* m)
{
	int i = 0;
	
	for(i=0; i<MAX_MISSIONS; i++)
	{
		m -> status = done;
	}
}

void missions_print(t_mission* m)
{
	int i = 0;
	
	for(i=0; i<MAX_MISSIONS; i++)
	{
		printf("Mission_status: %d \n", m -> status);
	}
}
