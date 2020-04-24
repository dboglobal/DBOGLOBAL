/***************************************************************************
 *                                                                         *
 * Module  : drvfns.h (xbox)                                               *
 *                                                                         *
 * Purpose : Driver functionality                                          *
 *                                                                         *
 **************************************************************************/

#ifndef DRVFNS_H
#define DRVFNS_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "batextur.h"
#include "d3d9dxttex.h"

/* RWPUBLIC */

#define RWD3D9_MAX_TEXTURE_STAGES  8

#define RWD3D9_MAX_VERTEX_STREAMS  2

typedef struct RxD3D9VertexStream RxD3D9VertexStream;
/**
 * \ingroup worldextensionsd3d9
 * \struct RxD3D9VertexStream
 * This structure contains D3D9 resource specific components.
 */
struct RxD3D9VertexStream
{
    void *vertexBuffer;     /**< Vertex buffer */
    RwUInt32 offset;        /**< Offset in bytes since the beginning
                                 of the Vertex buffer */
    RwUInt32 stride;        /**< Size of the components in bytes */
    RwUInt16 geometryFlags; /**< Geometry locked flags */
    RwUInt8 managed;        /**< Created by the Vertex Buffer Manager */
    RwUInt8 dynamicLock;    /**< Using RwD3D9DynamicVertexBufferLock */
};

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/****************************************************************************
 Function prototypes
 */

/*******/
/* API */
/*******/

/* Reports on whether D3D9 can render S3TC textures */
extern RwBool 
RwD3D9DeviceSupportsDXTTexture(void);

/* Get handle to D3D9 device - useful for setting D3D9 renderstate*/
extern void *
RwD3D9GetCurrentD3DDevice(void);

/* Get maximun number of multisampling levels */
extern RwUInt32
RwD3D9EngineGetMaxMultiSamplingLevels(void);

/* Set number of multisampling levels */
extern void
RwD3D9EngineSetMultiSamplingLevels(RwUInt32 numLevels);

/* Set maximun full screen refresh rate */
extern void
RwD3D9EngineSetRefreshRate(RwUInt32 refreshRate);

/* Create the Direct3D device as multithread safe */
extern void
RwD3D9EngineSetMultiThreadSafe(RwBool enable);

/* Create the Direct3D device to use software vertex processing */
extern void
RwD3D9EngineSetSoftwareVertexProcessing(RwBool enable);

/* Create the Direct3D device to support GDI dialog boxes on fullscreen */
extern void
RwD3D9EngineSetFullScreenDialogBoxMode(RwBool enable);

/* Get handle to D3D9 rendering surface - useful for advanced blit ops */
extern void *
RwD3D9GetCurrentD3DRenderTarget(RwUInt32 index);

extern RwBool
RwD3D9SetRenderTarget(RwUInt32 index, RwRaster *raster);

/* Change the video mode after RwEngineStart */
extern RwBool
RwD3D9ChangeVideoMode(RwInt32 modeIndex);

/* Change multisampling levels after RwEngineStart */
extern RwBool
RwD3D9ChangeMultiSamplingLevels(RwUInt32 numLevels);

/* Atach a window to a camera */
extern RwBool
RwD3D9CameraAttachWindow(void *camera, void *hwnd);

/* Immediate mode rendering */
extern void
RwD3D9SetStreamSource(RwUInt32 streamNumber,
                      void *streamData,
                      RwUInt32 offset,
                      RwUInt32 stride);

/* Draw Primitive functions */
#if (!defined(RWDEBUG) && !defined(RWSUPPRESSINLINE) && !defined(RWMETRICS))

#ifdef _D3D9_H_

extern LPDIRECT3DDEVICE9    _RwD3DDevice;

extern RwUInt32 _rwD3D9LastFVFUsed;
extern void     *_rwD3D9LastVertexDeclarationUsed;
extern void     *_rwD3D9LastVertexShaderUsed;
extern void     *_rwD3D9LastPixelShaderUsed;
extern void     *_rwD3D9LastIndexBufferUsed;

/* need to be call before any Draw Primitive */
extern void
_rwD3D9RenderStateFlushCache(void);

