/*
 * cRemote.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <inputCB.h>

class cRemote
{
    bool mState[4];
    cInputCB mInput[4] = {0,0,0,0};
    void (*callback)(uint8_t, bool) = 0;
public:
    cRemote(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void setCB(void (*cb)(uint8_t, bool));
    void run();
    virtual ~cRemote();
};

#endif /* REMOTE_H_ */
