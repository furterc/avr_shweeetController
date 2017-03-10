/*
 * cRemote.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef CREMOTE_H_
#define CREMOTE_H_

#include <avr/io.h>
#include "cRemoteButton.h"

	/* Setup Button port */
	#define BTN_DDR DDRL
	#define BTN_PIN PINL

class cRemote {

	static const uint8_t mBtnCount = 4;
	cRemoteButton mBtn[mBtnCount];
	uint8_t inputPin[4] = { PL2, PL1, PL6, PL7 };

public:
	cRemote();
	uint8_t setCB(uint8_t, void (*cb)(void));
	void run();
	virtual ~cRemote();
};



#endif /* CREMOTE_H_ */
