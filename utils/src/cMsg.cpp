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
    mType = 0;
    mTag = 0;
    mData0 = 0;
    mData1 = 0;
}

cMsg::cMsg(uint8_t * data)
{
    mType = data[0];
    mTag = data[1];
    mData0 = data[2];
    mData1 = data[3];
}

cMsg::cMsg(uint8_t type, uint8_t tag, uint8_t data0, uint8_t data1)
{
    mType = type;
    mTag = tag;
    mData0 = data0;
    mData1 = data1;
}

uint8_t cMsg::getType()
{
    return mType;
}

uint8_t cMsg::getTag()
{
    return mTag;
}

uint8_t cMsg::getData0()
{
    return mData0;
}
uint8_t cMsg::getData1()
{
    return mData1;
}

void cMsg::setTag(Tag tag)
{
    mTag = tag;
}

void cMsg::setTag(uint8_t tag)
{
    mTag = tag;
}

void cMsg::setType(Type type)
{
    mType = type;
}

void cMsg::setType(uint8_t type)
{
    mType = type;
}

void cMsg::setData0(uint8_t data0)
{
    mData0 = data0;
}

void cMsg::setData1(uint8_t data1)
{
    mData1 = data1;
}

uint8_t cMsg::toBytes(uint8_t * bytes)
{
    bytes[0] = mType;
    bytes[1] = mTag;
    bytes[2] = mData0;
    bytes[3] = mData1;
    return 4;
}

void cMsg::dbgPrint()
{
    printp("cMsg\n type: %02X\n tag: %02X\n data0: %02X\n data1: %02X\n", mType, mTag, mData0, mData1);
}

cMsg::~cMsg()
{
    // TODO Auto-generated destructor stub
}

