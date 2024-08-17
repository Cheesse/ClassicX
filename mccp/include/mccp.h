/* MineCraft Classic Protocol Library

Implements the Minecraft Classic
Protocol and its extensions for clients
and servers as defined at
https://wiki.vg/Classic_Protocol and at
https://wiki.vg/Classic_Protocol_Extension.

Only helps with parsing and packing
packets and keeping track of the
protocol state.

This library shall be fully compatible
with the original clients and servers,
and shall behave identically.

The way packets are copied is field-by-
field.

Some fields are too long to copy at
once, so multiple calls to a function
are sometimes needed, depending on the
system.

However, only array-type fields will be
partially copied.

All other types are copied individually
if separated by arrays, or grouped into
structs otherwise. */

/* TODO: decide whether to replace
signed ints with only unsigned. */

#ifndef MCCP_H
#define MCCP_H 1

/* Check compiler and architecture. */
#define MCLITTLE 0
#define MCBIG 1

#if defined _MSC_VER

/* TODO: Maybe add annotations. */
#ifdef MCEXPORT
#define MCAPI _declspec(dllexport)
#else
#define MCAPI _declspec(dllimport)
#endif

/* Set default endianness to
little-endian. */
#define MCEND (MCLITTLE)

#if	defined _M_IX86
/* TODO: Maybe change to _stdcall? */
#define MCF _fastcall
#elif	defined _M_X64
#define MCF
#else
#error Unknown architecture.
#endif

/* TODO: This is not the compiler. */
#elif defined __MSP430__
#define MCAPI
#define MCEND (MCLITTLE)
#define MCF
#else
#error Unknown compiler.
#endif

/* Max supported block ID. */
#define MCBMAX 49

/* Max supported protocol version. */
#define MCPVMAX 7

/* Size in bytes of a string. */
#define MCSTRLEN 64

/* Size in bytes of an array. */
#define MCARRAYLEN 1024

/* Error numbers are passed as NOT. */
enum mcerr {
	MCEUNKNOWN = -127,
	MCEUNSUPPORTED,
	MCEINVALIDOP,
	MCEOUTOFRANGE,
	MCENOTENOUGH,
	MCEVMISMATCH
};
/* Result of entity comparison. */
enum mcediff {
	MCEDEQ, /* Equal */
	MCEDHEAD, /* Different heading */
	MCEDPOS, /* Position not same */
	MCEDALL, /* All different */
	MCEDBIG /* Big difference */
};
/* (Block) update mode. */
enum mcumod {
	MCUMCLEAR,	MCUMSET
};
/* Client event. */
enum mccevt {
	MCCEINVALID = -1,
	MCCENOTHING,	MCCEGREETING,
	MCCEBLKEDIT,	MCCEPLYRSTAT,
	MCCECHATMSG,
	MCCECOUNT
};
/* Client type. */
enum mcctyp {
	MCCTINVALID = -1,
	MCCTEVTNO,	MCCTVERSION,
	MCCTNAME,	MCCTBLKCH,
	MCCTACTRNO,	MCCTACTRSTAT,
	MCCTMSG,	MCCTPASS,
	MCCTCFLGS,
	MCCTCOUNT
};
/* Server event. */
enum mcsevt {
	MCSEINVALID = -1,
	MCSENOTHING,	MCSEPGREETING,
	MCSEPING,	MCSELNEW,
	MCSELDATA,	MCSELDIM,
	MCSEBUPDATE,	MCSEENEW,
	MCSEESTATE,	MCSEEDIFF,
	MCSEESLIDE,	MCSEEHEAD,
	MCSEEDIED,	MCSECMSG,
	MCSEPBYE,	MCSECFLAGS,
	MCSECOUNT
};
/* Server type. */
enum mcstyp {
	MCSTINVALID = -1,
	MCSTEVTNO,	MCSTVERSION,
	MCSTNAME,	MCSTDATALEN,
	MCSTDATACHUNK,	MCSTPERCENT,
	MCSTLVLSIZE,	MCSTBLKCH,
	MCSTACTRNO,	MCSTACTRSTAT,
	MCSTACTRSLID,	MCSTACTRHEAD,
	MCSTMSG,	MCSTPLYRFLGS,
	MCSTCOUNT
};
/*               Integral             */
/* TODO: move this to compiler section.
*/
typedef signed char
/* Clamped signed byte (~0-127). */
mcb;

