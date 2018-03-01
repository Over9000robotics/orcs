/**
 * @file motion.h
 *
 */

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>

#define FORWARD 1
#define BACKWARD -1

enum State
{
	STATUS_IDLE = 'I',		 //0x49
	STATUS_MOVING = 'M',	 //0x4D
	STATUS_ROTATING = 'R', //0x52
	STATUS_STUCK = 'S',		 //0x53
	STATUS_ERROR = 'E'		 //0x45
};

typedef struct t_motionState
{
	int16_t x;
	int16_t y;
	int16_t angle;
	int16_t current_speed;
	int16_t max_speed;
	int16_t max_rot_speed;
	int16_t moving_direction;
	enum State state;
}t_motionState;


/**
 * Checks communication with motion board
 * @return 1 if success, 0 if not
 *
 */
uint8_t motion_check(void);

t_motionState* get_motion_state(void);

void motion_get_status_and_position(void);

/**
 * Sets position and orientation
 * @param x - x coordinate [mm]
 * @param y - y coordinate [mm]
 * @param o - orientation [degrees]
 */
void motion_set_position(int16_t x, int16_t y, int16_t o);

/**
 * Sets robot speed
 * @param speed - positive number [0 - 255]
 */
void motion_set_speed(uint8_t speed);

/**
 * Robot speed while rotating
 * @param speed - positive number [0 - 255]
 */
void motion_set_rotating_speed(uint8_t speed);

/**
 * Checks is packet received, and process him
 * @return 1 if packet and command was ok
 * @return 0 if there is no packet, or command was unknown
 */
uint8_t motion_msg_status(void);

void set_status_and_position(void);

void print_status_and_position(void);

/**
 * Moves to required point
 * @param x - x coordinate of the point [mm]
 * @param y - y coordinate of the point [mm]
 * @param direction - (0  - pick smallest rotation)
 * 					  (1  - forward)
 * 					  (-1 - backward)
 * @param radius - radius for curvilinear motion, not necessarily
*/
void motion_move_to(int16_t x, int16_t y, int8_t direction, int16_t radius);

/**
 * Calculates angle to setted point, turns and goes to the point
 * @param x - x coordinate of the point [mm]
 * @param y - y coordinate of the point [mm]
 * @param end_speed
 * @param direction - >0 forward, <0 backward
 */
void motion_turn_and_go(int16_t x, int16_t y, uint8_t end_speed, int8_t direction);

/**
 * Goes straight
 * @param length >0 forward [mm], <0 backward [mm]
 * @param end_speed - robot speed while reaching final point
 */
void motion_forward(int16_t length, uint8_t end_speed);

void motion_absolute_rotate(int16_t degrees);
void motion_relative_rotate(int16_t degrees);

void motion_hard_stop(void);
void motion_soft_stop(void);
void motion_reset_driver(void);
void motion_unstuck(void);

/**
 *	Checks desired speed validity and sets motion speed if
 *  desired speed is valid
 *  @param speed - desired robot motioning speed
 *  speed <= 0 -> motion speed will bi setted to MOTION_NORMAL_SPEED
 *							 which is defined in config.h
 */
void motion_speed_check_set(int speed);
void motion_print_state(void);

#endif
