////////////////////////////////////////////////////////////////////////////
//  Far Cry 1 — PS Vita Port
//  vita_module_registry.h
//
//  On Vita every engine module is statically linked.
//  CSystem::Init() loads them via LoadDLL / CryGetProcAddress (Win32 DLL API).
//  We override GetProcAddress to dispatch by function name to the real
//  statically-linked constructors.
////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _VITA_MODULE_REGISTRY_H_
#define _VITA_MODULE_REGISTRY_H_

#ifdef VITA

#include <string.h>

// ── Forward declarations matching each module's exported constructor ──────

struct ISystem;
struct IScriptSystemSink;
struct IScriptDebugSink;
struct IEntitySystem;
struct IInput;
struct ISoundSystem;
struct IPhysicalWorld;
struct IAISystem;
struct IScriptSystem;
struct ICryFont;
struct I3DEngine;
struct ICryCharManager;
struct IMovieSystem;
struct INetwork;
struct IRenderer;
struct SCryRenderInterface;

extern "C" {
    // Renderer (XRenderOGL / XRenderNULL)
    IRenderer*        PackageRenderConstructor(int argc, char* argv[], SCryRenderInterface* sp);
    // Entity
    IEntitySystem*    CreateEntitySystem(ISystem* pSystem);
    // Input
    IInput*           CreateInput(ISystem* pSystem, void* hinst, void* hwnd, bool usedInput);
    // Sound
    ISoundSystem*     CreateSoundSystem(ISystem* pSystem, void* hwnd);
    // Physics
    IPhysicalWorld*   CreatePhysicalWorld(ISystem* pSystem);
    // AI
    IAISystem*        CreateAISystem(ISystem* pSystem);
    // Script
    IScriptSystem*    CreateScriptSystem(ISystem* pSystem, IScriptSystemSink* pSink,
                                         IScriptDebugSink* pDebugSink, bool bStdLibs);
    // Font
    ICryFont*         CreateCryFontInterface(ISystem* pSystem);
    // 3D Engine
    I3DEngine*        CreateCry3DEngine(ISystem* pSystem, const char* szInterfaceVersion);
    // Animation
    ICryCharManager*  CreateCharManager(ISystem* pSystem, const char* szInterfaceVersion);
    // Movie
    IMovieSystem*     CreateMovieSystem(ISystem* pSystem);
    // Network (optional — not linked on Vita; stub returns NULL)
}

// ── GetProcAddress replacement ────────────────────────────────────────────
inline void* VitaGetProcAddress(void* /*handle*/, const char* funcName)
{
    if (!funcName) return NULL;
    if (strcmp(funcName, "PackageRenderConstructor") == 0) return (void*)PackageRenderConstructor;
    if (strcmp(funcName, "CreateEntitySystem")        == 0) return (void*)CreateEntitySystem;
    if (strcmp(funcName, "CreateInput")               == 0) return (void*)CreateInput;
    if (strcmp(funcName, "CreateSoundSystem")         == 0) return (void*)CreateSoundSystem;
    if (strcmp(funcName, "CreatePhysicalWorld")       == 0) return (void*)CreatePhysicalWorld;
    if (strcmp(funcName, "CreateAISystem")            == 0) return (void*)CreateAISystem;
    if (strcmp(funcName, "CreateScriptSystem")        == 0) return (void*)CreateScriptSystem;
    if (strcmp(funcName, "CreateCryFontInterface")    == 0) return (void*)CreateCryFontInterface;
    if (strcmp(funcName, "CreateCry3DEngine")         == 0) return (void*)CreateCry3DEngine;
    if (strcmp(funcName, "CreateCharManager")         == 0) return (void*)CreateCharManager;
    if (strcmp(funcName, "CreateMovieSystem")         == 0) return (void*)CreateMovieSystem;
    // Network not linked — return NULL, CSystem handles this gracefully
    if (strcmp(funcName, "CreateNetwork")             == 0) return NULL;
    return NULL;
}

// Override GetProcAddress for Vita
#ifdef GetProcAddress
#undef GetProcAddress
#endif
#define GetProcAddress(h, name) VitaGetProcAddress((void*)(h), (name))

#endif // VITA
#endif // _VITA_MODULE_REGISTRY_H_
