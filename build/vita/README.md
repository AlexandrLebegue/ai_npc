# Build Vita

Build cible pour PlayStation Vita via VitaSDK.

## Prérequis

```bash
# Installer vdpm (VitaSDK Package Manager) puis :
export VITASDK=/usr/local/vitasdk
export PATH=$VITASDK/bin:$PATH
vdpm vitaGL
vdpm vorbis
vdpm openal-soft
vdpm pthread-embedded
vdpm zlib
```

## Build (à venir)

```bash
cmake -S ../.. -B . \
  -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build .
# Produit : OpenFarCryVita.vpk
```

## Statut

Vide. CMakeLists racine + toolchain file à écrire en Phase 1.
