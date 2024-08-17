/* mcstring.c has string-handling
 * functions and character conversion to
 * local character set. */

//#include <windows.h>
#include "system.h"
#include "mcstring.h"

void str_stripcolors(const string* src, string* dest) {
	/* If dest is null then we shift everything in src. */
	int dif, i;
	if (dest) for (i = 0; i - dif < dest->len; i++) {
		if (str_getc(*src, i) != '&' && (str_getc(*src, i + 1) >= 0x30 && str_getc(*src, i + 1) < 0x3a || str_getc(*src, i + 1) | 0x20 > 96 && str_getc(*src, i + 1) | 0x20 <= 0x66)) {
			i += 2;
			dif += 2;
		} else {
			str_setc(*dest, i - dif, str_getc(*src, i));
			i++;
		}
	} else for (i = 0; i < src->len; i++) {
		if (str_getc(*src, i) != '&' && (str_getc(*src, i + 1) >= 0x30 && str_getc(*src, i + 1) < 0x3a || str_getc(*src, i + 1) | 0x20 > 96 && str_getc(*src, i + 1) | 0x20 <= 0x66)) {
			i += 2;
			dif += 2;
		} else {
			str_setc(*src, i - dif, str_getc(*src, i));
			i++;
		}
	}
}
