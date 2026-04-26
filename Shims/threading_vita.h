////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  threading_vita.h  —  Win32 threading API → POSIX/vitaSDK
//
//  The engine uses CreateThread, WaitForSingleObject, CreateEvent, etc.
//  vitaSDK provides pthreads + SceKernel semaphores.
//  This header maps Win32 threading calls to portable equivalents.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _THREADING_VITA_H_
#define _THREADING_VITA_H_

#ifdef VITA

#include <pthread.h>
#include <semaphore.h>
#include <psp2/kernel/threadmgr.h>
#include <stdlib.h>
#include <string.h>

// ── HANDLE types ─────────────────────────────────────────────────────────
// On Win32 HANDLE is void*; we alias thread/event/mutex handles as opaque.
// Already defined in PSVitaSpecific.h as void*.

// ── WaitForSingleObject timeout constants ────────────────────────────────
#define WAIT_OBJECT_0       0x00000000
#define WAIT_TIMEOUT        0x00000102
#define INFINITE            0xFFFFFFFF

// ── Internal helpers ─────────────────────────────────────────────────────
struct VitaEvent {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    volatile int    signalled;
    int             manual_reset;
};

inline HANDLE CreateEventA(void* /*sec*/, BOOL manual_reset, BOOL init, const char* /*name*/)
{
    VitaEvent* ev = (VitaEvent*)malloc(sizeof(VitaEvent));
    pthread_mutex_init(&ev->mutex, NULL);
    pthread_cond_init(&ev->cond, NULL);
    ev->signalled    = init ? 1 : 0;
    ev->manual_reset = manual_reset;
    return (HANDLE)ev;
}
#define CreateEvent(sec,mr,init,name) CreateEventA(sec,mr,init,name)

inline BOOL SetEvent(HANDLE h)
{
    VitaEvent* ev = (VitaEvent*)h;
    pthread_mutex_lock(&ev->mutex);
    ev->signalled = 1;
    pthread_cond_broadcast(&ev->cond);
    pthread_mutex_unlock(&ev->mutex);
    return TRUE;
}

inline BOOL ResetEvent(HANDLE h)
{
    VitaEvent* ev = (VitaEvent*)h;
    pthread_mutex_lock(&ev->mutex);
    ev->signalled = 0;
    pthread_mutex_unlock(&ev->mutex);
    return TRUE;
}

inline DWORD WaitForSingleObject(HANDLE h, DWORD ms)
{
    VitaEvent* ev = (VitaEvent*)h;
    pthread_mutex_lock(&ev->mutex);
    if (ms == INFINITE) {
        while (!ev->signalled)
            pthread_cond_wait(&ev->cond, &ev->mutex);
    } else {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec  += ms / 1000;
        ts.tv_nsec += (ms % 1000) * 1000000;
        if (ts.tv_nsec >= 1000000000) { ts.tv_sec++; ts.tv_nsec -= 1000000000; }
        while (!ev->signalled) {
            if (pthread_cond_timedwait(&ev->cond, &ev->mutex, &ts) != 0) {
                pthread_mutex_unlock(&ev->mutex);
                return WAIT_TIMEOUT;
            }
        }
    }
    if (!ev->manual_reset) ev->signalled = 0;
    pthread_mutex_unlock(&ev->mutex);
    return WAIT_OBJECT_0;
}

inline DWORD WaitForMultipleObjects(DWORD n, const HANDLE* h, BOOL all, DWORD ms)
{
    // Simplified: wait on first object only (engine mostly uses 1-2 events)
    for (DWORD i = 0; i < n; ++i) {
        DWORD r = WaitForSingleObject(h[i], ms);
        if (r == WAIT_OBJECT_0) return WAIT_OBJECT_0 + i;
    }
    return WAIT_TIMEOUT;
}

inline BOOL CloseHandleEvent(HANDLE h)
{
    VitaEvent* ev = (VitaEvent*)h;
    pthread_mutex_destroy(&ev->mutex);
    pthread_cond_destroy(&ev->cond);
    free(ev);
    return TRUE;
}

// ── CreateThread → pthread_create ────────────────────────────────────────
struct VitaThread {
    pthread_t       thread;
    void*           retval;
};

typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID);

struct VitaThreadArgs {
    LPTHREAD_START_ROUTINE fn;
    void*                  arg;
};

inline void* vita_thread_trampoline(void* arg)
{
    VitaThreadArgs* a = (VitaThreadArgs*)arg;
    LPTHREAD_START_ROUTINE fn = a->fn;
    void* user_arg = a->arg;
    free(a);
    DWORD ret = fn(user_arg);
    return (void*)(intptr_t)ret;
}

inline HANDLE CreateThread(void* /*sec*/, SIZE_T /*stack*/,
                             LPTHREAD_START_ROUTINE fn, LPVOID arg,
                             DWORD /*flags*/, DWORD* /*id*/)
{
    VitaThread* t = (VitaThread*)malloc(sizeof(VitaThread));
    VitaThreadArgs* a = (VitaThreadArgs*)malloc(sizeof(VitaThreadArgs));
    a->fn  = fn;
    a->arg = arg;
    if (pthread_create(&t->thread, NULL, vita_thread_trampoline, a) != 0) {
        free(a); free(t); return NULL;
    }
    return (HANDLE)t;
}

// ── Mutex ────────────────────────────────────────────────────────────────
inline HANDLE CreateMutexA(void* /*sec*/, BOOL owned, const char* /*name*/)
{
    pthread_mutex_t* m = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m, NULL);
    if (owned) pthread_mutex_lock(m);
    return (HANDLE)m;
}
#define CreateMutex(sec,own,name) CreateMutexA(sec,own,name)

inline BOOL ReleaseMutex(HANDLE h)
{
    pthread_mutex_unlock((pthread_mutex_t*)h);
    return TRUE;
}

// ── Critical Section ─────────────────────────────────────────────────────
typedef pthread_mutex_t CRITICAL_SECTION;
inline void InitializeCriticalSection(CRITICAL_SECTION* cs) { pthread_mutex_init(cs, NULL); }
inline void DeleteCriticalSection(CRITICAL_SECTION* cs)     { pthread_mutex_destroy(cs); }
inline void EnterCriticalSection(CRITICAL_SECTION* cs)      { pthread_mutex_lock(cs); }
inline void LeaveCriticalSection(CRITICAL_SECTION* cs)      { pthread_mutex_unlock(cs); }
inline BOOL TryEnterCriticalSection(CRITICAL_SECTION* cs)   { return pthread_mutex_trylock(cs) == 0; }

// ── Thread local storage ─────────────────────────────────────────────────
typedef pthread_key_t  DWORD_TLS;
#define TlsAlloc()              ({ pthread_key_t k; pthread_key_create(&k,NULL); (DWORD)(uintptr_t)k; })
#define TlsFree(idx)            pthread_key_delete((pthread_key_t)(uintptr_t)(idx))
#define TlsGetValue(idx)        pthread_getspecific((pthread_key_t)(uintptr_t)(idx))
#define TlsSetValue(idx,val)    pthread_setspecific((pthread_key_t)(uintptr_t)(idx),(val))

// ── Interlocked ops (already in PSVitaSpecific.h as __sync_*) ───────────
// Redeclare as inline to match WINAPI signature expectations
inline LONG InterlockedAdd(volatile LONG* p, LONG v) { return __sync_fetch_and_add(p,v)+v; }

#endif // VITA
#endif // _THREADING_VITA_H_
