/*
 * multithread.c
 *
 * by Jon Kinsey, 2008
 *
 * Multithreaded operations
 *
 *
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
 * 
 * $Id: multithread.c,v 1.70 2012/03/25 12:08:48 plm Exp $
 */

#include "config.h"
#if USE_MULTITHREAD
#ifdef WIN32
#include <process.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#if USE_GTK
#include <gtk/gtk.h>
#include <gtkgame.h>
#endif

#include "multithread.h"
#include "speed.h"
#include "rollout.h"
#include "util.h"

#define UI_UPDATETIME 250

#ifdef TRY_COUNTING_PROCEESING_UNITS
extern int GetLogicalProcssingUnitCount(void);
#endif
#ifdef DEBUG_MULTITHREADED
unsigned int mainThreadID;
#endif

#ifdef GLIB_THREADS
typedef struct _ManualEvent
{
    GCond* cond;
    int signalled;
} *ManualEvent;
typedef GPrivate* TLSItem;
typedef GCond* Event;
typedef GMutex* Mutex;
GMutex* condMutex=NULL;    /* Extra mutex needed for waiting */
GAsyncQueue *async_queue=NULL; /* Needed for async waiting */
#else
typedef HANDLE ManualEvent;
typedef DWORD TLSItem;
typedef HANDLE Event;
typedef HANDLE Mutex;
#endif

typedef struct _ThreadData
{
    ManualEvent activity;
    TLSItem tlsItem;
    Mutex queueLock;
    Mutex multiLock;
	ManualEvent syncStart;
	ManualEvent syncEnd;

    int addedTasks;
    int doneTasks;
    int totalTasks;

    GList *tasks;
    int result;

	int closingThreads;
	unsigned int numThreads;
} ThreadData;

ThreadData td;

#ifdef GLIB_THREADS

static void TLSCreate(TLSItem *pItem)
{
    *pItem = g_private_new(free);
}

static void TLSFree(TLSItem pItem)
{	/* Done automaticaly by glib */
}

static void TLSSetValue(TLSItem pItem, int value)
{
	int *pNew = (int*)malloc(sizeof(int));
	*pNew = value;
    g_private_set(pItem, (gpointer)pNew);
}

#define TLSGet(item) *((int*)g_private_get(item))

static void InitManualEvent(ManualEvent *pME)
{
    ManualEvent pNewME = malloc(sizeof(*pNewME));
    pNewME->cond = g_cond_new();
    pNewME->signalled = FALSE;
    *pME = pNewME;
}

static void FreeManualEvent(ManualEvent ME)
{
    g_cond_free(ME->cond);
    free(ME);
}

static void WaitForManualEvent(ManualEvent ME)
{
	GTimeVal tv;
	multi_debug("wait for manual event locks");
	g_mutex_lock(condMutex);
	while (!ME->signalled) {
		multi_debug("waiting for manual event");
		g_get_current_time(&tv);
		g_time_val_add(&tv, 10 * 1000 * 1000);
		if (g_cond_timed_wait(ME->cond, condMutex, &tv))
			break;
		else
		{
			multi_debug("still waiting for manual event");
		}
	}

	g_mutex_unlock(condMutex);
	multi_debug("wait for manual event unlocks");
}

static void ResetManualEvent(ManualEvent ME)
{
	multi_debug("reset manual event locks");
    g_mutex_lock(condMutex);
    ME->signalled = FALSE;
    g_mutex_unlock(condMutex);
	multi_debug("reset manual event unlocks");
}

static void SetManualEvent(ManualEvent ME)
{
	multi_debug("reset manual event locks");
    g_mutex_lock(condMutex);
    ME->signalled = TRUE;
    g_cond_broadcast(ME->cond);
    g_mutex_unlock(condMutex);
	multi_debug("reset manual event unlocks");
}

static void InitMutex(Mutex *pMutex)
{
    *pMutex = g_mutex_new();
}

static void FreeMutex(Mutex mutex)
{
    g_mutex_free(mutex);
}

