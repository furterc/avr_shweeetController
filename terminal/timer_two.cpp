
#include "timer_two.h"

void (*timerTwoCallback)(void) = 0;

ISR(TIMER2_OVF_vect);


void timerTwo_init()
{
	//set external clock
	ASSR |= _BV(AS2);

	//set the pre scaler to 128
	TCCR2B |= _BV(CS22) | _BV(CS20);
}

void timerTwo_setCB(void (*cb)(void))
{
    if(cb)
    {
        timerTwoCallback = cb;

        //enable timer 1 overflow interrupt
        TIMSK2 |= (1 << TOIE2);
    }
}


/*** Interrupt Service Routines ***/

ISR(TIMER2_OVF_vect)
{
    if(timerTwoCallback)
        timerTwoCallback();
}
