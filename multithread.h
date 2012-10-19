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

#ifndef _MULTITHREAD_H
#define _MULTITHREAD_H

#include "backgammon.h"

/*#define DEBUG_MULTITHREADED 1*/
#ifdef DEBUG_MULTITHREADED
void multi_debug(const char *str, ...);
#else
#define multi_debug(x)
#endif

typedef struct _Task
{
	AsyncFun fun;
	void *data;
	struct _Task *pLinkedTask;
} Task;

typedef struct _AnalyseMoveTask
{
	Task task;
	moverecord *pmr;
	listOLD *plGame;
	statcontext *psc;
	matchstate ms;
} AnalyseMoveTask;

extern int MT_GetDoneTasks(void);
extern void MT_AbortTasks(void);
extern void MT_AddTask(Task *pt, gboolean lock);
extern void mt_add_tasks(unsigned int num_tasks, AsyncFun pFun, void *taskData, gpointer linked);
extern int MT_WaitForTasks(gboolean (*pCallback)(gpointer), int callbackTime, int autosave);

#if USE_MULTITHREAD

#include <glib.h>

#ifdef WIN32
#ifndef GLIB_THREADS
#include <windows.h>
#endif
#endif

#define MAX_NUMTHREADS 48

extern unsigned int MT_GetNumThreads(void);
extern void MT_Release(void);
extern void MT_Exclusive(void);
extern void MT_InitThreads(void);
extern void MT_StartThreads(void);
extern void MT_Close(void);
extern void MT_SetNumThreads(unsigned int num);
extern int MT_GetThreadID(void);
extern void MT_SyncInit(void);
extern void MT_SyncStart(void);
extern double MT_SyncEnd(void);
extern void MT_SetResultFailed(void);

#ifdef GLIB_THREADS
  #define MT_SafeInc(x) g_atomic_int_add(x, 1)
  #define MT_SafeIncValue(x) (g_atomic_int_exchange_and_add(x, 1) + 1)
  #define MT_SafeIncCheck(x) (g_atomic_int_exchange_and_add(x, 1))
  #define MT_SafeAdd(x, y) g_atomic_int_exchange_and_add(x, y)
  #define MT_SafeDec(x) g_atomic_int_add(x, -1)
  #define MT_SafeDecCheck(x) g_atomic_int_dec_and_test(x)
#else
  #define MT_SafeInc(x) (void)InterlockedIncrement((long*)x)
  #define MT_SafeIncValue(x) InterlockedIncrement((long*)x)
  #define MT_SafeIncCheck(x) (InterlockedIncrement((long*)x) > 1)
  #define MT_SafeAdd(x, y) InterlockedExchangeAdd((long*)x, y)
  #define MT_SafeDec(x) (void)InterlockedDecrement((long*)x)
  #define MT_SafeDecCheck(x) (InterlockedDecrement((long*)x) == 0)
#endif

#else /*USE_MULTITHREAD*/
#define MAX_NUMTHREADS 1
extern int asyncRet;
#define MT_Exclusive() {}
#define MT_Release() {}
#define MT_GetNumThreads() 1
#define MT_SetResultFailed() asyncRet = -1
#define MT_SafeInc(x) (++(*x))
#define MT_SafeIncValue(x) (++(*x))
#define MT_SafeIncCheck(x) ((*x)++)
#define MT_SafeAdd(x, y) ((*x) += y)
#define MT_SafeDec(x) (--(*x))
#define MT_SafeDecCheck(x) ((--(*x)) == 0)
#define MT_GetThreadID() 0
#endif

#endif
