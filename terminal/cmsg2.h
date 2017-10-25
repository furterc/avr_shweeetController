/*
 * cmsg.h
 *
 *  Created on: 08 Oct 2017
 *      Author: christo
 */

#ifndef SRC_CMSG2_H_
#define SRC_CMSG2_H_

#include <stdint-gcc.h>

#define CMSG_LENGTH     5
typedef enum{
	CMSG_TYPE_SET,
	CMSG_TYPE_GET,
} mCMsgType_t;

typedef enum{
    CMSG_TAG_PING,
	CMSG_TAG_PWM,
	CMSG_TAG_INPUT,
} mCMsgTag_t;

typedef struct {
    uint8_t deviceId;
	mCMsgType_t type;
	mCMsgTag_t tag;
	uint8_t data0;
	uint8_t data1;
} mCMsg_t;

#endif /* SRC_CMSG2_H_ */
