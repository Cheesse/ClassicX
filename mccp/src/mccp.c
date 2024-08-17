/* MineCraft Classic Protocol Library

 * How this will be done for receiving:
 * make a queue for incoming packets and
 * a queue for outgoing packets.

 * Have one thread manage all
 * connections using select.

 * When data is ready to be received,
 * the thread will copy the data to a
 * buffer and begin deserializing.

 * The thread will remember the state of
 * the deserialization if the packet is
 * incomplete.

 * When data is ready to be sent, the
 * thread will serialize the data to a
 * buffer and send it.

 * The thread will remember the state of
 * the buffer if the packet is
 * imcomplete.

 * The client application may send
 * packets on demand without blocking
 * and may receive packets on demand
 * without blocking. */

#include "common.h"

/* TODO: need special functions to read
 * and write each structure.

 * Actually, no.

 * That can be done in each read or
 * write type function.

 * Of course, that makes the special
 * endianness-specific code mostly
 * obsolete. */

/* Basically just memcpy but aware of
 * endianness. */
/*
static void structcpy(
	_ mccpu *dst,
	_ mccpu const *src,
	_ unsigned qty
) {
	while (--qty) {
	/ TODO: is this correct? *
		*dst = *src++;
		dst += MCCPEND? 1: -1;
	}
}*/

/* TMP: Implements reads and readx. */
static mci
readz(
	struct mcseri *ser, mccu *src,
	mci qty, void *dst
);

/* TMP: Implements writes and writex. */
static mci
writez(
	struct mcseri *ser, mcu *dst,
	mci qty, mccv *src,
	mcu fill
);
mcu
dcheck(mcd dc) {
	register mcd dcu;

	if (0 < dc) {
		dc = -dc;
	}
	dcu = (-127 > dc);
	return (mcu)(u16)dcu;
}
mcu
d3chk(mccd d3c[3]) {
	register mcu d3cu = 0, d3ci = 3;

	do {
		d3cu |= dcheck(*d3c);
		d3c = &d3c[1];
		--d3ci;
	} while (d3ci);
	return d3cu;
}
mcb
readb(mccu src[]) {
	register mcu bru = *src;

	return (mcb)bru;
}
mci
readd(mccu src[]) {
	register mci dr;
	register mcu dru = *src;

	dr = (mci)dru;
	if (127 < dr) {
		dr -= 256;
	}
	return dr;
}
mci
readi(mccu src[]) {
	u16 ir;
	mcu iu[2];
	/*mcu riu = 2, ril = 0;*/

	/*do {
		--riu;
		iu[ril] = src[riu + (u16)offset];
		++ril;
	}
	while (riu);*/
	iu[1] = *src;
	src = &src[1];
	iu[0] = *src;
	/*ir = (u16)src[(u16)offset] << 8;
	++offset;
	ir |= (u16)src[(u16)offset];*/
	ir = *(u16 *)(void *)iu;
	return (mci)ir;
}
void
i3read(mci dst[3], mccu src[6]) {
	register mcu i3ri = 3;

	do {
		*dst = readi(src);
		dst = &dst[1];
		src = &src[2];
		--i3ri;
	} while (i3ri);
}
mcu
readu(mccu src[]) {
	return *src;
}
mci
reads(
	struct mcseri *ser, mccu *src,
	mci qty, char *dst
) {
	return readz(
		ser, src, qty, dst
	);
}
mci
readx(
	struct mcseri *ser, mccu *src,
	mci qty, mcu *dst
) {
	return readz(
		ser, src, qty, dst
	);
}
static mci
readz(
	struct mcseri *ser, mccu *src,
	mci qty, void *dst
) {
	register const mci
	mr = min(qty, ser->rem);
	if (dst) {
		mcopy_(dst, src, mr);
	}
	ser->rem -= mr;
	return mr;
}
/*
int readstruct(
	_ struct mccpseri *const z,
	_ mccpu const *const src,
	_ unsigned const qty,
	_ void *const dst
) {
	_ mccpu*const a = dst;
	_ mccpi
	*const rem = &z->remaining;
	_ mccpi const
	min = minimum(*rem, (mccpi)qty),
	start = MCCPEND? 0: min - 1;
	structcpy(a + start, src, min);
	*rem -= min; return qty;
}
*/

/* WRITE */

void MCF
writeb(mcu dst[1], mcb src) {
	*dst = (mcu)src;
}
void MCF
writed(mcu dst[1], mci src) {
	/*register u16 dw;*/

	/*dw = (u16)src;*/
	if (0 > src) {
		src += 256;
	}
	/*if (0 > src) {
		dw &= 0xFFu;
		dw += 256u;
	}
	else {
		dw &= 0x7Fu;
	}*/

	*dst = (mcu)(0xFFu & (u16)src);
}
void MCF
writei(mcu dst[2], mci src) {
	register u16 iw;
	/*mcu iu[2] = {
		((mcu [2])(void *)&src)[0],
		((mcu [2])(void *)&src)[1]
	};*/

	iw = (u16)src;
	/*dst[1u] = iu[0u];
	dst[0u] = iu[1u];*/

	dst[1u] = (mcu)(0xFFu & iw);
	dst[0u] = (mcu)(iw >> 8);
}
void MCF
writeu(mcu dst[1], mcu src) {
	*dst = src;
}
void MCF
writevec(mcu dst[6], mcci src[3]) {
	register mcu wv = 3u;

	/*do {
		--wv;
		wvu -= 2u;
		writei(
			&dst[wvu],
			src[wv]
		);
	} while (wv);*/
	do {
		writei(dst, *src);
		dst = &dst[2];
		src = &src[1];
		--wv;
	} while (wv);
	/*writei(dst, *src);
	dst = &dst[2];
	src = &src[1];
	writei(dst, *src);*/
}
void MCF
write3d(mcu dst[3], mccd src[3]) {
	register mcu w3d = 3u;

	do {
		writed(dst, *src);
		dst = &dst[1];
		src = &src[1];
		--w3d;
	} while (w3d);
}

