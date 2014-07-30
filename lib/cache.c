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
 *
 *
 * cache.c
 *
 * by Gary Wong, 1997-2000
 * $Id: cache.c,v 1.35 2013/06/16 02:16:23 mdpetch Exp $
 */

#include "config.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <stdio.h>
#endif

#include "cache.h"
#include "positionid.h"

#if USE_MULTITHREAD
#include "multithread.h"

#if defined(__GNUC__) && ( __GNUC__ * 100 + __GNUC_MINOR__ >= 401 ) \
  && (defined (__i386) || defined (__x86_64))

#define cache_lock(pc, k) \
    while (__sync_lock_test_and_set(&(pc->entries[k].lock), 1)) \
         while (pc->entries[k].lock) \
            __asm volatile ("pause" ::: "memory")

#define cache_unlock(pc, k) \
    __sync_lock_release(&(pc->entries[k].lock));

#else

#define cache_lock(pc, k) \
if (MT_SafeIncCheck(&(pc->entries[k].lock))) \
	WaitForLock(&(pc->entries[k].lock))

#define cache_unlock(pc, k) MT_SafeDec(&(pc->entries[k].lock))

static void
WaitForLock(volatile int *lock)
{
    do {
        MT_SafeDec(lock);
    } while (MT_SafeIncCheck(lock));
}

#endif

#endif                          /* USE_MULTITHREAD */


int
CacheCreate(evalCache * pc, unsigned int s)
{
#if CACHE_STATS
    pc->cLookup = 0;
    pc->cHit = 0;
    pc->nAdds = 0;
#endif

    if (s > 1u << 31)
        return -1;

    pc->size = s;
    /* adjust size to smallest power of 2 GE to s */
    while ((s & (s - 1)) != 0)
        s &= (s - 1);

    pc->size = (s < pc->size) ? 2 * s : s;
    pc->hashMask = (pc->size >> 1) - 1;

    pc->entries = (cacheNode *) malloc((pc->size / 2) * sizeof(*pc->entries));
    if (pc->entries == 0)
        return -1;

    CacheFlush(pc);
    return 0;
}

/*
 * 
 * from http://burtleburtle.net/bob/c/lookup3.c
 * 
 * -------------------------------------------------------------------------------
 * lookup3.c, by Bob Jenkins, May 2006, Public Domain.
 * 
 * These are functions for producing 32-bit hashes for hash table lookup.
 * hashword(), hashlittle(), hashlittle2(), hashbig(), mix(), and final() 
 * are externally useful functions.  Routines to test the hash are included 
 * if SELF_TEST is defined.  You can use this free for any purpose.  It's in
 * the public domain.  It has no warranty.
 * [...]
 * If you want to find a hash of, say, exactly 7 integers, do
 * a = i1;  b = i2;  c = i3;
 * mix(a,b,c);
 * a += i4; b += i5; c += i6;
 * mix(a,b,c);
 * a += i7;
 * final(a,b,c);
 * then use c as the hash value.
 * [...]
 * -------------------------------------------------------------------------------
 */

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

/*
 * -------------------------------------------------------------------------------
 * mix -- mix 3 32-bit values reversibly.
 * 
 * This is reversible, so any information in (a,b,c) before mix() is
 * still in (a,b,c) after mix().
 * 
 * If four pairs of (a,b,c) inputs are run through mix(), or through
 * mix() in reverse, there are at least 32 bits of the output that
 * are sometimes the same for one pair and different for another pair.
 * This was tested for:
 * * pairs that differed by one bit, by two bits, in any combination
 * of top bits of (a,b,c), or in any combination of bottom bits of
 * (a,b,c).
 * * "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
 * the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
 * is commonly produced by subtraction) look like a single 1-bit
 * difference.
 * * the base values were pseudorandom, all zero but one bit set, or 
 * all zero plus a counter that starts at zero.
 * 
 * Some k values for my "a-=c; a^=rot(c,k); c+=b;" arrangement that
 * satisfy this are
 * 4  6  8 16 19  4
 * 9 15  3 18 27 15
 * 14  9  3  7 17  3
 * Well, "9 15 3 18 27 15" didn't quite get 32 bits diffing
 * for "differ" defined as + with a one-bit base and a two-bit delta.  I
 * used http://burtleburtle.net/bob/hash/avalanche.html to choose 
 * the operations, constants, and arrangements of the variables.
 * 
 * This does not achieve avalanche.  There are input bits of (a,b,c)
 * that fail to affect some output bits of (a,b,c), especially of a.  The
 * most thoroughly mixed value is c, but it doesn't really even achieve
 * avalanche in c.
 * 
 * This allows some parallelism.  Read-after-writes are good at doubling
 * the number of bits affected, so the goal of mixing pulls in the opposite
 * direction as the goal of parallelism.  I did what I could.  Rotates
 * seem to cost as much as shifts on every machine I could lay my hands
 * on, and rotates are much kinder to the top and bottom bits, so I used
 * rotates.
 * -------------------------------------------------------------------------------
 */
#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}

/*
 * -------------------------------------------------------------------------------
 * final -- final mixing of 3 32-bit values (a,b,c) into c
 * 
 * Pairs of (a,b,c) values differing in only a few bits will usually
 * produce values of c that look totally different.  This was tested for
 * * pairs that differed by one bit, by two bits, in any combination
 * of top bits of (a,b,c), or in any combination of bottom bits of
 * (a,b,c).
 * * "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
 * the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
 * is commonly produced by subtraction) look like a single 1-bit
 * difference.
 * * the base values were pseudorandom, all zero but one bit set, or 
 * all zero plus a counter that starts at zero.
 * 
 * These constants passed:
 * 14 11 25 16 4 14 24
 * 12 14 25 16 4 14 24
 * and these came close:
 * 4  8 15 26 3 22 24
 * 10  8 15 26 3 22 24
 * 11  8 15 26 3 22 24
 * -------------------------------------------------------------------------------
 */
