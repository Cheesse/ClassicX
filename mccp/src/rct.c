/*          Read Client Type          */

#include "common.h"

mci
rctEventNumber(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	static const enum mcctyp
	ten[MCCECOUNT] = {
		MCCTEVTNO,
		MCCTVERSION,
		MCCTBLKCH,
		MCCTACTRNO,
		MCCTACTRNO,
	};
	register mci enln;

	enln = mcctlen(ctx, MCCTEVTNO);
	if (enln > qty) {
		enln = MCENOTENOUGH;
	}
	else {
		enum mccevt
		*out = dst, cevt;
		mci i = *src;

		cevt = cedecode(
			(enum pnum)i
		);
		*out = cevt;
		ctx->r.evt = cevt;
		if (cevt == MCCENOTHING) {
			ctx->r.type = MCCTINVALID;
		}
		else {
			ctx->r.type = ten[*out];
		}
	}
	return enln;
}
mci
rctVersion(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	mci vlen;

	vlen = mcctlen(ctx, MCCTVERSION);
	if (vlen > qty) {
		vlen = MCENOTENOUGH;
	}
	else {
		mcb cpv;
		mcu b;

		b = *src;
		if (32u > b) {
			cpv = (mcb)b;
			tchngs(&ctx->r, MCCTNAME);
		}
		else {
			cpv = 0;
			ctx->r.rem
			= (mci)MCSTRLEN - 1;
			tchng(&ctx->r, MCCTNAME);
		}
		if (dst) {
			*(mcb *)dst = cpv;
		}
		ctx->curpv = cpv;
	}
	/*dst? readb(a, src, qty, dst): 0;
	* If we try to read a version
	number and we get a printable
	ASCII character, it's safe to
	assume protocol version 0.

	Don't consume this byte and
	return. */
	return vlen;
}
mci
rctName(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	qty = reads(
		&ctx->r, src, qty, dst
	);
	if (0 == ctx->r.rem) {
		if (3 > ctx->curpv) {
			treset(&ctx->r);
			ctx->r.type = 0;
		}
		else {
			tchngs(
				&ctx->r,
				MCCTPASS
			);
		}
	}
	return qty;
	/* changeTypeAndSize
	(a, mccpctPassword, 64); */
}
mci
rctBlockChange(
	struct mcx *ctx, mccu src[],
	mci qty, void *dst
) {
	mci bcln = mcctlen(ctx, MCCTBLKCH);

	if (bcln > qty) {
		bcln = MCENOTENOUGH;
	}
	else {
		_ struct mcbup *upd = dst;

		i3read(
			(void *)&upd->bpos,
			src
		);
		src = &src[6];
		upd->umod = readb(src);
		src = &src[1];
		upd->bnew = readb(src);
		treset(&ctx->r);
		ctx->r.type = 0;
	}
	return bcln;
}
mci
rctActorNumber(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	mci anln;

	anln = mcctlen(ctx, MCCTACTRNO);
	if (anln > qty) {
		anln = MCENOTENOUGH;
	}
	else {
		static void
		(*const ctup[MCCECOUNT])(
			struct mcseri *srs,
			mcb cts
		) = {
			0, 0, 0, &tchng,
			&tchngs
		};
		static const mcb
		ctn[MCCECOUNT] = {
			0, 0, 0,
			MCCTACTRSTAT,
			MCCTMSG
		};
		void (*tup)(
			struct mcseri *srs,
			mcb cts
		);
		mcb *an = dst;
		mcb cei;

		cei = ctx->r.evt;
		tup = ctup[cei];

		if (an) {
			*an = readb(src);
		}
		tup(&ctx->r, ctn[cei]);
	}
	return anln;
}
mci
rctActorState(
	struct mcx *ctx, mccu src[],
	mci qty, void *dst
) {
	mci asln;

	asln = mcctlen(ctx, MCCTACTRSTAT);
	if (asln > qty) {
		asln = MCENOTENOUGH;
	}
	else {
		struct mceup *e = dst;

		i3read(
			(void *)&e->epos,
			src
		);
		src = &src[6];
		e->head.yaw = readu(src);
		src = &src[1];
		e->head.phi = readu(src);
		treset(&ctx->r);
		ctx->r.type = 0;
	}
	return asln;
}
mci
rctMessage(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	qty = reads(
		&ctx->r, src, qty, dst
	);
	if (0 == ctx->r.rem) {
		treset(&ctx->r);
		ctx->r.type = 0;
	}
	return qty;
}
mci
rctPassword(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	qty = reads(
		&ctx->r, src, qty, dst
	);
	if (0 == ctx->r.rem) {
		if (6 > ctx->curpv) {
			treset(&ctx->r);
			ctx->r.type = 0;
		}
		else {
			tchng(&ctx->r, MCCTCFLGS);
		}
	}
	return qty;
}
/* TODO: CPE ONLY */
mci
rctClientFlags(
	struct mcx *ctx, mccu *src,
	mci qty, void *dst
) {
	mci cfln = mcctlen(ctx, MCCTCFLGS);

	if (cfln > qty) {
		cfln = MCENOTENOUGH;
	}
	else {
		mcb f;

		f = readb(src);
		if (dst) {
			*(mcb *)dst = f;
		}
		treset(&ctx->r);
		ctx->r.type = 0;
	}
	return cfln;
}
