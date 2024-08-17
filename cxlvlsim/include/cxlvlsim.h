#ifndef CX_LVLSIM_H
#define CX_LVLSIM_H

/* #define CXF __stdcall */
#define CXF
/*
#ifndef CXAPI
	#define CXAPI \
	__declspec(dllexport)
#endif */
#define CXAPI
#define CSTRLEN(s) (sizeof (s) - 1)

/* How it will work:
Only implement version 0 for now.
Blindly echo player position packets.
Send a plain read-only .dat for level
data. */

/* All exported functions for the Blox
ClassiCube Server Library are here. */

/* Print text or do command with "/". */
CXAPI void *CXF
cxcmd(const char *cmd, unsigned len);

/* Gets a server variable (svar).
Server variables can be: ints,
fixed-points, strings, arrays of either.

Server variables cannot be created nor
destroyed, and their types are fixed. */
CXAPI const void *CXF
cxget(const char *vname);

/* Set a server variable. */
CXAPI int CXF cxset(
	const char *vname,
	const void *val
);

/* Start the server. */
CXAPI void CXF
cxstart(void);

/* Stop the server. */
CXAPI void CXF
cxstop(int code);

/*            User Actions            */

/* Gets the current client number of a
user if online. */
/*mccpapi(client_number)user_id(const char *user);*/
/* Blacklists a user from the server for a specific amount of time. */
/*mccpapi(int) user_ban(const char *user, unsigned seconds);*/
/* Sets a user's permission number. */
/*mccpapi(int) user_rank(const char *user, unsigned perm);*/
/* Whitelists a user to the server for a specific amount of time. */
/*mccpapi(int) user_allow(const char *user, unsigned seconds);*/
#endif /* CX_LVLSIM_H */