#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

extern uint32_t
GetHashKey(uint32_t hashMask, const cacheNodeDetail * e)
{
    uint32_t a, b, c;

    a = b = c = 0xdeadbeef + (uint32_t) e->nEvalContext;

    a = a + e->key.data[0];
    b = b + e->key.data[1];
    c = c + e->key.data[2];

    mix(a, b, c);

    a = a + e->key.data[3];
    b = b + e->key.data[4];
    b = b + e->key.data[5];

    mix(a, b, c);

    a = a + e->key.data[6];

    final(a, b, c);

    return (c & hashMask);
}

uint32_t
CacheLookupWithLocking(evalCache * pc, const cacheNodeDetail * e, float *arOut, float *arCubeful)
{
    uint32_t const l = GetHashKey(pc->hashMask, e);

#if CACHE_STATS
    ++pc->cLookup;
#endif
#if USE_MULTITHREAD
    cache_lock(pc, l);
#endif
    if (!EqualKeys(pc->entries[l].nd_primary.key, e->key) || pc->entries[l].nd_primary.nEvalContext != e->nEvalContext) {       /* Not in primary slot */
        if (!EqualKeys(pc->entries[l].nd_secondary.key, e->key) || pc->entries[l].nd_secondary.nEvalContext != e->nEvalContext) {       /* Cache miss */
#if USE_MULTITHREAD
            cache_unlock(pc, l);
#endif
            return l;
        } else {                /* Found in second slot, promote "hot" entry */
            cacheNodeDetail tmp = pc->entries[l].nd_primary;

            pc->entries[l].nd_primary = pc->entries[l].nd_secondary;
            pc->entries[l].nd_secondary = tmp;
        }
    }
    /* Cache hit */
#if CACHE_STATS
    ++pc->cHit;
#endif

    memcpy(arOut, pc->entries[l].nd_primary.ar, sizeof(float) * 5 /*NUM_OUTPUTS */ );
    if (arCubeful)
        *arCubeful = pc->entries[l].nd_primary.ar[5];   /* Cubeful equity stored in slot 5 */

#if USE_MULTITHREAD
    cache_unlock(pc, l);
#endif

    return CACHEHIT;
}

uint32_t
CacheLookupNoLocking(evalCache * pc, const cacheNodeDetail * e, float *arOut, float *arCubeful)
{
    uint32_t const l = GetHashKey(pc->hashMask, e);

#if CACHE_STATS
    ++pc->cLookup;
#endif
    if (!EqualKeys(pc->entries[l].nd_primary.key, e->key) || pc->entries[l].nd_primary.nEvalContext != e->nEvalContext) {       /* Not in primary slot */
        if (!EqualKeys(pc->entries[l].nd_secondary.key, e->key) || pc->entries[l].nd_secondary.nEvalContext != e->nEvalContext) {       /* Cache miss */
            return l;
        } else {                /* Found in second slot, promote "hot" entry */
            cacheNodeDetail tmp = pc->entries[l].nd_primary;

            pc->entries[l].nd_primary = pc->entries[l].nd_secondary;
            pc->entries[l].nd_secondary = tmp;
        }
    }
    /* Cache hit */
#if CACHE_STATS
    ++pc->cHit;
#endif

    memcpy(arOut, pc->entries[l].nd_primary.ar, sizeof(float) * 5 /*NUM_OUTPUTS */ );
    if (arCubeful)
        *arCubeful = pc->entries[l].nd_primary.ar[5];   /* Cubeful equity stored in slot 5 */

    return CACHEHIT;
}

void
CacheAddWithLocking(evalCache * pc, const cacheNodeDetail * e, uint32_t l)
{
#if USE_MULTITHREAD
    cache_lock(pc, l);
#endif

    pc->entries[l].nd_secondary = pc->entries[l].nd_primary;
    pc->entries[l].nd_primary = *e;

#if USE_MULTITHREAD
    cache_unlock(pc, l);
#endif

#if CACHE_STATS
    ++pc->nAdds;
#endif
}

void
CacheDestroy(const evalCache * pc)
{
    free(pc->entries);
}

void
CacheFlush(const evalCache * pc)
{
    unsigned int k;
    for (k = 0; k < pc->size / 2; ++k) {
        pc->entries[k].nd_primary.nEvalContext = -1;
        pc->entries[k].nd_secondary.nEvalContext = -1;
#if USE_MULTITHREAD
        pc->entries[k].lock = 0;
#endif
    }
}

int
CacheResize(evalCache * pc, unsigned int cNew)
{
    if (cNew != pc->size) {
        CacheDestroy(pc);
        if (CacheCreate(pc, cNew) != 0)
            return -1;
    }

    return (int) pc->size;
}

void
CacheStats(const evalCache * pc, unsigned int *pcLookup, unsigned int *pcHit, unsigned int *pcUsed)
{
#if CACHE_STATS
    if (pcLookup)
        *pcLookup = pc->cLookup;

    if (pcHit)
        *pcHit = pc->cHit;

    if (pcUsed)
        *pcUsed = pc->nAdds;
#else
    (void) pc;                  /* suppress unused parameter compiler warning */

    if (pcLookup)
        *pcLookup = 0;

    if (pcHit)
        *pcHit = 0;

    if (pcUsed)
        *pcUsed = 0;
#endif
}
