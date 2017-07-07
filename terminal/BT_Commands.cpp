#include <bluetooth.h>

extern const bt_dbg_entry btTime;
extern const bt_dbg_entry btAlarm;
extern const bt_dbg_entry btKitchen;
extern const bt_dbg_entry btStudy;
extern const bt_dbg_entry btBed;

const bt_dbg_entry* bt_dbg_entries[] =
{
		&btTime,
		&btAlarm,
		&btKitchen,
		&btStudy,
		&btBed,
		0
};
