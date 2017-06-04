#ifndef PACKET_H_
#define PACKET_H_

#include "stdint-gcc.h"

class cPacket {

	uint8_t mType;
	uint8_t mTag;
	uint8_t mData;
	uint8_t mCrc;


	uint8_t calcCrc();

public:
    typedef enum {
        TYPE_GET,
        TYPE_SET
    } Type;

    typedef enum {
        BT_HOURS,
        BT_MINUTES,
        BT_SECONDS,
        BT_KITCH_TOP,
        BT_KITCH_BOT,
        BT_STUDY_TOP,
        BT_STUDY_BOT,
        BT_ALARM_HOUR,
        BT_ALARM_MINUTE,
        BT_ALARM_ACTIVE,
        BT_BED_0,
        BT_BED_1,
        BT_BED_2,
        BT_BED_3,
        BT_BED_4,
        BT_BED_5,
        BT_BED_6,
        BT_BED_7,
        BT_BED_8
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
