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

#ifndef __INTEGER64__

__MODULEBEGIN__( "" )

/* begin extended integer type for c */
#ifdef __cplusplus
#	undef _integer64
#	undef _unsigned64
#	define _integer64 __integerx
#	define _unsigned64 __integerx
#endif

__MODULEEND__

#endif /* __INTEGER64__ */

