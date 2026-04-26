////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  win32_vita.h  —  Win32 API stubs for non-Windows platforms
//
//  Include this BEFORE any engine header that pulls in <windows.h>.
//  On Vita the pre-processor sees VITA so windows.h is never included;
//  this file provides the subset of Win32 the engine actually calls.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _WIN32_VITA_SHIM_H_
#define _WIN32_VITA_SHIM_H_

#ifdef VITA

// PSVitaSpecific.h already pulled in from Platform/PSVita/
#include <PSVitaSpecific.h>

// ── String functions (MSVC names → POSIX) ────────────────────────────────
#include <string.h>
#include <stdio.h>
#define _stricmp        strcasecmp
#define _strnicmp       strncasecmp
#define stricmp         strcasecmp
#define strnicmp        strncasecmp
#define _snprintf       snprintf
#define _vsnprintf      vsnprintf
#define _isnan          isnan
#define _finite         isfinite
#define _itoa(v,b,r)    (sprintf((b),"%d",(v)),(b))
#define _ltoa(v,b,r)    (sprintf((b),"%ld",(v)),(b))
#define _ultoa(v,b,r)   (sprintf((b),"%lu",(v)),(b))
#define _i64toa(v,b,r)  (sprintf((b),"%lld",(v)),(b))
#define _atoi64         atoll

// ── Memory functions ─────────────────────────────────────────────────────
#define _alloca         alloca
#define ZeroMemory(p,n) memset((p),0,(n))
#define FillMemory(p,n,v) memset((p),(v),(n))
#define CopyMemory(d,s,n) memcpy((d),(s),(n))
#define MoveMemory(d,s,n) memmove((d),(s),(n))

// ── Math ─────────────────────────────────────────────────────────────────
#include <math.h>
#define _hypot          hypot

// ── File I/O ─────────────────────────────────────────────────────────────
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
// The engine calls CreateFile / ReadFile / WriteFile — map to POSIX fd
#define GENERIC_READ             0x80000000
#define GENERIC_WRITE            0x40000000
#define OPEN_EXISTING            3
#define CREATE_ALWAYS            2
#define FILE_SHARE_READ          1
#define FILE_SHARE_WRITE         2
#define FILE_FLAG_OVERLAPPED     0x40000000
#define FILE_FLAG_NO_BUFFERING   0x20000000
#define INVALID_FILE_SIZE        0xFFFFFFFF
#define FILE_ATTRIBUTE_NORMAL    0x80
#define FILE_ATTRIBUTE_DIRECTORY 0x10

inline HANDLE VitaCreateFile(const char* path, DWORD access, DWORD /*share*/,
                               void* /*sec*/, DWORD disp, DWORD /*flags*/, HANDLE /*tmpl*/)
{
    int flags = 0;
    if ((access & GENERIC_READ)  && (access & GENERIC_WRITE)) flags = SCE_O_RDWR;
    else if (access & GENERIC_WRITE) flags = SCE_O_WRONLY;
    else flags = SCE_O_RDONLY;
    if (disp == CREATE_ALWAYS) flags |= SCE_O_CREAT | SCE_O_TRUNC;
    SceUID fd = sceIoOpen(path, flags, 0666);
    return (fd >= 0) ? (HANDLE)(intptr_t)fd : INVALID_HANDLE_VALUE;
}
#define CreateFile(p,a,s,sc,d,f,t)  VitaCreateFile(p,a,s,sc,d,f,t)

// ReadFile / WriteFile / GetFileSize are intentionally NOT macros to avoid
// conflicting with engine class member functions that share the same names.
// Use free functions in global scope — they coexist with class methods.
inline BOOL VitaReadFile(HANDLE h, void* buf, DWORD bytes, DWORD* read, void*)
{
    SceSSize r = sceIoRead((SceUID)(intptr_t)h, buf, bytes);
    if (read) *read = (r >= 0) ? (DWORD)r : 0;
    return r >= 0;
}
inline BOOL ReadFile(HANDLE h, void* buf, DWORD bytes, DWORD* read, void* ovl)
{
    return VitaReadFile(h, buf, bytes, read, ovl);
}

