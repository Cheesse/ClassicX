#ifndef COMP_H
#define COMP_H

#ifdef _MSC_VER
#define CXFUNC __fastcall

#ifdef CX_IMPORT
#define CXAPI(T) _declspec(dllimport) T CXFUNC
#else
#define CXAPI(T) _declspec(dllexport) T CXFUNC
#endif

#endif

#define LOCALFUNC(T) T CXFUNC
#endif
