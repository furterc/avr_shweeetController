/*
 * display.cpp
 *
 *  Created on: 19 Aug 2017
 *      Author: christo
 */

#include <stdlib.h>
#include <terminal.h>

#include "display.h"
#include "Time.h"

void cDisplay::pad_zero(uint8_t value, char *str, uint8_t *idx)
{
    uint8_t tmp = *idx;
    if(value < 10)
    {
        str[tmp++] = '0';
        itoa(value, str+tmp, 10);
    }
    else
    {
        itoa(value, str+tmp, 10);
        tmp++;
    }
    *idx = ++tmp;
}

void cDisplay::showVoltage(uint16_t voltage, char *str, uint8_t *idx)
{
    uint8_t tmp = *idx;
    voltage /= 100;
    uint8_t dec = voltage % 10;
    uint8_t real = voltage / 10;

    if( real < 10 )
    {
        str[tmp++] = ' ';
        itoa(real, str + tmp++, 10);
    }
    else
    {
        itoa(real, str + tmp++, 10);
        tmp++;
    }
    str[tmp++] = '.';
    itoa(dec, str + tmp++, 10);
    *idx = tmp;
}


cDisplay::cDisplay()
{
    lcd_init(LCD_DISP_ON);
    lcd_led(0);
    lcd_gotoxy(4, 0);
//    lcd_puts("SHWEEET!");
}

void cDisplay::set_led(uint8_t state)
{
    lcd_led(!state);
}

void cDisplay::updateTime()
{
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    myTime.getTime(&hours, &minutes, &seconds);

    char timeS[6];
    uint8_t idx = 0;
    pad_zero(hours, timeS, &idx);

    timeS[idx++] = ':';
    pad_zero(minutes, timeS, &idx);
    timeS[idx] = 0;

    lcd_gotoxy(20-idx, 0);
    lcd_puts(timeS);
}

void cDisplay::updateVoltage(uint16_t batVoltage, uint16_t psuVoltage)
{
    char voltage[10];
    uint8_t idx = 0;


    voltage[idx++] = 'V';
    voltage[idx++] = 'b';
    voltage[idx++] = ':';
    showVoltage(batVoltage, voltage, &idx);
    voltage[idx] = 0;
    lcd_gotoxy(20-idx, 2);
    lcd_puts(voltage);

    idx = 0;
    voltage[idx++] = 'V';
    voltage[idx++] = 'p';
    voltage[idx++] = ':';
    showVoltage(psuVoltage, voltage, &idx);
    voltage[idx] = 0;
    lcd_gotoxy(20-idx, 3);
    lcd_puts(voltage);
}

cDisplay::~cDisplay()
{
    // TODO Auto-generated destructor stub
}

