/**
 * @file color.h
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <stdint.h>

#define BR_SET_SPEED    'B' //brushless set speed character
#define SERVO_SET_ANGLE 'A'

#define BR_HEAD 1 		//main brushless on big the robot
#define BR_SECONDARY 2 	//secondary brushless motor on the big robot
#define NUM_OF_BRUSHLESS 2

#define SERVO1 1
#define SERVO2 2
#define SERVO3 3
#define NUM_OF_SERVOS 3

/**
 * @brief Sets speed of brushless motor
 * @param brushless_num - number of motor, written in actuator-sensor board code
 * @param procent - value from 0 to 100 (%)
 */
void brushless_set_speed(uint8_t brushless_num, uint8_t procent);

/**
 * @brief Sets position of servo motor on the actuator control board
 * @param servo_num - number of servo, written in actuator-sensor board
 * @param angle - agnle to go, MIN 0, MAX 180
 */
void servo_set_angle(int8_t servo_num, uint8_t angle);

#endif
