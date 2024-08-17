/************* MineCraft Classic Protocol Extension Server Header *************/

/* Implements a Minecraft Classic Protocol Extension server as defined at
* https://wiki.vg/Classic_Protocol and at
* https://wiki.vg/Classic_Protocol_Extension. Support will be added for the old
* Minecraft Alpha protocol. */

#ifndef MCCPES_H
#define MCCPES_H

#include "comp.h"

#define MCCPES_DEFAULT_PORT 25565
#define MCCPES_MAX_LINKS 4096
#define MCCPES_MIN_SIZE 16

#define MCCPE_FORMAT_SEQUENCE 0x0
#define MCCPE_FORMAT_CHUNKS   0x1
#define MCCPE_FORMAT_ISMAIN   0x2


/* Choose a transport layer API. Only option is TCP through UNIX sockets. */
#define TPAPI_TCP

#ifdef TPAPI_TCP

#if _WIN32
#define MCCPE_PAGESIZE 4096
#endif

#else
#error Unknown system.
#endif

/*********************************** TYPES ************************************/

/* Handler function type. */
typedef void *(BLOXFUNC *mccpeshand)(int pid, int a, const void *b);

/* ASCII strings with a maximum of 16 or 64 characters. */
typedef char *mccpess16, *mccpess64;

/* A block can be at least a byte*/
typedef unsigned char mccpe_block;

/* Context structure for each session. */
typedef struct mccpe_ctx mccpe_ctx;

/* A chunk is a 16x16x16 cube that holds 4096 blocks, which is one full page on
* most OSes. Partitioning maps like this allows for better data deduplication
* by avoiding unnecessary copies and by the OS. */
typedef struct mccpe_chunk { mccpe_block b[4096]; } mccpe_chunk;

/* CoOrinates. */
typedef struct mccpe_co {
	int x, y, z; unsigned short theta, phi;
} mccpe_co;

/* Block CHange */
typedef struct mccpe_bch {
	unsigned short x, y, z;
	union { mccpe_block block; unsigned short id; };
} mccpe_bch;

/* Identification packet structure. */
typedef struct mccpe_id {
	char name[64];
	char message[64];
	unsigned char version, usertype;
	unsigned short reserved;
} mccpe_id;

/* Message packet structure. */
typedef struct mccpe_msg { int a; char message[64]; } mccpe_msg;

/* Protocol extensions */
typedef enum mccpesext {
	MCCPE_EXT_PROTOCOL,          MCCPE_EXT_CLICK_DISTANCE,
	MCCPE_EXT_CUSTOM_BLOCKS,     MCCPE_EXT_HELD_BLOCK,
	MCCPE_EXT_EMOTE_FIX,         MCCPE_EXT_TEXT_HOT_KEY,
	MCCPE_EXT_PLAYER_LIST,       MCCPE_EXT_ENV_COLORS,
	MCCPE_EXT_SELECTION_CUBOID,  MCCPE_EXT_BLOCK_PERMISSIONS,
	MCCPE_EXT_CHANGE_MODEL,      MCCPE_EXT_ENV_MAP_APPEARANCE,
	MCCPE_EXT_ENV_WEATHER_TYPE,  MCCPE_EXT_HACK_CONTROL,
	MCCPE_EXT_MESSAGE_TYPES,     MCCPE_EXT_PLAYER_CLICK,
	MCCPE_EXT_LONGER_MESSAGES,   MCCPE_EXT_FULL_CP_437,
	MCCPE_EXT_BLOCK_DEFINITIONS, MCCPE_EXT_BLOCK_DEFINITIONS_EXT,
	MCCPE_EXT_BULK_BLOCK_UPDATE, MCCPE_EXT_TEXT_COLORS,
	MCCPE_EXT_ENV_MAP_ASPECT,    MCCPE_EXT_ENTITY_PROPERTY,
	MCCPE_EXT_ENTITY_POSITIONS,  MCCPE_EXT_TWO_WAY_PING,
	MCCPE_EXT_INVENTORY_ORDER,   MCCPE_EXT_INSTANT_MOTD,
	MCCPE_EXT_FAST_MAP,          MCCPE_EXT_EXTENDED_TEXTURES,
	MCCPE_EXT_CUSTOM_PARTICLES,  MCCPE_EXT_COUNT,
	MCCPE_EXT_MAX = MCCPE_EXT_COUNT
} mccpesext;

