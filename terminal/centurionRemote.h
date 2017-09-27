/*
 * centurionRemote.h
 *
 *  Created on: 07 Jul 2017
 *      Author: christo
 */

#ifndef CENTURIONREMOTE_H_
#define CENTURIONREMOTE_H_

#include <stdbool.h>
#include <inputCB.h>

class cCenturionRemote : public cInputCB
{
    void (*callback)(bool,uint8_t) = 0;
    bool mState;
public:
    cCenturionRemote(uint8_t pin);
    virtual ~cCenturionRemote();

    void runC();
    void setCB(void (*cb)(bool,uint8_t));
};

#endif /* CENTURIONREMOTE_H_ */
