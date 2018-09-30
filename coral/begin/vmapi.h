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

#ifndef __NOVMAPI__

#include <coral/inline.h>
#include <coral/vm.h>

#undef _CAPI
#undef _CAPI_FPRET
#undef _CAPI_NORET
#undef _API
#undef _API_FPRET
#undef _API_NORET
#undef _VM_API
#undef _VM_API_NORET
#undef _ARG
#undef _AARG

#define _CAPI(ret,name,param)
#define _CAPI_FPRET(ret,prefix,suffix,name,param)
#define _CAPI_NORET(name,param)
#define _API(ret,name,param)
#define _API_FPRET(ret,prefix,suffix,name,param)
#define _API_NORET(name,param)

#define _ARG(i,type,name) *(type __FAR*)VM_ARG(top,i)
#define _AARG(i,ret,arg,name) *(ret (__API*__FAR*) arg)VM_ARG(top,i)

#ifndef VM_BEGIN_API
#define VM_BEGIN_API(name,arg)\
int __API \
OP_CONCAT2(vm_,name) A_B(( vmt, i )) \
	A_1		struct _VMThread __FAR* vmt \
	A_N		unsigned i \
	A_E \
{\
	void __FAR* top = vmt->top;\
	(void) i;

#define VM_END_API(name,arg) name arg;\
	return 0;\
}
#endif


#define _VM_API(ret,name,arg,narg) \
VM_BEGIN_API(name,arg) \
	*(ret __FAR*)VM_ARG(top,narg) = \
VM_END_API(name,arg)

#define _VM_API_NORET(name,arg) \
VM_BEGIN_API(name,arg) VM_END_API(name,arg)

#endif
