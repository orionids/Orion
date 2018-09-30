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

/* API_RETURN and API_NORETURN must not be used here because export keyword is not allowed for fptr definition */
#define _CAPI(ret,name,param) ret (__FAR* API_NAME(name)) A_L(param);
#define _CAPI_FPRET(ret,prefix,suffix,name,param) ret ( prefix ( (__FAR* API_NAME(name)) A_L(param) ) ) suffix;
#define _CAPI_NORET(name,param) (__FAR* API_NAME(name)) A_L(param);
#define _API(ret,name,param) ret (__API* API_NAME(name)) A_L(param);
#define _API_FPRET(ret,prefix,suffix,name,param) ret ( __API prefix ( (__API* API_NAME(name)) A_L(param) ) ) suffix;
#define _API_NORET(name,param) __API (__API* API_NAME(name)) A_L(param)

