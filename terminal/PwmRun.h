/*
 * cPwmRun.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef PWMRUN_H_
#define PWMRUN_H_

#include "pwm.h"
#include "stdint.h"

class cPwmRun {
	cPWM * mPwm;
	uint8_t mNewDutyC;
	uint8_t mDelay;
	uint8_t mDelayCnt;
	uint8_t mDutyDelay;
	bool mEnabled = false;

public:
	cPwmRun(cPWM *);
	void setDuty(uint8_t,uint8_t);
	void setDelayedDuty(uint8_t dutyDelay, uint8_t delay, uint8_t newDuty);
	void run();

	virtual ~cPwmRun();
};

#endif /* PWMRUN_H_ */
