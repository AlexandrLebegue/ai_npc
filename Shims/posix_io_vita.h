// posix_io_vita.h — POSIX replacements for MSVC file-finding and I/O APIs
// Used by CryPak.cpp and related CrySystem files on PS Vita
#pragma once
#ifndef _VITA_POSIX_IO_H_
#define _VITA_POSIX_IO_H_

#ifdef VITA

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <psp2/io/dirent.h>

// ── File mode open flags ─────────────────────────────────────────────────
#ifndef _O_RDONLY
#  define _O_RDONLY   O_RDONLY
#endif
#ifndef _O_WRONLY
#  define _O_WRONLY   O_WRONLY
#endif
#ifndef _O_RDWR
#  define _O_RDWR     O_RDWR
#endif
#ifndef _O_CREAT
#  define _O_CREAT    O_CREAT
#endif
#ifndef _O_TRUNC
#  define _O_TRUNC    O_TRUNC
#endif
#ifndef _O_APPEND
#  define _O_APPEND   O_APPEND
#endif
#ifndef _O_BINARY
#  define _O_BINARY   0        // no binary/text distinction on POSIX
#endif
#ifndef _O_TEXT
#  define _O_TEXT     0
#endif

// ── File attribute constants ─────────────────────────────────────────────
#ifndef _A_NORMAL
#  define _A_NORMAL   0x00
#endif
#ifndef _A_RDONLY
#  define _A_RDONLY   0x01
#endif
#ifndef _A_HIDDEN
#  define _A_HIDDEN   0x02
#endif
#ifndef _A_SYSTEM
#  define _A_SYSTEM   0x04
#endif
#ifndef _A_SUBDIR
#  define _A_SUBDIR   0x10
#endif
#ifndef _A_ARCH
#  define _A_ARCH     0x20
#endif

// ── _finddata_t / __finddata64_t structs ────────────────────────────────
#ifndef _FINDDATA_T_DEFINED
#define _FINDDATA_T_DEFINED
struct _finddata_t {
    unsigned attrib;
    time_t   time_create;
    time_t   time_access;
    time_t   time_write;
    unsigned size;
    char     name[260];
};
struct __finddata64_t {
    unsigned  attrib;
    long long time_create;
    long long time_access;
    long long time_write;
    long long size;
    char      name[260];
};
#endif

// ── Internal state for find-file iteration (using sceIo directory scan) ──
#define VITA_FIND_MAX 512
struct _vita_find_handle {
    char   dir[512];       // directory path
    char   pattern[260];   // filename glob pattern
    SceUID uid;            // sceIoDopen handle
    char   matches[VITA_FIND_MAX][260];
    int    count;
    int    idx;
};

static inline int _vita_match_pattern(const char* name, const char* pat)
{
    // simple wildcard matching: * matches any sequence, ? matches one char
    if (strcmp(pat, "*") == 0 || strcmp(pat, "*.*") == 0) return 1;
    const char* n = name;
    const char* p = pat;
    const char* star_p = NULL;
    const char* star_n = NULL;
    while (*n) {
        if (*p == '?' || *p == *n) { n++; p++; }
        else if (*p == '*') { star_p = p++; star_n = n; }
        else if (star_p) { p = star_p + 1; n = ++star_n; }
        else return 0;
    }
    while (*p == '*') p++;
    return *p == '\0';
}

