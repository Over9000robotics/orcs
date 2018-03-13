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

/**
 * @brief Sets speed of brushless motor
 * @param procent - value from 0 to 100 (%)
 */
void brushless_set_speed(uint8_t brushless_num, uint8_t procent)
{
	uint8_t num_of_param = 0;
	
	tx_buffer[num_of_param++] = BR_SET_SPEED;
	tx_buffer[num_of_param++] = brushless_num;
	tx_buffer[num_of_param++] = procent;
	
	uart1_transmit(tx_buffer, num_of_param);
}
