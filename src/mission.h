/**
 * @file mission.h
 *
 */

#ifndef MISSION_H_
#define MISSION_H_

#include <stdint.h>

#define MAX_MISSIONS 200

enum MissionStatus
{
	mission_never_activated,
	mission_in_progress,
	mission_done,
	mission_interrupted,
	mission_sens_interrupted,
	mission_from_interrupted,
};

typedef void (*mission) (void);

typedef struct t_mission
{
	enum MissionStatus status;
	mission job;
	
}t_mission;

void mission_nuliraj_hvataljke(void);
void mission_podigni_sisaljke(void);

void mission_collect_cubes(void);
void mission_eject_cubes(void);

void mission_forward(int distance, int speed);
void mission_go(int x, int y, int speed, int direction);
void mission_rotate_abs(int angle);
void mission_rotate_rel(int angle);
void mission_switch_activate(void);

void mission_robot_stop(void);
void mission_wait(unsigned int time_ms);

void mission_ax_angle(uint8_t id, uint32_t degrees);
void mission_brushless(uint8_t brushless_num, uint8_t procent);
void mission_servo(uint8_t servo_num, uint8_t angle);

void missions_init(t_mission* m);
void missions_print(t_mission* m);

#endif
