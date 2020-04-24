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
#include "d3d8dxttex.h"

/* RWPUBLIC */

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

/* Reports on whether D3D8 can render S3TC textures */
extern RwBool 
RwD3D8DeviceSupportsDXTTexture(void);

/* Get handle to D3D8 device - useful for setting D3D8 renderstate*/
extern void *
RwD3D8GetCurrentD3DDevice(void);

/* Get maximun number of multisampling levels */
extern RwUInt32
RwD3D8EngineGetMaxMultiSamplingLevels(void);

/* Set number of multisampling levels */
extern void
RwD3D8EngineSetMultiSamplingLevels(RwUInt32 numLevels);

/* Set maximun full screen refresh rate */
extern void
RwD3D8EngineSetRefreshRate(RwUInt32 refreshRate);

/* Create the Direct3D device as multithread safe */
extern void
RwD3D8EngineSetMultiThreadSafe(RwBool enable);

/* Create the Direct3D device to use software vertex processing */
extern void
RwD3D8EngineSetSoftwareVertexProcessing(RwBool enable);

/* Get handle to D3D8 rendering surface - useful for advanced blit ops */
extern void *
RwD3D8GetCurrentD3DRenderTarget(void);

/* Change the video mode after RwEngineStart */
extern RwBool
RwD3D8ChangeVideoMode(RwInt32 modeIndex);

/* Atach a window to a camera */
extern RwBool
RwD3D8CameraAttachWindow(void *camera, void *hwnd);

/* Immediate mode rendering */
extern RwBool       RwD3D8SetVertexShader(RwUInt32 handle);
extern RwBool       RwD3D8SetPixelShader(RwUInt32 handle);
extern RwBool       RwD3D8SetStreamSource(RwUInt32 streamNumber, void *streamData, RwUInt32 stride);
extern RwBool       RwD3D8SetIndices(void *indexData, RwUInt32 baseVertexIndex);

extern RwBool       RwD3D8DrawIndexedPrimitiveUP(RwUInt32 primitiveType,
                                                  RwUInt32 minIndex,
                                                  RwUInt32 numVertices,
                                                  RwUInt32 numIndices,
                                                  const void* indexData,
                                                  const void* vertexStreamZeroData,
                                                  RwUInt32 VertexStreamZeroStride);

extern RwBool       RwD3D8DrawPrimitiveUP(RwUInt32 primitiveType,
                                                  RwUInt32 numVertices,
                                                  const void* vertexStreamZeroData,
                                                  RwUInt32 VertexStreamZeroStride);

extern RwBool       RwD3D8DrawIndexedPrimitive(RwUInt32 primitiveType,
                                                  RwUInt32 minIndex,
                                                  RwUInt32 numVertices,
                                                  RwUInt32 startIndex,
                                                  RwUInt32 numIndices);

extern RwBool       RwD3D8DrawPrimitive(RwUInt32 primitiveType,
                                                  RwUInt32 startVertex,
                                                  RwUInt32 numVertices);

extern RwBool       RwD3D8SetTexture(RwTexture *texture, RwUInt32 stage);

extern RwBool       RwD3D8SetRenderState(RwUInt32 state, RwUInt32 value);
extern RwBool       RwD3D8SetTextureStageState(RwUInt32 stage, RwUInt32 type, RwUInt32 value);
extern RwBool       RwD3D8SetTransform(RwUInt32 state, const void *matrix);
extern RwBool       RwD3D8SetMaterial(const void *material);
extern void         RwD3D8SetStencilClear(RwUInt32 stencilClear);


extern void         RwD3D8GetRenderState(RwUInt32 state, void *value);
extern void         RwD3D8GetTextureStageState(RwUInt32 stage, RwUInt32 type, void *value);
extern void         RwD3D8GetTransform(RwUInt32 state, void *matrix);
extern RwUInt32     RwD3D8GetStencilClear(void);

/*
 * To convert between RwMatrix to D3DMATRIX when setting the
 * world matrix
 */
extern RwBool       RwD3D8SetTransformWorld(const RwMatrix *matrix);

/*
 * To convert between RpMaterial to D3DMATERIAL8
 */
extern RwBool       RwD3D8SetSurfaceProperties(const RwRGBA *color,
                                                const RwSurfaceProperties *surfaceProps,
                                                RwBool modulate);

/*
 * To set on that index the light from the descriptor
 */
extern RwBool       RwD3D8SetLight(RwInt32 index, const void *light);
extern void         RwD3D8GetLight(RwInt32 index, void *light);

extern RwBool       RwD3D8EnableLight(RwInt32 index, RwBool enable);

/*
 * To get a pointer to a Dynamic vertex buffer
 * Use both functions, Create and Destroy, not only Create
 */
