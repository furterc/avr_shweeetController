#include <float.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <terminal.h>
#include <led.h>
#include <port.h>
#include <heartbeat.h>

#include <inputCB.h>

#include "timer_two.h"
#include "cMsg.h"
#include <bluetooth.h>

//#include "Lights.h"
#include "Remote.h"
#include "Time.h"
#include "RS485.h"
#include "centurionRemote.h"
#include "power_mon.h"
#include "display.h"

#include <analog.h>
#include "led_strip.h"


cDisplay Display = cDisplay();

void watchdogReset()
{
    __asm__ __volatile__ ( "wdr\n" );
}

void printBedPWM(uint8_t i, uint8_t duty)
{
    printp("pwm%d\t: %d\n", i, duty)
}

void switchBedLigts(bool enabled)
{
    cMsg msg = cMsg(0,0,0,0);
    msg.setType(msg.TYPE_SET);
    msg.setTag(msg.TAG_LED_BED);

    if(!enabled)
        msg.setData1(0);
    else
        msg.setData1(255);

    msg.setData0(0);
    uint8_t data[4];
    msg.toBytes(data);
    RS485.transmit_packet(data,4);

    _delay_ms(150);
    msg.setData0(1);
    msg.toBytes(data);
    RS485.transmit_packet(data,4);

    _delay_ms(150);
    msg.setData0(2);
    msg.toBytes(data);
    RS485.transmit_packet(data,4);

    _delay_ms(150);
    msg.setData0(3);
    msg.toBytes(data);
    RS485.transmit_packet(data,4);
}

/* Pass messages between 485 and bluetooth*/
cMsg tmsg = cMsg();
void bed485(cMsg msg)
{
    uint8_t data[4];
    msg.toBytes(data);
    msg.dbgPrint();
    Bluetooth.transmit_packet(data, 4);
}
extern const rs485_dbg_entry bedLEDEntry =
{ bed485, tmsg.TAG_LED_BED };

void bed(cMsg msg)
{
    uint8_t data[4];
    msg.toBytes(data);
    printf("RS485out\n");
    msg.dbgPrint();
    RS485.transmit_packet(data, 4);
}
extern const bt_dbg_entry btBed =
{ bed, tmsg.TAG_LED_BED };

void btCommandSend(uint8_t argc, char **argv)
{
    Bluetooth.transmit_command();

    if (argc > 1)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            Bluetooth.transmit_array(argv[i + 1]);
            Bluetooth.transmit_array(" ");
        }
    }
    Bluetooth.transmit_array("\r");
}
extern const dbg_entry btCommandSendEntry =
{ btCommandSend, "at+ab" };

//cOutput workspaceLED = cOutput(PORT_PH(6));
void light(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        //display data
        printp("Kitchen\n   Top: %d\n   Bot: %d\n",
                LedStrips.led_kitchen_top.pwm.getDutyC(),
                LedStrips.led_kitchen_bot.pwm.getDutyC());
        printp("Study\n   Top: %d\n   Bot: %d\n",
                LedStrips.led_study_top.pwm.getDutyC(),
                LedStrips.led_study_bot.pwm.getDutyC());
        return;
    }

    if (argc == 2)
    {
        if(!strcmp(argv[1], "k"))
        {
            if(LedStrips.led_kitchen_top.pwm.getDutyC())
                LedStrips.setKitchen(0);
            else
                LedStrips.setKitchen(1);

        }

        if(!strcmp(argv[1], "s"))
        {
            if(LedStrips.led_study_top.pwm.getDutyC())
                LedStrips.setStudy(0);
            else
                LedStrips.setStudy(1);

        }

        if(!strcmp(argv[1], "b"))
        {
            static uint8_t tmp;
            if(tmp++ == 0)
            {
                switchBedLigts(1);
            }
            else
            {
                tmp = 0;
                switchBedLigts(0);
            }
        }
    }

}
extern const dbg_entry lightEntry =
{ light, "l" };

void time(uint8_t argc, char **argv)
{
    if (argc > 1)
    {
        myTime.setHours(atoi(argv[1]));
        myTime.setMinutes(atoi(argv[2]));
        myTime.setSeconds(atoi(argv[3]));
    }
    else
    {
        printp("%02d:%02d:%02d", myTime.getHours(), myTime.getMinutes(),
                myTime.getSeconds());
    }

}
extern const dbg_entry timeEntry =
{ time, "t" };

cAnalog battVoltage = cAnalog(4);
cAnalog psuVoltage = cAnalog(5);
cOutput powerRelay = cOutput(PORT_PF(3));
cOutput chargerRelay = cOutput(PORT_PF(0));

cPowerMon powerMon = cPowerMon(&powerRelay, &chargerRelay, &battVoltage, &psuVoltage);
void power(uint8_t argc, char **argv)
{
    if (argc == 3)
    {
        if(!strcmp(argv[1], "c"))
        {
            uint8_t state = atoi(argv[2]);

            if (!powerMon.enableCharger(state))
            {
                printp("bat in use\n");
                return;
            }
            else
            {
                printp("charger enabled\n");
            }
        }
    }
    else if (argc == 2)
    {
        uint8_t state = atoi(argv[1]);
        if (state == 1)
        {
            powerMon.setState(powerMon.POWER_SRC_PSU);
            printf("psu ");
        }
        else
        {
            powerMon.setState(powerMon.POWER_SRC_BATTERY);
            printf("bat ");
        }
        printf("selected\n");
    }
    else
    {
        printp("pwr: ");
        if (powerRelay.get())
        {
            printp("psu\n");
            return;
        }

        printp("bat\n");
    }

}
extern const dbg_entry powerEntry =
{ power, "p" };

