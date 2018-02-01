/**
 * @file mission.h
 * 
 */

#ifndef MISSION_H_
#define MISSION_H_

#include <stdint.h>

#define MAX_MISSIONS 20
#define GO 1

enum MissionStatus
{
	not_active,
	in_progress,
	done
};

typedef void (*mission) (void);
typedef struct t_mission
{
	mission job;
	enum MissionStatus status;
}t_mission;

void missions_init(t_mission* m);
void missions_print(t_mission* m);

#endif
