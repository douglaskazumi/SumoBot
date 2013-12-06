/*
 * Sonar.h
 *
 * Created: 11/12/2013 12:54:50 PM
 *  Author: kazumi
 */ 

#ifndef SONAR_H_
#define SONAR_H_

//Stores the count that will be translated in distance
volatile unsigned int count;

//Boolean to keep track whether the signal is in the rising edge or not(reading the pin value was not reliable)
volatile unsigned int risingEdge;

//Stores the final calculated range for the center sonar
volatile float rangeCenter;

//Stores the final calculated range for the right side sonar
volatile float rangeRight;

//Variables used to store counts used on the proccess of rejecting outliers in the measurement
volatile unsigned int centerLowCount;
volatile unsigned int rightLowCount;
volatile unsigned int centerHighCount;
volatile unsigned int rightHighCount;

/************************************************************************/
/* Initialize interruptions and variables related to the sonar                                                                     */
/************************************************************************/
void initSonar(void);

/************************************************************************/
/* Clears the value in the timer counter                                                                     */
/************************************************************************/
void resetTimer(void);

/************************************************************************/
/* Gets the response of the sonar                                                                     */
/************************************************************************/
void updateCount(void);

/************************************************************************/
/* Triggers the measurement                                                                     */
/************************************************************************/
void startSonarMeasurement(int sonar);

/************************************************************************/
/* Main method while using the sonars. Calls startMeasurement for each sonar and do the proper calculations for the final ranges                                                                      */
/************************************************************************/
void updateRanges(void);

#endif /* SONAR_H_ */