#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_
#include "led.h"

class cHeartbeat
{
	cLED *mLED;
	uint8_t mCount;

	enum eLEDstate
	{
		LED_ERROR,
		LED_HEARTBEAT,
		LED_WIFI,
		LED_CONNECTING,
	};

	eLEDstate mState;

	void error();
	void heartBeat();
	void WiFiup();
	void connecting();

public:
	cHeartbeat(cLED *led);
	virtual ~cHeartbeat();

	void init(uint8_t pin);

	void run();

	void setError(){ mState = LED_ERROR; };
	void setIdle(){ mState = LED_HEARTBEAT; };
	void setConnecting(){ mState = LED_CONNECTING; };
	void setWifi(){ mState = LED_WIFI; };
};

#endif /* HEARTBEAT_H_ */
