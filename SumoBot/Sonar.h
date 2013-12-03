/*
 * Sonar.h
 *
 * Created: 11/12/2013 12:54:50 PM
 *  Author: kazumi
 */ 

#ifndef SONAR_H_
#define SONAR_H_

volatile unsigned int count;
volatile unsigned int risingEdge;

volatile float rangeCenterMean;
volatile float rangeRightMean;

struct Node
{
	float Data;
	struct Node* next;
}*rearCenter, *frontCenter, *rearRight, *frontRight;

void pop(struct Node *rear);

void push(float value, struct Node *front, struct Node *rear);

float getMean(struct Node *rear);

void initSonar(void);

void resetTimer(void);

void updateCount(void);

void startSonarMeasurement(int sonar);

void updateRanges(void);

#endif /* SONAR_H_ */