/*
 * cRemoteButton.cpp
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#include "cRemoteButton.h"

cRemoteButton::cRemoteButton()
{

}

void cRemoteButton::setCB(void (*cb)(void))
{
	if (!cb)
		return;

	buttonCallback = cb;
	mEnabled = true;
}

void cRemoteButton::runCB()
{
	if (mEnabled)
		buttonCallback();
}

void  cRemoteButton::setState(bool state)
{
	mBtnState = state;
}

bool  cRemoteButton::getState()
{
	return mBtnState;
}



cRemoteButton::~cRemoteButton() {
	// TODO Auto-generated destructor stub
}

