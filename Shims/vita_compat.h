////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  vita_compat.h  —  Master compatibility header
//
//  Include this FIRST in every StdAfx.h / precompiled-header on Vita.
//  It pulls in PSVitaSpecific.h (which provides Win32 type aliases),
//  then poisons the windows.h guard so the real windows.h can never
//  be included accidentally.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _VITA_COMPAT_H_
#define _VITA_COMPAT_H_

#ifdef VITA

// ── Pull in our full Vita platform layer ─────────────────────────────────
#include <PSVitaSpecific.h>
#include <win32_vita.h>
#include <directx_vita.h>
#include <threading_vita.h>
#include <memory_vita.h>
#include <posix_io_vita.h>
#include <vita_module_registry.h>

// ── Poison the real windows.h / winsock headers ──────────────────────────
// Any #include <windows.h> after this point is a no-op.
#ifndef _WINDOWS_
#  define _WINDOWS_
#endif
#ifndef _WINSOCKAPI_
#  define _WINSOCKAPI_
#endif
#ifndef _WINSOCK2API_
#  define _WINSOCK2API_
#endif

// ── Disable MSVC warning pragmas on GCC/Clang ────────────────────────────
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

// ── STLPORT thread-disabling macros the engine expects ───────────────────
#define _NOTHREADS
#define _STLP_NO_THREADS

// ── DLL export macros → no-op on Vita (static linking) ───────────────────
#ifndef DLL_EXPORT
#  define DLL_EXPORT
#endif
#ifndef DLL_IMPORT
#  define DLL_IMPORT
#endif

// ── Suppress _declspec entirely ──────────────────────────────────────────
#ifndef __declspec
#  define __declspec(x)
#endif

// ── MSVC __pragma → silence on GCC ───────────────────────────────────────
#ifndef __pragma
#  define __pragma(x)
#endif

// ── __try / __except / __finally → not available on GCC/Vita ────────────
// Wrap engine code that uses SEH with macros that degrade to try/catch.
#ifndef __try
#  define __try        try
#  define __except(x)  catch(...)
#  define __finally
#endif

// ── Naked functions not supported on ARM GCC ─────────────────────────────
// The engine uses __declspec(naked) for a few x86 asm stubs.
// On Vita they are replaced by C equivalents (see asm_replacements.h).
#define VITA_NAKED_STUB  /* nothing */

// ── DirectInput / DirectSound type placeholders ──────────────────────────
// Already defined in directx_vita.h — guard against double-def.

// ── Sockets ──────────────────────────────────────────────────────────────
// vitaSDK provides BSD-compat sockets via <psp2/net/net.h>
// but the engine includes <winsock2.h>.  Our win32_vita.h already
// provides the SOCKET typedef; include the real Vita net header here.
#include <psp2/net/net.h>

#endif // VITA
#endif // _VITA_COMPAT_H_
