/*
 * cPwmRun.cpp
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#include "PwmRun.h"

cPwmRun::cPwmRun(cPWM * pwm)
{
    mPwm = pwm;
    mNewDutyC = 0;
    mDelay = 0;
    mDelayCnt = 0;
    mDutyDelay = 0;
}

void cPwmRun::setDuty(uint8_t delay, uint8_t newDuty)
{
    mDelay = delay;
    mDelayCnt = 0;
    mNewDutyC = newDuty;
    mEnabled = true;
}

void cPwmRun::setDelayedDuty(uint8_t dutyDelay, uint8_t delay, uint8_t newDuty)
{
    mDelay = delay;
    mDutyDelay = dutyDelay;
    mDelayCnt = 0;
    mNewDutyC = newDuty;
    mEnabled = true;
}

void cPwmRun::run()
{
    if (!mEnabled)
        return;

    if (mDutyDelay > 0)
    {
        mDutyDelay--;
        return;
    }

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

cPwmRun::~cPwmRun()
{
    // TODO Auto-generated destructor stub
}

