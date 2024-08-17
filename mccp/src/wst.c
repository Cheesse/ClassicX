/*         Write Server Type          */
#include "common.h"
mci
wstEventNumber(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	/* Type that comes next. */
	static const enum mcstyp
	stype[MCSECOUNT] = {
		MCSTEVTNO,
		MCSTVERSION,
		MCSTEVTNO,
		MCSTEVTNO,
		MCSTDATALEN,
		MCSTLVLSIZE,
		MCSTBLKCH,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTACTRNO,
		MCSTMSG,
		MCSTPLYRFLGS
	};
	_ mci ren;
	_ enum mcsevt e;
	_ enum mcstyp t;

	ren = mcstlen(ctx, MCSTEVTNO);
	if (0 == src) {
		ren = MCEUNKNOWN;
	}
	else if (ren > qty) {
		ren = MCENOTENOUGH;
	}
	else {
		e = *(const enum mcsevt *)src;
		if (0 == e) {
			ren = 0;
		}
		else {
			if (mcsemax[ctx->curpv] < e) {
				ren = MCEINVALIDOP;
			}
			else {
				writeb(
					dst,
					0,
					seencode(e)
				);
				t = stype[e];
				if (MCSTEVTNO != t) {
					ctx->w.evt = (mcb)e;
					ctx->w.type = t;
				}
				/* TODO: make this auto. */
				if (MCSTMSG == t) {
					tchngs(&ctx->w, MCSTMSG);
				}
			}
		}
	}
	return ren;
}
/* TODO: remove this? */
mci
wstVersion(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rv;
	mcb spv;

	spv = *(mccb *)src;
	rv = mcstlen(ctx, MCSTVERSION);
	if (rv > qty) {
		rv = MCENOTENOUGH;
	}
	else {
		/* TODO: make sure version is in
		bounds. */
		if (MCPVMAX < spv) {
			rv = MCEOUTOFRANGE;
		}
		else {
			if (dst) {
				*dst = (mcu)spv;
			}
			tchngs(&ctx->w, MCSTNAME);
		}
	}
	return rv;
}
/* TODO: include version number here? */
mci
wstName(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	qty = writes(
		&ctx->w, dst, qty, src
	);
	if (0 == ctx->w.rem) {
		switch (ctx->w.evt) {
		case MCSEPGREETING:
			if (3 > ctx->curpv) {
				treset(&ctx->w);
			}
			else {
				tchngs(&ctx->w, MCSTMSG);
			}

			break;
		case MCSEENEW:
			tchng(
				&ctx->w,
				MCSTACTRSTAT
			);
			break;
		default:
			treset(&ctx->w);
			break;
		}
	}
	return qty;
}
/* Ok, I've decided this library won't
be responsible for compressing the level
data. */