#define _rwD3D9DrawIndexedPrimitiveUPMacro(primitiveType,           \
                                           minIndex,                \
                                           numVertices,             \
                                           primitiveCount,          \
                                           indexData,               \
                                           vertexStreamZeroData,    \
                                           vertexStreamZeroStride)  \
{                                                                   \
    RwD3D9SetStreamSource(0, NULL, 0, 0);                           \
    RwD3D9SetIndices(NULL);                                         \
    _rwD3D9RenderStateFlushCache();                                 \
    IDirect3DDevice9_DrawIndexedPrimitiveUP(_RwD3DDevice,           \
                                            primitiveType,          \
                                            minIndex,               \
                                            numVertices,            \
                                            primitiveCount,         \
                                            indexData,              \
                                            D3DFMT_INDEX16,         \
                                            vertexStreamZeroData,   \
                                            vertexStreamZeroStride);\
}

#define _rwD3D9DrawPrimitiveUPMacro(primitiveType,              \
                                    primitiveCount,             \
                                    vertexStreamZeroData,       \
                                    vertexStreamZeroStride)     \
{                                                               \
    RwD3D9SetStreamSource(0, NULL, 0, 0);                       \
    _rwD3D9RenderStateFlushCache();                             \
    IDirect3DDevice9_DrawPrimitiveUP(_RwD3DDevice,              \
                                     primitiveType,             \
                                     primitiveCount,            \
                                     vertexStreamZeroData,      \
                                     vertexStreamZeroStride);   \
}

#define _rwD3D9DrawIndexedPrimitiveMacro(primitiveType,     \
                                         baseVertexIndex,   \
                                         minIndex,          \
                                         numVertices,       \
                                         startIndex,        \
                                         primitiveCount)    \
{                                                           \
    _rwD3D9RenderStateFlushCache();                         \
    IDirect3DDevice9_DrawIndexedPrimitive(_RwD3DDevice,     \
                                          primitiveType,    \
                                          baseVertexIndex,  \
                                          minIndex,         \
                                          numVertices,      \
                                          startIndex,       \
                                          primitiveCount);  \
}

#define _rwD3D9DrawPrimitiveMacro(primitiveType,    \
                                  startVertex,      \
                                  primitiveCount)   \
{                                                   \
    _rwD3D9RenderStateFlushCache();                 \
    IDirect3DDevice9_DrawPrimitive(_RwD3DDevice,    \
                                   primitiveType,   \
                                   startVertex,     \
                                   primitiveCount); \
}


#define _rwD3D9SetVertexShaderConstantMacro(registerAddress,    \
                                            constantData,       \
                                            constantCount)      \
{                                                               \
    IDirect3DDevice9_SetVertexShaderConstantF(_RwD3DDevice,     \
                                              registerAddress,  \
                                              (const RwReal *)(constantData),     \
                                              constantCount);   \
}

#define _rwD3D9SetPixelShaderConstantMacro(registerAddress,     \
                                           constantData,        \
                                           constantCount)       \
{                                                               \
    IDirect3DDevice9_SetPixelShaderConstantF(_RwD3DDevice,      \
                                             registerAddress,   \
                                             (const RwReal *)(constantData),  \
                                             constantCount);    \
}

#define _rwD3D9SetFVFMacro(_fvf)                                \
{                                                               \
    if (_rwD3D9LastFVFUsed != (_fvf))                           \
    {                                                           \
        _rwD3D9LastFVFUsed = _fvf;                              \
        _rwD3D9LastVertexDeclarationUsed = (void *)0xffffffff;  \
        IDirect3DDevice9_SetFVF(_RwD3DDevice, _fvf);            \
    }                                                           \
}


#define _rwD3D9SetVertexDeclarationMacro(_vertexDeclaration)    \
{                                                               \
    if (_rwD3D9LastVertexDeclarationUsed != (_vertexDeclaration)) \
    {                                                           \
        _rwD3D9LastVertexDeclarationUsed = _vertexDeclaration;  \
        _rwD3D9LastFVFUsed = 0xffffffff;                        \
        IDirect3DDevice9_SetVertexDeclaration(_RwD3DDevice, _vertexDeclaration);    \
    }                                                           \
}

