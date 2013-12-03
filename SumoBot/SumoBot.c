/*
* SumoBot.c
*
* Created: 11/11/2013 9:23:28 AM
*  Author: kazumi
*/

#define F_CPU 16000000UL
#define FWD 255
#define STOP 127
#define BWD 0
#define LEFT 1
#define RIGHT 0
#define DISTTH 28

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "Sonar.h"
#include "Motor.h"
#include "QTI.h"

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
	/* To turn on remotely pin 2                                            */
	/************************************************************************/
	//while(PIND & (1<<PIND2)){
	//}
	
	PORTD |= (1<<PIND4);
	
	init_uart();
	qti = 0;
	first = 1;
	dead = 0;
	//Counter to use instead of timer to change the algorithm
	int counter = 0;
	
	//set up pwm
	setPWM();
	//set up QTI
	initQTI();
	//set up sonar
	initSonar();
	sei(); // enable global interrupts

	/************************************************************************/
	/* QTI test - part 1                                                    */
	/************************************************************************/
	//move(FWD);
	
	//turn(LEFT);
	//while(1){
	//}
	
	while(1)
	{
		///************************************************************************/
		///* QTI with interruptions test                                          */
		///************************************************************************/
		//if(qti > 0){
			///************************************************************************/
			///* If any of the QTIs is activated, do the action                       */
			///************************************************************************/
			//handleQTI();
			////printf("The qti is %u       \r\n", (uint16_t)qti);
		//}
		//else{
			/************************************************************************/
			/* Sonar - PC0 (Pin A0), PC1 (Pin A1)                              */
			/************************************************************************/
			updateRanges();
			//printf("The range is %u inches, %u         \r\n", (uint16_t)rangeCenter, (uint16_t)rangeRight); // Print the range in inches to serial as

			//If counter reaches 2100, that is aproximately the count after 360 degrees
			if(counter > 2100){
				//Move forward for 500 count(arbitrary time)
				if(counter < 2600){
					counter ++;
					move(FWD);
				}
				//After that go back to zero
				else{
					counter = 0;
				}
			}
			else {
				//If in front of us, go for it
				if (rangeCenter < DISTTH)
				{
					counter = 0;
					move(FWD);
				}
				//If it's in the right sonar sight, turn right
				else if(rangeRight < DISTTH){
					counter = 0;
					turn(RIGHT);
				}
				//Otherwise try left
				else{
					counter++;
					turn(LEFT);
				}
			}
		//}
		
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
/* Interruption that updates the qti variable                           */
/************************************************************************/
ISR(PCINT0_vect){
	qti = PINB & ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
	qti = qti ^ ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
	handleQTI();
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