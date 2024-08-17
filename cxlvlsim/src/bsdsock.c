#ifdef _WINDOWS
#include <WinSock.h>
#endif

#define _ register
char a;

/*
int listenermake(void) {
	struct sockaddr_in a;
	_ SOCKET const b = socket(
		AF_INET, SOCK_STREAM, 0
	);

	if (b == INVALID_SOCKET)
		return 0;

	a.sin_family = AF_INET;
	a.sin_addr.s_addr = INADDR_ANY;
	a.sin_port = htons(25565);

	if (bind(
		b, (void *)&a, sizeof a
	))
		return 0;

	if (listen(b, SOMAXCONN))
		return 0;

	return 1;
}*/
