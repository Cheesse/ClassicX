/* console.c */

/*#include "system.h"
#include "bstring.h"
#include "console.h"
#include "errors.h"
#include "buffer.h"
//#include "window.h"
// TODO: Move some of this code to window class
trigger console_started;
struct Buffer out_buf;
struct Buffer in_buf;
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//REWORK WRITELINE?
bool _gui;
syschar chBuf[512];
HANDLE consoleBuf;
HANDLE STDIN;
HANDLE STDOUT;
WORD CalculateColors(color c) {

}
void console_alert(const char text[]) {
	static syschar textw[128];
	str_tosys(text, textw, 128, true);
	MessageBeep(MB_DEFMASK);
	MessageBox(NULL, textw, L"Alert", MB_OK);
}

void console_init(bool gui) {
	_gui = gui;
	buffer_new(&in_buf, 4096);
	if (_gui) ;//window_init();
	else {
		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);
		//consoleBuf = CreateConsoleScreenBuffer(NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		STDIN = GetStdHandle(STD_INPUT_HANDLE);
		STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);
		if (((size_t)STDIN | (size_t)STDOUT) == INVALID_HANDLE_VALUE) {
			// error
		}
	} console_print("&aConsole initiated. Testing!");
	//trigger_set(console_initialized);
}
void console_read(string* dest, unsigned short size) {
	DWORD read;
	ReadConsole(STDIN, chBuf, size, &read, NULL);
	str_fromsys(chBuf, dest, size > read ? read : size);
}
void console_update() {
	// Flush out buffers and stuff. (Unnecessary.)
	//window_update();
}
// Queues messages to be sent to the console and triggers the console to write a line.
void console_print(const char* src) {
	unsigned short pos = 0;
	DWORD written;
	//if (_gui) window_print(src);
	//else while (src[pos]) {
		//str_tosys(src + pos, chBuf, src->len > 512 ? 512 : src->len);
		//WriteConsoleW(STDOUT, chBuf, src->len - pos > 512 ? 512 : src->len, &written, NULL);
	//} return 1;
}
void console_setcolor(color c) { // Instantly changes the text color.
	if (_gui) ;//window_setconsolecolor(c);
	else SetConsoleTextAttribute(STDOUT, CalculateColors(c));
}
void console_stop(void) {
	//window_close();
}
#endif /* _WIN32 */
static int i;
