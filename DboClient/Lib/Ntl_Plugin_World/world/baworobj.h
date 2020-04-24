/***************************************************************************
 *                                                                         *
 * Module  : baworobj.h                                                    *
 *                                                                         *
 * Purpose : World object handling functions.                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWWOROBJ_H
#define RWWOROBJ_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "basector.h"
#include "baclump.h"
#include "balight.h"
#include "baworld.h"

/****************************************************************************
 Defines
 */

#if (!defined(RWDEBUG))
/* Called from RpWorldRender() */
#define _rwCameraAssertWorld(camera, world) /* No op */
#endif /* (!defined(RWDEBUG)) */

/* RWPUBLIC */
/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void RpTieSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );
extern void RpLightTieSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

/* Adding and removing cameras to/from the world */
extern RpWorld *RpWorldRemoveCamera(RpWorld *world, RwCamera *camera);
extern RpWorld *RpWorldAddCamera(RpWorld *world, RwCamera *camera);
extern RpWorld *RwCameraGetWorld(const RwCamera *camera);

/* Adding and removing atomics to/from the world */
extern RpWorld *RpWorldRemoveAtomic(RpWorld *world, RpAtomic *atomic);
extern RpWorld *RpWorldAddAtomic(RpWorld *world, RpAtomic *atomic);
extern RpWorld *RpAtomicGetWorld(const RpAtomic *atomic);

/* Adding and removing clumps to/from the world */
extern RpWorld *RpWorldAddClump(RpWorld *world, RpClump *clump);
extern RpWorld *RpWorldRemoveClump(RpWorld *world, RpClump *clump);
extern RpWorld *RpClumpGetWorld(const RpClump *clump);

/* Adding and removing lights to/from the world */
extern RpWorld *RpWorldAddLight(RpWorld *world, RpLight *light);
extern RpWorld *RpWorldRemoveLight(RpWorld *world, RpLight *light);
extern RpWorld *RpLightGetWorld(const RpLight *light);

/* Finding whats in the view frustum */
extern RwCamera *RwCameraForAllClumpsInFrustum(RwCamera *camera, void *data);
extern RwCamera *RwCameraForAllAtomicsInFrustum(RwCamera *camera, 
                                                RpAtomicCallBack callback,
                                                void *data);
extern RwCamera *RwCameraForAllSectorsInFrustum(RwCamera *camera,
                                                RpWorldSectorCallBack callBack,
                                                void *pData);

/* Enumeration involving the world sectors */
extern RpLight *RpLightForAllWorldSectors(RpLight *light,
                                          RpWorldSectorCallBack callback,
                                          void *data);
extern RpAtomic *RpAtomicForAllWorldSectors(RpAtomic *atomic,
                                            RpWorldSectorCallBack callback,
                                            void *data);
extern RpWorldSector *RpWorldSectorForAllAtomics(RpWorldSector *sector,
                                                 RpAtomicCallBack callback,
                                                 void *data);
extern RpWorldSector *RpWorldSectorForAllCollisionAtomics(RpWorldSector *sector,
                                                 RpAtomicCallBack callback,
                                                 void *data);
extern RpWorldSector *RpWorldSectorForAllLights(RpWorldSector *sector,
                                                RpLightCallBack callback,
                                                void *data);

/* RWPUBLICEND */

/* Registering toolkits to attach objects into worlds */
extern RwBool _rpWorldObjRegisterExtensions(void);
extern RwBool _rpTieDestroy(RpTie *tie);
extern RwBool _rpLightTieDestroy(RpLightTie *tie);

#if (defined(RWDEBUG))
/* Called from RpWorldRender() */
void _rwCameraAssertWorld(RwCamera *camera, RpWorld *world);
#endif /* (defined(RWDEBUG)) */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWWOROBJ_H */
