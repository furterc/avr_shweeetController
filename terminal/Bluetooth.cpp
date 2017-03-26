#include "Bluetooth.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <terminal.h>
#include <string.h>

#include "BT_Commands.h"

#define BAUD 115200
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)

cBluetooth::cBluetooth()
{
    mHead = 0;
    mTail = 0;

    DDRE &= ~(1 << 1);  //input
    DDRE |= (1 << 0);   //output

    //set up the BAUD Rate
    UBRR0H = (unsigned char) (MYUBBR >> 8);
    UBRR0L = (unsigned char) (MYUBBR);

    //double transmission speed
    UCSR0A |= _BV(U2X0);
    //set the character size to 8-bit
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
    //Enable RX | TX interrupt
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE1);

    printp("BT started\n");
}

void cBluetooth::transmit_byte(uint8_t b)
{
    //wait until the transmitter is ready
    while (!(UCSR0A & (1 << UDRE0)));

    //write the data to the uart buffer
    UDR0 = b;

    //clear the transfer complete flag
    UCSR0A |= (1 << TXC0);
}

void cBluetooth::transmit_array(const char *str)
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

void cBluetooth::transmit_packet(uint8_t * buff, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        transmit_byte(buff[i]);
    }
    transmit_byte(0x0D);
}

void cBluetooth::transmit_command()
{
    char mCommand[7] =
    { "at+ab " };

    for (uint8_t i = 0; i < strlen(mCommand); i++)
        transmit_byte(mCommand[i]);
}

void cBluetooth::run()
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

void cBluetooth::handleCommand()
{
    mCommand[mTail] = 0;
    mHead = 0;
    mTail = 0;

    if (mCommand[0] == 0x41 && mCommand[1] == 0x54)
    {
        printp("%s\n\r", mCommand);
        return;
    }

    cPacket packet = cPacket();
    if (cPacket::check((uint8_t*) mCommand, &packet) == 1)
    {
        printp("0x%02X:0x%02X:0x%02X\n", packet.getType(), packet.getTag(),
                packet.getData(), packet.getCrc());

        uint8_t idx = 0;
        const bt_dbg_entry *currBtEntry = bt_dbg_entries[idx++];
        while (currBtEntry)
        {
            //printp("%s - %s\n", currEntry->command, mCommand);
            if ((currBtEntry->tag == packet.getTag()))
            {
                currBtEntry->func(packet);
                return;
            }

            currBtEntry = bt_dbg_entries[idx++];
        }
        return;
    }

    printp("bt: %s\n\r", mCommand);
}

void cBluetooth::handle(char ch)
{
    mCommand[mHead] = ch;

    if (++mHead > 63)
    {
        mHead = 0;
        mTail = 0;
    }
}

cBluetooth::~cBluetooth()
{
    // TODO Auto-generated destructor stub
}

ISR(USART0_RX_vect)
{
    char ch = UDR0;
    Bluetooth.handle(ch);
}

cBluetooth Bluetooth;
