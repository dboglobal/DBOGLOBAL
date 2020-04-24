/**
 * \ingroup rpworldrwcamera
 * \page rpworldrwcameraoverview RwCamera Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection worldrwcameraoverview Overview
 *
 * The World plugin directly extends the Core Library's Camera API (RwCamera).
 * Additional functionality provided when RpWorld is attached is as follows:
 *
 * \li Get the World object within which the specified Camera resides;
 * \li Iterate through all World Sectors in the Camera Frustum.
 *     (i.e., all visible World Sectors.)
 * \li Iterate through all Clumps in the Camera Frustum.
 *     (i.e., all visible Clumps)
 * \li Iterate through all Clumps not in the Camera Frustum.
 *     (i.e., all non-visible Clumps)
 *
 * (These functions retain the "RwCamera[METHOD]()" naming convention.)
 *
 * Further information is available in the Camera chapter of the User Guide.
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include <rwcore.h>
#include "baworld.h"
#include "bamesh.h"
#include "bapipew.h"
#include "babinwor.h"

#include "native.h"

#include "baworobj.h"

/****************************************************************************
 Local Types
 */

typedef struct RpWorldCameraExt RpWorldCameraExt;
struct RpWorldCameraExt
{
    /* List of atomic sectors within the frustum */
    RpWorldSector     **frustumSectors;
    RwInt32             space;
    RwInt32             position;

    /* The world the camera lives in */
    RpWorld            *world;

    /* Old start update/end update functions */
    RwCameraBeginUpdateFunc oldBeginUpdate;
    RwCameraEndUpdateFunc oldEndUpdate;

    /* Old sync function */
    RwObjectHasFrameSyncFunction oldSync;
};

typedef struct RpWorldAtomicExt RpWorldAtomicExt;
struct RpWorldAtomicExt
{
    /* The world the atomic lives in */
    RpWorld            *world;

    /* Old sync function */
    RwObjectHasFrameSyncFunction oldSync;
};

typedef struct RpWorldClumpExt RpWorldClumpExt;
struct RpWorldClumpExt
{
    /* The world the clump lives in */
    RpWorld            *world;

    /* clumpsInFrustumID - used to prevent multiple invocations
     * of the clump frustum callback
     */
    RwUInt32            clumpsInFrustumID;
};

typedef struct RpWorldLightExt RpWorldLightExt;
struct RpWorldLightExt
{
    /* The world the light lives in */
    RpWorld            *world;

    /* Old sync function */
    RwObjectHasFrameSyncFunction oldSync;
};

typedef struct rpWorldObjGlobals rpWorldObjGlobals;
struct rpWorldObjGlobals
{
    RwFreeList         *tieFreeList;
    RwFreeList         *lightTieFreeList;
    RwUInt32            clumpsInFrustumID;
};

/****************************************************************************
 Local (Static) Prototypes
 */

static RpWorld     *WorldSyncCamera(RpWorld * world, RwCamera * camera);

/****************************************************************************
 Local Defines
 */

#define rwCAMERADISPLAYSECTORSINCREASE 50

/* Accessing plugin memory */
#define WORLDCAMERAEXTFROMCAMERA(camera) \
   ((RpWorldCameraExt *)(((RwUInt8 *)(camera)) + cameraExtOffset))
#define WORLDCAMERAEXTFROMCONSTCAMERA(camera) \
   ((const RpWorldCameraExt *)(((const RwUInt8 *)(camera)) + cameraExtOffset))

#define ATOMICEXTFROMATOMIC(atomic) \
   ((RpWorldAtomicExt *)(((RwUInt8 *)(atomic)) + atomicExtOffset))
#define ATOMICEXTFROMCONSTATOMIC(atomic) \
   ((const RpWorldAtomicExt *)(((const RwUInt8 *)(atomic)) + atomicExtOffset))

#define CLUMPEXTFROMCLUMP(clump) \
   ((RpWorldClumpExt *)(((RwUInt8 *)(clump)) + clumpExtOffset))
#define CLUMPEXTFROMCONSTCLUMP(clump) \
   ((const RpWorldClumpExt *)(((const RwUInt8 *)(clump)) + clumpExtOffset))

#define LIGHTEXTFROMLIGHT(light) \
   ((RpWorldLightExt *)(((RwUInt8 *)(light)) + lightExtOffset))
#define LIGHTEXTFROMCONSTLIGHT(light) \
   ((const RpWorldLightExt *)(((const RwUInt8 *)(light)) + lightExtOffset))

#define RPWORLDOBJGLOBAL(var) (RWPLUGINOFFSET(rpWorldObjGlobals, RwEngineInstance, worldObjModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwInt32      cameraExtOffset = 0;
static RwInt32      atomicExtOffset = 0;
static RwInt32      clumpExtOffset = 0;
static RwInt32      lightExtOffset = 0;
static RwModuleInfo worldObjModule;

/****************************************************************************
 Functions
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             Opening / Closing

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwInt32 _rpTieFreeListBlockSize = 256,
               _rpTieFreeListPreallocBlocks = 1;
static RwFreeList _rpTieFreeList;

/**
 * \ingroup rpworldsub
 * \ref RpTieSetFreeListCreateParams allows the developer to specify
 * how many \ref RpTie s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RpTieSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpTieSetFreeListCreateParams"));
#endif
    _rpTieFreeListBlockSize = blockSize;
    _rpTieFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

static RwInt32 _rpLightTieFreeListBlockSize = 32,
               _rpLightTieFreeListPreallocBlocks = 1;
static RwFreeList _rpLightTieFreeList;

/**
 * \ingroup rplight
 * \ref RpLightTieSetFreeListCreateParams allows the developer to specify
 * how many \ref RpLightTie s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RpLightTieSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpLightTieSetFreeListCreateParams"));
#endif
    _rpLightTieFreeListBlockSize = blockSize;
    _rpLightTieFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/******************************************************************************
 WorldObjectOpen

 Open the world/object interface module

 On entry   : instance, offset, size
 On exit    : instance pointer on success
 */

static void        *
WorldObjectOpen(void *instance, RwInt32 offset,
                RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("WorldObjectOpen"));

    /* Grab the global variable offste (same for all instances) */
    worldObjModule.globalsOffset = offset;

    /* Get on with the setup */
    RPWORLDOBJGLOBAL(tieFreeList) =
        RwFreeListCreateAndPreallocateSpace(sizeof(RpTie), _rpTieFreeListBlockSize,
        sizeof(RwUInt32),
        _rpTieFreeListPreallocBlocks, &_rpTieFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_WORLDMODULE);
    if (!RPWORLDOBJGLOBAL(tieFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    RPWORLDOBJGLOBAL(lightTieFreeList) =
        RwFreeListCreateAndPreallocateSpace(sizeof(RpLightTie), _rpLightTieFreeListBlockSize,
        sizeof(RwUInt32),
        _rpLightTieFreeListPreallocBlocks, &_rpLightTieFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_WORLDMODULE);
    if (!RPWORLDOBJGLOBAL(lightTieFreeList))
    {
        /* Failure */
        RwFreeListDestroy(RPWORLDOBJGLOBAL(tieFreeList));
        RPWORLDOBJGLOBAL(tieFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    /* Reset the render frame */
    RWSRCGLOBAL(renderFrame) = 1;

    /* clumpsInFrustumID used by RwCameraForAllClumpsInFrustum */
    RPWORLDOBJGLOBAL(clumpsInFrustumID) = 0;

    /* One more module instance */
    worldObjModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/******************************************************************************
 WorldObjectClose

 Close the world/object interface module

 On entry   : instance, offset, size
 On exit    : instance pointer on success
 */

static void        *
WorldObjectClose(void *instance,
                 RwInt32 offset __RWUNUSED__,
                 RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("WorldObjectClose"));

    RWASSERT(instance);

    /* Blow away the tie lists */
    if (RPWORLDOBJGLOBAL(lightTieFreeList))
    {
        RwFreeListDestroy(RPWORLDOBJGLOBAL(lightTieFreeList));
        RPWORLDOBJGLOBAL(lightTieFreeList) = (RwFreeList *)NULL;
    }

    if (RPWORLDOBJGLOBAL(tieFreeList))
    {
        RwFreeListDestroy(RPWORLDOBJGLOBAL(tieFreeList));
        RPWORLDOBJGLOBAL(tieFreeList) = (RwFreeList *)NULL;
    }

    /* One more module instance */
    worldObjModule.numInstances--;

    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             SectorsInFrustum

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG))
/****************************************************************************
 _rwCameraAssertWorld
 */
void
_rwCameraAssertWorld(RwCamera *camera, RpWorld *world)
{
    RpWorldCameraExt *cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    RWFUNCTION(RWSTRING("_rwCameraAssertWorld"));

    /* This is called from RpWorldRender(), avoids externing too much */
    RWASSERT(cameraExt->world == world);

    RWRETURNVOID();
}
#endif /* (defined(RWDEBUG)) */

/****************************************************************************
 SectorsInFrustumDeinitialise

 On entry   : SectorsInFrustum
 On exit    : TRUE on success
 */

static              RwBool
SectorsInFrustumDeinitialise(RpWorldCameraExt * cameraExt)
{
    RWFUNCTION(RWSTRING("SectorsInFrustumDeinitialise"));

    RWASSERT(cameraExt);

    if (cameraExt->frustumSectors)
    {
        RwFree(cameraExt->frustumSectors);
    }
    cameraExt->frustumSectors = (RpWorldSector **)NULL;
    cameraExt->space = 0;
    cameraExt->position = 0;

    RWRETURN(TRUE);
}

/****************************************************************************
 SectorsInFrustumAddSpace

 On entry   : Rendered atomics
            : Amount of extra space
 On exit    : TRUE on success
 */

static              RwBool
SectorsInFrustumAddSpace(RpWorldCameraExt * cameraExt, RwInt32 nNum)
{
    RpWorldSector     **newFrustumSectors;
    RwInt32             memSize;

    RWFUNCTION(RWSTRING("SectorsInFrustumAddSpace"));

    RWASSERT(cameraExt);

    memSize = (cameraExt->space + nNum) * sizeof(RpWorldSector *);
    if (cameraExt->frustumSectors)
    {
        newFrustumSectors = (RpWorldSector **)
            RwRealloc(cameraExt->frustumSectors, memSize,
            rwMEMHINTDUR_EVENT | rwID_WORLDMODULE |
            rwMEMHINTFLAG_RESIZABLE);
    }
    else
    {
        newFrustumSectors = (RpWorldSector **) RwMalloc(memSize,
            rwMEMHINTDUR_EVENT | rwID_WORLDMODULE |
            rwMEMHINTFLAG_RESIZABLE);
    }

    if (newFrustumSectors)
    {
        cameraExt->frustumSectors = newFrustumSectors;
        cameraExt->space += nNum;

        RWRETURN(TRUE);
    }
    else
    {
        /* Failed */
        RWERROR((E_RW_NOMEM, memSize));
        RWRETURN(FALSE);
    }
}

/******************************************************************************
 WorldCameraBeginUpdate

 Start the camera update (grab the world)

 On entry   : camera
 On exit    : camera pointer on success
 */

static RwCamera    *
WorldCameraBeginUpdate(RwCamera * camera)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    RWFUNCTION(RWSTRING("WorldCameraBeginUpdate"));

    RWASSERT(camera);

    /* Save away the world */
    RWSRCGLOBAL(curWorld) = (void *) cameraExt->world;

    /* Increase the render frame count */
    RWSRCGLOBAL(renderFrame)++;

    /* Chain the old function */
    RWRETURN(cameraExt->oldBeginUpdate(camera));
}

/******************************************************************************
 WorldCameraEndUpdate

 End the camera update (no current world now)

 On entry   : camera
 On exit    : camera pointer on success
 */

static RwCamera    *
WorldCameraEndUpdate(RwCamera * camera)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    RWFUNCTION(RWSTRING("WorldCameraEndUpdate"));

    RWASSERT(camera);

    /* No current world now */
    RWSRCGLOBAL(curWorld) = NULL;

    /* Chain the old function */
    RWRETURN(cameraExt->oldEndUpdate(camera));
}

