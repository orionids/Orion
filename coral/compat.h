/* Coral library for platform compatibility
 Copyright (C) 1996-2018 AdaptiveFlow
 ( adaptiveflow@gmail.com )

 All files contained in Coral library are dual-licensed:

 GNU General Public License version 3 as published by
the Free Software Foundation. For the terms of this 
license, see <http://www.gnu.org/licenses/>.

 Alternatively, you can use these files under your own
license if you have written permission of the copyright
holder or corresponding equivalent conditions.
Refer to <http://www.orionids.org/projects/license.txt> */

/* Definitions to maintain compatibility

 < Platform Specific Macros >
 __VENDOR__       , vendor specific platform include file
	VENDOR_INITIALIZE, do initialization
 __UNIX__         , unix
 __BEOS__         , beos - untested yet
 __OS2__          , os2 - untested yet
 __MAC__          , mac
 __MACOS__        , modern macos
 __OLDMAC__       , old mac
 __ANDROID__      , android
 __PALMOS__       , palmos
 __SYMBIAN__      , symbian
 __WINOS__        , windows ( because __WINDOWS__ is used some windows.h to prevent duplicated inclusion, so it is replaced to this )
 __WIN32__        , 32 bit windows
 __WIN16__        , 16 bit windows
 __WINCE__        , Windows CE
 __DOS__          , dos
 __DOS16__        , 16 bit dos
 __DOS32__        , 32 bit dos
 __16__           , 16 bit architecture
 __ARCH__ , default pointer size of a platform
 __INTEGERX__     , compiler supports extended sized integer
 __INTOPR__       , define integer operators for C++
 __UNIXLIKE__     , OS which follows unix rules
 __UNIXBASED__    , Emulator running on UNIX based OS
 __UNIXC__        , a c compiler which follows unix rules
 __JAVA__         , applications are java based
 __WCS__          , wide character encoding is used
 __UTF8__         , utf8 character encoding is used
 __EMBEDDED__     , an embedded/mobile platform
 __MINIMAL__      , minimal code to run orion
 __NOEVLOOP__     , no event loop
 __NOWCS__        , no wcs support functions
 __NOFS__         , no file system
 __NOPROC__       , no process
 __NOSOCK__       , no socket API
 __NOINQ__        , input queue is not needed
 __NOOUTQ__       , output queue is not needed
 __NOGR__         , no graphics
 __NOAU__         , no audio
 __EMULATOR__     , target is emulator platform
 __EMULATOR_SUPPORTED__ , a platform supports emulator
 __GLOBAL_UNSUPPORTED__ , a platform which doesn't have data segment

 __BIG_ENDIAN__, for big endian machines
 __LITTLE_ENDIAN__, for little endian machines
 __PDP_ENDIAN__, for pdp endian machines

  Some other platform specific macro names contained in this source
 can be variable. */



#ifndef __CORAL_COMPAT_H
#	define __CORAL_COMPAT_H



/*------------------------------------------------------------------------*
	Some definitions needed for this header
 *------------------------------------------------------------------------*/



typedef unsigned short _wchar;
typedef int _bool;



/* Integer value limits */

#ifdef _MSC_VER
#	define C_1L  1i64
#	define C_1UL 1ui64 
#else
#	define C_1L 1LL
#	define C_1UL 1ULL
#endif

#define OP_MAXBITINT_EXT(_1,cnt)  (-(OP_MINBITINT_EXT(_1,cnt)+1))
#define OP_MINBITINT_EXT(_1,cnt)  ((-(_1))<<((cnt)-1))
#define OP_MAXBITINT(cnt)  OP_MAXBITINT_EXT(C_1L,cnt)
#define OP_MINBITINT(cnt)  OP_MINBITINT_EXT(C_1L,cnt)

/* to avoid 'left shift count >= width of type' warning in gcc, shift cnt-1
	and multiply 2 */
