/*
 * mBtUart.cpp
 *
 *  Created on: 03 Mar 2017
 *      Author: christo
 */

#include "mBtUart.h"
#include <avr/interrupt.h>

#include <avr/io.h>
#include <string.h>
#include <terminal.h>

#define BAUD 115200
#define MYUBBR ((F_CPU / (BAUD * 8UL))-1)

mBtUart::mBtUart() {
	// TODO Auto-generated constructor stub

}

void mBtUart::init()
{
	DDRE &= ~(1 << 1);	//input
	DDRE |= (1 << 0);	//output


	//set up the BAUD Rate
	UBRR0H = (unsigned char)(MYUBBR >> 8);
	UBRR0L = (unsigned char)(MYUBBR);

	//double transmission speed
	UCSR0A |= (1 << U2X0);

	//set the character size to 8-bit
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

	//Enable RX | TX interrupt
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE1);
	//enable global interupts
	//sei()?
}

void mBtUart::transmit_byte(char b)
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

void mBtUart::transmit_command()
{
	char mCommand[7]  = {"at+ab "};

	for(uint8_t i=0;i<strlen(mCommand);i++)
		transmit_byte(mCommand[i]);
}

void mBtUart::run()
{
	if (mDataReady)
	{
		mDataReady = false;
		//this is an at command
//		if (mReceiveBuf[0] == 0x41 && mReceiveBuf[1] == 0x54 && mReceiveBuf[0] == 0x2D)
//		{
			printp("%s", mReceiveBuf);
			printp("%c", 0x0A);

			//clear the array and count
			mReceiveCount = 0;
			for (uint8_t i=0; i<sizeof(mReceiveBuf);i++)
				mReceiveBuf[i] = 0;
//		}
	}
}

void mBtUart::buildCommand(uint8_t com)
{
//	printp("%c", com);
	//while not \r
	if (com != 0x0A)
	{
		mReceiveBuf[mReceiveCount++] = com;
	} else if (mReceiveBuf[0] == 0x41 && mReceiveBuf[1] == 0x54)
	{
		printp("%s", mReceiveBuf);
		printp("%c", 0x0A);

		//clear the array and count
		mReceiveCount = 0;
		for(uint8_t i=0; i<sizeof(mReceiveBuf); i++)
			mReceiveBuf[i] = 0;
	} else
	{
		printp("bt data: %s \n", mReceiveBuf);
		//clear the array and count
		mReceiveCount = 0;
		mReceiveBuf[0] = 0;
//		dataReady = true;
	}
}

ISR(USART0_RX_vect)
{
	uint8_t RXdata;

	// wait for a byte to be received
	while(!(UCSR0A & (1 << RXC0)));

	RXdata = UDR0;

	mBt.buildCommand(RXdata);
//	printp("%c=%02X, ", RXdata, RXdata);
}


mBtUart::~mBtUart() {
	// TODO Auto-generated destructor stub
}

