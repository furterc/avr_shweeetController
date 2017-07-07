/*
 * cMsg.h
 *
 *  Created on: 09 Jun 2017
 *      Author: christo
 */

#ifndef CMSG_H_
#define CMSG_H_

#include "stdint.h"

class cMsg
{
    uint8_t mType;
    uint8_t mTag;
    uint8_t mData0;
    uint8_t mData1;

public:
    typedef enum
    {
        TYPE_GET, TYPE_SET
    } Type;

    typedef enum
    {
        TAG_TIME,       //0
        TAG_ALARM,      //1
        TAG_LED_KITCHEN,//2
        TAG_LED_STUDY,  //3
        TAG_LED_BED    //4
    } Tag;

    cMsg();
    cMsg(uint8_t * data);
    cMsg(uint8_t, uint8_t, uint8_t, uint8_t);

    uint8_t getTag();
    uint8_t getType();
    uint8_t getData0();
    uint8_t getData1();

    void setTag(cMsg::Tag);
    void setTag(uint8_t);

    void setType(Type);
    void setType(uint8_t);

    void setData0(uint8_t);
    void setData1(uint8_t);

    uint8_t toBytes(uint8_t * bytes);
    void dbgPrint();

    virtual ~cMsg();
};

#endif /* CMSG_H_ */
