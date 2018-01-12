#ifndef MOTIONCMD_H
#define MOTIONCMD_H

#define	MOTION_SET_CONFIG 0x63	//c
#define	GET_CONFIG 0x43		//C
#define	MOVE_TO 0x4E		//N
#define	SEND_STATUS_AND_POSITION 0x50//P
#define	UNSTUCK 0x55		//U
#define	MOTOR 0x6D		//m
#define	SEND_POSITION 0x70	//p
#define	SET_SPEED 0x56		//V
#define	SET_ROTATION_SPEED 0x72	//r
#define	MOVE_FORWARD 0x72	//r
#define	RELATIVE_ROTATE 0x54	//T
#define	ABSOLUTE_ROTATE 0x41	//A
#define	TURN_AND_GO 0x47	//G
#define	CURVE 0x51		//Q
#define	HARD_STOP 0x53		//S
#define	SOFT_STOP 0x73		//s
#define	SMOOTH_STOP 0x74	//t
#define	RESET_DRIVER 0x52	//R
#define KILL_REGULATOR 0x48	//H
#define FORWARD 0x44		//D
#define	SET_POSITION_AND_ORIENTATION 0x49//I
#define	BREAK 0x69		//i

#endif