extern RwBool RwD3D8DynamicVertexBufferCreate(RwUInt32 fvf, RwUInt32 size,
                                               void **vertexBuffer);

extern void RwD3D8DynamicVertexBufferDestroy(void *vertexBuffer);

/*
 * To get a temporary pointer to a Dynamic vertex buffer memory
 * Don't use with the previous functions because
 * it mantains an internal list of dinamic vertex buffers
 */
extern RwBool RwD3D8DynamicVertexBufferLock(RwUInt32 vertexSize,
                                            RwUInt32 numVertex,
                                            void **vertexBufferOut,
                                            void **vertexDataOut,
                                            RwUInt32 *baseIndexOut);

extern RwBool RwD3D8DynamicVertexBufferUnlock(void *vertexBuffer);

/*
 * To get a pointer to an Index vertex buffer
 * It's created as WRITEONLY, MANAGED and D3DFMT_INDEX16
 */
extern RwBool RwD3D8IndexBufferCreate(RwUInt32 numIndices,
                                       void **indexBuffer);

/*
 * To create a vertex shader from the declaration, also the function
 * for deleting it.
 */
extern RwBool RwD3D8CreateVertexShader(const RwUInt32 *declaration,
                                      const RwUInt32 *function,
                                      RwUInt32 *handle,
                                      RwUInt32 usage);

extern void RwD3D8DeleteVertexShader(RwUInt32 handle);

extern RwBool RwD3D8SetVertexShaderConstant(RwUInt32 registerAddress,
                                          const void *constantData,
                                          RwUInt32  constantCount);

/*
 * To create a pixel shader from the function, also the function
 * for deleting it.
 */
extern RwBool RwD3D8CreatePixelShader(const RwUInt32 *function, RwUInt32 *handle);

extern void RwD3D8DeletePixelShader(RwUInt32 handle);

extern RwBool RwD3D8SetPixelShaderConstant(RwUInt32 registerAddress,
                                          const void *constantData,
                                          RwUInt32  constantCount);

/*
 * Get a const pointer to the D3DCAPS8 struct, (you need to do the cast)
 */
extern const void *RwD3D8GetCaps(void);

/*
 * Check if the sphere is fully inside of the frustum
 */
extern RwBool RwD3D8CameraIsSphereFullyInsideFrustum(const void *camera, const void *sphere);

/*
 * Check if the Bounding Box is fully inside of the frustum
 */
extern RwBool RwD3D8CameraIsBBoxFullyInsideFrustum(const void *camera, const void *boundingBox);

/*
 * Cheking the CPU capabilities
 */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RwBool _rwIntelMMXsupported(void);
extern RwBool _rwIntelSSEsupported(void);
extern RwBool _rwIntelSSE2supported(void);
extern RwBool _rwAMD3DNowSupported(void);

#else
extern RwBool _rwD3D8CPUSupportsMMX;
extern RwBool _rwD3D8CPUSupportsSSE;
extern RwBool _rwD3D8CPUSupportsSSE2;
extern RwBool _rwD3D8CPUSupports3DNow;

#define _rwIntelMMXsupported() _rwD3D8CPUSupportsMMX
#define _rwIntelSSEsupported() _rwD3D8CPUSupportsSSE
#define _rwIntelSSE2supported() _rwD3D8CPUSupportsSSE2
#define _rwAMD3DNowSupported() _rwD3D8CPUSupports3DNow
#endif

/*
 * Enabling texture format conversions when loading textures from a texture
 * dictionary. FALSE by default.
 */
extern void
_rwD3D8TexDictionaryEnableRasterFormatConversion(RwBool enable);

/* Called from RwEngineInit to give the driver a chance to register plugins */
extern RwBool _rwDeviceRegisterPlugin(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


/* LEGACY-MACRO */
/**
 * \ingroup rwtextured3d8
 * \ref RwD3DLoadNativeTexture is a legacy macro for compatibility with
 * RWD3D7 applications that can be used to read a compressed texture from
 * the specified DDS file on disk.
 *
 * It is recommended that applications for RWD3D8 should use the RWD3D8
 * function \ref RwD3D8DDSTextureRead instead.
 *
 * \param name Pointer to a string containing the name of the texture to be read.
 * \param maskname  Pointer to a string containing the name of the mask or
 * NULL if not required.
 *
 * \return Returns pointer to the new texture if successful or NULL if there
 * is an error.
 *
 * \see RwD3D8DDSTextureRead
 * \see RwD3D8DeviceSupportsDXTTexture
 * \see RwImageSetPath
 * \see RwTextureSetReadCallBack
 * \see RwTextureSetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureDestroy
 *
 */
#define RwD3DLoadNativeTexture RwD3D8DDSTextureRead

/* RWPUBLICEND */

#endif /* DRVFNS_H */
