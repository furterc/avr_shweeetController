#ifndef CTIMERPWM_H_
#define CTIMERPWM_H_

#include "pwm.h"

class cTimerPwm {

	cPWM * mPwmA;
	cPWM * mPwmB;
	cPWM * mPwmC;

	uint8_t mTimerNum;
	uint8_t mInv;
	bool mEnabled = false;

public:
	cTimerPwm();
	cTimerPwm(uint8_t inv, uint8_t timerNum, cPWM * pwmA, cPWM * pwmB, cPWM * pwmC);
	void init();
	virtual ~cTimerPwm();
};

#endif /* CTIMERPWM_H_ */
