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

/* Linked list */


#undef __DEBUG__
#define CORAL_SOURCE
#include <coralsrc/list.h>



#ifndef __REDUCED__
__MODULEBEGIN__( "laddc.c" )
/* listAddCircularNode */

void __API
listAddCircularNode
	A_B(( prev, node ))
	A_1		struct _List __FAR* prev
	A_N		struct _List __FAR* node
	A_E
{
	struct _List __FAR* next = prev->link;
	node->link = next;
	next[1].link = node;
	node[1].link = prev;
	prev->link = node;
}



/* listCutCircularNode */

void __API
listCutCircularNode
	A_B(( node ))
	A_1		struct _List __FAR* node
	A_E
{
	struct _List __FAR* next = node->link;
	struct _List __FAR* prev = node[1].link;
	prev->link = next;
	next[1].link = prev;
}
__MODULEEND__
#endif



__MODULEBEGIN_( "ladd.c" )

/* listAdd */
void __API
listAdd
	A_B(( prev, node ))
	A_1		struct _List __FAR* prev
	A_N		struct _List __FAR* node
	A_E
{
	node[1].link = prev;
	if ( prev ) {
		struct _List __FAR* next = prev->link;
		node->link = next;
		if ( next ) next[1].link = node;
		prev->link = node;
	} else {
		node->link = NULL;
	}
}



/* listCut */

void __API
listCut
	A_B(( node ))
	A_1		struct _List __FAR* node
	A_E
{
	struct _List __FAR* next = node->link;
	struct _List __FAR* prev = node[1].link;
	if ( prev ) prev->link = next;
	if ( next ) next[1].link = prev;
}
__MODULEEND__



/* end of list.c */

