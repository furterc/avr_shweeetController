/*
 * cRemote.h
 *
 *  Created on: 19 Feb 2017
 *      Author: christo
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include <avr/io.h>

#include "RemoteButton.h"

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



#endif /* REMOTE_H_ */
