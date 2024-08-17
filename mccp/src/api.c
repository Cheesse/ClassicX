#include "common.h"

mccb
mcbmax[] = {
	18, 18, 18, 18, 18, 20, 41, 49
};

#ifndef MCNOSTR

static mccc *const
mcbnam_[MCBMAX + 1] = {
	"air",  	"stone",
	"grass",	"dirt",
	"cobblestone",	"plank",
	"shrub",	"bedrock",
	"water",	"still_water",
	"lava",		"still_lava",
	"sand",		"gravel",
	"gold_ore",	"iron_ore",
	"coal" ,	"log",
	"leaves",	"sponge",
	"glass",	"red",
	"orange",	"yellow",
	"lime",		"green",
	"teal",		"aqua",
	"cyan",		"blue",
	"indigo",	"violet",
	"magenta",	"pink",
	"black",	"gray",
	"white",	"yellow_flower",
	"red_flower",
	"brown_mushroom",
	"red_mushroom",	"gold",
	"iron",		"double_slab",
	"slab",		"bricks",
	"dynamite",	"bookshelf",
	"mossy_rocks",	"obsidian"
};
static mccc *const
mccenam_[MCCECOUNT] = {
	"Nothing",	"Greeting",
	"BlockEdit",	"PlayerState",
	"ChatMessage"
};
static mccc *const
mcsenam_[MCSECOUNT] = {
	"Nothing",	"Greeting",
	"Ping",		"NewLevel",
	"LevelData",	"LevelSize",
	"BlockChange",	"NewActor",
	"ActorState",	"ActorChange",
	"ActorSlide",	"ActorHeading",
	"ActorDied",	"ChatMessage",
	"Goodbye",	"PlayerFlags"
};

#else

#endif

const enum mccevt
mccemax[] = {
	MCCEPLYRSTAT,	MCCECHATMSG,
	MCCECHATMSG,	MCCECHATMSG,
	MCCECHATMSG,	MCCECHATMSG,
	MCCECHATMSG,	MCCECHATMSG
};

/* mccc *const
mcctname[] = {
	"EventNumber",	"Version",
	"Name",		"BlockChange",
	"ActorNumber",	"ActorState",
	"Message",	"Password",
	"ClientFlags"
}; */
const enum mcsevt
mcsemax[] = {
	MCSEEHEAD,	MCSECMSG,
	MCSEPBYE,	MCSEPBYE,
	MCSEPBYE,	MCSEPBYE,
	MCSECFLAGS,	MCSECFLAGS
};

/* mccc *const
mcstname[] = {
	"EventNumber",	"Version",
	"Name",		"DataLength",
	"DataChunk",
	"PercentComplete",
	"LevelSize",	"BlockChange",
	"ActorNumber",	"ActorState",
	"ActorSlide",	"ActorHeading",
	"Message",	"PlayerFlags"
}; */

mccc MCF
*mcbname(mcb blk) {
#ifdef MCNOSTR
	return "";
#else
	return 0 > blk || 49 < blk
	? "": mcbnam_[blk];
#endif
}
mccc MCF
*mccenam(enum mccevt ce) {
#ifdef MCNOSTR
	return "";
#else
	return MCCEINVALID >= ce
	|| MCCECOUNT <= ce
	? "": mccenam_[ce];
#endif
}
mccc MCF
*mcsenam(enum mcsevt se) {
#ifdef MCNOSTR
	return "";
#else
	return MCSEINVALID >= se
	|| MCSECOUNT <= se
	? "": mcsenam_[se];
#endif
}

MCAPI mci MCF
mcbcec(mci ec) {
	u16 ubc = (u16)ec >> 5;

	if (0 > ec) {
		ubc |= 0xF800u;
	}
	return (mci)ubc;
}

