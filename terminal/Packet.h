#ifndef PACKET_H_
#define PACKET_H_

#include "stdint-gcc.h"

class cPacket
{

    uint8_t mType;
    uint8_t mTag;
    uint8_t mData;
    uint8_t mCrc;

    uint8_t calcCrc();

public:
    typedef enum
    {
        TYPE_GET, TYPE_SET
    } Type;

    typedef enum
    {
        BT_HOURS,       //0
        BT_MINUTES,     //1
        BT_SECONDS,     //2
        BT_KITCH_TOP,   //3
        BT_KITCH_BOT,   //4
        BT_STUDY_TOP,   //5
        BT_STUDY_BOT,   //6
        BT_ALARM_HOUR,  //7
        BT_ALARM_MINUTE,  //8
        BT_ALARM_ACTIVE,  //9
        SKIP_1,         //10
        BT_BED_0,       //11
        BT_BED_1,       //12
        SKIP_2,         //13
        BT_BED_2,       //14
        BT_BED_3,       //15
        BT_BED_4,       //16
        BT_BED_5,       //17
        BT_BED_6,       //18
        BT_BED_7,       //19
        BT_BED_8        //20
    } Tag;

    static uint8_t check(uint8_t *, cPacket * packet);

    cPacket();
    cPacket(Type, Tag, uint8_t);
    cPacket(uint8_t, uint8_t, uint8_t, uint8_t);

    uint8_t getTag();
    uint8_t getType();
    uint8_t getData();
    uint8_t getCrc();

    void setTag(Tag);
    void setTag(uint8_t);
    void setType(Type);
    void setType(uint8_t);
    void setData(uint8_t);
    void setCrc(uint8_t crc);
    uint8_t crc8(uint8_t * data, uint8_t len);
    uint8_t toBytes(uint8_t * bytes);
    virtual ~cPacket();
};

#endif /* PACKET_H_ */
