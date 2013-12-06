/*
 * Sonar.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL
#define RIGHT (1<<0)
#define CENTER (1<<1)
#define DISTTH 28
#define MEAS_COUNT 2
#define CONV 0.02712

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Sonar.h"

void initSonar(void) // enable interrupt
{
	//Initialize variables
	count = 10000;
	risingEdge = 1;
	rangeCenter = 10000;
	rangeRight = 10000;
	centerLowCount = 0;
	centerHighCount = 0;
	rightLowCount = 0;
	rightHighCount = 0;
	
	PCICR |= (1<<PCIE1); //enable pin change interrupt for PORTC
	PCMSK1 |= (1<<PCINT8)|(1<<PCINT9); //enable interrupt for PC0 (Pin A0), PC1 (Pin A1)
	
	TCCR1B |= (1<<CS11)|(1<<CS10); // set bit 1 and 0 to 1 to set prescaler to 64
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

void startSonarMeasurement(int sonar){
	DDRC |= sonar; // set PC0 to output
	PORTC |= sonar; // set PC0 to high
	_delay_us(5); // make trigger pulse
	PORTC &= ~sonar; //set PC0 to low
	DDRC &= ~sonar; // Set PC0 to input
	risingEdge = 1;
	
}

void updateRanges(void){
	startSonarMeasurement(CENTER); // start trigger pulse for new sonar measurement
	_delay_ms(20); // Minimum delay theoretically 18.5ms due to sonar
	rangeCenter = count * CONV; // Calculate range in inches from timer count
	if(rangeCenter < DISTTH){
		if(centerLowCount < MEAS_COUNT){
			centerLowCount++;
		}
		centerHighCount=0;
	}
	else{
		if(centerHighCount < MEAS_COUNT){
			centerHighCount++;
		}
		centerLowCount = 0;
	}
	
	startSonarMeasurement(RIGHT); // start trigger pulse for new sonar measurement
	_delay_ms(20); // Minimum delay theoretically 18.5ms due to sonar
	int reject = count;
	if(reject > 453 && reject < 473){
		count = 4000;
	}
	rangeRight = count * CONV; // Calculate range in inches from timer count
	if(rangeRight < DISTTH){
		if(rightLowCount < MEAS_COUNT){
			rightLowCount++;
		}
		rightHighCount=0;
	}
	else{
		if(rightHighCount < MEAS_COUNT){
			rightHighCount++;
		}
		rightLowCount = 0;
	}
}