/* Gzips the level volume and puts it in
the gzip buffer.

Does not actually write anything to the
output buffer. */
/* int wstLevelVolume(
	_ struct mccpsession *C z,
	_ mccpOutputByte *C y,
	_ C unsigned const x,
	_ C union mccpinput const w
) {
	_ C long *C a = w.pointer;

	_ mccpSerializationState
	*C b = &z->WriteState;

	_ mccp

	return 0;
} */
mci
wstDataLength(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rdl;

	rdl = mcstlen(ctx, MCSTDATALEN);
	if (rdl > qty) {
		rdl = MCENOTENOUGH;
	}
	else {
		writei(dst, 0, *(mcci *)src);
		tchngx(&ctx->w, MCSTDATACHUNK);
	}
	return rdl;
}
mci
wstDataChunk(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	qty = writex(
		&ctx->w, dst, qty, src
	);
	if (0 == ctx->w.rem) {
		tchng(&ctx->w, MCSTPERCENT);
	}
	return qty;
}
mci
wstPercentComplete(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rpc;

	rpc = mcstlen(ctx, MCSTPERCENT);
	if (rpc > qty) {
		rpc = MCENOTENOUGH;
	}
	else {
		writeb(dst, 0, *(mccb *)src);
		treset(&ctx->w);
	}
	return rpc;
}
mci
wstLevelSize(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rls;
	_ const struct mcvec *ls = src;

	rls = mcstlen(ctx, MCSTLVLSIZE);
	if (rls > qty) {
		rls = MCENOTENOUGH;
	}
	else {
		writei(dst, 0, ls->x);
		writei(dst, 2, ls->y);
		writei(dst, 4, ls->z);
		treset(&ctx->w);
	}
	return rls;
}
mci
wstBlockChange(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rbc;
	_ const struct mcbupd *upd = src;

	rbc = mcstlen(ctx, MCSTBLKCH);
	if (rbc > qty) {
		rbc = MCENOTENOUGH;
	}
	else {
		writei(dst, 0, upd->bpos.x);
		writei(dst, 2, upd->bpos.y);
		writei(dst, 4, upd->bpos.z);
		writeb(dst, 6, upd->bnew);
		treset(&ctx->w);
	}
	return rbc;
}
mci
wstActorNumber(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci ran;

	ran = mcstlen(ctx, MCSTACTRNO);
	if (ran > qty) {
		ran = MCENOTENOUGH;
	}
	else {
		writeb(dst, 0, *(mccb *)src);
		switch (ctx->w.evt) {
		case MCSEENEW:
			tchngs(&ctx->w, MCSTNAME);
			break;
		case MCSEESTATE:
			tchng(
				&ctx->w,
				MCSTACTRSTAT
			);
			break;
		case MCSEEDIFF:
		case MCSEESLIDE:
			tchng(
				&ctx->w,
				MCSTACTRSLID
			);
			break;
		case MCSEEHEAD:
			tchng(
				&ctx->w,
				MCSTACTRHEAD
			);
			break;
		case MCSECMSG:
			tchngs(&ctx->w, MCSTMSG);
			break;
		default:
			treset(&ctx->w);
			break;
		}
	}
	return ran;
}
mci
wstActorState(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci ras;
	_ const struct mceupd *as = src;

	ras = mcstlen(ctx, MCSTACTRSTAT);
	if (ras > qty) {
		ras = MCENOTENOUGH;
	}
	else {
		writei(dst, 0, as->epos.x);
		writei(dst, 2, as->epos.y);
		writei(dst, 4, as->epos.z);
		writeu(dst, 6, as->head.yaw);
		writeu(dst, 7, as->head.phi);
		treset(&ctx->w);
	}
	return ras;
}
mci
wstActorSlide(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rad;
	_ const struct mcvec *ad = src;

	rad = mcstlen(ctx, MCSTACTRSLID);
	if (rad > qty) {
		rad = MCENOTENOUGH;
	}
	else {
		writed(dst, 0, ad->x);
		writed(dst, 1, ad->y);
		writed(dst, 2, ad->z);
		switch (ctx->w.evt) {
		case MCSEEDIFF:
			tchng(
				&ctx->w,
				MCSTACTRHEAD
			);
			break;
		default:
			treset(&ctx->w);
			break;
		}
	}
	return rad;
}
mci
wstActorHeading(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rah;
	_ const struct mchead *ah = src;

	rah = mcstlen(ctx, MCSTACTRHEAD);
	if (rah > qty) {
		rah = MCENOTENOUGH;
	}
	else {
		writeu(dst, 0, ah->yaw);
		writeu(dst, 1, ah->phi);
		treset(&ctx->w);
	}
	return rah;
}
mci
wstMessage(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	qty = writes(
		&ctx->w, dst, qty, src
	);

	if (0 == ctx->w.rem) {
		switch (ctx->w.evt) {
		case MCSEPGREETING:
			if (6 > ctx->curpv) {
				treset(&ctx->w);
			}
			else {
				tchng(
					&ctx->w,
					MCSTPLYRFLGS
				);
			}
			break;
		default:
			treset(&ctx->w);
			break;
		}
	}
	return qty;
}
mci
wstPlayerFlags(
	struct mcx *ctx, mcu *dst,
	mci qty, mccv *src
) {
	mci rpf;

	rpf = mcstlen(ctx, MCSTPLYRFLGS);
	if (rpf > qty) {
		rpf = MCENOTENOUGH;
	}
	else {
		writeb(dst, 0, *(mccb *)src);
		treset(&ctx->w);
	}
	return rpf;
}
enum mcsevt dummy(void);
enum mcsevt dummy(void) {
	return mcsemax[0];
}