#define _rwD3D9SetVertexShaderMacro(_shader)                    \
{                                                               \
    if (_rwD3D9LastVertexShaderUsed != (_shader))               \
    {                                                           \
        _rwD3D9LastVertexShaderUsed = _shader;                  \
        IDirect3DDevice9_SetVertexShader(_RwD3DDevice, _shader);    \
    }                                                           \
}

#define _rwD3D9SetPixelShaderMacro(_shader)                     \
{                                                               \
    if (_rwD3D9LastPixelShaderUsed != (_shader))                \
    {                                                           \
        _rwD3D9LastPixelShaderUsed = _shader;                   \
        IDirect3DDevice9_SetPixelShader(_RwD3DDevice, _shader); \
    }                                                           \
}

#define _rwD3D9SetIndicesMacro(_indexBuffer)                    \
{                                                               \
    if (_rwD3D9LastIndexBufferUsed != (_indexBuffer))           \
    {                                                           \
        _rwD3D9LastIndexBufferUsed = _indexBuffer;              \
        IDirect3DDevice9_SetIndices(_RwD3DDevice, _indexBuffer);    \
    }                                                           \
}

#endif  /* _D3D9_H_ */

#endif  /* (!defined(RWDEBUG) && !defined(RWSUPPRESSINLINE) && !defined(RWMETRICS)) */

extern void
_rwD3D9DrawIndexedPrimitiveUP(RwUInt32 primitiveType,
                              RwUInt32 minIndex,
                              RwUInt32 numVertices,
                              RwUInt32 primitiveCount,
                              const void *indexData,
                              const void *vertexStreamZeroData,
                              RwUInt32 VertexStreamZeroStride);

extern void
_rwD3D9DrawPrimitiveUP(RwUInt32 primitiveType,
                       RwUInt32 primitiveCount,
                       const void *vertexStreamZeroData,
                       RwUInt32 VertexStreamZeroStride);

extern void
_rwD3D9DrawIndexedPrimitive(RwUInt32 primitiveType,
                            RwInt32 baseVertexIndex,
                            RwUInt32 minIndex,
                            RwUInt32 numVertices,
                            RwUInt32 startIndex,
                            RwUInt32 primitiveCount);

extern void
_rwD3D9DrawPrimitive(RwUInt32 primitiveType,
                     RwUInt32 startVertex,
                     RwUInt32 primitiveCount);

extern void
_rwD3D9SetVertexShaderConstant(RwUInt32 registerAddress,
                               const void *constantData,
                               RwUInt32  constantCount);

extern void
_rwD3D9SetPixelShaderConstant(RwUInt32 registerAddress,
                              const void *constantData,
                              RwUInt32  constantCount);

extern void
_rwD3D9SetFVF(RwUInt32 fvf);

extern void
_rwD3D9SetVertexDeclaration(void *vertexDeclaration);

extern void
_rwD3D9SetVertexShader(void *shader);

extern void
_rwD3D9SetPixelShader(void *shader);

extern void
_rwD3D9SetIndices(void *indexBuffer);


#ifdef _rwD3D9DrawIndexedPrimitiveUPMacro
#define RwD3D9DrawIndexedPrimitiveUP    _rwD3D9DrawIndexedPrimitiveUPMacro
#else
#define RwD3D9DrawIndexedPrimitiveUP    _rwD3D9DrawIndexedPrimitiveUP
#endif

#ifdef _rwD3D9DrawPrimitiveUPMacro
#define RwD3D9DrawPrimitiveUP    _rwD3D9DrawPrimitiveUPMacro
#else
#define RwD3D9DrawPrimitiveUP    _rwD3D9DrawPrimitiveUP
#endif

#ifdef _rwD3D9DrawIndexedPrimitiveMacro
#define RwD3D9DrawIndexedPrimitive    _rwD3D9DrawIndexedPrimitiveMacro
#else
#define RwD3D9DrawIndexedPrimitive    _rwD3D9DrawIndexedPrimitive
#endif

