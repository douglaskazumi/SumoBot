/*
 * QTI.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL
#define FWD 0
#define STOP 127
#define BWD 255
#define LEFT 1
#define RIGHT 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "QTI.h"

void checkQTI(void){
		qti = PIND & ((1<<PIND1) | (1<<PIND2));
}

void handleQTI(void){
	if (!(PIND & (1<<PIND7)))
	{
		move(STOP);
		dead = 1;
	}
	//else
	int back = 300;
	int turnT = 120;
	
	/************************************************************************/
	/* Code for remaining inside black                                      */
	/************************************************************************/
	//if(!(PIND & ((1<<PIND7) | (1<<PIND2)))){ //Both front
		//move(BWD);
		//_delay_ms(back);
		//turn(LEFT);
		//_delay_ms(turnT);
		///************************************************************************/
		///* Disable if using with sonar                                          */
		///************************************************************************/
		////move(FWD);
	//}
	//else if(!(PIND & (1<<PIND2))){ //Front right
		//move(BWD);
		//_delay_ms(back);
		//turn(LEFT);
		//_delay_ms(turnT);
		///************************************************************************/
		///* Disable if using with sonar                                          */
		///************************************************************************/
		//move(FWD);
	//}
	//else if (!(PIND & (1<<PIND7))) //Front left
	//{
		//move(BWD);
		//_delay_ms(back);
		//turn(RIGHT);
		//_delay_ms(turnT);
		///************************************************************************/
		///* Disable if using with sonar                                          */
		///************************************************************************/
		//move(FWD);
	//}
	
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