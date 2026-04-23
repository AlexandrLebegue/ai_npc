# OpenFarCry-Vita

Portage non commercial de **Far Cry 1** (CryEngine 1) vers la **PlayStation Vita** en homebrew, à but d'archivage et d'étude technique.

> Statut : **Phase 0 — bootstrap**. Rien ne tourne encore. Plan de portage complet dans `docs/PORT_PLAN.md`.

## But

Produire un exécutable Vita (`.vpk`) capable de lancer Far Cry 1 à partir des assets de la version PC que le joueur possède légitimement (DVD ou clé Ubisoft Connect). Le dépôt ne contient **aucun asset** du jeu — uniquement du code.

Cible : Vita avec CFW HENkaku / Enso, framework VitaSDK, renderer vitaGL (OpenGL ES 2.0 sur GXM).

## Structure du dépôt

```
.
├── upstream/             # Submodule : code source CryEngine 1 original (OpenFarCry1), non modifié
├── port/
│   ├── hal/              # Couche d'abstraction plateforme (fichiers, threads, input, audio)
│   ├── renderer_vita/    # Renderer réécrit au-dessus de vitaGL
│   │   └── shaders/      # Shaders GLSL ES transpilés depuis HLSL
│   └── vita/             # Code spécifique Vita (boot, livearea, packaging)
├── build/
│   ├── vita/             # Build target Vita (CMake + vitasdk.cmake)
│   └── pc-gles/          # Build desktop Linux/Windows GLES pour tests rapides
├── tools/
│   ├── shader-transpile/ # Pipeline HLSL → GLSL ES 2.0 (offline)
│   └── asset-pipeline/   # Réduction résolution + compression PVRTC
├── vita/                 # Ressources packaging (param.sfo, livearea/, icon0.png)
├── docs/                 # Documentation technique
└── CMakeLists.txt        # Racine CMake (à venir)
```

## Prérequis (Phase 0 — à installer avant de builder)

- [VitaSDK](https://vitasdk.org) — toolchain `arm-vita-eabi-gcc`, via `vdpm`
- [vitaGL](https://github.com/Rinnegatamante/vitaGL) — wrapper GLES sur GXM
- CMake ≥ 3.20
- Python 3 ≥ 3.10 (pour les scripts d'asset pipeline)
- Git LFS *optionnel* (si shaders transpilés versionnés)

## Licence

- **Code du fork** : MIT (dossiers `port/`, `tools/`, `build/`, `vita/`)
- **Code upstream** (`upstream/`) : propriété Crytek / Ubisoft, mis à disposition par le dépôt [OpenFarCry1](https://github.com/OpenFarCry1/Far-Cry-1-CryEngine1) à des fins d'archivage non commercial uniquement. Ce fork ne redistribue aucune version modifiée du code upstream — il pointe dessus via submodule git.
- **Assets du jeu** : non fournis. Le joueur doit posséder une copie légitime de Far Cry 1.

## Documentation

- `docs/PORT_PLAN.md` — plan de portage complet (phases, modules, problématiques)
- `docs/HAL.md` — conception de la couche d'abstraction plateforme *(à venir)*
- `docs/RENDERER.md` — mapping D3D9 → vitaGL *(à venir)*
- `docs/SHADERS.md` — pipeline de transpilation HLSL → GLSL ES *(à venir)*
- `docs/CONTROLS.md` — mapping sceCtrl / sceTouch *(à venir)*
- `docs/BUILD.md` — procédure de build locale *(à venir)*

## État

Aucun code n'est encore écrit. Le dépôt en est à la mise en place du squelette. Voir `docs/PORT_PLAN.md` pour la feuille de route.
