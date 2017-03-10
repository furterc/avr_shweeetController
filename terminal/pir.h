/*
 * pir.h
 *
 *  Created on: 29 Jan 2017
 *      Author: christo
 */

#ifndef PIR_H_
#define PIR_H_

#include "avr/io.h"

/* Setup Button port */
#define PIR_DDR DDRL
#define PIR_PIN PINL
#define PIR_INPUTPIN PL0



class pir {

	typedef enum {
		TRIGGERING,
		TRIGGERED,
		OFF
	} pir_state;

	pir_state pirState;

public:
	pir();
	void run();
	void pirOffsetCB(void (*cb)(void));
	void pirTriggersetCB(void (*cb)(void));

	virtual ~pir();
};

#endif /* PIR_H_ */
