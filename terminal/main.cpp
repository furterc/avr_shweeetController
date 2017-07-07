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

#include "Lights.h"
#include "Remote.h"
#include "Time.h"
#include "RS485.h"
//
//extern "C"
//{
//#include "iicDisplay/lcdpcf8574.h"
//}

void watchdogReset()
{
    __asm__ __volatile__ ( "wdr\n" );
}

void printBedPWM(uint8_t i, uint8_t duty)
{
    printp("pwm%d\t: %d\n", i, duty)
}

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
        printp("Kitchen\n Top: %d\n Bot: %d\n",
                mLights.getDuty(mLights.LIGHT_KITCHEN_TOP),
                mLights.getDuty(mLights.LIGHT_KITCHEN_BOT));
        printp("Study\n Top: %d\n Bot: %d\n",
                mLights.getDuty(mLights.LIGHT_STUDY_TOP),
                mLights.getDuty(mLights.LIGHT_STUDY_BOT));
        return;
    }

    if (argc == 2)
    {
        if (!strcmp(argv[1], "k"))
        {
            if (mLights.getDuty(mLights.LIGHT_KITCHEN_TOP) != 0
                    || mLights.getDuty(mLights.LIGHT_KITCHEN_BOT) != 0)
            {
                mLights.setSoft(mLights.LIGHT_KITCHEN_TOP, 0, 0);
                mLights.setSoftDelayed(255, mLights.LIGHT_KITCHEN_BOT, 0, 0);
            }
            else
            {
                mLights.setSoftDelayed(150, mLights.LIGHT_KITCHEN_TOP, 4, 255);
                mLights.setSoft(mLights.LIGHT_KITCHEN_BOT, 4, 255);
            }
        }

        if (!strcmp(argv[1], "s"))
        {
            if (mLights.getDuty(mLights.LIGHT_STUDY_TOP) != 0
                    || mLights.getDuty(mLights.LIGHT_STUDY_BOT) != 0)
            {
                mLights.setSoft(mLights.LIGHT_STUDY_TOP, 0, 0);
                mLights.setSoftDelayed(255, mLights.LIGHT_STUDY_BOT, 0, 0);
//                workspaceLED.reset();
            }
            else
            {
                mLights.setSoftDelayed(150, mLights.LIGHT_STUDY_TOP, 4, 255);
                mLights.setSoft(mLights.LIGHT_STUDY_BOT, 4, 255);
//                workspaceLED.set();
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

void btSend(uint8_t argc, char **argv)
{

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
        mLights.incLevel(mLights.LIGHT_KITCHEN_TOP);
        break;
    case 1:
        mLights.incLevel(mLights.LIGHT_KITCHEN_BOT);
        break;
    case 2:
        mLights.incLevel(mLights.LIGHT_STUDY_BOT);
        break;
    case 3:
        mLights.incLevel(mLights.LIGHT_STUDY_TOP);
        break;
    default:
        break;
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
            printp("setting\n");
            mLights.setLevel(mLights.LIGHT_STUDY_TOP, 5);
            mLights.setLevel(mLights.LIGHT_STUDY_BOT, 5);
            setted = true;
        }
        else
        {
            printp("unsetting\n");
            mLights.setLevel(mLights.LIGHT_STUDY_TOP, 0);
            mLights.setLevel(mLights.LIGHT_STUDY_BOT, 0);
            setted = false;
        }
    }
}

void timerTwoCB(void)
{
    myTime.incTime();
}


static uint16_t cnt = 0;
void hekRemoteCB(bool state)
{

    //only interrupt upon click
    if (state)
    {
        cnt++;
        return;
    }


    printp("hek4: %d\n", cnt);
    cnt = 0;

    if (mLights.getDuty(mLights.LIGHT_KITCHEN_TOP) != 0
            || mLights.getDuty(mLights.LIGHT_KITCHEN_BOT) != 0)
    {
        mLights.setSoft(mLights.LIGHT_KITCHEN_TOP, 0, 0);
        mLights.setSoftDelayed(255, mLights.LIGHT_KITCHEN_BOT, 0, 0);

        cMsg msg = cMsg(0,0,0,0);
        msg.setType(msg.TYPE_SET);
        msg.setTag(msg.TAG_LED_BED);
        msg.setData1(0);
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
    else
    {
        mLights.setSoftDelayed(50, mLights.LIGHT_KITCHEN_TOP, 2, 255);
        mLights.setSoft(mLights.LIGHT_KITCHEN_BOT, 1, 255);

        cMsg msg = cMsg(0,0,0,0);
        msg.setType(msg.TYPE_SET);
        msg.setTag(msg.TAG_LED_BED);
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
}

void pirCB(bool state)
{
    if (state)
        printp("pir: %d:%d\n", myTime.getHours(), myTime.getMinutes());
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

    cInputCB hekRemote = cInputCB(PORT_PH(2));
    hekRemote.setCB(hekRemoteCB);

    cInputCB button5 = cInputCB(PORT_PJ(6));
    button5.setCB(btn5CB);

    cInputCB pir = cInputCB(PORT_PL(0));
    pir.setCB(pirCB);

    cRemote remote = cRemote(PORT_PL(2), PORT_PL(1), PORT_PL(6), PORT_PL(7));
    remote.setCB(remoteCB);

    cOutput bt_reset = cOutput(PORT_PG(5));
    bt_reset.set();

//    lcd_init(LCD_DISP_ON);
//    lcd_led(0);
//    lcd_gotoxy(4, 0);
//    lcd_puts("SHWEEET!");

    uint8_t delayDivider = 0;
    while (1)
    {
        mLights.runDelay();
        _delay_ms(10);

        if (++delayDivider > 10)
        {
            watchdogReset();
            Bluetooth.run();
            RS485.run();
            delayDivider = 0;
            Terminal.run();
            heartbeat.run();
            hekRemote.run();
            remote.run();
            pir.run();
            button5.run();
        }
    }
    return 0;
}