#ifdef DEBUG_MULTITHREADED
void Mutex_Lock(Mutex mutex, const char *reason)
{
	multi_debug(reason);
	g_mutex_lock(mutex);
}
void Mutex_Release(Mutex mutex)
{
	multi_debug("Releasing lock");
	g_mutex_unlock(mutex);
}
#else
#define Mutex_Lock(mutex, reason) g_mutex_lock(mutex)
#define Mutex_Release(mutex) g_mutex_unlock(mutex)
#endif

#else    /* win32 */

static void TLSCreate(TLSItem *ppItem)
{
    *ppItem = TlsAlloc();
	if (*ppItem == TLS_OUT_OF_INDEXES)
		PrintSystemError("calling TlsAlloc");
}

static void TLSFree(TLSItem pItem)
{
	free(TlsGetValue(pItem));
	TlsFree(pItem);
}

static void TLSSetValue(TLSItem pItem, int value)
{
	int *pNew = (int*)malloc(sizeof(int));
	*pNew = value;
    if (TlsSetValue(pItem, pNew) == 0)
		PrintSystemError("calling TLSSetValue");
}

#define TLSGet(item) *((int*)TlsGetValue(item))

static void InitManualEvent(ManualEvent *pME)
{
    *pME = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (*pME == NULL)
		PrintSystemError("creating manual event");
}

static void FreeManualEvent(ManualEvent ME)
{
    CloseHandle(ME);
}

#define WaitForManualEvent(ME) WaitForSingleObject(ME, INFINITE)
#define ResetManualEvent(ME) ResetEvent(ME)
#define SetManualEvent(ME) SetEvent(ME)

static void InitMutex(Mutex *pMutex)
{
    *pMutex = CreateMutex(NULL, FALSE, NULL);
	if (*pMutex == NULL)
		PrintSystemError("creating mutex");
}

static void FreeMutex(Mutex mutex)
{
    CloseHandle(mutex);
}

#ifdef DEBUG_MULTITHREADED
void Mutex_Lock(Mutex mutex, const char *reason)
{
	if (WaitForSingleObject(mutex, 0) == WAIT_OBJECT_0)
	{	/* Got mutex */
		multi_debug("%s: lock acquired", reason);
	}
	else
	{
		multi_debug("%s: waiting on lock", reason);
		WaitForSingleObject(mutex, INFINITE);
		multi_debug("lock relinquished");
	}
}
void Mutex_Release(Mutex mutex)
{
	multi_debug("Releasing lock");
	ReleaseMutex(mutex);
}
#else
#define Mutex_Lock(mutex, reason) WaitForSingleObject(mutex, INFINITE)
#define Mutex_Release(mutex) ReleaseMutex(mutex)
#endif

#endif

extern unsigned int MT_GetNumThreads(void)
{
    return td.numThreads;
}

static void CloseThread(void *UNUSED(unused))
{
	g_assert(td.closingThreads);
	MT_SafeInc(&td.result);
}

static void MT_CloseThreads(void)
{
	td.closingThreads = TRUE;
    mt_add_tasks(td.numThreads, CloseThread, NULL, NULL);
    if (MT_WaitForTasks(NULL, 0, FALSE) != (int)td.numThreads)
		g_print("Error closing threads!\n");
}

static void MT_TaskDone(Task *pt)
{
    MT_SafeInc(&td.doneTasks);

    if (pt)
    {
        free(pt->pLinkedTask);
        free(pt);
    }
}

static Task *MT_GetTask(void)
{
    Task *task = NULL;

    Mutex_Lock(td.queueLock, "get task");

    if (g_list_length(td.tasks) > 0)
    {
        task = (Task*)g_list_first(td.tasks)->data;
        td.tasks = g_list_delete_link(td.tasks, g_list_first(td.tasks));
        if (g_list_length(td.tasks) == 0)
        {
            ResetManualEvent(td.activity);
        }
    }

    multi_debug("get task: release");
    Mutex_Release(td.queueLock);

    return task;
}

extern void MT_AbortTasks(void)
{
    Task *task;
    /* Remove tasks from list */
    while ((task = MT_GetTask()) != NULL)
        MT_TaskDone(task);

	td.result = -1;
}

