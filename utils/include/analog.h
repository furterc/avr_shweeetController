#ifndef ANALOG_H_
#define ANALOG_H_
#include <stdint.h>

class cAnalog
{
	uint8_t mChannel;
	uint16_t mLastSample;

public:
	cAnalog(uint8_t channel);
	virtual ~cAnalog();

	void sample();
	uint16_t lastSample();
};

#endif /* ANALOG_H_ */
