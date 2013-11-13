/*
* Motor.c
*
* Created: 11/12/2013 11:22:26 PM
*  Author: kazumi
*/
#define F_CPU 16000000UL
#define DEADTIME 1
#define FWD 0
#define STOP 127
#define BWD 255
#define LEFT 1
#define RIGHT 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Motor.h"

void moveMotor( unsigned int offset, int motor )
{
	if(motor == RIGHT){
		//Timer 0 controls right side motor through pins 5 and 6
		
		//Logic from lecture
		if(setPoint > DEADTIME){
			OCR0A = setPoint - DEADTIME;
		}
		else {
			OCR0A = 1;
		}

		if (setPoint < (0xFF - DEADTIME))
		{
			OCR0B = setPoint + DEADTIME;
		}
		else
		{
			OCR0B = 0xFF - 1;
		}
	}
	else{
		//Timer 2 controls left side motor through pins 3 and 11
		
		//Logic from lecture
		if(setPoint > DEADTIME){
			OCR2A = setPoint - DEADTIME;
		}
		else {
			OCR2A = 1;
		}

		if (setPoint < (0xFF - DEADTIME))
		{
			OCR2B = setPoint + DEADTIME;
		}
		else
		{
			OCR2B = 0xFF - 1;
		}
	}
	
}

void setPWM(void){
	/*
	Set the appropriate pins as outputs:
	PD6 - OC0A (pin 6)
	PD5 - OC0B (pin 5)
	PB3 - OC2A (pin 11)
	PD3 - OC2B (pin 3)
	*/
	DDRB = (1<<3);
	DDRD = (1<<3)|(1<<5)|(1<<6);
	
	//Clear the timer control registers
	TCCR0A = 0;
	TCCR0B = 0;
	TCCR2A = 0;
	TCCR2B = 0;

	/*
	Timer 0
	*/
	//Setup OC0A/B pins to be the PWM output but with opposite signs.
	//Clears OC0A while up-counting, sets it while down-counting.
	TCCR0A |= (1<<COM0A1);
	
	//Sets OC0B while up-counting, clears it while down-counting.
	TCCR0A |= (1<<COM0B1);
	TCCR0A |= (1<<COM0B0);
	
	//Set the timer to mode 1 (phase correct PWM, with 0xFF being the TOP value)
	TCCR0A |= (1<<WGM00);
	
	//Set the prescaler to 1
	TCCR0B |= (1<<CS00);

	/*
	Timer 2
	*/
	//Setup OC2A/B pins to be the PWM output but with opposite signs.
	//Clears OC2A while up-counting, sets it while down-counting.
	TCCR2A |= (1<<COM2A1);
	
	//Sets OC2B while up-counting, clears it while down-counting.
	TCCR2A |= (1<<COM2B1);
	TCCR2A |= (1<<COM2B0);
	
	//Set the timer to mode 1 (phase correct PWM, with 0xFF being the TOP value)
	TCCR2A |= (1<<WGM20);
	
	//Set the prescaler to 1
	TCCR2B |= (1<<CS20);
	
	//Initialize motors stopped
	moveMotor(STOP,LEFT);
	moveMotor(STOP,RIGHT);
}

/************************************************************************/
/* Turns left or right.   direction = [LEFT,RIGHT]                      */
/************************************************************************/
void turn(int direction){
	if(direction == RIGHT){
		moveMotor(BWD - 50,RIGHT);
		moveMotor(FWD + 50,LEFT);
	}
	else if (direction == LEFT){
		moveMotor(FWD + 50,RIGHT);
		moveMotor(BWD - 50,LEFT);
	}
}

/************************************************************************/
/* Moves in direction.  direction = [FWD,BWD,STOP]                      */
/************************************************************************/
void move(int direction){
	if(direction == FWD){
		moveMotor(FWD,RIGHT);
		moveMotor(FWD,LEFT);
	}
	else if (direction == BWD){
		moveMotor(BWD,RIGHT);
		moveMotor(BWD,LEFT);
	}
	else {
		moveMotor(STOP,RIGHT);
		moveMotor(STOP,LEFT);
	}
}