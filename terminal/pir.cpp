/*
 * pir.cpp
 *
 *  Created on: 29 Jan 2017
 *      Author: christo
 */

#include <terminal.h>
#include "pir.h"

void (*pirOffCallback)(void) = 0;
void (*pirTriggerCallback)(void) = 0;

pir::pir()
{
	//set input
	PIR_DDR &= ~(1 << PIR_INPUTPIN);
	pirState = OFF;
}


void pir::pirOffsetCB(void (*cb)(void))
{
	if(cb)
	{
		pirOffCallback = cb;
	}
}

void pir::pirTriggersetCB(void (*cb)(void))
{
	if(cb)
	{
		pirTriggerCallback = cb;
	}
}

void pir::run()
{
	/* switch changed */
		if (!(PIR_PIN & (1 << PIR_INPUTPIN)))
		{
			if (pirState != OFF)
			{
				printp("PIR off");
				pirState = OFF;
				if(pirOffCallback)
					pirOffCallback();
			}
		} else
		{
			if (pirState !=TRIGGERING)
			{
				printp("PIR triggering");
				pirState = TRIGGERING;
				if(pirTriggerCallback)
					pirTriggerCallback();
			}
		}
}

pir::~pir()
{

}

