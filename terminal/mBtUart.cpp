/*
 * mBtUart.cpp
 *
 *  Created on: 03 Mar 2017
 *      Author: christo
 */

#include "mBtUart.h"
#include <avr/interrupt.h>

#include "Packet.h"


#include <avr/io.h>
#include <string.h>
#include <terminal.h>

#define BAUD 115200
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)

mBtUart::mBtUart()
{
    // TODO Auto-generated constructor stub

}

void mBtUart::init()
{
    DDRE &= ~(1 << 1);	//input
    DDRE |= (1 << 0);	//output

    //set up the BAUD Rate
    UBRR0H = (unsigned char) (MYUBBR >> 8);
    UBRR0L = (unsigned char) (MYUBBR);

    //double transmission speed
    UCSR0A |= (1 << U2X0);

    //set the character size to 8-bit
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    //Enable RX | TX interrupt
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE1);
}

void mBtUart::transmit_byte(uint8_t b)
{
    //wait until the transmitter is ready
    while (!(UCSR0A & (1 << UDRE0)));

    //write the data to the uart buffer
    UDR0 = b;

    //clear the transfer complete flag
    UCSR0A |= (1 << TXC0);
}

void mBtUart::transmit_array(const char *str)
{
    //determine the length of the array
    uint8_t sLength = strlen(str);
    //count through the array and send each individual character
    uint8_t idx = 0;
    while (idx != sLength)
    {
        transmit_byte(str[idx]);
        idx++;
    }
}

void mBtUart::transmit_packet(uint8_t * buff, uint8_t len)
{

    for (uint8_t i = 0; i < len; i++)
    {
        transmit_byte(buff[i]);
    }
    transmit_byte(0x0D);
}

void mBtUart::transmit_command()
{
    char mCommand[7] =
    { "at+ab " };

    for (uint8_t i = 0; i < strlen(mCommand); i++)
        transmit_byte(mCommand[i]);
}

void mBtUart::run()
{
//    if (mDataReady)
//    {
//        mDataReady = false;
//
//        printp("%s", mReceiveBuf);
//        printp("%c", 0x0A);
//
//        //clear the array and count
//        mReceiveCount = 0;
//        for (uint8_t i = 0; i < sizeof(mReceiveBuf); i++)
//            mReceiveBuf[i] = 0;
//
//    }
}

void mBtUart::buildCommand(uint8_t com)
{
//	printp("%c", com);
    //while not \r
    if (com != 0x0A)
    {
        mReceiveBuf[mReceiveCount++] = com;
        return;
    }

    if (com == 0x0D)
        return;

    if (mReceiveBuf[0] == 0x41 && mReceiveBuf[1] == 0x54)
    {
        printp("%s", mReceiveBuf);
        //newline
        printp("%c", 0x0A);

        //clear the array and count
        cleanBuff();
        return;
    }

    cPacket packet = cPacket();

    if (cPacket::check(mReceiveBuf, &packet)==1)
    {
        printp("0x%02X:0x%02X:0x%02X\n", packet.getType(), packet.getTag(), packet.getData(), packet.getCrc());
//        cleanBuff();
//        return;
    }

//        printp("bt data: %s \n", mReceiveBuf);
        //clear the array and count

        printp("data");
        for (uint8_t i=0;i < mReceiveCount;i++)
        {
            printp(":0x%02X:", mReceiveBuf[i]);
        }
        printp("\n");
        cleanBuff();


//        mReceiveCount = 0;
//        mReceiveBuf[0] = 0;
//		dataReady = true;
}

void mBtUart::cleanBuff()
{
    mReceiveCount = 0;
                for (uint8_t i = 0; i < sizeof(mReceiveBuf); i++)
                    mReceiveBuf[i] = 0;
}

ISR(USART0_RX_vect)
{
    // wait for a byte to be received
    while (!(UCSR0A & (1 << RXC0)));

    uint8_t RXdata = UDR0;

    mBt.buildCommand(RXdata);
//	printp("%c=%02X, ", RXdata, RXdata);
}

mBtUart::~mBtUart()
{
    // TODO Auto-generated destructor stub
}