#ifdef _rwD3D9DrawPrimitiveMacro
#define RwD3D9DrawPrimitive    _rwD3D9DrawPrimitiveMacro
#else
#define RwD3D9DrawPrimitive    _rwD3D9DrawPrimitive
#endif

#ifdef _rwD3D9SetVertexShaderConstantMacro
#define RwD3D9SetVertexShaderConstant    _rwD3D9SetVertexShaderConstantMacro
#else
#define RwD3D9SetVertexShaderConstant    _rwD3D9SetVertexShaderConstant
#endif

#ifdef _rwD3D9SetPixelShaderConstantMacro
#define RwD3D9SetPixelShaderConstant    _rwD3D9SetPixelShaderConstantMacro
#else
#define RwD3D9SetPixelShaderConstant    _rwD3D9SetPixelShaderConstant
#endif

#ifdef _rwD3D9SetFVFMacro
#define RwD3D9SetFVF    _rwD3D9SetFVFMacro
#else
#define RwD3D9SetFVF    _rwD3D9SetFVF
#endif

#ifdef _rwD3D9SetVertexDeclarationMacro
#define RwD3D9SetVertexDeclaration    _rwD3D9SetVertexDeclarationMacro
#else
#define RwD3D9SetVertexDeclaration    _rwD3D9SetVertexDeclaration
#endif

#ifdef _rwD3D9SetVertexShaderMacro
#define RwD3D9SetVertexShader    _rwD3D9SetVertexShaderMacro
#else
#define RwD3D9SetVertexShader    _rwD3D9SetVertexShader
#endif

#ifdef _rwD3D9SetPixelShaderMacro
#define RwD3D9SetPixelShader    _rwD3D9SetPixelShaderMacro
#else
#define RwD3D9SetPixelShader    _rwD3D9SetPixelShader
#endif

#ifdef _rwD3D9SetIndicesMacro
#define RwD3D9SetIndices    _rwD3D9SetIndicesMacro
#else
#define RwD3D9SetIndices    _rwD3D9SetIndices
#endif


extern void
RwD3D9SetRenderState(RwUInt32 state, RwUInt32 value);

extern void
RwD3D9GetRenderState(RwUInt32 state, void *value);

extern void
RwD3D9SetTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value);

extern void
RwD3D9GetTextureStageState(RwUInt32 stage, RwUInt32 type, void *value);

extern void
RwD3D9SetSamplerState(RwUInt32 stage, RwUInt32 type, RwUInt32 value);

extern void
RwD3D9GetSamplerState(RwUInt32 stage, RwUInt32 type, void *value);


extern void
_rwD3D9ForceRenderState(RwUInt32 state, RwUInt32 value);

extern void
_rwD3D9ForceTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value);

extern void
_rwD3D9ForceSamplerState(RwUInt32 stage, RwUInt32 type, RwUInt32 value);


extern void
RwD3D9SetStencilClear(RwUInt32 stencilClear);

extern RwUInt32
RwD3D9GetStencilClear(void);


extern RwBool
RwD3D9SetTexture(RwTexture *texture, RwUInt32 stage);

extern RwBool
RwD3D9SetTransform(RwUInt32 state, const void *matrix);

extern void
RwD3D9GetTransform(RwUInt32 state, void *matrix);

extern RwBool
RwD3D9SetMaterial(const void *material);

extern RwBool
RwD3D9SetClipPlane(RwUInt32 index, const RwV4d *plane);


/*
 * To convert between RwMatrix to D3DMATRIX when setting the
 * world matrix
 */
extern RwBool       RwD3D9SetTransformWorld(const RwMatrix *matrix);

/*
 * To convert between RpMaterial to D3DMATERIAL9
 */
extern RwBool       RwD3D9SetSurfaceProperties(const RwSurfaceProperties *surfaceProps,
                                               const RwRGBA *color,
                                               RwUInt32 flags);

/*
 * To set on that index the light from the descriptor
 */
extern RwBool       RwD3D9SetLight(RwInt32 index, const void *light);
extern void         RwD3D9GetLight(RwInt32 index, void *light);

