/* wse.c */

#include "common.h"

/* #define UNUSED(...) (0)*/

typedef mci (*wtup)(
	struct mcx *ctx, mcu *dst,
	mci qty, void *src
);

/* Byte Write, Type Increment 0. */
static mci
bwti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
);

/* Byte Write, Type Increment String. */
static mci
bwtis(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
);

/* Byte Write, Type Reset. */
static mci
bwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
);

/* Diff[3] Write, Type Increment 0. */
static mci
d3wti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mccd src[3]
);

/* Difference[3] Write, Type Reset. */
static mci
d3wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccd src[3]
);

/* Entity STate Write, Type Reset. */
static mci
estwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, const struct mceup *src
);

/* Integer Write, Type Incr. Array. */
static mci
iwtia(
	struct mcx *ctx, mcu dst[],
	mci qty, mci src
);

/* Integer[3] Write, Type Incr. 0. */
static mci
i3wti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mcci src[3]
);

/* Integer[3] Write, Type Reset. */
static mci
i3wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcci src[3]
);

/* String Write, Type Increment 0. */
static mci
swti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
);

/* String Write, Type Incr. String. */
static mci
swtis(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
);

/* String Write, Type Reset. */
static mci
swtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
);

/* Unsigned Write, Type Reset. */
static mci
uwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcu src
);

/* Unsigned[2] Write, Type Reset. */
static mci
u2wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccu src[2]
);

typedef mci (MCF pgwtr)(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mcgto *src
);

static pgwtr pgw1, pgw2, pgw3, pgw4;

static mci
pgw1(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mcgto *src
) {
	return bwtis(
		ctx, dst, qty, src->gpv
	);
}
static mci
pgw2(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mcgto *src
) {
	if (src->gnam) {
		mci gnln = src->gnln;

		qty = min(qty, gnln);
	}
	if (3 > ctx->curpv) {
		qty = swtr(
			ctx, dst, qty,
			src->gnam
		);
	}
	else {
		qty = swtis(
			ctx, dst, qty,
			src->gnam
		);
	}
	if (src->gnam) {
		src->gnln -= (mcb)qty;
		src->gnam = csincr(
			src->gnam, qty
		);
	}
	return qty;
}
static mci
pgw3(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mcgto *src
) {
	if (src->gmsg) {
		mci gmln = src->gmln;

		qty = min(qty, gmln);
	}
	if (6 > ctx->curpv) {
		qty = swtr(
			ctx, dst, qty,
			src->gmsg
		);
	}
	else {
		qty = swti0(
			ctx, dst, qty,
			src->gmsg
		);
	}
	if (src->gmsg) {
		src->gmln -= (mcb)qty;
		src->gmsg = csincr(
			src->gmsg, qty
		);
	}
	return qty;
}
static mci
pgw4(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mcgto *src
) {
	return uwtr(
		ctx, dst, qty, src->gcfg
	);
}

typedef mci (MCF ldwtr)(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mccho *src
);

static ldwtr ldw1, ldw2, ldw3;

static mci
ldw1(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mccho *src
) {
	return iwtia(
		ctx, dst, qty, src->clen
	);
}
static mci
ldw2(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mccho *src
) {
	if (src->cdat) {
		qty = min(
			qty, src->cdln
		);
	}
	qty = writex(
		&ctx->w, dst, qty,
		src->cdat
	);
	if (src->cdat) {
		src->cdln -= qty;
		src->cdat = caincr(
			src->cdat, qty
		);
	}
	if (0 == ctx->w.rem) {
		tincr(&ctx->w, 0);
	}
	return qty;
}
static mci
ldw3(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mccho *src
) {
	return bwtr(
		ctx, dst, qty, src->cpct
	);
}

typedef mci(MCF buwtr)(
	struct mcx *ctx, mcu dst[],
	mci qty, const struct mcbup *src
);

static buwtr buw1, buw2;

static mci MCF
buw1(
	struct mcx *ctx, mcu dst[],
	mci qty, const struct mcbup *src
) {
	return i3wti0(
		ctx, dst, qty,
		(mccv *)&src->bpos
	);
}
static mci MCF
buw2(
	struct mcx *ctx, mcu dst[],
	mci qty, const struct mcbup *src
) {
	return bwtr(
		ctx, dst, qty, src->bnew
	);
}

typedef mci (MCF enwtr)(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mceno *src
);

static enwtr enw1, enw2, enw3;

