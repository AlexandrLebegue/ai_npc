# Asset pipeline

Scripts offline pour préparer les assets Far Cry (extraits par le joueur depuis sa copie légitime) au format Vita :

- Redimensionnement des textures (2048→1024, 1024→512, 512→256) selon budget VRAM
- Compression PVRTC (format natif SGX543) via `PVRTexTool`
- Simplification optionnelle des meshes lourds (outils type Simplygon non inclus)
- Repacking en fichiers `.pak` compatibles `ICryPak`

## Non inclus dans ce dépôt

Aucun asset du jeu n'est versionné. Le joueur fournit son propre chemin d'installation Far Cry lors du build.

## Statut

Vide. Scripts à écrire en Phase 2/4.