void MCF
write2u(mcu dst[2], mccu src[2]) {
	/*register mcu w2u = 2u;*/

	/*do {
		--w2u;
		writeu(
			&dst[w2u],
			src[w2u]
		);
	} while (w2u);*/
	writeu(dst, *src);
	dst = &dst[1];
	src = &src[1];
	writeu(dst, *src);
}
mci
writes(
	struct mcseri *ser, mcu *dst,
	mci qty, mccc *src
) {
	return writez(
		ser, dst, qty, src, ' '
	);
}
mci
writex(
	struct mcseri *ser, mcu *dst,
	mci qty, mccu *src
) {
	return writez(
		ser, dst, qty, src, 0u
	);
}
static mci
writez(
	struct mcseri *ser, mcu *dst,
	mci qty, mccv *src, mcu fill
) {
	register const mci
	mw = min(qty, ser->rem);

	src? mcopy_(dst, src, mw)
	: mset(dst, fill, mw);
	ser->rem -= mw;
	return mw;
}
/*int writebig(
	_ struct mccpseri*const z,
	_ mccpu*const dst,
	_ unsigned const qty,
	_ void const*const src
) {
	_ mccpi
	*const rem = &z->remaining;
	_ mccpi const
	min = minimum(*rem, (mccpi)qty),
	start = MCCPEND? 0: min - 1;
	structcpy(
		dst + start, src, min
	);
	*rem -= min;
	return min;
}*/
/*
int writeFill (
	_ SerializationState_ z,
	_ mccpOutputBytes y,
	_ mccpNumberOfBytes x,
	_ InputByte w
) {
	_ Counter_
	c = &z->RemainingBytes;

	_ mccpNumberOfBytes
	d = minimum (*c, x);

	memset (y, w, d);
	*c -= d;

	return d;
}*/

void
tchng(struct mcseri *ser, mcb nxt) {
	ser->type = nxt;
	if (!nxt) {
		ser->evt = 0;
	}
}
void
tchngs(struct mcseri *ser, mcb ts) {
	ser->rem = (mci)MCSTRLEN;
	tchng(ser, ts);
}
void
tchngx(struct mcseri *ser, mcb tx) {
	ser->rem = (mci)MCARRAYLEN;
	tchng(ser, tx);
}
void MCF
tincr(
	struct mcseri *ser, mci tlen
) {
	++ser->type;
	ser->rem = tlen;
}
void
treset(struct mcseri *ser) {
	ser->evt = 0;
}
/*
int invalidEventNumber (
	_ EventNumberIn z
) {
	return ~mccpeUnspecifiedError;
}

int invalidTypeNumber (
	_ TypeNumber z
) {
	return ~mccpeUnspecifiedError;
}
*/
enum mccevt
cedecode(enum pnum pnc) {
	static const enum mccevt
	dec[PNCOUNT] = {
		MCCEGREETING,
		MCCENOTHING,
		MCCENOTHING,
		MCCENOTHING,
		MCCENOTHING,
		MCCEBLKEDIT,
		MCCENOTHING,
		MCCENOTHING,
		MCCEPLYRSTAT,
		MCCENOTHING,
		MCCENOTHING,
		MCCENOTHING,
		MCCENOTHING,
		MCCECHATMSG,
		MCCENOTHING,
		MCCENOTHING
	};
	register enum mccevt ced;
	
	if (
		(0 > pnc)
		|| (PNCOUNT <= pnc)
	) {
		ced = MCCENOTHING;
	}
	else {
		ced = dec[pnc];
	}
	return ced;
}
enum pnum
seencode(enum mcsevt se) {
	static const enum pnum enc[] = {
		PNINVALID,
		PNGREETING,
		PNPING,
		PNNEWLVL,
		PNLVLDATA,
		PNLVLSIZE,
		PNBLKCH,
		PNNEWACTOR,
		PNACTORSTAT,
		PNACTORCH,
		PNACTORSLID,
		PNACTORHEAD,
		PNACTORDIED,
		PNCHATMSG,
		PNBYE,
		PNUSERCH
	};
	register enum pnum
	pns = PNINVALID;

	if (
		(MCSEINVALID < se)
		&& (MCSECOUNT > se)
	) {
		pns = enc[se];
	}
	return pns;
}
mci
min(mci i0, mci i1) {
	return i0 < i1? i0: i1;
}
void
mcopy_(mcu dst[], mccu src[], mci qty) {
	while (0 != qty) {
		*dst = *src;
		dst = &dst[1];
		src = &src[1];
		--qty;
	}
}
void
mset(mcu dst[], mcu src, mci qty) {
	while (0 != qty) {
		*dst = src;
		dst = &dst[1];
		--qty;
	}
}
mcu
*aincr(mcu dst[], mci qty) {
	return &dst[qty];
}
char
*sincr(char dst[], mci qty) {
	return &dst[qty];
}
mccu
*caincr(mccu dst[], mci qty) {
	return &dst[qty];
}
mccc
*csincr(mccc dst[], mci qty) {
	return &dst[qty];
}