/******************************************************************************
 WorldCameraSync

 Sync the camera - find sectors in the frustum too

 On entry   : camera
 On exit    : camera pointer on success
 */

static RwObjectHasFrame *
WorldCameraSync(RwObjectHasFrame * object)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(object);

    RWFUNCTION(RWSTRING("WorldCameraSync"));

    RWASSERT(object);

    /* Do the default to build the frustum planes */
    if (cameraExt->oldSync(object))
    {
        RwCamera           *camera = (RwCamera *) object;
        RpWorld            *world = cameraExt->world;

        /* Now update the sector list in the camera */
        if (world)
        {
            RwStandardFunc const HintRenderF2BFunc =
                RWSRCGLOBAL(stdFunc[rwSTANDARDHINTRENDERF2B]);

            RWASSERTISTYPE(world, rpWORLD);
            WorldSyncCamera(world, camera);

            /* Hint to the driver what the rendering order is going to be,
             * to optimise rendering of alpha polygons
             */

            RWASSERT(NULL != HintRenderF2BFunc);
            HintRenderF2BFunc(NULL, NULL,
                              (world->renderOrder ==
                               rpWORLDRENDERFRONT2BACK));
        }

        /* Return success */
        RWRETURN(object);
    }
    RWRETURN((RwObjectHasFrame *)NULL);
}

/******************************************************************************
 WorldInitCameraExt

 Initialise the rendered atomics to something reasonable

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldInitCameraExt(void *object,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(object);
    RwCamera           *camera = (RwCamera *) object;

    RWFUNCTION(RWSTRING("WorldInitCameraExt"));

    RWASSERT(object);

    cameraExt->frustumSectors = (RpWorldSector **)NULL;
    cameraExt->space = 0;
    cameraExt->position = 0;

    /* Chain the update functions so we can capture the world */
    cameraExt->oldBeginUpdate = camera->beginUpdate;
    cameraExt->oldEndUpdate = camera->endUpdate;
    cameraExt->oldSync = camera->object.sync;
    camera->object.sync = WorldCameraSync;
    camera->beginUpdate = WorldCameraBeginUpdate;
    camera->endUpdate = WorldCameraEndUpdate;

    /* Not in a world when created */
    cameraExt->world = (RpWorld *)NULL;

    RWRETURN(object);
}

/******************************************************************************
 WorldCopyCameraExt

 Initialise the rendered atomics to something reasonable

 On entry   : Object, object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldCopyCameraExt(void *dstObject,
                   const void *srcObject,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    RpWorldCameraExt   *dstCameraExt =
        WORLDCAMERAEXTFROMCAMERA(dstObject);
    const RpWorldCameraExt *srcCameraExt =
        WORLDCAMERAEXTFROMCONSTCAMERA(srcObject);

    RWFUNCTION(RWSTRING("WorldCopyCameraExt"));

    RWASSERT(dstObject);
    RWASSERT(srcObject);

    dstCameraExt->frustumSectors = (RpWorldSector **)NULL;
    dstCameraExt->space = 0;
    dstCameraExt->position = 0;

    /* I'm assuming that the update functions are OK -
     * sorted by the constructor */

    if (srcCameraExt->world)
    {
        /* Copy the world */
        RpWorldAddCamera(srcCameraExt->world, (RwCamera *) dstObject);
    }

    RWRETURN(dstObject);
}

/******************************************************************************
 WorldDeInitCameraExt

 De-initialises the rendered atomics to something reasonable

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldDeInitCameraExt(void *object,
                     RwInt32 offsetInObject __RWUNUSED__,
                     RwInt32 sizeInObject __RWUNUSED__)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(object);
    RwCamera           *camera = (RwCamera *) object;

    RWFUNCTION(RWSTRING("WorldDeInitCameraExt"));

    RWASSERT(object);

#ifdef RWDEBUG
    if (cameraExt->world)
    {
        RWMESSAGE((RWSTRING
                   ("Camera destroyed whilst still in a world")));
        RpWorldRemoveCamera(cameraExt->world, camera);
    }
#endif /* RWDEBUG */

    SectorsInFrustumDeinitialise(cameraExt);

    /* Reverse the interface overload */
    camera->beginUpdate = cameraExt->oldBeginUpdate;
    camera->endUpdate = cameraExt->oldEndUpdate;
    camera->object.sync = cameraExt->oldSync;

    RWRETURN(object);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Finding the atomics positions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

RwBool
_rpLightTieDestroy(RpLightTie * tie)
{
    RWFUNCTION(RWSTRING("_rpLightTieDestroy"));

    RWASSERT(tie);

    rwLinkListRemoveLLLink(&tie->WorldSectorInLight);
    rwLinkListRemoveLLLink(&tie->lightInWorldSector);

    /* Free up the ties memory */
    RwFreeListFree(RPWORLDOBJGLOBAL(lightTieFreeList), tie);

    RWRETURN(TRUE);
}

RwBool
_rpTieDestroy(RpTie * tie)
{
    RWFUNCTION(RWSTRING("_rpTieDestroy"));

    RWASSERT(tie);

    /* There is the potential of this being a dummy tie.  These are
     * characterised by their being on the stack, and having NULL
     * apAtom and worldSector pointers.  Do not destroy them.
     */
    if (tie->apAtom && tie->worldSector)
    {
        rwLinkListRemoveLLLink(&tie->lWorldSectorInAtomic);
        rwLinkListRemoveLLLink(&tie->lAtomicInWorldSector);

        /* Free up the ties memory */
        RwFreeListFree(RPWORLDOBJGLOBAL(tieFreeList), tie);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 AtomicDestroyTies

 On entry   : Atomic
 On exit    :
 */

static void
AtomicDestroyTies(RpAtomic * atomic)
{
    /* Detach all of an atomics links to sectors */

    RwLLLink           *cur, *end;
    RpTie              *tie;

    RWFUNCTION(RWSTRING("AtomicDestroyTies"));

    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);
    cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
    while (cur != end)
    {
        /* Remove the tie */
        tie = rwLLLinkGetData(cur, RpTie, lWorldSectorInAtomic);
        cur = rwLLLinkGetNext(cur);
        _rpTieDestroy(tie);
    }

    RWRETURNVOID();
}

/****************************************************************************
 WorldAttachAtomicSphere

 Assumes that the collision has been set up
 Assumes the BBox has been set up

 On entry   : World
 On exit    : TRUE on success
 */

static void
WorldAttachAtomicSphere(RpWorld * world, RpAtomic * atomic)
{
    RpSector           *spSect;
    RpSector           *spaStack[rpWORLDMAXBSPDEPTH];
    RwInt32             nStack = 0;
    RwV3d               inf, sup;
    const RwSphere     *worldSphere;

    RWFUNCTION(RWSTRING("WorldAttachAtomicSphere"));

    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* Get the atomic's world space sphere */
    worldSphere = RpAtomicGetWorldBoundingSphere(atomic);
    RWASSERT(worldSphere);

    /* Set up the bounding box */
    RwV3dAssign(&inf, &worldSphere->center);
    RwV3dAssign(&sup, &inf);

    inf.x -= worldSphere->radius;
    inf.y -= worldSphere->radius;
    inf.z -= worldSphere->radius;

    sup.x += worldSphere->radius;
    sup.y += worldSphere->radius;
    sup.z += worldSphere->radius;

    /* Start at the top */
    spSect = world->rootSector;

    do
    {
        RWASSERT(spSect);

        if (spSect->type < 0)
        {
            RpTie              *tie;

            /* Its an atomic sector */
            RWASSERT(RPWORLDOBJGLOBAL(tieFreeList));
            tie = (RpTie *)RwFreeListAlloc(RPWORLDOBJGLOBAL(tieFreeList),
                                 rwMEMHINTDUR_EVENT | rwID_WORLDMODULE);

            tie->worldSector = (RpWorldSector *) spSect;
            tie->apAtom = atomic;

            rwLinkListAddLLLink(&((RpWorldSector *)spSect)->collAtomicsInWorldSector,
                                &tie->lAtomicInWorldSector);

            rwLinkListAddLLLink(&atomic->llWorldSectorsInAtomic,
                                &tie->lWorldSectorInAtomic);

            /* Try next */
            spSect = spaStack[nStack--];
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *pspPlane = (RpPlaneSector *) spSect;

            /* Left */
            if (GETCOORD(inf, spSect->type) < pspPlane->leftValue)
            {
                spSect = pspPlane->leftSubTree;

                /* Right */
                if (pspPlane->rightValue <
                    GETCOORD(sup, pspPlane->type))
                {
                    spaStack[++nStack] = pspPlane->rightSubTree;
                }
            }
            else
            {
                /* Right */
                if (pspPlane->rightValue <
                    GETCOORD(sup, pspPlane->type))
                {
                    spSect = pspPlane->rightSubTree;
                }
                else
                {
                    /* this case can occur although  */
                    /* very unlikely this line stops */
                    /* RW ending up in an infinite   */
                    /* loop                          */

                    spSect = spaStack[nStack--];
                }
            }
        }
    }
    while (nStack >= 0);

    /* All done */
    RWRETURNVOID();
}

/******************************************************************************
 WorldAtomicSync

 Sync the atomic - find sectors affected

 On entry   : atomic
 On exit    : atomic pointer on success
 */

static RwObjectHasFrame *
WorldAtomicSync(RwObjectHasFrame * type)
{
    RpAtomic           *atomic = (RpAtomic *) type;
    RpWorldAtomicExt   *atomicExt = ATOMICEXTFROMATOMIC(type);

    RWFUNCTION(RWSTRING("WorldAtomicSync"));

    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* Do the default thing first */
    if (atomicExt->oldSync(type))
    {
        RpWorld            *world = atomicExt->world;

        if (world)
        {
            /* Needs to be in a world to have ties */
            /* Now update the ties */
            RWASSERTISTYPE(world, rpWORLD);

            /* Detach this atomic from all its sectors */
            AtomicDestroyTies(atomic);

            /* It can be treated as a sphere */
            WorldAttachAtomicSphere(world, atomic);
        }

        /* Return success */
        RWRETURN(type);
    }
    RWRETURN((RwObjectHasFrame *)NULL);
}

/******************************************************************************
 WorldInitAtomicExt

 Initialise the atomic to something reasonable,
 and overload the sync functionality

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldInitAtomicExt(void *object,
                   RwInt32 offset __RWUNUSED__,
                   RwInt32 size __RWUNUSED__)
{
    RpWorldAtomicExt   *atomicExt = ATOMICEXTFROMATOMIC(object);
    RpAtomic           *atomic = (RpAtomic *) object;

    RWFUNCTION(RWSTRING("WorldInitAtomicExt"));

    RWASSERT(object);

    /* Not in a world yet */
    atomicExt->world = (RpWorld *)NULL;

    /* Set the counters */
    atomic->renderFrame = RWSRCGLOBAL(renderFrame) - 1;

    /* Overload the interface */
    atomicExt->oldSync = atomic->object.sync;
    atomic->object.sync = WorldAtomicSync;

    RWRETURN(object);
}

