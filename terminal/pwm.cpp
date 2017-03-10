/*
 * pwm.cpp
 *
 *  Created on: 17 Feb 2017
 *      Author: christo
 */

#include <terminal.h>
#include "pwm.h"


cPWM::cPWM(){}

void cPWM::setOCRn(volatile uint16_t * ocr)
{
	mOCRn = ocr;
	mEnabled = true;
}

uint8_t cPWM::setDutyC(uint8_t duty)
{
	if (!mEnabled)
		return 0;
	mDutyC = duty;
	*mOCRn = mDutyC;
	return 1;
}

void cPWM::incDutyC()
{
	if (mDutyC == 0xFF)
		return;

	setDutyC(++mDutyC);
}

void cPWM::decDutyC()
{
	if (mDutyC == 0x00)
		return;

	setDutyC(--mDutyC);
}

uint8_t cPWM::getDutyC()
{
	return mDutyC;
}

cPWM::~cPWM() {}

