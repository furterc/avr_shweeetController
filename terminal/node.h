/*
 * node.h
 *
 *  Created on: 25 Oct 2017
 *      Author: christo
 */

#ifndef NODE_H_
#define NODE_H_

#define NODE_NUMBER_PWM     2

#include <stdint-gcc.h>
#include <stdbool.h>

#include "cmsg2.h"

class cNode
{
    typedef enum{
        NODE_STATE_UNKNOWN,
        NODE_STATE_INIT,
        NODE_STATE_READY,
        NODE_STATE_SEND,
        NODE_STATE_RECEIVE,
    }mNodeState_t;

    mNodeState_t mState;
    uint8_t mId;
    bool mInitialized;
    uint8_t mPmwDuty[NODE_NUMBER_PWM];
    mCMsg_t msg;

    uint8_t connect();

public:
    cNode(uint8_t id);
    virtual ~cNode();

    void init();
    void ping();

    uint8_t setPwm(uint8_t *pwms);
    uint8_t setPwm(uint8_t pwmNumber, uint8_t duty);
    uint8_t getPwm(uint8_t *pwms);

    void run();

};

#endif /* NODE_H_ */
