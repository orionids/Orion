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

#undef _CAPI
#undef _CAPI_FPRET
#undef _CAPI_NORET
#undef _API
#undef _API_FPRET
#undef _API_NORET

#undef _NAMED_CAPI
#undef _NAMED_CAPI_FPRET
#undef _NAMED_CAPI_NORET
#undef _NAMED_API
#undef _NAMED_API_FPRET
#undef _NAMED_API_NORET
#undef _VM_API
#undef _VM_API_NORET
#undef _ARG
#undef _AARG

#define _CAPI(ret,name,param) API_RETURN(ret) API_NAME(name) A_L(param);
#define _CAPI_FPRET(ret,prefix,suffix,name,param) API_RETURN(ret) ( prefix ( API_NAME(name) A_L(param) ) ) suffix;
#define _CAPI_NORET(name,param) API_NORETURN API_NAME(name) A_L(param);
#define _API(ret,name,param) API_RETURN(ret) __API API_NAME(name) A_L(param);
#define _API_FPRET(ret,prefix,suffix,name,param) API_RETURN(ret) ( __API prefix ( __API API_NAME(name) A_L(param) ) ) suffix;
#define _API_NORET(name,param) API_NORETURN __API API_NAME(name) A_L(param);

#define _NAMED_CAPI(s,ret,name,param) _CAPI(ret,name,param)
#define _NAMED_CAPI_FPRET(s,ret,prefix,suffix,name,param) _CAPI_FPRET(ret,prefix,suffix,name,param)
#define _NAMED_CAPI_NORET(s,name,param) _CAPI_NORET(name,param)
#define _NAMED_API(s,ret,name,param) _API(ret,name,param)
#define _NAMED_API_FPRET(s,ret,prefix,suffix,name,param) _API_FPRET(ret,prefix,suffix,name,param)
#define _NAMED_API_NORET(s,name,param) _API_NORET(name,param)

#define _VM_API(ret,name,arg,narg) _API(ret,name,arg)
#define _VM_API_NORET _API_NORET
#define _ARG(i,type,name) type name
#define _AARG(i,ret,arg,name) ret (__API* name) arg

