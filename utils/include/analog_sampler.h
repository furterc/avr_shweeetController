#ifndef ANALOG_MONITOR_H_
#define ANALOG_MONITOR_H_
#include <stdint.h>
#include <analog.h>

class cAnalogSampler
{
	uint8_t mTick;
	cAnalog **mAnalogInputs;

public:
	cAnalogSampler(cAnalog **analogInputs);
	virtual ~cAnalogSampler();

	void run();
};

#endif /* ANALOG_MONITOR_H_ */
