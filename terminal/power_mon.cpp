/*
 * power_mon.cpp
 *
 *  Created on: 18 Aug 2017
 *      Author: christo
 */

#include "power_mon.h"

cPowerMon::cPowerMon(cOutput *sourceRelay, cOutput *chargerRelay, cAnalog *batteryAnalog, cAnalog *psuAnalog)
{
    relaySource = sourceRelay;
    relayCharger = chargerRelay;
    analogBattery = batteryAnalog;
    analogPSU = psuAnalog;

    source = POWER_SRC_BATTERY;

//    setState(POWER_SRC_PSU);
    voltageBattery = 0;
    voltagePSU = 0;
}

uint8_t cPowerMon::setState(PowerSource_t src)
{
    if (src == source)
        return 1;

    source = src;
    switch(src)
    {
    case POWER_SRC_BATTERY:
        /* Disable charger */
    {
        enableCharger(0);
        relaySource->reset();
    }
    break;
    case POWER_SRC_PSU:
        relaySource->set();
        break;
    }

    return 1;
}

uint8_t cPowerMon::enableCharger(uint8_t state)
{
    /* Disable charger */
    if(!state)
    {
        /* Charger is off  */
        if(!relayCharger->get())
            return 1;

        relayCharger->reset();
        return 1;
    }

    /* Battery currently in use */
    if(!relaySource->get())
        return 0;

    /* Enable the charger */
    relayCharger->set();

    return 1;
}

void cPowerMon::getVoltage(uint16_t *batteryV, uint16_t *psuV)
{
    *batteryV = voltageBattery;
    *psuV = voltagePSU;
}

void cPowerMon::run()
{
    static uint8_t tmp;
    if (tmp++ == 0)
    {
        if(analogPSU)
        {
            analogPSU->sample();
            voltagePSU = (uint16_t)(15000.0 * (analogPSU->lastSample() / 1024.0));
        }
    }
    else
    {
        tmp = 0;
        if(analogBattery)
        {
            analogBattery->sample();
            voltageBattery = (uint16_t)(15000.0 * (analogBattery->lastSample() / 1024.0));
        }
    }
}



cPowerMon::~cPowerMon()
{

}



