#ifndef BAPIPEW_H
#define BAPIPEW_H

#include <rwcore.h>

#include "baworld.h"
#include "basector.h"
#include "baclump.h"
#include "bamateri.h"

/* API */

extern RwBool _rpWorldPipeAttach(void);
extern RwBool _rpWorldPipelineOpen( void);
extern void   _rpWorldPipelineClose(void);

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */
extern RwBool _rxWorldDevicePluginAttach(void);
#ifdef    __cplusplus
}
#endif /* __cplusplus */

/****************************************************************************
 <macro/inline functionality
 */

/* RWPUBLIC */

#define RpWorldGetDefaultSectorPipelineMacro() \
    (RXPIPELINEGLOBAL(currentWorldSectorPipeline))

#define RpWorldSetSectorPipelineMacro(_world, _pipeline) \
    ( ((_world)->pipeline = _pipeline), _world )

#define RpWorldGetSectorPipelineMacro(_world, _pipeline) \
    ( (*(_pipeline) = (_world)->pipeline), _world )

#define RpWorldSectorSetPipelineMacro(_sector, _pipeline) \
    ( ((_sector)->pipeline = _pipeline), _sector )

#define RpWorldSectorGetPipelineMacro(_sector, _pipeline) \
    ( (*(_pipeline) = (_sector)->pipeline), _sector )

#define RpAtomicGetDefaultPipelineMacro() \
    (RXPIPELINEGLOBAL(currentAtomicPipeline))

#define RpAtomicSetPipelineMacro(_atomic, _pipeline) \
    ( ((_atomic)->pipeline = _pipeline), _atomic )

#define RpAtomicGetPipelineMacro(_atomic, _pipeline) \
    ( (*(_pipeline) = (_atomic)->pipeline), _atomic )

#define RpMaterialGetDefaultPipelineMacro() \
    (RXPIPELINEGLOBAL(currentMaterialPipeline))

#define RpMaterialSetPipelineMacro(_material, _pipeline) \
    ( ((_material)->pipeline = _pipeline), _material )

#define RpMaterialGetPipelineMacro(_material, _pipeline) \
    ( (*(_pipeline) = (_material)->pipeline), _material )


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpWorldGetDefaultSectorPipeline RpWorldGetDefaultSectorPipelineMacro
#define RpWorldSetSectorPipeline        RpWorldSetSectorPipelineMacro
#define RpWorldGetSectorPipeline        RpWorldGetSectorPipelineMacro
#define RpWorldSectorSetPipeline        RpWorldSectorSetPipelineMacro
#define RpWorldSectorGetPipeline        RpWorldSectorGetPipelineMacro

#define RpAtomicGetDefaultPipeline      RpAtomicGetDefaultPipelineMacro
#define RpAtomicSetPipeline             RpAtomicSetPipelineMacro
#define RpAtomicGetPipeline             RpAtomicGetPipelineMacro

#define RpMaterialGetDefaultPipeline    RpMaterialGetDefaultPipelineMacro
#define RpMaterialSetPipeline           RpMaterialSetPipelineMacro
#define RpMaterialGetPipeline           RpMaterialGetPipelineMacro

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* RWPUBLICEND */

/****************************************************************************
 Function prototypes
 */

/* RWPUBLIC */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RxPipeline       *RpWorldSetDefaultSectorPipeline(RxPipeline *pipeline);
extern RxPipeline       *RpAtomicSetDefaultPipeline(RxPipeline *pipeline);
extern RxPipeline       *RpMaterialSetDefaultPipeline(RxPipeline *pipeline);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

extern RxPipeline       *RpWorldGetGenericSectorPipeline(void);
extern RxPipeline       *RpWorldGetDefaultSectorPipeline(void);
extern RpWorld          *RpWorldSetSectorPipeline(RpWorld       *world,
                                                  RxPipeline    *pipeline);
extern RpWorld          *RpWorldGetSectorPipeline(RpWorld       *world,
                                                  RxPipeline   **pipeline);
extern RpWorldSector    *RpWorldSectorSetPipeline(RpWorldSector *sector,
                                                  RxPipeline    *pipeline);
extern RpWorldSector    *RpWorldSectorGetPipeline(RpWorldSector *sector,
                                                  RxPipeline   **pipeline);

extern RxPipeline       *RpAtomicGetGenericPipeline(void);
extern RxPipeline       *RpAtomicGetDefaultPipeline(void);
extern RpAtomic         *RpAtomicSetPipeline(RpAtomic             *atomic,
                                             RxPipeline           *pipeline);
extern const RpAtomic   *RpAtomicGetPipeline(const RpAtomic *const atomic, 
                                             RxPipeline          **pipeline);

extern RxPipeline       *RpMaterialGetGenericPipeline(void);
extern RxPipeline       *RpMaterialGetDefaultPipeline(void);
extern RpMaterial       *RpMaterialSetPipeline(RpMaterial  *material,
                                               RxPipeline  *pipeline);
extern RpMaterial       *RpMaterialGetPipeline(RpMaterial  *material,
                                               RxPipeline **pipeline);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern const RpGeometry *RpGeometryIsCorrectlySorted(const RpGeometry * geometry,
                                                     RwBool * result);
extern RpGeometry       *RpGeometrySortByMaterial(const RpGeometry * geometry,
                                                  RpGeometrySortByMaterialCallBack  callback);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* LEGACY-SUPPORT macros */
#define RpWorldSetDefaultSectorInstancePipeline RpWorldSetDefaultSectorPipeline
#define RpWorldGetDefaultSectorInstancePipeline RpWorldGetDefaultSectorPipeline
#define RpWorldSetSectorInstancePipeline        RpWorldSetSectorPipeline
#define RpWorldGetSectorInstancePipeline        RpWorldGetSectorPipeline
#define RpWorldSectorSetInstancePipeline        RpWorldSectorSetPipeline
#define RpWorldSectorGetInstancePipeline        RpWorldSectorGetPipeline

#define RpAtomicGetDefaultInstancePipeline      RpAtomicGetDefaultPipeline
#define RpAtomicSetDefaultInstancePipeline      RpAtomicSetDefaultPipeline
#define RpAtomicSetInstancePipeline             RpAtomicSetPipeline
#define RpAtomicGetInstancePipeline             RpAtomicGetPipeline

#define RpMaterialSetDefaultRenderPipeline      RpMaterialSetDefaultPipeline
#define RpMaterialGetDefaultRenderPipeline      RpMaterialGetDefaultPipeline
#define RpMaterialSetRenderPipeline             RpMaterialSetPipeline
#define RpMaterialGetRenderPipeline             RpMaterialGetPipeline

/* RWPUBLICEND */

#endif /* BAPIPEW_H */
