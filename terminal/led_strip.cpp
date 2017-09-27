/*
 * kitchen.cpp
 *
 *  Created on: 13 Jul 2017
 *      Author: christo
 */

#include "led_strip.h"

#include <bluetooth.h>
#include <terminal.h>
#include "BT_Commands.h"

#include <cMsg.h>
#include <RS485.h>

cMsg kcmsg;
void bedIO485(cMsg msg)
{
    if(msg.getType() == kcmsg.TYPE_GET)
        return;

    if(msg.getData0() == 0)
    {
        LedStrips.setKitchen(0);
        LedStrips.setStudy(0);
    }
    else if(msg.getData0() == 1)
    {
        LedStrips.setKitchen(0);
        LedStrips.setDutyDelayedSoft(&LedStrips.led_kitchen_bot, 0, 1, 20);
        LedStrips.setStudy(0);
    }
    else if(msg.getData0() == 2)
    {
        LedStrips.setKitchen(1);
        LedStrips.setStudy(1);
    }

    uint8_t data[4];


    msg.toBytes(data);
    msg.dbgPrint();
//    Bluetooth.transmit_packet(data, 4);
}
extern const rs485_dbg_entry bedIOEntry =
{ bedIO485, kcmsg.TAG_IO_BED };

void kitchen(cMsg msg)
{
    if (msg.getType() == msg.TYPE_GET)
    {
        msg.setType(msg.TYPE_SET);

        switch(msg.getData0())
        {
        case 0:
            msg.setData1(LedStrips.led_kitchen_top.pwm.getDutyC());
            break;
        case 1:
            msg.setData1(LedStrips.led_kitchen_bot.pwm.getDutyC());
            break;
        default:
            break;
        }

        uint8_t bytes[4];
        msg.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
    }
    else
    {
        switch(msg.getData0())
        {
        case 0:
            LedStrips.led_kitchen_top.pwmRunner.setDuty(5, msg.getData1());
            break;
        case 1:
            LedStrips.led_kitchen_bot.pwmRunner.setDuty(5, msg.getData1());
            break;
        default:
            break;
        }
    }
}
extern const bt_dbg_entry btKitchen =
{ kitchen, kcmsg.TAG_LED_KITCHEN };

void study(cMsg msg)
{
    if (msg.getType() == msg.TYPE_GET)
    {
        msg.setType(msg.TYPE_SET);

        if (msg.getData0() == 0)
            msg.setData1(LedStrips.led_study_top.pwm.getDutyC());
        else if (msg.getData0() == 11)
            msg.setData1(LedStrips.led_study_bot.pwm.getDutyC());

        uint8_t bytes[4];
        msg.toBytes(bytes);
        Bluetooth.transmit_packet(bytes, 4);
    }
    else
    {
        if (msg.getData0() == 0)
            LedStrips.led_study_top.pwmRunner.setDuty(5, msg.getData1());
        else if (msg.getData0() == 1)
            LedStrips.led_study_bot.pwmRunner.setDuty(5, msg.getData1());
    }
}
extern const bt_dbg_entry btStudy =
{ study, kcmsg.TAG_LED_STUDY };


void pirCB(bool state)
{
    printp("hier gebeur iets\n");
    LedStrips.pirTrg(state);

}

cLedStrip::cLedStrip()
{
    led_kitchen_top.pwmRunner.setPWM(&led_kitchen_top.pwm);
    led_kitchen_bot.pwmRunner.setPWM(&led_kitchen_bot.pwm);

    cTimerPwm tFivePwm = cTimerPwm(1, 5, &led_kitchen_top.pwm, &altLight.pwm, &led_kitchen_bot.pwm);
    tFivePwm.init();


    led_study_top.pwmRunner.setPWM(&led_study_top.pwm);
    led_study_bot.pwmRunner.setPWM(&led_study_bot.pwm);

    cTimerPwm tFourPwm = cTimerPwm(1, 4, &led_study_top.pwm, &led_study_bot.pwm, &led_study_alt.pwm);
    tFourPwm.init();



    pir.setCB(pirCB);

    //    kitchenState = LIGHT_OFF;
}

void cLedStrip::setDutySoft(ledStrip_t *strip, uint8_t timeStep, uint8_t duty)
{
    strip->pwmRunner.setDuty(timeStep, duty);
}

void cLedStrip::setDutyDelayedSoft(ledStrip_t *strip, uint8_t delay,  uint8_t timeStep, uint8_t duty)
{
    strip->pwmRunner.setDelayedDuty(timeStep, delay, duty);
}

uint8_t cLedStrip::run()
{
//    pir.run();
    uint8_t state = 0;
    state |= led_kitchen_top.pwmRunner.run();
    state |= led_kitchen_bot.pwmRunner.run();
    state |= led_study_top.pwmRunner.run();
    state |= led_study_bot.pwmRunner.run();
    return state;
}

void cLedStrip::pirTrg(bool state)
{
//    printp("POES PIR!\n");
}

void cLedStrip::softOn()
{

}

void cLedStrip::setKitchen(uint8_t state)
{
    if (state == 1)
    {
        LedStrips.setDutyDelayedSoft(&led_kitchen_top, 60, 1, 255);
        LedStrips.setDutyDelayedSoft(&led_kitchen_bot, 0, 1, 255);
    }
    else
    {
        LedStrips.setDutyDelayedSoft(&led_kitchen_top, 0, 1, 0);
        LedStrips.setDutyDelayedSoft(&led_kitchen_bot, 100, 1, 0);
    }

}

void cLedStrip::setStudy(uint8_t state)
{
    if (state == 1)
    {
        LedStrips.setDutyDelayedSoft(&led_study_top, 60, 1, 255);
        LedStrips.setDutyDelayedSoft(&led_study_bot, 0, 1, 255);
    }
    else
    {
        LedStrips.setDutyDelayedSoft(&led_study_top, 0, 1, 0);
        LedStrips.setDutyDelayedSoft(&led_study_bot, 100, 1, 0);
    }
}

cLedStrip::~cLedStrip()
{
    // TODO Auto-generated destructor stub
}

cLedStrip LedStrips;


