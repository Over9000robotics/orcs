/**
 * @file config.h
 * @brief robot configuration file
 * @todo make side variable
 */
#include <stdint.h>

#ifndef CONFIG_H_
#define CONFIG_H_

//#define DEBUG 1
//#define DEBUG_VERIFY
//#define DEBUG_UART1 1
#define AX_PKT_DEBUG 1

//START options
#define NO_START_OPTION 0
#define ENTER 1
#define CINCH 2

#define ROUND_TIME 60000

//motion config
#define MOTION_RESPONSE_MS 100	/**< wait 50 ms for motion response message */

#define MOTION_START_X 0	/**< X coordinate on startup */
#define MOTION_START_Y 0	/**< Y coordinate on startup */
#define MOTION_START_O 0	/**< Orientation on startup */
#define MOTION_REFRESH_INTERVAL 50
#define MOTION_MIN_SPEED 1
#define MOTION_MAX_SPEED 255
#define MOTION_NORMAL_SPEED 50 /* 0x32 */
#define MOTION_SAFE_SPEED 50

#define GREEN 1
#define ORANGE 2

/**
 * Config keys
 */
enum ConfEnum { 
	CONF_ROTATION_REGULATOR = 0,
	CONF_STUCK = 1,
	CONF_DEBUG = 2,
	CONF_KEEP_COUNT = 3,
	CONF_MOTOR_CONNECTED = 4,
	CONF_TMR = 5,
	CONF_DISTANCE_REGULATOR = 6,
	CONF_STATUS_CHANGE_REPORT = 7,
	CONF_ENABLE_STUCK = 8,

	CONF_MOTOR_CONST_ROC = 9,
	CONF_STUCK_DISTANCE_MAX_FAIL_COUNT = 10,
	CONF_STUCK_DISTANCE_JUMP = 11,
	CONF_MOTOR_RATE_OF_CHANGE = 12,
	CONF_MOTOR_SPEED_LIMIT = 13,
	CONF_SEND_STATUS_INTERVAL = 14,
	CONF_STUCK_ROTATION_MAX_FAIL_COUNT = 15,
	CONF_STUCK_ROTATION_JUMP = 16,

	CONF_OMEGA = 17,
	CONF_WHEEL_R2 = 18,
	CONF_SLOWDOWN = 19,
	CONF_PID_R_P = 20,
	CONF_ANGLE_SPEEDUP = 21,
	CONF_ALPHA = 22,
	CONF_ACCEL = 23,
	CONF_WHEEL_R1 = 24,
	CONF_SPEED_DROP = 25,
	CONF_WHEEL_DISTANCE = 26,
	CONF_SLOWDOWN_ANGLE = 27,
	CONF_VMAX = 28,
	CONF_PID_D_D = 29,
	CONF_PID_D_P = 30,
	CONF_PID_R_D = 31,
};

uint8_t use_input_arguments(int argc, char* argv[]);
uint8_t set_side(char* s);
uint8_t get_side(void);
void print_side(void);


#endif