inline BOOL VitaWriteFile(HANDLE h, const void* buf, DWORD bytes, DWORD* written, void*)
{
    SceSSize w = sceIoWrite((SceUID)(intptr_t)h, buf, bytes);
    if (written) *written = (w >= 0) ? (DWORD)w : 0;
    return w >= 0;
}
inline BOOL WriteFile(HANDLE h, const void* buf, DWORD bytes, DWORD* written, void* ovl)
{
    return VitaWriteFile(h, buf, bytes, written, ovl);
}

inline BOOL VitaCloseHandle(HANDLE h)
{
    return sceIoClose((SceUID)(intptr_t)h) >= 0;
}
#define CloseHandle(h) VitaCloseHandle(h)

inline DWORD VitaGetFileSize(HANDLE h, DWORD* high)
{
    SceIoStat st;
    if (sceIoGetstatByFd((SceUID)(intptr_t)h, &st) < 0) return 0xFFFFFFFF;
    if (high) *high = (DWORD)(st.st_size >> 32);
    return (DWORD)(st.st_size & 0xFFFFFFFF);
}
inline DWORD GetFileSize(HANDLE h, DWORD* high)
{
    return VitaGetFileSize(h, high);
}

// ── Timing ───────────────────────────────────────────────────────────────
#include <psp2/rtc.h>
inline BOOL VitaQueryPerformanceCounter(LONGLONG* out)
{
    SceRtcTick t;  sceRtcGetCurrentTick(&t);
    *out = (LONGLONG)t.tick;  return TRUE;
}
inline BOOL VitaQueryPerformanceFrequency(LONGLONG* out)
{
    *out = sceRtcGetTickResolution();  return TRUE;
}
#define QueryPerformanceCounter(p)   VitaQueryPerformanceCounter((LONGLONG*)(p))
#define QueryPerformanceFrequency(p) VitaQueryPerformanceFrequency((LONGLONG*)(p))

inline DWORD VitaGetTickCount()
{
    SceRtcTick t;  sceRtcGetCurrentTick(&t);
    return (DWORD)(t.tick / (sceRtcGetTickResolution() / 1000));
}
#define GetTickCount() VitaGetTickCount()

inline void VitaSleep(DWORD ms) { sceKernelDelayThread(ms * 1000); }
#define Sleep(ms) VitaSleep(ms)

// ── LARGE_INTEGER ─────────────────────────────────────────────────────────
typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG HighPart; };
    struct { DWORD LowPart; LONG HighPart; } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

// ── Threading stubs ──────────────────────────────────────────────────────
#include <pthread.h>
#define GetCurrentThreadId()  ((DWORD)(uintptr_t)pthread_self())
#define WaitForSingleObjectEx(h,ms,alert) WaitForSingleObject((h),(ms))
#define SleepEx(ms,alert)     Sleep(ms)

// ── Module loading (everything is statically linked on Vita) ─────────────
#define LoadLibraryA(path)        ((HMODULE)1)
#define LoadLibraryW(path)        ((HMODULE)1)
#define FreeLibrary(h)            (TRUE)
#define GetProcAddress(h,name)    ((void*)NULL)
#define GetLastError()            (0)
inline void SetLastError(DWORD) {}

// ── OutputDebugString ────────────────────────────────────────────────────
#define OutputDebugStringA(s)     printf("%s", (s))
#define OutputDebugStringW(s)     /* wide strings not needed on Vita */
#define OutputDebugString(s)      OutputDebugStringA(s)

// ── File deletion stubs ──────────────────────────────────────────────────
inline BOOL VitaDeleteFile(const char* path) { return sceIoRemove(path) >= 0; }
#define DeleteFile(p)       VitaDeleteFile(p)
inline BOOL VitaRemoveDirectory(const char* path) { return sceIoRmdir(path) >= 0; }
#define RemoveDirectory(p)  VitaRemoveDirectory(p)
inline BOOL VitaMoveFile(const char* src, const char* dst) { return sceIoRename(src, dst) >= 0; }
#define MoveFile(s,d)       VitaMoveFile(s,d)
inline BOOL VitaCreateDirectory(const char* path, void*) { return sceIoMkdir(path, 0755) >= 0; }
#define CreateDirectory(p,s) VitaCreateDirectory(p,s)

