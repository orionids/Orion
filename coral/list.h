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

  Alternatively, you can use this library under any other
 OSS licences ( like BSD ) or your own commercial license,
 as set out in license/orion.txt. */


/* Coral Linked List */



#ifndef __CORAL_LIST_H
# define __CORAL_LIST_H



#	include <coral/def.h>



#ifndef CORAL_SOURCE
#	define LIST_UNDEF_CORAL_SOURCE
#	define CORAL_SOURCE
#endif



__CBEGIN__



/* _ListCloset */

struct _ListCloset {
	struct _List list;
	int (__API* key)(); /* function pointer as a key */
};



#include <coral/api/list.h>



#ifdef __REDUCED__
#	define listAddCircularNode listAdd
#	define listCutCircularNode listCut
#endif

#ifdef LIST_UNDEF_CORAL_SOURCE
#	undef LIST_UNDEF_CORAL_SOURCE
#	undef CORAL_SOURCE
#endif

#ifndef CORAL_SOURCE
#	include <coral/export.h>
#endif



__CEND__



#endif /* __CORAL_LIST_H */