static mci MCF
enw1(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mceno *src
) {
	return bwtis(
		ctx, dst, qty, src->eno
	);
}
static mci MCF
enw2(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mceno *src
) {
	mci renw;

	if (src->enam) {
		mci enln = src->enln;

		qty = min(qty, enln);
	}
	renw = swti0(
		ctx, dst, qty, src->enam
	);
	if (src->enam) {
		src->enln -= (mcb)renw;
		src->enam = csincr(
			src->enam, renw
		);
	}
	return renw;
}
static mci MCF
enw3(
	struct mcx *ctx, mcu dst[],
	mci qty, struct mceno *src
) {
	return estwtr(
		ctx, dst, qty,
		(mccv *)src
	);
}

static mcb
/* Type Increment X if Non-Zero. */
/*tixnz(
	struct mcx *ctx, mcb ti,
	mci qty, mci tlen
),*/
/* Type Increment X if Zero. */
tixz(
	struct mcx *ctx, mcb ti,
	mci qty, mci tlen
),
/* Type Reset if Non-Zero. */
/*trnz(mcb ti, mci qty),*/
/* Type Reset if Zero. */
trz(struct mcx *ctx, mcb ti, mci qty);


/* Return 0 if done.
 * Return + if wrote.
 * Return - if not enough to write any.
 */

mci MCF
senw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	enum mcsevt *arg = src;
	enum pnum op;
	register mci rnw;

	if (0 != ctx->w.type) {
		/* Leftover from a
		 * previous write.
		 * Just return 0 to mark
		 * the end. */
		ctx->w.type = 0;
		rnw = 0;
	}
	else {
		op = seencode(*arg);
		if (PNINVALID == op) {
			rnw
			= MCEINVALIDOP;
		}
		else {
			/* Set to
			MCSTRLEN just in
			case. */
			rnw = bwtis(
				ctx,
				dst,
				qty,
				(mcb)op
			);
			ctx->w.evt = *arg;
			if (
				(
					*arg
					== MCSEPGREETING
				)
				&& (
					0
					== ctx->curpv
				)
			) {
				tincr(&ctx->w, MCSTRLEN);
			}
		}
	}
	return rnw;
}


