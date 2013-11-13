/*
 * Sonar.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Sonar.h"

void initSonar(void) // enable interrupt
{
	//Initialize variables
	count = 10000;
	risingEdge = 1;
	range = 10000;
	
	PCICR |= (1<<PCIE1); //enable pin change interrupt for PORTC
	PCMSK1 |= (1<<PCINT8); //enable interrupt for PC0 (Pin A0)
	
	TCCR1B |= ((1<<1)|(1<<0)); // set bit 1 and 0 to 1 to set prescaler to 64
}

void resetTimer(void)
{
	unsigned char sreg;
	sreg = SREG; //save global interrupt flag
	cli(); //disable all interrupts
	TCNT1 = 0; // set TCNT1 to 0 to reset timer
	risingEdge = 0;
	SREG = sreg; // restore global interrupt flag
}

void updateCount(void){
	unsigned char sreg;
	sreg = SREG;
	cli();
	count = TCNT1;
	SREG = sreg;
}

void startSonarMeasurement(void){
	DDRC |= 1; // set PC0 to output
	PORTC |= 1; // set PC0 to high
	_delay_us(5); // make trigger pulse
	PORTC &= ~1; //set PC0 to low
	DDRC &= ~1; // Set PC0 to input
	risingEdge = 1;
	
}

void getSonar(void){
	startSonarMeasurement(); // start trigger pulse for new sonar measurement
	_delay_ms(50); // Minimum delay theoretically 18.5ms due to sonar
	range = count * 0.02712; // Calculate range in inches from timer count
}

ISR(PCINT1_vect){
	if(risingEdge){
		resetTimer();
	}
	else{
		updateCount();
	}
}