#define OP_MAXBITUINT_EXT(_1u,cnt) ((((_1u)<<(cnt-1))<<1)-1)
#define OP_MAXBITUINT(cnt) OP_MAXBITUINT_EXT(C_1UL,cnt)



/*-------------------------------------------------------------------------*
	Platform Specific Macros
 *-------------------------------------------------------------------------*/


#ifdef __VENDOR__
#	define VENDOR_INITIALIZE
#	include __VENDOR__ /* undef __VENDOR__ if there is no corresponding platform */
#endif

#ifdef ANDROID

/* because android can run on any operating system kernel theoretically,
 detect it first */

#	ifndef __ANDROID__
#		define __ANDROID__
#	endif

#	ifndef __EMULATOR_SUPPORTED__
#		define __JAVA__
#		define __EMBEDDED__
#		define __UTF8__

#		if ANDROID_API_LEVEL <= 8
#			define DL_UNUSABLE_IN_CONSTRUCTOR /* dlopen cannot be used in gcc constructor */
#		endif
#	endif /* __EMULATOR_SUPPORTED__ */

#else /* !ANDROID */


/* MACOS */

#ifdef __APPLE__

/* check apple before evaluating unix */

#	define __UNIX__
#	ifndef __EMULATOR_SUPPORTED__
#		define __MACOS__
#		define __MAC__
#		define __UTF8__
#		include <TargetConditionals.h>
#		if TARGET_OS_IPHONE
#			define __IOS__
#		endif
#	endif

#else /* !__APPLE__ */
#ifdef macintosh

#	ifndef __EMULATOR_SUPPORTED__
#		define __OLDMAC__
#		define __MAC__
#	endif
/* End of MACOS */

#else /* !macintosh */

/* UNIX */

#if ( defined(__unix) || defined(__unix__) ) && \
	!defined(DJGPP) && !defined(__CYGWIN__)

#	ifndef __UNIX__
#		define __UNIX__
#	endif

#	define FLAG_PATH_NAME 0

/* End of UNIX */



#else /* !__UNIX__ */

#	ifndef __EMULATOR_SUPPORTED__


/* PalmOS */

#ifdef __palmos__

#	define __PALMOS__
#	define __EMBEDDED__

/* End of PalmOS */



#else /* !__palmos__ */



#ifdef _DEBUG
#	ifndef __DEBUG__
#		define __DEBUG__
#	endif
#endif



/* Symbian */

#ifdef __SYMBIAN32__

#	ifndef __SYMBIAN__
#		define __SYMBIAN__
#	endif

#	define __WCS__

#	ifndef __EXPORT__
#		define __EXPORT__
#	endif
#	define EXPORT_KEYWORD __declspec(dllexport)
#	define EXPORT_BEFORE_RETURN

#	define __EMBEDDED__

#	ifndef __SERIES60_3X__ /* it must be less than this : consider future release */
#		define __SYMBIAN_DLLAPP__
#		ifndef __GLOBAL_UNSUPPORTED__
#			define __GLOBAL_UNSUPPORTED__
#		endif
#	endif

#else /* !__SYMBIAN32__ */

/* WINDOWS */

#if defined(_Windows) && !defined(__WINOS__)
#	define __WINOS__
#endif /* _Windows */


#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__)
#	ifndef __WINOS__
#		define __WINOS__
#	endif
  /* borland c automatically defines __WIN32__ */
#	ifndef __WIN32__
#		define __WIN32__
#	endif
#	define NULL_IS_ZERO
#endif /* WIN32 || _WIN32 */



#ifdef __WINOS__

#	if defined(__STRICT__) && !defined(STRICT)
#		define STRICT
#	endif

/* @cmt0001 */
#	if defined (__EXPORT__)
#		if defined(__TURBOC__)
#			define EXPORT_KEYWORD __export
#			define EXPORT_AFTER_RETURN
#		else
#			define EXPORT_KEYWORD __declspec(dllexport)
#			define EXPORT_BEFORE_RETURN
#		endif
#	endif /* __EXPORT__ */

