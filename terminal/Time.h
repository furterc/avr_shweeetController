#ifndef TIME_H_
#define TIME_H_

#include <stdint-gcc.h>

class cTime {
	uint8_t mHours ,mMinutes, mSeconds;
public:
	cTime();
	void incTime();
	void setHours(uint8_t);
	uint8_t getHours();
	void setMinutes(uint8_t);
	uint8_t getMinutes();
	void setSeconds(uint8_t);
	uint8_t getSeconds();
	void printTime();
	virtual ~cTime();
};

#endif /* TIME_H_ */
