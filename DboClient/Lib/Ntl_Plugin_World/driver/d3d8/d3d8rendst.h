/***************************************************************************
 *                                                                         *
 * Module  : D3D8rendst.h                                                    *
 *                                                                         *
 * Purpose : Renderstates.                                                 *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8RENDST_H
#define D3D8RENDST_H

#include "batypes.h"
#include "baimmedi.h"

/****************************************************************************
 Function prototypes
 */

#define MAX_TEXTURE_STAGES  8

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void _rwD3D8RenderStateOpen(void);

extern void _rwD3D8RenderStateReset(void);

extern void _rwD3D8RenderStateFlushCache(void); /* need to be call before any Draw Primitive */

extern RwBool _rwD3D8RWSetRasterStage(RwRaster *raster, RwUInt32 stage);
extern RwRaster *_rwD3D8RWGetRasterStage(RwUInt32 stage);

extern RwBool _rwD3D8RWSetRenderState(RwRenderState nState, void *pParam);
extern RwBool _rwD3D8RWGetRenderState(RwRenderState nState, void *pParam);

extern void
_rwD3D8SetTextureAnisotropyOffset(RwInt32 anisotTextureOffset);

/* Fast Render State Set functions */
extern RwBool _rwD3D8RenderStateTextureFilter(RwTextureFilterMode filterMode);

extern RwBool _rwD3D8RenderStateTextureAddress(RwTextureAddressMode addressMode);
extern RwBool _rwD3D8RenderStateTextureAddressU(RwTextureAddressMode addressMode);
extern RwBool _rwD3D8RenderStateTextureAddressV(RwTextureAddressMode addressMode);

extern RwBool _rwD3D8RenderStateVertexAlphaEnable(RwBool enable);
extern RwBool _rwD3D8RenderStateIsVertexAlphaEnable(void);

extern RwBool _rwD3D8RenderStateSrcBlend(RwBlendFunction srcBlend);
extern RwBool _rwD3D8RenderStateDestBlend(RwBlendFunction dstBlend);

extern RwBool _rwD3D8RenderStateStencilEnable(RwBool enable);

extern RwBool _rwD3D8RenderStateStencilFail(RwStencilOperation operation);
extern RwBool _rwD3D8RenderStateStencilZFail(RwStencilOperation operation);
extern RwBool _rwD3D8RenderStateStencilPass(RwStencilOperation operation);

extern RwBool _rwD3D8RenderStateStencilFunction(RwStencilFunction function);
extern RwBool _rwD3D8RenderStateStencilFunctionRef(RwInt32 ref);
extern RwBool _rwD3D8RenderStateStencilFunctionMask(RwInt32 mask);
extern RwBool _rwD3D8RenderStateStencilFunctionWriteMask(RwInt32 writeMask);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D8RENDST_H */



