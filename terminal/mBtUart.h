/*
 * mBtUart.h
 *
 *  Created on: 03 Mar 2017
 *      Author: christo
 */

#ifndef MBTUART_H_
#define MBTUART_H_

#include <stdint-gcc.h>

class mBtUart {

	uint8_t mReceiveBuf[32];
	uint8_t mReceiveCount = 0;
	bool mDataReady = false;

	void cleanBuff();
public:



	mBtUart();
	void init();
	void transmit_byte(uint8_t);
	void transmit_packet(uint8_t *, uint8_t);
	void transmit_array(const char*);
	void run();
	void buildCommand(uint8_t);
	void transmit_command();
	virtual ~mBtUart();
};

extern mBtUart mBt;

#endif /* MBTUART_H_ */
