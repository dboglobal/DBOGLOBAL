/***************************************************************************
 *                                                                         *
 * Module  : D3D82drend.h                                                    *
 *                                                                         *
 * Purpose : 2D primitive rendering functions.                             *
 *                                                                         *
 **************************************************************************/

#ifndef D3D82DREND_H
#define D3D82DREND_H

extern RwBool _rwD3D8Im2DRenderOpen(void);

extern RwBool _rwD3D8Im2DRenderLine(RwIm2DVertex *verts, RwInt32 numVerts,
                                  RwInt32 vert1, RwInt32 vert2);
extern RwBool _rwD3D8Im2DRenderTriangle(RwIm2DVertex *verts, RwInt32 numVerts,
                               RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
extern RwBool _rwD3D8Im2DRenderPrimitive(RwPrimitiveType primType,
                                       RwIm2DVertex *verts,
                                       RwInt32 numVerts);
extern RwBool _rwD3D8Im2DRenderIndexedPrimitive(RwPrimitiveType primType,
                                              RwIm2DVertex *verts,
                                              RwInt32 numVerts,
                                              RwImVertexIndex *indices,
                                              RwInt32 numIndices);

extern void _rwD3D8Im2DRenderClose(void);

#endif /* D3D82DREND_H */
