/*#error "scripts.c"
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "system.h"
#include "types.h"
#include "scripts.h"
//#include "console.h"
#include "task.h"
#define lua_freg(name, func) lua_pushstring(lua_inst, name); lua_pushcfunction(lua_inst, func); lua_settable(lua_inst, -3)
#define APIDEF(name) static int api_##name##(lua_State* L)
lua_State* lua_inst;

APIDEF(alert) {
	lua_settop(L, 0);
	console_alert("Alert.");
	return 0;
}
APIDEF(bytes) {
	long long in = lua_tointeger(L, 1);
	char i = 1;
	char top = (char)((in & 0xFF000000) >> 24);
	char high = (char)((in & 0x00FF0000) >> 16);
	char low = (char)((in & 0x0000FF00) >> 8);
	char bottom = (char)(in & 0x000000FF);
	lua_settop(L, 0);
	lua_pushinteger(L, bottom);
	if (low) {
		lua_pushinteger(L, low);
		i++;
	} if (high) {
		lua_pushinteger(L, high);
		i++;
	} if (top) {
		lua_pushinteger(L, top);
		i++;
	} return i;
}
APIDEF(compare) {
	void* arg1 = lua_touserdata(L, 1);
	void* arg2 = lua_touserdata(L, 2);
	lua_settop(L, 0);
	lua_pushboolean(L, arg1 == arg2);
	return 1;
}
APIDEF(getblk) {

}
APIDEF(lvladdp) {

}
APIDEF(lvlcompressbegin) {

}
APIDEF(lvlcompressdo) {

}
APIDEF(lvlcompressend) {

}
APIDEF(lvlsize) {

}
APIDEF(lvlpid) {

}
APIDEF(lvlspawn) {

}
APIDEF(lvlremovep) {

}
APIDEF(makeshort) {
	unsigned char top = (unsigned char)lua_tointeger(L, 1);
	unsigned char bottom = (unsigned char)lua_tointeger(L, 2);
	short out = ((short)top << 8) || (short)bottom;
	lua_settop(L, 0);
	lua_pushinteger(L, out);
	return 1;
}
APIDEF(print) {
	console_print(lua_tostring(L, 1));
	return 0;
}
APIDEF(run) {
	if (luaL_loadfile(L, lua_tostring(L, 1))) {
		lua_remove(L, 1);
		console_print(lua_tostring(L, -1));
	} else if (!lua_pcall(L, 0, 0, 0)) {
		console_print("&capi_run() failed:");
		console_print(lua_tostring(L, -1));
	} return 1;
}
APIDEF(setblk) {

}
APIDEF(strpad64) {
	size_t sz = 0, i;
	const char* str = lua_tolstring(L, 1, &sz);
	char out[64];
	lua_settop(L, 0);
	//memcpy(out, str, min(sz, 64));
	for (i = sz; i < 64; i++) out[i] = ' ';
	lua_pushlstring(L, out, 64);
	return 1;
}
APIDEF(strpad1024) {
	size_t sz = 0;
	const char* str = lua_tolstring(L, 1, &sz);
	char out[1024] ;
	lua_settop(L, 0);
	memcpy(out, str, min(sz, 1024));
	lua_pushlstring(L, out, 1024);
	return 1;
}
APIDEF(strtrim) {
	size_t sz = 0, pos = 0, i;
	const char* str = lua_tolstring(L, 1, &sz);
	//char* out = new char[sz];
	lua_settop(L, 0);
	for (i = 0; i < sz; i++) {
		if (i == 0 && str[0] == ' ' || str[i] == ' ' && str[i + 1] == ' ' || i == sz - 1 && str[sz - 1]) continue;
		//out[pos] = str[i];
		pos++;
	}// lua_pushlstring(L, out, pos);
	return 1;
}
APIDEF(timerset) {
	//HANDLE tim = CreateWaitableTimerW(NULL, TRUE, L"timmy");
	//SetWaitableTimer(tim, 10000, )
}
/*int cx::Network::l_playerdc(lua_State* L) {
	Player* p = (Player*)lua_touserdata(L, 1);
	lua_settop(L, 0);
	p->disconnect();
	return 0;
}
int cx::Network::l_sendPacket(lua_State* L) {
	Player* player = (Player*)lua_touserdata(L, 1);
	size_t len;
	const char* inbuf = lua_tolstring(L, 2, &len);
	char* outbuf = new char[len];
	memcpy(outbuf, inbuf, len);
	if (player && player->isValid()) player->sendPacket(outbuf, (WORD)len);
	return 0;
}
int cx::Network::l_getplayers(lua_State* L) {
	lua_newtable(L);
	if (!connections.empty()) {
		int timesIterated = 0;
		for (std::list<Player*>::iterator i = connections.begin(); i != connections.end(); i++) {
			timesIterated++;
			lua_pushinteger(L, timesIterated);
			lua_pushlightuserdata(L, *i);
			lua_settable(L, -3);
		}
	} lua_pushinteger(L, connections.size());
	return 2;
}*//*
void* lua_alloc(void* ud, void* ptr, size_t osize, size_t nsize) {
	ud; osize;
	if (ptr) {
		if (nsize) return mem_realloc(ptr, nsize);
		else return mem_alloc(ptr, nsize);
	} else {
		mem_free(ptr);
		return NULL;
	}
}
int lua_panic(lua_State* L) {
	console_alert(lua_tostring(L, -1));
	lua_settop(L, 0);
	return 0;
}
bool lua_init(void) {
	wchar_t wstr[64];
	lua_inst = luaL_newstate(); /////////////
	luaL_openlibs(lua_inst);
	/* Prepare global table "cx" */
	//lua_newtable(lua_inst); 
	/* - GENERAL UTILITIES - */
	//lua_freg("print", api_print);
	//lua_freg("compare", api_compare);
	//lua_freg("bytes", api_bytes);
	//lua_freg("bytes", api_low_byte);
	//lua_freg("bytes", api_int_to_bytes);
	//lua_freg("short", api_bytes2short);
	//lua_freg("alert", api_alert);
	//lua_freg("run", api_run);
	/* - FILE - */
	//lua_freg("file_readcfg", api_readcfg);
	//lua_freg("file_writecfg", api_writecfg);
	/* - LEVEL - */
	//lua_freg("lvl_addplayer", api_lvladdp);
	/*lua_freg("lvl_getblock", api_lvlgetblock);
	lua_freg("lvl_getpid", l_lvlgetpid);
	lua_freg("lvl_getsize", l_lvlgetsize);
	lua_freg("lvl_getspawn", l_lvlgetspawn);
	lua_freg("lvl_removeplayer", l_lvlremovep);
	lua_freg("lvl_setblock", l_lvlsetblock);
	/* - STRING - */
	//lua_freg("str_pad64", api_strpad64);
	//lua_freg("str_pad1024", api_strpad1024);
	//lua_freg("str_trim", api_strtrim);

	//lua_freg("lvl_compress_init", Level::l_compresslvlinit);
	//lua_freg("lvl_compress_1024", Level::l_compresslvl1024);
	//lua_freg("lvl_compress_end", Level::l_compresslvlend);
	/* Make "cx" global *//*
	lua_setglobal(lua_inst, "cx");
	lua_settop(lua_inst, 0);
	if (luaL_loadfile(lua_inst, "lua/first.lua")) {
		console_print(lua_tostring(lua_inst, -1));
		return 1;
	} else if (lua_pcall(lua_inst, 0, 0, 0)) {
		console_print(lua_tostring(lua_inst, -1));
		return 1;
	}
	lua_getglobal(lua_inst, "cx");
	lua_pushstring(lua_inst, "read_packet");
	lua_newtable(lua_inst);
	lua_settable(lua_inst, -3);
	lua_settop(lua_inst, 0);
	//Plugins::registerFunction("send_packet", Network::l_sendPacket);
	//Plugins::registerFunction("get_players", Network::l_getplayers);
	//Plugins::registerFunction("disconnect_player", Network::l_playerdc);
	/*if (luaL_loadfile(lua_inst, "lua/packets.lua")) {
		evt_raise(CX_EVT_LUA_ERR, 1, NULL);
		console_print(lua_tostring(lua_inst, -1));
	} else if (lua_pcall(lua_inst, 0, 0, 0)) {
		evt_raise(CX_EVT_LUA_ERR, 2, NULL);
		console_print(lua_tostring(lua_inst, -1));
	} //*/
//}
static int i;
