/*
 * Sonar.c
 *
 * Created: 11/12/2013 12:54:06 PM
 *  Author: kazumi
 */ 
#define F_CPU 16000000UL
#define RIGHT (1<<0)
#define CENTER (1<<1)
#define NULL 0
#define SAMPLE_SIZE 5

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Sonar.h"

void initSonar(void) // enable interrupt
{
	//Initialize variables
	count = 10000;
	risingEdge = 1;
	rangeCenterMean = 10000;
	rangeRightMean = 10000;
	frontCenter = NULL;
	frontRight = NULL;
	for(int i = 0; i < SAMPLE_SIZE; i++){
		push(300,frontCenter,rearCenter);
		push(300,frontRight,rearRight);
	}
	
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
	push(count * 0.02712,frontCenter,rearCenter); // Calculate range in inches from timer count
	pop(rearCenter);
	rangeCenterMean = getMean(rearCenter);
	
	startSonarMeasurement(RIGHT); // start trigger pulse for new sonar measurement
	_delay_ms(20); // Minimum delay theoretically 18.5ms due to sonar
	push(count * 0.02712,frontRight,rearRight); // Calculate range in inches from timer count
	pop(rearRight);
	rangeRightMean = getMean(rearRight);
}

void pop(struct Node *rear)
{
	struct Node *temp, *var=rear;
	if(var==rear)
	{
		rear = rear->next;
		free(var);
	}
}

void push(float value, struct Node *front, struct Node *rear)
{
	struct Node *temp;
	temp=(struct Node *)malloc(sizeof(struct Node));
	temp->Data=value;
	if (front == NULL)
	{
		front=temp;
		front->next=NULL;
		rear=front;
	}
	else
	{
		front->next=temp;
		front=temp;
		front->next=NULL;
	}
}

float getMean(struct Node *rear){
	struct Node *var=rear;
	float mean = 0;
	float count = 0;
	if(var!=NULL)
	{
		while(var!=NULL)
		{
			mean += var->Data;
			count += 1;
			var=var->next;
		}
	}
	return mean/count;
}