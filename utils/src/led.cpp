#include <avr/io.h>

#include "led.h"

cLED::cLED(cOutput *red, cOutput *green) : mGreen(green), mRed(red)
{
}


void cLED::off()
{
	mRed->reset();
	mGreen->reset();
}

void cLED::red()
{
	mRed->set();
	mGreen->reset();
}

void cLED::green()
{
	mRed->reset();
	mGreen->set();
}

cLED::~cLED()
{
}

