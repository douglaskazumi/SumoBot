/*
* SumoBot.c
*
* Created: 11/11/2013 9:23:28 AM
*  Author: kazumi
*/

#define F_CPU 16000000UL
#define FWD 0
#define STOP 127
#define BWD 255
#define LEFT 1
#define RIGHT 0

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "serial.h"
#include "Sonar.h"
#include "Motor.h"
#include "QTI.h"

int main(void)
{
	//init_uart();
	qti = 0;
	first = 1;
	dead = 0;
	DDRB |= (1<<PINB5);
	PORTB &= ~(1<<PINB5);

	//set up pwm
	setPWM();
	//set up QTI
	//set up sonar
	initSonar();
	sei(); // enable global interrupts

	/************************************************************************/
	/* QTI test - part 1                                                    */
	/************************************************************************/
	move(FWD);
	
	
	while(1)
	{			
		//checkQTI();
		///************************************************************************/
		///* 2 - Front left / 4 - Front right / 6 - Both                          */
		///************************************************************************/
		//printf("The qti is %i\r\n", qti); // Print the range in inches to serial as
		//if(!(PIND & (1<<PIND2)) || !(PIND & (1<<PIND7))){
			///************************************************************************/
			///* QTI test - part 2                                                    */
			///************************************************************************/
			handleQTI();
		//}
		//else{
		//
			/////************************************************************************/
			/////* Sonar test - PC0 (Pin A0), PC1 (Pin A1), PC2 (Pin A2)                */
			/////************************************************************************/
			//updateRanges();
			////printf("The range is %u inches, %u          %u\r\n", (uint16_t)rangeCenter, (uint16_t)rangeLeft, (uint16_t)rangeRight); // Print the range in inches to serial as
//
			//if (rangeCenter < 25)
			//{
				//move(FWD);
			//} else {
				//turn(LEFT);
			//}
		//}
		
		if(dead){
			PORTB |= (1<<PINB5);
			return;
		}
		
		//printf("The range is %u inches, %u          %u\r\n", (uint16_t)rangeCenter, (uint16_t)rangeLeft, (uint16_t)rangeRight); // Print the range in inches to serial as
		//if(rangeCenter < 12 && rangeRight < 12){
			//if(rangeCenter < 8){
				//move(FWD);
			//}
			//else if (rangeRight < 8)
			//{
				//turn(LEFT);
			//}
			//else
			//{
				//turn(RIGHT);
			//}
		//}
		
		/************************************************************************/
		/* IR test - Pin 9                                                      */
		/************************************************************************/
		//if(!(PINB & (1 << PINB1))){
		//move(FWD);
		//}
		//else{
		//move(STOP);
		//}
	}
}

ISR(PCINT1_vect){
	if(risingEdge){
		resetTimer();
	}
	else{
		updateCount();
	}
}