/*
 * pwm.h
 *
 *  Created on: 17 Feb 2017
 *      Author: christo
 */

#ifndef PWM_H_
#define PWM_H_

#include "stdint-gcc.h"

class cPWM {
	uint8_t mDutyC = 0;
	bool mEnabled = false;
	volatile uint16_t * mOCRn;
public:
	cPWM();
	void setOCRn(volatile uint16_t *);
	uint8_t setDutyC(uint8_t);
	uint8_t getDutyC();
	void incDutyC();
	void decDutyC();
	virtual ~cPWM();
};

#endif /* PWM_H_ */
