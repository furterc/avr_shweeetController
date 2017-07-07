#include "terminal.h"

extern const dbg_entry btResetEntry;
extern const dbg_entry rebootEntry;
extern const dbg_entry btSendEntry;
extern const dbg_entry btCommandSendEntry;
extern const dbg_entry timeEntry;
extern const dbg_entry lightEntry;

const dbg_entry* dbg_entries[] =
{
		&helpEntry,
		&btResetEntry,
		&rebootEntry,
		&btSendEntry,
		&btCommandSendEntry,
		&timeEntry,
		&lightEntry,
		0
};
