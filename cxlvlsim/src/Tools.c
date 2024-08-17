//#include "System.h"
//#include "Types.h"
//#include "Tools.h"
/*
unsigned short colorcode_findnext(const wchar_t wstr[], unsigned short len) {

}
//Returns the color code at a position and removes it from the string.
char colorcode_get(const wchar_t* wstr) {
	
}
unsigned short wstr_to_buf(const wchar_t wstr[], unsigned short pos, char out[], unsigned short len) {
	static unsigned short i;
	for (i = 0u; i < len; i++) {
		out[i * 2u] = wstr[i + pos] & 0x00ffu;
		out[i * 2u + 1u] = (wstr[i + pos] & 0xff00u) >> 8;
	} pos += len;
	return pos;
}
unsigned short cstr_to_wstr(const char cstr[], wchar_t out[], unsigned long len) {
	static unsigned short i;
	if (!cstr) return 0;
	for (i = 0; cstr[i] && i < len; i++) out[i] = (wchar_t)cstr[i];
	return i;
}
color colorcode_to_rgb(char code){
	// All shades of gray are brighter

}
/* Prints up to 15 characters (+ null)*\/
void ip4_to_wstr(IP4_ADDRESS ip, wchar_t wstr[]) {
	swprintf(wstr, L"%hu.%hu.%hu.%hu", wstr, (unsigned short)(ip & 0x000000ff), (unsigned short)((ip & 0x0000ff00) >> 8u), (unsigned short)((ip & 0x00ff00000) >> 16u), (unsigned short)((ip & 0xff000000) >> 24u));
}
void wstr_lshift(wchar_t wstr[], unsigned short size, signed char delta) {
	static unsigned int i;
	for (i = 0; i < size; i++) {
		if (i < (unsigned int)((short)size - delta)) wstr[i] = wstr[i + delta];
		else wstr[i] = 0;
	}
}
void int_to_wstr(int i, wchar_t wstr[]) {
	swprintf(wstr, L"%d", i);
}
unsigned short wstr_to_cstr(wchar_t wstr[], unsigned short pos, char out[], unsigned short len) {
	static unsigned short i;
	for (i = 0U; i < len; i++) out[i] = (char)(wstr[i + pos] & (unsigned short)0x00ffu);
	pos += len;
	return pos;
}*/
int ea;