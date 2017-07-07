#include <bluetooth.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <terminal.h>
#include <string.h>

#define BAUD 115200
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)

cBluetooth::cBluetooth()
{
    mDataReady = false;
    mCommandLen = 0;

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
    while (!(UCSR0A & (1 << UDRE0)))
        ;

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
    uint8_t frame_ptr[64];
    uint32_t frame_length = 64;
    cHDLCframer::frame(buff, 4, frame_ptr, &frame_length);

    for (uint8_t i = 0; i < frame_length; i++)
        transmit_byte(frame_ptr[i]);
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
    if (mDataReady)
    {
        mDataReady = false;
        handleCommand();
    }
}

void cBluetooth::handleCommand()
{
    mCommand[mCommandLen] = 0;

    uint8_t data[mCommandLen];
    memcpy(&data, mCommand, mCommandLen);
    memset(mCommand, 0xFF, 64);

    cMsg cmsgIn = cMsg(data);
    uint8_t idx = 0;
    const bt_dbg_entry *currBtEntry = bt_dbg_entries[idx++];
    while (currBtEntry)
    {
        if ((currBtEntry->tag == cmsgIn.getTag()))
        {
            currBtEntry->func(cmsgIn);
            return;
        }
        currBtEntry = bt_dbg_entries[idx++];
    }
}

void cBluetooth::handle(uint8_t ch)
{
    int rxLen = framer.pack(ch);
    if (rxLen)
    {
        mCommandLen = rxLen;
        memcpy(&mCommand, framer.buffer(), rxLen);
        mDataReady = true;
    }
}

cBluetooth::~cBluetooth()
{

}

ISR(USART0_RX_vect)
{
    Bluetooth.handle(UDR0);
}

cBluetooth Bluetooth;