/******************************************************************************
 WorldCopyAtomicExt

 Copy from another atomic

 On entry   : Object, object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldCopyAtomicExt(void *dstObject,
                   const void * srcObject __RWUNUSEDRELEASE__,
                   RwInt32 offset __RWUNUSED__,
                   RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("WorldCopyAtomicExt"));

    RWASSERT(dstObject);
    RWASSERT(srcObject);

    /* Currently cloning an atomic does not clone the frame.  Adding an atomic
     * with no frame to the world is meaningless.
     */

    /* Assume that sync function is OK */

    RWRETURN(dstObject);
}

/******************************************************************************
 WorldDeInitAtomicExt

 De-initialises the atomic

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldDeInitAtomicExt(void *object,
                     RwInt32 offset __RWUNUSED__,
                     RwInt32 size __RWUNUSED__)
{
    RpWorldAtomicExt   *atomicExt = ATOMICEXTFROMATOMIC(object);
    RpAtomic           *atomic = (RpAtomic *) object;

    RWFUNCTION(RWSTRING("WorldDeInitAtomicExt"));

    RWASSERT(object);

    /* Detach it from it's sectors */
    AtomicDestroyTies(atomic);

#ifdef RWDEBUG
    if (atomicExt->world)
    {
        RWMESSAGE((RWSTRING("Atomic destroyed whilst still in a world")));
        RpWorldRemoveAtomic(atomicExt->world, atomic);
    }
#endif /* RWDEBUG */

    /* Reverse the interface overload */
    atomic->object.sync = atomicExt->oldSync;

    RWRETURN(object);
}

/******************************************************************************
 WorldInitClumpExt

 Initialise the clump to something reasonable

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldInitClumpExt(void *object,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    RpWorldClumpExt    *clumpExt = CLUMPEXTFROMCLUMP(object);

    RWFUNCTION(RWSTRING("WorldInitClumpExt"));

    RWASSERT(object);

    /* Not in a world yet */
    clumpExt->world = (RpWorld *)NULL;
    clumpExt->clumpsInFrustumID = RPWORLDOBJGLOBAL(clumpsInFrustumID);

    RWRETURN(object);
}

/******************************************************************************
 WorldCopyClumpExt

 Copy from another

 On entry   : Object, object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldCopyClumpExt(void *dstObject, const void *srcObject,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    const RpWorldClumpExt *srcClumpExt =
        CLUMPEXTFROMCONSTCLUMP(srcObject);

    RWFUNCTION(RWSTRING("WorldCopyClumpExt"));

    RWASSERT(dstObject);
    RWASSERT(srcObject);

    if (srcClumpExt->world)
    {
        /* Copy the world */
        RpWorldAddClump(srcClumpExt->world, (RpClump *) dstObject);
    }

    RWRETURN(dstObject);
}

/******************************************************************************
 WorldDeInitClumpExt

 De-initialises the clump

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldDeInitClumpExt(void *object,
                    RwInt32 offset __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("WorldDeInitClumpExt"));
    RWASSERT(object);

    RWASSERTM(!(CLUMPEXTFROMCLUMP(object)->world),
        (RWSTRING("Clump destroyed whilst still in a world")));

    RWRETURN(object);
}

/****************************************************************************
 LightDestroyTies

 On entry   : light
 On exit    :
 */

static void
LightDestroyTies(RpLight * light)
{
    RwLLLink           *cur, *end;
    RpLightTie         *tie;

    RWFUNCTION(RWSTRING("LightDestroyTies"));

    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    cur = rwLinkListGetFirstLLLink(&light->WorldSectorsInLight);
    end = rwLinkListGetTerminator(&light->WorldSectorsInLight);

    while (cur != end)
    {
        /* Remove the tie */
        tie = rwLLLinkGetData(cur, RpLightTie, WorldSectorInLight);
        cur = rwLLLinkGetNext(cur);
        _rpLightTieDestroy(tie);
    }

    RWRETURNVOID();
}

/******************************************************************************
 WorldLightSync

 Sync the light - find sectors affected

 On entry   : light
 On exit    : light pointer on success
 */

static RwObjectHasFrame *
WorldLightSync(RwObjectHasFrame * object)
{
    RpLight            *light = (RpLight *) object;
    RpWorldLightExt    *lightExt = LIGHTEXTFROMLIGHT(light);

    RWFUNCTION(RWSTRING("WorldLightSync"));

    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* Do the default thing first */
    if (lightExt->oldSync(object))
    {
        RpWorld            *world;
        RwFrame            *lightFrame;
        RpLightType         type;

        /* Now attach to world sectors */

        /* Some lights don't need to be positioned */
        type = RpLightGetType(light);
        if ((RwInt32) type < (RwInt32) rpLIGHTPOSITIONINGSTART)
        {
            RWRETURN(object);
        }

        /* Must belong to a world and have a frame to have ties */
        world = lightExt->world;
        lightFrame = RpLightGetFrame(light);
        if (world && lightFrame)
        {
            RpSector           *sect;
            RpSector           *spaStack[rpWORLDMAXBSPDEPTH];
            RwInt32             nStack = 0;
            RwV3d               inf, sup;
            RwReal              radius = light->radius;

            RWASSERTISTYPE(world, rpWORLD);

            /* Remove the lights current ties */
            LightDestroyTies(light);

            /* Set up the bounding box */
            inf = RwFrameGetLTM(lightFrame)->pos;
            sup = inf;

            sup.x += radius;
            sup.y += radius;
            sup.z += radius;

            inf.x -= radius;
            inf.y -= radius;
            inf.z -= radius;

            /* Start at the top */
            sect = world->rootSector;

            do
            {
                if (sect->type < 0)
                {
                    RpLightTie         *tie;

                    /* Its an atomic sector */
                    tie = (RpLightTie *)RwFreeListAlloc(RPWORLDOBJGLOBAL(lightTieFreeList),
                                                   rwMEMHINTDUR_EVENT | rwID_WORLDMODULE);

                    tie->sect = (RpWorldSector *) sect;
                    tie->light = light;

                    rwLinkListAddLLLink(&((RpWorldSector *)sect)->lightsInWorldSector,
                                        &tie->lightInWorldSector);
                    rwLinkListAddLLLink(&light->WorldSectorsInLight,
                                        &tie->WorldSectorInLight);

                    /* Try next */
                    sect = spaStack[nStack--];
                }
                else
                {
                    RpPlaneSector      *pspPlane = (RpPlaneSector *) sect;

                    /* Its a plane */

                    /* Left */
                    if (GETCOORD(inf, sect->type) < pspPlane->leftValue)
                    {
                        sect = pspPlane->leftSubTree;
                        /* Right */
                        if (pspPlane->rightValue < GETCOORD(sup, pspPlane->type))
                        {
                            spaStack[++nStack] = pspPlane->rightSubTree;
                        }
                    }
                    else
                    {
                        /* Right */
                        if (pspPlane->rightValue < GETCOORD(sup, pspPlane->type))
                        {
                            sect = pspPlane->rightSubTree;
                        }
                        else
                        {
                            /* this case can occur although  */
                            /* very unlikely this line stops */
                            /* RW ending up in an infinite   */
                            /* loop                          */

                            sect = spaStack[nStack--];
                        }
                    }
                }
            }
            while (nStack >= 0);
        }
    }
    else
    {
        RWERROR((E_RW_NULLP));
    }

    /* All done */
    RWRETURN(object);
}

/******************************************************************************
 WorldInitLightExt

 Initialise the light to something reasonable

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldInitLightExt(void *object,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    RpLight            *light = (RpLight *) object;
    RpWorldLightExt    *lightExt = LIGHTEXTFROMLIGHT(object);

    RWFUNCTION(RWSTRING("WorldInitLightExt"));

    RWASSERT(object);

    /* Not in a world yet */
    lightExt->world = (RpWorld *)NULL;

    /* Overload the sync function */
    lightExt->oldSync = light->object.sync;
    light->object.sync = WorldLightSync;

    RWRETURN(object);
}

/******************************************************************************
 WorldCopyLightExt

 Copy from another

 On entry   : Object, object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldCopyLightExt(void *dstObject,
                  const void *srcObject,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    const RpWorldLightExt *srcLightExt =
        LIGHTEXTFROMCONSTLIGHT(srcObject);

    RWFUNCTION(RWSTRING("WorldCopyLightExt"));

    RWASSERT(dstObject);
    RWASSERT(srcObject);

    if (srcLightExt->world)
    {
        /* Copy the world */
        RpWorldAddLight(srcLightExt->world, (RpLight *) dstObject);
    }

    RWRETURN(dstObject);
}

/******************************************************************************
 WorldDeInitLightExt

 De-initialises the light

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
WorldDeInitLightExt(void *object,
                    RwInt32 offset __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    RpLight            *light = (RpLight *) object;

#ifdef RWDEBUG
    RpWorldLightExt    *lightExt = LIGHTEXTFROMLIGHT(object);

#endif /* RWDEBUG */

    RWFUNCTION(RWSTRING("WorldDeInitLightExt"));

    RWASSERT(object);

#ifdef RWDEBUG
    if (lightExt->world)
    {
        RWMESSAGE((RWSTRING("Light destroyed whilst still in a world")));
        RpWorldRemoveLight(lightExt->world, light);
    }
#endif /* RWDEBUG */

    /* Blow away it's ties to world sectors */
    LightDestroyTies(light);

    RWRETURN(object);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Atomic sectors and cameras

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/****************************************************************************
 WorldSyncCamera

 On entry   : World
            : Camera
 On exit    : World pointer on success
 */

