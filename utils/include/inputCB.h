/*
 * inputCB.h
 *
 *  Created on: 10 Jun 2017
 *      Author: christo
 */

#ifndef INPUTCB_H_
#define INPUTCB_H_

#include <port.h>

class cInputCB : public cPort
{
    void (*callback)(bool) = 0;
    bool mState;
public:
    cInputCB(uint8_t pin);
    virtual ~cInputCB();

    void setCB(void (*cb)(bool));
    bool getState();
    void run();
};

#endif /* INPUTCB_H_ */
