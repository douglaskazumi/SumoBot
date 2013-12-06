/*
 * Sonar.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL

//Pins for each sonar
#define RIGHT (1<<0)
#define CENTER (1<<1)

//Upper threshold for distances that we consider
#define DISTTH 28

//Number of sequential measurements that we need to do under the threshold to consider a valid measurement
#define MEAS_COUNT 2

//Value that converts counting to inches
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
	// start trigger pulse for new sonar measurement
	startSonarMeasurement(CENTER); 
	// Minimum delay theoretically 18.5ms due to sonar
	_delay_ms(20); 
	// Calculate range in inches from timer count
	rangeCenter = count * CONV; 
	
	//Code to reject outliers
	if(rangeCenter < DISTTH){
		//This ensures that the count doesn't blow
		if(centerLowCount < MEAS_COUNT){
			centerLowCount++;
		}
		centerHighCount=0;
	}
	else{
		//This ensures that the count doesn't blow
		if(centerHighCount < MEAS_COUNT){
			centerHighCount++;
		}
		centerLowCount = 0;
	}
	
	// start trigger pulse for new sonar measurement
	startSonarMeasurement(RIGHT); 
	// Minimum delay theoretically 18.5ms due to sonar
	_delay_ms(20); 
	
	/************************************************************************/
	/* The following chunk of code is to handle some issues that start happening
		with the right side sonar, where it was outputing 12 inches randomly when nothing was in front of it
		this started happening on the Thursday before competition.
		After spending 6 hours trying to fix that properly, we've decided filtering the bad values.             */
	/************************************************************************/	
	int reject = count;
	if(reject > 453 && reject < 473){
		count = 4000;
	}
	
	// Calculate range in inches from timer count
	rangeRight = count * CONV; 
	
	//Code to reject outliers
	if(rangeRight < DISTTH){
		//This ensures that the count doesn't blow
		if(rightLowCount < MEAS_COUNT){
			rightLowCount++;
		}
		rightHighCount=0;
	}
	else{
		//This ensures that the count doesn't blow
		if(rightHighCount < MEAS_COUNT){
			rightHighCount++;
		}
		rightLowCount = 0;
	}
}

