/**
 * @file color.c
 * @brief Functions for manipulation with robot actuators
 */

#include <stdio.h>
#include <stdint.h>

#include "actuator.h"
#include "uart.h"

static uint8_t tx_buffer[20];
static uint8_t rx_buffer[20];

void brushless_set_speed(uint8_t brushless_num, uint8_t procent)
{
	uint8_t num_of_param = 0;
	
	tx_buffer[num_of_param++] = BR_SET_SPEED;
	tx_buffer[num_of_param++] = brushless_num;
	tx_buffer[num_of_param++] = procent;
	
	uart1_transmit(tx_buffer, num_of_param);
}

/**
 * @brief Sets position of servo motor on the actuator control board
 * @param servo_num - number of servo, written in actuator-sensor board
 * @param angle - agnle to go, MIN 0, MAX 180
 */
void servo_set_angle(int8_t servo_num, uint8_t angle)
{
	uint8_t num_of_param = 0;
	
	tx_buffer[num_of_param++] = SERVO_SET_ANGLE;
	tx_buffer[num_of_param++] = servo_num;
	tx_buffer[num_of_param++] = angle;
	
	uart1_transmit(tx_buffer, num_of_param);
}

