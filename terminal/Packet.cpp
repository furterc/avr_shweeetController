/*
 * cPacket.cpp
 *
 *  Created on: 19 Mar 2017
 *      Author: christo
 */

#include "Packet.h"

#define CRC_8_CONSTANT  0xE5

uint8_t cPacket::check(uint8_t * data, cPacket * packet)
{
    uint8_t mData[] =
    { data[0], data[1], data[2] };

    cPacket p = cPacket();
    uint8_t crc = p.crc8(mData, sizeof(mData));

    if (crc == data[3])
    {
        packet->mType = mData[0];
        packet->mTag = mData[1];
        packet->mData = mData[2];
        packet->mCrc = crc;
        return 1;
    }

    return 0;
}

uint8_t cPacket::crc8(uint8_t * data, uint8_t len)
{
    uint8_t crc = 0x00;
    uint8_t idx = 0x00;
    while (idx < len)
    {
        crc ^= data[idx];
        for (uint8_t i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC_8_CONSTANT;
            else
                crc <<= 1;
        }
        idx++;
    }
    return crc;
}

uint8_t cPacket::calcCrc()
{
    uint8_t data[] =
    { mType, mTag, mData };
    return crc8(data, sizeof(data));
}

cPacket::cPacket()
{
    mType = 0;
    mTag = 0;
    mData = 0;
    mCrc = 0;
}

cPacket::cPacket(uint8_t type, uint8_t tag, uint8_t data, uint8_t crc)
{
    mType = type;
    mTag = tag;
    mData = data;
    mCrc = crc;
}

cPacket::cPacket(Type type, Tag tag, uint8_t data)
{
    mType = type;
    mTag = tag;
    mData = data;
    mCrc = calcCrc();
}

uint8_t cPacket::getType()
{
    return mType;
}

uint8_t cPacket::getTag()
{
    return mTag;
}

uint8_t cPacket::getData()
{
    return mData;
}

uint8_t cPacket::getCrc()
{
    return mCrc;
}

void cPacket::setType(Type type)
{
    mType = type;
    mCrc = calcCrc();
}

void cPacket::setType(uint8_t type)
{
    mType = type;
    mCrc = calcCrc();
}

void cPacket::setTag(Tag tag)
{
    mTag = tag;
    mCrc = calcCrc();
}

void cPacket::setData(uint8_t data)
{
    mData = data;
    mCrc = calcCrc();
}

void cPacket::setCrc(uint8_t crc)
{
    mCrc = crc;
}
uint8_t cPacket::toBytes(uint8_t * bytes)
{
    bytes[0] = mType;
    bytes[1] = mTag;
    bytes[2] = mData;
    bytes[3] = mCrc;
    return 4;
}
cPacket::~cPacket()
{
    // TODO Auto-generated destructor stub
}

