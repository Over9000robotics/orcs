/**
 * Constans (commands) for message types for motion packetss
 * @file motioncmd.h
 */

#ifndef MOTIONCMD_H
#define MOTIONCMD_H

#define	MOTION_SET_CONFIG 0x63	//c
#define	MOTION_GET_CONFIG 0x43		//C
#define	MOTION_MOVE_TO 0x4E		//N
#define	MOTION_GET_STATUS_AND_POSITION 0x50	//P /* ask for status and position */
#define	MOTION_UNSTUCK 0x55		//U
#define	MOTION_MOTOR 0x6D		//m
#define	MOTION_GET_POSITION 0x70	//p
#define	MOTION_SET_SPEED 0x56		//V
#define	MOTION_SET_ROTATION_SPEED 0x72	//r
#define	MOTION_MOVE_FORWARD 0x72	//r
#define	MOTION_RELATIVE_ROTATE 0x54	//T
#define	MOTION_ABSOLUTE_ROTATE 0x41	//A
#define	MOTION_TURN_AND_GO 0x47	//G
#define	MOTION_CURVE 0x51		//Q
#define	MOTION_HARD_STOP 0x53		//S
#define	MOTION_SOFT_STOP 0x73		//s
#define	MOTION_SMOOTH_STOP 0x74	//t
#define	MOTION_RESET_DRIVER 0x52	//R
#define MOTION_KILL_REGULATOR 0x48	//H
#define MOTION_FORWARD 0x44		//D
#define	MOTION_SET_POSITION_AND_ORIENTATION 0x49	//I
#define	MOTION_BREAK 0x69		//i

#endif
