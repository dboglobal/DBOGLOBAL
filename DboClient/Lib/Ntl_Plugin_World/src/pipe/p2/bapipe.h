#ifndef BAPIPE_H

#define BAPIPE_H

#include "bacamera.h"

#include "p2core.h"
#include "p2stdcls.h"
#include "p2renderstate.h"
#include "baim3d.h"

/* MODULE REGISTRATION */

/* RWPUBLIC */

#if (!defined(DOXYGEN))
struct rwPipeGlobals
{
    RwFreeList          *pipesFreeList;   /* Save mallocs, use a freelist */
    RxRenderStateVector  defaultRenderState;
    RwLinkList           allPipelines;    /* Unused as of yet, meant to be used to keep track of all
                                           * created pipelines (for CBs and maybe cleanup) */
    RwUInt32             maxNodesPerPipe; /* Initialised from _rxPipelineMaxNodes at startup. Used to
                                           * allow conservative, single allocations during pipelock */

    /* NOTE: Rw and RpWorld PowerPipe globals kept together for simplicity */

    /* The current default pipelines (used if pipe == NULL for an object) */
    RxPipeline          *currentAtomicPipeline;
    RxPipeline          *currentWorldSectorPipeline;
    RxPipeline          *currentMaterialPipeline;
    /* Generic C-based pipes that run on all platforms
     * - these are set as the current pipes at startup unless
     *   platform-specific pipes (below) are created */
    RxPipeline          *genericAtomicPipeline;
    RxPipeline          *genericWorldSectorPipeline;
    RxPipeline          *genericMaterialPipeline;
    /* Platforms that have their own non-generic pipelines
     * (OPENGL, D3D8, SKY2, KAMUI2, DOLPHIN) put them here: */
    RxPipeline          *platformAtomicPipeline;
    RxPipeline          *platformWorldSectorPipeline;
    RxPipeline          *platformMaterialPipeline;
};
#endif /* (!defined(DOXYGEN)) */

typedef struct rwPipeGlobals rwPipeGlobals;

#define RXPIPELINEGLOBAL(var) (RWPLUGINOFFSET(rwPipeGlobals, RwEngineInstance, _rxPipelineGlobalsOffset)->var)

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwInt32      _rxPipelineGlobalsOffset;

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void *_rwRenderPipelineOpen( void *instance, RwInt32 offset, RwInt32 size);
extern void *_rwRenderPipelineClose(void *instance, RwInt32 offset, RwInt32 size);

/* API */

extern RwBool _rwPipeAttach(void);

#define rwPipeAttach() _rwPipeAttach()

extern void   _rwPipeInitForCamera(const RwCamera *camera);
#define rwPipeInitForCamera(_camera)  _rwPipeInitForCamera(_camera)

#if (defined(RWSEATIDENTIFIER))
extern RwBool _rwSeatIdentifier(RwChar *RootPathName,
                                RwUInt32 *VolumeSerialNumber);
#endif /* (defined(RWSEATIDENTIFIER)) */

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* BAPIPE_H */
