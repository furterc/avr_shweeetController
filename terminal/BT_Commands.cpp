#include "Bluetooth.h"

extern const bt_dbg_entry btHourEntry;
extern const bt_dbg_entry btMinuteEntry;
extern const bt_dbg_entry btSecondEntry;

const bt_dbg_entry* bt_dbg_entries[] =
{
		&btHourEntry,
		&btMinuteEntry,
		&btSecondEntry,
		0
};