// ── Windows time conversions ──────────────────────────────────────────────
inline BOOL SystemTimeToFileTime(const SYSTEMTIME* st, FILETIME* ft)
{
    // Approximate: convert SYSTEMTIME to 100-nanosecond intervals since 1601
    // For PS Vita, we just return a zero timestamp as we don't need precise times
    ft->dwLowDateTime  = 0;
    ft->dwHighDateTime = 0;
    return TRUE;
}

// ── memicmp (case-insensitive memory compare) ─────────────────────────────
#define memicmp(a,b,n) strncasecmp((const char*)(a),(const char*)(b),(n))

// ── Multimedia timer stub ─────────────────────────────────────────────────
#define timeGetTime()  VitaGetTickCount()

// ── Process/module handles ────────────────────────────────────────────────
typedef HANDLE HMODULE;
typedef HANDLE HINSTANCE;
#define GetModuleHandle(n)    ((HMODULE)0)
#define GetModuleFileName(h,b,s) (0u)

// ── Misc Windows API stubs ───────────────────────────────────────────────
#define IsDebuggerPresent()       (0)
#define DebugBreak()              __builtin_trap()

// GetSystemInfo — report Vita CPU specs
struct SYSTEM_INFO {
    DWORD  dwNumberOfProcessors;
    DWORD  dwPageSize;
    DWORD  dwProcessorType;
};
inline void GetSystemInfo(SYSTEM_INFO* si)
{
    si->dwNumberOfProcessors = 4;   // Cortex-A9 quad-core
    si->dwPageSize           = 4096;
    si->dwProcessorType      = 0;
}

// GlobalMemoryStatus — report Vita RAM
struct MEMORYSTATUS {
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
};
inline void GlobalMemoryStatus(MEMORYSTATUS* ms)
{
    ms->dwTotalPhys = 512 * 1024 * 1024;  // 512 MB
    ms->dwAvailPhys = 384 * 1024 * 1024;  // approximate
}

// Registry stubs (engine reads from registry on Win32; we return failure)
#define RegOpenKeyExA(...)      (1)   // ERROR_SUCCESS = 0; 1 = failure
#define RegQueryValueExA(...)   (1)
#define RegCloseKey(...)        (0)

// ── Async I/O stubs (not available on Vita) ──────────────────────────────
#define CancelIo(h)               (TRUE)
#define SetFilePointer(h,d,dh,m)  ((DWORD)sceIoLseek((SceUID)(intptr_t)(h),(d),(m)))
#define ReadFileEx(h,b,n,o,cb)    (FALSE)
#define GetOverlappedResult(h,o,n,w) (FALSE)
#define FILE_BEGIN   SCE_SEEK_SET
#define FILE_CURRENT SCE_SEEK_CUR
#define FILE_END     SCE_SEEK_END

// ── Win32 error codes ─────────────────────────────────────────────────────
// Note: ERROR_OUT_OF_MEMORY is intentionally NOT defined here because
// IStreamEngine.h defines it as an engine-specific enum value (0xF0000009).
#ifndef ERROR_SUCCESS
#  define ERROR_SUCCESS               0
#endif
#ifndef ERROR_FILE_NOT_FOUND
#  define ERROR_FILE_NOT_FOUND        2
#endif
#ifndef ERROR_PATH_NOT_FOUND
#  define ERROR_PATH_NOT_FOUND        3
#endif

// ── Overlapped I/O stubs (async file I/O — not available on Vita) ────────
#ifndef OVERLAPPED
typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    DWORD     Offset;
    DWORD     OffsetHigh;
    HANDLE    hEvent;
    void*     pCaller;  // Linux/Vita extension used by RefReadStreamProxy
} OVERLAPPED, *LPOVERLAPPED;
#define OVERLAPPED OVERLAPPED
#endif

// MessageBox / dialog stubs
#define MB_OK       0
#define MB_YESNO    4
#define MB_ICONERROR  0x10
#define IDOK        1
#define IDYES       6
#define IDNO        7
#define MessageBoxA(h,t,c,f)    (printf("[MSG] %s: %s\n",(c),(t)), IDOK)

#endif // VITA
#endif // _WIN32_VITA_SHIM_H_
