# Build PC GLES (pour tests rapides)

Build desktop Linux/Windows utilisant **OpenGL ES 2.0** (via ANGLE ou Mesa) pour reproduire rapidement les bugs renderer sans avoir à flasher un VPK à chaque itération.

- Même code que le build Vita, même backend `CryRenderer_Vita` (qui est GLES-only)
- Remplace le HAL Vita par un HAL SDL2 (fichier/thread/input/audio)
- Permet d'utiliser gdb, valgrind, RenderDoc

## Statut

Vide. À mettre en place en Phase 2, dès qu'on a un premier triangle dans `CryRenderer_Vita`.
