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



#ifndef __CORALSRC_LIST_H
# define __CORALSRC_LIST_H

#include <coralsrc/def.h>
#include <coral/list.h>



#define _LIST_LINK(node,offset) LIST_LINK(void __FAR*,node,offset)
#define _LIST_CLOSET_BODY_OFFSET _MEM_ALIGN(sizeof(struct _ListCloset))
#define _LIST_CLOSET_HEADER(body) ((struct _ListCloset __FAR*)( (_integer8 __FAR*)(body) - _LIST_CLOSET_BODY_OFFSET ))
#define _LIST_CLOSET_BODY(header) ( (_integer8 __FAR*)(header) + _LIST_CLOSET_BODY_OFFSET )

#define _IMPLEMENT_LIST_GET_CLOSET(l,key) \
	while ( l ) { \
		struct _ListCloset __FAR* header = OP_SUBCLASS(struct _ListCloset,l,link); \
		if ( header->ptr.key == key ) return _LIST_CLOSET_BODY(header); \
		l = l->next; \
	} \
	return NULL;



__CBEGIN__
__CEND__



#endif /* __CORALSRC_LIST_H */
