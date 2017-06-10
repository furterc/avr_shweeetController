#include <terminal.h>

#include "Remote.h"

cRemote::cRemote(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3)
{
    mInput[0] = cInputCB(pin0);
    mInput[1] = cInputCB(pin1);
    mInput[2] = cInputCB(pin2);
    mInput[3] = cInputCB(pin3);

    for (uint8_t i=0; i < 4; i++)
        mState[i] = mInput[i].getState();
}

void cRemote::setCB(void (*cb)(uint8_t button, bool state))
{
    if (!cb)
        return;

    callback = cb;
}

void cRemote::run()
{
    if (!callback)
        return;

    for (uint8_t j=0; j < 4; j++)
    {
        mInput[j].run();
        if (mState[j] != mInput[j].getState())
        {
            mState[j] = mInput[j].getState();
            callback(j, mState[j]);
        }
    }
}

cRemote::~cRemote()
{
}

