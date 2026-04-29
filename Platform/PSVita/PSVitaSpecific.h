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
#include <psp2/audioout.h>
#include <psp2/audioin.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/rtc.h>
#include <psp2/power.h>

// Standard POSIX (vitaSDK provides a POSIX compatibility layer)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
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

// ── Engine numeric types (like Linux32Specific.h) ────────────────────────
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;
typedef signed long long    INT64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef float               f32;
typedef double              f64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

// ── Heap validity stub ────────────────────────────────────────────────────
#ifdef __cplusplus
#  ifndef _ISHEAPVALID_DEFINED
#  define _ISHEAPVALID_DEFINED
inline int IsHeapValid() { return 1; }
#  endif
#endif

// ── MSVC-isms mapped to GCC/Clang equivalents ────────────────────────────
#ifndef __forceinline
#  define __forceinline  __attribute__((always_inline)) inline
#endif
#ifndef _inline
#  define _inline        inline
#endif
#ifndef __cdecl
#  define __cdecl
#endif
#ifndef __stdcall
#  define __stdcall
#endif
#ifndef __fastcall
#  define __fastcall
#endif
// __declspec is defined as empty only if not already handled
#ifndef __declspec
#  define __declspec(x)
#endif
#ifndef IN
#  define IN
#endif
#ifndef OUT
#  define OUT
#endif

// ── Windows primitive types ──────────────────────────────────────────────
#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef UCHAR
typedef unsigned char       UCHAR;
#endif
#ifndef WORD
typedef unsigned short      WORD;
#endif
#ifndef DWORD
typedef unsigned int        DWORD;
#endif
#ifndef LPDWORD
typedef unsigned int*       LPDWORD;
#endif
#ifndef DWORD64
typedef unsigned long long  DWORD64;
#endif
#ifndef LONG
typedef signed int          LONG;
#endif
#ifndef ULONG
typedef unsigned int        ULONG;
#endif
#ifndef ULONGLONG
typedef unsigned long long  ULONGLONG;
#endif
#ifndef LONGLONG
typedef long long           LONGLONG;
#endif
#ifndef LPVOID
typedef void*               LPVOID;
#endif
#ifndef LPCVOID
typedef const void*         LPCVOID;
#endif
#ifndef BOOL
typedef int                 BOOL;
#endif
#ifndef HRESULT
typedef int                 HRESULT;
#endif
#ifndef HANDLE
typedef void*               HANDLE;
#endif
#ifndef HWND
typedef void*               HWND;
#endif
#ifndef HMODULE
typedef void*               HMODULE;
#endif
#ifndef HINSTANCE
typedef void*               HINSTANCE;
#endif
#ifndef LPSTR
typedef char*               LPSTR;
#endif
#ifndef LPCSTR
typedef const char*         LPCSTR;
#endif
#ifndef WCHAR
typedef wchar_t             WCHAR;
#endif
#ifndef LPWSTR
typedef WCHAR*              LPWSTR;
#endif
#ifndef LPCWSTR
typedef const WCHAR*        LPCWSTR;
#endif
#ifndef UINT
typedef unsigned int        UINT;
#endif
#ifndef INT
typedef int                 INT;
#endif
#ifndef FLOAT
typedef float               FLOAT;
#endif
#ifndef DOUBLE
typedef double              DOUBLE;
#endif
#ifndef SIZE_T
typedef size_t              SIZE_T;
#endif
#ifndef INT_PTR
typedef intptr_t            INT_PTR;
#endif
#ifndef UINT_PTR
typedef uintptr_t           UINT_PTR;
#endif

#ifndef VOID
#  define VOID              void
#endif
#ifndef PVOID
#  define PVOID             void*
#endif

#ifndef TRUE
#  define TRUE              1
#endif
#ifndef FALSE
#  define FALSE             0
#endif
#ifndef S_OK
#  define S_OK              ((HRESULT)0)
#endif
#ifndef S_FALSE
#  define S_FALSE           ((HRESULT)1)
#endif
#ifndef E_FAIL
#  define E_FAIL            ((HRESULT)0x80004005L)
#endif
#ifndef E_INVALIDARG
#  define E_INVALIDARG      ((HRESULT)0x80070057L)
#endif
#ifndef E_NOTIMPL
#  define E_NOTIMPL         ((HRESULT)0x80004001L)
#endif
#ifndef E_OUTOFMEMORY
#  define E_OUTOFMEMORY     ((HRESULT)0x8007000EL)
#endif
#ifndef SUCCEEDED
#  define SUCCEEDED(hr)     (((HRESULT)(hr)) >= 0)
#endif
#ifndef FAILED
#  define FAILED(hr)        (((HRESULT)(hr)) < 0)
#endif

#ifndef MAX_PATH
#  define MAX_PATH          260
#endif
#ifndef _MAX_PATH
#  define _MAX_PATH         MAX_PATH
#endif

// ── Pointer-size types ───────────────────────────────────────────────────
#ifndef DWORD_PTR
typedef uintptr_t           DWORD_PTR;
#endif
#ifndef LONG_PTR
typedef intptr_t            LONG_PTR;
#endif
#ifndef ULONG_PTR
typedef uintptr_t           ULONG_PTR;
#endif

