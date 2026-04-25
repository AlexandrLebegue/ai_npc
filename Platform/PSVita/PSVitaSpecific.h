////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  PSVitaSpecific.h  —  Platform abstraction layer for PS Vita (PSP2)
//
//  Modelled after CryCommon/LinuxSpecific.h.
//  Included automatically by CryCommon/platform.h when VITA is defined.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _CRY_COMMON_PSVITA_SPECIFIC_HDR_
#define _CRY_COMMON_PSVITA_SPECIFIC_HDR_

// ── vitaSDK system headers ────────────────────────────────────────────────
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/audio.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/rtc.h>
#include <psp2/power.h>

// Standard POSIX (vitaSDK provides a POSIX compatibility layer)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>

#ifdef __cplusplus
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#endif

// ── Heap validity stub ────────────────────────────────────────────────────
#ifdef __cplusplus
inline int IsHeapValid() { return 1; }
#endif

// ── MSVC-isms mapped to GCC/Clang equivalents ────────────────────────────
#define __forceinline       __attribute__((always_inline)) inline
#define _inline             inline
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
#define IN
#define OUT

// ── Windows primitive types ──────────────────────────────────────────────
typedef unsigned char       BYTE;
typedef unsigned char       UCHAR;
typedef unsigned short      WORD;
typedef unsigned int        DWORD;
typedef unsigned int*       LPDWORD;
typedef unsigned long long  DWORD64;
typedef signed int          LONG;
typedef unsigned int        ULONG;
typedef unsigned long long  ULONGLONG;
typedef long long           LONGLONG;
typedef void*               LPVOID;
typedef const void*         LPCVOID;
typedef int                 BOOL;
typedef int                 HRESULT;
typedef void*               HANDLE;
typedef void*               HWND;
typedef void*               HMODULE;
typedef void*               HINSTANCE;
typedef char*               LPSTR;
typedef const char*         LPCSTR;
typedef wchar_t             WCHAR;
typedef WCHAR*              LPWSTR;
typedef const WCHAR*        LPCWSTR;
typedef unsigned int        UINT;
typedef int                 INT;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef size_t              SIZE_T;
typedef intptr_t            INT_PTR;
typedef uintptr_t           UINT_PTR;

#define VOID                void
#define PVOID               void*

#define TRUE                1
#define FALSE               0
#define S_OK                ((HRESULT)0)
#define S_FALSE             ((HRESULT)1)
#define E_FAIL              ((HRESULT)0x80004005L)
#define E_INVALIDARG        ((HRESULT)0x80070057L)
#define E_NOTIMPL           ((HRESULT)0x80004001L)
#define E_OUTOFMEMORY       ((HRESULT)0x8007000EL)
#define SUCCEEDED(hr)       (((HRESULT)(hr)) >= 0)
#define FAILED(hr)          (((HRESULT)(hr)) < 0)

#ifndef MAX_PATH
#  define MAX_PATH          260
#endif
#ifndef _MAX_PATH
#  define _MAX_PATH         MAX_PATH
#endif

// ── Bit / byte helpers (replicate windows.h macros) ─────────────────────
#define MAKEWORD(a,b)       ((WORD)(((BYTE)((DWORD_PTR)(a)&0xff))|((WORD)((BYTE)((DWORD_PTR)(b)&0xff)))<<8))
#define MAKELONG(a,b)       ((LONG)(((WORD)((DWORD_PTR)(a)&0xffff))|((DWORD)((WORD)((DWORD_PTR)(b)&0xffff)))<<16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l)&0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l)>>16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w)&0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w)>>8))
#define MAKEFOURCC(a,b,c,d) ((DWORD)(BYTE)(a)|((DWORD)(BYTE)(b)<<8)|((DWORD)(BYTE)(c)<<16)|((DWORD)(BYTE)(d)<<24))

// ── Pointer-size types ───────────────────────────────────────────────────
typedef uintptr_t           DWORD_PTR;
typedef intptr_t            LONG_PTR;
typedef uintptr_t           ULONG_PTR;

// ── Calling convention / DLL export stubs ────────────────────────────────
#define CALLBACK
#define WINAPI
#define APIENTRY

// ── Safe-release macros ──────────────────────────────────────────────────
#ifndef SAFE_DELETE
#  define SAFE_DELETE(p)          { if(p){ delete (p);    (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#  define SAFE_DELETE_ARRAY(p)    { if(p){ delete[] (p);  (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#  define SAFE_RELEASE(p)         { if(p){ (p)->Release();(p)=NULL; } }
#endif
#ifndef SAFE_RELEASE_FORCE
#  define SAFE_RELEASE_FORCE(p)   { if(p){ (p)->Release(1);(p)=NULL; } }
#endif

// ── Assert override ──────────────────────────────────────────────────────
#undef  assert
#define assert(exp) \
    (void)( (exp) || (printf("[ASSERT] %s:%d  '%s'\n", __FILE__, __LINE__, #exp), 0) )

// ── Thread primitives (pthreads via vitaSDK) ─────────────────────────────
typedef pthread_t           THREAD_HANDLE;
typedef pthread_t           EVENT_HANDLE;  // approximation — use semaphores if needed

// ── Socket compatibility ─────────────────────────────────────────────────
#define SOCKET              int
#define INVALID_SOCKET      (-1)
#define SOCKET_ERROR        (-1)

// ── File attribute stubs ─────────────────────────────────────────────────
#define FILE_ATTRIBUTE_NORMAL       0x00000080
#define FILE_ATTRIBUTE_DIRECTORY    0x00000010
#define INVALID_HANDLE_VALUE        ((HANDLE)(-1))

// ── Atomic / interlocked stubs (single-threaded safe approximations) ──────
#define InterlockedIncrement(p)     __sync_fetch_and_add(p, 1)
#define InterlockedDecrement(p)     __sync_fetch_and_sub(p, 1)
#define InterlockedExchange(p,v)    __sync_lock_test_and_set(p, v)
#define InterlockedCompareExchange(p,v,c) __sync_val_compare_and_swap(p, c, v)

// ── CPU / SIMD ────────────────────────────────────────────────────────────
// Vita has ARM Cortex-A9 with NEON; no SSE/MMX/3DNow
#define _CPU_ARM
#undef  CPUF_SSE
#undef  CPUF_SSE2
#undef  CPUF_3DNOW
#undef  CPUF_MMX

// ── Math helpers ─────────────────────────────────────────────────────────
#ifndef _isnan
#  define _isnan(x)  isnan(x)
#endif
#ifndef _finite
#  define _finite(x) isfinite(x)
#endif

// ── Debug break ──────────────────────────────────────────────────────────
#define DEBUG_BREAK         __builtin_trap()

// ── Platform string for logging ──────────────────────────────────────────
#define CRY_PLATFORM_STR    "PS Vita"

#endif // _CRY_COMMON_PSVITA_SPECIFIC_HDR_
