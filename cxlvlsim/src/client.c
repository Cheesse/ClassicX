#define MCSERVER
#include <mccp.h>
#include "platform.h"
#include "client.h"
#include "level.h"
#include "util.h"

#define _ register
#define INBUFLEN 64
#define OUTBUFLEN 16

typedef enum mcctyp
ctreader(
	struct cli *z, cu8 *src, mci qty
);

/* Client event handlers. */
static enum mcctyp
ctdefault(struct cli *z, cu8 *src);

static ctreader
ctevtno,	ctversion,
ctname,		ctblkch,
ctname,		ctactrno,
ctactrstat,	ctmsg,
ctpass,		ctcflg;

/* Send server events.*/
static int
/* sedefault(struct cli *z),
senothing(struct cli *z), */
segreeting(struct cli *z),
seping(struct cli *z),
senewlvl(struct cli *z),
selvldata(
	struct cli *z, mci dlen,
	cv *src, mcb pcnt
),
selvlsize(
	struct cli *z, struct mcvec *dim
),
seblkch(
	struct cli *z, struct mcbup *upd
),
senewactr(
	struct cli *z, struct mceno *e
),
seactrstat(
	struct cli *z, struct mceup *e
),
seactrch(
	struct cli *z,
	struct mceup *diff
),
seactrslid(
	struct cli *z,
	struct mceup *diff
),
seactrhead(
	struct cli *z, struct mceup *ah
),
seactrdied(struct cli *z, mcb eid),
sechatmsg(
	struct cli *z, mcb eid,
	cchr *msg
),
sebye(struct cli *z, cchr *msg);

static int
/*awrite(
	struct mcs *s, u8 *dst,
	mci dlen, cu8 src[],
	mci sqty, mci tlen
),
cwrite(
	struct mcs *s, u8 *dst,
	mci len, cv *src, mci idx
),*/
initseq(struct cli *z);/*,
twrite(
	struct mcs *s, u8 *dst,
	mci dlen, cv *src, mci sqty,
	mci idx
);*/

static int ctr;

enum mcctyp
cloop(struct cli *z, enum mcctyp nxt) {
	/* TODO: figure out how to
	stack allocate this without
	breaking MCCTNAMEv0. */
	static u8 in[INBUFLEN];
	enum mcctyp ct;
	mci qty;
	/* char msg[] = "00 bytes.\n"; */

	qty = mcctlen(&z->session, nxt);
	/* TODO: move this to mccp */
	if (MCSTRLEN - 1 == qty) {
		qty = (mci)input(
			&in[1], qty
		);
		++qty;
	}
	else {
		qty = input(in, qty);
	}
	if (0 > qty) {
		ct = -1;
	}
	else {

		/*msg[0] += (char)qty / 10;
		msg[1] += (char)qty % 10;
		warn("Read ");
		warn(msg);
		warn("Type is ");
		warn(mcctname[nxt]);
		warn(".\n");*/

		switch (nxt) {
		case MCCTEVTNO:
			ct = ctevtno(
				z, in, qty
			);
			break;
		case MCCTVERSION:
			ct = ctversion(
				z, in, qty
			);
			break;
		case MCCTNAME:
			ct = ctname(z, in, qty);
			break;
		case MCCTBLKCH:
			ct = ctblkch(
				z, in, qty
			);
			break;
		case MCCTACTRNO:
			ct = ctactrno(
				z, in, qty
			);
			break;
		case MCCTACTRSTAT:
			ct = ctactrstat(
				z, in, qty
			);
			break;
		case MCCTMSG:
			ct = ctmsg(z, in, qty);
			break;
		case MCCTPASS:
			ct = ctpass(z, in, qty);
			break;
		case MCCTCFLGS:
			ct = ctcflg(z, in, qty);
			break;
		default:
			ct = ctdefault(z, in);
			break;
		}
	}
	return ct;
	/* TODO: put the replies here.*/
}
int
csetup(struct cli *z, struct lvl *l) {
	mcreset(&z->session, 1, 0);
	z->cent.enam = (char *)0;
	z->cent.enln = 0;
	z->lcur = l;
	z->flg[0] = 0u;
	ctr = 0;
	return 0;
}

int
ctest(struct cli *z) {
	return segreeting(z);
}

/* RECEIVE */

