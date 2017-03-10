/*
 * cPwmRun.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef CPWMRUN_H_
#define CPWMRUN_H_

#include "pwm.h"
#include "stdint.h"

class cPwmRun {
	cPWM * mPwm;
	uint8_t mNewDutyC;
	uint8_t mDelay;
	uint8_t mDelayCnt;
	bool mEnabled = false;

public:
	cPwmRun(cPWM *);
	void setDuty(uint8_t,uint8_t);
	void run();

	virtual ~cPwmRun();
};

#endif /* CPWMRUN_H_ */
