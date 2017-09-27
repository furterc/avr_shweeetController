/*
 * cir_qeue.h
 *
 *  Created on: 27 Sep 2017
 *      Author: christo
 */

#ifndef INCLUDE_CIR_QUEUE_H_
#define INCLUDE_CIR_QUEUE_H_

#include "stdint-gcc.h"

#define MAX_QEUE_ITEMS 4

class cCirQueue
{
    uint8_t mHead;
    uint8_t mTail;

    void *data[MAX_QEUE_ITEMS];
public:
    cCirQueue();
    virtual ~cCirQueue();

    uint8_t putItem(void *obj);
    void *getItem();
};

#endif /* INCLUDE_CIR_QUEUE_H_ */
