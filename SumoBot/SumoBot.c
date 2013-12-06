/*
* SumoBot.c
*
* Created: 11/11/2013 9:23:28 AM
*  Author: kazumi
*/

#define F_CPU 16000000UL

//Motor driving values
#define FWD 255
#define STOP 127
#define BWD 0
#define LEFT 1
#define RIGHT 0

//"Times" for avoid spinning forever
#define SPIN_LIMIT 2100
#define AFTER_SPIN 2900

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "Sonar.h"
#include "Motor.h"
#include "QTI.h"

volatile int spinningLeftCount;

int main(void)
{
	/************************************************************************/
	/* LED   Pin 7 Red                                                      */
	/************************************************************************/
	DDRD |= (1<<PIND7);
	PORTD &= ~(1<<PIND7);
	
	/************************************************************************/
	/* LED   Pin 4 Green                                                    */
	/************************************************************************/
	DDRD |= (1<<PIND4);
	PORTD &= ~(1<<PIND4);
	
	/************************************************************************/
	/* To turn on remotely (pin 2)                                            */
	/************************************************************************/
	while(PIND & (1<<PIND2)){
	}
	
	//Light on the green LED
	PORTD |= (1<<PIND4);
	
	//initialize serial
	//init_uart();
	
	//initialize variables
	qti = 0;
	dead = 0;
	spinningLeftCount = 0;
	
	//set up pwm
	setPWM();
	//set up QTI
	initQTI();
	//set up sonar
	initSonar();
	// enable global interrupts
	sei(); 
	
	
	/************************************************************************/
	/* Routine to drop the plows                                            */
	/************************************************************************/
	move(FWD);
	_delay_ms(300);
	move(BWD);
	_delay_ms(300);
	turn(LEFT);
	_delay_ms(300);
	turn(RIGHT);
	_delay_ms(300);
	turn(LEFT);
	_delay_ms(300);
	move(STOP);
	
	while(1)
	{
		if(qti > 0){
			/************************************************************************/
			/* If any of the QTIs is activated, handle                              */
			/************************************************************************/
			handleQTI();
			//printf("The qti is %u       \r\n", (uint16_t)qti);
			spinningLeftCount = 0;
		}
		else{
			/************************************************************************/
			/* Sonar - PC0 (Pin A0), PC1 (Pin A1)                              */
			/************************************************************************/
			updateRanges();
			//if((int)(rangeRight * 0.02712) == 12){
				//printf("The range is %u inches, %u         %u\r\n", (uint16_t)rangeCenter, (uint16_t)rangeRight, (uint16_t)(rangeRight * 0.02712)); // Print the range in inches to serial as
			//}
			//printf("The range is %u inches, %u         \r\n", (uint16_t)rangeCenter, (uint16_t)rangeRight);
			
			//If completed one spin and didn't detect, go forward
			if(spinningLeftCount > SPIN_LIMIT){
				//Go forward if within the count to go forward
				if(spinningLeftCount < AFTER_SPIN){
					move(FWD);
					spinningLeftCount += 1;
				}
				//After finishing going forward, reset counter
				else{
					spinningLeftCount = 0;
				}
			}
			else{
				//If in front of us, go for it
				if (centerLowCount > 1)
				{
					move(FWD);
					spinningLeftCount = 0;
				}
				//If it's in the right sonar sight, turn right
				else if(rightLowCount > 1){
					turn(RIGHT);
					spinningLeftCount = 0;
				}
				//Otherwise try left
				else if(rightHighCount > 1 && centerHighCount > 1){
					turn(LEFT);
					spinningLeftCount += 1;
				}
			}
		}
		
		/************************************************************************/
		/* If dead == 1, center qti have detected white
		Turn off green led, turn on red led and exit the while loop */
		/************************************************************************/
		if(dead){
			PORTD |= (1<<PIND7);
			PORTD &= ~(1<<PIND4);
			return;
		}
	}
}

/************************************************************************/
/* Interruption that updates the qti variable (xor makes the bits for qti detecting white 1)                          */
/************************************************************************/
ISR(PCINT0_vect){
	qti = PINB & ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
	qti = qti ^ ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
}

/************************************************************************/
/* Interruption for the sonar                                           */
/************************************************************************/
ISR(PCINT1_vect){
	if(risingEdge){
		resetTimer();
	}
	else{
		updateCount();
	}
}