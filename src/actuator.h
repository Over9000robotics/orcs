/**
 * @file actuator.h
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <stdint.h>

#define COLLECT_CUBES '1'

#define BR_SET_SPEED    'B' //brushless set speed character
#define SERVO_SET_ANGLE 'A'

#define BR_HEAD 1 		//main brushless on big the robot
#define BR_SECONDARY 2 	//secondary brushless motor on the big robot
#define NUM_OF_BRUSHLESS 2

#define SERVO1 1
#define SERVO2 2
#define SERVO3 3
#define NUM_OF_SERVOS 3


/**AX-12 IDs*/
#define HEAD_AX 1 /** ID of AX-12 motor is 1 */
#define NUM_OF_AX 1

/** AX-12 packet properties */
#define AX_PACKET_HEADER 5
#define AX_MAX_PKT_SIZE 20

/** AX-12 packet structure */
typedef struct t_axPacket
{
	uint8_t sync_1;
	uint8_t sync_2;
	uint8_t ax_id;
	uint8_t length; //NUM OF PARAM + 2
	uint8_t instruction;
	uint8_t parameters[AX_MAX_PKT_SIZE-AX_PACKET_HEADER];
	uint8_t checksum;
}t_axPacket;

/** AX-12 instructions */
#define INST_PING  0x01
#define INST_READ  0x02
#define INST_WRITE 0x03
#define INST_REG_WRITE 0x04

//--- Control Table Address ---
/** AX-12 EEPROM AREA */
#define P_MODEL_NUMBER_L 0
#define P_MODOEL_NUMBER_H 1
#define P_VERSION 2
#define P_ID 3
#define P_BAUD_RATE 4
#define P_RETURN_DELAY_TIME 5
#define P_CW_ANGLE_LIMIT_L 6
#define P_CW_ANGLE_LIMIT_H 7
#define P_CCW_ANGLE_LIMIT_L 8
#define P_CCW_ANGLE_LIMIT_H 9
#define P_SYSTEM_DATA2 10
#define P_LIMIT_TEMPERATURE 11
#define P_DOWN_LIMIT_VOLTAGE 12
#define P_UP_LIMIT_VOLTAGE 13
#define P_MAX_TORQUE_L 14
#define P_MAX_TORQUE_H 15
#define P_RETURN_LEVEL 16
#define P_ALARM_LED 17
#define P_ALARM_SHUTDOWN 18
#define P_OPERATING_MODE 19
#define P_DOWN_CALIBRATION_L 20
#define P_DOWN_CALIBRATION_H 21
#define P_UP_CALIBRATION_L 22
#define P_UP_CALIBRATION_H 23

/** AX-12 RAM area */
#define P_TORQUE_ENABLE (24)
#define P_LED (25)
#define P_CW_COMPLIANCE_MARGIN (26)
#define P_CCW_COMPLIANCE_MARGIN (27)
#define P_CW_COMPLIANCE_SLOPE (28)
#define P_CCW_COMPLIANCE_SLOPE (29)
#define P_GOAL_POSITION_L (30)
#define P_GOAL_POSITION_H (31)
#define P_GOAL_SPEED_L (32)
#define P_GOAL_SPEED_H (33)
#define P_TORQUE_LIMIT_L (34)
#define P_TORQUE_LIMIT_H (35)
#define P_PRESENT_POSITION_L (36)
#define P_PRESENT_POSITION_H (37)
#define P_PRESENT_SPEED_L (38)
#define P_PRESENT_SPEED_H (39)
#define P_PRESENT_LOAD_L (40)
#define P_PRESENT_LOAD_H (41)
#define P_PRESENT_VOLTAGE (42)
#define P_PRESENT_TEMPERATURE (43)
#define P_REGISTERED_INSTRUCTION (44)
#define P_PAUSE_TIME (45)
#define P_MOVING (46)
#define P_LOCK (47)
#define P_PUNCH_L (48)
#define P_PUNCH_H (49)

void actuators_init(void);

void collect_cubes(void);

void ax_set_angle(uint8_t id, uint32_t degrees);
void ax_set_speed(uint8_t id, uint32_t speed);

void axpacket_prepare(uint8_t ax_id, uint8_t instruction, uint8_t data_addr);
void axpacket_put_byte(uint8_t byte);
void axpacket_put_word(uint16_t word);
void axpacket_end(void);

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

/**
 * @brief Stopps all running actuators on actuator-sensor board
 */
void actuator_stop_all(void);
#endif
