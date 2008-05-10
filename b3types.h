/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#ifndef _B3_TYPES_H
	#define _B3_TYPES_H

/* It seems that we need to include stdint.h before anything else
 * We need INT64_MAX, which for most systems comes from stdint.h. However, MSVC
 * does not have stdint.h and apparently neither does MorphOS, so define
 * INT64_MAX for them ourselves.
 * Sometimes OSX headers manages to include stdint.h before this but without
 * __STDC_LIMIT_MACROS so it will be without INT64_*. We need to define those
 * too if this is the case. */
#if !defined(_MSC_VER) && !defined( __MORPHOS__) && !defined(_STDINT_H_)
	#if defined(SUNOS)
		/* SunOS/Solaris does not have stdint.h, but inttypes.h defines everything
		 * stdint.h defines and we need. */
		#include <inttypes.h>
	# else
		#define __STDC_LIMIT_MACROS
		#include <stdint.h>
	#endif
#else
	#define UINT64_MAX (18446744073709551615ULL)
	#define INT64_MAX  (9223372036854775807LL)
	#define INT64_MIN  (-INT64_MAX - 1)
	#define UINT32_MAX (4294967295U)
	#define INT32_MAX  (2147483647)
	#define INT32_MIN  (-INT32_MAX - 1)
	#define UINT16_MAX (65535U)
	#define INT16_MAX  (32767)
	#define INT16_MIN  (-INT16_MAX - 1)
#endif

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

typedef unsigned char byte;


/* This is already defined in unix, but not in QNX Neutrino (6.x)*/
#if (!defined(UNIX) && !defined(__CYGWIN__) && !defined(__BEOS__) && !defined(__MORPHOS__)) || defined(__QNXNTO__)
	typedef unsigned int uint;
#endif

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
