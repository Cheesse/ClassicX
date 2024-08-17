#ifndef CX_MACROS_H
#define CX_MACROS_H
#ifdef _MSC_VER
#define EXPORTED __declspec(dllexport, noinline)
#define NOINLINE __declspec(noinline)
#define PRIVATE
#else
#error Unrecognized compiler.
#endif
#endif /* CX_MACROS_H */
