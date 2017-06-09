#include "Bluetooth.h"

extern const bt_dbg_entry btTime;
extern const bt_dbg_entry btAlarm;
extern const bt_dbg_entry btKitchen;
extern const bt_dbg_entry btStudy;

const bt_dbg_entry* bt_dbg_entries[] =
{
		&btTime,
		&btAlarm,
		&btKitchen,
		&btStudy,
		0
};
