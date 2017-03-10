#include <avr/io.h>

uint8_t m4ADuty;
uint8_t m4BDuty;
uint8_t m4CDuty;

void timerFourPWMA_init(void)
{
	// set PH3 & PH4 & PH5 as output
	DDRH |= _BV(3) | _BV(4) | _BV(5);

	// inverting mode A & B
	TCCR4A |= _BV(COM4A1);
	TCCR4A |= _BV(COM4B1);
	TCCR4A |= _BV(COM4C1);

	// set Waveform as phase correct PWM / 8 bit
 	TCCR4A |=_BV(WGM40);
 	// set the prescaler as clk/1024
	TCCR4B |= _BV(CS41) | _BV(CS40);
	m4ADuty = 0;
	m4BDuty = 0;
	m4CDuty = 0;
	OCR4A = m4ADuty;
	OCR4B = m4BDuty;
	OCR4C = m4CDuty;
}

void timerFourPWMA_set(uint8_t duty)
{
	m4ADuty = duty;
	OCR4A = m4ADuty;
}

void timerFourPWMB_set(uint8_t duty)
{
	m4BDuty = duty;
	OCR4B = m4BDuty;
}

void timerFourPWMC_set(uint8_t duty)
{
	m4CDuty = duty;
	OCR4C = m4CDuty;
}