#	ifdef __WIN32__

		/* WINAPI is standard calling convention */
#		ifdef __GNUC__
#			define __API __attribute__((stdcall))
			/* maybe it was a bug in old version ( the exact version is
				unclear ) cygwin gcc that __attribute__((stdcall)) cannot be
				used for the function pointer casting */
#			if __GNUC__<0 && __GNUC_MINOR__<0 && __GNUC_PATCHLEVEL__<0
#				define __APICAST
#			endif

#		else
#			define __API _stdcall
#		endif /* __GNUC__ */

#	else /* !__WIN32__ */

#		define __16__
#		define __WIN16__
#		define __API __FAR pascal

#	endif /* __WIN32__ */

#ifdef UNDER_CE /* _WINCE */
#	ifndef __WINCE__
#		define __WINCE__ /* typically, __WIN32__ is also defined for wince */
#	endif /* __WINCE__ */
#	define __WCS__
#	define __EMBEDDED__
#endif /* _WINCE */


/* End of WINDOWS */



#else /* !__WINOS__ */



/* DOS */

#if defined(MSDOS) || defined(__MSDOS__) || defined(__FLAT__)

#	ifndef __DOS__
#		define __DOS__
#	endif

#	define THREAD_UNSUPPORTED

/* detect dos16 or dos32 */
#	ifdef __FLAT__
#		define __DOS32__
#	else /* !__FLAT__ */
#	ifdef _MSC_VER
#		if !defined(M_I86TM) && !defined(M_I86SM) && !defined(M_I86CM) && \
		   !defined(M_I86MM) && !defined(M_I86LM) && !defined(M_I86HM)
#			define __DOS32__
#		endif
#	else /* !_MSC_VER */

#		if !defined(__TINY__) && !defined(__SMALL__) && !defined(__MEDIUM__) && \
		   !defined(__LARGE__) && !defined(__HUGE__) && !defined(__COMPACT__)
#			define __DOS32__
#		endif

#	endif /* _MSC_VER */
#	endif /* __FLAT__ */

#	ifndef __DOS32__
#		define __16__
#		define __DOS16__
#	endif

#	define __API __FAR pascal

/* End of DOS */



/* Other platforms */

#else /* !( defined(MSDOS) || defined(__MSDOS__) || defined(__FLAT__) ) */

#	define __MINIMAL__
#	define __UTF8__
#	define __NOWCS__
#	define __NOFS__
#	define __NOPROC__
#	ifndef __NOSOCK__
#		define __NOSOCK__
#	endif
#	define __EMBEDDED__
#	define THREAD_UNSUPPORTED
#	define DIO_COMPACT_HANDLE
#	undef _MALLOC_OVERRIDE

#endif /* defined(MSDOS) || defined(__MSDOS__) || defined(__FLAT__) */

/* End of other platforms */



#endif /* __WINOS__ */

#endif /* !__SYMBIAN32__ */

#endif /* __palmos__ */

#endif /* __EMULATOR_SUPPORTED__ */

#endif /* ( __unix || __unix__ ) && !DJGPP && !__CYGWIN__ */

#endif /* MAC */

#endif /* __APPLE__ */

#endif /* ANDROID */

#if defined(__UNIX__) || defined(__ANDROID__)
#	ifdef __EMULATOR_SUPPORTED__
#		undef __UNIX__
#		undef __ANDROID__
#		define __UNIXBASED__
#	else
#		define __UNIXLIKE__
#		define __UNIXBASED__
#	endif
#endif

#ifdef __VENDOR__
#	undef VENDOR_INITIALIZE
#	include __VENDOR__
#	undef __VENDOR__
#endif


#if !defined(__WIN16__) && !defined(__DOS__)
#	define __VAS__ /* vitual address space */
#endif



/* to write 'internal' interface for L77, define L77_INTERFACE previously
   : 32 bit Borland C/C++ is needed to support Lahey F77 */
