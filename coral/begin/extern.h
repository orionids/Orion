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

#ifdef coral_compat_limits_h
#	include coral_compat_limits_h
#else
#ifdef CORAL_PATH
#	include CORAL_PATH/compat/limits.h>
#else
#	include <coral/compat/limits.h>
#endif
#endif

#undef DEFINE_GLOBAL
#define DEFINE_GLOBAL extern
#undef INITIAL_VALUE
#define INITIAL_VALUE(value)
