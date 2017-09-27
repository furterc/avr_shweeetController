/*
 * kitchen.h
 *
 *  Created on: 13 Jul 2017
 *      Author: christo
 */

#ifndef LED_STRIP_H_
#define LED_STRIP_H_

#include <pwm.h>
#include <inputCB.h>
#include "PwmRun.h"
#include "timer_pwm.h"


class cLedStrip
{

    typedef struct
    {
        cPWM pwm;
        cPwmRun pwmRunner;
    }ledStrip_t;

    cInputCB pir = cInputCB(PORT_PL(0));


//    kitchenState_t kitchenState;

public:
    ledStrip_t led_kitchen_top;
    ledStrip_t led_kitchen_bot;
    ledStrip_t altLight;
    ledStrip_t led_study_top;
    ledStrip_t led_study_bot;
    ledStrip_t led_study_alt;

    cLedStrip();
    virtual ~cLedStrip();

    void setDutySoft(ledStrip_t *strip, uint8_t timeStep, uint8_t duty);
    void setDutyDelayedSoft(ledStrip_t *strip, uint8_t delay, uint8_t timeStep, uint8_t duty);
    uint8_t run();
    void pirTrg(bool state);
    void softOn();
    void setKitchen(uint8_t state);
    void setStudy(uint8_t state);
};

extern cLedStrip LedStrips;

#endif /* LED_STRIP_H_ */
