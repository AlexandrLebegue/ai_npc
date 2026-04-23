# Pipeline de transpilation shaders

Pipeline **offline** (exécuté sur machine de dev, pas sur Vita) :

```
upstream/RenderDll/XRenderD3D9/Shaders/  (HLSL SM2/SM3 dans .cfx)
    │
    ▼  [extracteur .cfx]       ← à écrire (Python/C++)
variantes HLSL isolées
    │
    ▼  [HLSLcc ou SPIRV-Cross]
GLSL ES 2.0
    │
    ▼  [shacc du VitaSDK]
blobs .gxp
    │
    ▼  [empaquetage]
port/renderer_vita/shaders/compiled.pak   (embarqué dans le VPK)
```

## Notes

- Les fichiers `.cfx` CryEngine 1 ne sont **pas** du pur HLSL : ils embarquent de la méta-info sur les permutations (uber-shader). Un parseur dédié est nécessaire.
- On limite le nombre de permutations générées aux combinaisons effectivement utilisées par les niveaux portés (Training, Carrier, etc.) pour éviter l'explosion combinatoire.
- Cible GLSL ES 2.0 (pas 3.0) car vitaGL ne supporte pas complètement GLES 3.

## Statut

Vide. Script à écrire en Phase 2.
