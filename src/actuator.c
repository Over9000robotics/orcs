/**
 * @file actuator.c
 * @brief Functions for manipulation with robot actuators
 */
#include <stdio.h>
#include <stdint.h>

#include "actuator.h"
#include "uart.h"

static t_axPacket tx_packet;
static uint8_t tx_buffer[20];
static uint8_t rx_buffer[20];


void axpacket_prepare(uint8_t ax_id, uint8_t instruction, uint8_t data_addr)
{
	tx_packet.sync_1 = 0xFF;
	tx_packet.sync_2 = 0xFF;
	tx_packet.ax_id = ax_id;
	tx_packet.instruction = instruction;
	tx_packet.length = 0;
	
	//parameter0 = strarting adress of 
	//location where data is to be written
	axpacket_put_byte(data_addr);
}

void axpacket_put_byte(uint8_t byte)
{
	tx_packet.parameters[tx_packet.length++] = byte;
}

void axpacket_put_word(uint16_t word)
{
	tx_packet.parameters[tx_packet.length++] = (uint8_t) word;
	tx_packet.parameters[tx_packet.length++] = word >> 8;
}

void axpacket_end(void)
{
	uint16_t checksum;
	
	tx_packet.length += 2; //num of parameters + 2
	
	//calculate checksum
	checksum = tx_packet.ax_id + tx_packet.length + tx_packet.instruction;
	
	int i;
	for(i=0; i<tx_packet.length-2; i++)
	{
		checksum += tx_packet.parameters[i];
	}
	
	tx_packet.checksum = (uint8_t) (~(checksum));
	
	uart_send_axpacket(&tx_packet);
}

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

