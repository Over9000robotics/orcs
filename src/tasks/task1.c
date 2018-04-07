/**
 * @file task1.c
 * @todo Input cinch option
 */

#include <stdio.h>
#include <stdint.h>

#include "../task.h"
#include "../motion.h"
#include "../config.h"
#include "../color.h"
#include "../mission.h"


static t_mission missions[MAX_MISSIONS];
static t_mission* mission_ptr = &missions[0];

missions[0].job = mission_go(200, 200, 0)

void task1(void)
{
	missions_init(missions);
	mission_go();
	
}
