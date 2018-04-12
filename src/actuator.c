/**
 * @file actuator.c
 * @brief Functions for manipulation with robot actuators
 */
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "actuator.h"
#include "uart.h"
#include "uart_ax.h"
#include "color.h"

static t_axPacket tx_packet;
static uint8_t tx_buffer[20];

void actuators_init(void)
{
	brushless_set_speed(BR_HEAD, 0);
	brushless_set_speed(BR_SECONDARY, 0);

	ax_set_speed(HEAD_AX, 1023);
}

void ax_set_angle(uint8_t id, uint32_t degrees)
{
	uint32_t angle = 0;
	
	if(degrees > 300 || degrees < 0)
	{
		print_red();
		printf("AX: ");
		print_reset();
		printf("Incorrect degrees value! (%d)  \n", degrees);
		
		return;
	}
	
	if(id < 1 || id > NUM_OF_AX)
	{
		print_red();
		printf("AX: ");
		print_reset();
		printf("Incorrect id! \n");
		
		return;
	}
	
	angle = (degrees * 1023) / 300;
	printf("Angle: %d \n", angle);
	
	axpacket_prepare(id, INST_WRITE, P_GOAL_POSITION_L);
	axpacket_put_word(angle);
	axpacket_end();
}

void ax_set_speed(uint8_t id, uint32_t speed)
{
	if(speed < 0 || speed > 1023)
	{
		print_red();
		printf("AX: ");
		print_reset();
		printf("Incorrect speed value! (%d) \n", speed);
		
		return;
	}
	
	axpacket_prepare(id, INST_WRITE, P_GOAL_SPEED_L);
	axpacket_put_word(speed);
	axpacket_end();
}

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
	
	if(procent > 100 || procent < 0)
	{
		print_red();
		printf("Brushless: ");
		print_yellow();
		printf("invalid procent parameter! (%d) \n", procent);
		return;
	}
			
	if(brushless_num < 0 || brushless_num > NUM_OF_BRUSHLESS)
	{
		print_red();
		printf("Brushless: ");
		print_yellow();
		printf("invalid brushless motor selected! (%d) \n", brushless_num);
		return;
	}
	
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

void actuator_stop_all(void)
{
	//turn off al brushlesses
	int i;
	for(i=0; i<NUM_OF_BRUSHLESS; i++)
	{
		brushless_set_speed(i, 20);
	}
	delay(500);
	for(i=0; i<NUM_OF_BRUSHLESS; i++)
	{
		brushless_set_speed(i, 0);
	}
}
