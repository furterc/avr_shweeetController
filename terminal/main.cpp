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

#include "timer_two.h"
#include "Packet.h"
#include "Bluetooth.h"
#include "button.h"

#include "Lights.h"
#include "ManualButton.h"
#include "Remote.h"
#include "Time.h"
#include "RS485.h"

extern "C"
{
#include "iicDisplay/lcdpcf8574.h"
}

void watchdogReset()
{
    __asm__ __volatile__ ( "wdr\n" );
}

cPacket mpacket = cPacket();

void rs485Test(cPacket packet)
{
    printp("FUCKYEA \n data: %d\n", packet.getData());
//    if (packet.getType() == packet.TYPE_GET)
//    {
//        mpacket.setType(packet.TYPE_SET);
//        uint8_t data = mLights.getDuty(mLights.LIGHT_STUDY_TOP);
//
//        mpacket.setData(data);
//        uint8_t bytes[4];
//        mpacket.toBytes(bytes);
//        Bluetooth.transmit_packet(bytes, 4);
//        return;
//    }
//    else
//        mLights.setSoft(mLights.LIGHT_STUDY_TOP, 5, packet.getData());
}
extern const rs485_dbg_entry rs485TestEntry=
{ rs485Test, mpacket.BT_STUDY_TOP};

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
    cPacket packet = cPacket(72, 73, 10, 13);

    uint8_t data[4];
    uint8_t len = packet.toBytes(data);
    RS485.transmit_packet(data, len);
    _delay_us(10);

    printp("data out: %c,%c,%c,%c", packet.getType(), packet.getTag(), packet.getData(), packet.getCrc());
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

void backLight(uint8_t argc, char **argv)
{
    cOutput bt_reset(PORT_PG(5));
    bt_reset.reset();
    _delay_ms(100);
    bt_reset.set();
}
extern const dbg_entry backlightEntry =
{ backLight, "bl" };

cRemote myRemote;
void button4_run(void)
{
    mLights.incLevel(mLights.LIGHT_STUDY_TOP);
}

void button3_run(void)
{
    mLights.incLevel(mLights.LIGHT_STUDY_BOT);
}

void button2_run(void)
{
    mLights.incLevel(mLights.LIGHT_KITCHEN_BOT);
}

void button1_run(void)
{
    mLights.incLevel(mLights.LIGHT_KITCHEN_TOP);
}

void pirOffCB(void)
{
    printp("PIR off\n");
}

void pirTriggerCB(void)
{
    printp("PIR off\n");
}

void timerTwoCB(void)
{
    myTime.incTime();
}

void btnTrigger(void)
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
    cManualButton mBtn = cManualButton();
    mBtn.btnSetCB(btnTrigger);

    lcd_init(LCD_DISP_ON);
    lcd_led(0);
    lcd_gotoxy(4, 0);
    lcd_puts("SHWEEET!");

    myRemote.setCB(1, button1_run);
    myRemote.setCB(2, button2_run);
    myRemote.setCB(3, button3_run);
    myRemote.setCB(4, button4_run);

    cOutput bt_reset = cOutput(PORT_PG(5));
    bt_reset.set();

    uint8_t delayDivider = 0;

//    uint8_t b = 80;

    while (1)
    {
        mLights.runDelay();
        _delay_ms(10);

        if (++delayDivider > 10)
        {
            Buttons.run();
            watchdogReset();
            Bluetooth.run();
            RS485.run();
            delayDivider = 0;
            Terminal.run();
            heartbeat.run();
            myRemote.run();
            mBtn.run();
        }
    }
    return 0;
}
