/**
 * @file mission.h
 * 
 */

#ifndef MISSION_H_
#define MISSION_H_

#include <stdint.h>

#define MAX_MISSIONS 20
#define GO 1

/*
enum MissionList
{
	mission1,
	mission2,
	mission3,
	mission4,
	end_of_missions
};
*/
enum MissionStatus
{
	mission_never_activated,
	mission_in_progress,
	mission_done,
	mission_interrupted,
};

typedef void (*mission) (void);

typedef struct t_mission
{
	enum MissionStatus status;
	mission job;
}t_mission;

void mission_forward(int distance, int speed);
void mission_go(int x, int y, int speed);
void missions_init(t_mission* m);
void missions_print(t_mission* m);

#endif
