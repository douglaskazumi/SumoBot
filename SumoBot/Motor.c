/*
* Motor.c
*
* Created: 11/12/2013 11:22:26 PM
*  Author: kazumi
* The base of the code is copied from Frank's lecture, some comments are not complete because where not completely visible in the videonote.
* What I did is replace the use of timer 1 by timers 0 and 2 and then create some wrapper functions.
*/
#define F_CPU 16000000UL
#define DEADTIME 1
#define FWD 255
#define STOP 127
#define BWD 0
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
		if(offset > DEADTIME){
			OCR0A = offset - DEADTIME;
		}
		else {
			OCR0A = 1;
		}

		if (offset < (0xFF - DEADTIME))
		{
			OCR0B = offset + DEADTIME;
		}
		else
		{
			OCR0B = 0xFF - 1;
		}
	}
	else{
		//Timer 2 controls left side motor through pins 3 and 11
		
		//Logic from lecture
		if(offset > DEADTIME){
			OCR2A = offset - DEADTIME;
		}
		else {
			OCR2A = 1;
		}

		if (offset < (0xFF - DEADTIME))
		{
			OCR2B = offset + DEADTIME;
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
	DDRB |= (1<<PINB3);
	DDRD |= (1<<PIND3)|(1<<PIND5)|(1<<PIND6);
	
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
		moveMotor(BWD,RIGHT);
		moveMotor(FWD,LEFT);
	}
	else if (direction == LEFT){
		moveMotor(FWD,RIGHT);
		moveMotor(BWD,LEFT);
	}
}

/************************************************************************/
/* Moves in direction.  direction = [FWD,BWD,STOP]                      */
/************************************************************************/
void move(int direction){
	if(direction == FWD){
		//moveMotor(FWD+11,RIGHT); //Hack to try to drive straight
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