/*
 * cTimerPwm.cpp
 *
 *  Created on: 18 Feb 2017
 *      Author: christo
 */

#include <avr/io.h>
#include <terminal.h>
#include <timer_pwm.h>



cTimerPwm::cTimerPwm() {
	mInv = 0;
	mTimerNum = 0;

	mPwmA = 0;
	mPwmB = 0;
	mPwmC = 0;
	// TODO Auto-generated constructor stub

}

cTimerPwm::cTimerPwm(uint8_t inv, uint8_t timerNum, cPWM * pwmA, cPWM * pwmB, cPWM * pwmC)
{
	mTimerNum = timerNum;
	mInv = inv;

	mPwmA = pwmA;
	mPwmB = pwmB;
	mPwmC = pwmC;
}

void cTimerPwm::init()
{
	if (mEnabled)
		return;

	switch (mTimerNum)
	{
	case 4:
	{
		// set PH3 & PH4 & PH5 as output
		DDRH |= _BV(3) | _BV(4) | _BV(5);

		if (mInv == 1)
		{
			// inverting mode A & B
			TCCR4A |= _BV(COM4A1);
			TCCR4A |= _BV(COM4B1);
			TCCR4A |= _BV(COM4C1);
		} else
		{
			// non-inverting mode A & B
			TCCR4A |= _BV(COM4A1) | _BV(COM4A0);
			TCCR4A |= _BV(COM4B1) | _BV(COM4B0);
			TCCR4A |= _BV(COM4C1) | _BV(COM4C0);
		}

		// set Waveform as phase correct PWM / 8 bit
		TCCR4A |=_BV(WGM40);
		// set the prescaler as clk/1024
		TCCR4B |= _BV(CS41) | _BV(CS40);

		mPwmA->setOCRn(&OCR4A);
		mPwmB->setOCRn(&OCR4B);
		mPwmC->setOCRn(&OCR4C);
	}
	break;

	case 5:
	{
		// set PL3 & PL4 & PL5 as output
		DDRL |= _BV(3) | _BV(4) | _BV(5);

		if (mInv == 1)
		{
			// inverting mode A & B
			TCCR5A |= _BV(COM5A1) | _BV(COM5A0);
			TCCR5A |= _BV(COM5B1) | _BV(COM5B0);
			TCCR5A |= _BV(COM5C1) | _BV(COM5C0);
		} else
		{
			// non-inverting mode A & B
			TCCR5A |= _BV(COM5A1);
			TCCR5A |= _BV(COM5B1);
			TCCR5A |= _BV(COM5C1);
		}

		// set Waveform as phase correct PWM / 8 bit
		TCCR5A |= _BV(WGM50);

		// set the prescaler as clk/1024
		TCCR5B |= _BV(CS51) | _BV(CS50);

		mPwmA->setOCRn(&OCR5A);
		mPwmB->setOCRn(&OCR5B);
		mPwmC->setOCRn(&OCR5C);
	}
	break;

	default:
		return;
	}

	mEnabled = true;
	printp("PWM%d init\n",mTimerNum);
}


cTimerPwm::~cTimerPwm()
{
	// TODO Auto-generated destructor stub
}

