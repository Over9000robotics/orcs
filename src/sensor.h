#include <stdio.h>

#ifndef SENSOR_H_
#define SENSOR_H_

#define FRONT_LEFT  0
#define REAR_LEFT   1
#define FRONT_RIGHT 2
#define REAR_RIGHT  3

#define FRONT 0
#define REAR  1

#define NUM_OF_SENS 4

#define SENSOR_ASK 'S'

void update_sensor_status(void);
uint8_t* get_sensors(void);
void sensor_ask_for_status(void);
void print_sensors_state(void);
void ignore_sensors(uint8_t option);

#endif
