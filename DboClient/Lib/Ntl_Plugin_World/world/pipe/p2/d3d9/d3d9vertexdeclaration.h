/*****************************************************************************
 *                                                                           *
 * Module  : d3d9vertexdeclaration.h                                         *
 *                                                                           *
 * Purpose : See d3d9vertexdeclaration.c                                     *
 *                                                                           *
 *****************************************************************************/

#ifndef D3D9VERTEXDECLARATION_H
#define D3D9VERTEXDECLARATION_H

/*****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "baworld.h"

#include "d3d9pipe.h"

/*****************************************************************************
 Global Defines
 */

/******************************************************************************
 Global Types
 */

/* RWPUBLIC */

/*****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwUInt32
_rpD3D9VertexDeclarationGetSize(RwUInt32 type);

extern RwUInt32
_rpD3D9VertexDeclarationGetStride(const void *vertexDeclaration);

extern RwUInt32
_rpD3D9VertexDeclarationInstV3d(RwUInt32 type,
                                RwUInt8 *mem,
                                const RwV3d *src,
                                RwInt32 numVerts,
                                RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstV3dComp(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RpVertexNormal *src,
                                    RwInt32 numVerts,
                                    RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstV3dMorph(RwUInt32 type,
                                     RwUInt8 *mem,
                                     const RwV3d *src1,
                                     const RwV3d *src2,
                                     RwReal scale,
                                     RwInt32 numVerts,
                                     RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstV2d(RwUInt32 type,
                                RwUInt8 *mem,
                                const RwV2d *src,
                                RwInt32 numVerts,
                                RwUInt32 stride);

extern RwBool
_rpD3D9VertexDeclarationInstColor(RwUInt8 *mem,
                                  const RwRGBA *color,
                                  RwInt32 numVerts,
                                  RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstIndices(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwUInt32 *indices,
                                    RwInt32 numVerts,
                                    RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstIndicesRemap(RwUInt32 type,
                                         RwUInt8 *mem,
                                         const RwUInt32 *indices,
                                         const RwUInt8 *remap,
                                         RwInt32 numVerts,
                                         RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstWeights(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwV4d *src,
                                    RwInt32 numVerts,
                                    RwUInt32 stride);

extern RwUInt32
_rpD3D9VertexDeclarationInstTangent(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwV3d *pos,
                                    const RwV3d *normal,
                                    const RpVertexNormal *packedNormal,
                                    const RwTexCoords *texCoord,
                                    RxD3D9ResEntryHeader *meshHeader,
                                    RwUInt32 stride);

extern void
_rpD3D9VertexDeclarationUnInstV3d(RwUInt32 type,
                                  RwV3d *dst,
                                  const RwUInt8 *src);

extern void
_rpD3D9VertexDeclarationUnInstV2d(RwUInt32 type,
                                  RwV2d *dst,
                                  const RwUInt8 *src);

extern void
_rpD3D9GetMinMaxValuesV3d(const RwV3d *src,
                          RwInt32 numElements,
                          RwV3d *min,
                          RwV3d *max);

extern void
_rpD3D9GetMinMaxValuesV2d(const RwV2d *src,
                          RwInt32 numElements,
                          RwV2d *min,
                          RwV2d *max);

extern RwUInt32
_rpD3D9FindFormatV3d(const RwV3d *src,
                     RwInt32 numElements);

extern RwUInt32
_rpD3D9FindFormatV2d(const RwV2d *src,
                     RwInt32 numElements);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9VERTEXDECLARATION_H */