#ifdef GLIB_THREADS
static gpointer MT_WorkerThreadFunction(void *id)
#else
static void MT_WorkerThreadFunction(void *id)
#endif
{
	/* why do we need this align ? - because of a gcc bug */
#if __GNUC__ && defined(WIN32)
	/* Align stack pointer on 16 byte boundary so SSE variables work correctly */
	int align_offset;
	asm  __volatile__  ("andl $-16, %%esp" : : : "%esp");
	align_offset = ((int)(&align_offset)) % 16;
#endif
	{
		int *pID = (int*)id;
		TLSSetValue(td.tlsItem, *pID);
		free(pID);
		MT_SafeInc(&td.result);
		MT_TaskDone(NULL);    /* Thread created */
		do
		{
			Task *task;
			WaitForManualEvent(td.activity);
			task = MT_GetTask();
			if (task)
			{
				task->fun(task->data);
				MT_TaskDone(task);
			}
		} while (!td.closingThreads);

#ifdef GLIB_THREADS
#if __GNUC__ && defined(WIN32)
		/* De-align stack pointer to avoid crash on exit */
		asm  __volatile__  ("addl %0, %%esp" : : "r"(align_offset) : "%esp");
#endif
		return NULL;
#endif
	}
}

static gboolean WaitingForThreads(gpointer unused)
{	/* Unlikely to be called */
	multi_debug("Waiting for threads to be created!");
	return FALSE;
}

static void MT_CreateThreads(void)
{
    unsigned int i;
	multi_debug("CreateThreads()");
	td.result = 0;
	td.closingThreads = FALSE;
    for (i = 0; i < td.numThreads; i++)
    {
    	int *pID = (int*)malloc(sizeof(int));
    	*pID = i;
#ifdef GLIB_THREADS
        if (!g_thread_create(MT_WorkerThreadFunction, pID, FALSE, NULL))
#else
        if (_beginthread(MT_WorkerThreadFunction, 0, pID) == 0)
#endif
            printf("Failed to create thread\n");
    }
	td.addedTasks = td.numThreads;
	/* Wait for all the threads to be created (timeout after 1 second) */
	if (MT_WaitForTasks(WaitingForThreads, 1000, FALSE) != (int)td.numThreads)
		g_print("Error creating threads!\n");
}

void MT_SetNumThreads(unsigned int num)
{
	if (num != td.numThreads)
	{
		if (td.numThreads != 0)
			MT_CloseThreads();
		td.numThreads = num;
		MT_CreateThreads();
		if (num == 1)
		{	/* No locking in evals */
			EvaluatePosition = EvaluatePositionNoLocking;
			GeneralCubeDecisionE = GeneralCubeDecisionENoLocking;
			GeneralEvaluationE = GeneralEvaluationENoLocking;
			ScoreMove = ScoreMoveNoLocking;
			FindBestMove = FindBestMoveNoLocking;
			FindnSaveBestMoves = FindnSaveBestMovesNoLocking;
			BasicCubefulRollout = BasicCubefulRolloutNoLocking;
		}
		else
		{	/* Locking version of evals */
			EvaluatePosition = EvaluatePositionWithLocking;
			GeneralCubeDecisionE = GeneralCubeDecisionEWithLocking;
			GeneralEvaluationE = GeneralEvaluationEWithLocking;
			ScoreMove = ScoreMoveWithLocking;
			FindBestMove = FindBestMoveWithLocking;
			FindnSaveBestMoves = FindnSaveBestMovesWithLocking;
			BasicCubefulRollout = BasicCubefulRolloutWithLocking;
		}
	}
}

extern void MT_InitThreads(void)
{
	if (!g_thread_supported ())
		g_thread_init (NULL);
	g_assert(g_thread_supported());

    td.tasks = NULL;
	td.doneTasks = td.addedTasks = 0;
	td.totalTasks = -1;
    InitManualEvent(&td.activity);
    TLSCreate(&td.tlsItem);
	TLSSetValue(td.tlsItem, 0);	/* Main thread shares id 0 */
#ifdef DEBUG_MULTITHREADED
	mainThreadID = GetCurrentThreadId();
#endif
    InitMutex(&td.multiLock);
    InitMutex(&td.queueLock);
	InitManualEvent(&td.syncStart);
	InitManualEvent(&td.syncEnd);
#ifdef GLIB_THREADS
    if (condMutex == NULL)
        condMutex = g_mutex_new();
#endif
	td.numThreads = 0;
}

