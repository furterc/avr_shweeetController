/*
 * inputCB.cpp
 *
 *  Created on: 10 Jun 2017
 *      Author: christo
 */

#include "inputCB.h"

cInputCB::cInputCB(uint8_t pin) : cPort(pin)
{
    clearDDRn();
    mState = get();
}

void cInputCB::setCB(void (*cb)(bool state))
{
    if(cb)
        callback = cb;
}

void cInputCB::run()
{
    if(!callback)
        return;

    if(mState == get())
        return;

    mState = get();
    callback(mState);
}


cInputCB::~cInputCB()
{
}