// ── Bit / byte helpers (replicate windows.h macros) ─────────────────────
#ifndef MAKEWORD
#  define MAKEWORD(a,b)     ((WORD)(((BYTE)((DWORD_PTR)(a)&0xff))|((WORD)((BYTE)((DWORD_PTR)(b)&0xff)))<<8))
#endif
#ifndef MAKELONG
#  define MAKELONG(a,b)     ((LONG)(((WORD)((DWORD_PTR)(a)&0xffff))|((DWORD)((WORD)((DWORD_PTR)(b)&0xffff)))<<16))
#endif
#ifndef LOWORD
#  define LOWORD(l)         ((WORD)((DWORD_PTR)(l)&0xffff))
#endif
#ifndef HIWORD
#  define HIWORD(l)         ((WORD)((DWORD_PTR)(l)>>16))
#endif
#ifndef LOBYTE
#  define LOBYTE(w)         ((BYTE)((DWORD_PTR)(w)&0xff))
#endif
#ifndef HIBYTE
#  define HIBYTE(w)         ((BYTE)((DWORD_PTR)(w)>>8))
#endif
#ifndef MAKEFOURCC
#  define MAKEFOURCC(a,b,c,d) ((DWORD)(BYTE)(a)|((DWORD)(BYTE)(b)<<8)|((DWORD)(BYTE)(c)<<16)|((DWORD)(BYTE)(d)<<24))
#endif

// ── Calling convention / DLL export stubs ────────────────────────────────
#ifndef CALLBACK
#  define CALLBACK
#endif
#ifndef WINAPI
#  define WINAPI
#endif
#ifndef APIENTRY
#  define APIENTRY
#endif

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
#ifndef THREAD_HANDLE
typedef pthread_t           THREAD_HANDLE;
#endif
#ifndef EVENT_HANDLE
typedef pthread_t           EVENT_HANDLE;
#endif

// ── Socket compatibility ─────────────────────────────────────────────────
#ifndef SOCKET
#  define SOCKET            int
#endif
#ifndef INVALID_SOCKET
#  define INVALID_SOCKET    (-1)
#endif
#ifndef SOCKET_ERROR
#  define SOCKET_ERROR      (-1)
#endif

// ── Windows time/file structs ────────────────────────────────────────────
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME;

typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay;
    WORD wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME;

// ── BMP structs (used by debug texture dumpers) ──────────────────────────
#ifndef BITMAPFILEHEADER
typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;
#define BITMAPFILEHEADER BITMAPFILEHEADER
#endif
#ifndef BITMAPINFOHEADER
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;
#define BITMAPINFOHEADER BITMAPINFOHEADER
#endif

// ── File attribute stubs ─────────────────────────────────────────────────
#ifndef FILE_ATTRIBUTE_NORMAL
#  define FILE_ATTRIBUTE_NORMAL     0x00000080
#endif
#ifndef FILE_ATTRIBUTE_DIRECTORY
#  define FILE_ATTRIBUTE_DIRECTORY  0x00000010
#endif
#ifndef INVALID_HANDLE_VALUE
#  define INVALID_HANDLE_VALUE      ((HANDLE)(-1))
#endif

// ── Atomic / interlocked stubs ───────────────────────────────────────────
#ifndef InterlockedIncrement
#  define InterlockedIncrement(p)   __sync_fetch_and_add(p, 1)
#endif
#ifndef InterlockedDecrement
#  define InterlockedDecrement(p)   __sync_fetch_and_sub(p, 1)
#endif
#ifndef InterlockedExchange
#  define InterlockedExchange(p,v)  __sync_lock_test_and_set(p, v)
#endif
#ifndef InterlockedCompareExchange
#  define InterlockedCompareExchange(p,v,c) __sync_val_compare_and_swap(p, c, v)
#endif

// ── CPU / SIMD ────────────────────────────────────────────────────────────
// Vita has ARM Cortex-A9 with NEON; no SSE/MMX/3DNow
#ifndef _CPU_ARM
#  define _CPU_ARM
#endif
#undef  CPUF_SSE
#undef  CPUF_SSE2
#undef  CPUF_3DNOW
#undef  CPUF_MMX

// ── MSVC built-in min/max ────────────────────────────────────────────────
#ifndef __min
#  define __min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef __max
#  define __max(a,b) ((a)>(b)?(a):(b))
#endif
// Global min/max — pulled from std:: (windows.h normally injects these)
#ifdef __cplusplus
#  include <algorithm>
   using std::min;
   using std::max;
#endif

// ── Math helpers ─────────────────────────────────────────────────────────
#ifndef _isnan
#  define _isnan(x)  isnan(x)
#endif
#ifndef _finite
#  define _finite(x) isfinite(x)
#endif

// ── Debug break ──────────────────────────────────────────────────────────
#ifndef DEBUG_BREAK
#  define DEBUG_BREAK       __builtin_trap()
#endif

// ── Platform string for logging ──────────────────────────────────────────
#ifndef CRY_PLATFORM_STR
#  define CRY_PLATFORM_STR  "PS Vita"
#endif

#endif // _CRY_COMMON_PSVITA_SPECIFIC_HDR_
