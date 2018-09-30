/* Coral library for low level data structure implementation
 Copyright (C) 1996-2004 Hee S. Chung

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/* Coral Internal Definition File

Automatic definitions :

__NATIVE__, coral native c run-time library is used */

#ifndef __CORALSRC_DEF_H
#	define __CORALSRC_DEF_H




/* force internal contexts */
#ifndef __INTERNAL__
#	define __INTERNAL__
#endif



/* @cmt0006 */
/* #include <stddef.h> // palm conflict!!! */
#include <coral/compat.h>



/* bcc32 and lahey f77 linking service */
#if defined(__L77__)
# define __NATIVE__ /* indicate native c run-time library is used */
# define _L77_END   /* asm mov esp, ebp : to define an interface function for F77L3 */
#endif


#include <coral/def.h>



#endif /* __CORALSRC_DEF_H */

