/***************************************************************************
 *                                                                         *
 * Module  : D3D9rendst.h                                                    *
 *                                                                         *
 * Purpose : Renderstates.                                                 *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9RENDST_H
#define D3D9RENDST_H

#include "batypes.h"
#include "baimmedi.h"

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void _rwD3D9RenderStateOpen(void);

extern void _rwD3D9RenderStateReset(void);

extern void _rwD3D9RenderStateFlushCache(void); /* need to be call before any Draw Primitive */

extern RwBool _rwD3D9RWSetRasterStage(RwRaster *raster, RwUInt32 stage);
extern RwRaster *_rwD3D9RWGetRasterStage(RwUInt32 stage);

extern RwBool _rwD3D9RWSetRenderState(RwRenderState nState, void *pParam);
extern RwBool _rwD3D9RWGetRenderState(RwRenderState nState, void *pParam);

extern void
_rwD3D9SetTextureAnisotropyOffset(RwInt32 anisotTextureOffset);

/* Fast Render State Set functions */
extern RwBool _rwD3D9RenderStateTextureFilter(RwTextureFilterMode filterMode);

extern RwBool _rwD3D9RenderStateTextureAddress(RwTextureAddressMode addressMode);
extern RwBool _rwD3D9RenderStateTextureAddressU(RwTextureAddressMode addressMode);
extern RwBool _rwD3D9RenderStateTextureAddressV(RwTextureAddressMode addressMode);

extern RwBool _rwD3D9RenderStateVertexAlphaEnable(RwBool enable);
extern RwBool _rwD3D9RenderStateIsVertexAlphaEnable(void);

extern RwBool _rwD3D9RenderStateSrcBlend(RwBlendFunction srcBlend);
extern RwBool _rwD3D9RenderStateDestBlend(RwBlendFunction dstBlend);

extern RwBool _rwD3D9RenderStateStencilEnable(RwBool enable);

extern RwBool _rwD3D9RenderStateStencilFail(RwStencilOperation operation);
extern RwBool _rwD3D9RenderStateStencilZFail(RwStencilOperation operation);
extern RwBool _rwD3D9RenderStateStencilPass(RwStencilOperation operation);

extern RwBool _rwD3D9RenderStateStencilFunction(RwStencilFunction function);
extern RwBool _rwD3D9RenderStateStencilFunctionRef(RwInt32 ref);
extern RwBool _rwD3D9RenderStateStencilFunctionMask(RwInt32 mask);
extern RwBool _rwD3D9RenderStateStencilFunctionWriteMask(RwInt32 writeMask);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D9RENDST_H */