extern void MT_StartThreads(void)
{
    if (td.numThreads == 0)
	{
#ifdef TRY_COUNTING_PROCEESING_UNITS
        td.numThreads = GetLogicalProcssingUnitCount();
#else
        td.numThreads = 1;
#endif
		MT_CreateThreads();
	}
}

void MT_AddTask(Task *pt, gboolean lock)
{
	if (lock)
	{
		Mutex_Lock(td.queueLock, "add task");
	}
	if (td.addedTasks == 0)
		td.result = 0;	/* Reset result for new tasks */
	td.addedTasks++;
	td.tasks = g_list_append(td.tasks, pt);
	if (g_list_length(td.tasks) == 1)
	{    /* New tasks */
		SetManualEvent(td.activity);
	}
	if (lock)
	{
		multi_debug("add task: release");
		Mutex_Release(td.queueLock);
	}
}

void mt_add_tasks(unsigned int num_tasks, AsyncFun pFun, void *taskData, gpointer linked)
{
	unsigned int i;
	{
#ifdef DEBUG_MULTITHREADED
		char buf[20];
		sprintf(buf, "add %d tasks", num_tasks);
		Mutex_Lock(td.queueLock, buf);
#else
		Mutex_Lock(td.queueLock, NULL);
#endif
	}
	for (i = 0; i < num_tasks; i++)
	{
		Task *pt = (Task*)malloc(sizeof(Task));
		pt->fun = pFun;
		pt->data = taskData;
		pt->pLinkedTask = linked;
		MT_AddTask(pt, FALSE);
	}
	multi_debug("add many release: lock");
	Mutex_Release(td.queueLock);
}

static gboolean WaitForAllTasks(int time)
{
	int j=0;
	while (td.doneTasks != td.totalTasks)
	{
		if (j == 10)
			return FALSE;	/* Not done yet */

		j++;
		g_usleep(100*time);
	}
	return TRUE;
}

int MT_WaitForTasks(gboolean (*pCallback)(gpointer), int callbackTime, int autosave)
{
    int callbackLoops = callbackTime / UI_UPDATETIME;
    int waits = 0;
	int polltime = callbackLoops ? UI_UPDATETIME : callbackTime;
	guint as_source = 0;

	/* Set total tasks to wait for */
    td.totalTasks = td.addedTasks;
#if USE_GTK
	GTKSuspendInput();
#endif

	if (autosave)
		as_source = g_timeout_add(nAutoSaveTime*60000, save_autosave, NULL);
	multi_debug("Waiting for all tasks");
    while (!WaitForAllTasks(polltime))
    {
        waits++;
        if (pCallback && waits >= callbackLoops)
        {
            waits = 0;
            pCallback(NULL);
        }
	ProcessEvents();
	}
	if (autosave)
	{
		g_source_remove(as_source);
		save_autosave(NULL);
	}
	multi_debug("Done waiting for all tasks");

	td.doneTasks = td.addedTasks = 0;
	td.totalTasks = -1;

#if USE_GTK
	GTKResumeInput();
#endif
    return td.result;
}

extern void MT_SetResultFailed(void)
{
	td.result = -1;
}

extern void MT_Close(void)
{
    MT_CloseThreads();

    FreeManualEvent(td.activity);
    FreeMutex(td.multiLock);
    FreeMutex(td.queueLock);

	FreeManualEvent(td.syncStart);
	FreeManualEvent(td.syncEnd);
    TLSFree(td.tlsItem);
}

extern int MT_GetThreadID(void)
{
    return TLSGet(td.tlsItem);
}

extern void MT_Exclusive(void)
{
	Mutex_Lock(td.multiLock, "Exclusive lock");
}

extern void MT_Release(void)
{
	Mutex_Release(td.multiLock);
}

extern int MT_GetDoneTasks(void)
{
    return td.doneTasks;
}