static RpWorld     *
WorldSyncCamera(RpWorld * world, RwCamera * camera)
{
    RpSector           *spaStack[rpWORLDMAXBSPDEPTH];
    RpSector           *spSect;
    RwV3d               vViewPoint;
    RwV3d               inf, sup;
    const RwFrustumPlane *plpPlanes;
    RpWorldCameraExt   *cameraExt;
    RwBool              goBackToFront;
    RwInt32             nStack = 0;
    RwInt32             position = 0;

    RWFUNCTION(RWSTRING("WorldSyncCamera"));

    RWASSERT(world);
    RWASSERT(camera);

    /* Quick access */
    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);
    vViewPoint = RwFrameGetLTM(RwCameraGetFrame(camera))->pos;
    inf = camera->frustumBoundBox.inf;
    sup = camera->frustumBoundBox.sup;
    plpPlanes = camera->frustumPlanes;
    goBackToFront = (world->renderOrder == rpWORLDRENDERBACK2FRONT);

    /* Find out all of the intersecting atomic sectors */
    spSect = cameraExt->world->rootSector;
    do
    {
        const RwBool isAtomic = (spSect->type < 0);

        if (isAtomic)
        {
            /* Its an atomic/leaf sector */
            RwBool              outside;
            RwInt32             i;
            RpWorldSector     * worldSector = (RpWorldSector *) spSect;
            const RwV3d * const base = (RwV3d *)&worldSector->boundingBox;

            /* Is it in the viewing frustum ? */

            for (i = 0; i < 6; i++)
            {
                RwV3d               vCorner;
                RwSplitBits         sbSide;

                /* Check sector against plane */
                vCorner.x = base[plpPlanes[i].closestX].x;
                vCorner.y = base[plpPlanes[i].closestY].y;
                vCorner.z = base[plpPlanes[i].closestZ].z;

                sbSide.nReal =
                    RwV3dDotProduct(&vCorner, &plpPlanes[i].plane.normal) -
                    plpPlanes[i].plane.distance;

                outside = (sbSide.nInt > 0);

                if (outside)
                {
                    /* Its outside the plane */
                    break;
                }
            }

            /* Only add if it is in the view frustum */
            if (!outside)
            {
                /* Could assert here
                 * that camera->frustumBoundBox contains worldSector ...
                 */

                /* Ensure there's enough space */
                if (position >= cameraExt->space)
                {
                    const RwInt32 nNum = rwCAMERADISPLAYSECTORSINCREASE;

                    if (!SectorsInFrustumAddSpace(cameraExt, nNum))
                    {
                        /* Give up, we can't allocate more space */
                        cameraExt->position = position;
                        RWRETURN(world);
                    }
                }

                /* Append to list */
                cameraExt->frustumSectors[position++] = worldSector;
            }

            /* Recurse from stack*/
            spSect = spaStack[nStack--];
        }
        else
        {
            /* Its a splitting plane */
            RpPlaneSector      *pspPlane = (RpPlaneSector *) spSect;
            RwSplitBits         sbLeft;
            RwSplitBits         sbRight;

            sbLeft.nReal = GETCOORD(inf, spSect->type) - pspPlane->leftValue;
            sbRight.nReal = pspPlane->rightValue - GETCOORD(sup, spSect->type);

            if ((sbLeft.nInt < 0) && (sbRight.nInt < 0))
            {
                const RwBool  viewPointIsHigher =
                    GETCOORD(vViewPoint, pspPlane->type) > pspPlane->value;

                /* Do both sectors */
                if (  (goBackToFront && viewPointIsHigher) ||
                     !(goBackToFront || viewPointIsHigher) )
                {
                    /* Left first then right */
                    spSect = pspPlane->leftSubTree;
                    spaStack[++nStack] = pspPlane->rightSubTree;
                }
                else
                {
                    /* Right first then left */
                    spSect = pspPlane->rightSubTree;
                    spaStack[++nStack] = pspPlane->leftSubTree;
                }
            }
            else
            {
                /* Do one or the other - Must do one of these */
                spSect = (sbLeft.nInt < 0)?
                    (pspPlane->leftSubTree) : (pspPlane->rightSubTree);
            }
        }
    }
    while (nStack >= 0);

    cameraExt->position = position;

    RWRETURN(world);
}

/****************************************************************************
 WorldAddClumpAtomic

 On entry   : Atomic
            : User data pointer (world)
 On exit    : Atomic pointer on success
 */

static RpAtomic    *
WorldAddClumpAtomic(RpAtomic * atomic, void *data)
{
    RWFUNCTION(RWSTRING("WorldAddClumpAtomic"));

    RWASSERT(atomic);
    RWASSERT(data);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RpWorldAddAtomic((RpWorld *) data, atomic);
    RWRETURN(atomic);
}

/****************************************************************************
 WorldRemoveClumpAtomic

 On entry   : Atomic
            : User data pointer (world)
 On exit    : Atomic pointer on success
 */

static RpAtomic    *
WorldRemoveClumpAtomic(RpAtomic * atomic, void *data)
{
    RpWorld * const     world = (RpWorld *)data;
    RpWorldAtomicExt   *atomicExt;
    RWFUNCTION(RWSTRING("WorldRemoveClumpAtomic"));

    RWASSERT(atomic);
    RWASSERT(world);
    RWASSERTISTYPE(atomic, rpATOMIC);
    RWASSERTISTYPE(world, rpWORLD);

    atomicExt = ATOMICEXTFROMATOMIC(atomic);

    if ( atomicExt && (world == atomicExt->world))
    {
        RpWorldRemoveAtomic(world, atomic);
    }

    RWRETURN(atomic);
}


/****************************************************************************
 WorldAddClumpLight

 On entry   : Light
            : User data pointer (world)
 On exit    : Light pointer on success
 */

static RpLight    *
WorldAddClumpLight(RpLight *light, void *data)
{
    RWFUNCTION(RWSTRING("WorldAddClumpLight"));

    RWASSERT(light);
    RWASSERT(data);
    RWASSERTISTYPE(light, rpLIGHT);

    RpWorldAddLight((RpWorld *) data, light);
    RWRETURN(light);
}

/****************************************************************************
 WorldRemoveClumpLight

 On entry   : Light
            : User data pointer (world)
 On exit    : Light pointer on success
 */

static RpLight    *
WorldRemoveClumpLight(RpLight * light, void *data)
{
    RpWorld * const     world = (RpWorld *)data;
    RpWorldLightExt   *lightExt;
    RWFUNCTION(RWSTRING("WorldRemoveClumpLight"));

    RWASSERT(light);
    RWASSERT(world);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERTISTYPE(world, rpWORLD);

    lightExt = LIGHTEXTFROMLIGHT(light);

    if ( lightExt && (world == lightExt->world))
    {
        RpWorldRemoveLight(world, light);
    }

    RWRETURN(light);
}

/****************************************************************************
 WorldAddClumpCamera

 On entry   : Camera
            : User data pointer (world)
 On exit    : Camera pointer on success
 */

static RwCamera    *
WorldAddClumpCamera(RwCamera * camera, void *data)
{
    RWFUNCTION(RWSTRING("WorldAddClumpCamera"));

    RWASSERT(camera);
    RWASSERT(data);
    RWASSERTISTYPE(camera, rwCAMERA);

    RpWorldAddCamera((RpWorld *) data, camera);
    RWRETURN(camera);
}

/****************************************************************************
 WorldRemoveClumpCamera

 On entry   : Camera
            : User data pointer (world)
 On exit    : Camera pointer on success
 */

static RwCamera    *
WorldRemoveClumpCamera(RwCamera * camera, void *data)
{
    RpWorld * const     world = (RpWorld *)data;
    RpWorldCameraExt   *cameraExt;
    RWFUNCTION(RWSTRING("WorldRemoveClumpCamera"));

    RWASSERT(camera);
    RWASSERT(world);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERTISTYPE(world, rpWORLD);

    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    if ( cameraExt && (world == cameraExt->world))
    {
        RpWorldRemoveCamera(world, camera);
    }

    RWRETURN(camera);
}


/****************************************************************************
 writeGeometryMesh

 Writes a geometry's mesh to the given stream

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes) (not used)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */

static RwStream    *
writeGeometryMesh(RwStream * stream,
                  RwInt32 binaryLength __RWUNUSED__,
                  const void *object,
                  RwInt32 offsetInObject __RWUNUSED__,
                  RwInt32 sizeInObject __RWUNUSED__)
{
    const RpGeometry   *geometry = (const RpGeometry *) object;

    RWFUNCTION(RWSTRING("writeGeometryMesh"));

    RWASSERT(geometry);
    RWASSERT(stream);

    RWRETURN(_rpMeshWrite(geometry->mesh, (const void *) geometry,
                          stream, &geometry->matList));
}

/****************************************************************************
 readGeometryMesh

 Reads a geometry's mesh from the given stream

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */

static RwStream    *
readGeometryMesh(RwStream * stream,
                 RwInt32 binaryLength __RWUNUSED__,
                 void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    RpGeometry         *geometry = (RpGeometry *) object;

    RWFUNCTION(RWSTRING("readGeometryMesh"));

    RWASSERT(geometry);
    RWASSERT(stream);

    geometry->mesh = _rpMeshRead(stream, (const void *) geometry,
                        &geometry->matList);
    if (geometry->mesh)
    {
        RWRETURN(stream);
    }
    else
    {
        RWRETURN((RwStream *)NULL);
    }
}

/****************************************************************************
 sizeGeometryMesh

 Gets the size of a geometry's mesh (when serialised)

 On entry   : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Size of mesh when serialised (in bytes)
 */

static RwInt32
sizeGeometryMesh(const void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject __RWUNUSED__)
{
    const RpGeometry   *geometry = (const RpGeometry *)object;

    RWFUNCTION(RWSTRING("sizeGeometryMesh"));
    RWASSERT(geometry);

    RWRETURN(_rpMeshSize(geometry->mesh, object));
}

/****************************************************************************
 writeGeometryNative

 Writes a native geometry (instanced data)

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes) (not used)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */
static RwStream *
writeGeometryNative(RwStream *stream,
                    RwInt32 binaryLength __RWUNUSED__,
                    const void *object,
                    RwInt32 offsetInObject __RWUNUSED__,
                    RwInt32 sizeInObject __RWUNUSED__)
{
    const RpGeometry    *geometry = (const RpGeometry *)object;

    RWFUNCTION(RWSTRING("writeGeometryNative"));
    RWASSERT(geometry);
    RWASSERT(stream);

    RWRETURN(_rpGeometryNativeWrite(stream, geometry));
}

/****************************************************************************
 readGeometryNative

 Reads a native geometry (instanced data)

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */
static RwStream *
readGeometryNative(RwStream *stream,
                   RwInt32 binaryLength __RWUNUSED__,
                   void *object,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    RpGeometry  *geometry = (RpGeometry *)object;

    RWFUNCTION(RWSTRING("readGeometryNative"));
    RWASSERT(geometry);
    RWASSERT(stream);

    if (_rpGeometryNativeRead(stream, geometry))
    {
        RWRETURN(stream);
    }
    else
    {
        RWRETURN((RwStream *)NULL);
    }
}

/****************************************************************************
 sizeGeometryNative

 Gets the size of a geometry's mesh (when serialised)

 On entry   : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Size of mesh when serialised (in bytes)
 */
static RwInt32
sizeGeometryNative(const void *object,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    const RpGeometry    *geometry = (const RpGeometry *)object;

    RWFUNCTION(RWSTRING("sizeGeometryNative"));

    RWASSERT(geometry);

    RWRETURN(_rpGeometryNativeSize(geometry));
}

/****************************************************************************
 writeWorldSectorNative

 Writes a native world sector (instanced data)

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes) (not used)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */
static RwStream *
writeWorldSectorNative(RwStream *stream,
                       RwInt32 binaryLength __RWUNUSED__,
                       const void *object,
                       RwInt32 offsetInObject __RWUNUSED__,
                       RwInt32 sizeInObject __RWUNUSED__)
{
    const RpWorldSector *sector = (const RpWorldSector *)object;

    RWFUNCTION(RWSTRING("writeWorldSectorNative"));
    RWASSERT(sector);
    RWASSERT(stream);

    RWRETURN(_rpWorldSectorNativeWrite(stream, sector));
}

/****************************************************************************
 readWorldSectorNative

 Reads a native world sector (instanced data)

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes)
            : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */
