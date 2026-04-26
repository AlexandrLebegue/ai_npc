////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  memory_vita.h  —  Win32 memory API → standard C / POSIX
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _MEMORY_VITA_H_
#define _MEMORY_VITA_H_

#ifdef VITA

#include <stdlib.h>
#include <string.h>

// ── VirtualAlloc / VirtualFree ────────────────────────────────────────────
#define MEM_COMMIT      0x1000
#define MEM_RESERVE     0x2000
#define MEM_RELEASE     0x8000
#define PAGE_READWRITE  0x04
#define PAGE_READONLY   0x02

inline void* VirtualAlloc(void* /*addr*/, SIZE_T size, DWORD /*type*/, DWORD /*prot*/)
{
    void* p = malloc(size);
    if (p) memset(p, 0, size);
    return p;
}

inline BOOL VirtualFree(void* p, SIZE_T /*size*/, DWORD /*type*/)
{
    free(p);
    return TRUE;
}

inline BOOL VirtualProtect(void* /*p*/, SIZE_T /*size*/, DWORD /*new_prot*/, DWORD* /*old_prot*/)
{
    return TRUE;  // no-op on Vita; all user pages are RW
}

// ── HeapAlloc / HeapFree ─────────────────────────────────────────────────
#define HEAP_ZERO_MEMORY  0x00000008
typedef void* HANDLE_HEAP;

inline HANDLE_HEAP GetProcessHeap() { return (HANDLE_HEAP)1; }

inline void* HeapAlloc(HANDLE_HEAP /*h*/, DWORD flags, SIZE_T size)
{
    void* p = malloc(size);
    if (p && (flags & HEAP_ZERO_MEMORY)) memset(p, 0, size);
    return p;
}

inline void* HeapReAlloc(HANDLE_HEAP /*h*/, DWORD /*flags*/, void* p, SIZE_T size)
{
    return realloc(p, size);
}

inline BOOL HeapFree(HANDLE_HEAP /*h*/, DWORD /*flags*/, void* p)
{
    free(p);
    return TRUE;
}

inline SIZE_T HeapSize(HANDLE_HEAP /*h*/, DWORD /*flags*/, const void* /*p*/)
{
    return 0;  // not trackable via standard malloc
}

// ── GlobalAlloc / GlobalFree ─────────────────────────────────────────────
#define GMEM_FIXED      0x0000
#define GMEM_ZEROINIT   0x0040
typedef void* HGLOBAL;

inline HGLOBAL GlobalAlloc(UINT flags, SIZE_T size)
{
    void* p = malloc(size);
    if (p && (flags & GMEM_ZEROINIT)) memset(p, 0, size);
    return p;
}
inline HGLOBAL GlobalFree(HGLOBAL p) { free(p); return NULL; }
inline void*   GlobalLock(HGLOBAL p) { return p; }
inline BOOL    GlobalUnlock(HGLOBAL /*p*/) { return TRUE; }
inline SIZE_T  GlobalSize(HGLOBAL /*p*/) { return 0; }

// ── LocalAlloc / LocalFree ───────────────────────────────────────────────
typedef void* HLOCAL;
#define LMEM_FIXED    0x0000
#define LMEM_ZEROINIT 0x0040
inline HLOCAL LocalAlloc(UINT flags, SIZE_T size) { return GlobalAlloc(flags, size); }
inline HLOCAL LocalFree(HLOCAL p) { free(p); return NULL; }

// ── FlushInstructionCache ────────────────────────────────────────────────
inline BOOL FlushInstructionCache(HANDLE /*proc*/, void* /*base*/, SIZE_T /*size*/) { return TRUE; }

#endif // VITA
#endif // _MEMORY_VITA_H_
