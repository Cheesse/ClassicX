#include <mccp.h>
#include "platform.h"
#include "level.h"
#define _ register

/* static struct chdr emptych = { ~0u, 0, 0 }; */

int lload(_ struct lvl *l, _ file f) {
	extern cu8 LDEFAULT[];
	extern ci16 LDSIZE, LDDIM[3];

	int a = -1;

	if (!f) {
		l->f = 0;
		l->flags = 0xFFu;
		l->cdat = LDEFAULT;
		l->filelen
		= (long)LDSIZE;
		l->dim.x = LDDIM[0];
		l->dim.y = LDDIM[1];
		l->dim.z = LDDIM[2];
		l->chunks = (void *)0;
		a = 0;
	}
	return a;

	/*
	if (!len) return;

	
	for (BYTE i = 0; i < 255; ++i) players[i] = NULL;
	numPlayers = 0;

	const NBT::TAG* temp_ptr = NBT::find(tag, "X");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) sizeB.x = (*(NBT::TAG_Short*)temp_ptr).payload;
	temp_ptr = NBT::find(tag, "Y");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) sizeB.y = (*(NBT::TAG_Short*)temp_ptr).payload;
	temp_ptr = NBT::find(tag, "Z");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) sizeB.z = (*(NBT::TAG_Short*)temp_ptr).payload;

	temp_ptr = NBT::find(tag, "Spawn/X");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) spawnP.x = (*(NBT::TAG_Short*)temp_ptr).payload * 32 + 16;
	temp_ptr = NBT::find(tag, "Spawn/Y");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) spawnP.y = (*(NBT::TAG_Short*)temp_ptr).payload * 32 + 51;
	temp_ptr = NBT::find(tag, "Spawn/Z");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_SHORT) spawnP.z = (*(NBT::TAG_Short*)temp_ptr).payload * 32 + 16;
	temp_ptr = NBT::find(tag, "Spawn/H");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_BYTE) spawnP.theta = (BYTE)(*(NBT::TAG_Byte*)temp_ptr).payload;
	temp_ptr = NBT::find(tag, "Spawn/P");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_BYTE) spawnP.phi = (BYTE)(*(NBT::TAG_Byte*)temp_ptr).payload;

	temp_ptr = NBT::find(tag, "Name");
	if (temp_ptr) if (*temp_ptr == BLXNBT_TAG_STRING) name = (*(NBT::TAG_String*)temp_ptr).payload;

	temp_ptr = NBT::find(tag, "BlockArray");
	if (temp_ptr) {
		if (*temp_ptr == BLXNBT_TAG_BYTE_ARRAY) {
			blockArray = (block*)(*(NBT::TAG_Byte_Array*)temp_ptr).payload.data;
			(*(NBT::TAG_Byte_Array*)temp_ptr).payload.safe = TRUE;
		}
	} tag->payload.destroy();
	delete tag; */
}

int lunload(struct lvl *l) {
	l->flags = 0u;
	return 0;
}
