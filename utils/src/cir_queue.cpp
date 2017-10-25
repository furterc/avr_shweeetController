/*
 * cir_qeue.cpp
 *
 *  Created on: 27 Sep 2017
 *      Author: christo
 */

#include <cir_queue.h>

#include <string.h>
#include <terminal.h>

cCirQueue::cCirQueue()
{
    mHead = 0;
    mTail = 0;
    memset(data, 0, MAX_QEUE_ITEMS);
}

cCirQueue::~cCirQueue()
{
    // TODO Auto-generated destructor stub
}

uint8_t cCirQueue::putItem(void *obj)
{
    uint8_t head = mHead;

    if(data[head])
    {
        printp("cQueue overrun!\n");
        return 0;
    }

    data[head] = obj;

    uint8_t newHead = (mHead + 1) % MAX_QEUE_ITEMS;
    mHead = newHead;

    return 1;
}

void *cCirQueue::getItem()
{
    void *obj = 0;
    obj = data[mTail];

    if(obj)
    {
        data[mTail] = 0;
        mTail = (mTail + 1) % MAX_QEUE_ITEMS;
    }
    return obj;
}
