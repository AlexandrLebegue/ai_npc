////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  directx_vita.h  —  DirectX / DXGI stubs and type compatibility
//
//  Far Cry uses D3D9 types in some headers even when using the OpenGL
//  renderer path.  This file provides the minimal type and constant
//  definitions needed so those headers compile on Vita (where D3D is
//  unavailable).  Actual rendering goes through XRenderOGL → vitaGL.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _DIRECTX_VITA_SHIM_H_
#define _DIRECTX_VITA_SHIM_H_

#ifdef VITA

#include <PSVitaSpecific.h>

// ── HRESULT / COM conventions already in PSVitaSpecific.h ────────────────

// ── D3D colour format constants used by the texture system ───────────────
typedef unsigned int D3DFORMAT;
#define D3DFMT_UNKNOWN          0
#define D3DFMT_A8R8G8B8         21
#define D3DFMT_X8R8G8B8         22
#define D3DFMT_R5G6B5           23
#define D3DFMT_A1R5G5B5         25
#define D3DFMT_A4R4G4B4         26
#define D3DFMT_R8G8B8           20
#define D3DFMT_A8               28
#define D3DFMT_L8               50
#define D3DFMT_DXT1             0x31545844   // MAKEFOURCC('D','X','T','1')
#define D3DFMT_DXT3             0x33545844
#define D3DFMT_DXT5             0x35545844
#define D3DFMT_D16              80
#define D3DFMT_D24S8            75
#define D3DFMT_D32              71

// ── D3D primitive types ──────────────────────────────────────────────────
typedef unsigned int D3DPRIMITIVETYPE;
#define D3DPT_POINTLIST         1
#define D3DPT_LINELIST          2
#define D3DPT_LINESTRIP         3
#define D3DPT_TRIANGLELIST      4
#define D3DPT_TRIANGLESTRIP     5
#define D3DPT_TRIANGLEFAN       6

// ── D3D matrix type ───────────────────────────────────────────────────────
struct D3DMATRIX {
    union {
        struct { float _11,_12,_13,_14, _21,_22,_23,_24,
                       _31,_32,_33,_34, _41,_42,_43,_44; };
        float m[4][4];
    };
};

// ── D3D viewport ─────────────────────────────────────────────────────────
struct D3DVIEWPORT9 {
    unsigned int X, Y, Width, Height;
    float MinZ, MaxZ;
};

// ── D3D colour value ─────────────────────────────────────────────────────
typedef unsigned int D3DCOLOR;
#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)

// ── D3D transform states (used in fixed-function path stubs) ─────────────
typedef unsigned int D3DTRANSFORMSTATETYPE;
#define D3DTS_WORLD         256
#define D3DTS_VIEW          2
#define D3DTS_PROJECTION    3

// ── Minimal IDirect3D9 / IDirect3DDevice9 stub (never called on Vita) ────
// The engine factories check for a valid pointer and skip D3D code paths
// when running the OpenGL renderer.  Providing NULL is sufficient.
#define DIRECT3D_VERSION  0x0900
typedef void IDirect3D9;
typedef void IDirect3DDevice9;
typedef void IDirect3DTexture9;
typedef void IDirect3DVertexBuffer9;
typedef void IDirect3DIndexBuffer9;
typedef void IDirect3DSurface9;

// D3DXMatrixMultiply — used in some animation code
inline D3DMATRIX* D3DXMatrixMultiply(D3DMATRIX* out, const D3DMATRIX* a, const D3DMATRIX* b)
{
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            out->m[r][c] = 0.0f;
            for (int k = 0; k < 4; ++k)
                out->m[r][c] += a->m[r][k] * b->m[k][c];
        }
    return out;
}

// ── DirectSound stubs (audio goes through SceAudio) ──────────────────────
typedef void IDirectSound8;
typedef void IDirectSoundBuffer8;
typedef void IDirectSoundBuffer;
#define DSBCAPS_GLOBALFOCUS     0x00008000
#define DSBPLAY_LOOPING         0x00000001

// ── DirectInput stubs (input goes through SceCtrl) ───────────────────────
typedef void IDirectInput8;
typedef void IDirectInputDevice8;
#define DISCL_FOREGROUND        0x00000004
#define DISCL_NONEXCLUSIVE      0x00000002

#endif // VITA
#endif // _DIRECTX_VITA_SHIM_H_
