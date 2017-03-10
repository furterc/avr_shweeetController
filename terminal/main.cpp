#include <stdio.h>
#include <float.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include <terminal.h>
#include <led.h>
#include <heartbeat.h>
#include <analog_sampler.h>

#include "pwm.h"
#include "cPwmRun.h"
#include "cTimerPwm.h"
#include "cRemote.h"
#include "cManualButton.h"
#include "mBtUart.h"

extern "C" {
#include "iicDisplay/lcdpcf8574.h"
}

void watchdogReset()
{
  __asm__ __volatile__ ( "wdr\n" );
}

mBtUart mBt = mBtUart();

void btCommandSend(uint8_t argc, char **argv)
{
	mBt.transmit_command();

	if(argc > 1)
	{
		for(int i=0; i<argc-1;i++)
		{
			mBt.transmit_array(argv[i+1]);
			mBt.transmit_array(" ");
		}
	}
	mBt.transmit_array("\r");

}
extern const dbg_entry btCommandSendEntry = {btCommandSend, "at+ab"};


void btSend(uint8_t argc, char **argv)
{
	if(argc > 1)
	{
		for(int i=0; i<argc-1;i++)
		{
			mBt.transmit_array(argv[i+1]);
			mBt.transmit_array(" ");
		}
		mBt.transmit_array("\r");

	}

}
extern const dbg_entry btSendEntry = {btSend, "b"};

void pwmTest(uint8_t argc, char **argv)
{
	if(argc > 1)
	{
		uint8_t num = atoi(argv[1]);
		printp("pwm duty: %d\n",num);
	}

}
extern const dbg_entry pwmTestEntry = {pwmTest, "p"};

void backLight(uint8_t argc, char **argv)
{
	cOutput bt_reset(PORT_PG(5));
	bt_reset.reset();
	_delay_ms(100);
	bt_reset.set();
}
extern const dbg_entry backlightEntry = {backLight, "l"};

cOutput relay1(PORT_PF(0));
cOutput relay2(PORT_PF(1));
cOutput relay3(PORT_PF(2));
cOutput relay4(PORT_PF(3));
void debugOut(uint8_t argc, char **argv)
{
	cOutput *relay = 0;
	if(argc > 1)
	{
		uint8_t num = atoi(argv[1]);
		switch(num)
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

		if(argc > 2)
		{
			bool state = atoi(argv[2]);
			printp("%s - ", state?"set":"reset", num);
			if(state)
				relay->set();
			else
				relay->reset();
		}

		bool state = relay->get();
		printp("RL%d: %s\n", num, state?"ON":"OFF");
	}
	else
	{
		printp("RL1: %s\n", relay1.get()?"ON":"OFF");
		printp("RL2: %s\n", relay2.get()?"ON":"OFF");
		printp("RL3: %s\n", relay3.get()?"ON":"OFF");
		printp("RL4: %s\n", relay4.get()?"ON":"OFF");
	}
}
extern const dbg_entry outputEntry = {debugOut, "o"};

cAnalog analogIn1(4);
cAnalog analogIn2(5);
cAnalog analogIn3(6);
cAnalog analogIn4(7);

void measureTemp(uint8_t argc, char **argv)
{
	printp("Measuring temp:\n");

	double sample = analogIn1.lastSample();
	double temp = 500 * (sample/1024.0) - 273.0;
	printf(" 1: %.1f\n", temp);

	sample = analogIn2.lastSample();
	temp = 500 * (sample/1024.0) - 273.0;
	printf(" 2: %.1f\n", temp);

	sample = analogIn3.lastSample();
	temp = 5 * (sample/1024.0);
	printf(" 3: %.1f\n", temp);

	sample = analogIn4.lastSample();
	temp = 5 * (sample/1024.0);
	printf(" 4: %.1f\n", temp);
}
extern const dbg_entry mainEntry = {measureTemp, "temp"};

cPWM tableTop;
cPWM tableBot;
cPWM tableAlt;
cTimerPwm tFourPwm = cTimerPwm(0, 4, &tableTop, &tableBot, &tableAlt);

uint8_t m4APWM_State = 0;
void button4_run(void)
{
	uint8_t dutyC = 0;

	switch(++m4APWM_State)
	{
	case 1:
		dutyC = 0x0;
		break;
	case 2:
		dutyC = 0x15;
		break;
	case 3:
		dutyC = 0x30;
		break;
	case 4:
		dutyC = 0x50;
		break;
	case 5:
		dutyC = 0xFF;
		m4APWM_State = 0;
		break;
	default:
		m4APWM_State = 0;
	}

	tableTop.setDutyC(dutyC);
}


