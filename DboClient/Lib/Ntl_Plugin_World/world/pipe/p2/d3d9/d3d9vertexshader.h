/*****************************************************************************
 *                                                                           *
 * Module  : d3d9vertexshader.h                                              *
 *                                                                           *
 * Purpose : See d3d9vertexshader.c                                          *
 *                                                                           *
 *****************************************************************************/

#ifndef D3D9VERTEXSHADER_H
#define D3D9VERTEXSHADER_H

/*****************************************************************************
 Includes
 */

#include <rwcore.h>
#include <d3d9pipe.h>

/* RWPUBLIC */

/*****************************************************************************
 Global Defines & Enums
 */

#define RWD3D9VSCONST_TRANSFORM_OFFSET  0
#define RWD3D9VSCONST_AMBIENT_OFFSET    4
#define RWD3D9VSCONST_LIGHTS_OFFSET     5

typedef enum rwD3D9VertexShaderEffectType
{
    rwD3D9VERTEXSHADEREFFECT_NONE = 0,
    rwD3D9VERTEXSHADEREFFECT_BUMPMAP = 1,
    rwD3D9VERTEXSHADEREFFECT_ENVMAP,
    rwD3D9VERTEXSHADEREFFECT_BUMPENVMAP,
    rwD3D9VERTEXSHADEREFFECT_DUAL,
    rwD3D9VERTEXSHADEREFFECT_DUALREPLICATE,
    rwD3D9VERTEXSHADEREFFECT_UVANIM,
    rwD3D9VERTEXSHADEREFFECT_DUALUVANIM,
    rwD3D9VERTEXSHADEREFFECT_NORMALMAP,
    rwD3D9VERTEXSHADEREFFECT_NORMALENVMAP,
    rwD3D9VERTEXSHADEREFFECT_CUBEMAP,
    rwD3D9VERTEXSHADEREFFECT_BUMPCUBEMAP,

    rwD3D9VERTEXSHADEREFFECT_MAX = (1 << 6),

} rwD3D9VertexShaderEffectType;


typedef enum rwD3D9VertexShaderFogMode
{
    rwD3D9VERTEXSHADERFOG_NONE = 0,
    rwD3D9VERTEXSHADERFOG_LINEAR = 1,
    rwD3D9VERTEXSHADERFOG_EXP,
    rwD3D9VERTEXSHADERFOG_EXP2,

    rwD3D9VERTEXSHADERFOG_MAX
} rwD3D9VertexShaderFogMode;

/******************************************************************************
 Global Types
 */

#if !defined(DOXYGEN)

/* This struct is used to specify the required functionality of the vertex shader */
typedef struct _rpD3D9VertexShaderDescriptor
{
    RwUInt8 numDirectionalLights : 4;
    RwUInt8 numPointLights : 4;
    RwUInt8 numSpotLights : 4;
    RwUInt8 numTexCoords : 4;

    RwUInt8 morphing : 1;
    RwUInt8 numWeights : 3;
    RwUInt8 prelit : 1;
    RwUInt8 normals : 1;
    RwUInt8 normalizeNormals : 1;
    RwUInt8 modulateMaterial : 1;
    RwUInt8 fogMode : 2;
    RwUInt8 effect : 6;

} _rpD3D9VertexShaderDescriptor;

/*
 * Offsets for the required vertex shader constants.
 * The combined transposed matrix world-view-projection is always at offset 0.
 * The ambient is always at offset 4.
 * The first light is always at offset 5.
 * 0xFF indicates that the specified constant is not used.
 */
typedef struct _rpD3D9VertexShaderDispatchDescriptor
{
    RwUInt8 offsetMaterialColor;
    RwUInt8 offsetFogRange;
    RwUInt8 offsetEffect;
    RwUInt8 offsetMorphingCoef;
    RwUInt8 offsetBoneMatrices;
    RwUInt8 pad[3];

} _rpD3D9VertexShaderDispatchDescriptor;

#endif /* !defined(DOXYGEN) */

/*
 * Vertex shader pipeline callbacks
 */

typedef struct RxD3D9ResEntryHeader RxD3D9ResEntryHeader;
typedef struct RxD3D9InstanceData RxD3D9InstanceData;
typedef struct RpMaterial RpMaterial;

/* Initialize shared components of "desc" and to prepare internal render data */

typedef RwBool (*_rxD3D9VertexShaderInstanceCallBack)(void *object,
                                                      RxD3D9ResEntryHeader *resEntryHeader,
                                                      RwBool reinstance);

typedef void (*_rxD3D9VertexShaderBeginCallBack)(void *object,
                                                 RwUInt32 type,
                                                 _rpD3D9VertexShaderDescriptor  *desc);

/* _rwD3D9VSSetActiveWorldMatrix MUST be called before using this two callbacks */

