#include <stdio.h>

#ifndef SENSOR_H_
#define SENSOR_H_

#define NUM_OF_SENS 2

#define SENSOR_ASK 'S'

void update_sensor_status(void);
void print_sensors_state(void);
void sensor_ask_for_status(void);

#endif
