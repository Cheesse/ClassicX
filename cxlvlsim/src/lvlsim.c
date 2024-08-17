#include <mccp.h>
#include "platform.h"
#include "cxlvlsim.h"
#include "client.h"
#include "level.h"
/*#include "server.h"*/

#define _ register

/* TODO: (BUNCH OF STUFF)
 * Move heartbeat implementation
 * somewhere else.
 * Fix player data read & write.
 * Add useful Lua functions.
 * Make the read/write file operations
 * more asynchronous?
 * Improve GUI.
 * Consider using C FILE or os-specific
 * file stuff for files?
 * Figure out how to abstract blocks
 * enough to add 10-bit blocks, or inf
 * id!
 * Add CPE support.
 * Abstract level loading - make it
 * generic.
 * File loading in Lua (maybe). */

/* Executes a command as console with
 * maximum privileges and certain
 * limitations. */
/*
void *cxcmd(
	_ const char *cmd,
	_ unsigned len
) {
	// Send a command as console.
}
const void *cxget(
	_ const char *name,
	_ unsigned len
) {
	// Get the value of some
	// published global variable
}
int cxload(_ const char x) {

}
void cxprint(
	_ const char *text,
	_ unsigned len
) {
	output(text, len);
}

void cx_set (
	_ const char *name,
	_ const void *value
) {
	// Set the value of some
	// published global variable
}
*/
int
start(int ac, char **av) {
	int code;
	struct lvl l = { 0 };
	struct cli c = { 0 };
	_ enum mcctyp ct;

	ac = ac;
	av = av;
	warn("LVLSIM started.\n");

	/* Load main level? */
	/* Don't actually load anything,
	just prepare the flat level so
	we can send it. */
	lload(&l, 0);

	/* Set up network interfaces. */
	/* a = netstart(); */

	csetup(&c, &l);
	/* ctest(&c); */
	ct = MCCTEVTNO;
	/* return 0; */
	while (MCCTINVALID < ct) {
		ct = cloop(&c, ct);
	}
	code = ct;
	return code;
}
