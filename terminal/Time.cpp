/*
 * cTime.cpp
 *
 *  Created on: 11 Mar 2017
 *      Author: christo
 */

#include <terminal.h>
#include "Time.h"


cTime::cTime()
{
	mHours = 0;
	mMinutes = 0;
	mSeconds = 0;
}


void cTime::incTime()
{
	if(++mSeconds>59)
	{
		mSeconds = 0;
		if (++mMinutes>59)
		{
			mMinutes = 0;
			if (++mHours>23)
				mHours = 0;
		}
	}
}

void cTime::setHours(uint8_t hours)
{
	mHours = hours;
}

uint8_t cTime::getHours()
{
	return mHours;
}

void cTime::setMinutes(uint8_t minutes)
{
	mMinutes = minutes;
}

uint8_t cTime::getMinutes()
{
	return mMinutes;
}
void cTime::setSeconds(uint8_t seconds)
{
	mSeconds = seconds;
}

uint8_t cTime::getSeconds()
{
	return mSeconds;
}

void cTime::printTime()
{
	printp("%02d:%02d:%02d\n", mHours, mMinutes, mSeconds);
}

cTime::~cTime() {
	// TODO Auto-generated destructor stub
}



