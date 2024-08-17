/*              SERVER.H              */

/* server.h declares network-related
 * functions for byte streams between
 * remote hosts.

 * These functions are implemented on
 * top of the underlying API or driver
 * provided by the system.

 * If the system is not supported, you
 * will have to write your own
 * implementations.

 * It is assumed that the system will
 * buffer all the received bytes anyway.

 * So, we will just have a thread (or
 * many threads) calling recv() with the
 * number of bytes we want to read ahead
 * of time.

 * This would be inefficient if the
 * system didn't buffer the bytes, but
 * then we would have to do it ourselves
 * probably.

 * Maybe the read and write functions
 * should be type-specific?

 * At the very least, as the server, we
 * need to be able to buffer up to 64
 * bytes.

 * It *should* be possible on an MSP430.
 */

#ifndef CX_SERVER_H
#define CX_SERVER_H

#define CX_DEFAULT_PORT 25565

/********************************************************** NETWORK ***********************************************************/

/* Function return codes. */
typedef enum NetworkResult {
	SUCCESSFUL_NETWORK_OPERATION,
	FAILED_NETWORK_OPERATION,
	NO_INTERNET,
	SOCKET_FAILURE,
	LISTENING_FAILURE,
	NETWORK_BACKEND_FAILURE,
	ADDRESS_INTERPRETATION_FAILURE,
	CONNECTION_FAILURE
}
NetworkResult;

typedef void *SocketNumber;
typedef struct Session Session;

typedef struct CxServer {
	char isStopping;
	char _reserved[7];
	SocketNumber listener;
	Session *session;
} CxServer;

typedef NetworkResult
Reader(
	CxServer *const,
	SocketNumber const
);

NetworkResult

startServer(
	CxServer *const,
	char const*const *interfaces_to_listen_on,
	short timeout_period
),

stopServer(CxServer *const),

/* Determine the number of bytes
available to read currently for each
listed connection. */
check_available_bytes (
	unsigned const *connids,
	unsigned *number_of_bytes,
	unsigned number_of_connections
);

Reader
readEventCode,
readGreeting,
readBlockChange,
readActorState;

NetworkResult

sendEventCode(
	CxServer *const,
	SocketNumber const,
	char const
),

/* Send the greeting.
Includes the LevelInitialize packet. */
sendGreeting(
	CxServer *const,
	SocketNumber const,
	char const*const,
	char const*const
),

sendLevelChunkSize(
	CxServer *const,
	SocketNumber const,
	short const
),

sendLevelChunk(
	CxServer *const,
	SocketNumber const,
	unsigned char const*const
),

sendPercentComplete(
	CxServer *const,
	SocketNumber const,
	char const
);

#endif /* CX_SERVER_H */
