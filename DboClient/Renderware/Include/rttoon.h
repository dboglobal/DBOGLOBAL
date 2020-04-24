/*****************************************************************************

    File: rttoon.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef RTTOON_H
#define RTTOON_H

/*****************************************************************************
 Includes
 */
#include <rwcore.h>
#include <rpworld.h>

#include "rptoon.h"

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

/*****************************************************************************
 Function prototypes
 */
extern RwInt32 RtToonGeoGetNumEdges(const RpToonGeo *toonGeo);
extern RpToonEdge * RtToonGeoGetEdges(const RpToonGeo *toonGeo);
extern RwUInt16 RtToonGeoFindEdgeIndex(const RpToonGeo *toonGeo,
                                       RwInt32 rwV0,
                                       RwInt32 rwV1);
extern void RtToonGeoSetEdgeInkID(RpToonGeo *toonGeo,
                                  RwUInt16 edgeIndex,
                                  RpToonInkType type,
                                  RwInt32 inkID);

extern void RtToonGeoGetVertexRemap(const RpToonGeo *toonGeo,
                                    const RwV3d *positions,
                                    RwInt32 *vertexRemap);

extern RwInt32 RtToonGeoGetInkIDFromName(RpToonGeo *toonGeo,
                                         const RwChar *inkName);

extern RpToonGeo *RtToonGeometryCreateToonGeo(RpGeometry *geometry,
                                              RwBool generateCreaseEdgesFromSmoothingGroups);
extern RpToonGeo *
RtToonWorldSectorCreateToonGeo(RpWorldSector *sector,
                               RwBool generateCreaseEdgesFromSmoothingGroups);

extern RpToonGeo *RtToonGeoOptimizeEdgeInkIDs(RpToonGeo *toonGeo);
extern void RtToonWorldOptimize(RpWorld *world);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* RTTOON_H */
