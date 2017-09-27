/*
 * cPwmRun.cpp
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#include "PwmRun.h"

cPwmRun::cPwmRun()
{
    mPwm = 0;
    mNewDutyC = 0;
    mDelay = 0;
    mDelayCnt = 0;
    mDutyDelay = 0;
}

cPwmRun::cPwmRun(cPWM * pwm)
{
    mPwm = pwm;
    mNewDutyC = 0;
    mDelay = 0;
    mDelayCnt = 0;
    mDutyDelay = 0;
}

void cPwmRun::setPWM(cPWM *pwm)
{
    mPwm = pwm;
}

void cPwmRun::setDuty(uint8_t delay, uint8_t newDuty)
{
    if(!mPwm)
        return;

    mDelay = delay;
    mDelayCnt = 0;
    mNewDutyC = newDuty;
    mEnabled = true;
}

void cPwmRun::setDelayedDuty(uint8_t dutyDelay, uint8_t delay, uint8_t newDuty)
{
    if(!mPwm)
        return;

    mDelay = delay;
    mDutyDelay = dutyDelay;
    mDelayCnt = 0;
    mNewDutyC = newDuty;
    mEnabled = true;
}

/* PWM run return:
 * -1   !mPWM
 * 0    !enabled
 * 1    busy
 */
int8_t cPwmRun::run()
{
    if(!mPwm)
        return -1;

    if (!mEnabled)
        return 0;

    if (mDutyDelay > 0)
    {
        mDutyDelay--;
        return 1;
    }

    //delay counter
    if (++mDelayCnt < mDelay)
        return 1;

    uint8_t dutyTemp = mPwm->getDutyC();

    if (dutyTemp == mNewDutyC)
    {
        mEnabled = false;
        return 0;
    }

    if (dutyTemp < mNewDutyC)
        mPwm->incDutyC();
    else
        mPwm->decDutyC();

    return 1;
}

cPwmRun::~cPwmRun()
{
    // TODO Auto-generated destructor stub
}

