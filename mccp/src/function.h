#ifndef MCCP_FUNCTION_H
#define MCCP_FUNCTION_H 1

/* Type readers */
treader
rctEventNumber, rctVersion,
rctName,	rctBlockChange,
rctActorNumber,	rctActorState,
rctMessage,	rctPassword,
rctClientFlags;

/* Type writers */
twriter
wstEventNumber,	wstVersion,
wstName,	wstDataLength,
wstDataChunk,	wstPercentComplete,
wstLevelSize,	wstBlockChange,
wstActorNumber,	wstActorState,
wstActorSlide,	wstActorHeading,
wstMessage,	wstPlayerFlags;

/* Event writers (in enum order?). */
ewtr
senw,	sepgw,	se0w,
seldtw,	seldmw,	sebuw,	seenw,
seestw,	seedw,	seesdw,	seehw,
seexw,	secmw,	sepbw,	secfw;

/* Check if difference is in range. */
mcu
dcheck(mcd dc);

mcu
d3chk(mccd d3c[3]);

/*          Read Basic Types          */

/* Read a clamped signed byte. */
mcb MCF
readb(mccu src[1]);

/* Read an unclamped signed byte. */
mci MCF
readd(mccu src[1]);

/* Read a clamped signed integer. */
mci MCF
readi(mccu src[2]);

void MCF
i3read(mci dst[3], mccu src[6]);

/* Read an unsigned byte. */
mcu MCF
readu(mccu src[1]);

/* Read a string of ASCII characters
with no non-printable characters. */
mci MCF
reads(
	struct mcseri *ser, mccu *src,
	mci qty, char *dst
);

/* Read arbitrary binary data. */
mci MCF
readx(
	struct mcseri *ser, mccu *src,
	mci qty, mcu *dst
);

/*         Write Basic Types          */

/* Write a clamped signed byte. */
void MCF
writeb(mcu dst[1], mcb src);

/* Write an unclamped signed byte. */
void MCF
writed(mcu dst[1], mcd src);

/* Write a clamped signed integer. */
void MCF
writei(mcu dst[2], mci src);

void MCF
writevec(mcu dst[6], mcci src[3]);

void MCF
write3d(mcu dst[3], mccd src[3]);

void MCF
write2u(mcu dst[2], mccu src[2]);

/* Write an unsigned byte. */
void MCF
writeu(mcu dst[1], mcu src);

/* Write a string of ASCII characters
with no non-printable characters. */
mci MCF
writes(
	struct mcseri *ser, mcu *dst,
	mci qty, mccc *src
);

/* Write arbitrary binary data. */
mci MCF
writex(
	struct mcseri *ser, mcu *dst,
	mci qty, mccu *src
);

/* More internal functions. */

/* Change the type of the serialized. */
void MCF
tchng(struct mcseri *ser, mcb nxt);

/* Change type and size to a string. */
void MCF
tchngs(struct mcseri *ser, mcb ts);

/* Change type and size to array. */
void MCF
tchngx(struct mcseri *ser, mcb tx);

/* Increment type and set size */
void MCF
tincr(struct mcseri *ser, mci tlen);

/* */
mci MCF
chkinparams(
	struct mcseri *ser, mccu *src,
	mci qty, mcb tsize
);

/* Called on invalid event number. */
mci MCF
invalen(mcb ei);

/* Called on invalid type number. */
mci MCF
invaltn(mcb ti);

/* Reset the type being copied. */
void MCF
treset(struct mcseri *ser);

/* Decode the client event number. */
enum mccevt MCF
cedecode(enum pnum pnc);

/* Encode the server event number. */
enum pnum MCF
seencode(enum mcsevt se);

/* Get the minimum. */
mci MCF
min(mci i0, mci i1);

/* Copies memory like memcpy. */
void MCF
mcopy_(mcu dst[], mccu src[], mci qty);

void MCF
mset(mcu dst[], mcu src, mci qty);

/* Increment a byte array pointer. */
mcu *MCF
aincr(mcu dst[], mci qty);

char *MCF
sincr(char dst[], mci qty);

mccu *MCF
caincr(mccu dst[], mci qty);

mccc *MCF
csincr(mccc dst[], mci qty);

#endif