mcb
mcreset(
	struct mcx sany[], mci qty,
	mcb pv
) {
	mset(
		(void *)sany, 0u,
		(mci)sizeof *sany
	);
	while (qty) {
		--qty;
		sany[(u16)qty].curpv
		= pv;
	}
	return pv;
}
/* TODO: use semantics instead. */
mci
mcctlen(
	const struct mcx *ssrv,
	enum mcctyp ct
) {
	static const mci
	ctlen[MCCTCOUNT] = {
		 1,	 1,
		64,	 8,
		 1,	 8,
		64,	64,
		 1
	};
	mci ctl;

	if (MCPVMAX >= ssrv->curpv) {
		ctl = ctlen[ct];
		if (
			(0 == ssrv->curpv)
			&& (
				MCCEGREETING
				== ssrv->r.evt
			)
			&& (MCCTNAME == ct)
		) {
			--ctl;
		}
	}
	else {
		ctl = 0;
	}
	return ctl;
}
mci
mcstlen(
	const struct mcx *scli,
	enum mcstyp st
) {
	static const mci
	stlen[MCSTCOUNT] = {
		   1,	 1,
		  64,	 2,
		1024,	 1,
		   6,	 7,
		   1,	 8,
		   3,	 2,
		  64,	 1
	};

	return MCPVMAX < scli->curpv
	? 0: stlen[st];
}
enum mcediff
mcescmp(
	const struct mceup *dst,
	const struct mceup *src
) {
	struct mcvec dv;
	enum mcediff e;
	u16 diff;

	dv.x = dst->epos.x - src->epos.x;
	dv.y = dst->epos.y - src->epos.y;
	dv.z = dst->epos.z - src->epos.z;

	diff = d3chk((mccv *)&dv);
	if (0u != diff) {
		e = MCEDBIG;
	}
	else {
		mci di;
		mcu db;

		diff = 0u;
		di = dv.x;
		diff |= (u16)di;
		di = dv.z;
		diff |= (u16)di;
		di = dv.y;
		diff |= (u16)di;
		if (diff){
			diff = 0x2u;
		}

		db = dst->head.yaw
		^ src->head.yaw
		^ dst->head.phi
		^ src->head.phi;
		if (db) {
			diff |= 0x1u;
		}

		e = (enum mcediff)diff;
	}
	return e;
}
enum mcctyp
mcsread(
	struct mcx *ssrv, mccu *src,
	mci qty, void *dst
) {
	static treader
	*const ctrd[MCCTCOUNT] = {
		&rctEventNumber,
		&rctVersion,
		&rctName,
		&rctBlockChange,
		&rctActorNumber,
		&rctActorState,
		&rctMessage,
		&rctPassword,
		&rctClientFlags
	};
	enum mcctyp rct = MCCTEVTNO;

	ctrd[ssrv->r.type](
		ssrv, src, qty, dst
	);
	rct = (enum mcctyp)ssrv->r.type;
	/* rct = MCCTEVTNO; */
	return rct;
} /*
mci
mcswrite(
	struct mcx *ssrv, mcu *dst,
	mci qty, mccv *src
) {
	switch (ssrv->w.type) {
	case MCSTEVTNO:
		qty = wstEventNumber(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTVERSION:
		qty = wstVersion(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTNAME:
		qty = wstName(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTDATALEN:
		qty = wstDataLength(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTDATACHUNK:
		qty = wstDataChunk(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTPERCENT:
		qty = wstPercentComplete(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTLVLSIZE:
		qty = wstLevelSize(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTBLKCH:
		qty = wstBlockChange(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTACTRNO:
		qty = wstActorNumber(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTACTRSTAT:
		qty = wstActorState(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTACTRSLID:
		qty = wstActorSlide(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTACTRHEAD:
		qty = wstActorHeading(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTMSG:
		qty = wstMessage(
			ssrv, dst, qty,
			src
		);
		break;
	case MCSTPLYRFLGS:
		qty = wstPlayerFlags(
			ssrv, dst, qty,
			src
		);
		break;
	default:
		qty = MCEINVALIDOP;
		break;
	}
	return qty;
}*/

mci MCF
mcswr(
	struct mcx *ssrv, mcu *dst,
	mci qty, void *src
) {
	static ewtr
	*const sewr[MCSECOUNT] = {
		&senw,	&sepgw,
		&se0w,	&se0w,
		&seldtw,&seldmw,
		&sebuw,	&seenw,
		&seestw,&seedw,
		&seesdw,&seehw,
		&seexw,	&secmw,
		&sepbw,	&secfw
	};
	register mci rsw;
	rsw = sewr[ssrv->w.evt](
		ssrv, dst, qty, src
	);
	/* rsw = MCEINVALIDOP; */
	if (
		(
			MCSENOTHING
			!= ssrv->w.evt
		)
		&& (0 == rsw)
	) {
		rsw = MCENOTENOUGH;
	}
	return rsw;
}
