/************** SERVER.H **************/
/* Server.h is the representation of the
 * actual server and serves as the hub of
 * all other modules. */

/* This might move to a different
 * project entirely. */
#ifndef CX_SERVER_H
#define CX_SERVER_H
static unsigned const
CX_LEVEL_ARRAY_SIZE = 256,
CX_SERVER_MAX_THREADS = 256;
extern char server_active;
extern unsigned long
server_playersOnline;
extern struct level *server_mainlvl;
/*struct Task {
	char name[20];
	void(*func)(void*);
	void* data;
};*/
extern printfunc print;
char sstart(void);
void handleplayerconnect(void *ccpid);
#endif/*CX_SERVER_H*/
