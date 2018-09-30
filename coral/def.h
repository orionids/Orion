/* Coral library for low level data structure implementation
  Copyright (C) 1996-2017 Hee S. Chung

  This library is dual-licensed: you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as 
 published by the Free Software Foundation. For the terms of this 
 license, see license/gpl-3.txt or <http://www.gnu.org/licenses/>.

  You are free to use this library under the terms of the GNU General
 Public License, but WITHOUT ANY WARRANTY; without even the implied 
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 Alternatively, you can use these files under your own
license if you have written permission of the copyright
holder or corresponding equivalent conditions.
Refer to <http://www.orionids.org/projects/license.txt> */

/* Coral library Definitions

 < Code Generation Switches >

 __STRICT__      , use strict type, this option will affect windows.h
 __EXPORT__      , export functions without def file if possible
 __L77__         , settings are switched to use L77
 __MODERNPARAM__ , use modern c or c++ parameter definition rules
 __CLASSICPARAM__, use classic c parameter definition rules
 __BUILDSHARED__ , use it to build shared library ( must be needed for the systems which virtual address space is not supported )
 __RELEASE__     , remove debug codes */



#ifndef __CORAL_DEF_H
#	define __CORAL_DEF_H



#include <coral/compat.h>
#include <coral/const.h>


#ifdef __PROPRIETARY__
#	include __PROPRIETARY__
#endif


/* version 1.0.0 */

#define __CORALVERSION__ 0x00010000



#ifdef __RELEASE__
#	undef __DEBUG__
#endif




#ifdef DL_VTABLE
	extern const struct _coral_api __FAR* coral_api;
#else
#ifndef CORAL_SOURCE
#	define CORAL_SOURCE /* force CORAL_SOURCE so macros to override function call won't be defined */
#endif
#endif



/* multi statement */

#ifdef __MULTISTATEMENT__
#	define BEGIN_BLOCK do {
#	define END_BLOCK  } while(0)
#else
#	define BEGIN_BLOCK {
#	define END_BLOCK   }
#endif



/* Module specifiers */

#ifdef __PREPROCESS__
#	define __MODULEBEGIN__(name) __MODULEBEGIN name
#	define __MODULEEND__         __MODULEEND
#else /* !__PREPROCESS__ */
#	define __MODULEBEGIN__(name)
#	define __MODULEEND__
#endif /* __PREPROCESS__ */



/* frequently used operators */
#define _OP_STRING(str) #str
#define _OP_RESOLVE(s) _OP_STRING(s)

#define OP_INCFILE(sym) _OP_RESOLVE(sym)
#define OP_OFFSET(c,m) ((_pointer)((char*)&((c*)0)->m-(char*)0))
#define OP_RELATIVE_OFFSET(c,m,b) (OP_OFFSET(c,m)-OP_OFFSET(c,b))
#define OP_SUPERCLASS(_type,p,offset) ((_type __FAR*)((_integer8 __FAR*)(p)+(offset)))
#define OP_SUBCLASS_PTR(p,offset) ((_integer8 __FAR*)(p)-(offset))
#define OP_SUBCLASS(_type,p,m) ((_type __FAR*)OP_SUBCLASS_PTR(p,OP_OFFSET(_type,m)))

/* byte-1 will be constant if byte is constant and division will
 be optimized by the compiler if byte can be represented by 2^n,
 n is integer */
#define OP_BYTE(val,byte) (((val)+((byte)-1))/(byte))
#define OP_ALIGN(val,byte) (OP_BYTE(val,byte)*(byte))



/* Context templates */
#define DECLARE_CONTEXT(type,name) type; typedef type __FAR* name



/* Priority
	0=normal, 0x1000 to 0x6000 priority level ( 0x3000 is medium ) */
#define PRIORITY_MASK          0xf000 /* mask to get priority */
#define PRIORITY_MASK_LEVEL    0x7000 /* priority level */
#define PRIORITY_FLAG_NEGATIVE 0x8000 /* negative priority */