static RwStream *
readWorldSectorNative(RwStream *stream,
                      RwInt32 binaryLength __RWUNUSED__,
                      void *object,
                      RwInt32 offsetInObject __RWUNUSED__,
                      RwInt32 sizeInObject __RWUNUSED__)
{
    RpWorldSector   *sector = (RpWorldSector *)object;

    RWFUNCTION(RWSTRING("readWorldSectorNative"));
    RWASSERT(sector);
    RWASSERT(stream);

    if (_rpWorldSectorNativeRead(stream, sector))
    {
        RWRETURN(stream);
    }
    else
    {
        RWRETURN((RwStream *)NULL);
    }
}

/****************************************************************************
 sizeWorldSectorNative

 Gets the size of a world sector's mesh (when serialised)

 On entry   : Object (geometry)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Size of mesh when serialised (in bytes)
 */
static RwInt32
sizeWorldSectorNative(const void *object,
                      RwInt32 offsetInObject __RWUNUSED__,
                      RwInt32 sizeInObject __RWUNUSED__)
{
    const RpWorldSector *sector = (const RpWorldSector *)object;

    RWFUNCTION(RWSTRING("sizeWorldSectorNative"));
    RWASSERT(sector);

    RWRETURN(_rpWorldSectorNativeSize(sector));
}

/****************************************************************************
 writeSectorMesh

 Writes a sector's mesh to the given stream

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes) (not used)
            : Object (world sector)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */

static RwStream    *
writeSectorMesh(RwStream * stream,
                RwInt32 binaryLength __RWUNUSED__,
                const void *object,
                RwInt32 offsetInObject __RWUNUSED__,
                RwInt32 sizeInObject __RWUNUSED__)
{
    const RpWorldSector *sector = (const RpWorldSector *) object;
    const RpWorld      *world;

    RWFUNCTION(RWSTRING("writeSectorMesh"));

    RWASSERT(sector);
    RWASSERT(stream);

    world = RpWorldSectorGetWorld(sector);
    RWRETURN(_rpMeshWrite(sector->mesh, (const void *) world,
                          stream, &world->matList));
}

/****************************************************************************
 readSectorMesh

 Reads a sector's mesh from the given stream

 On entry   : Stream to read from
            : Size of meshed when serialised (in bytes)
            : Object (world sector)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Stream pointer on success
 */

static RwStream    *
readSectorMesh(RwStream * stream,
               RwInt32 binaryLength __RWUNUSED__,
               void *object,
               RwInt32 offsetInObject __RWUNUSED__,
               RwInt32 sizeInObject __RWUNUSED__)
{
    RpWorldSector      *sector = (RpWorldSector *) object;
    const RpWorld      *world;

    RWFUNCTION(RWSTRING("readSectorMesh"));

    RWASSERT(sector);
    RWASSERT(stream);

    world = RpWorldSectorGetWorld(sector);
    sector->mesh =
        _rpMeshRead(stream, (const void *) world, &world->matList);
    if (sector->mesh)
    {
        RWRETURN(stream);
    }
    else
    {
        RWRETURN((RwStream *)NULL);
    }
}

/****************************************************************************
 sizeSectorMesh

 Gets the size of a sector's mesh (when serialised)

 On entry   : Object (world sector)
            : Plugin data offset (not used)
            : Plugin data size (not used)
 On exit    : Size of mesh when serialised (in bytes)
 */

static RwInt32
sizeSectorMesh(const void *object,
               RwInt32 offsetInObject __RWUNUSED__,
               RwInt32 sizeInObject __RWUNUSED__)
{
    const RpWorldSector *sector = (const RpWorldSector *)object;

    RWFUNCTION(RWSTRING("sizeSectorMesh"));
    RWASSERT(sector);

    RWRETURN(_rpMeshSize(sector->mesh,
             (RwObject *)RpWorldSectorGetWorld(sector)));
}

/******************************************************************************
 _rpWorldObjRegisterExtensions

 Registers the necessary memory/functions to maintain objects in the world

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rpWorldObjRegisterExtensions(void)
{
    RwInt32             status;

    RWFUNCTION(RWSTRING("_rpWorldObjRegisterExtensions"));

    /* Just try and register everything, then see later if we failed. */

    /* This registers global memory and the module with the engine for instance intialization */
    status = RwEngineRegisterPlugin(sizeof(rpWorldObjGlobals),
                                    rwID_WORLDOBJMODULE,
                                    WorldObjectOpen, WorldObjectClose);

    /* This attaches the necessary data to lights, clumps, atomics and cameras
     * to facilitate their placement and maintenance within a world.
     * None saves binary data, so use not a TK ID
     */
    cameraExtOffset = RwCameraRegisterPlugin(sizeof(RpWorldCameraExt),
                                             rwID_WORLDOBJMODULE,
                                             WorldInitCameraExt,
                                             WorldDeInitCameraExt,
                                             WorldCopyCameraExt);
    status |= cameraExtOffset;

    atomicExtOffset = RpAtomicRegisterPlugin(sizeof(RpWorldAtomicExt),
                                             rwID_WORLDOBJMODULE,
                                             WorldInitAtomicExt,
                                             WorldDeInitAtomicExt,
                                             WorldCopyAtomicExt);
    status |= atomicExtOffset;

    clumpExtOffset = RpClumpRegisterPlugin(sizeof(RpWorldClumpExt),
                                           rwID_WORLDOBJMODULE,
                                           WorldInitClumpExt,
                                           WorldDeInitClumpExt,
                                           WorldCopyClumpExt);
    status |= clumpExtOffset;

    lightExtOffset = RpLightRegisterPlugin(sizeof(RpWorldLightExt),
                                           rwID_WORLDOBJMODULE,
                                           WorldInitLightExt,
                                           WorldDeInitLightExt,
                                           WorldCopyLightExt);
    status |= lightExtOffset;

    /* And now the mesh serialisation modules for geometry and sectors.
     * These are used just to register something to hand serialisation
     * callbacks from.
     */
    status |=
        RpGeometryRegisterPlugin(0,
                                 rwID_BINMESHPLUGIN,
                                 (RwPluginObjectConstructor)NULL,
                                 (RwPluginObjectDestructor)NULL,
                                 (RwPluginObjectCopy)NULL);

    status |=
        RpWorldSectorRegisterPlugin(0,
                                    rwID_BINMESHPLUGIN,
                                    (RwPluginObjectConstructor)NULL,
                                    (RwPluginObjectDestructor)NULL,
                                    (RwPluginObjectCopy)NULL);
    status |=
        RpGeometryRegisterPluginStream(rwID_BINMESHPLUGIN,
                                       readGeometryMesh,
                                       writeGeometryMesh,
                                       sizeGeometryMesh);
    status |=
        RpWorldSectorRegisterPluginStream(rwID_BINMESHPLUGIN,
                                          readSectorMesh,
                                          writeSectorMesh,
                                          sizeSectorMesh);

    /* Platform specific serialization */
    status |= RpGeometryRegisterPlugin(0, rwID_NATIVEDATAPLUGIN,
                                       (RwPluginObjectConstructor)NULL,
                                       (RwPluginObjectDestructor)NULL,
                                       (RwPluginObjectCopy)NULL);

    status |= RpWorldSectorRegisterPlugin(0, rwID_NATIVEDATAPLUGIN,
                                          (RwPluginObjectConstructor)NULL,
                                          (RwPluginObjectDestructor)NULL,
                                          (RwPluginObjectCopy)NULL);

    status |= RpGeometryRegisterPluginStream(rwID_NATIVEDATAPLUGIN,
                                             readGeometryNative,
                                             writeGeometryNative,
                                             sizeGeometryNative);

    status |= RpWorldSectorRegisterPluginStream(rwID_NATIVEDATAPLUGIN,
                                                readWorldSectorNative,
                                                writeWorldSectorNative,
                                                sizeWorldSectorNative);

    /* We now attached the rights serialisation plugin. */
    status |= RpAtomicRegisterPlugin(0, rwID_RIGHTTORENDER,
                                     (RwPluginObjectConstructor)NULL,
                                     (RwPluginObjectDestructor)NULL,
                                     (RwPluginObjectCopy)NULL);

    status |= RpAtomicRegisterPluginStream(rwID_RIGHTTORENDER,
                                           _rpReadAtomicRights,
                                           _rpWriteAtomicRights,
                                           _rpSizeAtomicRights);

    status |= RpWorldRegisterPlugin(0, rwID_RIGHTTORENDER,
                                    (RwPluginObjectConstructor)NULL,
                                    (RwPluginObjectDestructor)NULL,
                                    (RwPluginObjectCopy)NULL);

    status |= RpWorldRegisterPluginStream(rwID_RIGHTTORENDER,
                                          _rpReadWorldRights,
                                          _rpWriteWorldRights,
                                          _rpSizeWorldRights);

    status |= RpWorldSectorRegisterPlugin(0, rwID_RIGHTTORENDER,
                                          (RwPluginObjectConstructor)NULL,
                                          (RwPluginObjectDestructor)NULL,
                                          (RwPluginObjectCopy)NULL);

    status |= RpWorldSectorRegisterPluginStream(rwID_RIGHTTORENDER,
                                                _rpReadSectRights,
                                                _rpWriteSectRights,
                                                _rpSizeSectRights);

    status |= RpMaterialRegisterPlugin(0, rwID_RIGHTTORENDER,
                                          (RwPluginObjectConstructor)NULL,
                                          (RwPluginObjectDestructor)NULL,
                                          (RwPluginObjectCopy)NULL);

    status |= RpMaterialRegisterPluginStream(rwID_RIGHTTORENDER,
                                                _rpReadMaterialRights,
                                                _rpWriteMaterialRights,
                                                _rpSizeMaterialRights);

    if (status < 0)
    {
        /* Something went wrong */
        RWRETURN(FALSE);
    }

    /* And the pipe specific extensions */
    if (!_rpWorldPipeAttach())
    {
        /* Oooops */
        RWRETURN(FALSE);
    }

    /* Success */
    RWRETURN(TRUE);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldAddCamera is used to add the specified camera to the
 * given world. Cameras must be added to a world before they can be used to
 * render objects.
 *
 * This function is a world plugin extension and the world plugin must be attached
 * before using this function.
 *
 * \param world  Pointer to the world.
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to the world if successful or NULL if
 * there is an error.
 *
 * \warning This function can only be called outside of the 
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block.
 *
 * \see RpWorldRemoveCamera
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddClump
 * \see RpWorldRemoveClump
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpWorldCreate
 * \see RwCameraCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldAddCamera(RpWorld * world, RwCamera * camera)
{
    RwFrame            *frame;
    RpWorldCameraExt   *cameraExt;

    RWAPIFUNCTION(RWSTRING("RpWorldAddCamera"));

    RWASSERT(world);
    RWASSERT(camera);
    RWASSERT(worldObjModule.numInstances);

    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERTISTYPE(world, rpWORLD);

    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);
    RWASSERT(!cameraExt->world); /* Should not be in a world */

    frame = RwCameraGetFrame(camera);
    if (frame)
    {
        /* Invalidate the frame -> forcing camera to be positioned */
        RwFrameUpdateObjects(frame);
    }

    /* Say that its in this world */
    cameraExt->world = world;

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldRemoveCamera is used to remove the specified camera from
 * the given world. Unlike \ref RwCameraDestroy this function preserves the
 * camera's definition, hence the camera can be reused without creating it
 * again.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the camera.
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to the world if successful or NULL if there is
 * an error.
 *
 * \see RpWorldAddCamera
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveClump
 * \see RpWorldAddClump
 * \see RpWorldRemoveLight
 * \see RpWorldAddLight
 * \see RwCameraDestroy
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldRemoveCamera(RpWorld * world, RwCamera * camera)
{
    RpWorldCameraExt   *cameraExt;

    RWAPIFUNCTION(RWSTRING("RpWorldRemoveCamera"));

    RWASSERT(camera);
    RWASSERT(world);
    RWASSERT(worldObjModule.numInstances);

    RWASSERTISTYPE(world, rpWORLD);
    RWASSERTISTYPE(camera, rwCAMERA);

    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);

    RWASSERT(cameraExt->world); /* Should be in a world */
    RWASSERT(cameraExt->world == world); /* This one in fact */

    if (cameraExt->world)
    {
        RWASSERTISTYPE(cameraExt->world, rpWORLD);

        cameraExt->world = (RpWorld *)NULL;
        cameraExt->position = 0;

        RWRETURN(world);
    }

    /* It's not in a world */
    RWRETURN((RpWorld *)NULL);
}

