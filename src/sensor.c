#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#include "sensor.h"
#include "uart.h"
#include "color.h"

#define RX_BUFF_SIZE 10
#define TX_BUFF_SIZE 10

static uint8_t tx_buffer[TX_BUFF_SIZE];
static uint8_t rx_buffer[RX_BUFF_SIZE];
static uint8_t sensors[NUM_OF_SENS];
static uint8_t sensors_fr[2]; /** Front or back */
static uint8_t* sensors_fr_ptr;

static uint8_t state_changed_flag = 0;
static uint8_t const communication_type;

/*	PACKET COMPOSITION
 * 
 * START	
 * 	 0xFE
 * 
 * DATA
 *   SENSOR1 DATA (0xFF active or 0x00 not active)
 *   SENSOR2 DATA (0xFF active or 0x00 not active)
 * 
 * END
 *   0xFC
 */
void update_sensor_status(void)
{
	
	uart1_receive_byte(rx_buffer);
	
	if(rx_buffer[0] != 0xFE)
		return;

	else
		delay(5);

		int i;
		for(i=0; i<NUM_OF_SENS; i++)
		{
			while(uart1_receive_byte(rx_buffer+i) == 0);
		}

	//END of PACKET
	while(uart1_receive_byte(rx_buffer+NUM_OF_SENS) == 0);
	
	//ignore if invalid packet end
	if(rx_buffer[NUM_OF_SENS] != 0xFC)
		return;
	
	else
	{
		int i;
		for(i=0; i<NUM_OF_SENS; i++)
		{
			if(sensors[i] != rx_buffer[i])
				state_changed_flag = 1;
				
			sensors[i] = rx_buffer[i];
		}
	}
	
	if((sensors[FRONT_LEFT] == 0xFF) || (sensors[FRONT_RIGHT] == 0xFF))
		sensors_fr[FRONT] = 0xFF;
	else
		sensors_fr[FRONT] = 0x00;
	
	if((sensors[REAR_LEFT] == 0xFF) || (sensors[REAR_RIGHT] == 0xFF))
		sensors_fr[REAR] = 0xFF;
	else
		sensors_fr[REAR] = 0x00;
	
	if(state_changed_flag)
	{
		print_sensors_state();
		state_changed_flag = 0;
	}
}

uint8_t* get_sensors(void)
{
	sensors_fr_ptr = &sensors_fr[0];
	return sensors_fr_ptr;
}

void sensor_ask_for_status(void)
{
	tx_buffer[0] = SENSOR_ASK;
	uart1_transmit(&tx_buffer[0], 1);
}

void print_sensors_state(void)
{
	printf("\n");
	int i;
	for(i=0; i<NUM_OF_SENS; i++)
	{
		print_yellow();
		printf("Sens[%d]: ", i);
		print_reset();
		printf("%x \n", sensors[i]);
	}
}