static enum mcctyp
ctdefault(struct cli *z, cu8 *src) {
	char sym[] = "00!\n";
	u8 uc[2] = { 0x30, 0x30 };
	
	z->flg[0] = 0u;
	z->cent.enam = (char *)0;
	z->cent.enln = 0;
	uc[0] |= *src >> 4;
	uc[1] |= (u8)0xFu & *src;
	sym[0] = (char)uc[0];
	sym[1] = (char)uc[1];
	warn("Unknown pktno ");
	warn(sym);
	sebye(z, "Invalid packet number!");
	mcreset(&z->session, 1, 0);

	return MCCTINVALID;
}

static enum mcctyp
ctevtno(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;
	enum mccevt evt = MCCENOTHING;

	nxt = mcsread(
		&z->session, src, qty,
		&evt
	);
	return nxt;
}
static enum mcctyp
ctversion(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;
	mcb ver = 0;

	nxt = mcsread(
		&z->session, src, qty,
		&ver
	);
	return nxt;
}
static enum mcctyp
ctname(
	struct cli *z, cu8 *src, mci qty
) {
	/* Buffer for up to 20 chars */
	static char nbuf[21];
	enum mcctyp nxt;
	mcb b;

	/* TODO: */
	nxt = mcsread(
		&z->session, src, qty,
		(void *)0
	);
	/* Trim and save name. */
	/* if (!z->name) {
		mfree(z->name);
	} */
	z->cent.enam = nbuf;
	b = strim(
		nbuf, (cchr *)src,
		(i8)sizeof nbuf - 1
	);
	z->cent.enln = (mcb)b;
	if (0 > b) {
		sebye(
			z,
			"Your name is too long!"
		);
	}
	else if (0 == b) {
		sebye(
			z,
			"You don't have a name!"
		);
	}

	/* Respond if necessary. */
	else if (MCCTEVTNO != nxt) {
		;
	}
	else if (initseq(z)) {
		nxt = MCCTINVALID;
	}
	else {

	}
	return nxt;
}
static enum mcctyp
ctblkch(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;
	struct mcbup u = { 0 };
	/* static blk b = 1; */

	nxt = mcsread(
		&z->session, src, qty,
		&u
	);

	/* Echo back the change. */
	if ((mcb)MCUMCLEAR == u.umod) {
		u.bnew = 0;
	}
	warn(z->cent.enam);
	warn(" placed ");
	warn(mcbname(u.bnew));
	warn(".\n");
	/* Change the block if placing.*/
	/* else u.blk = b++; */
	if (seblkch(z, &u)) {
		nxt = MCCTINVALID;
	}
	else if (seping(z)) {
		nxt = MCCTINVALID;
	}
	else {

	}
	return nxt;
}
static enum mcctyp
ctactrno(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;

	/* This thing is useless. */
	nxt = mcsread(
		&z->session, src, qty,
		(void *)0
	);

	return nxt;
}
static enum mcctyp
ctactrstat(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;
	struct mceup
	new = { 0 }, old = { 0 };
	enum mcediff d;

	mcopy(
		&old, &z->cent,
		(mci)sizeof old
	);
	nxt = mcsread(
		&z->session, src, qty,
		&new
	);

	/* Do something? */
	/* Send ping if no response. */
	/* Ignore if... */
	/* ...level isn't sent. */
	if (!z->flg[0]) {
		if (seping(z)) {
			nxt = MCCTINVALID;
		}
		else {
			nxt = MCCTEVTNO;
		}
	}
	/* ...player state is same. */
	else {
		d = mcescmp(&old, &new);
		if (MCEDEQ == d) {
			if (seping(z)) {
				nxt = MCCTINVALID;
			}
			else {
				nxt = MCCTEVTNO;
			}
		}
		else {
			mci coord;
			/* If position is different*/
			mcopy(
				&z->cent, &new,
				(i8)sizeof new
			);

			new.head.yaw += 128u;
			/* Version 0 no support? */
			if (0 == z->session.curpv) {
				d = MCEDBIG;
			}
			/* Send back the same state at
			opposite side. */
			new.eno = 0;
			switch (d) {
			case MCEDBIG:
				coord = (mci)((u16)16 << 5)
				- new.epos.x;
				new.epos.x = coord;

				coord = (mci)((u16)16 << 5)
				- new.epos.z;
				new.epos.z = coord;

				seactrstat(z, &new);
				break;
			case MCEDALL:
				new.epos.x = old.epos.x
				- new.epos.x;
				new.epos.y -= old.epos.y;
				new.epos.z = old.epos.z
				- new.epos.z;

				seactrch(z, &new);
				break;
			case MCEDPOS:
				new.epos.x = old.epos.x
				- new.epos.x;
				new.epos.y -= old.epos.y;
				new.epos.z = old.epos.z
				- new.epos.z;

				seactrslid(
					z, &new
				);
				break;
			case MCEDHEAD:
				seactrhead(
					z, &new
				);
				break;
			default:
				break;
			}
			if (seping(z)){
				nxt = MCCTINVALID;
			}
			else {
				nxt = MCCTEVTNO;
			}
		}
		if (ctr == 512) {
			mcb eid;
			if (seactrdied(z, 0)) {
				nxt = MCCTINVALID;
			}
			if (segreeting(z)) {
				nxt = MCCTINVALID;
			}
			eid = z->cent.eno;
			z->cent.eno = -1;
			if (senewactr(z, (void *)&z->cent)) {
				nxt = MCCTINVALID;
			}
			z->cent.eno = 0;
			if (senewactr(z, (void *)&z->cent)) {
				nxt = MCCTINVALID;
			}
			z->cent.eno = eid;
			if (seping(z)) {
				nxt = MCCTINVALID;
			}
		}
		if (ctr >= 1024) {
			if (initseq(z)) {
				nxt = MCCTINVALID;
			}
		}
	}
	++ctr;
	return nxt;
}
static enum mcctyp
ctmsg(
	struct cli *z, cu8 src[],
	mci qty
) {
	enum mcctyp nxt;
	/* TODO: this will never work on
	a 128 B RAM platform.
	Return and stack allocate above
	instead.*/
	char msg[MCSTRLEN] = "";
	mci len, max;

	nxt = mcsread(
		&z->session, src, qty,
		(void *)0
	);
	warn(z->cent.enam);
	warn(" sent a message.\n");
	if ('/' == (char)src[0]) {
		char c = (char)src[1];

		if ('p' == c) {
			/* TODO: do proper bounds checking. */
			struct mcbup u;
			mcu
			u0 = 10u * ((u8)0xFu & src[2]),
			u1 = 0xFu & src[3];

			u.bpos.x
			= mcbcec(z->cent.epos.x);
			u.bpos.y
			= mcbcec(z->cent.epos.y);
			u.bpos.z
			= mcbcec(z->cent.epos.z);

			if (u0 && u1) {
				u.bnew = (mcb)u0;
				u.bnew += (mcb)u1;
				seblkch(z, &u);
			}
			else {
				if (sechatmsg(
					z, -1,
					"Invalid usage."
				)) {
					nxt = MCCTINVALID;
				}
			}
		}
		else if ('q' == c) {
			if (sebye(
				z,
				"You quit."
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('r' == c) {
			struct mceno a = { 0 };
			mcopy(
				&a,
				&z->cent,
				(mci)sizeof a
			);
			a.eno = 0;
			a.enam = "&8You";
			a.enln = (mcb)sizeof"&8You" - 1;
			seactrdied(z, 0);
			if (senewactr(
				z, &a
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('s' == c) {
			struct mceno a = { 0 };
			a.eno = -1;
			a.epos.x = (16 << 5);
			a.epos.y = (16 << 5);
			a.epos.z = (16 << 5);
			a.head.yaw = 224;
			a.head.phi = 64;
			a.enam = z->cent.enam;
			a.enln = z->cent.enln;
			if (senewactr(z, &a)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('t' == c) {
			if (sechatmsg(
				z, -1,
				"0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('u' == c) {
			if (sechatmsg(
				z, -1,
				"~`!@#$%^*()-_+={}[]|\\:;\"'<,>.?/"
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('v' == c) {
			if (sechatmsg(
				z, -1,
				"&4g&z&&&*a& q"
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('w' == c) {
			if (sechatmsg(
				z, -1,
				"&"
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('x' == c) {
			if (sechatmsg(
				z, -1,
				"'\1'\2'\3'\4'\5'\6'\7'\10'\11'\12'\13'\14'\15'\16'\17'\20'\21'\22'\23'\24'\25'\26'\27'\30'\31'\32'\33'\34'\35'\36'\37'"
			)) {
				nxt = MCCTINVALID;
			}
		}
		else if ('y' == c) {
			struct mceup a = { 0 };
			a.eno = -1;
			a.epos.x = (16 << 5);
			a.epos.y = (16 << 5);
			a.epos.z = (16 << 5);
			a.head.yaw = 224;
			a.head.phi = 64;
			if (seactrstat(z, &a)) {
				nxt = MCCTINVALID;
			}
		}
		else {
			if (sechatmsg(
				z, -1,
				"Available commands: &7/q - quit; /r - respawn shadow;"
			)) {
				nxt = MCCTINVALID;
			}
			else if (sechatmsg(
				z, -1,
				"| &7/s - respawn; /t - print alphanumeric;"
			)) {
				nxt = MCCTINVALID;
			}
			else if (sechatmsg(
				z, -1,
				"| &7/u - print symbols except ampersand;"
			)) {
				nxt = MCCTINVALID;
			}
			else if (sechatmsg(
				z, -1,
				"| &7/v - print text with ampersands; /w - print one ampersand;"
			)) {
				nxt = MCCTINVALID;
			}
			else if (sechatmsg(
				z, -1,
				"| &7/x - print \"'\" and control characters except NUL;"
			)) {
				nxt = MCCTINVALID;
			}
			else if (sechatmsg(
				z, -1,
				"| &7/y - teleport."
			)) {
				nxt = MCCTINVALID;
			}
			else {

			}
		}
		if (seping(z)) {
			nxt = MCCTINVALID;
		}
	}
	else {
		mcb bb;
		/* Echo back the message. */
		scopy(msg, z->cent.enam);
		bb = z->cent.enln;
		len = bb;
		if (MCSTRLEN != len) {
			msg[len] = ':';
			++len;
		}
		if (MCSTRLEN != len) {
			msg[len] = ' ';
			++len;
		}
		/* qty will always be max 64 */
		max = mins(MCSTRLEN - len, qty);
		mcopy(&msg[len], src, max);
		len += max;
		if (MCSTRLEN != len) {
			msg[len] = '\0';
		}
		if (sechatmsg(z, 0, msg)) {
			nxt = MCCTINVALID;
		}
		else if (seping(z)) {
			nxt = MCCTINVALID;
		}
		else {

		}
	}
	return nxt;
}
static enum mcctyp
ctpass(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;

	nxt = mcsread(
		&z->session, src, qty,
		(void *)0
	);
	/* TODO: don't ignore pass. */

	/* Respond if necessary. */
	if (MCCTEVTNO != nxt) {
		;
	}
	else if (initseq(z)) {
		nxt = MCCTINVALID;
	}
	else {

	}
	return nxt;
}
static enum mcctyp
ctcflg(
	struct cli *z, cu8 *src, mci qty
) {
	enum mcctyp nxt;
	mcb flg = 0;

	nxt = mcsread(
		&z->session, src, qty,
		&flg
	);

	/* Respond if necessary. */
	if (MCCTEVTNO != nxt) {
		;
	}
	else if (initseq(z)) {
		nxt = MCCTINVALID;
	}
	else {

	}
	return nxt;
}
static int
segeneric(
	struct cli *z, enum mcsevt sevt,
	void *param
);

static int
segeneric(
	struct cli *z, enum mcsevt sevt,
	void *param
) {
	u8 out[OUTBUFLEN] = { 0 };
	register mci
	qty, idx = 0,
	len = (mci)sizeof out;

	qty = mcswr(
		&z->session, out,
		len, &sevt
	);
	if (
		MCSEEDIED == sevt
		&& 0 == z->session.curpv
	) {
		*out = 9u;
	}
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				param
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}

		}
		output(out, idx);
		flush();
	}
	return qty;
}

/* SEND */
static int
segreeting(_ struct cli *z) {
	static enum mcsevt
	se = MCSEPGREETING;
	static cchr
	title[] =
	"MCCv0 Server on an MSP430G2553 with 512 Bytes of RAM!",
	msg[] = "Test message!";
	u8 out[OUTBUFLEN] = { 0 };
	register mci
	qty = 0, idx = 0,
	len = (mci)sizeof out;
	struct mcgto grt;

	grt.gpv = (mcb)z->session.curpv;
	grt.gnam = title;
	grt.gnln = (mcb)slen(title);
	grt.gmsg = msg;
	grt.gmln = (mcb)slen(msg);
	grt.gcfg = 0u;

	qty = mcswr(
		&z->session, out,
		len, &se
	);
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				&grt
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}
			if (0 == len) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			if (
				(0 != grt.gnam)
				&& (0 == grt.gnln)
			) {
				grt.gnam = (cv *)0;
			}
			if (
				(0 != grt.gmsg)
				&& (0 == grt.gmln)
			) {
				grt.gmsg = (cv *)0;
			}

		}
		output(out, idx);
		flush();
	}
	return qty;
}
static INLINE int
seping(_ struct cli *z) {
	static const enum mcsevt
	se = MCSEPING;

	return segeneric(z, se, (void *)0);
}
static INLINE int
senewlvl(struct cli *z) {
	static const enum mcsevt
	se = MCSELNEW;

	return segeneric(z, se, (void *)0);
}
static int
selvldata(
	_ struct cli *z, mci dlen,
	_ cv *src, mcb pcnt
) {
	static enum mcsevt
	se = MCSELDATA;
	u8 out[OUTBUFLEN] = { 0 };
	register mci
	qty = 0, idx = 0,
	len = (mci)sizeof out;
	struct mccho chnk;

	chnk.clen = dlen;
	chnk.cdat = src;
	chnk.cdln = dlen;
	chnk.cpct = pcnt;
	qty = mcswr(
		&z->session, out,
		len, &se
	);
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				&chnk
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}
			if (0 == len) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			if (
				(0 != chnk.cdat)
				&& (0 == chnk.cdln)
			) {
				chnk.cdat = (cv *)0;
			}
		}
		output(out, idx);
		flush();
	}
	return qty;
}
static INLINE int
selvlsize(
	struct cli *z, struct mcvec *dim
) {
	static const enum mcsevt
	se = MCSELDIM;

	return segeneric(z, se, dim);
}
static INLINE int
seblkch(
	struct cli *z, struct mcbup *upd
) {
	static const enum mcsevt
	se = MCSEBUPDATE;

	return segeneric(z, se, upd);
}
static int
senewactr(
	struct cli *z, struct mceno *e
) {
	static enum mcsevt
	se = MCSEENEW;
	u8 out[OUTBUFLEN] = { 0 };
	struct mceno ent = { 0 };
	register mci
	idx = 0, qty = 0,
	len = (mci)sizeof out;

	mcopy(&ent, e, (mci)sizeof ent);
	if (0 > ent.eno) {
		ent.epos.y -= 20;
	}
	qty = mcswr(
		&z->session, out,
		len, &se
	);
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				&ent
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}
			if (0 == len) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			if (
				(0 != ent.enam)
				&& (0 == ent.enln)
			) {
				ent.enam = (cv *)0;
			}
		}
		output(out, idx);
		flush();
	}
	return qty;
}
static INLINE int
seactrstat(
	struct cli *z, struct mceup *e
) {
	static const enum mcsevt
	se = MCSEESTATE;

	return segeneric(z, se, e);
}
static INLINE int
seactrch(
	struct cli *z,
	struct mceup *diff
) {
	static const enum mcsevt
	se = MCSEEDIFF;

	return segeneric(z, se, diff);
}
static INLINE int
seactrslid(
	struct cli *z,
	struct mceup *diff
) {
	static const enum mcsevt
	se = MCSEESLIDE;

	return segeneric(z, se, diff);
}
static INLINE int
seactrhead(
	struct cli *z, struct mceup *ah
) {
	static const enum mcsevt
	se = MCSEEHEAD;

	return segeneric(z, se, ah);
}
static INLINE int
seactrdied(struct cli *z, mcb eid) {
	static const enum mcsevt
	se = MCSEEDIED;

	return segeneric(z, se, &eid);
}
static int
sechatmsg(
	struct cli *z, mcb eid,
	cchr *msg
) {
	static enum mcsevt
	se = MCSECMSG;
	u8 out[OUTBUFLEN] = { 0 };
	struct mcmso cmsg;
	register mci
	idx = 0, qty = 0,
	len = (mci)sizeof out;

	cmsg.eno = eid;
	cmsg.msg = msg;
	cmsg.mlen = (mcb)slen(msg);
	qty = mcswr(
		&z->session, out,
		len, &se
	);
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				&cmsg
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}
			if (0 == len) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			if (
				(0 != cmsg.msg)
				&& (0 == cmsg.mlen)
			) {
				cmsg.msg = (cv *)0;
			}
		}
		output(out, idx);
		flush();
	}
	return qty;
}
static int
sebye(struct cli *z, cchr *msg) {
	static enum mcsevt
	se = MCSEPBYE;
	u8 out[OUTBUFLEN] = { 0 };
	struct mcmso bmsg;
	register mci
	idx = 0, qty = 0,
	len = (mci)sizeof out;

	z->flg[0] = 0u;
	bmsg.msg = msg;
	bmsg.mlen = (mcb)slen(msg);
	qty = mcswr(
		&z->session, out,
		len, &se
	);
	if (0 < qty) {
		idx += qty;
		len -= qty;
		while (0 != qty) {
			qty = mcswr(
				&z->session,
				&out[idx],
				len,
				&bmsg
			);
			if (MCENOTENOUGH == qty) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			else if (0 > qty) {
				qty = 0;
			}
			else {
				idx += qty;
				len -= qty;
			}
			if (0 == len) {
				output(out, idx);
				idx = 0;
				len = (mci)sizeof out;
			}
			if (
				(0 != bmsg.msg)
				&& (0 == bmsg.mlen)
			) {
				bmsg.msg = (cv *)0;
			}
		}
		output(out, idx);
		flush();
	}
	return qty;
}

static int
initseq(struct cli *z) {
	const u16 cntr = (u16)16u << 5;
	struct mceno spwn = { 0 };
	struct mcvec
	*dim = &z->lcur->dim;
	char msg[] = "0.\n";
	u8 uc = 0x30;
	long i;
	i8 sc;
	mci ret = 0;

	spwn.epos.x = (mci)cntr;
	spwn.epos.y = (mci)cntr;
	spwn.epos.z = (mci)cntr;
	spwn.head.yaw = 224;
	spwn.head.phi = 64;
	z->flg[0] = 1u;
	/* Show info in stderr. */
	sc = (i8)z->session.curpv;
	uc |= (u8)sc;
	msg[0] = (char)uc;
	warn(z->cent.enam);
	warn(" joined with version ");
	warn(msg);
	/* Send server info. */
	/*if (ctr < 1024) {*/
		if (segreeting(z)) {
			ret = -1;
		}
	/*}
	else {*/
		ctr = 0;
	/*}*/

	if (senewlvl(z)) {
		ret = -1;
	}
	/* Send level data. */
	for (
		i = 0;
		i < z->lcur->filelen;
		i += MCARRAYLEN
	) {
		cv *buf;
		mci rem;

		buf = &((cu8 *)z->lcur->cdat)[i];
		rem = (mci)(z->lcur->filelen - i);
		if (selvldata(
			z,
			mins(rem, MCARRAYLEN),
			buf, 0
		)) {
			ret = -1;
		}
	}
	/* Send level size. */
	if (selvlsize(z, dim)) {
		ret = -1;
	}
	spwn.eno = -1;
	spwn.enam = "You";
	spwn.enln = (mcb)sizeof "You" - 1;
	/* Set player's spawn. */
	if (senewactr(z, &spwn)) {
		ret = -1;
	}
	/* Spawn a clone. */
	spwn.head.yaw ^= 128u;
	spwn.eno = 0;
	spwn.enam = "&8You";
	spwn.enln = (mcb)sizeof "&8You" - 1;
	if (senewactr(z, &spwn)) {
		ret = -1;
	}
	/* Kill and respawn clone. */
	/*if (seactrdied(z, 0)) {
		ret = -1;
	}/*
	/*if (senewactr(z, &spwn)) {
		ret = -1;
	}*/
	/*if (sechatmsg(z, -1, "Test.")) {
		ret = -1;
	}*/
	/* Ping to mark the end. */
	if (seping(z)) {
		ret = -1;
	}
	return ret;
}
