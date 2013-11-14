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
		if(!(PIND & (1<<PIND2)) && !(PIND & (1<<PIND1))){ //Both front
			move(BWD);
			_delay_ms(50);
			turn(LEFT);
			_delay_ms(1000);
			move(FWD);
		}
		else if(!(PIND & (1<<PIND2))){ //Front right
			move(BWD);
			_delay_ms(50);
			turn(LEFT);
			_delay_ms(120);
			move(FWD);
		}
		else if (!(PIND & (1<<PIND1))) //Front left
		{
			move(BWD);
			_delay_ms(50);
			turn(RIGHT);
			_delay_ms(120);
			move(FWD);
		}
}