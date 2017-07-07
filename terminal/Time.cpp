/*
 * cTime.cpp
 *
 *  Created on: 11 Mar 2017
 *      Author: christo
 */

#include <terminal.h>
#include "Time.h"
#include "Bluetooth.h"
#include "BT_Commands.h"

#include "Lights.h"
#include "cMsg.h"
#include "RS485.h"
#include "avr/delay.h"


uint8_t getTime(uint8_t data0)
{
    switch (data0)
    {
    case 0:
        return myTime.getHours();
        break;
    case 1:
        return myTime.getMinutes();
        break;
    case 2:
        return myTime.getSeconds();
        break;
    }
    return 0;
}

void setTime(uint8_t data0, uint8_t data1)
{
    switch (data0)
    {
    case 0:
        myTime.setHours(data1);
        break;
    case 1:
        myTime.setMinutes(data1);
        break;
    case 2:
        myTime.setSeconds(data1);
        break;
    }
}

cMsg cmsg;
void timeEntry(cMsg msg)
{
    if (msg.getType() == msg.TYPE_GET)
    {
        msg.setType(msg.TYPE_SET);
        msg.setData1(getTime(msg.getData0()));
        uint8_t bytes[4];
        msg.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
    }
    else
        setTime(msg.getData0(), msg.getData1());
}
extern const bt_dbg_entry btTime =
{ timeEntry, cmsg.TAG_TIME };

void alarmEntry(cMsg msg)
{
    if (msg.getType() == msg.TYPE_GET)
    {
        msg.setType(msg.TYPE_SET);

        if (msg.getData0() == 0)
            msg.setData1(myTime.getAlarmHour());
        else if (msg.getData0() == 1)
            msg.setData1(myTime.getAlarmMinute());

        uint8_t bytes[4];
        msg.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
    }
    else
    {
        if (msg.getData0() == 0)
            myTime.setAlarmHour(msg.getData1());
        else if (msg.getData0() == 1)
            myTime.setAlarmMinute(msg.getData1());
    }
}
extern const bt_dbg_entry btAlarm =
{ alarmEntry, cmsg.TAG_ALARM };

void cTime::checkAlarms()
{
    if (mAlarmSet == 0)
        return;

    if (mAlarmHour == mHours && mAlarmMinute == mMinutes)
    {
        mLights.setSoft(mLights.LIGHT_KITCHEN_TOP, 255, 255);
        mLights.setSoft(mLights.LIGHT_KITCHEN_BOT, 255, 255);

        printp("alarm!!!\n");

        cMsg msg = cMsg(0,0,0,0);
        msg.setType(msg.TYPE_SET);
        msg.setTag(msg.TAG_LED_BED);
        msg.setData1(255);
        msg.setData0(0);
        uint8_t data[4];
        msg.toBytes(data);
        RS485.transmit_packet(data,4);

        _delay_ms(150);
        msg.setData0(1);
        msg.toBytes(data);
        RS485.transmit_packet(data,4);

        _delay_ms(150);
        msg.setData0(2);
        msg.toBytes(data);
        RS485.transmit_packet(data,4);

        _delay_ms(150);
        msg.setData0(3);
        msg.toBytes(data);
        RS485.transmit_packet(data,4);
    }
}

void cTime::setAlarm(uint8_t hour, uint8_t minute)
{
    mAlarmHour = hour;
    mAlarmMinute = minute;
    mAlarmSet = 1;
}

void cTime::setAlarmHour(uint8_t hour)
{
    mAlarmHour = hour;
    mAlarmSet = 1;
}

uint8_t cTime::getAlarmHour()
{
    return mAlarmHour;
}

void cTime::setAlarmMinute(uint8_t minute)
{
    mAlarmMinute = minute;
    mAlarmSet = 1;
}

uint8_t cTime::getAlarmMinute()
{
    return mAlarmMinute;
}

cTime::cTime()
{
    mHours = 0;
    mMinutes = 0;
    mSeconds = 0;

    mAlarmHour = 0;
    mAlarmMinute = 0;
    mAlarmSet = 0;
}

void cTime::incTime()
{
    if (++mSeconds > 59)
    {
        mSeconds = 0;
        if (++mMinutes > 59)
        {
            mMinutes = 0;
            if (++mHours > 23)
                mHours = 0;
        }
        checkAlarms();
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

cTime::~cTime()
{
    // TODO Auto-generated destructor stub
}

cTime myTime;

