/*
 * cManualButton.cpp
 *
 *  Created on: 23 Feb 2017
 *      Author: christo
 */

#include "cManualButton.h"

void (*callback)(void) = 0;

cManualButton::cManualButton()
{
	printp("btn init\n");
	MAN_BTN_DDR &= ~(1 << MAN_BTN_INPUTPIN);
	mState = false;

}

void cManualButton::btnSetCB(void (*cb)(void))
{
	if(cb)
	{
		callback = cb;
	}
}

uint8_t cManualButton::getCount()
{
	return mCount;
}

void cManualButton::run()
{
	/* switch changed */
	if (!(MAN_BTN_PIN & (1 << MAN_BTN_INPUTPIN)))
	{
		if (!mState)
		{
			printp("btn on\n");
			if(callback)
				callback();
			mState = true;
			mCount = 0;
		}
		mCount++;
	} else
	{
		if (mState)
		{
			printp("btn off\n");
			mState = false;
		}
	}
}

cManualButton::~cManualButton() {
	// TODO Auto-generated destructor stub
}

