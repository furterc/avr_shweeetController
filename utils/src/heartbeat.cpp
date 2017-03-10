#include "heartbeat.h"

cHeartbeat::cHeartbeat(cLED *led)
{
	mLED = led;
	mState = LED_HEARTBEAT;
	mCount = 0;
}

void cHeartbeat::run()
{
	if(!mLED)
		return;

	switch(mState)
	{
	case LED_HEARTBEAT:
		heartBeat();
		break;
	case LED_CONNECTING:
		connecting();
		break;
	case LED_ERROR:
		error();
		break;
	case LED_WIFI:
		WiFiup();
		break;
	}
}

void cHeartbeat::error()
{
	switch(mCount++)
	{
	case 0:
		mLED->red();
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	case 6:
		mLED->off();
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		break;
	case 12:
	default:
		mCount = 0;
		break;
	}
}

void cHeartbeat::heartBeat()
{
	switch(mCount++)
	{
	case 0:
		mLED->green();
		break;
	case 1:
		mLED->off();
		break;
	case 2:
		mLED->green();
		break;
	case 3:
		mLED->off();
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		break;
	case 12:
	default:
		mCount = 0;
		break;
	}
}

void cHeartbeat::connecting()
{
	if(mCount++ > 0)
	{
		mLED->green();
		mCount = 0;
	}
	else
	{
		mLED->off();
	}
}

void cHeartbeat::WiFiup()
{
	switch(mCount++)
	{
	case 0:
		mLED->green();
		break;
	case 1:
		mLED->red();
		break;
	case 2:
		mLED->green();
		break;
	case 3:
		mLED->red();
		break;
	case 4:
		mLED->green();
		break;
	case 5:
		mLED->red();
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		break;
	case 12:
	default:
		mCount = 0;
		break;
	}
}

cHeartbeat::~cHeartbeat()
{
}

