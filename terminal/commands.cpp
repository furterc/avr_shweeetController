#include "terminal.h"

extern const dbg_entry mainEntry;
extern const dbg_entry backlightEntry;
extern const dbg_entry outputEntry;
extern const dbg_entry rebootEntry;
extern const dbg_entry btSendEntry;
extern const dbg_entry btCommandSendEntry;
extern const dbg_entry timeEntry;

const dbg_entry* dbg_entries[] =
{
		&helpEntry,
		&mainEntry,
		&backlightEntry,
		&outputEntry,
		&rebootEntry,
		&btSendEntry,
		&btCommandSendEntry,
		&timeEntry,
		0
};
