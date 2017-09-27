/*
 * power_mon.h
 *
 *  Created on: 18 Aug 2017
 *      Author: christo
 */

#ifndef SRC_POWER_MON_H_
#define SRC_POWER_MON_H_

#include <stdint-gcc.h>

#include "output.h"
#include "analog.h"

class cPowerMon
{
    cOutput *relaySource;
    cOutput *relayCharger;
    cAnalog *analogBattery;
    cAnalog *analogPSU;

    uint16_t voltageBattery;
    uint16_t voltagePSU;

public:
    typedef enum
    {
        POWER_SRC_BATTERY,
        POWER_SRC_PSU
    }PowerSource_t;

    PowerSource_t source;

    cPowerMon(cOutput *sourceRelay, cOutput *chargerRelay, cAnalog *batteryAnalog, cAnalog *psuAnalog);
    virtual ~cPowerMon();

    uint8_t setState(PowerSource_t src);
    uint8_t enableCharger(uint8_t state);
    void getVoltage(uint16_t *batteryV, uint16_t *psuV);
    void run();
};

#endif /* SRC_POWER_MON_H_ */
