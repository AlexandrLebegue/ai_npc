// crtdbg.h — PS Vita stub
// MSVC debug runtime header — stubs for Vita build
#pragma once
#ifndef _VITA_CRTDBG_H_
#define _VITA_CRTDBG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Debug flag stubs
#define _CRTDBG_ALLOC_MEM_DF       0x01
#define _CRTDBG_LEAK_CHECK_DF      0x20
#define _CRTDBG_CHECK_ALWAYS_DF    0x04
#define _CRTDBG_MAP_ALLOC          0

#ifndef _ASSERTE
#  define _ASSERTE(expr)  ((void)0)
#endif
#ifndef _ASSERT
#  define _ASSERT(expr)   ((void)0)
#endif

inline int  _CrtSetDbgFlag(int f)                 { (void)f; return 0; }
inline void _CrtDumpMemoryLeaks(void)             {}
inline void _CrtSetReportMode(int t, int m)       { (void)t; (void)m; }
inline void _CrtMemCheckpoint(void* s)            { (void)s; }
inline int  _CrtMemDifference(void*d,void*o,void*n){ (void)d;(void)o;(void)n; return 0; }
inline void _CrtMemDumpStatistics(const void* s)  { (void)s; }

#ifdef __cplusplus
}
#endif

#endif // _VITA_CRTDBG_H_
