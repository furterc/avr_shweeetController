#include <stdio.h>
#include <avr/io.h>

#include "analog.h"

cAnalog::cAnalog(uint8_t channel) : mChannel(channel)
{
	mLastSample = 0;
	sample();
}
uint16_t cAnalog::lastSample()
{
	return mLastSample;
}

void cAnalog::sample()
{
	if(mChannel > 7)
		return;

	uint32_t sum = 0;

	//enable ADC
	ADCSRA = _BV(ADEN) | 7;

	//setup ADC
	ADMUX = 0x40 | mChannel;

	for(uint8_t k = 0; k < 16; k++)
	{
		//Start conversion
		ADCSRA |= _BV(ADSC);

		uint16_t cnt = 1000;
		//wait for conversion
		while((ADCSRA & _BV(ADSC)) && cnt)
		{
			cnt--;
			if(!cnt)
			{
				ADCSRA = 0;
				return;
			}
		}

		sum += ADC;
	}

	//disable ADC
	ADCSRA = 0;

	mLastSample = (sum >> 4);
}

cAnalog::~cAnalog()
{
}

