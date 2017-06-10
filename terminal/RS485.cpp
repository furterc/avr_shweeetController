/*
 * RS485.cpp
 *
 *  Created on: 29 Apr 2017
 *      Author: christo
 */

#include "RS485.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <terminal.h>
#include <output.h>
#include <string.h>

#define BAUD 9600
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)
//#define MYUBBR 103

cOutput mWriteEnable = cOutput(PORT_PH(6));

cRS485::cRS485()
{
    mDataReady = false;
    mCommandLen = 0;

    DDRH |= _BV(PH1);   //output

    //set up the BAUD Rate
    UBRR2H = (unsigned char) (MYUBBR >> 8);
    UBRR2L = (unsigned char) (MYUBBR);

    //double transmission speed
    UCSR2A |= _BV(U2X2);
    //set the character size to 8-bit
    UCSR2C = _BV(UCSZ20) | _BV(UCSZ21);
    //Enable RX | TX interrupt
    UCSR2B = _BV(TXEN2) | _BV(RXEN2) | _BV(RXCIE2) | _BV(TXCIE2);

    printp("RS485 started\n");
    mWriteEnable.reset();
}

void cRS485::transmit_byte(uint8_t b)
{
    mWriteEnable.set();
    //wait until the transmitter is ready
    while (!(UCSR2A & (1 << UDRE2)))
        ;

    //write the data to the uart buffer
    UDR2 = b;

    //clear the transfer complete flag
    UCSR2A |= (1 << TXC2);
}

void cRS485::transmit_packet(uint8_t * buff, uint8_t len)
{
    printp("485 data in: ");
    for (uint8_t j = 0; j < len; j++)
    {
        printp("%02X ", buff[j]);
    }
    printp("\n");

    uint8_t frame_ptr[64];
    uint32_t frame_length = 64;
    cHDLCframer::frame(buff, 4, frame_ptr, &frame_length);

    printp("hdlc len: %d\n", frame_length);

    printp("485 data out: ");
    for (uint8_t i = 0; i < frame_length; i++)
    {
        printp("%02X ", frame_ptr[i]);
        transmit_byte(frame_ptr[i]);
    }
}

void cRS485::handleCommand()
{
    mCommand[mCommandLen] = 0;

        uint8_t data[mCommandLen];
        memcpy(&data, mCommand, mCommandLen);
        memset(mCommand, 0xFF, 64);

        cMsg cmsgIn = cMsg(data);
        uint8_t idx = 0;
        const rs485_dbg_entry *curr485Entry = rs485_dbg_entries[idx++];
        while (curr485Entry)
        {
            if ((curr485Entry->tag == cmsgIn.getTag()))
            {
                curr485Entry->func(cmsgIn);
                return;
            }
            curr485Entry = rs485_dbg_entries[idx++];
        }
}

void cRS485::run()
{
    if (mDataReady)
        {
            mDataReady = false;
            handleCommand();
        }
}

void cRS485::handle(uint8_t ch)
{
    int rxLen = framer.pack(ch);
        if (rxLen)
        {
            mCommandLen = rxLen;
            memcpy(&mCommand, framer.buffer(), rxLen);
            mDataReady = true;
        }
}

ISR(USART2_RX_vect)
{
    RS485.handle(UDR2);
}

ISR(USART2_TX_vect)
{
    mWriteEnable.reset();
}

cRS485::~cRS485()
{
    // TODO Auto-generated destructor stub
}

cRS485 RS485;
