/*
 * RS485.h
 *
 *  Created on: 29 Apr 2017
 *      Author: christo
 */

#ifndef RS485_H_
#define RS485_H_

#include <stdint-gcc.h>

#include "cMsg.h"
#include "hdlc_framer.h"
typedef void (*rs485_dbg_func)(cMsg packet);

struct rs485_dbg_entry
{
    rs485_dbg_func func;
    const uint8_t tag;
};

class cRS485
{
    cHDLCframer framer = cHDLCframer(32);

    bool mDataReady;
    char mCommand[64];
    uint8_t mCommandLen;

    void handleCommand();
    void transmit_byte(uint8_t b);
public:
    cRS485();
    void transmit_packet(uint8_t * buff, uint8_t len);
    void run();
    void handle(uint8_t);

    virtual ~cRS485();
};

extern cRS485 RS485;
extern const rs485_dbg_entry* rs485_dbg_entries[];

#endif /* RS485_H_ */
