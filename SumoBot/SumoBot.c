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
	/* LED   Pin 7                                                          */
	/************************************************************************/
	//DDRD |= (1<<PIND7);
	//PORTD &= ~(1<<PIND7);
	
	/************************************************************************/
	/* To turn on remotely pin 2                                            */
	/************************************************************************/
	//while(PIND & (1<<PIND2)){
	//}
	
	init_uart();
	qti = 0;
	first = 1;
	dead = 0;
	


	//set up pwm
	setPWM();
	//set up QTI
	initQTI();
	//set up sonar
	//initSonar();
	sei(); // enable global interrupts

	/************************************************************************/
	/* QTI test - part 1                                                    */
	/************************************************************************/
	//move(FWD);
	turn(LEFT);
	
	while(1)
	{		
		/************************************************************************/
		/* QTI with interruptions test                                          */
		/************************************************************************/
		
		//if(qti > 0){
			/////************************************************************************/
			/////* QTI test - part 2                                                    */
			/////************************************************************************/
			//handleQTI();
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
		
		//if(dead){
			//PORTB |= (1<<PINB5);
			//return;
		//}
		
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

ISR(PCINT0_vect){
	//qti = PINB & ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
	//qti = qti ^ ((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB4)|(1<<PINB5));
	qti = PINB & ((1<<PINB4)|(1<<PINB5));
	qti = qti ^ ((1<<PINB4)|(1<<PINB5));
}

ISR(PCINT1_vect){
	if(risingEdge){
		resetTimer();
	}
	else{
		updateCount();
	}
}