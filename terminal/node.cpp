/*
 * node.cpp
 *
 *  Created on: 25 Oct 2017
 *      Author: christo
 */

#include "node.h"

#include <string.h>
#include <RS485.h>



cNode::cNode(uint8_t id)
{
    mInitialized = false;
    mId = id;

    mState = NODE_STATE_UNKNOWN;
    memset(mPmwDuty, 0x00, NODE_NUMBER_PWM);
}

cNode::~cNode()
{

    // TODO Auto-generated destructor stub
}

uint8_t cNode::connect()
{

//    if()

    return 1;
}

void cNode::init(void)
{
}

void cNode::ping()
{
    memset(&msg, 0x00, CMSG_LENGTH);
    msg.deviceId = mId;
    msg.type = CMSG_TYPE_SET;
    msg.tag = CMSG_TAG_PING;
    uint8_t data[CMSG_LENGTH];
    memcpy(&data, &msg, CMSG_LENGTH);
    RS485.transmit_packet(data, CMSG_LENGTH);
}

uint8_t cNode::setPwm(uint8_t *pwms)
{

    return 1;
}

uint8_t cNode::setPwm(uint8_t pwmNumber, uint8_t duty)
{
    memset(&msg, 0x00, CMSG_LENGTH);
    msg.deviceId = mId;
    msg.type = CMSG_TYPE_SET;
    msg.tag = CMSG_TAG_PWM;
    msg.data0 = pwmNumber;
    msg.data1 = duty;

    uint8_t data[CMSG_LENGTH];
    memcpy(&data, &msg, CMSG_LENGTH);
    RS485.transmit_packet(data, CMSG_LENGTH);
    return 1;
}

uint8_t cNode::getPwm(uint8_t *pwms)
{

    return 1;
}

void cNode::run()
{
    switch (mState)
    {
    case NODE_STATE_INIT:
    {

    }
        break;
    case NODE_STATE_READY:
    {

    }
        break;
    case NODE_STATE_SEND:
    {

    }
        break;
    case NODE_STATE_RECEIVE:
    {

    }
        break;
    default:
        if (connect())
            mState = NODE_STATE_READY;
        break;
    }
}

