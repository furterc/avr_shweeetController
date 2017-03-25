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
        BT_SECONDS
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
