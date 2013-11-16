/*
 * QTI.h
 *
 * Created: 11/12/2013 12:54:50 PM
 *  Author: kazumi
 */ 

#ifndef QTI_H_
#define QTI_H_

volatile int qti;
volatile int first;
volatile int dead;

void checkQTI(void);

void handleQTI(void);

#endif /* QTI_H_ */