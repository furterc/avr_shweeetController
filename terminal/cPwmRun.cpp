/*
 * cPwmRun.cpp
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#include "cPwmRun.h"

cPwmRun::cPwmRun(cPWM * pwm)
{
	mPwm = pwm;
}

void cPwmRun::setDuty(uint8_t delay, uint8_t newDuty)
{
	mDelay = delay;
	mDelayCnt = 0;
	mNewDutyC = newDuty;
	mEnabled = true;
}

void cPwmRun::run()
{
	if (!mEnabled)
		return;

	//delay counter
	if (++mDelayCnt < mDelay)
		return;

	uint8_t dutyTemp = mPwm->getDutyC();

	if (dutyTemp == mNewDutyC)
	{
		mEnabled = false;
		return;
	}

	if (dutyTemp < mNewDutyC)
		mPwm->incDutyC();
	else
		mPwm->decDutyC();

}


cPwmRun::~cPwmRun() {
	// TODO Auto-generated destructor stub
}

