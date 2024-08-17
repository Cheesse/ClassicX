#ifndef CX_TYPES_H
#define CX_TYPES_H
#if defined(_MSC_VER)
typedef unsigned __int8 byte;
typedef __int8 bool;
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
typedef struct _ip { char addr[4]; } ip_address;
//typedef unsigned int size_t;
typedef void *thread, *trigger, *module;
#ifdef _WIN32
#ifdef _UNICODE
typedef unsigned short syschar;
#else
typedef char syschar;
#endif /* _UNICODE */
#endif /* _WIN32 */
#else
#error Unrecognized compiler.
#endif /* Compiler-specific*/
#endif /* CX_TYPES_H */
