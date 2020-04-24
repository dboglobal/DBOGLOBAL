/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctbuild.h
 *  Purpose: Collision partition tree generation
 */

#ifndef CTBUILD_H
#define CTBUILD_H

/******************************************************************************
 *  Functions
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RpCollTree *
_rpCollTreeBuild(
    RwInt32             numVertices,
    RwV3d               *vertices,
    RwInt32             numTriangles,
    RpTriangle          *triangles,
    RwUInt16            *triangleSortMap
    );

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* CTBUILD_H */
