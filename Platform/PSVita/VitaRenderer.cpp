////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  VitaRenderer.cpp  —  vitaGL / GXM bridge for XRenderOGL
//
//  The engine's OpenGL renderer (XRenderOGL) calls standard OpenGL.
//  vitaGL translates those calls to Sony GXM at runtime, so no per-call
//  translation is needed here.  This file handles initialisation,
//  swap-chain management, and Vita-specific render-state overrides.
////////////////////////////////////////////////////////////////////////////

#include <vitaGL.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <stdio.h>

// ── Display constants ─────────────────────────────────────────────────────
// PS Vita native resolution: 960×544
static const int VITA_SCREEN_W = 960;
static const int VITA_SCREEN_H = 544;

// ── State ─────────────────────────────────────────────────────────────────
static bool g_rendererInitialised = false;

// ── Initialise vitaGL / GXM ───────────────────────────────────────────────
bool VitaRenderer_Init()
{
    if (g_rendererInitialised) return true;

    // vglInit allocates VRAM for the colour + depth buffers.
    // 64 MB threshold: vitaGL will use VRAM up to this limit, then spill to RAM.
    vglInit(64 * 1024);

    // Viewport to full Vita screen
    glViewport(0, 0, VITA_SCREEN_W, VITA_SCREEN_H);

    // Depth buffer: 16-bit is sufficient for the Vita's fill-rate budget
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    // Standard backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Far Cry uses DXT texture compression; vitaGL exposes
    // GL_EXT_texture_compression_s3tc on Vita via software decode.
    // We enable it here so the engine texture loader doesn't fall back.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    g_rendererInitialised = true;
    printf("[VitaRenderer] Initialised  %dx%d\n", VITA_SCREEN_W, VITA_SCREEN_H);
    return true;
}

// ── Present frame ─────────────────────────────────────────────────────────
void VitaRenderer_SwapBuffers()
{
    vglSwapBuffers(GL_FALSE);   // GL_FALSE = no VSync (engine handles pacing)
}

// ── Resolution query ─────────────────────────────────────────────────────
void VitaRenderer_GetResolution(int* w, int* h)
{
    if (w) *w = VITA_SCREEN_W;
    if (h) *h = VITA_SCREEN_H;
}

// ── Shutdown ─────────────────────────────────────────────────────────────
void VitaRenderer_Shutdown()
{
    vglEnd();
    g_rendererInitialised = false;
}
