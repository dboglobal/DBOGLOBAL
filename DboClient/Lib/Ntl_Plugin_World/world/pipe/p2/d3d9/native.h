/****************************************************************************
 *                                                                          *
 * module : native.h                                                        *
 *                                                                          *
 * purpose: see native.c                                                    *
 *                                                                          *
 ****************************************************************************/

#ifndef NATIVE_H
#define NATIVE_H

#include "rwcore.h"

/****************************************************************************
 global prototypes
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Geometry */
extern RwStream *_rpGeometryNativeWrite(RwStream *stream,
                                        const RpGeometry *geometry);
extern RpGeometry *_rpGeometryNativeRead(RwStream *stream,
                                         RpGeometry *geometry);
extern RwInt32 _rpGeometryNativeSize(const RpGeometry *geometry);

/* World sectors */
extern RwStream *_rpWorldSectorNativeWrite(RwStream *stream,
                                           const RpWorldSector *sector);
extern RpWorldSector *_rpWorldSectorNativeRead(RwStream *stream,
                                               RpWorldSector *sector);
extern RwInt32 _rpWorldSectorNativeSize(const RpWorldSector *sector);

#ifdef    __cplusplus
}
#endif /* __cplusplus */


#endif /* NATIVE_H */
