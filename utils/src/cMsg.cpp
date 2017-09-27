/*
 * cMsg.cpp
 *
 *  Created on: 09 Jun 2017
 *      Author: christo
 */

#include "cMsg.h"
#include <terminal.h>

cMsg::cMsg()
{
    msg.type = 0;
    msg.tag = 0;
    msg.data0 = 0;
    msg.data1 = 0;
}

cMsg::cMsg(cmsg_t data)
{
    msg = data;
}

cMsg::cMsg(uint8_t * data)
{
    msg.type = data[0];
    msg.tag = data[1];
    msg.data0 = data[2];
    msg.data1 = data[3];
}

cMsg::cMsg(uint8_t type, uint8_t tag, uint8_t data0, uint8_t data1)
{
    msg.type = type;
    msg.tag = tag;
    msg.data0 = data0;
    msg.data1 = data1;
}

uint8_t cMsg::getType()
{
    return msg.type;
}

uint8_t cMsg::getTag()
{
    return msg.tag;
}

uint8_t cMsg::getData0()
{
    return msg.data0;
}
uint8_t cMsg::getData1()
{
    return msg.data1;
}

void cMsg::setTag(Tag tag)
{
    msg.tag = tag;
}

void cMsg::setTag(uint8_t tag)
{
    msg.tag = tag;
}

void cMsg::setType(Type type)
{
    msg.type = type;
}

void cMsg::setType(uint8_t type)
{
    msg.type = type;
}

void cMsg::setData0(uint8_t data0)
{
    msg.data0 = data0;
}

void cMsg::setData1(uint8_t data1)
{
    msg.data1 = data1;
}

uint8_t cMsg::toBytes(uint8_t * bytes)
{
    bytes[0] = msg.type;
    bytes[1] = msg.tag;
    bytes[2] = msg.data0;
    bytes[3] = msg.data1;
    return 4;
}

void cMsg::dbgPrint()
{
    printp("cMsg\n type: %02X\n tag: %02X\n data0: %02X\n data1: %02X\n", msg.type, msg.tag, msg.data0, msg.data1);
}

cMsg::~cMsg()
{
    // TODO Auto-generated destructor stub
}