mci MCF
sepgw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci rpgw, pgi = 0;

	do {
		static pgwtr
		*const pgw[] = {
			0,	&pgw1,
			&pgw2,	&pgw3,
			&pgw4
		};
		rpgw = pgw[ctx->w.type](
			ctx, dst, qty,
			src
		);
		dst = &dst[rpgw];
		pgi += rpgw;
		qty -= rpgw;
	}
	while (
		rpgw && qty
		&& (0 != ctx->w.evt)
	);
	return pgi;
}
mci MCF
se0w(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	treset(&ctx->w);
	ctx->w.type = 0;
	dst = dst;
	qty = qty;
	src = src;
	return 0;
}
mci MCF
seldtw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci rdtw, ldti = 0;

	do {
		static ldwtr
		*const ldw[] = {
			0,	&ldw1,
			&ldw2,	&ldw3,
		};
		struct mccho *arg = src;

		if (
			MCARRAYLEN
			< arg->clen
		) {
			arg->clen
			= MCARRAYLEN;
		}
		rdtw = ldw[ctx->w.type](
			ctx, dst, qty,
			arg
		);
		dst = &dst[rdtw];
		ldti += rdtw;
		qty -= rdtw;
	}
	while (
		rdtw && qty
		&& (0 != ctx->w.evt)
	);
	return ldti;
}
mci MCF
seldmw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	register mci rldw;
	/*const struct mcvec *arg = src;*/

	rldw = i3wtr(
		ctx, dst, qty,
		(mcci *)src
	);
	return rldw;
}
mci MCF
sebuw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci rldw, bui = 0;

	do {
		static buwtr
			*const buw[] = {
			0,	&buw1,
			&buw2
		};

		rldw = buw[ctx->w.type](
			ctx, dst, qty,
			src
		);
		dst = &dst[rldw];
		qty -= rldw;
		bui += rldw;
	}
	while (
		rldw && qty
		&& (0 != ctx->w.evt)
	);
	return bui;
}
mci MCF
seenw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci renw, eni = 0;

	do {
		static enwtr
		*const enw[] = {
			0,	&enw1,
			&enw2,	&enw3
		};

		renw = enw[ctx->w.type](
			ctx, dst, qty,
			src
		);
		dst = &dst[renw];
		eni += renw;
		qty -= renw;
	}
	while (
		renw && qty
		&& (0 != ctx->w.evt)
	);
	return eni;
}
mci MCF
seestw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci retw, esti = 0;

	do {
		struct mceup *arg = src;

		switch (ctx->w.type) {
		case 1:
			retw = bwti0(
				ctx,
				dst,
				qty,
				arg->eno
			);
			break;
		case 2:
			retw = estwtr(
				ctx,
				dst,
				qty,
				arg
			);
			break;
		default:
			retw = 0;
			break;
		}
		dst = &dst[retw];
		esti += retw;
		qty -= retw;
	}
	while (
		retw && qty
		&& (0 != ctx->w.evt)
	);
	return esti;
}
mci MCF
seedw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci redw, edi = 0;

	do {
		struct mceup *arg = src;

		switch (ctx->w.type) {
		case 1:
			redw = bwti0(
				ctx,
				dst,
				qty,
				arg->eno
			);
			break;
		case 2:
			redw = d3wti0(
				ctx,
				dst,
				qty,
				(mccv *)&arg->epos
			);
			break;
		case 3:
			redw = u2wtr(
				ctx,
				dst,
				qty,
				(mccv *)&arg->head
			);
			break;
		default:
			redw = 0;
			break;
		}
		dst = &dst[redw];
		edi += redw;
		qty -= redw;
	}
	while (
		redw && qty
		&& (0 != ctx->w.evt)
	);
	return edi;
}
mci MCF
seesdw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci resw, esdi = 0;

	do {
		struct mceup *arg = src;

		switch (ctx->w.type) {
		case 1:
			resw = bwti0(
				ctx,
				dst,
				qty,
				arg->eno
			);
			break;
		case 2:
			resw = d3wtr(
				ctx,
				dst,
				qty,
				(mccv *)&arg->epos
			);
			break;
		default:
			resw = 0;
			break;
		}
		dst = &dst[resw];
		esdi += resw;
		qty -= resw;
	}
	while (
		resw && qty
		&& (0 != ctx->w.evt)
	);
	return esdi;
}
mci MCF
seehw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci rehw, ehi = 0;

	do {
		struct mceup *arg = src;

		switch (ctx->w.type) {
		case 1:
			rehw = bwti0(
				ctx,
				dst,
				qty,
				arg->eno
			);
			break;
		case 2:
			rehw = u2wtr(
				ctx,
				dst,
				qty,
				(mccv *)&arg->head
			);
			break;
		default:
			rehw = 0;
			break;
		}
		dst = &dst[rehw];
		ehi += rehw;
		qty -= rehw;
	}
	while (
		rehw && qty
		&& (0 != ctx->w.evt)
	);
	return ehi;
}
mci MCF
seexw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	register mci redw;
	mccb *arg = src;

	redw = bwtr(
		ctx, dst, qty, *arg
	);
	return redw;
}
mci MCF
secmw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	mci rcmw, cmi = 0;

	do {
		struct mcmso *arg = src;

		switch (ctx->w.type) {
		case 1:
			rcmw = bwtis(
				ctx,
				dst,
				qty,
				arg->eno
			);
			break;
		case 2:
			if (arg->msg) {
				mci mlen
				= arg->mlen;
				qty = min(
					qty,
					mlen
				);
			}
			rcmw = swtr(
				ctx,
				dst,
				qty,
				arg->msg
			);
			if (arg->msg) {
				arg->mlen
				-= (mcb)rcmw;
				arg->msg
				= csincr(
					arg->msg,
					rcmw
				);
			}
			break;
		default:
			rcmw = 0;
			break;
		}
		dst = &dst[rcmw];
		cmi += rcmw;
		qty -= rcmw;
	}
	while (
		rcmw && qty
		&& (0 != ctx->w.evt)
	);
	return cmi;
}
mci MCF
sepbw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	register mci rpbw;
	struct mcmso *arg = src;

	if (arg->msg) {
		mci mlen = arg->mlen;
		qty = min(qty, mlen);
	}
	rpbw = swtr(
		ctx, dst, qty, arg->msg
	);
	if (arg->msg) {
		arg->mlen -= (mcb)rpbw;
		arg->msg = csincr(
			arg->msg,
			rpbw
		);
	}
	return rpbw;
}
mci MCF
secfw(
	struct mcx *ctx, mcu dst[],
	mci qty, void *src
) {
	register mci rcfw;
	const mcu *arg = src;

	rcfw = uwtr(
		ctx, dst, qty, *arg
	);
	return rcfw;
}
static mci
bwti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
) {
	register const mci tlen = 1;
	register mci rbw;

	if (tlen > qty) {
		rbw = 0;
	}
	else {
		rbw = tlen;
		writeb(dst, src);
		tincr(&ctx->w, 0);
	}
	return rbw;
}
static mci
bwtis(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
) {
	register const mci tlen = 1;
	register mci rbw;

	if (tlen > qty) {
		rbw = 0;
	}
	else {
		rbw = tlen;
		writeb(dst, src);
		tincr(
			&ctx->w,
			MCSTRLEN
		);
	}
	return rbw;
}
static mci
bwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcb src
) {
	register const mci tlen = 1;
	register mci rbw;

	if (tlen > qty) {
		rbw = 0;
	}
	else {
		rbw = tlen;
		writeb(dst, src);
		treset(&ctx->w);
	}
	return rbw;
}
static mci
d3wti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mccd src[]
) {
	register const mci tlen = 3;
	register mci rd3w;

	if (tlen > qty) {
		rd3w = 0;
	}
	else {
		rd3w = tlen;
		write3d(dst, src);
		tincr(&ctx->w, 0);
	}
	return rd3w;
}
static mci
d3wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccd src[]
) {
	register const mci tlen = 3;
	register mci rd3w;

	if (tlen > qty) {
		rd3w = 0;
	}
	else {
		rd3w = tlen;
		write3d(dst, src);
		treset(&ctx->w);
	}
	return rd3w;
}
static mci
estwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, const struct mceup *src
) {
	register const mci tlen = 8;
	register mci resw;

	if (tlen > qty) {
		resw = MCENOTENOUGH;
	}
	else {
		resw = tlen;
		writevec(
			dst,
			(mccv *)&src->epos
		);
		write2u(
			&dst[6],
			(mccv *)&src->head
		);
		treset(&ctx->w);
	}
	return resw;
}
static mci
i3wti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mcci src[]
) {
	register const mci tlen = 6;
	register mci ri3w;

	if (tlen > qty) {
		ri3w = MCENOTENOUGH;
	}
	else {
		ri3w = tlen;
		writevec(dst, src);
		tincr(&ctx->w, 0);
	}
	return ri3w;
}
static mci
i3wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcci src[]
) {
	register const mci tlen = 6;
	register mci ri3w;

	if (tlen > qty) {
		ri3w = MCENOTENOUGH;
	}
	else {
		ri3w = tlen;
		writevec(dst, src);
		treset(&ctx->w);
	}
	return ri3w;
}
static mci
iwtia(
	struct mcx *ctx, mcu dst[],
	mci qty, mci src
) {
	register const mci tlen = 2;
	register mci riw;

	if (tlen > qty) {
		riw = 0;
	}
	else {
		riw = tlen;
		writei(dst, src);
		tincr(
			&ctx->w,
			MCARRAYLEN
		);
	}
	return riw;
}
static mci
swti0(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
) {
	register mci rsw, rem;
	register mcb type;

	rem = ctx->w.rem;
	rsw = writes(
		&ctx->w, dst, qty, src
	);
	type = ctx->w.type;
	rem = ctx->w.rem;

	type = tixz(ctx, type, rem, 0);
	ctx->w.type = type;
	return rsw;
}
static mci
swtis(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
) {
	register mci rsw, rem;
	register mcb type;

	rem = ctx->w.rem;
	rsw = writes(
		&ctx->w, dst, qty, src
	);
	rem = ctx->w.rem;
	type = ctx->w.type;
	type = tixz(
		ctx, type, rem, MCSTRLEN
	);
	ctx->w.type = type;
	return rsw;
}
static mci
swtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccc src[]
) {
	register mci rsw, rem;
	register mcb typ = ctx->w.type;

	rsw = writes(
		&ctx->w, dst, qty, src
	);
	rem = ctx->w.rem;
	ctx->w.type = trz(
		ctx, typ, rem
	);
	return rsw;
}
static mci
uwtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mcu src
) {
	register const mci tlen = 1;
	register mci ruw;

	if (tlen > qty) {
		ruw = 0;
	}
	else {
		ruw = tlen;
		writeu(dst, src);
		treset(&ctx->w);
	}
	return ruw;
}
static mci
u2wtr(
	struct mcx *ctx, mcu dst[],
	mci qty, mccu src[2]
) {
	register const mci tlen = 2;
	register mci ru2w;

	if (tlen > qty) {
		ru2w = 0;
	}
	else {
		ru2w = tlen;
		write2u(dst, src);
		treset(&ctx->w);
	}
	return ru2w;
}

/*
static mcb
tixnz(
	struct mcx *ctx, mcb ti,
	mci qty, mci tlen
) {
	if (qty) {
		ti += 1;
		ctx->w.rem = tlen;
	}
	return ti;
}*/
static mcb
tixz(
	struct mcx *ctx, mcb ti,
	mci qty, mci tlen
) {
	if (!qty) {
		ti += 1;
		ctx->w.rem = tlen;
	}
	return ti;
}
/*static mcb
trnz(mcb ti, mci qty) {
	if (qty) {
		ti = 0;
	}
	return ti;
}*/
static mcb
trz(struct mcx *ctx, mcb ti, mci qty) {
	if (!qty) {
		ti += 1;
		ctx->w.evt = 0;
	}
	return ti;
}