typedef short
/* Signed byte difference (-128-127). */
mcd,

/* Clamped signed integer (~0-32767). */
mci;

typedef unsigned char
/* Unsigned byte (0-255). */
mcu;

/* Constant versions of each type. */

typedef const mcb
mccb;

typedef const char
mccc;

typedef const mcd
mccd;

typedef const mci
mcci;

typedef const mcu
mccu;

typedef const void
mccv;

/*               Compound             */

/* 3D vector. */
struct mcvec { mci x, y, z; };

/* Protocol greeting in. */
struct mcgti {
	/* Client/server name. */
	char *gnam;

	mcb
	/* Name length. */
	gnln,
	/* Protocol version. */
	gpv;

	mcu
	/* Client flags. */
	gcfg,
	grsv_[4];

	/* Password/message length. */
	mcb gmln;

	/* Password or message. */
	char *gmsg;
};

/* Protocol greeting out. */
struct mcgto {
	/* Client/server name. */
	mccc *gnam;

	mcb
	/* Name length. */
	gnln,
	/* Protocol version. */
	gpv;

	mcu
	/* Client flags. */
	gcfg,
	grsv_[4];

	/* Password/message length. */
	mcb gmln;

	/* Password or message. */
	mccc *gmsg;
};

/* Level chunk in. */
struct mcchi {
	/* Chunk payload. */
	mcu *cdat;
	mci
	/* Chunk payload length. */
	cdln,
	/* Chunk length. */
	clen;
	/* Percent of total data. */
	mcb cpct;
	/* Reserved. */
	mcu crsv_[3];
};

/* Level chunk out. */
struct mccho {
	/* Chunk payload. */
	mccu *cdat;
	mci
	/* Chunk payload length. */
	cdln,
	/* Chunk length. */
	clen;
	/* Percent of total data. */
	mcb cpct;
	/* Reserved. */
	mcu crsv_[3];
};

/* Block update. */
struct mcbup {
	/* Block position */
	struct mcvec bpos;
	/* Block mode and number. */
	mcb umod, bnew;
};

/* Head coordinates. */
struct mchead { mcu yaw, phi; };

/* Entity state/update. */
struct mceup {
	/* Entity number. */
	mcb eno;
	mcu estr_[1];
	/* Entity position or slide. */
	struct mcvec epos;
	/* Entity head. */
	struct mchead head;
};

/* Entity in. */
struct mceni {
	/* Entity number. */
	mcb eno;
	mcu ers0_[1];
	/* Entity state. */
	struct mcvec epos;
	/* Entity head. */
	struct mchead head;
	mcu ers1_[4];
	/* Entity name length. */
	mcb enln;
	/* Entity name. */
	char *enam;
};

/* Entity out. */
struct mceno {
	/* Entity number. */
	mcb eno;
	mcu ers0_[1];
	/* Entity state. */
	struct mcvec epos;
	/* Entity head. */
	struct mchead head;
	mcu ers1_[4];
	/* Entity name length. */
	mcb enln;
	/* Entity name. */
	mccc *enam;
};

/* Message in. */
struct mcmsi {
	char *msg;
	mcb mlen, eno;
	mcu mrsv_[6];
};

/* Message out. */
struct mcmso {
	mccc *msg;
	mcb mlen, eno;
	mcu mrsv_[6];
};

/* Do not use. */
typedef struct mcseri {
	mci rem;
	mcb evt, type;
} mcseri_;

/* Protocol session */
struct mcx {
	mcb curpv, pstate;
	mcu flg[2];
	mcseri_ r, w;
};