#if defined(L77_INTERFACE) || defined(__L77__)

#	if defined(__TURBOC__) && defined(__WIN32__)

 /* override platform specifiers */
#		undef __WINOS__
#		undef __WIN32__
#		undef __EXPORT__
#		undef __API
#		define __DOS__
#		define __DOS32__
#		define __API
#		ifdef L77_INTERFACE
#			define API_NAME(name) _##name
#		endif

#	else /* !__TURBOC__ || !__WIN32__ */

... /* error */

#	endif /* __TURBOC__ && __WIN32__ */

#else /* !L77_INTERFACE && !__L77__ */

#ifdef _EiC
#	undef __EXPORT__
#	undef __API
#endif

#endif /* L77_INTERFACE || __L77__ */



/* byte order definitions */
#if defined(mc68000) || defined(__mc68000__) || defined (__sparc__) || defined (__PPC__) \
 || (defined(__mips__) && (defined(MIPSEB) || defined (__MIPSEB__)))
#	ifndef __BIG_ENDIAN__
#		define __BIG_ENDIAN__
#	endif
#	define __ENDIAN__ 4321
#else /* !big endian platforms */
#if defined(PDP_ENDIAN_PLATFORM)
#	ifndef __PDP_ENDIAN__
#		define __PDP_ENDIAN__
#	endif
#	define __ENDIAN__ 3412
#else /* !pdp endian platforms */
#	ifndef __LITTLE_ENDIAN__
#		define __LITTLE_ENDIAN__
#	endif
#	define __ENDIAN__ 1234
#endif /* pdp endian platforms */
#endif /* big endian platforms */



#ifdef __16__ /* __16__ is the special symbol only for 16 bit platform */

#	define __FAR far
#	define __HUGE huge
	typedef long _integer32;
	typedef unsigned long _unsigned32;
	typedef long _min32;
	typedef unsigned long _umin32;
#	define __ARCH__ 16

#else /* !__16__ */

	typedef int _min32;
	typedef unsigned _umin32;

#	define __FAR
#	define __HUGE

#	ifdef __TURBOC__

#		define __ARCH__ (sizeof(long)*8) /* borland c allows to use sizeof in preprocessor */

#	else /* !__TURBOC__ */

#		include <limits.h>
#		ifdef SIZE_MAX
#			define _ARCH_MAX SIZE_MAX /* 2^64 for win64 */
#		else
#			define _ARCH_MAX ULONG_MAX
#		endif
#		if _ARCH_MAX == OP_MAXBITUINT(32)
#			define __ARCH__ 32
			typedef long _integer32;
			typedef unsigned long _unsigned32;
#		else
#		if _ARCH_MAX == OP_MAXBITUINT(64) /* this passes for win64, so its preprocessor allows 64 bit shift for long consts */
#			define __ARCH__ 64
			typedef int _integer32;
			typedef unsigned int _unsigned32;
#			ifdef _WIN64
#			undef C_1L
#			undef C_1UL
#			define C_1L 1LL
#			define C_1UL 1ULL
			typedef __int64 _integer64;
			typedef unsigned __int64 _unsigned64;
#			else
			typedef long _integer64;
			typedef unsigned long _unsigned64;
#			endif
#			define __INTEGER64__
#		else
			... /* #error Unsupported architecture. */
#		endif
#		endif

#	endif /* __TURBOC__ */

#endif /* __16__ */

#ifdef __arm__
#ifndef __arm
#	define __arm
#endif
#endif


#if !defined(__SYMBIAN__) || !defined(__SYMBIAN_DLLAPP__)
#if defined(__GNUC__) || defined(__arm)
	typedef long long _builtin_integerx;
	typedef unsigned long long _builtin_unsignedx;
#	define __BUILTIN_INTEGERX__

#else /* !__GNUC__ */
#ifdef _MSC_VER
#	if __ARCH__>=32
		typedef __int64 _builtin_integerx;
		typedef unsigned __int64 _builtin_unsignedx;
