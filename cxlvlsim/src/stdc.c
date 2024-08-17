/* This is the pure Standard C89
 * implementation.
 * This has some very important quirks.
 * The level won't tick until it
 * receives something through stdin.
 * Obviously this is usually
 * unacceptable, but it works well
 * enough for debugging or testing on a
 * new platform.
 * This is also how the local (through
 * the standard files) connection works,
 * even with socket support. */

#ifdef __MSP430__

#warning You need at least 1KiB of RAM and 16 KiB of ROM to use the Standard C Library.

#ifndef NDEBUG
#pragma diag_push
#pragma CHECK_MISRA("-20.9")
#pragma CHECK_MISRA("-20.12")
#endif

#endif

#ifdef _WINDOWS

/* Enable unsafe functions. */
#define _CRT_SECURE_NO_WARNINGS

/* For non-standard mode change. */
#include <io.h>
#include <fcntl.h>

#endif

#include <string.h>
#include <stdio.h>
#include "platform.h"

int
main(int ac, char **av);

i16
input(u8 *dst, i16 qty) {
	register size_t a = fread(
		dst, sizeof *dst,
		(size_t)qty, stdin
	);
	if (!a) {
		a = (unsigned)feof(
			stdin
		);
		a |=(unsigned)ferror(
			stdin
		);
		a = a? 0xFFFFu: a;
	}
	return (i16)a;
}
i16
output(cu8 *src, i16 qty) {
	register size_t a = fwrite(
		src, sizeof *src,
		(size_t)qty, stdout
	);
	if (ferror(stdout)) {
		a = 0xFFFFu;
	}
	return (i16)a;
}
void
flush(void) {
	fflush(stdout);
}
void
warn(cchr *msg) {
	fputs(msg, stderr);
}
void
scopy(char dst[64], cchr *src) {
	/* size_t a = strlen(src);
	size_t b = 64u < a? 64u: a;
	memcpy(dst, src, b); */
	strncpy(dst, src, 64u);
}
i8
strim(
	char dst[], cchr buf[64], i8 max
) {
	register i8 i = 64;

	do { --i; }
	while (i && (' ' == buf[i]));
	/* TODO: maybe change this. */
	++i;
	if (i > max) { i = -1; }
	else {
		memcpy(dst, buf, (u8)i);
		dst[i] = '\0';
	}
	return i;
}
i16
slen(register cchr src[64]) {
	register u8 i = 0u, len = 64u;

	while (
		('\0' != *src)
		&& (0 != len)
	) {
		src = &src[1];
		++i;
		--len;
	}
	return (i16)i;
}
void
mcopy(void *dst, cv *src, i16 qty) {
	memcpy(dst, src, (size_t)qty);
}

int
main(int ac, char *av[]) {
	int a = 1;

#ifdef _WINDOWS
	_setmode(0, _O_BINARY);
	_setmode(1, _O_BINARY);
#endif

	while (a != ac) {
		if (!strcmp(av[a], "-i")) {
			freopen(av[1+a], "rb", stdin);
			++a;
		}
		else if (!strcmp(av[a], "-o")) {
			freopen(av[1+a], "wb", stdout);
			++a;
		}
		else {
			fputs("Invalid parameter!\n", stderr);
			a = -1;
			break;
		}
		++a;
	}
	/*freopen("demo-in7.bin", "rb", stdin);
	freopen("demo-out7.bin", "wb", stdout);
	*/
	if (0 < a) {
		ac = start(ac, av);
	}
	return ac;
}

#ifdef __MSP430__

#ifndef NDEBUG
#pragma diag_pop
#endif

#endif
