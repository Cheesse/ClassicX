#include <stddef.h>
#include <Windows.h>
#include "platform.h"
#include "task.h"

#define FILEMAPPINGDATA_BUFFER_SIZE 8
#define DEFAULT_BUFFER_SIZE 4096 /* Must be a power of 2 and a multiple of 4096. */

struct FileMappingData {
	HANDLE hFile, hFileMapping;
	LPCVOID lpcFile;
};

static int fnext = 0;
static struct FileMappingData
FileMappingDataBuffer[
	FILEMAPPINGDATA_BUFFER_SIZE
];

static CRITICAL_SECTION CriticalSection;

/* Finds the first free file slot. */
static inline void getfree(void) {
	int i;

	for (
		i = 0;
		i < FILEMAPPINGDATA_BUFFER_SIZE;
		i++
	) {
		if (
			FileMappingDataBuffer[i].hFile
		)
			continue;
		fnext = i;
		return;
	}
}

const void *fmap(
	const char *filepath,
	/*unsigned int len,*/
	size_t *size
) {
	MEMORY_BASIC_INFORMATION 
	MemoryBasicInformation;
	struct FileMappingData
	*lpFileMappingData;
	HANDLE hFile, hFileMapping;
	LPCVOID lpcFile;

	/* Calculate true string length
	if necessary. */
	//if (len == ~0) for (len = 0;
	//filepath[len]; len++);

	if (
		fnext
		== FILEMAPPINGDATA_BUFFER_SIZE
	)
		return NULL;

	/* Now try to open the file. */
	hFile = CreateFileA(
		filepath, GENERIC_READ,
		0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL
		| FILE_FLAG_OVERLAPPED
		| FILE_FLAG_SEQUENTIAL_SCAN,
		NULL
	);

	if (
		hFile
		== INVALID_HANDLE_VALUE
	)
		return NULL;
	else {
		/* Add this file to the
		list of files currently
		mapped. */
		EnterCriticalSection(
			&CriticalSection
		);
		lpFileMappingData
		= FileMappingDataBuffer
		+ fnext;
		lpFileMappingData->hFile
		= hFile;
		getfree();
		LeaveCriticalSection(
			&CriticalSection
		);

		/* Create file mapping.
		*/
		hFileMapping
		= CreateFileMapping(
			hFile, NULL,
			PAGE_READONLY,
			0, 0, NULL
		);
		lpFileMappingData->hFileMapping
		= hFileMapping;

		/* Create file view. */
		lpcFile = MapViewOfFile(
			hFileMapping,
			FILE_MAP_READ,
			0, 0, 0
		);
		lpFileMappingData->lpcFile
		= lpcFile;

		/* Get memory range
		size. */
		VirtualQuery(
			lpcFile,
			&MemoryBasicInformation,
			sizeof
			MemoryBasicInformation
		);
		*size
		= MemoryBasicInformation.RegionSize;

		return lpcFile;
	}
}

int funmap(const void *ptr) {
	struct FileMappingData
	*lpFileMappingData;
	INT i;

	/* Get the file mapping data
	structure that contains this
	base pointer. */
	for (
		i = 0;
		i != FILEMAPPINGDATA_BUFFER_SIZE;
		i++
	)
		if (
			FileMappingDataBuffer[i].lpcFile
			== ptr
		)
			break;
	lpFileMappingData
	= FileMappingDataBuffer + i;

	/* Now unmap the file view,
	delete the file mapping, and
	close the file. */
	i = i
	!= FILEMAPPINGDATA_BUFFER_SIZE
	&& UnmapViewOfFile(ptr)
	&& CloseHandle(
		lpFileMappingData->hFileMapping
	) && CloseHandle(
		lpFileMappingData->hFile
	);
	lpFileMappingData->hFile = 0;
	return i;
}

/*size_t file_decompress(FILE* fin, z_streamp stream, byte* in, byte* out, size_t inSize, size_t outSize) {
	static int err, eof;
	err = 0;
	stream->next_out = (Bytef*)out;
	stream->avail_out = outSize;
	for (;;) {
		if (!err) {
			if (!stream->avail_out) return outSize;
			else if (!stream->avail_in) {
				stream->next_in = (Bytef*)in;
				if ((stream->avail_in = fread(in, 1, inSize, fin)) < outSize) continue; // did not read anything for some reason.
			}
		} else if (err == Z_STREAM_END) return outSize - stream->avail_out;
		else {
			char error[14] = "Zlib error: ";
			error[12] = 0x30 | -err;
			error[13] = 0;
			console_print(error);
			if (stream->msg) console_print(stream->msg);
			return 0;
		} err = inflate(stream, Z_NO_FLUSH); //This will always try to fill the buffer until there is no more input
	}
}
/*
/*void inline cx::File::readText(const char* path, size_t pos, wchar_t* out, size_t size) {
	// rewrite eventually
	FILE* ifile = fopen(path, "r");
	fseek(ifile, pos, 0); // wrong way to do this
	fwscanf(ifile, L"%ls", size);
	fclose(ifile);
}
void inline cx::File::writeText(const char* path, size_t pos, const wchar_t* in) {
	// rewrite eventually
	FILE* ofile = fopen(path, "w");
	fseek(ofile, pos, 0); // wrong
	fwprintf(ofile, L"%ls", in);
	fclose(ofile);
}
int cx::File::l_readCfg(lua_State* L) {
	static FILE* ifile; static const char* path; static char name[64], value[64], str[128];
	path = lua_tostring(L, 1);
	ifile = fopen(path, "r");
	if (!ifile) {
		lua_settop(L, 0);
		lua_pushnil(L);
	} else {
		lua_newtable(L);
		for (;;) {
			fgets(str, 256, ifile); // Get one line.
			//if (str[0] = '/') continue; // This line is a comment, so skip.
			if (str[0] == '\n' || feof(ifile)) break; // No more objects.
			sscanf(str, "%20[^=\n]=%64[^\n]", name, value);
			lua_pushstring(L, name);
			lua_pushstring(L, value);
			lua_settable(L, -3);
		}
	} return 1;
}
int cx::File::l_writeCfg(lua_State* L) {
	static unsigned long offset; static FILE* ofile; static const char* names, *value; static char name[65];
	ofile = fopen(lua_tostring(L, 1), "w");
	if (!ofile) {
		lua_settop(L, 0);
		lua_pushboolean(L, FALSE);
	} else {
		names = lua_tostring(L, 3);
		offset = 0;
		for (;;) {
			memset(name, 0, 64);
			sscanf(names + offset, "%64s", name);
			if (!name[0]) break;
			offset += strlen(name) + 1;
			lua_pushstring(L, name);
			lua_gettable(L, 2);
			value = lua_tostring(L, -1);
			fprintf(ofile, "%s=%s\n", name, value);
			lua_pop(L, 1);
		} fclose(ofile);
		lua_settop(L, 0);
		lua_pushboolean(L, TRUE);
	} return 1;
}*/