#		define __BUILTIN_INTEGERX__
#	endif
#endif /* _MSC_VER */
#endif /* __GNUC__ */
#endif /* !__SYMBIAN__ */



#ifndef __INTEGERX_UNSUPPORTED__
#	ifdef __BUILTIN_INTEGERX__
		typedef _builtin_integerx __integerx;
		typedef _builtin_unsignedx __unsignedx;
#		define __INTEGERX__
#	endif
#endif /* __INTEGERX_UNSUPPORTED__ */



#ifdef __INTEGERX__
#	ifndef __INTEGER64__
	typedef __integerx _integer64;
	typedef __unsignedx _unsigned64;
#	define __INTEGER64__
#	endif
#else

	typedef long _halfx;
	typedef unsigned long _uhalfx;
#	if __ARCH__<=32
	typedef short _quarterx;
	typedef unsigned short _uquarterx;
#	else
	typedef int _quarterx;
	typedef unsigned int _uquarterx;
#	endif /* __ARCH__ */

	typedef struct __integerx {
		_uhalfx part[2];
	} __integerx;

#	define __unsignedx __integerx

#		if defined(__cplusplus) && defined(__INTOPR__)

#			ifdef coral_integer_hpp
#				include coral_integer_hpp
#			else
#			ifdef CORAL_PATH
#				include CORAL_PATH/integer.hpp>
#			else
#				include <coral/integer.hpp>
#			endif
#			endif

#			define _integer64 _integerx
#			define _unsigned64 _unsignedx
#		else
#			undef __INTOPR__
#			define _integer64 __integerx
#			define _unsigned64 __integerx
#		endif
	/* don't define __INTEGER64__ here so coral/begin/integer.h and
	  coral/end/integer.h work correctly */

#endif /* !__INTEGERX__ */

#	define __integer64 __integerx
#	define __unsigned64 __unsignedx


#ifdef __GNUC__
#	if __GNUC__>=3
#		define __RESTRICT __restrict__
#	else
#		define __RESTRICT
#	endif
#		define BEGIN_MACRO_BLOCK ({
#		define END_MACRO_BLOCK })

#	define __likely(expr) __builtin_expect(expr,1) /* so true means 1, not to use '!!' though it's formal */
#	define __unlikely(expr) __builtin_expect(expr,0)
#else
#	define __RESTRICT
#	define __likely(expr) expr
#	define __unlikely(expr) expr
#	ifdef DO_WHILE_ZERO
#		define BEGIN_MACRO_BLOCK do {
#		define END_MACRO_BLOCK } while ( 0 )
#		ifdef _MSC_VER
#			pragma warning(disable:4127) /* conditional expression is constant */
#		endif
#	else
#		define BEGIN_MACRO_BLOCK {
#		define END_MACRO_BLOCK }
#	endif
#	ifdef _MSC_VER
		/* I don't know the exact vc release which supports security enhancement
		functions but I assume it is vs2003.net */
#		if _MSC_VER>=1310
#			define _MSC_SECURITY_ENHANCEMENT
#		else
#			define __FUNCTION__ __FILE__ /* no way ! */
#		endif
#		define _MSC_WARN_UNREFERENCED 4100

//#		pragma warning(disable:4514) /* supress unreferenced inline function has been removed */
#		ifndef __cplusplus
#			define inline __inline
#			define __func__ __FUNCTION__
#		endif

#	endif /* _MSC_VER */
#endif /* __GNUC__ */



/*------------------------------------------------------------------------*
   Definitions for functions
 *------------------------------------------------------------------------*/


/* __UNIXC__ for unix-like c compilers */
#if defined(__UNIXBASED__) || defined(__GNUC__) || defined(__SYMBIAN__)
#	define __UNIXC__
#else
#	if !defined(__arm) && !defined(__func__)
#		define __func__ "__func__"
#	endif
#endif



