/**
 * @file motion.h
 * 
 */

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>

enum State
{
	STATUS_IDLE = 'I',		//0x49
	STATUS_MOVING = 'M',	//0x4D
	STATUS_ROTATING = 'R',	//0x52
	STATUS_STUCK = 'S',		//0x53
	STATUS_ERROR = 'E'		//0x45
};

typedef struct t_motionState
{
	int16_t x;
	int16_t y;
	int16_t angle;
	int16_t current_speed;
	int16_t max_speed;
	enum State state;
}t_motionState;


/**
 * Checks communication with motion board
 * @return 1 if success, 0 if not 
 * 
 */
uint8_t motion_check(void);

/**
 * Sets position and orientation
 * @param x - x coordinate
 * @param y - y coordinate
 * @param o - orientation
 */
void motion_set_position(int16_t x, int16_t y, int16_t o);

/**
 * Checks is packet received, and process him
 * @return 1 if packet and command was ok
 * @return 0 if there is no packet, or command was unknown
 */
uint8_t motion_msg_status(void);

void set_status_and_position(void);

void print_status_and_position(void);

void motion_print_state(void);

#endif
