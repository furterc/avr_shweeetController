#include "analog_sampler.h"

#define SAMPLE_PERIOD 10  //set to be 1 second

cAnalogSampler::cAnalogSampler(cAnalog **analogInputs) : mAnalogInputs(analogInputs)
{
	mTick = SAMPLE_PERIOD;
}

void cAnalogSampler::run()
{
	if(mTick--)
		return;
	mTick = SAMPLE_PERIOD;

	if(!mAnalogInputs)
		return;

	uint8_t k = 0;
	cAnalog *port = mAnalogInputs[k++];
	while(port)
	{
		port->sample();
		port = mAnalogInputs[k++];
	}
}

cAnalogSampler::~cAnalogSampler()
{
}

