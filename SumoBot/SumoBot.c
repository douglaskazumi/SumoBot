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


/*
PORTB
0 - nothing
1<<1 - front left
1<<2 - front right
1<<3 - back right
1<<4 - back left
1<<5 - center
*/
volatile int qti = 0;

int main(void)
{
	init_uart();

	//set up pwm
	setPWM();
	//set up QTI
	//set up sonar
	initSonar();
	sei(); // enable global interrupts
	
	while(1)
	{
		updateRanges();
		
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