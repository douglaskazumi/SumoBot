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

	//set up pwm
	setPWM();
	//set up QTI
	//set up sonar
	initSonar();
	sei(); // enable global interrupts
	
	move(FWD);
	
	while(1)
	{			
		checkQTI();
		//updateRanges();
		
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