cOutput workLED = cOutput(PORT_PH(6));

uint8_t m4BPWM_State = 0;
void button3_run(void)
{
	uint8_t dutyC = 0;

	switch(++m4BPWM_State)
	{
	case 1:
		workLED.set();
		dutyC = 0x0;
		break;
	case 2:
		workLED.reset();
		dutyC = 0x15;
		break;
	case 3:
		workLED.set();
		dutyC = 0x30;
		break;
	case 4:
		workLED.reset();
		dutyC = 0x50;
		break;
	case 5:
		dutyC = 0xFF;
		m4BPWM_State = 0;
		break;
	default:
		m4BPWM_State = 0;
	}

	tableBot.setDutyC(dutyC);
}


cPWM kitchenTop;
cPwmRun kitchenTopRun = cPwmRun(&kitchenTop);
cPWM kitchenBot;
cPwmRun kitchenBotRun = cPwmRun(&kitchenBot);
cPWM kitchenAlt;
cTimerPwm tFivePwm = cTimerPwm(1, 5, &kitchenTop, &kitchenAlt, &kitchenBot);

uint8_t mCPWM_State = 0;
void button2_run(void)
{
	switch(++mCPWM_State)
		{
		case 1:
			kitchenBotRun.setDuty(1,0x0);
			break;
		case 2:
			kitchenBotRun.setDuty(1,0x15);
			break;
		case 3:
			kitchenBotRun.setDuty(1,0x30);
			break;
		case 4:
			kitchenBotRun.setDuty(1,0x50);
			break;
		case 5:
			kitchenBotRun.setDuty(1,0xFF);
			mCPWM_State = 0;
			break;
		default:
			mCPWM_State = 0;
		}
}

uint8_t mAPWM_State = 0;

void button1_run(void)
{
	switch(++mAPWM_State)
	{
	case 1:
		kitchenTopRun.setDuty(1,0x0);
		break;
	case 2:
		kitchenTopRun.setDuty(1, 0x15);
		break;
	case 3:
		kitchenTopRun.setDuty(1,0x30);
		break;
	case 4:
		kitchenTopRun.setDuty(1,0x50);
		break;
	case 5:
		kitchenTopRun.setDuty(1,0xFF);
		mAPWM_State = 0;
		break;
	default:
		mAPWM_State = 0;
	}
}

cRemote myRemote;

void pirOffCB(void)
{
//	timerFivePWMA_set(0x02);
}

void pirTriggerCB(void)
{
//	timerFivePWMA_set(0x20);
}


uint8_t onOffState = 0;
void btnTrigger(void)
{
	if(onOffState++ == 1)
	{
		onOffState = 0;
		kitchenTopRun.setDuty(1, 0);
		kitchenBotRun.setDuty(1, 0);
	} else
	{
		kitchenTopRun.setDuty(1, 0xFF);
		kitchenBotRun.setDuty(1, 0xFF);
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

	tFivePwm.init();
	tFourPwm.init();

	cManualButton mBtn = cManualButton();
	mBtn.btnSetCB(btnTrigger);

	cAnalog *analogList[] =
	{
			&analogIn1,
			&analogIn2,
			&analogIn3,
			&analogIn4,
			0
	};
	cAnalogSampler analogSampler(analogList);

	lcd_init(LCD_DISP_ON);
	lcd_led(0);
	lcd_gotoxy(4,0);
	lcd_puts("SHWEEET!");

	myRemote.setCB(1, button1_run);
	myRemote.setCB(2, button2_run);
	myRemote.setCB(3, button3_run);
	myRemote.setCB(4, button4_run);

	mBt.init();

//	char * mAr = "AT";

	mBt.transmit_array("AT\n\r");

	cOutput bt_reset = cOutput(PORT_PG(5));
	bt_reset.set();

	uint8_t delayDivider = 0;
	while(1)
	{
		watchdogReset();
		kitchenTopRun.run();
		kitchenBotRun.run();

		_delay_ms(10);

		if (++delayDivider > 9)
		{
			delayDivider = 0;
			Terminal.run();
			heartbeat.run();
			heartbeat2.run();
			analogSampler.run();
			myRemote.run();
			mBtn.run();
			mBt.run();

		}
	}

	return 0;
}