#ifdef __cplusplus

#	define P_D(value) = value
#	ifdef __CLASSICPARAM__
#		undef __CLASSICPARAM__
#	endif
#	ifndef __MODERNPARAM__
#		define __MODERNPARAM__
#	endif

#else /* !__cplusplus */

#	define P_D(value)

#	if !defined(__STDC__) && defined(__UNIX__) && defined(__UNIXC__)

	/* some symbols K&R C doesn't recognize */
#		define signed
#		define const

	/* classic parameter is default if modern parameter was not forced */
#		ifndef __MODERNPARAM__
#			define __CLASSICPARAM__
#		endif

#		define __KNR__

#	else /* defined(__STDC__) || !defined(__UNIXC__) */

	/* modern parameter is default if classic parameter was not forced */
#		ifndef __CLASSICPARAM__
#			define __MODERNPARAM__
#		endif

#	endif /* !defined(__STDC__) && defined(__UNIXC__) */

#endif /* __cplusplus */



#ifndef __API
#	define __API
#endif

#ifndef __APICAST
#	define __APICAST __API
#endif



/* Mixed usage of modern function prototype and classic function definition
 is restricted because automatic type promotion rule is quite complex
 problem to be clearly solved by some macro definitions.
 ARG_BEGIN, ARG_1ST, ARG_AND, ARG_END, ARG_VOID or
 A_B, A_1, A_N, A_E, A_V are available */

#if defined(__MODERNPARAM__)
#	define A_L(param) param
#	define A_B(param) (
#	define A_E )
#	define A_X ,...)
#	define A_N ,
#	define A_V ( void )
#else
#	define A_L(param) ()
#	define A_B(param) param
#	define A_E ;
#	define A_X ; va_dcl
#	define A_N ;
#	define A_V ()
#endif /* defined(__MODERNPARAM__) */

#define A_1
#define ARG_BEGIN A_B
#define ARG_END   A_E
#define ARG_VAR   A_X
#define ARG_AND   A_N
#define ARG_VOID  A_V
#define ARG_1ST   A_1
#define ARG_LIST  A_L

/* export specification */

#ifdef __EXPORT__
#	ifdef EXPORT_BEFORE_RETURN
#		define API_RETURN(ret) EXPORT_KEYWORD ret
#		define API_NORETURN EXPORT_KEYWORD
#	else
#	ifdef EXPORT_AFTER_RETURN
#		define API_RETURN(ret) ret EXPORT_KEYWORD
#		define API_NORETURN EXPORT_KEYWORD
#	else
#	endif /* EXPORT_AFTER_RETURN */
#	endif /* EXPORT_BEFORE_RETURN */
#endif /* __EXPORT__ */

#ifndef API_RETURN
#	define API_RETURN(ret) ret
#	define API_NORETURN
#endif /* __EXPORT__ */

#ifndef API_NAME
#	define API_NAME(name) name
#endif

#include <coral/end/vtable.h>



#ifndef __GLOBAL_UNSUPPORTED__
#	define __GLOBAL_SUPPORTED__
#	define DEFINE_GLOBAL
#	define DEFINE_STATIC static
#	define SET_INITIAL_VALUE(value) = value
#else
#	define DEFINE_GLOBAL
#	define DEFINE_STATIC
#	define SET_INITIAL_VALUE(value)
#endif

#define INITIAL_VALUE SET_INITIAL_VALUE


#ifndef PROCESS_EXTERN
#	define PROCESS_EXTERN PROCESS_EXTERN_STATEMENT
#endif

#ifndef PROCESS_STATIC
#	define PROCESS_STATIC PROCESS_STATIC_STATEMENT
#endif





/* old version borland c generates undefined structure warning */

#ifndef __INTERNAL__
#if ( defined(__TURBOC__) && __TURBOC__<=0x410 )
#	define __DEFINE_STRUCT__
#endif
#endif


#endif /* __CORAL_COMPAT_H */

