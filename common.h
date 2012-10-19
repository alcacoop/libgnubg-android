/* This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 or later of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*! \file common.h
    \brief Odd definitions
*/

#ifndef _COMMON_H_
#define _COMMON_H_
#include <math.h>

#if !_GNU_SOURCE && !defined (__attribute__)
/*! \brief GNU C specific attributes, e.g. unused
// */
#define __attribute__(X)
#endif

/*! \brief Safe double error value (and float with a cast)
 */
#if defined(HUGE_VAL)
#define ERR_VAL (-HUGE_VAL)
#else
#define ERR_VAL (-FLT_MAX)
#endif

/*! \brief Macro to extract sign
 */
#ifndef SGN
#define SGN(x) ((x) > 0 ? 1 : -1)
#endif

/*! \brief typedef of signal handler function
 */
#if HAVE_SIGACTION
typedef struct sigaction psighandler;
#elif HAVE_SIGVEC
typedef struct sigvec psighandler;
#else
typedef void(*psighandler) (int);
#endif

/* abs returns unsigned int by definition */
#define Abs(a) ((unsigned int)abs(a))

/* signbit() is used only in a somewhat performance sensitive place
   in lib/sigmoid.h. If HAVE_DECL_SIGNBIT is false, maybe we should
   work around it there instead of using this */
#if !HAVE_DECL_SIGNBIT
/* copysign() caters for special IEEE 754 numbers */
#define signbit(x) (copysign(1, (x)) < 0)
#endif

/* Do we need to use g_utf8_casefold() for utf8 anywhere? */
#define StrCaseCmp(s1, s2) g_ascii_strcasecmp(s1, s2)
#define StrNCaseCmp(s1, s2, n) g_ascii_strncasecmp(s1, s2, (gsize)n)
/* Avoid new code using strcase functions */
#define strcasecmp strcasecmp_error_use_StrCaseCmp
#define strncasecmp strncasecmp_error_use_StrNCaseCmp

/* Macro to mark paramaters that aren't used in the function */	
#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(_lint)
# define UNUSED(x) /*lint -e{715, 818}*/ _unused_##x
#else
# define UNUSED(x) _unused_##x
#endif
#endif
