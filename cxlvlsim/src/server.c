/*             SERVER.C               */
/* server.c is the system-specific
 * implementation of an interface
 * controller used to send and receive
 * data through several connected remote
 * devices.

 * Interfaces that could be used include
 * serial ports, pipes, Unix sockets,
 * TCP/IP sockets, and more, depending
 * on the support.

 * This module only uses high-level
 * interfaces provided by the system. */

/* Although we typically expect to
 * receive data at very low bandwitdh
 * rates from a client, we must still be
 * able to handle extreme and
 * unreasonable cases.

 * This includes cases were the server
 * is spammed with packets in short
 * succession.

 * In the interest of not having to
 * allocate large buffers and allocating as
 * few times as possible, we set a hard
 * bandwidth cap that will immediately
 * disconnect. */

#include <stddef.h>

#include "cxlvlsim.h"
#include "server.h"
#include "platform.h"
#include "task.h"
#include "mccp.h"

#define _ register

/**************** DNS *****************/
/* TODO: heartbeat will be handled by a
separate program. */

/* Used to connect to the heartbeat
server by default. */
/*static void dnsip4(const char *hostname, IP4_ADDRESS *addresses) {
	DNS_RECORD* fdr, *cdr; /* First DNS record, current DNS record. *

	if (DnsQuery_A(hostname, DNS_TYPE_A, DNS_QUERY_STANDARD, NULL, &fdr, NULL)) {
		DnsFree(fdr, DnsFreeRecordList);

		return addresses;
	} cdr = fdr;
	while (cdr) cdr = cdr->pNext;
	DnsFree(fdr, DnsFreeRecordList);

	return addresses;
}*/