static inline intptr_t _findfirst64(const char* pattern, struct __finddata64_t* fd)
{
    struct _vita_find_handle* h = (struct _vita_find_handle*)malloc(sizeof(*h));
    if (!h) return -1;
    memset(h, 0, sizeof(*h));

    // Split pattern into directory and filename parts
    const char* slash = strrchr(pattern, '/');
    const char* bslash = strrchr(pattern, '\\');
    const char* sep = slash > bslash ? slash : bslash;
    if (sep) {
        size_t dlen = (size_t)(sep - pattern);
        strncpy(h->dir, pattern, dlen < sizeof(h->dir)-1 ? dlen : sizeof(h->dir)-1);
        h->dir[dlen < sizeof(h->dir)-1 ? dlen : sizeof(h->dir)-1] = '\0';
        strncpy(h->pattern, sep+1, sizeof(h->pattern)-1);
    } else {
        strncpy(h->dir, ".", sizeof(h->dir)-1);
        strncpy(h->pattern, pattern, sizeof(h->pattern)-1);
    }

    // Scan directory and collect matching entries
    h->uid = sceIoDopen(h->dir);
    if (h->uid < 0) { free(h); return -1; }

    SceIoDirent entry;
    h->count = 0;
    while (sceIoDread(h->uid, &entry) > 0 && h->count < VITA_FIND_MAX) {
        if (_vita_match_pattern(entry.d_name, h->pattern)) {
            strncpy(h->matches[h->count], entry.d_name, 259);
            h->matches[h->count][259] = '\0';
            h->count++;
        }
    }
    sceIoDclose(h->uid);
    h->uid = -1;

    if (h->count == 0) { free(h); return -1; }

    // Fill first result
    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", h->dir, h->matches[0]);
    SceIoStat st;
    if (sceIoGetstat(fullpath, &st) == 0) {
        strncpy(fd->name, h->matches[0], 259); fd->name[259] = '\0';
        fd->size        = (long long)st.st_size;
        fd->attrib      = SCE_S_ISDIR(st.st_mode) ? _A_SUBDIR : _A_NORMAL;
        fd->time_write  = 0;
        fd->time_access = 0;
        fd->time_create = 0;
    } else {
        strncpy(fd->name, h->matches[0], 259); fd->name[259] = '\0';
        fd->attrib = _A_NORMAL; fd->size = 0;
        fd->time_write = fd->time_access = fd->time_create = 0;
    }
    h->idx = 1;
    return (intptr_t)h;
}

static inline int _findnext64(intptr_t handle, struct __finddata64_t* fd)
{
    struct _vita_find_handle* h = (struct _vita_find_handle*)handle;
    if (!h || h->idx >= h->count) return -1;

    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", h->dir, h->matches[h->idx]);
    SceIoStat st;
    if (sceIoGetstat(fullpath, &st) == 0) {
        strncpy(fd->name, h->matches[h->idx], 259); fd->name[259] = '\0';
        fd->size   = (long long)st.st_size;
        fd->attrib = SCE_S_ISDIR(st.st_mode) ? _A_SUBDIR : _A_NORMAL;
        fd->time_write = fd->time_access = fd->time_create = 0;
    } else {
        strncpy(fd->name, h->matches[h->idx], 259); fd->name[259] = '\0';
        fd->attrib = _A_NORMAL; fd->size = 0;
        fd->time_write = fd->time_access = fd->time_create = 0;
    }
    h->idx++;
    return 0;
}

static inline void _findclose(intptr_t handle)
{
    if (!handle) return;
    free((struct _vita_find_handle*)handle);
}

// Also provide _findfirst / _findnext wrappers using 64-bit versions
static inline intptr_t _findfirst(const char* pattern, struct _finddata_t* fd)
{
    struct __finddata64_t fd64;
    intptr_t h = _findfirst64(pattern, &fd64);
    if (h == -1) return -1;
    strncpy(fd->name, fd64.name, sizeof(fd->name)-1);
    fd->name[sizeof(fd->name)-1] = 0;
    fd->size        = (unsigned)fd64.size;
    fd->attrib      = fd64.attrib;
    fd->time_write  = (time_t)fd64.time_write;
    fd->time_access = (time_t)fd64.time_access;
    fd->time_create = (time_t)fd64.time_create;
    return h;
}

static inline int _findnext(intptr_t handle, struct _finddata_t* fd)
{
    struct __finddata64_t fd64;
    int r = _findnext64(handle, &fd64);
    if (r != 0) return r;
    strncpy(fd->name, fd64.name, sizeof(fd->name)-1);
    fd->name[sizeof(fd->name)-1] = 0;
    fd->size        = (unsigned)fd64.size;
    fd->attrib      = fd64.attrib;
    fd->time_write  = (time_t)fd64.time_write;
    fd->time_access = (time_t)fd64.time_access;
    fd->time_create = (time_t)fd64.time_create;
    return 0;
}

// ── _fullpath — map to realpath ──────────────────────────────────────────
static inline char* _fullpath(char* dst, const char* src, size_t maxlen)
{
    char* r = realpath(src, NULL);
    if (!r) {
        // realpath fails if file doesn't exist; construct absolute manually
        if (src[0] == '/')
            strncpy(dst, src, maxlen-1);
        else {
            char cwd[512] = {0};
            getcwd(cwd, sizeof(cwd));
            snprintf(dst, maxlen, "%s/%s", cwd, src);
        }
        dst[maxlen-1] = 0;
        return dst;
    }
    strncpy(dst, r, maxlen-1);
    dst[maxlen-1] = 0;
    free(r);
    return dst;
}

