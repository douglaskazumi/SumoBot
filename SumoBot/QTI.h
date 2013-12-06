/*
 * QTI.h
 *
 * Created: 11/12/2013 12:54:50 PM
 *  Author: kazumi
 */ 

#ifndef QTI_H_
#define QTI_H_

//Stores the values of the sensors
volatile int qti;

//Boolean to store when the center QTI detects white
volatile int dead;

//Based on value of qti variable, take the correct action
void handleQTI(void);

#endif /* QTI_H_ */