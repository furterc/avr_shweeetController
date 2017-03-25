/*
 * timer1.h
 *
 *  Created on: 12 May 2016
 *      Author: cfurter
 */

#ifndef SRC_TIMER_TWO_H_
#define SRC_TIMER_TWO_H_

#include <avr/io.h>
#include <avr/interrupt.h>


void timerTwo_init();
void timerTwo_setCB(void (*cb)(void));

#endif /* SRC_TIMER_TWO_H_ */
