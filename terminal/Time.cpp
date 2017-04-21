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
#include "Packet.h"

#include "Lights.h"

cPacket mPacket;
void hourEntry(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
{
    packet.setType(packet.TYPE_SET);

    packet.setData(myTime.getHours());
    uint8_t bytes[4];
    packet.toBytes(bytes);
    Bluetooth.transmit_packet(bytes, 4);
    return;
}
else
    myTime.setHours(packet.getData());
}
extern const bt_dbg_entry btHourEntry =
{ hourEntry, mPacket.BT_HOURS };
void minuteEntry(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
{
    packet.setType(packet.TYPE_SET);

    packet.setData(myTime.getMinutes());
    uint8_t bytes[4];
    packet.toBytes(bytes);
    Bluetooth.transmit_packet(bytes, 4);
    return;
}
else
    myTime.setMinutes(packet.getData());
}
extern const bt_dbg_entry btMinuteEntry =
{ minuteEntry, mPacket.BT_MINUTES };

void secondEntry(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
{
    packet.setType(packet.TYPE_SET);

    packet.setData(myTime.getSeconds());
    uint8_t bytes[4];
    packet.toBytes(bytes);
    Bluetooth.transmit_packet(bytes, 4);
    return;
}
else
    myTime.setSeconds(packet.getData());
}
extern const bt_dbg_entry btSecondEntry =
{ secondEntry, mPacket.BT_SECONDS };

void alarmHour(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
{
    packet.setType(packet.TYPE_SET);

    packet.setData(myTime.getAlarmHour());
    uint8_t bytes[4];
    packet.toBytes(bytes);
    Bluetooth.transmit_packet(bytes, 4);
    return;
}
else
    myTime.setAlarmHour(packet.getData());

    printp("aHour: %d\n", packet.getData());
}
extern const bt_dbg_entry btAlarmHour =
{ alarmHour, mPacket.BT_ALARM_HOUR };

void alarmMinute(cPacket packet)
{
    if (packet.getType() == packet.TYPE_GET)
{
    packet.setType(packet.TYPE_SET);

    packet.setData(myTime.getAlarmMinute());
    uint8_t bytes[4];
    packet.toBytes(bytes);
    Bluetooth.transmit_packet(bytes, 4);
    return;
}
else
    myTime.setAlarmMinute(packet.getData());
    printp("aMinute: %d\n", packet.getData());
}
extern const bt_dbg_entry btAlarmMinute= { alarmMinute, mPacket.BT_ALARM_MINUTE};

void cTime::checkAlarms()
{
    if (mAlarmSet == 0)
       return;

    if (mAlarmHour == mHours && mAlarmMinute == mMinutes)
    {
        mLights.setSoft(mLights.LIGHT_KITCHEN_TOP, 255, 255);
        mLights.setSoft(mLights.LIGHT_KITCHEN_BOT, 255, 255);

        printp("fokinpoeswat alarm!!!\n");
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

