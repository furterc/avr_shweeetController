#include "terminal.h"

extern const dbg_entry mainEntry;
extern const dbg_entry backlightEntry;
extern const dbg_entry outputEntry;
extern const dbg_entry pwmTestEntry;
extern const dbg_entry btSendEntry;
extern const dbg_entry btCommandSendEntry;

const dbg_entry* dbg_entries[] =
{
		&helpEntry,
		&mainEntry,
		&backlightEntry,
		&outputEntry,
		&pwmTestEntry,
		&btSendEntry,
		&btCommandSendEntry,
		0
};
