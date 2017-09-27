/*
 * centurionRemote.cpp
 *
 *  Created on: 07 Jul 2017
 *      Author: christo
 */

#include "centurionRemote.h"
#include <terminal.h>

cCenturionRemote::cCenturionRemote(uint8_t pin) : cInputCB(pin)
{
    mState = getState();
}

void cCenturionRemote::runC()
{
    static uint8_t pulseLen;
    bool tempState = getState();

    if (mState == tempState)
    {
        if(!tempState)
            pulseLen++;

        return;
    }

    mState = tempState;

    uint8_t tmp = pulseLen;
    pulseLen = 0;

    if(!callback)
        return;

    callback(mState, tmp);
}

void cCenturionRemote::setCB(void (*cb)(bool state, uint8_t count))
{
    if(cb)
        callback = cb;
}

cCenturionRemote::~cCenturionRemote()
{
    // TODO Auto-generated destructor stub
}