/* VALUES */

/* Maximum block number by version. */
MCAPI extern mccb
mcbmax[MCPVMAX + 1];

/* Block names. */
MCAPI mccc MCF
*mcbname(mcb blk);

/* Maximum client event by version. */
MCAPI extern const enum mccevt
mccemax[MCPVMAX + 1];

/* Client event names. */
MCAPI mccc MCF
*mccenam(enum mccevt ce);

/* Client type names. */
/* MCAPI extern mccc *const
mcctname[MCCTCOUNT]; */

/* Maximum server event by version. */
MCAPI extern const enum mcsevt
mcsemax[MCPVMAX + 1];

/* Server event names. */
MCAPI mccc MCF
*mcsenam(enum mcsevt se);

/* Server type names. */
/* MCAPI extern mccc *const
mcstname[MCSTCOUNT]; */

/*    COMMON FUNCTION DECLARATIONS    */

/* Block coordinate from entity one. */
MCAPI mci MCF
mcbcec(mci ec);

/* Reset at least 1 session and set the
desired protocol version.
Returns the number of the protocol
version that will be used. */
MCAPI mcb MCF
mcreset(struct mcx *sany, mci qty, mcb pv);

/* Only up to 1024 bytes are copied,
and the next type is returned. */

#if defined MCCLIENT || defined MCEXPORT

/* Read client-side protocol. */
MCAPI enum mcstyp MCF
mccread(struct mcx *scli, mccu *src,
	mci qty, void *dst);

/* Write client-side protocol. */
MCAPI mci MCF
mccwrite(struct mcx *scli, mcu *dst,
	mci qty, mccv *src);

#endif

/* Get length of a client type. */
MCAPI mci MCF
mcctlen(
	const struct mcx *ssrv,
	enum mcctyp ct
);

/* Compare 2 entity states. */
MCAPI enum mcediff MCF
mcescmp(
	const struct mceup *dst,
	const struct mceup *src
);

#if defined MCSERVER || defined MCEXPORT

/* Read server-side protocol. */
MCAPI enum mcctyp MCF
mcsread(
	struct mcx *ssrv, mccu *src,
	mci qty, void *dst
);

/* Read server-side protocol. */

/* Write server-side protocol. */
/*MCAPI mci MCF
mcswrite(
	struct mcx *ssrv, mcu *dst,
	mci qty, mccv *src
);*/
MCAPI mci MCF
mcswr(
	struct mcx *ssrv, mcu *dst,
	mci qty, void *src
);

/* Get length of a server type. */
MCAPI mci MCF
mcstlen(
	const struct mcx *scli,
	enum mcstyp st
);

/*
static int
mccpFunction mccprceEventNumber(
	mccpSession *C z,
	C mccpInputByte *C y,
	C mccpNumberOfBytes x,
	mccpClientEventNumber *C w
) {

}*/
#endif

#endif

/* For the integral typedefs, these
typedefs rely on the assumption that:
* char is 8 bits
* short is 16 bits
* long is 32 bits

There is more than 1 signed integer
representation that C supports.

From what I know there is one's
complement, two's complement, and
sign-magnitude that have actually been
implemented.

The protocol almost exclusively uses
signed integers, but rarely uses the
negative numbers.

Typically only in displacement
coordinates, rotation coordinates,
and actor numbers are
found negative numbers.

Displacement coordinates are treated as
signed 16-bit integers.

Rotation coordinates are treated as
unsigned 8-bit integers.

All coordinates are only treated when
together. Structs are copied unsigned.

Actor numbers only have 1 valid negative
value of -1, so they are treated as
signed 8-bit integers.

The non-2's complement representations
of -1 would be problematic, but the
protocol treats all negative numbers as
-1 for actor numbers.

Position coordinates can in theory be
negative, and can be with hacked
clients, but in practice they should
never be negative.

I might consider clamping such out-of-
bounds values.
*/
