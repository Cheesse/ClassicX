/* level.h is the abstract interface to
 * an MC Classic level.
 * Functions are also declared here for
 * repackaging block arrays. */

#ifndef CX_LEVEL_H
#define CX_LEVEL_H

#define CX_LEVEL_NAME_BUFFER_SIZE 62
#define CX_LEVEL_PLAYER_ARRAY_SIZE 256
#define CXMAXLVLS 1

/* Default level format */
#ifndef CXDEFLVLFMT
#define CXDEFLVLFMT (CXBF8)
#endif

/* Level format */
enum cxlvlfmt {
	/* MC Level (.dat) */
	CXLFMCL,
	/* ClassicWorld */
	CXLFCW,
	/* ClassicX ChunkMap */
	CXLFBOT
};
/* Block format */
enum cxblkfmt {
	/* Variable-length (utf8) */
	CXBFVAR,
	/* 7-bit block */
	CXBF7,
	/* 16-bit block */
	CXBF16
};
/* Level compression */
enum cxlvlcmp {
	/* No compression */
	CXLCNONE,
	/* GZIP compression */
	CXLCGZ
};

/* Partitioning maps like this allows for
better data deduplication by avoiding
unnecessary copies and by the OS. */

/* Cube that holds 4096 blocks. */
struct chunk { mcb b[4096]; };

/* A chunk header holds a pointer to a
chunk and its properties.
This holds data that helps to avoid
unnecessary copies.
Very useful for making multiple
instances of a level. */

/* Chunk properties and pointer. */
struct chdr {
	/* Number of references - 1. */
	unsigned refs;
	/* XOR hashed chunk. */
	unsigned hash;
	struct chunk *c;
};
/* Level properties and chdr array. */
struct lvl {
	file f;
	struct chdr *chunks;
	void *data;
	cv *cdat;
	void *vartree, *entity;
	long filelen;
	u8 flags;
	u8 userflags;
	u8 _reserved[2];
	struct mcvec dim;
	i16 _reserved0;
};
/* Generator function that makes levels. */
typedef void
(*generator)(
	struct chdr *chbuffer,
	void *args
);

/* Default empty air-filled chunk. */
extern struct chdr emptych;

/* Load from file or default if 0. */
int
lload(struct lvl *l, file f);
/* Destroy a level without saving. */
int
lunload(struct lvl *l);
/* Get a block and set it if b >= 0. */
mcb
lvlblk(
	struct lvl *l, short x, short y,
	short z, mcb b
);
#endif /* CX_LEVEL_H */
