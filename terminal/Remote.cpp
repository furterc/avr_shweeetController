#include <terminal.h>

#include "Remote.h"

cRemote::cRemote()
{
}

uint8_t cRemote::setCB(uint8_t channel, void (*cb)(void))
{
    if (!cb)
        return 0;

    if (channel > 0 && channel < 5)
    {
        channel--; //decrement one to index from zero

        //make pin input
        BTN_DDR &= ~(1 << inputPin[channel]);

        mBtn[channel].setCB(cb);
        mBtn[channel].setState(!(BTN_PIN & _BV(inputPin[channel])));

        return 1;
    }
    else
        return 0;
}

void cRemote::run()
{
    for (uint8_t idx = 0; idx < mBtnCount; idx++)
    {
        if (!(BTN_PIN & _BV(inputPin[idx])))
        {
            if (!mBtn[idx].getState())
            {
                mBtn[idx].setState(true);
                mBtn[idx].runCB();
                printp("Remote %d\n", idx + 1);
            }
        }
        else
        {
            if (mBtn[idx].getState())
            {
                mBtn[idx].setState(false);
                mBtn[idx].runCB();
                printp("Remote %d\n", idx + 1);
            }
        }
    }
}

cRemote::~cRemote()
{
    // TODO Auto-generated destructor stub
}

