#ifndef D3D9TOON_H
#define D3D9TOON_H

extern void
_rpToonD3D9RenderCallback(RwResEntry *repEntry,
                             void *object,
                             RwUInt8 type,
                             RwUInt32 flags);

extern void _rpToonD3D9BackfaceExtrudeInit();

extern void _rpToonD3D9BackfaceExtrudeDestroy();

extern void _rpToonD3D9BackfaceExtrudeRender
    (
    RwResEntry *repEntry,
    void *object,
    RwUInt8 type,
    RwUInt32 flags,
    RpToonGeo *toonGeo
    );

/* Internal driver functions */
extern RwBool _rwD3D9RenderStateVertexAlphaEnable(RwBool enable);
extern RwBool _rwD3D9RenderStateSrcBlend(RwBlendFunction srcBlend);
extern RwBool _rwD3D9RenderStateDestBlend(RwBlendFunction dstBlend);

#endif
