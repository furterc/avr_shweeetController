#include <stdio.h>
#include <avr/io.h>

static int putch(char ch, FILE *s)
{
	while (!(UCSR1A & _BV(UDRE0)));

	if(ch == '\n')
	{
		UDR1 = '\r';
		while (!(UCSR1A & _BV(UDRE0)));
	}

	UDR1 = ch;

	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(putch, NULL, _FDEV_SETUP_WRITE);

void mystdio_init()
{
	stdout = &mystdout;
}