extern RwBool       RwD3D9EnableLight(RwInt32 index, RwBool enable);

/*
 * To get a pointer to an Index vertex buffer
 * It's created as WRITEONLY, MANAGED and D3DFMT_INDEX16
 */
extern RwBool RwD3D9IndexBufferCreate(RwUInt32 numIndices,
                                       void **indexBuffer);

/* Vertex declaration */
extern RwBool
RwD3D9CreateVertexDeclaration(const void *elements,
                              void **vertexdeclaration);

extern void
RwD3D9DeleteVertexDeclaration(void *vertexdeclaration);

/*
 * To create a vertex shader from the declaration, also the function
 * for deleting it.
 */
extern RwBool RwD3D9CreateVertexShader(const RwUInt32 *function,
                                       void **shader);

extern void RwD3D9DeleteVertexShader(void *shader);

/*
 * To create a pixel shader from the function, also the function
 * for deleting it.
 */
extern RwBool RwD3D9CreatePixelShader(const RwUInt32 *function, void **shader);

extern void RwD3D9DeletePixelShader(void *shader);

/*
 * Utility function for setting streams
 */
extern void
_rwD3D9SetStreams(const RxD3D9VertexStream *streams,
                  RwBool useOffsets);

/*
 * Get a const pointer to the D3DCAPS9 struct, (you need to do the cast)
 */
extern const void *RwD3D9GetCaps(void);

/*
 * Check if the sphere is fully inside of the frustum
 */
extern RwBool RwD3D9CameraIsSphereFullyInsideFrustum(const void *camera, const void *sphere);

/*
 * Check if the Bounding Box is fully inside of the frustum
 */
extern RwBool RwD3D9CameraIsBBoxFullyInsideFrustum(const void *camera, const void *boundingBox);

/*
 * Create native rasters
 */
extern RwRaster *
RwD3D9RasterCreate(RwUInt32 width,
                   RwUInt32 height,
                   RwUInt32 d3dFormat,
                   RwUInt32 flags);

/*
 * Convert palettized texture to a non palettized one
 */
extern void
_rwD3D9RasterConvertToNonPalettized(RwRaster *raster);

/*
 * Cheking the CPU capabilities
 */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RwBool _rwIntelMMXsupported(void);
extern RwBool _rwIntelSSEsupported(void);
extern RwBool _rwIntelSSE2supported(void);
extern RwBool _rwAMD3DNowSupported(void);

#else
extern RwBool _rwD3D9CPUSupportsMMX;
extern RwBool _rwD3D9CPUSupportsSSE;
extern RwBool _rwD3D9CPUSupportsSSE2;
extern RwBool _rwD3D9CPUSupports3DNow;

#define _rwIntelMMXsupported() _rwD3D9CPUSupportsMMX
#define _rwIntelSSEsupported() _rwD3D9CPUSupportsSSE
#define _rwIntelSSE2supported() _rwD3D9CPUSupportsSSE2
#define _rwAMD3DNowSupported() _rwD3D9CPUSupports3DNow
#endif

/* Called from RwEngineInit to give the driver a chance to register plugins */
extern RwBool _rwDeviceRegisterPlugin(void);

/*
 * Set a release device callback that would be called after a device lost.
 * Be careful to call the previous callback (if different than NULL),
 * right at the beginning of you own callback.
 */
typedef void (*rwD3D9DeviceReleaseCallBack)(void);

extern void
_rwD3D9DeviceSetReleaseCallback(rwD3D9DeviceReleaseCallBack callback);

extern rwD3D9DeviceReleaseCallBack
_rwD3D9DeviceGetReleaseCallback(void);

/*
 * Set a restore device callback that would be called after a device has been reset.
 * Be careful to call the previous callback (if different than NULL),
 * right at the beginning of you own callback.
 */
typedef void (*rwD3D9DeviceRestoreCallBack)(void);

extern void
_rwD3D9DeviceSetRestoreCallback(rwD3D9DeviceRestoreCallBack callback);

extern rwD3D9DeviceRestoreCallBack
_rwD3D9DeviceGetRestoreCallback(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* DRVFNS_H */
