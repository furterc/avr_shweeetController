/*
 * display.h
 *
 *  Created on: 19 Aug 2017
 *      Author: christo
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint-gcc.h>

extern "C"
{
#include "iicDisplay/lcdpcf8574.h"
}

class cDisplay
{
    void pad_zero(uint8_t value, char *str, uint8_t *idx);
    void showVoltage(uint16_t voltage, char *str, uint8_t *idx);

public:
    cDisplay();
    virtual ~cDisplay();

    void set_led(uint8_t state);
    void updateTime();
    void updateVoltage(uint16_t batVoltage, uint16_t psuVoltage);

};

#endif /* DISPLAY_H_ */
