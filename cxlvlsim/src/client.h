#ifndef CX_CLIENT_H
#define CX_CLIENT_H
#define MAXCLIENTS 64

/* Client */
struct cli {
	struct mcx session;
	struct mceni cent;
	struct lvl *lcur;
	u8 flg[8];
};

enum mcctyp
cloop(struct cli *z, enum mcctyp nxt);

int
csetup(struct cli *z, struct lvl *l);

/* For testing purposes only. */
int
ctest(struct cli *z);

#endif
