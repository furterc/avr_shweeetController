#include "Bluetooth.h"

extern const bt_dbg_entry btHourEntry;
extern const bt_dbg_entry btMinuteEntry;
extern const bt_dbg_entry btSecondEntry;
extern const bt_dbg_entry btKitchenTop;
extern const bt_dbg_entry btKitchenBot;
extern const bt_dbg_entry btStudyTop;
extern const bt_dbg_entry btStudyBot;
extern const bt_dbg_entry btAlarmHour;
extern const bt_dbg_entry btAlarmMinute;

const bt_dbg_entry* bt_dbg_entries[] =
{
		&btHourEntry,
		&btMinuteEntry,
		&btSecondEntry,
		&btKitchenTop,
		&btKitchenBot,
        &btStudyTop,
        &btStudyBot,
        &btAlarmHour,
        &btAlarmMinute,
		0
};
