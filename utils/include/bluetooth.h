#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint-gcc.h>
#include <avr/pgmspace.h>
//#include "Packet.h"
#include "cMsg.h"
#include "hdlc_framer.h"
#include "cir_queue.h"
typedef void (*bt_dbg_func)(cMsg packet);

struct bt_dbg_entry
{
    bt_dbg_func func;
    const uint8_t tag;
};

void help();

class cBluetooth
{
    cHDLCframer framer = cHDLCframer(32, 2);

    cCirQueue queue = cCirQueue();

    void handleCommand(cmsg_t *obj);
    void transmit_byte(uint8_t b);

public:
    void transmit_array(const char *str);
    void transmit_packet(uint8_t * buff, uint8_t len);
    void transmit_command();

    cBluetooth();
    void run();
    void handle(uint8_t);
    virtual ~cBluetooth();
};

extern cBluetooth Bluetooth;
extern const bt_dbg_entry* bt_dbg_entries[];

#endif /* BLUETOOTH_H_ */
