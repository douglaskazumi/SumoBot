/*
 * QTI.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL
#define FWD 255
#define STOP 127
#define BWD 0
#define LEFT 1
#define RIGHT 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "QTI.h"

void initQTI(void){
	PCICR |= 0x01;
	//P5 - Front Right - pin 13
	//P4 - front left - pin 12
	//P2 - back left - pin10
	//P1 - back right - pin 9
	//P0 - Center qti - pin 8
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1)|(1<<PCINT2)|(1<<PCINT4)|(1<<PCINT5);
}

void handleQTI(void){
	//Check for center QTI
	if (qti & (1<<0))
	{
		move(STOP);
		dead = 1;
	}
	else{
		int back = 300;
		int turnT = 300;
	
		/************************************************************************/
		/* Code for remaining inside black                                      */
		/************************************************************************/
		if((qti & (1<<PINB4)) && (qti & (1<<PINB5))){ //Both front
			move(BWD);
			_delay_ms(back);
			turn(LEFT);
			_delay_ms(turnT);
			/************************************************************************/
			/* Disable if using with sonar                                          */
			/************************************************************************/
			move(FWD);
		}
		else if(qti & (1<<PINB5)){ //Front right
			move(BWD);
			_delay_ms(back);
			turn(LEFT);
			_delay_ms(turnT);
			/************************************************************************/
			/* Disable if using with sonar                                          */
			/************************************************************************/
			move(FWD);
		}
		else if (qti & (1<<PINB4)) //Front left
		{
			move(BWD);
			_delay_ms(back);
			turn(RIGHT);
			_delay_ms(turnT);
			/************************************************************************/
			/* Disable if using with sonar                                          */
			/************************************************************************/
			move(FWD);
		}
		else if (qti & (1<<PINB2)) //Back left
		{
			turn(RIGHT);
			_delay_ms(turnT);
			move(FWD);
			_delay_ms(back);
			/************************************************************************/
			/* Disable if using with sonar                                          */
			/************************************************************************/
			move(FWD);
		}
		else if (qti & (1<<PINB1)) //Back Right
		{
			turn(LEFT);
			_delay_ms(turnT);
			move(FWD);
			_delay_ms(back);
			/************************************************************************/
			/* Disable if using with sonar                                          */
			/************************************************************************/
			move(FWD);
		}
		
	}
	
	/************************************************************************/
	/* Code for the straight line milestone                                 */
	/************************************************************************/
	//if((!(qti & (1<<PIND2))||!(qti & (1<<PIND1)))){ //Front right
		//if(first){
			//move(BWD);
			//_delay_ms(1000);
			//turn(LEFT);
			//_delay_ms(1300);
			//move(FWD);
			//first = 0;
		//}
		//else{
			//if(!(qti & ((1<<PIND1) | (1<<PIND2)))){ //Both front
				//move(BWD);
				//_delay_ms(back);
				//turn(LEFT);
				//_delay_ms(turnT);
				//move(FWD);
			//}
			//else if(!(qti & (1<<PIND2))){ //Front right
				//move(BWD);
				//_delay_ms(back);
				//turn(LEFT);
				//_delay_ms(turnT);
				//move(FWD);
			//}
			//else if (!(qti & (1<<PIND1))) //Front left
			//{
				//move(BWD);
				//_delay_ms(back);
				//turn(RIGHT);
				//_delay_ms(turnT);
				//move(FWD);
			//}
		//}
	//}
}