typedef RwV4d * (*_rxD3D9VertexShaderLightingCallBack)(void *object,
                                                       RwUInt32 type,
                                                       RwV4d    *shaderConstantPtr,
                                                       _rpD3D9VertexShaderDescriptor  *desc);

typedef void *(*_rxD3D9VertexShaderGetMaterialShaderCallBack)(const RpMaterial *material,
                                                              _rpD3D9VertexShaderDescriptor *desc,
                                                              _rpD3D9VertexShaderDispatchDescriptor *dispatch);


typedef void (*_rxD3D9VertexShaderMeshRenderCallBack)(RxD3D9ResEntryHeader *resEntryHeader,
                                                      RxD3D9InstanceData *instancedMesh,
                                                      const _rpD3D9VertexShaderDescriptor  *desc,
                                                      const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

typedef void (*_rxD3D9VertexShaderEndCallBack)(void *object,
                                               RwUInt32 type,
                                               _rpD3D9VertexShaderDescriptor  *desc);

/* RWPUBLICEND */

typedef struct __rxD3D9VertexShaderInstanceNodeData _rxD3D9VertexShaderInstanceNodeData;
struct __rxD3D9VertexShaderInstanceNodeData
{
    _rxD3D9VertexShaderInstanceCallBack     instanceCallback;
    _rxD3D9VertexShaderBeginCallBack        beginCallback;
    _rxD3D9VertexShaderLightingCallBack     lightingCallback;
    _rxD3D9VertexShaderGetMaterialShaderCallBack getmaterialshaderCallback;
    _rxD3D9VertexShaderMeshRenderCallBack   meshRenderCallback;
    _rxD3D9VertexShaderEndCallBack          endCallback;
};

/*****************************************************************************
 Global variables
 */
extern RwV4d _rxD3D9VertexShaderConstants[];

extern RwUInt32 _rwD3D9VertexShaderMaxConstants;

/* RWPUBLIC */
/*****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* RWPUBLICEND */

extern void
_rpD3D9VertexShaderCacheOpen(void);

/* RWPUBLIC */
/*
 * This function frees the vertex shaders cache.
 */
extern void
_rpD3D9VertexShaderCachePurge(void);

/*
 * This function returns the vertex shader that supports the "desc" operations
 * and fills the info on the "dispatch" variable.
 */
extern void *
_rpD3D9GetVertexShader(const _rpD3D9VertexShaderDescriptor *desc,
                       _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern RwUInt32
_rpD3D9GetNumConstantsUsed(const _rpD3D9VertexShaderDescriptor *desc);

/* Vertex shader pipeline functions */
extern void
_rxD3D9VertexShaderDefaultBeginCallBack(void *object,
                                        RwUInt32 type,
                                        _rpD3D9VertexShaderDescriptor  *desc);

extern RwV4d *
_rxD3D9VertexShaderDefaultLightingCallBack(void *object,
                                           RwUInt32 type,
                                           RwV4d    *shaderConstantPtr,
                                           _rpD3D9VertexShaderDescriptor  *desc);

extern void *
_rxD3D9VertexShaderDefaultGetMaterialShaderCallBack(const RpMaterial *material,
                                                    _rpD3D9VertexShaderDescriptor *desc,
                                                    _rpD3D9VertexShaderDispatchDescriptor *dispatch);
extern void
_rxD3D9VertexShaderDefaultMeshRenderCallBack(RxD3D9ResEntryHeader *resEntryHeader,
                                             RxD3D9InstanceData *instancedMesh,
                                             const _rpD3D9VertexShaderDescriptor  *desc,
                                             const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern void
_rxD3D9VertexShaderDefaultEndCallBack(void *object,
                                      RwUInt32 type,
                                      _rpD3D9VertexShaderDescriptor  *desc);

/* Useful vertex shader functions */
extern void
_rpD3D9VertexShaderUpdateLightsColors(RwV4d *shaderConstantPtr,
                                      const _rpD3D9VertexShaderDescriptor  *desc,
                                      RwReal ambientCoef,
                                      RwReal diffuseCoef);

extern void
_rpD3D9VertexShaderUpdateMaterialColor(const RwRGBA *color,
                                       const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern void
_rpD3D9VertexShaderUpdateFogData(const _rpD3D9VertexShaderDescriptor  *desc,
                                 const _rpD3D9VertexShaderDispatchDescriptor *dispatch);
extern void
_rpD3D9VertexShaderUpdateMorphingCoef(RwReal morphingCoef,
                                      const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern void
_rpD3DVertexShaderSetUVAnimMatrix(RwMatrix *matrix,
                                  const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern void
_rpD3D9VertexShaderSetEnvMatrix(RwFrame *frame,
                                const _rpD3D9VertexShaderDescriptor  *desc,
                                const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

extern void
_rpD3D9VertexShaderSetBumpMatrix(RwFrame *frame,
                                 RwReal factor,
                                 const _rpD3D9VertexShaderDispatchDescriptor *dispatch);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9VERTEXSHADER_H */