/* Code below used in calibrate to try and get a resonable figure for multiple threads */

static double start; /* used for timekeeping */

extern void MT_SyncInit(void)
{
	ResetManualEvent(td.syncStart);
	ResetManualEvent(td.syncEnd);
}

extern void MT_SyncStart(void)
{
	static int count = 0;

	/* Wait for all threads to get here */
	if (MT_SafeIncValue(&count) == (int)td.numThreads)
	{
		count--;
		start = get_time();
		SetManualEvent(td.syncStart);
	}
	else
	{
		WaitForManualEvent(td.syncStart);
		if (MT_SafeDecCheck(&count))
			ResetManualEvent(td.syncStart);
	}
}

extern double MT_SyncEnd(void)
{
	static int count = 0;
	double now;

	/* Wait for all threads to get here */
	if (MT_SafeIncValue(&count) == (int)td.numThreads)
	{
		now = get_time();
		count--;
		SetManualEvent(td.syncEnd);
		return now - start;
	}
	else
	{
		WaitForManualEvent(td.syncEnd);
		if (MT_SafeDecCheck(&count))
			ResetManualEvent(td.syncEnd);
		return 0;
	}
}

#ifdef DEBUG_MULTITHREADED
void multi_debug(const char *str, ...)
{
	int id;
	va_list vl;
	char tn[4];
	char buf[1024];
	/* Sync output so order makes some sense */
#ifdef GLIB_THREADS
	g_mutex_lock(td.multiLock);
#else
	WaitForSingleObject(td.multiLock, INFINITE);
#endif

	va_start(vl, str);
	vsprintf(buf, str, vl);

	id = MT_GetThreadID();
	if (id == 0 && GetCurrentThreadId() == mainThreadID)
		strcpy(tn, "MT");
	else
		sprintf(tn, "T%d", id + 1);

	printf("%s: %s\n", tn, buf);

#ifdef GLIB_THREADS
	g_mutex_unlock(td.multiLock);
#else
	ReleaseMutex(td.multiLock);
#endif
}
#endif

#else
#include "multithread.h"
#include <stdlib.h>
#if USE_GTK
#include <gtk/gtk.h>
#include <gtkgame.h>
#endif

#define UI_UPDATETIME 250

typedef struct _ThreadData {
	int doneTasks;
       	GList * tasks;
	int result;
} ThreadData;
ThreadData td;

int asyncRet;
void MT_AddTask(Task *pt, gboolean lock)
{
	td.result = 0;		/* Reset result for new tasks */
	td.tasks = g_list_append(td.tasks, pt);
}

void mt_add_tasks(unsigned int num_tasks, AsyncFun pFun, void *taskData, gpointer linked)
{
	unsigned int i;
	for (i = 0; i < num_tasks; i++) {
		Task *pt = (Task *)malloc(sizeof(Task));
		pt->fun = pFun;
		pt->data = taskData;
		pt->pLinkedTask = linked;
		MT_AddTask(pt, FALSE);
	}
}

extern int MT_GetDoneTasks(void)
{
	return td.doneTasks;
}

int MT_WaitForTasks(gboolean(*pCallback) (gpointer), int callbackTime, int autosave)
{
	GList *member;
	guint cb_source = 0;
	guint  as_source = 0;
	td.doneTasks = 0;

#if USE_GTK
	GTKSuspendInput();
#endif

	multi_debug("Waiting for all tasks");

	pCallback(NULL);
	cb_source = g_timeout_add(1000, pCallback, NULL);
	if (autosave)
		as_source = g_timeout_add(nAutoSaveTime*60000, save_autosave, NULL);
	for (member = g_list_first(td.tasks); member; member = member->next, td.doneTasks++) {
		Task *task = member->data;
		task->fun(task->data);
		free(task->pLinkedTask);
		free(task);
		ProcessEvents();
	}
	g_list_free(td.tasks);
	if (autosave)
	{
		g_source_remove(as_source);
		save_autosave(NULL);
	}
	td.tasks = NULL;

#if USE_GTK
	GTKResumeInput();
#endif
	return td.result;
}

extern void MT_AbortTasks(void)
{
	td.result = -1;
}
#endif