/**
 * \ingroup rpworldrwcamera
 * \ref RwCameraGetWorld is used to determine the world the specified
 * camera belongs to, if any.
 *
 * This function is a world plugin extension and the world plugin must be attached
 * before using this function.
 *
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to the world which contains the camera if successful
 * or NULL if there is an error or if the camera does not belong to any world.
 *
 * \see RpWorldAddCamera
 * \see RpWorldRemoveCamera
 * \see RwCameraCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RwCameraGetWorld(const RwCamera * camera)
{
    const RpWorldCameraExt *cameraExt;

    RWAPIFUNCTION(RWSTRING("RwCameraGetWorld"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(camera);

    cameraExt = WORLDCAMERAEXTFROMCONSTCAMERA(camera);

    RWASSERTISTYPE(camera, rwCAMERA);
    RWRETURN(cameraExt->world);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                        Adding and removing atomics

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsub
 * \ref RpWorldAddAtomic is used to add the specified atomic to the
 * given world. Atomics must be added to a world if they are to be
 * rendered using \ref RpWorldRender or be illuminated by point, spot
 * or soft spot lights.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \warning This function can only be called outside of the 
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block.
 *
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddCamera
 * \see RpWorldRemoveCamera
 * \see RpWorldAddClump
 * \see RpWorldRemoveClump
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpWorldCreate
 * \see RpAtomicCreate
 * \see RpAtomicRender
 * \see RpClumpRender
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldAddAtomic(RpWorld * world, RpAtomic * atomic)
{
    RwFrame            *frame;
    RpWorldAtomicExt   *atomicExt;

    RWAPIFUNCTION(RWSTRING("RpWorldAddAtomic"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERT(atomic);

    RWASSERTISTYPE(world, rpWORLD);
    RWASSERTISTYPE(atomic, rpATOMIC);

    atomicExt = ATOMICEXTFROMATOMIC(atomic);

    RWASSERT(!atomicExt->world); /* Should not be in a world */

    frame = RpAtomicGetFrame(atomic);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    atomicExt->world = world;

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldRemoveAtomic is used to remove the specified atomic from
 * the given world. Unlike \ref RpAtomicDestroy this function preserves the
 * atomic's definition, hence the atomic can be reused without creating it
 * again.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the atomic.
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the world if successful or NULL if there is
 * an error.
 *
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveCamera
 * \see RpWorldAddCamera
 * \see RpWorldRemoveClump
 * \see RpWorldAddClump
 * \see RpWorldRemoveLight
 * \see RpWorldAddLight
 * \see RpAtomicDestroy
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldRemoveAtomic(RpWorld * world, RpAtomic * atomic)
{
    RpWorldAtomicExt   *atomicExt;

    RWAPIFUNCTION(RWSTRING("RpWorldRemoveAtomic"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERT(atomic);

    RWASSERTISTYPE(world, rpWORLD);
    RWASSERTISTYPE(atomic, rpATOMIC);

    atomicExt = ATOMICEXTFROMATOMIC(atomic);

    RWASSERT(atomicExt->world); /* Should be in a world */
    RWASSERT(atomicExt->world == world); /* This one in fact */

    /* Blow away it's instance copy if it has one */

#if defined(PSP_DRVMODEL_H)
	/* This will avoid problems with PTank atomics on PSP, as 
	PTank data is stored in the atomic's RwResEntry. */
#else
    if (atomic->repEntry)
    {
        RwResourcesFreeResEntry(atomic->repEntry);
    }
#endif /* defined(PSP_DRVMODEL_H) */
    AtomicDestroyTies(atomic);

    atomicExt->world = (RpWorld *)NULL;

    RWRETURN(world);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetWorld is used to determine the world the specified
 * atomic belongs to, if any.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the world which contains the atomic if successful
 * or NULL if there is an error or if the atomic does not belong to any world.
 *
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveAtomic
 * \see RpAtomicCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpAtomicGetWorld(const RpAtomic * atomic)
{
    const RpWorldAtomicExt *atomicExt;

    RWAPIFUNCTION(RWSTRING("RpAtomicGetWorld"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(atomic);

    RWASSERTISTYPE(atomic, rpATOMIC);

    atomicExt = ATOMICEXTFROMCONSTATOMIC(atomic);

    RWRETURN(atomicExt->world);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicForAllWorldSectors is used to apply the given callback
 * function to all world sectors the specified atomic is in. The format of the
 * callback function is:
 *
 * RpWorldSector *(*RpWorldSectorCallBack)( RpWorldSector *sector, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated. However, RpAtomicForAllWorldSectors will still return
 * successfully.

 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param callback  Callback function to be called with each world sector.
 * \param data  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns the atomic pointer on success, or NULL on failure.
 *
 * \see RpWorldSectorForAllAtomics
 * \see RpWorldPluginAttach
 */
RpAtomic           *
RpAtomicForAllWorldSectors(RpAtomic * atomic,
                           RpWorldSectorCallBack callback, void *data)
{
    RwLLLink           *cur, *end;

    RWAPIFUNCTION(RWSTRING("RpAtomicForAllWorldSectors"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(atomic);
    RWASSERT(callback);

    /* For all sectors that this atomic lies in, apply all lights within */
    cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
    end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);
    while (cur != end)
    {
        RpTie *tie = rwLLLinkGetData(cur, RpTie, lWorldSectorInAtomic);
        RwLLLink *next = rwLLLinkGetNext(cur);

        /* Call the callback for each sector */
        if (!callback(tie->worldSector, data))
        {
            RWRETURN(atomic);
        }

        /* Next one */
        cur = next;
    }

    RWRETURN(atomic);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorForAllAtomics is used to apply the given callback
 * function to all atomics lying in the specified world sector. The format of
 * the callback function is:
 *
 * RpAtomic * (*RpAtomicCallBack)(RpAtomic *atomic, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated. However, \ref RpWorldSectorForAllAtomics will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the sector.
 * \param callback  Callback function to be called with each atomic.
 * \param data  User data pointer to be passed to the callback.
 *
 * \return Returns the world sector pointer on success, or NULL on failure.
 *
 * \see RpWorldSectorForAllCollisionAtomics
 * \see RpAtomicForAllWorldSectors
 * \see RpWorldSectorForAllLights
 * \see RpWorldPluginAttach
 */
RpWorldSector      *
RpWorldSectorForAllAtomics(RpWorldSector * sector,
                           RpAtomicCallBack callback, void *data)
{
    RwLLLink           *cur, *end;
    RpTie               dummyTie;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorForAllAtomics"));

    RWASSERT(sector);
    RWASSERT(callback);
    RWASSERT(worldObjModule.numInstances);

    /* Step through all the atomics in this sector */

    dummyTie.apAtom = (RpAtomic *)NULL;
    dummyTie.worldSector = (RpWorldSector *)NULL;

    /* High priority */
    cur = rwLinkListGetFirstLLLink(&(sector->collAtomicsInWorldSector));
    end = rwLinkListGetTerminator(&(sector->collAtomicsInWorldSector));
    while (cur != end)
    {
        RpTie *tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);

        /* Add in a dummy tie that we know won't be messed with */
        rwLinkListAddLLLink((RwLinkList *) cur, &dummyTie.lAtomicInWorldSector);

        /* NB tie may actually be a dummyTie from a enclosing ForAll */
        if (tie->apAtom)
            if (!callback(tie->apAtom, data))
            {
                /* Early out, but remove the dummy tie first */
                rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
                RWRETURN(sector);
            }

        /* Query the dummy tie for next pointer, and remove it */
        cur = rwLLLinkGetNext(&dummyTie.lAtomicInWorldSector);
        rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
    }

    RWRETURN(sector);
}


/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorForAllCollisionAtomics is used to apply the given callback
 * function to all atomics lying in the specified world sector which are
 * marked as collision atomics. This applies to atomics which have their
 * rpATOMICCOLLISIONTEST flag set (see \ref RpAtomicFlag), but note that any
 * change in this flag will not be registered until after the next
 * synchronistion phase during an \ref RwCameraBeginUpdate.
 *
 * The use of this function is potentially faster than using
 * \ref RpWorldSectorForAllAtomics and testing the collision flag for each
 * atomic.
 *
 * The format of the callback function is:
 *
 * RpAtomic * (*RpAtomicCallBack)(RpAtomic *atomic, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated. However, \ref RpWorldSectorForAllCollisionAtomics
 * will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the sector.
 * \param callback  Callback function to be called with each atomic.
 * \param data  User data pointer to be passed to the callback.
 *
 * \return Returns the world sector pointer on success, or NULL on failure.
 *
 * \see RpAtomicSetFlags
 * \see RpWorldSectorForAllAtomics
 * \see RpAtomicForAllWorldSectors
 * \see RpWorldSectorForAllLights
 * \see RpWorldPluginAttach
 */
RpWorldSector      *
RpWorldSectorForAllCollisionAtomics(RpWorldSector * sector,
                           RpAtomicCallBack callback, void *data)
{
    RwLLLink           *cur, *end;
    RpTie               dummyTie;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorForAllCollisionAtomics"));

    RWASSERT(sector);
    RWASSERT(callback);
    RWASSERT(worldObjModule.numInstances);

    /* Step through all the atomics in this sector */

    dummyTie.apAtom = (RpAtomic *)NULL;
    dummyTie.worldSector = (RpWorldSector *)NULL;

    /* High priority */
    cur = rwLinkListGetFirstLLLink(&(sector->collAtomicsInWorldSector));
    end = rwLinkListGetTerminator(&(sector->collAtomicsInWorldSector));
    while (cur != end)
    {
        RpTie *tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);

        /* Add in a dummy tie that we know won't be messed with */
        rwLinkListAddLLLink((RwLinkList *) cur, &dummyTie.lAtomicInWorldSector);

        /* NB tie may actually be a dummyTie from a enclosing ForAll */
        if (tie->apAtom)
            if (!callback(tie->apAtom, data))
            {
                /* Early out, but remove the dummy tie first */
                rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
                RWRETURN(sector);
            }

        /* Query the dummy tie for next pointer, and remove it */
        cur = rwLLLinkGetNext(&dummyTie.lAtomicInWorldSector);
        rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
    }

    RWRETURN(sector);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorForAllLights is used to apply the given callback
 * function to all lights lying in the specified world sector. The format of
 * the callback function is:
 *
 * RpLight * (*RpLightCallBack)(RpLight *light, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated. However, \ref RpWorldSectorForAllLights will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the sector.
 * \param callback  Callback function to be called with each light.
 * \param data  User data pointer to be passed to the callback.
 *
 * \return Returns the world sector pointer on success, or NULL on failure.
 *
 * \see RpLightForAllWorldSectors
 * \see RpWorldSectorForAllAtomics
 * \see RpWorldPluginAttach
 */
RpWorldSector      *
RpWorldSectorForAllLights(RpWorldSector * sector,
                          RpLightCallBack callback, void *data)
{
    RwLLLink           *cur, *end;
    RpWorld            *world;
    RpLightTie          dummyTie;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorForAllLights"));

    RWASSERT(sector);
    RWASSERT(callback);
    RWASSERT(worldObjModule.numInstances);

    dummyTie.light = (RpLight *)NULL;
    dummyTie.sect = (RpWorldSector *)NULL;

    /* Step through all the lights in this sector */
    cur = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
    end = rwLinkListGetTerminator(&sector->lightsInWorldSector);

    while (cur != end)
    {
        RpLightTie *lightTie = rwLLLinkGetData(cur, RpLightTie, lightInWorldSector);

        /* Add a dummy tie that we know won't be messed with */
        rwLinkListAddLLLink((RwLinkList *) cur, &dummyTie.lightInWorldSector);

        /* NB lightTie may actually be a dummyTie from a enclosing ForAll */
        if (lightTie->light)
            if (!callback(lightTie->light, data))
            {
                /* Early out, but remove the dummy tie first */
                rwLinkListRemoveLLLink(&dummyTie.lightInWorldSector);
                RWRETURN(sector);
            }

        /* Next */
        cur = rwLLLinkGetNext(&dummyTie.lightInWorldSector);

        rwLinkListRemoveLLLink(&dummyTie.lightInWorldSector);
    }

    /* Also all the non local lights in the world */
    world = RpWorldSectorGetWorld(sector);
    if (world)
    {
        /* Global lights */
        cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
        end = rwLinkListGetTerminator(&world->directionalLightList);
        while (cur != end)
        {
            RpLight *light = rwLLLinkGetData(cur, RpLight, inWorld);

            /* Add a dummy tie that we know won't be messed with */
            rwLinkListAddLLLink((RwLinkList *) cur,
                                &dummyTie.lightInWorldSector);

            /* NB light may actually be a dummyTie from a enclosing ForAll */
            if (light)
                if (!callback(light, data))
                {
                    /* Early out, but remove the dummy tie first */
                    rwLinkListRemoveLLLink(&dummyTie.lightInWorldSector);
                    RWRETURN(sector);
                }

            /* Next */
            cur = rwLLLinkGetNext(&dummyTie.lightInWorldSector);
            rwLinkListRemoveLLLink(&dummyTie.lightInWorldSector);
        }
    }

    RWRETURN(sector);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldAddClump is used to add the specified clump to the
 * given world. Clumps must be added to a world if they are to be rendered
 * using \ref RpWorldRender or be illuminated by point, spot or soft
 * spot lights.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param clump  Pointer to the clump.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \warning This function can only be called outside of the 
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block.
 *
 * \see RpWorldRemoveClump
 * \see RpWorldAddCamera
 * \see RpWorldRemoveCamera
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpWorldCreate
 * \see RpClumpCreate
 * \see RpWorldRender
 * \see RpClumpRender
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldAddClump(RpWorld * world, RpClump * clump)
{
    RpWorldClumpExt    *clumpExt;
    RwFrame            *clumpFrame = RpClumpGetFrame(clump);

    RWAPIFUNCTION(RWSTRING("RpWorldAddClump"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERT(clump);

    RWASSERTISTYPE(world, rpWORLD);
    RWASSERTISTYPE(clump, rpCLUMP);

    clumpExt = CLUMPEXTFROMCLUMP(clump);
    RWASSERT(!clumpExt->world); /* Should not be in a world */

    /* Check there are any atomics in it in the first place */
    rwLinkListAddLLLink(&world->clumpList, &clump->inWorldLink);

    world->numClumpsInWorld++;

    clumpExt->world = world;

    /* The atomics in the clump must be added to the world */
    RpClumpForAllAtomics(clump, WorldAddClumpAtomic, world);
    RpClumpForAllLights(clump,  WorldAddClumpLight, world);
    RpClumpForAllCameras(clump, WorldAddClumpCamera, world);

    if (clumpFrame)
    {
        /* When a clump is added to the world its matrix MUST be
         * marked as dirty, and the frame added to the dirty list.
         */
        /* RwMatrixUpdate(&clumpFrame->modelling); */
        RwMatrixOptimize(&clumpFrame->modelling,
                         (RwMatrixTolerance *) NULL);
        RwFrameUpdateObjects(clumpFrame);
    }

    /* Initialize renderFrame marker for frustum tests */
    clumpExt->clumpsInFrustumID = RPWORLDOBJGLOBAL(clumpsInFrustumID);

    /* Done */
    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldRemoveClump is used to remove the specified clump from
 * the given world. Unlike \ref RpClumpDestroy this function preserves the
 * clump's definition, hence the clump can be reused without creating it
 * again.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the clump.
 * \param clump  Pointer to the clump.
 *
 * \return Returns pointer to the world if successful or NULL if there is
 * an error.
 *
 * \see RpWorldAddClump
 * \see RpWorldRemoveCamera
 * \see RpWorldAddCamera
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveLight
 * \see RpWorldAddLight
 * \see RpClumpDestroy
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldRemoveClump(RpWorld * world, RpClump * clump)
{
    RpWorldClumpExt    *clumpExt;

    RWAPIFUNCTION(RWSTRING("RpWorldRemoveClump"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    clumpExt = CLUMPEXTFROMCLUMP(clump);

    RWASSERT(clumpExt->world); /* Should be in a world */
    RWASSERT(clumpExt->world == world); /* This one in fact */

    if (clumpExt->world)
    {
        RWASSERTISTYPE(clumpExt->world, rpWORLD);

        /* Make sure the # of clumps, and current are up to date */
        (clumpExt->world)->numClumpsInWorld--;

        if ((&clump->inWorldLink) ==
            (clumpExt->world)->currentClumpLink)
        {
            (clumpExt->world)->currentClumpLink =
                rwLLLinkGetNext(&clump->inWorldLink);
        }

        /* Remove link list from world */
        rwLinkListRemoveLLLink(&clump->inWorldLink);

        /* Remove ties */
        RpClumpForAllAtomics(clump, WorldRemoveClumpAtomic, world);
        RpClumpForAllLights(clump,  WorldRemoveClumpLight, world);
        RpClumpForAllCameras(clump, WorldRemoveClumpCamera, world);

        /* Mark as not in the world */
        clumpExt->world = (RpWorld *)NULL;

        /* Done */
        RWRETURN(world);
    }

    /* Not in a world */
    RWERROR((E_RP_WORLD_CLUMPNOTINWORLD));
    RWRETURN((RpWorld *)NULL);
}

/**
 * \ingroup rpworldrwcamera
 * \ref RwCameraForAllSectorsInFrustum is used to apply the given
 * callback function to all world sectors that lie inside the specified
 * camera's view frustum. World sectors that are only partially inside the
 * camera's frustum will also get a callback. The format of the callback
 * function is:
 * \verbatim
   RpWorldSector *(*RpWorldSectorCallBack)(RpWorldSector *sector, void *data);
   \endverbatim
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * \note If any invocation of the callback function returns a failure
 * status the iteration is terminated. However,
 * \ref RwCameraForAllSectorsInFrustum will still return successfully.
 *
 * \note The view frustum is only defined by the last render, so it is
 * recommended that this function is called after the scene has
 * been rendered.
 *
 * This function is a world plugin extension and the world plugin must be attached
 * before using this function.
 *
 * \param camera  Pointer to the camera whose frustum to enumerate world sectors within.
 * \param callback  Callback function to call with each world sector.
 * \param data  User data pointer to pass to the callback function.
 *
 * \return Returns pointer to the camera if successful or NULL if there is
 * an error.
 *
 * \see RwCameraForAllClumpsInFrustum
 * \see RpWorldSectorForAllAtomics
 * \see RpWorldSectorForAllLights
 */

RwCamera           *
RwCameraForAllSectorsInFrustum(RwCamera * camera,
                               RpWorldSectorCallBack callback,
                               void *data)
{
    RpWorldCameraExt   *cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);
    RpWorldSector     **frustumSectors;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwCameraForAllSectorsInFrustum"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(camera);
    RWASSERT(callback);

    RWASSERTISTYPE(camera, rwCAMERA);

    frustumSectors = cameraExt->frustumSectors;

    /* We may have no sectors if the camera is out of the world, but
     * position will be zero in this case.
     */
    for (i = cameraExt->position; i; i--)
    {
        RWASSERT(*frustumSectors);

        if (!callback(*frustumSectors, data))
        {
            /* Early out !!!! */
            RWRETURN(camera);
        }

        frustumSectors++;
    }

    RWRETURN(camera);
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetWorld is used to determine the world the specified
 * clump belongs to, if any.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns pointer to the world which contains the clump if successful
 * or NULL if there is an error or if the clump does not belong to any world.
 *
 * \see RpWorldAddClump
 * \see RpWorldRemoveClump
 * \see RpClumpCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpClumpGetWorld(const RpClump * clump)
{
    const RpWorldClumpExt *clumpExt;

    RWAPIFUNCTION(RWSTRING("RpClumpGetWorld"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    clumpExt = CLUMPEXTFROMCONSTCLUMP(clump);

    RWRETURN(clumpExt->world);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Finding what's in the frustum

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldrwcamera
 * \ref RwCameraForAllClumpsInFrustum is used to determine which
 * clumps lie wholly or partially within the specified camera's view frustum
 * and then to execute each of those clump's callback function (see
 * \ref RpClumpSetCallBack). The enumeration may be terminated at any time
 * by returning NULL from the clump callback.
 *
 * \note The view frustum is only defined by the last render, so
 * this function must be called after a scene has been rendered.
 *
 * The results of calling function can be thought of as equivalent
 * to the following scheme:-
 *
 * -# \ref RwCameraForAllSectorsInFrustum to iterate through the current set
 *    of frustum sectors
 * -# \ref RpWorldSectorForAllAtomics to iterate over all atomics in a
 *    sector.
 * -# \ref RpAtomicGetClump to get the \ref RpClump that contains a
 *    particular atomic. At this stage, a quick check can be made that the
 *    clump hasn't already been counted.
 * -# \ref RwCameraFrustumTestSphere to finally check whether the clump's
 *    atomic is definitely inside the camera frustum.
 * -# Execute the clump's callback function.
 *
 * Variations on this scheme may be used for other situations,
 * such as operating just on the atomics contained in the frustum.
 *
 * The world plugin must be attached before using this function.
 *
 * \param camera    Pointer to the camera.
 * \param data      User data pointer to pass to each clump callback
 *
 * \return Returns pointer to the specified camera if successful or NULL if
 * there is an error.
 *
 * \see RwCameraForAllSectorsInFrustum
 * \see RpClumpSetCallBack
 * \see RpClumpGetCallBack
 * \see RpWorldPluginAttach
 */
RwCamera *
RwCameraForAllClumpsInFrustum(RwCamera *camera,
                              void     *data)
{
    RwUInt32            clumpsInFrustumID;
    RpWorldCameraExt   *cameraExt;
    RpWorldSector     **frustumSectors;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwCameraForAllClumpsInFrustum"));
    RWASSERT(worldObjModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(RwCameraGetWorld(camera));

    /* Make the id unique */
    RPWORLDOBJGLOBAL(clumpsInFrustumID)++;
    clumpsInFrustumID = RPWORLDOBJGLOBAL(clumpsInFrustumID);

    /* Iterate over all of the rendered atomic sectors */
    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);
    frustumSectors = cameraExt->frustumSectors;
    for (i = cameraExt->position; i; i--, frustumSectors++)
    {
        RpTie            dummyTie;
        RwLinkList      *atomicList;
        RwLLLink        *cur, *end;
        RpTie           *tie;
        RpClump         *clump;
        RpWorldClumpExt *clumpExt;
        const RwSphere  *sphere;

        dummyTie.apAtom = (RpAtomic *)NULL;
        dummyTie.worldSector = (RpWorldSector *)NULL;

        /* High priority */
        atomicList = &(*frustumSectors)->collAtomicsInWorldSector;
        cur = rwLinkListGetFirstLLLink(atomicList);
        end = rwLinkListGetTerminator(atomicList);
        while (cur != end)
        {
            tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
            clump = tie->apAtom->clump;

            /* Add a dummy tie which we know won't get messed with */
            rwLinkListAddLLLink((RwLinkList *) cur,
                                &dummyTie.lAtomicInWorldSector);
            if (clump)
            {
                clumpExt = CLUMPEXTFROMCLUMP(clump);
                if (clumpExt->clumpsInFrustumID != clumpsInFrustumID)
                {
                    /* Test the atomic's bounding sphere */
                    sphere = RpAtomicGetWorldBoundingSphere(tie->apAtom);
                    if (RwCameraFrustumTestSphere(camera, sphere)
                        != rwSPHEREOUTSIDE)
                    {
                        clumpExt->clumpsInFrustumID = clumpsInFrustumID;
                        if (!clump->callback(clump, data))
                        {
                            /* Remove dummy tie and early out */
                            rwLinkListRemoveLLLink(
                                &dummyTie.lAtomicInWorldSector);
                            RWRETURN(camera);
                        }
                    }
                }
            }

            /* Query the dummy tie for the next pointer, and remove it */
            cur = rwLLLinkGetNext(&dummyTie.lAtomicInWorldSector);
            rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
        }
    }

    RWRETURN(camera);
}

/**
 * \ingroup rpworldrwcamera
 * \ref RwCameraForAllAtomicsInFrustum is used to determine which
 * atomics lie wholly or partially within the specified camera's view frustum
 * and then to execute the specified callback function on each atomic found.
 * The enumeration may be terminated at any time by returning NULL from
 * the atomic callback.
 *
 * \note The view frustum is only defined by the last render, so
 * this function must be called after a scene has been rendered.
 *
 * The results of calling function can be thought of as equivalent
 * to the following scheme:-
 *
 * -# \ref RwCameraForAllSectorsInFrustum to iterate through the current set
 *    of frustum sectors
 * -# \ref RpWorldSectorForAllAtomics to iterate over all atomics in a
 *    sector.
 * -# \ref RwCameraFrustumTestSphere to finally check whether the
 *    atomic is definitely inside the camera frustum.
 * -# Execute the callback function.
 *
 * The world plugin must be attached before using this function.
 *
 * \param camera    Pointer to the camera.
 * \param callback  Callback to be called with each atomic.
 * \param data      User data pointer to pass to each atomic callback.
 *
 * \return Returns pointer to the specified camera if successful or NULL if
 * there is an error.
 *
 * \see RwCameraForAllSectorsInFrustum
 * \see RpAtomicCallBack
 * \see RpWorldPluginAttach
 */
RwCamera *
RwCameraForAllAtomicsInFrustum(RwCamera *camera,
                               RpAtomicCallBack callback,
                              void     *data)
{
    RwUInt32            clumpsInFrustumID;
    RpWorldCameraExt   *cameraExt;
    RpWorldSector     **frustumSectors;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwCameraForAllAtomicsInFrustum"));
    RWASSERT(worldObjModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(RwCameraGetWorld(camera));

    /* Make the id unique */
    RPWORLDOBJGLOBAL(clumpsInFrustumID)++;
    clumpsInFrustumID = RPWORLDOBJGLOBAL(clumpsInFrustumID);

    /* Iterate over all of the rendered atomic sectors */
    cameraExt = WORLDCAMERAEXTFROMCAMERA(camera);
    frustumSectors = cameraExt->frustumSectors;
    for (i = cameraExt->position; i; i--, frustumSectors++)
    {
        RpTie            dummyTie;
        RwLinkList      *atomicList;
        RwLLLink        *cur, *end;
        RpTie           *tie;
        RpAtomic        *atom;
        const RwSphere  *sphere;

        dummyTie.apAtom = (RpAtomic *)NULL;
        dummyTie.worldSector = (RpWorldSector *)NULL;

        /* High priority */
        atomicList = &(*frustumSectors)->collAtomicsInWorldSector;
        cur = rwLinkListGetFirstLLLink(atomicList);
        end = rwLinkListGetTerminator(atomicList);
        while (cur != end)
        {
            tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
            atom = tie->apAtom;

            /* Add a dummy tie which we know won't get messed with */
            rwLinkListAddLLLink((RwLinkList *) cur, &dummyTie.lAtomicInWorldSector);
            if (atom)
            {
                /* Don't render objects multiple times within one frame - there's no point */
                if (atom->renderFrame != clumpsInFrustumID)
                {
                    /* Test the atomic's bounding sphere */
                    sphere = RpAtomicGetWorldBoundingSphere(tie->apAtom);
                    if (RwCameraFrustumTestSphere(camera, sphere) != rwSPHEREOUTSIDE)
                    {
                        atom->renderFrame = clumpsInFrustumID;
                        if (!callback(atom, data))
                        {
                            /* Remove dummy tie and early out */
                            rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
                            RWRETURN(camera);
                        }
                    }
                }
            }

            /* Query the dummy tie for the next pointer, and remove it */
            cur = rwLLLinkGetNext(&dummyTie.lAtomicInWorldSector);
            rwLinkListRemoveLLLink(&dummyTie.lAtomicInWorldSector);
        }
    }

    RWRETURN(camera);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldAddLight is used to add the specified light to the
 * given world. Lights must be added to a world if they are to illuminate
 * objects. Otherwise, they will not contribute to any rendering that
 * takes place.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldRemoveLight
 * \see RpWorldAddCamera
 * \see RpWorldRemoveCamera
 * \see RpWorldAddClump
 * \see RpWorldRemoveClump
 * \see RpWorldAddAtomic
 * \see RpWorldRemoveAtomic
 * \see RpWorldCreate
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldAddLight(RpWorld * world, RpLight * light)
{
    RpWorldLightExt    *lightExt;

    RWAPIFUNCTION(RWSTRING("RpWorldAddLight"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    lightExt = LIGHTEXTFROMLIGHT(light);

    RWASSERT(!lightExt->world);

    lightExt->world = world;

    if (rwObjectGetSubType(light) < rpLIGHTPOSITIONINGSTART)
    {
        /* Just add to the non poisition list */
        rwLinkListAddLLLink(&world->directionalLightList,
                            &light->inWorld);
    }
    else
    {
        RwFrame            *frame;

        frame = RpLightGetFrame(light);
        if (frame)
        {
            /* When a light is added to the world its matrix must be
             * updated to force ties to update.
             */
            RwFrameUpdateObjects(frame);
        }

        /* Need to make it partake in the ties mechanism - update its
         * frame.
         */
        rwLinkListAddLLLink(&world->lightList, &light->inWorld);
    }

    /* The object will be repositioned in the world */
    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldRemoveLight is used to remove the specified light from
 * the given world. Unlike \ref RpLightDestroy this function preserves the
 * light's definition, hence the light can be reused without creating it
 * again.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the light.
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to the world if successful or NULL if there is
 * an error.
 *
 * \see RpWorldAddLight
 * \see RpWorldRemoveCamera
 * \see RpWorldAddCamera
 * \see RpWorldRemoveClump
 * \see RpWorldAddClump
 * \see RpWorldRemoveAtomic
 * \see RpWorldAddAtomic
 * \see RpLightDestroy
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpWorldRemoveLight(RpWorld * world, RpLight * light)
{
    RpWorldLightExt    *lightExt;

    RWAPIFUNCTION(RWSTRING("RpWorldRemoveLight"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    lightExt = LIGHTEXTFROMLIGHT(light);
    RWASSERT(lightExt->world); /* Should be in a world */
    RWASSERT(world == lightExt->world); /* This one in fact */

    lightExt->world = (RpWorld *)NULL;

    LightDestroyTies(light);

    rwLinkListRemoveLLLink(&light->inWorld);

    RWRETURN(world);
}

/**
 * \ingroup rplight
 * \ref RpLightGetWorld is used to determine the world the specified
 * light belongs to, if any.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to the world which contains the light if successful
 * or NULL if there is an error or if the light does not belong to any world.
 *
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 */
RpWorld            *
RpLightGetWorld(const RpLight * light)
{
    const RpWorldLightExt *lightExt;

    RWAPIFUNCTION(RWSTRING("RpLightGetWorld"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    lightExt = LIGHTEXTFROMCONSTLIGHT(light);

    RWRETURN(lightExt->world);
}

/**
 * \ingroup rplight
 * \ref RpLightForAllWorldSectors is used to enumerate all the world sectors
 * that a light affects.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated.  However, RpLightForAllWorldSectors will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param callback  Callback to be called with each world sector.
 * \param data  User data pointer to be passed to the callback.
 *
 * \return Calls the specified callback with all the world sectors
 * that a light is used to illuminate.
 *
 * \see RpWorldSectorForAllLights
 * \see RpWorldPluginAttach
 */
RpLight            *
RpLightForAllWorldSectors(RpLight * light,
                          RpWorldSectorCallBack callback, void *data)
{
    RpLightType         type;

    RWAPIFUNCTION(RWSTRING("RpLightForAllWorldSectors"));

    RWASSERT(worldObjModule.numInstances);
    RWASSERT(light);
    RWASSERT(callback);

    /* Is this local or global? */
    type = RpLightGetType(light);

    if ((RwInt32) type < (RwInt32) rpLIGHTPOSITIONINGSTART)
    {
        /* This is a global light, enumerate all sectors in this world */
        RpWorld            *world = RpLightGetWorld(light);

        if (world)
        {
            if (!RpWorldForAllWorldSectors(world, callback, data))
            {
                RWRETURN((RpLight *)NULL);
            }
        }
    }
    else
    {
        /* This is a local light, it affects just the sectors it is in */
        RwLLLink           *cur, *end, *next;

        cur = rwLinkListGetFirstLLLink(&light->WorldSectorsInLight);
        end = rwLinkListGetTerminator(&light->WorldSectorsInLight);

        while (cur != end)
        {
            RpLightTie         *tie;

            next = rwLLLinkGetNext(cur);

            tie = rwLLLinkGetData(cur, RpLightTie, WorldSectorInLight);

            /* Do the callback */
            if (!callback(tie->sect, data))
            {
                /* Early out */
                RWRETURN(light);
            }

            /* Next */
            cur = next;
        }
    }

    RWRETURN(light);
}
