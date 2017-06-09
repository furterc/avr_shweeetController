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

#define BAUD 9600
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)
//#define MYUBBR 103

cOutput mWriteEnable = cOutput(PORT_PH(6));

cRS485::cRS485()
{
    mHead = 0;
    mTail = 0;
    sei();

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
    while (!(UCSR2A & (1 << UDRE2)));

    //write the data to the uart buffer
    UDR2 = b;

    //clear the transfer complete flag
    UCSR2A |= (1 << TXC2);
}

void cRS485::transmit_packet(uint8_t * buff, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        transmit_byte(buff[i]);
    }
    transmit_byte(0x0D);
}

void cRS485::handleCommand()
{
    mCommand[mTail] = 0;
    mHead = 0;
    mTail = 0;

    cPacket packet = cPacket();
    if (cPacket::check((uint8_t*) mCommand, &packet) == 1)
    {
        uint8_t idx = 0;
        const rs485_dbg_entry *currRS485Entry = rs485_dbg_entries[idx++];
        while (currRS485Entry)
        {
            if ((currRS485Entry->tag == packet.getTag()))
            {

                currRS485Entry->func(packet);
                return;
            }

            currRS485Entry = rs485_dbg_entries[idx++];
        }
        return;
    }

    printp("485!crc\n");
}

void cRS485::run()
{
    while (mHead != mTail)
    {
        if ((mCommand[mTail] == '\n') || (mCommand[mTail] == '\r'))
        {
            handleCommand();
        }
        else
            mTail++;
    }
}

void cRS485::handle(char ch)
{
    mCommand[mHead] = ch;

    if (++mHead > 63)
    {
        mHead = 0;
        mTail = 0;
    }
}

ISR(USART2_RX_vect)
{
    char ch = UDR2;
    RS485.handle(ch);
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
