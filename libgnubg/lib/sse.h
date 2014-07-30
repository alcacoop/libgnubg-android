/* $Id: sse.h,v 1.8 2007/07/02 14:14:18 ace Exp $ 
 * This program is free software; you can redistribute it and/or modify
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

#if USE_SSE_VECTORIZE

#include <stdlib.h>

#define ALIGN_SIZE 16

#ifdef _MSC_VER
#define SSE_ALIGN(D) __declspec(align(ALIGN_SIZE)) D
#else
#define SSE_ALIGN(D) D __attribute__ ((aligned(ALIGN_SIZE)))
#endif

#define sse_aligned(ar) (!(((int)ar) % ALIGN_SIZE))

extern float *sse_malloc(size_t size);
extern void sse_free(float* ptr);

#else

#define SSE_ALIGN(D) D
#define sse_malloc malloc
#define sse_free free

#endif