/* extensible parameter */
#define PARAM_MASK_INTEGER  0x0f00  /* mask to determine number of integer parameters */
#define PARAM_MASK_STRING   0xf000  /* mask to determine number of string parameters */
#define PARAM_MASK_ID       0x00ff  /* mask to determine parameter identifier */
#define PARAM_SHIFT_INTEGER 8       /* shifting constant to determine number of integer parameters */
#define PARAM_SHIFT_STRING  12      /* shifting constant to determine number of string parameters */
#define PARAM_END           0

/* system identifier */
#define SYSTEM_LF      0x01             /* system uses line feed as line ends */
#define SYSTEM_CR      0x02             /* system uses carriage return as line ends */
#define SYSTEM_CRLF    (0x01|0x02)      /* system uses cr/lf combi as line ends */
#define SYSTEM_LFCR    (0x01|0x02|0x04) /* system uses lf/cr combi as line ends */
#define SYSTEM_UNIX    SYSTEM_LF        /* unix or cygwin on windows */
#define SYSTEM_MAC     SYSTEM_CR        /* macintosh */
#define SYSTEM_DOS     SYSTEM_CRLF      /* dos */
#define SYSTEM_CYGWIN  (0x01|0x04)      /* cygwin */
#define SYSTEM_MINGW   (0x01|0x08)      /* mingw */
#define SYSTEM_UNKNOWN 0x08             /* system format cannot be detected */

#ifndef NULL
#   ifdef __cplusplus
#       define NULL 0
#   else
#       ifdef __16__
#           define NULL ((void __FAR*)0)
#       else
#           define NULL ((void*)0)
#       endif
#   endif
#endif



#ifdef __cplusplus
#	define __C__ extern "C"
#	define __CBEGIN__ __C__ {
#	define __CEND__ }
#	ifndef __STRICT__
#		define __STRICT__
#	endif
#else /* !__cplusplus */
#	define __C__
#	define __CBEGIN__
#	define __CEND__
#endif /* __cplusplus */



/* XXX _MEM_MSC_DEBUG is used in ge/gemain.cpp so it cannot be internal
	macro so rename it */
#ifdef __DEBUG__

#	if defined(_MSC_VER) && !defined(__SYMBIAN__) && !defined(__WINCE__)
#		define _MEM_MSC_DEBUG
#	endif

#	if !defined(_MEM_MSC_DEBUG) && !defined(_MEM_DEBUG) && !defined(_MEM_DISABLE_DEBUG)
#		define _MEM_DEBUG /* force _MEM_DEBUG to detect memory leak */
#	endif

#endif


#ifdef __MINIMAL__
#	ifndef __REDUCED___
#		define __REDUCED__
#	endif
#endif


/* Explicit size integers */

typedef signed char    _integer8;
typedef unsigned char  _unsigned8;
typedef _integer8      __integer8;
typedef _unsigned8     __unsigned8;
typedef short          _integer16;
typedef unsigned short _unsigned16;
typedef int            _min16;
typedef unsigned       _umin16;


typedef _unsigned8 _utf8char;
typedef _unsigned16 _ucs2char;
typedef _integer32 _fix;

#if __ARCH__ > 64
...
#else
#if __ARCH__ == 64
/* to consider the case __INTEGERX_UNSUPPORTED__ is defined not to use built-in extended integer,
_pointer and _address uses it explicitly */
typedef _builtin_integerx  _pointer;
typedef _builtin_unsignedx _address;
#	define TYPE_POINTER TYPE_U64
#	define TYPE_POINTER_ID TYPE_U64_ID
#else
#if __ARCH__ >= 16
typedef _integer32  _pointer;
typedef _unsigned32 _address;
#	define TYPE_POINTER    TYPE_U32
#	define TYPE_POINTER_ID TYPE_U64_ID
#else
typedef _integer16  _pointer;
typedef _unsigned16 _address;
#	define TYPE_POINTER    TYPE_U16
#	define TYPE_POINTER_ID TYPE_U16_ID
#endif
#endif
#endif

