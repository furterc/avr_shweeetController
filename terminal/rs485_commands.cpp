/*
 * rs485_commands.cpp
 *
 *  Created on: 11 May 2017
 *      Author: christo
 */

#include "RS485.h"

extern const rs485_dbg_entry bedLEDEntry;

const rs485_dbg_entry* rs485_dbg_entries[] =
{
        &bedLEDEntry,
        0
};

