//#include "stdafx.h"
//#include "Heartbeat.h"
/*HINTERNET cx::Heartbeat::heartbeatSession = NULL;
HINTERNET cx::Heartbeat::heartbeatRequest = NULL;
bool cx::Heartbeat::initialized = NULL;
char cx::Heartbeat::buf[256];
DWORD cx::Heartbeat::sz = 0;
lua_State* cx::Heartbeat::_l = NULL;

/* HTTP */

/*cx::Network::HttpSession cx::Network::newHttpSession(const wchar_t* host) {
	HINTERNET h1 = InternetConnectW(internet, host, INTERNET_DEFAULT_HTTP_PORT, nullptr, nullptr, INTERNET_SERVICE_HTTP, NULL, NULL);
	unsigned int num = rand();

	return }*/
//This function has asynchronous capabilities.
/*HINTERNET cx::Network::newHttpRequest(HINTERNET session, const wchar_t* verb, const wchar_t* object, std::wstring* acceptTypes) {
	HINTERNET h2 = HttpOpenRequestW(h1, verb, object, nullptr, nullptr, nullptr, INTERNET_FLAG_NO_AUTH | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, NULL);
	return }*/

/*char cx::Network::sendHttpRequest(std::wstring* url, LPCTSTR verb, LPVOID data, LPCTSTR* acceptTypes) {
	WCHAR hostname[64];
	DWORD sz;
	std::wstring objectPath = *url;
	// Separate the hostname from the rest of the url.
	CoInternetParseUrl(url->c_str(), PARSE_DOMAIN, NULL, hostname, 64u, &sz, NULL);
	std::wstring hostname_str = hostname;
	size_t strindex = objectPath.find(hostname_str);
	objectPath.erase(strindex, hostname_str.size());
	HINTERNET h1 = InternetConnectW(internet, hostname_str.c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
	HINTERNET h2 = HttpOpenRequestW(h1, verb, objectPath.c_str(), NULL, NULL, acceptTypes, INTERNET_FLAG_NO_AUTH | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, NULL);
	HttpSendRequestW(h2, NULL, NULL, data, NULL /*size);
	return 0;
}*/

/*#ifdef _WINDOWS
unsigned char cx::Heartbeat::init() {
	if (!Network::online())	return CX_HB_NO_INTERNET;
	std::list<IP4_ADDRESS>* addresses = NULL;
	addresses = Network::DNSGetIP4(L"classicube.net"); //This will eventually be left to the user.
	if (addresses) {
		for (std::list<IP4_ADDRESS>::iterator i = addresses->begin(); i != addresses->end(); i++) {
			//This function has async capabilities.
			heartbeatSession = InternetConnectW(Network::getInternet(), Tools::IP42WString(*i).c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
			heartbeatRequest = HttpOpenRequestW(heartbeatSession, L"POST", L"/server/heartbeat/", NULL, NULL, NULL, INTERNET_FLAG_NO_AUTH | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, NULL);
			switch (sendHeartbeat(NULL)) {
			case CX_HB_FINE:
				initialized = TRUE;
				delete addresses;
				return CX_HB_FINE;
			case CX_HB_SEND_ERR:
				InternetCloseHandle(heartbeatSession);
				heartbeatSession = NULL;
				InternetCloseHandle(heartbeatRequest);
				heartbeatRequest = NULL;
				break;
			default:
				return CX_HB_ERROR;
			}
		}
	} delete addresses;
	InternetCloseHandle(heartbeatSession);
	heartbeatSession = NULL;
	InternetCloseHandle(heartbeatRequest);
	heartbeatRequest = NULL;
	return CX_HB_ERROR;
}
#endif

#ifdef _WINDOWS
unsigned char cx::Heartbeat::sendHeartbeat(DWORD* playersPtr) {
	static unsigned short bufsize; static char databuf[512];
	lua_getglobal(_l, "server_name");
	lua_getglobal(_l, "server_max");
	sprintf(databuf, "name=%s&max=%hu&port=25565&public=True&salt=96e&software=ClassicX%%20Prototype&users=%hu", lua_tostring(_l, -2), (unsigned short)lua_tointeger(_l, -1), (unsigned short)(playersPtr ? *playersPtr : 0));
	lua_pop(_l, 2);
	bufsize = (unsigned short)strlen(databuf);
	if (HttpSendRequestW(heartbeatRequest, L"Content-Type: application/x-www-form-urlencoded\r\nHost: www.classicube.net\r\n", 75u, (LPVOID)databuf, (DWORD)bufsize)) {
		if (!initialized) {
			bool loop;
			do {
				InternetReadFile(heartbeatRequest, buf, 255, &sz);
				sz == 255 ? loop = TRUE : loop = FALSE;
				if (!sz) break;
				buf[sz] = '\0';
				std::wstring* wstr = new std::wstring(std::wstring(L"Classicube URL found: ") + Tools::cstr2wstr(buf));
				Event::add(CX_EVT_MSG, wstr);
			} while (loop);
		} // else Event::add(CX_EVT_MSG, new std::wstring(L"Heartbeat sent."));
		return CX_HB_FINE;
	} else {
		Event::add(CX_EVT_HBEAT_ERR, (void*)CX_HB_SEND_ERR);
		return CX_HB_SEND_ERR;
	}
}
#endif*/
