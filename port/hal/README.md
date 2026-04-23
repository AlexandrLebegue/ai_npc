# Platform HAL (Hardware/OS Abstraction Layer)

Couche tampon qui absorbe les différences entre les plateformes. Tout le code en amont (CryGame, Cry3DEngine, etc.) ne doit appeler que les interfaces définies ici — jamais directement Win32, POSIX ou VitaSDK.

## Interfaces prévues

| Fichier | Rôle | Backend Vita | Backend PC (build test) |
|---|---|---|---|
| `PlatformFile.h` | Ouvrir / lire / écrire / mmap fichiers | `sceIo*` | `fopen` / `mmap` |
| `PlatformThread.h` | Création threads, mutex, cond, TLS | `pthread-embedded` | `pthread` natif |
| `PlatformTime.h` | Horloge monotone haute précision | `sceKernelGetSystemTimeWide` | `clock_gettime` |
| `PlatformMemory.h` | Allocation pages, stats mémoire | `sceKernelAllocMemBlock` | `mmap` / `VirtualAlloc` |
| `PlatformInput.h` | Etat pad, touch, gyro, pointer | `sceCtrl` + `sceTouch` + `sceMotion` | SDL2 |
| `PlatformAudio.h` | Sortie PCM bas niveau | OpenAL-Soft Vita | OpenAL-Soft |
| `PlatformLog.h` | Log console / fichier | `psvDebugScreenPrintf` + `sceIo` fichier | `stderr` |
| `PlatformDynLib.h` | Chargement de modules | **stubbé** (tout linké statique) | `dlopen` |

## Principe d'intégration

Les sources upstream utilisent massivement des `#ifdef _WIN32`. On ajoute `#ifdef _VITA` (ou `#ifdef __vita__`) qui redirige vers les implémentations HAL. Jamais de modification directe d'upstream — les changements nécessaires passent soit par `#include "platform/PlatformXXX.h"` soit par des patchs appliqués à la build via CMake.

## Statut

Rien d'implémenté. Phase 1 du projet.