// ── GetFileAttributes / INVALID_FILE_ATTRIBUTES ──────────────────────────
#ifndef INVALID_FILE_ATTRIBUTES
#  define INVALID_FILE_ATTRIBUTES ((DWORD)0xFFFFFFFF)
#endif
#ifndef GetFileAttributes
static inline DWORD VitaGetFileAttributes(const char* path)
{
    struct stat st;
    if (stat(path, &st) != 0) return INVALID_FILE_ATTRIBUTES;
    DWORD attr = 0;
    if (S_ISDIR(st.st_mode))   attr |= FILE_ATTRIBUTE_DIRECTORY;
    if (!(st.st_mode & S_IWUSR)) attr |= 0x01; // FILE_ATTRIBUTE_READONLY
    if (!attr) attr = FILE_ATTRIBUTE_NORMAL;
    return attr;
}
#  define GetFileAttributes(p) VitaGetFileAttributes(p)
#endif

// ── _mkdir ───────────────────────────────────────────────────────────────
#ifndef _mkdir
#  define _mkdir(p)  mkdir((p), 0755)
#endif

// ── strlwr (in-place lowercase) ──────────────────────────────────────────
#ifndef strlwr
static inline char* strlwr(char* s)
{
    for (char* p = s; *p; ++p)
        if (*p >= 'A' && *p <= 'Z') *p += 32;
    return s;
}
#endif

// ── _fmode global (POSIX default is binary) ──────────────────────────────
#ifndef _fmode
static int _fmode = _O_BINARY;
#endif

// ── _access ──────────────────────────────────────────────────────────────
#ifndef _access
#  define _access(p,m)  access((p),(m))
#endif

// ── GetCurrentDirectory ──────────────────────────────────────────────────
#ifndef GetCurrentDirectory
static inline BOOL VitaGetCurrentDirectory(DWORD len, char* buf)
{
    return getcwd(buf, (size_t)len) != NULL;
}
#define GetCurrentDirectory(len,buf) VitaGetCurrentDirectory(len,buf)
#endif

// ── Int32x32To64 ─────────────────────────────────────────────────────────
#ifndef Int32x32To64
#  define Int32x32To64(a,b) ((long long)(int)(a) * (long long)(int)(b))
#endif

// ── stat64 / _fstat64 ────────────────────────────────────────────────────
// Vita is 32-bit; use regular stat/fstat
#ifndef stat64
#  define stat64   stat
#  define _fstat64 fstat
#endif

// ── Additional open flags (hint-only on POSIX) ───────────────────────────
#ifndef _O_SEQUENTIAL
#  define _O_SEQUENTIAL   0
#endif
#ifndef _O_RANDOM
#  define _O_RANDOM       0
#endif
#ifndef _O_SHORT_LIVED
#  define _O_SHORT_LIVED  0
#endif
#ifndef _O_TEMPORARY
#  define _O_TEMPORARY    0
#endif

// ── Linux path helper functions (missing from codebase for non-Linux) ────

#include <string>

// adaptFilenameToLinux: convert backslashes to forward slashes, lowercase
static inline void adaptFilenameToLinux(std::string& path)
{
    for (size_t i = 0; i < path.size(); ++i)
    {
        if (path[i] == '\\') path[i] = '/';
        else path[i] = (char)tolower((unsigned char)path[i]);
    }
}

// getFilenameNoCase: on Vita the FS is case-insensitive; just return path as-is
static inline bool getFilenameNoCase(const char* /*base*/, std::string& result)
{
    // Vita PSVFS is case-insensitive — accept path as given
    (void)result;
    return false; // caller will use adjustedFilename
}

// replaceDoublePathFilename: collapse // into /
static inline void replaceDoublePathFilename(char* path)
{
    if (!path) return;
    char* src = path;
    char* dst = path;
    while (*src)
    {
        *dst++ = *src;
        if (*src == '/')
            while (src[1] == '/') ++src;
        ++src;
    }
    *dst = '\0';
}

// comparePathNames: case-insensitive path prefix compare (returns 0 if equal)
static inline int comparePathNames(const char* a, const char* b, size_t len)
{
    return strncasecmp(a, b, len);
}

#endif // VITA
#endif // _VITA_POSIX_IO_H_
