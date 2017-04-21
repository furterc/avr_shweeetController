#ifndef TIME_H_
#define TIME_H_

#include <stdint-gcc.h>

class cTime
{
    uint8_t mHours, mMinutes, mSeconds;

    uint8_t mAlarmSet;
    uint8_t mAlarmHour;
    uint8_t mAlarmMinute;

    void checkAlarms();

public:
    cTime();

    void setAlarm(uint8_t hour, uint8_t minute);
    void setAlarmHour(uint8_t);
    uint8_t getAlarmHour();
    void setAlarmMinute(uint8_t);
    uint8_t getAlarmMinute();

    void incTime();
    void setHours(uint8_t);
    uint8_t getHours();
    void setMinutes(uint8_t);
    uint8_t getMinutes();
    void setSeconds(uint8_t);
    uint8_t getSeconds();
    void printTime();
    virtual ~cTime();
};

extern cTime myTime;

#endif /* TIME_H_ */
