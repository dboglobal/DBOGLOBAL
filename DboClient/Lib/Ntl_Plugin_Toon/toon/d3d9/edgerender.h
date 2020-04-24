/*****************************************************************************

    File: edgerender.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef EDGERENDER_H
#define EDGERENDER_H

/*****************************************************************************
 Includes
 */
#include "toonink.h"
#include "toonpaint.h"
#include "toonmaterial.h"
#include "toongeo.h"

/*****************************************************************************
 Defines
 */
/* limits we're stuck with because of renderware geometries/world sectors */
#define RPTOON_MAX_VERTICES    65535
#define RPTOON_MAX_FACES       65535
#define RPTOON_MAX_EDGES       65535

/* threshold to check whether edge is too short to attempt to normalize */
#define RPTOON_REALLY_SMALL_EDGE_THRESHOLD 0.001f

/*****************************************************************************
 Enums
 */

/*****************************************************************************
 Typedef Enums
 */

/*****************************************************************************
 Typedef Structs
 */
typedef struct ScreenSpaceProjectionTag ScreenSpaceProjection;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */
/* project a 3D vertex down to 2D */
struct ScreenSpaceProjectionTag
{
    RwMatrix matrix;
};

#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */

/*****************************************************************************
 Global Variables
 */

/*****************************************************************************
 Function prototypes
 */

extern void _rpToonEdgeRenderInit(void);
extern void _rpToonEdgeRenderClean(void);
extern void _rpToonEdgeRenderPushState(void);
extern void _rpToonEdgeRenderPopState(void);


extern void
GetScreenSpaceProjection(ScreenSpaceProjection *projection,
                         const RwMatrix *transform);

extern void
_rpToonEdgeRenderPrepareSubmit(RwUInt32 maxNumEdges);

extern void
_rpToonEdgeRenderAppendQuad(const RwV3d *vIn0,
                            const RwV3d *vIn1,
                            const RwV2d *vOut0,
                            const RwV2d *vOut1,
                            RwRGBA color);

extern void
_rpToonEdgeRenderSubmit(void);

extern void
_rpToonEdgeRenderPrepareFlatSubmit(RwUInt32 maxNumEdges,
                                   RwRGBA color);

extern void
_rpToonEdgeRenderAppendFlatQuad(const RwV3d *vIn0,
                                const RwV3d *vIn1,
                                const RwV2d *vOut0,
                                const RwV2d *vOut1);

extern void
_rpToonEdgeRenderFlatSubmit(void);

extern void
_rpToonEdgeRenderPrepareVertexShaderSubmit(RwUInt32 maxNumEdges);

extern void
_rpToonEdgeRenderAppendVertexShaderQuad(const RwV3d *vPos0,
                                        const RwV3d *vPos1,
                                        const RwV3d *vNormal0,
                                        const RwV3d *vNormal1,
                                        RwReal vertexThicknesses0,
                                        RwReal vertexThicknesses1,
                                        const RpToonInk *ink);

extern void
_rpToonEdgeRenderVertexShaderSubmit(void);

extern void
_rpToonEdgeRenderPrepareVertexShaderFlatSubmit(RwUInt32 maxNumEdges,
                                               RwRGBA color);

extern void
_rpToonEdgeRenderAppendVertexShaderFlatQuad(const RwV3d *vPos0,
                                            const RwV3d *vPos1,
                                            const RwV3d *vNormal0,
                                            const RwV3d *vNormal1,
                                            RwReal vertexThicknesses0,
                                            RwReal vertexThicknesses1);

extern void
_rpToonEdgeRenderVertexShaderFlatSubmit(void);

/* Computes perspective scale factor per vertex given camera space z value & precomputed coefficients */
static _inline RwReal
_rpToonComputePerspectiveScale(const RpToonInk *ink,
                               RwReal camSpaceZ)
{
    RWFUNCTION(RWSTRING("_rpToonComputePerspectiveScale"));
    RWASSERT(ink);
    RWRETURN(ink->perspectiveScale_a + ink->perspectiveScale_b * (camSpaceZ - ink->perspectiveScale_zMin) );
}

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* EDGERENDER_H */
