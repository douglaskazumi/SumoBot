/*
 * Motor.h
 *
 * Created: 11/12/2013 11:22:06 PM
 *  Author: kazumi
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

/************************************************************************/
/* 
	Controls individual motors.

	offset is a number that controls the motor speed.
		BWD (0) - move backwards
		STOP (127) - stops the motor
		FWD (255) - move forward
		any other value moves forward or backwards slower.
		
	motor is which physical motor is controlled
		LEFT - left side motor
		RIGHT - right side motor
*/
/************************************************************************/
void moveMotor(unsigned int offset, int motor);

void setPWM(void);

void turn(int direction);

void move(int direction);

#endif /* MOTOR_H_ */