/*****************************************************************************

    File: brutesilhouette.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef BRUTESILHOUETTE_H
#define BRUTESILHOUETTE_H

/*****************************************************************************
 Includes
 */

/*****************************************************************************
 Defines
 */

/*****************************************************************************
 Enums
 */

/*****************************************************************************
 Typedef Enums
 */

/*****************************************************************************
 Typedef Structs
 */

typedef struct
{
    RwV3d camspaceInnerVertex;  /* Used temporary also as eyespace */
    RwV2d camspaceOuterVertex;
    const RpToonInk  *ink;
} _rpToonVertexCache;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */

#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */

/*****************************************************************************
 Global Variables
 */
extern _rpToonVertexCache *_rpToonD3D9VertexCache;

extern RwUInt8  *_rpToonD3D9IsFrontFace;       /* scratch area for dot product tests */

/*****************************************************************************
 Function prototypes
 */
extern void _rpToonBruteSilhouetteInit(void);
extern void _rpToonBruteSilhouetteClean(void);
extern void _rpToonBruteSilhouetteRender(RpToonGeo *toonGeo,
                                         const RwV3d *verts,
                                         const RwV3d *normals,
                                         RwBool faceNormalsInvalid,
                                         const RwMatrix *transform,
                                         const ScreenSpaceProjection *ssp);

extern void _rpToonBruteSilhouetteRenderOptimized(RpToonGeo *toonGeo,
                                         const RwV3d *verts,
                                         const RwV3d *normals,
                                         RwBool faceNormalsInvalid,
                                         const RwMatrix *transform,
                                         const ScreenSpaceProjection *ssp);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* BRUTESILHOUETTE_H */