/* extension flags usually combinated to high 16 bit of _umin32 */
#define EXT_FLAG(p) (((_umin32)(p)) << 16)
#define EXT_MASK    ((_umin32)0xffff0000UL)
#define EXT_SHIFT   16


/* _List */

struct _List {
	struct _List __FAR* link;
};


/* _BinTree */

#if 0
//def __MINIMAL__
#	define _BinTree _List
#	define BINTREE_PAD
#else
/* _BinTree */

struct _BinTree {
	struct _BinTree __FAR* left, __FAR* right;
#if __ARCH__ <= 16
	_integer8 bf[1];
#	define BINTREE_PAD32 _integer8 __pad[3];
#	define BINTREE_PAD64 _integer8 __pad[7];
#else
#	define BINTREE_PAD32
#	if __ARCH__ <= 32
#		define BINTREE_PAD64 _integer8 __pad[4];
#	else
#		define BINTREE_PAD64
#	endif
	_integer8 bf[sizeof(struct _BinTree __FAR*)];
#endif
};
#endif


/* Memory expansion */

struct _MemExpansion {
	void __FAR* ctx;
	unsigned   _size;
};



/* _MemQueue, for initializer preferable offset of member buf and n is selected  */

struct _MemQueue {
	void __FAR* buf; /* buffer */
	unsigned n, head, tail; /* head, tail and buffer size */
};



/* Symbol system context */

struct _SymContext {
	const char __FAR* (__API* search) A_L(( void __FAR* handle, const char __FAR* key ));
	void __FAR* handle; /* handle to access symbol source */
};



/* Coral I/O */

struct _IOContext {
	long (__API* read) A_L(( struct _IOContext __FAR* io, void __FAR* p, long size ));
	long (__API* write) A_L(( struct _IOContext __FAR* io, void __FAR* p, long size ));
	long (__API* seek) A_L(( struct _IOContext __FAR* io, long offset, long __FAR* offset_high, int seekpos ));
	void __FAR* handle;
	void __FAR* cache;  /* I/O cache / temporary pointer to store accepting client for server side / or wr pipe */
	void __FAR* closet; /* typically cross reference for async context */
	struct _IOLogContext __FAR* iolc;
	unsigned mode;
};



/* _diohandle */

#ifdef __EMBEDDED__
#	ifndef DIO_COMPACT_HANDLE
#		define DIO_COMPACT_HANDLE
#	endif
#endif

/* DIO_COMPACT_HANDLE must be used if implementation was
 build for compact handle. Basically dio handle type is
 unique regardless of dio storage */

#ifdef DIO_COMPACT_HANDLE
#define IO_COMPACT_HANDLE
typedef _unsigned32 _diohandle;
#else
typedef _unsigned64 _diohandle;
#endif

typedef _diohandle _iohandle;


/* dynamic loader function */

typedef int (__API* _DLFunction) A_L(( void __FAR* ctx, unsigned action, void __FAR* param ));

struct _DLCallback {
	_DLFunction function;
	void __FAR* context;
};


/* Coral I/O buffer */

struct _IOBuffer {
	void __FAR* context;       /* I/O buffer context */
	_unsigned8 __FAR* (__API* lock) A_L(( void __FAR* context, int __FAR* locked, int size, unsigned flag ));
	void (__API* unlock) A_L(( void __FAR* context, _unsigned8 __FAR* current ));
};



/* Coral asynchronous scheduler */

struct _AsyncContext {
	void __FAR* closet;    /* custom closet */
	_DLFunction callback;
	long c0, c1; /* channel status */
};



/* _StringReference */

struct _StringReference {
	_integer16 ref; /* reference counter */
	char     s[1];
};


/* _IOSource */

struct _IOSource {
	unsigned flag;
	_DLFunction ctrl; /* first parameter is _IOSource */
	struct _IOBuffer iob;
	struct _StringReference __FAR* name;
};


#ifdef IMA_INTEGRATED
#	ifdef IMA_DEF
#		include OP_INCFILE(IMA_DEF)
#	else
#		include <ima/def.h>
#	endif
#endif


#endif /* __CORAL_DEF_H */
