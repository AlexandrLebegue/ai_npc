# Plan de portage OpenFarCry-Vita

Ce document est le plan de référence du projet. Pour un aperçu rapide, voir le README racine.

## Contexte

Portage de **Far Cry 1 (CryEngine 1)** depuis [OpenFarCry1/Far-Cry-1-CryEngine1](https://github.com/OpenFarCry1/Far-Cry-1-CryEngine1) vers la **PlayStation Vita** en homebrew. Le code upstream est C++ MSVC 2005, Win32 + DirectX 9 + x86, non exécutable tel quel sur Vita (ARMv7 + GXM/GLES). Objectif : produire un fork portable compilable avec le VitaSDK, avec un renderer vitaGL et une couche d'abstraction plateforme.

Projet solo, multi-années, non commercial.

## État des lieux du dépôt upstream

Modules présents dans `upstream/` :

| Module | Rôle | Sort dans le portage |
|---|---|---|
| `Cry3DEngine` | Scene graph, culling, streaming | À porter (gros module haut-niveau) |
| `CryAISystem` | IA des ennemis, pathfinding, goals | Portable (C++ pur) |
| `CryAnimation` | Skinning, blending, IK | Porter après audit SSE → NEON |
| `CryCommon` | Maths, containers, interfaces | Portable, héberge la HAL |
| `CryEntitySystem` | Entités & composants | Portable |
| `CryFont` | Rendu de texte | Portable (utilise le renderer) |
| `CryGame` | Gameplay Far Cry (scripts, mission, armes) | Portable |
| `CryInput` | Abstraction clavier/souris/pad | À réécrire sur sceCtrl/sceTouch |
| `CryMovie` | Cinématiques (Bink ?) | À vérifier, probablement à remplacer |
| `CryNetwork` | Multijoueur, WinSock | Stubbé Phase 3, activé Phase 5 |
| `CryPhysics` | Physique | Portable |
| `CryScriptSystem` | Lua 5.0 | Portable tel quel |
| `CrySoundSystem` | Audio (wrapper FMOD 3.74) | **Réécrire sur OpenAL-Soft** |
| `CrySystem` | Loop principal, timer, logging, PAK | À porter (Win32 → VitaSDK) |
| `RenderDll` | Renderer D3D9 | **Réécrire en `CryRenderer_Vita`** |
| `Editor` | Sandbox (MFC + XT Lib) | **Exclu** |
| `FARCRY` | Exécutable final | À porter (entry point) |
| `FarCry_WinSV` | Dedicated server Windows | Exclu (Phase 5 éventuel) |
| `PunkBuster` | Anti-cheat propriétaire | **Exclu** (fermé) |
| `ResourceCompiler*` | Outils offline de build d'assets | Porter sur PC Linux/Windows pour pipeline |
| `STLPORT` | STL alternative pour VS2005 | **Exclu**, on utilise libstdc++ moderne |
| `Win32APIWrapper` | Wrapper Win32 | **À remplacer intégralement par HAL** |

## Dépendances tierces

D'après `upstream/BuildAMD64.txt` :

| Dépendance | Usage | Action |
|---|---|---|
| FMOD 3.74 | Audio runtime | Remplacer par OpenAL-Soft |
| XT Lib (Codejock) | MFC UI, éditeur Sandbox | Non nécessaire (éditeur exclu) |
| CompressATI3.dll | Compression DXT runtime | Remplacer par PVRTexTool offline |
| Microsoft PSDK 1289 + MFC 7.1 | Éditeur Sandbox | Non nécessaire |
| DirectX 9 SDK | Renderer & input | Remplacer par vitaGL + sceCtrl |

## Architecture cible

```
+-----------------------------------------------------+
|  CryGame / CryAISystem / CryPhysics / CryAnimation  |  ← portable quasi inchangé
|  CryEntitySystem / CryScriptSystem (Lua)            |
|  CryMovie / CryFont / Cry3DEngine (haut-niveau)     |
+-----------------------------------------------------+
|  Platform HAL (port/hal/, nouveau)                  |  ← couche tampon
|  PlatformFile / PlatformThread / PlatformTime       |
|  PlatformInput / PlatformAudio / PlatformLog        |
+-----------------------------------------------------+
|  CryRenderer_Vita (port/renderer_vita/, réécriture) |  ← plus gros chantier
|  IRenderer → vitaGL (GLES 2.0)                      |
|  HLSL SM2/3 → GLSL ES 2.0 (transpilation offline)   |
+-----------------------------------------------------+
|  VitaSDK : newlib, pthread-embedded, sceCtrl,       |
|  sceAudio, sceIo, sceNet, vitaGL, sceGxm            |
+-----------------------------------------------------+
```

## Phases

### Phase 0 — Bootstrap (1–2 semaines) — **EN COURS**
- [x] Supprimer l'ancien projet, créer la branche de travail
- [x] Cloner OpenFarCry1 en submodule `upstream/`
- [x] Créer le squelette de dossiers
- [x] Rédiger README + docs initiales
- [ ] Installer VitaSDK + vitaGL sur la machine de dev
- [ ] Builder un "Hello Triangle" vitaGL, le tester sur Vita réelle

### Phase 1 — Build minimal CryCommon + HAL (1–2 mois)
- [ ] Remplacer les .sln VS2005 par CMake avec toolchain Vita
- [ ] Faire compiler `CryCommon` avec `arm-vita-eabi-gcc`
- [ ] Implémenter `PlatformFile` (sceIo), `PlatformThread` (pthread-embedded), `PlatformTime` (sceKernel), `PlatformLog`
- [ ] Porter `ICryPak` (lecture des fichiers .pak + zlib)
- [ ] Test : lister le contenu d'un .pak Far Cry sur Vita

### Phase 2 — Renderer Vita (4–8 mois)
- [ ] Squelette `CryRenderer_Vita` sur vitaGL
- [ ] Primitives : swap chain, clear, VBO/IBO, textures 2D, RTT
- [ ] Pipeline shader offline : .cfx → HLSL → GLSL ES → shacc
- [ ] Rendu progressif : mesh statique → skinning → terrain → végétation → eau → post-FX
- [ ] Compression PVRTC + downsampling des textures
- [ ] Simplifications assumées : ombres dynamiques réduites, pas de parallax occlusion

### Phase 3 — Subsystèmes runtime (3–5 mois, parallélisable)
- [ ] Audio : wrapper OpenAL-Soft imitant l'API FMOD haut-niveau
- [ ] Input : sceCtrl + sceTouch + gyro (sceMotion)
- [ ] Réseau : stub "multijoueur indisponible"
- [ ] UI 960×544

### Phase 4 — Boot du 1er niveau (2–3 mois)
- [ ] Charger le niveau Training
- [ ] Résoudre OOM, VRAM overflow, formats assets
- [ ] Streaming agressif
- [ ] **Jalon** : Training jouable 60 s sans crash

### Phase 5 — Optimisation (ouvert)
- [ ] Overclock 500 MHz, épinglage threads
- [ ] Compression assets Zstd/Oodle
- [ ] Niveaux suivants un par un
- [ ] Éventuel multijoueur LAN (sceNet)

## Problématiques majeures

1. **Licence upstream floue** — pas de fichier LICENSE formel, seulement "non commercial" dans la description. Projet homebrew non commercial, aucune redistribution d'assets → risque faible mais non nul.
2. **FMOD 3.74 non redistribuable** — remplacement OpenAL-Soft obligatoire, réécriture wrapper.
3. **D3D9 → GLES 2.0** — chantier principal. Sémantiques très différentes (matrices, coordonnées Y, render states).
4. **Shaders .cfx** — format CryEngine 1 propriétaire, combinatoire de permutations. Parseur dédié nécessaire avant transpilation.
5. **Mémoire serrée** — 640 Mo Vita vs 320 Mo PC minimum. Downsampling et streaming agressif requis.
6. **Aim FPS au stick Vita** — prévoir sensibilité dynamique, aim assist, gyroaim optionnel.
7. **Intrinsics SSE** (`xmmintrin.h`) dans `CryAnimation`, `CryPhysics`, `CryCommon/Cry_Math` → audit + NEON/scalaire.
8. **Alignement ARMv7** plus strict que x86 → vigilance sur casts pointeurs dans la sérialisation PAK.
9. **Build system** — migration complète VS2005 → CMake.
10. **Debug Vita limité** — prévoir build `pc-gles` pour reproduire bugs rapidement sur desktop.
11. **CompressATI3.dll manquante** — compression DXT runtime impossible → pipeline offline via PVRTexTool.
12. **Projet solo long** — communication via Discord upstream pour attirer contributeurs.

## Exclusions assumées

- Éditeur Sandbox (MFC)
- Multijoueur en phase initiale
- Mode 64 bits
- Effets D3D9 avancés (HDR 64bpp, multi-pass refraction)
- Parité visuelle pixel-perfect

## Vérifications / tests par phase

| Phase | Vérification |
|---|---|
| 0 | "Hello Triangle" vitaGL tourne sur Vita réelle |
| 1 | `cmake --build build/vita --target CryCommon` produit un .a propre ; test unitaire de lecture PAK sur Vita |
| 2 | Rendu d'un .cgf statique texturé, comparé visuellement au build `pc-gles` |
| 3 | Lecture OGG audible ; input pad loggé |
| 4 | Niveau Training joué 60 s sans crash — **jalon majeur** |
| 5 | ≥ 20 FPS combat léger, ≥ 15 FPS combat intensif ; niveau Carrier complet |

## Outils et bibliothèques réutilisés

- [VitaSDK](https://vitasdk.org) — GCC ARM + newlib
- [vitaGL](https://github.com/Rinnegatamante/vitaGL) — GLES sur GXM
- [pthread-embedded](https://github.com/vitasdk/pthread-embedded) — threads POSIX Vita
- [OpenAL-Soft Vita](https://github.com/vitasdk/packages/tree/master/openal-soft)
- libvorbis / libogg
- Zlib (livré VitaSDK)
- [PVRTexTool](https://developer.imaginationtech.com/pvrtextool/)
- [HLSLcc](https://github.com/Unity-Technologies/HLSLcc) ou [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross)
- CMake + vita-makepkg / vita-mksfoex / vita-pack-vpk
