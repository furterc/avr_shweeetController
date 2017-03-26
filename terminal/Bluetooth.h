#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint-gcc.h>
#include <avr/pgmspace.h>
#include "Packet.h"

typedef void (*bt_dbg_func)(cPacket packet);

struct bt_dbg_entry
{
    bt_dbg_func func;
    const uint8_t tag;
};

void help();

class cBluetooth
{
    uint8_t mHead;
    uint8_t mTail;
    char mCommand[64];

    void handleCommand();
    void transmit_byte(uint8_t b);
public:
    void transmit_array(const char *str);
    void transmit_packet(uint8_t * buff, uint8_t len);
    void transmit_command();

    cBluetooth();
    void run();
    void handle(char);
    virtual ~cBluetooth();
};

extern cBluetooth Bluetooth;
extern const bt_dbg_entry* bt_dbg_entries[];

#endif /* BLUETOOTH_H_ */
