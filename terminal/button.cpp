#include <avr/interrupt.h>
#include <util/delay.h>

#include <terminal.h>
#include "button.h"

#define BUTTON_DEBUG

cButtonListner::cButtonListner()
{
    mEnabled = false;
}

void cButtonListner::enable()
{
    mEnabled = true;
    sei();
}

void cButtonListner::disable()
{
    cli();
    mEnabled = false;
}

cButtons::cButtons()
{
    mListener = 0;
    mPressed = 0;
    PCICR = 1 << PCIE1;

    PCMSK1 |= _BV(PCINT11) | _BV(PCINT12) | _BV(PCINT13) | _BV(PCINT14) | _BV(PCINT15);
}

void cButtons::handleButton(uint8_t buttons)
{
    if(buttons & 0x04)
        mPressed = 11;
    else if(buttons & 0x08)
        mPressed = 12;
    else if(buttons & 0x10)
        mPressed = 13;
    else if(buttons & 0x20)
        mPressed = 14;
    else if(buttons & 0x40)
        mPressed = 15;
}

void cButtons::run()
{
    switch(mPressed)
    {
    case 11:
#ifdef BUTTON_DEBUG
        printp("UP\n");
#endif
        if(mListener) mListener->pressed(cButtonListner::MENU_UP);
        break;
    case 12:
#ifdef BUTTON_DEBUG
        printp("DOWN\n");
#endif
        if(mListener) mListener->pressed(cButtonListner::MENU_DOWN);
        break;
    case 13:
#ifdef BUTTON_DEBUG
        printp("LEFT\n");
#endif
        if(mListener) mListener->pressed(cButtonListner::MENU_LEFT);
        break;
    case 14:
#ifdef BUTTON_DEBUG
        printp("RIGHT\n");
#endif
        if(mListener) mListener->pressed(cButtonListner::MENU_RIGHT);
        break;
    case 15:
#ifdef BUTTON_DEBUG
        printp("ENTER\n");
#endif
        if(mListener) mListener->pressed(cButtonListner::MENU_ENTER);
        break;
    default:
        break;
    }

    mPressed = 0;
}

cButtons::~cButtons()
{
}


ISR(PCINT1_vect)
{
    _delay_us(5000);

    Buttons.handleButton(~PINJ);
}

cButtons Buttons;
