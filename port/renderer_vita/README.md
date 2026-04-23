# CryRenderer_Vita

Réécriture du renderer CryEngine 1 (`upstream/RenderDll/`, backend DirectX 9) au-dessus de **vitaGL** (OpenGL ES 2.0 sur GXM).

## Portée

- Implémente l'interface `IRenderer` (définie dans `upstream/CryCommon/IRenderer.h`)
- Remplace intégralement `upstream/RenderDll/XRenderD3D9/`
- Ne s'appuie **que** sur vitaGL + `port/hal/` — aucune API Win32 ni D3D

## Blocs fonctionnels à implémenter

| Bloc | Priorité | Complexité |
|---|---|---|
| Swap chain, clear, viewport | P0 | Faible |
| Vertex/Index buffers, layouts | P0 | Faible |
| Textures 2D + mipmaps + PVRTC | P0 | Moyenne |
| Shaders (GLSL ES 2.0) + uniforms | P0 | Moyenne |
| Mesh statique | P1 | Moyenne |
| Skinning (squelettes) | P1 | Moyenne |
| Terrain (LOD + texture splatting) | P1 | Élevée |
| Végétation (instanciée, alpha-test) | P2 | Élevée |
| Eau (réflexion/réfraction simplifiée) | P2 | Élevée |
| Post-FX (bloom, tone map) | P2 | Moyenne |
| Render to texture | P1 | Moyenne |
| Ombres dynamiques (simplifiées) | P2 | Élevée |

## Shaders

- Source : fichiers `.cfx` dans les assets Far Cry (HLSL SM2/SM3 + méta combinatoires)
- Pipeline : `tools/shader-transpile/` extrait chaque permutation utile, convertit HLSL → GLSL ES 2.0 via HLSLcc ou SPIRV-Cross, puis compile via `shacc` du VitaSDK pour produire des blobs binaires embarqués dans un pak spécifique.
- Fallback : shaders fixes écrits à la main pour les effets trop complexes à transpiler automatiquement.

## Contraintes plateforme

- VRAM 128 Mo → atlas + streaming + PVRTC obligatoires
- GLES 2.0 uniquement : pas d'instancing natif (émulation manuelle via vertex streams), pas de compute, pas de MRT généralisé
- Y flip : D3D9 origine haut-gauche vs GL bas-gauche → flip des coordonnées texture ou du viewport selon le cas

## Statut

Rien d'implémenté. Phase 2 du projet.
