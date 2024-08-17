//#include <winsdkver.h>

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT \
	_WIN32_WINNT_NT4
#endif /* _WIN32_WINNT */

#define WIN32_LEAN_AND_MEAN	1
#define NOGDICAPMASKS	1
#define NOVIRTUALKEYCODES	1
#define NOWINMESSAGES	1
#define NOWINSTYLES	1
#define NOSYSMETRICS	1
#define NOMENUS	1
#define NOICONS	1
#define NOKEYSTATES	1
#define NOSYSCOMMANDS	1
#define NORASTEROPS	1
#define NOSHOWWINDOW	1
#define OEMRESOURCE	1
#define NOATOM	1
#define NOCLIPBOARD	1
#define NOCOLOR	1
#define NOCTLMGR	1
#define NODRAWTEXT	1
#define NOGDI	1
#define NOKERNEL	1
#define NOUSER	1
#define NONLS	1
#define NOMB	1
#define NOMEMMGR	1
#define NOMETAFILE	1
#define NOMINMAX	1
#define NOMSG	1
#define NOOPENFILE	1
#define NOSCROLL	1
#define NOSERVICE	1
#define NOSOUND	1
#define NOTEXTMETRIC	1
#define NOWH	1
#define NOWINOFFSETS	1
#define NOCOMM	1
#define NOKANJI	1
#define NOHELP	1
#define NOPROFILER	1
#define NODEFERWINDOWPOS	1
#define NOMCX	1
#define NOIME	1
#include <Windows.h>
#include <WinSock2.h>
#include "platform.h"

/* 16 * 256 <= 4k, which is usually 1
page; must be a power of 2. */
#define TASK_QUEUE_SIZE 256u 
/* Task queue. */
//static task tqueue[TASK_QUEUE_SIZE];
static unsigned tfront = 0, tcount = 0;

#define PAGE_SIZE 4096
static CRITICAL_SECTION
CriticalSection;

static HANDLE
hWorkerThreads
[SYSTEM_THREAD_COUNT],
hStreamThread,
hTaskPending,
hHeap;
static HINSTANCE hInstance;
static DWORD dwTlsIndex;

#define _ register

/* Main function for worker
threads. */
static DWORD WINAPI WorkerProc (
	_ LPVOID const lpThreadParameter
) {
	TlsSetValue (
		dwTlsIndex, 
		lpThreadParameter
	);

	/* Normally we would
	wait, but we expect no
	APCs on these threads
	since they do not deal
	with I/O. */
	/* while (taskdo ()); */

	return 0;
}

/* Main function for stream
threads. */
static DWORD WINAPI StreamProc(
	_ LPVOID const
	lpThreadParameter
) {
	TlsSetValue (
		dwTlsIndex,
		lpThreadParameter
	);

	for (;;) {
		SleepEx
		(50, TRUE);

		/* Change this
		to be a task. */
		//nettick ();
	}
}

/* TODO: Rework these to use the
actual page size.
	
(Typically it is 4096 bytes.) */
void *palloc (void) {
	return VirtualAlloc (
		NULL,
		PAGE_SIZE,
		MEM_COMMIT
		| MEM_RESERVE,
		PAGE_READWRITE
	);
}

void pfree (_ void *const ptr){
	VirtualFree(ptr,PAGE_SIZE,MEM_DECOMMIT|MEM_RELEASE);
}
int startSystem (void) {
	WSADATA a;
	_ INT i;

	WSAStartup(MAKEWORD(2, 2), &a);

	hInstance
	= GetModuleHandle(NULL);

	hHeap = GetProcessHeap();

	dwTlsIndex = TlsAlloc();

	InitializeCriticalSection
	(&CriticalSection);

	hTaskPending = CreateEvent(
		NULL, FALSE,
		FALSE,
		TEXT("TASK QUEUE")
	);

	i = SYSTEM_THREAD_COUNT;

	while (i--) {
		SetThreadIdealProcessor(
			hWorkerThreads[i]
			= CreateThread(
				NULL,
				0,
				WorkerProc,
				(LPVOID)
				(INT_PTR)i,
				0,
				NULL
			),
			i
		);
	}


	hStreamThread
	= CreateThread (
		NULL,
		0,
		StreamProc,
		NULL,
		0,
		NULL
	);

	return 0;
}

int getThreadNumber (void) {
	return (int)(INT_PTR)TlsGetValue
	(dwTlsIndex);
}

int doTask (void) {
	//_ task const *t;
		
	//_ taskfunc f;
		
	/* _ void const* a; */

	/* Wait if there is
	nothing in the queue. */
	WaitForSingleObject (
		hTaskPending,
		INFINITE
	);

	EnterCriticalSection
	(&CriticalSection);

	//t = tqueue + tfront;

	//f = t->callback;

	//a = t->param;

	tfront = (tfront + 1)
	& (TASK_QUEUE_SIZE - 1);

	if (--tcount) SetEvent
	(hTaskPending);

	LeaveCriticalSection
	(&CriticalSection);

	/* f(a); */

	return 0;
}
/*
void createTask (
	_ int const urgent,
	_ taskfunc const callback,
	_ void const*const param
){
	//_ task *t;

	/* Currently does not
	check for a full queue.
	*
	EnterCriticalSection
	(&CriticalSection);

	if (urgent) {
		tfront
		= (tfront - 1)
		& (TASK_QUEUE_SIZE - 1);

		t = tqueue
		+ tfront;
			
		tcount++;
	}
	else t = tqueue
	+ (
		(tfront + tcount++)
		& (TASK_QUEUE_SIZE - 1)
	);

	t->param = param;
		
	t->callback = callback;
	if (tcount) SetEvent
	(hTaskPending);

	LeaveCriticalSection
	(&CriticalSection);
}*/

/*
ThreadNumber makeThread(
	_ void*(WINAPI*const z)(),
	_ void*const y
) {
	HANDLE const a = CreateThread(
		0, 0, z, y, 0,
		0
	);

	return 0;
}*/
/*
int WINAPI WinMain(
	_ HINSTANCE z,
	_ HINSTANCE y,
	_ LPSTR x,
	_ int w
) {
	return 0;
}*/