// winsock.h — PS Vita stub
// Provides WSAE* error codes and basic socket types for the Vita build.
// Real network functionality uses vitaSDK's SceSysmodule_Net.
#pragma once
#ifndef _VITA_WINSOCK_H_
#define _VITA_WINSOCK_H_

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>

// ── Basic socket types ──────────────────────────────────────────────────────
#ifndef SOCKET
typedef int SOCKET;
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR    (-1)
#endif

typedef unsigned short u_short;
typedef unsigned long  u_long;

// ── WSA error constants (mirroring Winsock numeric values) ──────────────────
#define WSAEINTR            10004
#define WSAEBADF            10009
#define WSAEACCES           10013
#define WSAEFAULT           10014
#define WSAEINVAL           10022
#define WSAEMFILE           10024
#define WSAEWOULDBLOCK      10035
#define WSAEINPROGRESS      10036
#define WSAEALREADY         10037
#define WSAENOTSOCK         10038
#define WSAEDESTADDRREQ     10039
#define WSAEMSGSIZE         10040
#define WSAEPROTOTYPE       10041
#define WSAENOPROTOOPT      10042
#define WSAEPROTONOSUPPORT  10043
#define WSAESOCKTNOSUPPORT  10044
#define WSAEOPNOTSUPP       10045
#define WSAEPFNOSUPPORT     10046
#define WSAEAFNOSUPPORT     10047
#define WSAEADDRINUSE       10048
#define WSAEADDRNOTAVAIL    10049
#define WSAENETDOWN         10050
#define WSAENETUNREACH      10051
#define WSAENETRESET        10052
#define WSAECONNABORTED     10053
#define WSAECONNRESET       10054
#define WSAENOBUFS          10055
#define WSAEISCONN          10056
#define WSAENOTCONN         10057
#define WSAESHUTDOWN        10058
#define WSAETOOMANYREFS     10059
#define WSAETIMEDOUT        10060
#define WSAECONNREFUSED     10061
#define WSAELOOP            10062
#define WSAENAMETOOLONG     10063
#define WSAEHOSTDOWN        10064
#define WSAEHOSTUNREACH     10065
#define WSAENOTEMPTY        10066
#define WSAEPROCLIM         10067
#define WSAEUSERS           10068
#define WSAEDQUOT           10069
#define WSAESTALE           10070
#define WSAEREMOTE          10071
#define WSAHOST_NOT_FOUND   11001
#define WSATRY_AGAIN        11002
#define WSANO_RECOVERY      11003
#define WSANO_DATA          11004
#define WSANO_ADDRESS       11004
#define WSASYSNOTREADY      10091
#define WSAVERNOTSUPPORTED  10092
#define WSANOTINITIALISED   10093
#define WSAEDISCON          10101

// ── sockaddr types (provided by vitaSDK's net layer) ───────────────────────
// SceNetSockaddrIn and SceNetInAddr are the native types.
// Provide POSIX-compatible aliases.
#ifndef _SOCKADDR_DEFINED
#define _SOCKADDR_DEFINED
typedef SceNetSockaddr      sockaddr;
typedef SceNetSockaddrIn    sockaddr_in;
typedef SceNetInAddr        in_addr;
#endif

// ── WSAStartup stub ────────────────────────────────────────────────────────
#ifdef __cplusplus
inline int WSAGetLastError() { return 0; }
inline int WSAStartup(u_short ver, void* data) { (void)ver; (void)data; return 0; }
inline void WSACleanup() {}
#endif

#endif // _VITA_WINSOCK_H_
