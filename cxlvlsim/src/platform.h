#ifndef CX_PLATFORM_H
#define CX_PLATFORM_H

/* All the system-specific stuff goes
here. */

#define SYSTEM_THREAD_COUNT 2
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _WIN32
#define INLINE __inline
typedef void *file;
#else
typedef int file;
#endif

#ifdef __MSP430__
#define INLINE __inline
#endif

#define CXPAGESIZE (4 * 4 * 4)

typedef signed char	i8;
typedef signed short	i16;
typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned long	u32;

typedef const void	cv;
typedef const char	cchr;
typedef const u8	cu8;
typedef const i16	ci16;

/* Start of the actual program. */
int start(int ac, char **av);

/* Flush the output buffer. */
void flush(void);

/* Read bytes from standard input. */
i16 input(u8 *dst, i16 qty);

/* Write bytes to standard output. */
i16 output(cu8 *src, i16 qty);

/* Write message to standard error. */
void warn(cchr *msg);

/* Get clock time. */
u32 curtime(void);

/* Open a file, maybe read-only. */
file openf(cchr *fnam, int ro);

/* Close the file. */
void closef(file f);

/* Copy memory. */
void
mcopy(void *dst, cv *src, i16 qty);

/* Copies a string to a buffer. */
void
scopy(char dst[64], cchr *src);

/* Measures a string up to 64 bytes. */
i16
slen(cchr src[64]);

/* Trim a string and write to buffer. */
i8
strim(char dst[], cchr buf[64], i8 max);

void
tmp0(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CX_PLATFORM_H */