/* Protocol packet opcodes. */
typedef enum mccpeop {
	MCCPE_PKT_IDENTIFICATION, MCCPE_PKT_PING,
	MCCPE_PKT_LEVEL_INITIALIZE, MCCPE_PKT_LEVEL_DATA_CHUNK,
	MCCPE_PACKET_LEVEL_FINALIZE, MCCPE_PACKET_SET_BLOCK_0,
	MCCPE_PKT_SET_BLOCK_1, MCCPE_PACKET_SPAWN_PLAYER,
	MCCPE_PACKET_PLAYER_TELEPORT, MCCPE_PKT_POSITION_ORIENTATION_UPDATE,
	MCCPE_PACKET_POSITION_UPDATE, MCCPE_PACKET_ORIENTATION_UPDATE,
	MCCPE_PKT_DESPAWN_PLAYER, MCCPE_PACKET_MESSAGE,
	MCCPE_PACKET_DISCONNECT_PLAYER, MCCPE_PKT_UPDATE_USER_TYPE,
	MCCPE_PACKET_EXT_INFO, MCCPE_PACKET_EXT_ENTRY,
	MCCPE_PKT_SET_CLICK_DISTANCE, MCCPE_PACKET_CUSTOM_BLOCK_SUPPORT_LEVEL,
	MCCPE_PACKET_HOLD_THIS, MCCPE_PKT_SET_TEXT_HOT_KEY,
	MCCPE_PACKET_EXT_ADD_PLAYER_NAME, MCCPE_PACKET_EXT_ADD_ENTITY,
	MCCPE_PKT_EXT_REMOVE_PLAYER_NAME, MCCPE_PACKET_ENV_SET_COLOR,
	MCCPE_PACKET_MAKE_SELECTION, MCCPE_PKT_REMOVE_SELECTION,
	MCCPE_PACKET_SET_BLOCK_PERMISSION, MCCPE_PACKET_CHANGE_MODEL,
	MCCPE_PKT_ENV_SET_MAP_APPEARANCE, MCCPE_PACKET_ENV_SET_WEATHER_TYPE,
	MCCPE_PACKET_HACK_CONTROL, MCCPE_PKT_EXT_ADD_ENTITY_2,
	MCCPE_PACKET_PLAYER_CLICKED, MCCPE_PACKET_DEFINE_BLOCK,
	MCCPE_PKT_REMOVE_BLOCK_DEFINITION, MCCPE_PACKET_DEFINE_BLOCK_EXT,
	MCCPE_PACKET_BULK_BLOCK_UPDATE, MCCPE_PKT_SET_TEXT_COLOR,
	MCCPE_PACKET_SET_MAP_ENV_URL, MCCPE_PACKET_SET_MAP_ENV_PROPERTY,
	MCCPE_PKT_SET_ENTITY_PROPERTY, MCCPE_PACKET_TWO_WAY_PING,
	MCCPE_PACKET_SET_INVENTORY_ORDER, MCCPE_PKT_DEFINE_EFFECT = 48,
	MCCPE_PACKET_SPAWN_EFFECT, MCCPE_PKT_COUNT,
	MCCPE_PACKET_MAX = 256
} mccpe_op;

//static_assert(MCCPE_PKT_COUNT > MCCPE_PACKET_MAX);
/******************************* CONFIGURATION ********************************/

/* Sets the supported version of a specific extension. Do not call this function
* after calling mccpe_start or before calling mccpe_stop. */
MCCPE_API(int) mccpe_conf(mccpe_ctx *ctx, mccpe_ext ext, int en);

/* Creates the protocol context. */
MCCPE_API(mccpe_ctx*) mccpe_create(int mode, int ver);

/* Cleanly destroys the protocol context. */
MCCPE_API(int) mccpe_destroy(mccpe_ctx *ctx);

/* Sets up the library functions. 'mode' determines the functionality and
* 'callback' is called when a packet is received. */
MCCPE_API(int) mccpe_start(
	mccpe_ctx *ctx, int max, mccpe_h callback, const mccpe_s16 salt
);

/* Stops all functionality and cancels pending I/O operations. */
MCCPE_API(int) mccpe_stop(mccpe_ctx *ctx);

/****************************** CLASSIC PROTOCOL ******************************/

/* Joins a player to a level. The text shown in the loading screen can be
* customized. */
MCCPE_API(int) mccpe_join(
	mccpe_ctx *ctx, int pid, int lid, const mccpe_s64 title,
	const mccpe_s64 subtitle
);

/* Disconnects a player and sends them a message. */
MCCPE_API(int) mccpe_kick(mccpe_ctx *ctx, int pid, const mccpe_s64 msg);

/* Despawns an entity as seen by a player. */
MCCPE_API(int) mccpe_kill(mccpe_ctx *ctx, int pid, int eid);

/* Loads a level using a callback function. Returns the level ID. All levels are
* automatically unloaded when all players leave them, except for the main
* level. */
MCCPE_API(int) mccpe_load(
	mccpe_ctx *ctx, int fmt, mccpe_h callback, void *args
);

/* Moves a local entity to a specific location in the level. An entity ID of -1
* will move the player. */
MCCPE_API(int) mccpe_move(mccpe_ctx *ctx, int pid, int eid, const mccpe_co *co);

/* Sends a message to a player. The message ID is used only when the
* MessageTypes extension is mutually supported. */
MCCPE_API(int) mccpe_msg(mccpe_ctx *ctx, int pid, int mid, const mccpe_s64 msg);

/* Sends the Ping packet. This functionality is obsolete since the underlying
* TCP basically does the same thing. */
MCCPE_API(int) mccpe_ping(mccpe_ctx *ctx, int pid);

/* Sets the block at a certain position. */
MCCPE_API(int) mccpe_set(mccpe_ctx *ctx, int pid, const mccpe_bch *bch);

/* Spawns an entity as seen by a player. */
MCCPE_API(int) mccpe_spawn(
	mccpe_ctx *ctx, int pid, int eid, mccpe_co *co, const mccpe_s64 name
);

/* Changes the user type of a player. This is only used to give players access
* to unplaceable blocks. */
MCCPE_API(int) mccpe_type(mccpe_ctx *ctx, int pid, int type);

/********************************* BASE LEVEL *********************************/

/* Use of these functions should be avoided, unless they are being used to
* implement custom extensions. */

/* Forcibly closes the connection. Called by mccpe_kick. There should usually
* be no need to call this function. */
MCCPE_API(int) mccpe_close(mccpe_ctx *ctx, int pid);

/* Sets the packet handler for a specific packet. */
MCCPE_API(mccpe_h) mccpe_handle(mccpe_ctx *ctx, mccpe_op op, mccpe_h h);

/* Sends a raw packet. The other functions should be used instead of this
* function, except for implementing custom extensions. */
MCCPE_API(int) mccpe_send(
	mccpe_ctx *ctx, int pid, int len, const mccpe_u8 *buf
);



#endif
