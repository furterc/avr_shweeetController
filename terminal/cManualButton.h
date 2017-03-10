/*
 * cManualButton.h
 *
 *  Created on: 23 Feb 2017
 *      Author: christo
 */

#ifndef CMANUALBUTTON_H_
#define CMANUALBUTTON_H_

#include <terminal.h>

class cManualButton {

	/* Setup Button port */
	#define MAN_BTN_DDR DDRH
	#define MAN_BTN_PIN PINH
	#define MAN_BTN_INPUTPIN PH0

	bool mState = false;
	uint8_t mCount = 0;

public:
	cManualButton();
	void run();
	uint8_t getCount();
	void btnSetCB(void (*cb)(void));
	virtual ~cManualButton();
};

#endif /* CMANUALBUTTON_H_ */
