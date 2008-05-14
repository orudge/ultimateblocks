/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#ifndef _B3_TYPES_H
	#define _B3_TYPES_H

/* Stuff for GCC */
#if defined(__GNUC__)
	#define NORETURN __attribute__ ((noreturn))
	#define FORCEINLINE inline
	#define CDECL
	#define __int64 long long
	#define GCC_PACK __attribute__((packed))

	#if (__GNUC__ == 2)
		#undef VARARRAY_SIZE
		#define VARARRAY_SIZE 0
	#endif
#endif /* __GNUC__ */

#if defined(_MSC_VER)
	#define FORCEINLINE __forceinline
	#define inline _inline

	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
#endif

typedef unsigned char byte;


#if !defined(__BEOS__) && !defined(__NDS__) /* Already defined on BEOS and NDS */
	typedef unsigned char    uint8;
	typedef   signed char     int8;
	typedef unsigned short   uint16;
	typedef   signed short    int16;
	typedef unsigned int     uint32;
	typedef   signed int      int32;
	typedef unsigned __int64 uint64;
	typedef   signed __int64  int64;
#endif /* !__BEOS__ && !__NDS__ */

/* Compile time assertions */
#if defined(__OS2__)
	#define assert_compile(expr)
#else
	#define assert_compile(expr) void __ct_assert__(int a[1 - 2 * !(expr)])
#endif /* __OS2__ */

/* Check if the types have the bitsizes like we are using them */
assert_compile(sizeof(uint64) == 8);
assert_compile(sizeof(uint32) == 4);
assert_compile(sizeof(uint16) == 2);
assert_compile(sizeof(uint8)  == 1);

#define lengthof(x) (sizeof(x)/sizeof(x[0]))
#define endof(x) (&x[lengthof(x)])
#define lastof(x) (&x[lengthof(x) - 1])

#endif
