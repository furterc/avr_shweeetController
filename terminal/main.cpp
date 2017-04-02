#include <float.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include <terminal.h>
#include <led.h>
#include <port.h>
#include <heartbeat.h>
#include <analog_sampler.h>

//#include "mBtUart.h"
#include "timer_two.h"
#include "Packet.h"
#include "Bluetooth.h"
//#include "BT_Commands.h"

#include "Lights.h"
#include "ManualButton.h"
#include "Remote.h"
#include "Time.h"

extern "C"
{
#include "iicDisplay/lcdpcf8574.h"
}

void watchdogReset()
{
    __asm__ __volatile__ ( "wdr\n" );
}

//mBtUart mBt = mBtUart();
//cBlueTerm mBtTerm = cBlueTerm(&mBt);
cTime myTime = cTime();
//cLights mLights = cLights();


cPacket mPacket;
void hourEntry(cPacket packet)
{
    myTime.setHours(packet.getData());
}
extern const bt_dbg_entry btHourEntry =
{ hourEntry,  mPacket.BT_HOURS };
void minuteEntry(cPacket packet)
{
    myTime.setMinutes(packet.getData());
}
extern const bt_dbg_entry btMinuteEntry =
{ minuteEntry,  mPacket.BT_MINUTES};

void secondEntry(cPacket packet)
{
    myTime.setSeconds(packet.getData());
}
extern const bt_dbg_entry btSecondEntry =
{ secondEntry,  mPacket.BT_SECONDS};


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

void time(uint8_t argc, char **argv)
{
    if (argc > 3)
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
    if (argc > 1)
    {
        cPacket packet = cPacket(packet.TYPE_SET, packet.BT_HOURS,
                myTime.getHours());
        uint8_t data[4];
        uint8_t len = packet.toBytes(data);
        Bluetooth.transmit_packet(data, len);
        _delay_us(10);

		cPacket packetMinute = cPacket(packetMinute.TYPE_SET, packetMinute.BT_MINUTES, myTime.getMinutes());
		len = packetMinute.toBytes(data);
		Bluetooth.transmit_packet(data, len);
		_delay_us(10);

		cPacket packetSecond = cPacket(packetSecond.TYPE_SET, packetSecond.BT_SECONDS, myTime.getSeconds());
		len = packetSecond.toBytes(data);
		Bluetooth.transmit_packet(data, len);
		_delay_us(10);

//		for(uint8_t i=0; i<argc-1;i++)
//		{
//			mBt.transmit_array(argv[i+1]);
//			mBt.transmit_array(" ");
//		}
//		mBt.transmit_array("\r");
    }
}
extern const dbg_entry btSendEntry =
{ btSend, "b" };

void reboot(uint8_t argc, char **argv)
{
    printp("reboot");
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
{ backLight, "l" };

cOutput relay1(PORT_PF(0));
cOutput relay2(PORT_PF(1));
cOutput relay3(PORT_PF(2));
cOutput relay4(PORT_PF(3));
void debugOut(uint8_t argc, char **argv)
{
    cOutput *relay = 0;
    if (argc > 1)
    {
        uint8_t num = atoi(argv[1]);
        switch (num)
        {
        case 1:
            relay = &relay1;
            break;
        case 2:
            relay = &relay2;
            break;
        case 3:
            relay = &relay3;
            break;
        case 4:
            relay = &relay4;
            break;
        default:
            return;
        }

        if (argc > 2)
        {
            bool state = atoi(argv[2]);
            printp("%s - ", state ? "set" : "reset", num);
            if (state)
                relay->set();
            else
                relay->reset();
        }

        bool state = relay->get();
        printp("RL%d: %s\n", num, state ? "ON" : "OFF");
    }
    else
    {
        printp("RL1: %s\n", relay1.get() ? "ON" : "OFF");
        printp("RL2: %s\n", relay2.get() ? "ON" : "OFF");
        printp("RL3: %s\n", relay3.get() ? "ON" : "OFF");
        printp("RL4: %s\n", relay4.get() ? "ON" : "OFF");
    }
}
extern const dbg_entry outputEntry =
{ debugOut, "o" };

cAnalog analogIn1(4);
cAnalog analogIn2(5);
cAnalog analogIn3(6);
cAnalog analogIn4(7);

void measureTemp(uint8_t argc, char **argv)
{
    printp("Measuring temp:\n");

    double sample = analogIn1.lastSample();
    double temp = 500 * (sample / 1024.0) - 273.0;
    printf(" 1: %.1f\n", temp);

    sample = analogIn2.lastSample();
    temp = 500 * (sample / 1024.0) - 273.0;
    printf(" 2: %.1f\n", temp);

    sample = analogIn3.lastSample();
    temp = 5 * (sample / 1024.0);
    printf(" 3: %.1f\n", temp);

    sample = analogIn4.lastSample();
    temp = 5 * (sample / 1024.0);
    printf(" 4: %.1f\n", temp);
}
extern const dbg_entry mainEntry =
{ measureTemp, "temp" };

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

}

void pirTriggerCB(void)
{

}

void timerTwoCB(void)
{
    myTime.incTime();
}

//uint8_t onOffState = 0;
void btnTrigger(void)
{
    if (mLights.getLevel(mLights.LIGHT_KITCHEN_TOP) != 0 || mLights.getLevel(mLights.LIGHT_KITCHEN_BOT) != 0)
    {
        mLights.setLevel(mLights.LIGHT_KITCHEN_TOP, 0);
        mLights.setLevel(mLights.LIGHT_KITCHEN_BOT, 0);
    }
    else
    {
        mLights.setLevel(mLights.LIGHT_KITCHEN_TOP, 5, 5);
        mLights.setLevel(mLights.LIGHT_KITCHEN_BOT, 5, 1);
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
    cHeartbeat heartbeat2(&led4);

    timerTwo_init();
    timerTwo_setCB(timerTwoCB);
    cManualButton mBtn = cManualButton();
    mBtn.btnSetCB(btnTrigger);

    cAnalog *analogList[] =
    { &analogIn1, &analogIn2, &analogIn3, &analogIn4, 0 };
    cAnalogSampler analogSampler(analogList);

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
    while (1)
    {
        watchdogReset();
        mLights.runDelay();
        Bluetooth.run();
        _delay_ms(2);

        if (++delayDivider > 49)
        {
            delayDivider = 0;
            Terminal.run();
            heartbeat.run();
            heartbeat2.run();
            analogSampler.run();
            myRemote.run();
            mBtn.run();
        }
    }
    return 0;
}