void btSend(uint8_t argc, char **argv)
{
    uint16_t bVoltage = (uint16_t)(15000.0 * (battVoltage.lastSample() / 1024.0));
    printf("batV: %d mV\n", bVoltage);

    uint16_t pVoltage = (uint16_t)(15000.0 * (psuVoltage.lastSample() / 1024.0));
    printf("psuV: %d mV\n", pVoltage);

}
extern const dbg_entry btSendEntry =
{ btSend, "b" };

void reboot(uint8_t argc, char **argv)
{
    printp("rebooting..\n");
    do
    {
        for (;;)
        {
        }
    } while (0);
}
extern const dbg_entry rebootEntry =
{ reboot, "r" };

void btReset(uint8_t argc, char **argv)
{
    cOutput bt_reset(PORT_PG(5));
    bt_reset.reset();
    _delay_ms(100);
    bt_reset.set();
}
extern const dbg_entry btResetEntry =
{ btReset, "btr" };

void remoteCB(uint8_t button, bool state)
{
    switch (button)
    {
    case 0:
//        mLights.incLevel(mLights.LIGHT_KITCHEN_TOP);
        break;
    case 1:
//        mLights.incLevel(mLights.LIGHT_KITCHEN_BOT);
        break;
    case 2:
//        mLights.incLevel(mLights.LIGHT_STUDY_BOT);
        break;
    case 3:
//        mLights.incLevel(mLights.LIGHT_STUDY_TOP);
        break;
    default:
        break;
    }
}

void btn4CB(bool state)
{
    static bool setted;
    //only interrupt upon click
    if (state)
    {
        if(!setted)
        {
            printp("kitchen on\n");

            LedStrips.setDutySoft(&LedStrips.led_kitchen_top, 1, 255);
            LedStrips.setDutySoft(&LedStrips.led_kitchen_bot, 1, 255);
            setted = true;
        }
        else
        {
            printp("kitchen off\n");
            LedStrips.setDutySoft(&LedStrips.led_kitchen_top, 1, 0);
            LedStrips.setDutySoft(&LedStrips.led_kitchen_bot, 1, 0);
            setted = false;
        }
    }
}

void btn5CB(bool state)
{
    static bool setted;
    //only interrupt upon click
    if (state)
    {
        if(!setted)
        {
            printp("study on\n");
            LedStrips.setDutySoft(&LedStrips.led_study_top, 1, 255);
            LedStrips.setDutySoft(&LedStrips.led_study_bot, 1, 255);


            setted = true;
        }
        else
        {
            printp("study off\n");
            LedStrips.setDutySoft(&LedStrips.led_study_top, 1, 0);
            LedStrips.setDutySoft(&LedStrips.led_study_bot, 1, 0);
            setted = false;
        }
    }
}

void timerTwoCB(void)
{
    myTime.incTime();
}




void hekRemoteCB(bool state, uint8_t pulseLen)
{
    bool ligtsOn = false;

    if(LedStrips.led_kitchen_top.pwm.getDutyC() == 255)
        ligtsOn = true;

    if (state)
    {
        //interrupt op hoog
        if((pulseLen > 10) && (!ligtsOn))
        {
            switchBedLigts(1);
            LedStrips.setStudy(1);
        }
        return;
    }

    if(ligtsOn)
    {
        LedStrips.setKitchen(0);
        LedStrips.setStudy(0);

        switchBedLigts(0);
    }
    else
    {
        //kitchen on
        LedStrips.setKitchen(1);
    }
}

/* main program starts here */
int main(void)
{
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE));

    sei();

    cOutput statusRed(0x60);
    cOutput statusGreen(0x61);
    cLED status(&statusRed, &statusGreen);

    cOutput led1red(0x20);
    cOutput led1green(0x21);
    cLED led1(&led1red, &led1green);

    cOutput led2red(0x22);
    cOutput led2green(0x23);
    cLED led2(&led2red, &led2green);

    cOutput led3red(0x24);
    cOutput led3green(0x25);
    cLED led3(&led3red, &led3green);

    cOutput led4red(0x26);
    cOutput led4green(0x27);
    cLED led4(&led4red, &led4green);

    cHeartbeat heartbeat(&status);

    timerTwo_init();
    timerTwo_setCB(timerTwoCB);

    cCenturionRemote hekRemote = cCenturionRemote(PORT_PH(2));
    hekRemote.setCB(hekRemoteCB);

    cInputCB button4 = cInputCB(PORT_PJ(5));
    button4.setCB(btn4CB);

    cInputCB button5 = cInputCB(PORT_PJ(6));
    button5.setCB(btn5CB);

//    cInputCB pir = cInputCB(PORT_PL(0));
//    pir.setCB(pirCB);

    cRemote remote = cRemote(PORT_PL(2), PORT_PL(1), PORT_PL(6), PORT_PL(7));
    remote.setCB(remoteCB);

    cOutput bt_reset = cOutput(PORT_PG(5));
    bt_reset.set();



    uint8_t delayDivider = 0;
    uint8_t ledState = 0;
    while (1)
    {
        ledState = LedStrips.run();
        _delay_ms(10);

        if (++delayDivider > 10)
        {
            delayDivider = 0;
            watchdogReset();

            Bluetooth.run();
            RS485.run();
            Terminal.run();
            heartbeat.run();
            hekRemote.runC();
            remote.run();
            button4.run();
            button5.run();
            //run only if leds not busy
            if (!ledState)
            {
                powerMon.run();
                Display.updateTime();

                uint16_t vBat = 0;
                uint16_t vPSU = 0;
                powerMon.getVoltage(&vBat, &vPSU);
                Display.updateVoltage(vBat, vPSU);
            }

        }
    }
    return 0;
}
