/*
 * rs485_commands.cpp
 *
 *  Created on: 11 May 2017
 *      Author: christo
 */

#include "RS485.h"

extern const rs485_dbg_entry bed0Entry;
extern const rs485_dbg_entry bed1Entry;
extern const rs485_dbg_entry bed2Entry;


const rs485_dbg_entry* rs485_dbg_entries[] =
{
        &bed0Entry,
        &bed1Entry,
        &bed2Entry,
        0
};

