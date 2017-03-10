#ifndef LED_H_
#define LED_H_
#include <avr/io.h>

#include "output.h"

class cLED
{
	cOutput *mGreen;
	cOutput *mRed;

public:
	cLED(cOutput *red, cOutput *green);
	virtual ~cLED();

	void off();
	void red();
	void green();
};

#endif /* LED_H_ */
