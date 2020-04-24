

/**
 * \ingroup rpworldsub
 * \page rpworldsuboverview RpWorld Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection worldsuboverview Overview
 *
 * This is the 'master' object for RenderWare Graphics' Retained Mode API.
 * It has a number of purposes:
 * \li Store and maintain World Sector (static model) data in a BSP tree
 * \li Insert Atomics, Clumps, Lights and Cameras within appropriate World Sectors
 * \li Track all these dynamic scene objects as they are moved during the lifetime of the scene
 * \li Manage the rendering of the entire scene
 *
 * World Sectors, and the BSP trees they live in, make up the basic structure of
 * the World object. When a supported dynamic object, such as an Atomic or Light,
 * is 'Added' to the World, the plugin calculates where new object is in the scene
 * and adds it to the appropriate World Sector(s).
 *
 * When rendering a World, using \ref RpWorldRender, the World plugin traverses the
 * BSP tree, rendering visible World Sectors as it does so. As each World Sector
 * is rendered, the plugin also renders any dynamic scene objects that have been
 * added to it too, resulting in the rendering of a complete scene.
 *
 *Further information is available in the World & Static Models chapter of the User Guide.
 *
*/

/**
 * \ingroup rpworldsector
 * \page rpworldsectoroverview RpWorldSector Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection worldsectoroverview Overview
 *
 * World Sectors contain static geometry - if we use a TV studio metaphor,
 * these represent the scenery and sets. These objects are stored using a
 * BSP-tree structure to speed up the rendering process. The World object passes
 * dynamic scene objects, such as Atomics, Cameras and Lights, to World Sectors
 * for storage. For this reason, World Sectors contain iterators which can be used
 * to access such dynamic objects. World Sector geometry data is not expected to
 * change. This opens up more optimization techniques, such as Potentially Visible
 * Sets (available by attaching the RpPVS plugin). The disadvantage is that the
 * geometry data must remain static.
 *
 *Further information is available in the World & Static Models chapter of the User Guide.
 *
 */

/* Define this to instance empty sectors.
 * This will allow the rest of the pipeline
 * to execute for space filling sectors.
 */
#define INSTEMPTYSECTORSx

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include <rwcore.h>

/* Module components */
#include "babinwor.h"
#include "baclump.h"
#include "bageomet.h"
#include "balight.h"
#include "bamateri.h"
#include "bamatlst.h"
#include "bamesh.h"
#include "bameshop.h"
#include "bapipew.h"
#include "basector.h"
#include "native.h"
#include "baworobj.h" /* How to do things with objects in the world */

#include "baworld.h"

/****************************************************************************
 Local Types
 */

typedef struct RpListedWorld RpListedWorld;
struct RpListedWorld
{
        RpWorld            *world;
        RwUInt32            memorySize;
        RwLLLink            nextEl;
};

typedef struct rpWorldGlobals rpWorldGlobals;
struct rpWorldGlobals
{
        RwFreeList         *worldListFreeList;
        RwLinkList          worldListHead;
};

typedef struct rpWorldFindSectorData rpWorldFindSectorData;
struct rpWorldFindSectorData
{
        const RpWorldSector *sector;
        RwBool              foundSector;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/* Accessing global memory */
#define RPWORLDGLOBAL(var)                      \
    (RWPLUGINOFFSET(rpWorldGlobals,             \
     RwEngineInstance,                          \
      worldModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    worldTKList =
    { sizeof(RpWorld),
      sizeof(RpWorld),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo worldModule;

/****************************************************************************
 Functions
 */

/******************************************************************************
 WorldFindSector

 Enumerate fn for each world sector in a world - test to see if it a particular
 one we look for.

 On entry   : sector to test
            : find data block
 On exit    : sector pointer, or NULL to early out
 */
static RpWorldSector *
WorldFindSector(RpWorldSector * sector, void *data)
{
    rpWorldFindSectorData *findSectorData = (rpWorldFindSectorData *) data;

    RWFUNCTION(RWSTRING("WorldFindSector"));
    RWASSERT(data);

    if (findSectorData->sector == sector)
    {
        /* It's this one, flag it and early out */
        findSectorData->foundSector = TRUE;
        RWRETURN((RpWorldSector *)NULL);
    }

    RWRETURN(sector);
}


/****************************************************************************
 WorldSectorRenderAtomics

 Render atomics in an atomic sector

 On entry   : Atomic sector
            : Function for rendering an atomic
            : Camera to use for frustum test
 On exit    :
 */

static void
WorldSectorRenderAtomics(RpWorldSector * worldSector)
{
    RwLLLink           *cur, *end;
    RpTie              *tie;
    RpAtomic           *atomic;

    RWFUNCTION(RWSTRING("WorldSectorRenderAtomics"));
    RWASSERT(worldSector);

    /* Coll objects first */
    cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
    end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);

    while (cur != end)
    {
        /* Render the sucker */
        tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
        atomic = tie->apAtom;

        RWASSERTISTYPE(atomic, rpATOMIC);

        if (rwObjectTestFlags(atomic, rpATOMICRENDER))
        {
            /* Don't render objects multiple times within one frame - there's no point */
            if (atomic->renderFrame != RWSRCGLOBAL(renderFrame))
            {
                const RwSphere     *atomicBoundingSphere;

                /* Frustum test it */
                atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(atomic);
                RWASSERT(atomicBoundingSphere);

                if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),
                                              atomicBoundingSphere) != rwSPHEREOUTSIDE)
                {
                    RpAtomicRender(atomic);
                }

                /* We don't really want to do the frustum test for
                 * the next sector too - it'll give the same answer
                 */
                atomic->renderFrame = RWSRCGLOBAL(renderFrame);
            }
        }

        /* Next */
        cur = rwLLLinkGetNext(cur);
    }

    /* All done */
    RWRETURNVOID();
}

/****************************************************************************
 WorldSectorRender

 Render world sector and the atomics within

 On entry   : World sector
            : User data
 On exit    : World sector pointer on success
 */

static RpWorldSector *
WorldSectorRender( RpWorldSector * sector,
                   void * pData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("WorldSectorRender"));
    RWASSERT(sector);

    if (RpWorldSectorRender(sector))
    {
        WorldSectorRenderAtomics(sector);
    }

    RWRETURN(sector);
}

static RpWorld     *
WorldBuildMeshAtomicSector(RpWorld * world,
                      RpBuildMesh * buildMesh,
                      RpWorldSector * worldSector,
                      RpMaterial ** matBase)
{
    RpMeshHeader       *mesh;
    RwInt32             i;
    RwTexture **textureArray;
    RwRaster **rasterArray;
    RxPipeline **pipelineArray;
    RwUInt16 numTex = 0;
    RwUInt16 numRas = 0;
    RwUInt16 numPip = 0;
    RwUInt32 numMaterials;

    RWFUNCTION(RWSTRING("WorldBuildMeshAtomicSector"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(worldSector);

    numMaterials = RpWorldGetNumMaterials(world);

    RWASSERTM( ( !((worldSector->numTriangles>0) && (0==numMaterials)) ),
               (RWSTRING("Must have materials assigned to triangles to unlock "
                         "the world")));
    if (numMaterials)
    {
        textureArray = RwMalloc(sizeof(RwTexture *) * numMaterials,
            rwID_TEXTURE | rwMEMHINTDUR_FUNCTION);
        rasterArray = RwMalloc(sizeof(RwRaster *) * numMaterials,
            rwMEMHINTDUR_FUNCTION | rwID_WORLDMODULE);
        pipelineArray = RwMalloc(sizeof(RxPipeline *) * numMaterials,
            rwMEMHINTDUR_FUNCTION | rwID_WORLDMODULE);

        for (i = 0; i < worldSector->numTriangles; i++)
        {
            RpTriangle  *tri;
            RpMaterial  *material;
            RwUInt16 texIndex, rasIndex, pipIndex;
            RxPipeline *pipeline = NULL;
            RwTexture *texture = NULL;
            RwRaster *raster = NULL;

            RWASSERT(matBase);

            tri = &worldSector->triangles[i];
            material = matBase[tri->matIndex];
            RWASSERTM(NULL != material,
                      (RWSTRING("All triangles in a world must have materials "
                                "assigned to unlock the world")));

            texture = RpMaterialGetTexture(material);
            for (texIndex = 0; texIndex < numTex; texIndex++)
            {
                if (textureArray[texIndex] == texture)
                {
                    /* Found texture break out the loop */
                    break;
                }
            }
            /* Was the texture found, if not add it */
            if (texIndex == numTex)
            {
                textureArray[texIndex] = texture;
                numTex++;
            }
            if (texture)
            {
                raster = RwTextureGetRaster(texture);
            }

            for (rasIndex = 0; rasIndex < numRas; rasIndex++)
            {
                if (rasterArray[rasIndex] == raster)
                {
                    /* Found raster break out the loop */
                    break;
                }
            }
            /* Was the raster found, if not add it */
            if (rasIndex == numRas)
            {
                rasterArray[rasIndex] = raster;
                numRas++;
            }

            RpMaterialGetPipeline(material, &pipeline);
            for (pipIndex = 0; pipIndex < numPip; pipIndex++)
            {
                if (pipelineArray[pipIndex] == pipeline)
                {
                    /* Found pipeline break out the loop */
                    break;
                }
            }
            /* Was the pipeline found, if not add it */
            if (pipIndex == numPip)
            {
                pipelineArray[pipIndex] = pipeline;
                numPip++;
            }

            _rpBuildMeshAddTriangle(buildMesh, material,
                                   tri->vertIndex[0], tri->vertIndex[1],
                                   tri->vertIndex[2], tri->matIndex,
                                   texIndex, rasIndex, pipIndex);
        }

        RwFree(textureArray);
        RwFree(rasterArray);
        RwFree(pipelineArray);
    }

    if (rpWORLDTRISTRIP & RpWorldGetFlags(world))
    {
        mesh = _rpMeshOptimise(buildMesh, rpMESHHEADERTRISTRIP);
    }
    else
    {
        mesh = _rpMeshOptimise(buildMesh, 0);
    }

    if (mesh)
    {
        worldSector->mesh = mesh;
    }
    else
    {
        /* Failed to generate the mesh - fail the world unlock */
        _rpBuildMeshDestroy(buildMesh);
        RWRETURN((RpWorld *)NULL);
    }

    RWRETURN(world);
}

/****************************************************************************
 _rpWorldSectorDeinstanceAll

 On entry       : Sector
 On exit        :
 */

void
_rpWorldSectorDeinstanceAll(RpSector * sector)
{
    RWFUNCTION(RWSTRING("_rpWorldSectorDeinstanceAll"));
    RWASSERT(sector);

    switch (sector->type)
    {
        case rwSECTORATOMIC:
            {
                RpWorldSector      *worldSector = (RpWorldSector *) sector;
                RwLLLink           *cur, *end;
                RpTie              *tie;
                RpLightTie         *lightTie;

                /* Blow away any instanced resources */
                if (worldSector->repEntry)
                {
                    RwResourcesFreeResEntry(worldSector->repEntry);
                }

                /* Undo all the ties - Coll objects first */
                cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
                end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);
                while (cur != end)
                {
                    /* Get rid of the tie */
                    tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
                    cur = rwLLLinkGetNext(cur);
                    _rpTieDestroy(tie);
                }

                /* And the lights */
                cur = rwLinkListGetFirstLLLink(&worldSector->lightsInWorldSector);
                end = rwLinkListGetTerminator(&worldSector->lightsInWorldSector);
                while (cur != end)
                {
                    lightTie = rwLLLinkGetData(cur, RpLightTie, lightInWorldSector);
                    cur = rwLLLinkGetNext(cur);
                    _rpLightTieDestroy(lightTie);
                }

                /* Call the destructor too - it is assumed that this object is going away */
                rwPluginRegistryDeInitObject(&sectorTKList, worldSector);

                /* Destroy the mesh */
                if (worldSector->mesh)
                {
                    _rpMeshDestroy((RpMeshHeader *) worldSector->mesh);
                    worldSector->mesh = (RpMeshHeader *)NULL;
                }

                break;
            }
        case rwSECTORBUILD:
            {
                /* We don't ever expect to encounter build sectors in this code,
                 * and they are never instanced if we do */

                break;
            }
        default:
            {
                RpPlaneSector      *planeSector = (RpPlaneSector *) sector;

                /* Its a plane */
                if (planeSector->leftSubTree)
                    _rpWorldSectorDeinstanceAll(planeSector->leftSubTree);
                if (planeSector->rightSubTree)
                    _rpWorldSectorDeinstanceAll(planeSector->rightSubTree);

                break;
            }
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpWorldSectorDestroyRecurse

 On entry       : Sector
 On exit        :
 */

void
_rpWorldSectorDestroyRecurse(RpSector * sector)
{
    RWFUNCTION(RWSTRING("_rpWorldSectorDestroyRecurse"));
    RWASSERT(sector);

    switch (sector->type)
    {
        case rwSECTORATOMIC:
            {
                RpWorldSector      *worldSector = (RpWorldSector *) sector;
                RwLLLink           *cur, *end;
                RpTie              *tie;
                RpLightTie         *lightTie;
                RwInt32             i;

                /* Blow away any instanced resources */
                if (worldSector->repEntry)
                {
                    RwResourcesFreeResEntry(worldSector->repEntry);
                }

                /* Undo all the ties - Coll objects first */
                cur = rwLinkListGetFirstLLLink(&worldSector->collAtomicsInWorldSector);
                end = rwLinkListGetTerminator(&worldSector->collAtomicsInWorldSector);
                while (cur != end)
                {
                    /* Get rid of the tie */
                    tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
                    cur = rwLLLinkGetNext(cur);
                    _rpTieDestroy(tie);
                }

                /* And the lights */
                cur = rwLinkListGetFirstLLLink(&worldSector->lightsInWorldSector);
                end = rwLinkListGetTerminator(&worldSector->lightsInWorldSector);
                while (cur != end)
                {
                    lightTie = rwLLLinkGetData(cur, RpLightTie, lightInWorldSector);
                    cur = rwLLLinkGetNext(cur);
                    _rpLightTieDestroy(lightTie);
                }

                /* Call the destructor */
                rwPluginRegistryDeInitObject(&sectorTKList, worldSector);

                /* Destroy the atomic sector */
                if (worldSector->vertices)
                {
                    RwFree(worldSector->vertices);
                    worldSector->vertices = (RwV3d *)NULL;
                }
                if (worldSector->normals)
                {
                    RwFree(worldSector->normals);
                    worldSector->normals = (RpVertexNormal *)NULL;
                }
                if (worldSector->preLitLum)
                {
                    RwFree(worldSector->preLitLum);
                    worldSector->preLitLum = (RwRGBA *)NULL;
                }
                if (worldSector->triangles)
                {
                    RwFree(worldSector->triangles);
                    worldSector->triangles = (RpTriangle *)NULL;
                }

                for (i = 0; i < rwMAXTEXTURECOORDS;i ++)
                {
                    if (worldSector->texCoords[i])
                    {
                        RwFree(worldSector->texCoords[i]);
                        worldSector->texCoords[i] = (RwTexCoords *)NULL;
                    }
                }

                /* Destroy the mesh */
                if (worldSector->mesh)
                {
                    _rpMeshDestroy((RpMeshHeader *) worldSector->mesh);
                    worldSector->mesh = (RpMeshHeader *)NULL;
                }

                RwFree(worldSector);

                break;
            }
        case rwSECTORBUILD:
            {
                /* We don't ever expect to encounter build sectors in this code */
                /* In the interests of modularity, put up with a memory leak if we do */
                RwFree(sector);

                break;
            }
        default:
            {
                RpPlaneSector      *planeSector = (RpPlaneSector *) sector;

                /* Its a plane */
                if (planeSector->leftSubTree)
                    _rpWorldSectorDestroyRecurse(planeSector->leftSubTree);
                planeSector->leftSubTree = (RpSector *)NULL;

                if (planeSector->rightSubTree)
                    _rpWorldSectorDestroyRecurse(planeSector->rightSubTree);
                planeSector->rightSubTree = (RpSector *)NULL;

                RwFree(planeSector);

                break;
            }
    }

    RWRETURNVOID();
}

#ifdef RWDEBUG

/******************************************************************************
 WorldRemoveLight

 Removes a light from the world

 On entry   : Light, user data (world)
 On exit    : Light pointer on success
 */

static RpLight     *
WorldRemoveLight(RpLight * light, void *data)
{
    RWFUNCTION(RWSTRING("WorldRemoveLight"));
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RpWorldRemoveLight((RpWorld *) data, light);

    /* I know we are returning a pointer to a destroyed light, but hey!!... */
    RWRETURN(light);
}

/******************************************************************************
 WorldRemoveClump

 Removes a clump from the world

 On entry   : Clump, user data (world)
 On exit    : Clump pointer on success
 */

static RpClump     *
WorldRemoveClump(RpClump * clump, void *data)
{
    RWFUNCTION(RWSTRING("WorldRemoveClump"));
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    RpWorldRemoveClump((RpWorld *) data, clump);

    /* I know we are returning a pointer to a destroyed clump, but hey!!... */
    RWRETURN(clump);
}

#endif /* RWDEBUG */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             Opening / Closing

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/******************************************************************************
 WorldClose

 Closes the world module.  Destroys the rendering pipeline

 On entry   : None
 On exit    : TRUE on success
 */

static void        *
WorldClose( void *instance,
            RwInt32 offset __RWUNUSED__,
            RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("WorldClose"));

    if (RPWORLDGLOBAL(worldListFreeList))
    {
        RwFreeListDestroy(RPWORLDGLOBAL(worldListFreeList));
        RPWORLDGLOBAL(worldListFreeList) = (RwFreeList *)NULL;
    }

    _rpWorldPipelineClose();

    /* One less module instance */
    worldModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

static RwFreeList _rpWorldListFreeList;

/******************************************************************************
 WorldOpen

 Opens the world module.  Creates the rendering pipeline

 On entry   : None
 On exit    : TRUE on success
 */

static void        *
WorldOpen( void *instance,
           RwInt32 offset,
           RwInt32 size __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("WorldOpen"));

    /* Grab the global data offset (same for all instances) */
    worldModule.globalsOffset = offset;

    if (!_rpWorldPipelineOpen())
    {
        RWRETURN(NULL);
    }

    /* Can't see many of these being allocated */
    RPWORLDGLOBAL(worldListFreeList) = RwFreeListCreateAndPreallocateSpace(sizeof(RpListedWorld), 8,
        sizeof(RwUInt32),1,&_rpWorldListFreeList, rwMEMHINTDUR_GLOBAL | rwID_WORLD);
    if (!RPWORLDGLOBAL(worldListFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    rwLinkListInitialize(&RPWORLDGLOBAL(worldListHead));

    /* One more module instance */
    worldModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 __rpSectorDefaultRenderCallBack

 Executes the object pipeline on the atomic.

 On entry       : RpWorldSector
 On exit        : sector pointer if OK
 */

RpWorldSector *
_rpSectorDefaultRenderCallBack(RpWorldSector *sector)
{
    RxPipeline  *pipeline = (RxPipeline *)NULL;

    RWFUNCTION(RWSTRING("_rpSectorDefaultRenderCallBack"));
    RWASSERT(sector);

    /* Don't bother rendering empty worldsectors */
    if (sector->numTriangles <= 0) RWRETURN(sector);

    if (sector->pipeline)
    {
        pipeline = sector->pipeline;
    }
    else
    {
        if (((RpWorld *) RWSRCGLOBAL(curWorld))->pipeline)
        {
            pipeline = ((RpWorld *) RWSRCGLOBAL(curWorld))->pipeline;
        }
        else
        {
            pipeline = RXPIPELINEGLOBAL(currentWorldSectorPipeline);
        }
    }

    if (RxPipelineExecute(pipeline, (void *) sector, TRUE))
    {
        RWRETURN(sector);
    }

    RWRETURN((RpWorldSector *)NULL);
}

/******************************************************************************
 _rpWorldForAllGlobalLights

 Iterates over all global lights

 On entry   : Callback for each light, user data pointer
 On exit    : TRUE on success
 */

RwBool
_rpWorldForAllGlobalLights(RpLightCallBack callBack, void *pData)
{
    RpWorld            *world = (RpWorld *) RWSRCGLOBAL(curWorld);
    RwLLLink           *cur, *end, *next;

    RWFUNCTION(RWSTRING("_rpWorldForAllGlobalLights"));
    RWASSERT(world);
    RWASSERT(callBack);

    /* Directional light it */
    cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
    end = rwLinkListGetTerminator(&world->directionalLightList);
    while (cur != end)
    {
        RpLight *light = rwLLLinkGetData(cur, RpLight, inWorld);

        next = rwLLLinkGetNext(cur);

        /* NB light may actually be a dummyTie from a enclosing ForAll */
        if (light)
            if (!callBack(light, pData))
            {
                /* Early out */
                RWRETURN(TRUE);
            }

        /* Next */
        cur = next;
    }

    RWRETURN(TRUE);
}

/******************************************************************************
 _rpWorldSectorForAllLocalLights

 Iterates over all lights which have an effect in a sector.

 On entry   : Sector to iterate lights for, callback for each light, user data pointer
 On exit    : World sector pointer on success
 */

RpWorldSector      *
_rpWorldSectorForAllLocalLights(RpWorldSector * sector,
                               RpLightCallBack callBack, void *pData)
{
    RwLLLink           *cur, *end, *next;

    RWFUNCTION(RWSTRING("_rpWorldSectorForAllLocalLights"));
    RWASSERT(sector);
    RWASSERT(callBack);

    /* Lights in the sector */
    cur = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
    end = rwLinkListGetTerminator(&sector->lightsInWorldSector);

    while (cur != end)
    {
        RpLightTie         *lightTie =

            rwLLLinkGetData(cur, RpLightTie, lightInWorldSector);

        next = rwLLLinkGetNext(cur);

        /* NB lightTie may actually be a dummyTie from a enclosing ForAll */
        if (lightTie->light)
            if (!callBack(lightTie->light, pData))
            {
                /* Early out */
                RWRETURN(sector);
            }

        /* Next */
        cur = next;
    }

    RWRETURN(sector);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Finding the world size

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/******************************************************************************
 _rpWorldFindBBox

 Finds a BBox which surrounds the world - NOTE THAT THIS TRAVERSES THE
 TREE in order to find the size. It also uses the tight sector bounding boxes
 which are generated when the world is created and streamed out. The
 loose bounding boxes should be generated after this.

 On entry   : World
            : BBox (OUT)
 On exit    : TRUE on success
 */

RwBool
_rpWorldFindBBox(RpWorld * world, RwBBox * boundingBox)
{
    RpSector           *sector;
    RpSector           *sectorStack[rpWORLDMAXBSPDEPTH];
    RwInt32             stack = 0;
    RwBool              doneFirst = FALSE;

    RWFUNCTION(RWSTRING("_rpWorldFindBBox"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(boundingBox);

    /* Start at the top */
    sector = world->rootSector;
    do
    {
        if (sector->type < 0)
        {
            /* Its an atomic sector */
            RpWorldSector      *worldSector = (RpWorldSector *) sector;

            if ((!doneFirst))
            {
                RwBBoxInitialize(boundingBox,
                                 &worldSector->tightBoundingBox.inf);
                doneFirst = TRUE;
            }
            else
            {
                RwBBoxAddPoint(boundingBox,
                               &worldSector->tightBoundingBox.inf);
            }

            RwBBoxAddPoint(boundingBox, &worldSector->tightBoundingBox.sup);

            /* Pop from the stack */
            sector = sectorStack[stack--];
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *planeSector = (RpPlaneSector *) sector;

            sector = planeSector->leftSubTree;
            sectorStack[++stack] = planeSector->rightSubTree;
        }
    }
    while (stack >= 0);

    RWRETURN(TRUE);
}


/******************************************************************************
 _rpWorldSetupSectorBoundingBoxes

 Traverses the BSP tree, and sets up the world sector bounding boxes based
 on the BSP planes. These boxes are used for frustum testing as they may
 contain sector geometry _and_ atomics.

 They are distinct from the tight bounding boxes which are generated when the
 world is created and are streamed in/out. These can only be used for frustum
 testing sector geometry as atomics may lie outside.

 Note that the world bounding box MUST be set up prior to this.

 On entry   : World
            : BBox (OUT)
 On exit    : TRUE on success
 */
RpWorld *
_rpWorldSetupSectorBoundingBoxes(RpWorld *world)
{
    RwInt32             nStack;
    RpSector           *sector, *sectorStack[rpWORLDMAXBSPDEPTH];
    RwBBox              bbox, bboxStack[rpWORLDMAXBSPDEPTH];

    RWFUNCTION(RWSTRING("_rpWorldSetupSectorBoundingBoxes"));
    RWASSERT(world);

    /* Start at the top */
    nStack = 0;
    sector = world->rootSector;
    bbox = world->boundingBox;

    /* Recurse down the tree */
    while (nStack >= 0)
    {
        if (sector->type < 0)
        {
            RpWorldSector   *worldSector = (RpWorldSector *) sector;

            /* Copy current bounding box to sector */
            worldSector->boundingBox = bbox;

            /* Pop stack */
            sector = sectorStack[nStack];
            bbox = bboxStack[nStack];
            nStack--;
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *plane = (RpPlaneSector *) sector;

            /* Push right onto stack */
            nStack++;
            sectorStack[nStack] = plane->rightSubTree;
            bboxStack[nStack] = bbox;
            SETCOORD(bboxStack[nStack].inf, plane->type, plane->rightValue);

            /* Go left */
            SETCOORD(bbox.sup, plane->type, plane->leftValue);
            sector = plane->leftSubTree;
        }
    }

    /* All done */
    RWRETURN(world);
}

/******************************************************************************
 _rpWorldRegisterWorld

 Add a world to the list of worlds

 On entry   : world to add
            : amount of memory allocated for world bit (in one allocation)
 On exit    : None
 */
void
_rpWorldRegisterWorld(RpWorld * world, RwUInt32 memorySize)
{
    RpListedWorld      *listedWorld;

    RWFUNCTION(RWSTRING("_rpWorldRegisterWorld"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    listedWorld = (RpListedWorld *)
        RwFreeListAlloc(RPWORLDGLOBAL(worldListFreeList),
                        rwMEMHINTDUR_GLOBAL | rwID_WORLDMODULE);
    if (listedWorld)
    {
        listedWorld->world = world;
        listedWorld->memorySize = memorySize;
        rwLinkListAddLLLink(&RPWORLDGLOBAL(worldListHead),
                            &listedWorld->nextEl);
    }

    RWRETURNVOID();
}

/******************************************************************************
 _rpWorldUnregisterWorld

 Remove a world from the list

 On entry   : world to remove
 On exit    : None
 */
void
_rpWorldUnregisterWorld(RpWorld * world)
{
    RwLLLink           *cur, *end;

    RWFUNCTION(RWSTRING("_rpWorldUnregisterWorld"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    cur = rwLinkListGetFirstLLLink(&RPWORLDGLOBAL(worldListHead));
    end = rwLinkListGetTerminator(&RPWORLDGLOBAL(worldListHead));

    while (cur != end)
    {
        RpListedWorld *listedWorld =
            rwLLLinkGetData(cur,RpListedWorld,nextEl);

        if (listedWorld->world == world)
        {
            /* Remove it and get outta here */
            rwLinkListRemoveLLLink(&listedWorld->nextEl);

            RwFreeListFree(RPWORLDGLOBAL(worldListFreeList),
                           listedWorld);

            RWRETURNVOID();
        }

        cur = rwLLLinkGetNext(cur);
    }

    RWRETURNVOID();
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                    Locking/Releasing world

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/****************************************************************************
 RpWorldLock

 Locking a world blows away all the meshes so a world can be destroyed or
 modified

 On entry   : World
 On exit    : World pointer on success
 */

/**
 * \ingroup rpworldsub
 * \ref RpWorldLock is used to lock the specified world so an application
 * can access it's internal data structures. After the data has been modified
 * the world should be unlocked so that the meshes can be rebuilt. When the
 * world is locked the present meshes are destroyed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns pointer to the world if successful or
 * NULL if there is an error.
 *
 * \see RpWorldUnlock
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldLock(RpWorld * world)
{
    RpSector           *sector;
    RpSector           *sectorStack[rpWORLDMAXBSPDEPTH];
    RwInt32             stack = 0;

    RWAPIFUNCTION(RWSTRING("RpWorldLock"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    /* Start at the top */
    sector = world->rootSector;

    if (sector == NULL)
        RWRETURN((RpWorld *) NULL);

    do
    {
        RWASSERT(sector);

        if (sector->type < 0)
        {
            RpWorldSector      *worldSector = (RpWorldSector *) sector;

            /* Its an atomic sector */
            if (worldSector->mesh)
            {
                _rpMeshDestroy((RpMeshHeader *) worldSector->mesh);
                worldSector->mesh = (RpMeshHeader *)NULL;
            }

            sector = sectorStack[stack--];
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *planeSector = (RpPlaneSector *) sector;

            /* Left then right */
            sector = planeSector->leftSubTree;
            sectorStack[++stack] = planeSector->rightSubTree;
        }
    }
    while (stack >= 0);

    RWRETURN(world);
}


/****************************************************************************
 RpWorldUnlock

 Unlocking a world builds the meshes for the atomic sector (used for rendering)

 On entry   : World
 On exit    : World pointer on success
 */

/**
 * \ingroup rpworldsub
 * \ref RpWorldUnlock is used to unlock the specified world. This function
 * releases the world's data structures after modification so that the
 * world's meshes can be rebuilt.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns pointer to the world if successful or
 * NULL if there is an error.
 *
 * \see RpWorldLock
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldUnlock(RpWorld * world)
{
    RpSector           *sector;
    RpSector           *sectorStack[rpWORLDMAXBSPDEPTH];
    RwInt32             stack = 0;

    RWAPIFUNCTION(RWSTRING("RpWorldUnlock"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    /* Start at the top */
    sector = world->rootSector;

    if (sector == NULL)
        RWRETURN((RpWorld *) NULL);

    do
    {
        if (sector->type < 0)
        {
            RpWorldSector      *worldSector = (RpWorldSector *) sector;
            RpMaterial        **matBase =
                &((world->matList.materials)[worldSector->matListWindowBase]);

            /* Its an atomic sector */
            if (!worldSector->mesh)
            {
                RpBuildMesh        *buildMesh;

                /* Create a mesh for this sector
                 * (provide a useful hint as to size) */
                buildMesh = _rpBuildMeshCreate(worldSector->numTriangles);

                if (NULL != buildMesh)
                {
                    world =
                        WorldBuildMeshAtomicSector(world, buildMesh,
                                              worldSector, matBase);

                    if (NULL == world)
                        RWRETURN((RpWorld *)NULL);
                }
                else
                {
                    /* Failed to generate the mesh - fail the world unlock */
                    RWRETURN((RpWorld *)NULL);
                }
            }

            /* Stack back */
            sector = sectorStack[stack--];
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *planeSector = (RpPlaneSector *) sector;

            /* Left then right */
            sector = planeSector->leftSubTree;
            sectorStack[++stack] = planeSector->rightSubTree;
        }
    }
    while (stack >= 0);

    RWRETURN(world);
}



/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Setting/Getting world parameters

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetWorld is used to retrieve the specified world
 * sector's parent world.  Use this function to determine the world that
 * contains the world sector.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns pointer to the world if successful or
 * NULL if there is an error.
 *
 * \see RpWorldSectorGetBBox
 * \see RpWorldSectorForAllMeshes
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldSectorGetWorld(const RpWorldSector * sector)
{
    RwLLLink           *cur, *end;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetWorld"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(sector);

    cur = rwLinkListGetFirstLLLink(&RPWORLDGLOBAL(worldListHead));
    end = rwLinkListGetTerminator(&RPWORLDGLOBAL(worldListHead));

    while (cur != end)
    {
        const RpListedWorld *listedWorld =
            rwLLLinkGetData(cur, RpListedWorld, nextEl);

        if (rwObjectTestPrivateFlags(listedWorld->world, rpWORLDSINGLEMALLOC))
        {
            /* We have a single malloced world,
             * so check the pointer ranges */
            /* First test against the beginning of the block */
            if ((const void *) sector >= (const void *) listedWorld->world)
            {
                /* Test against the end of the world */
                const void *endOfWorld =
                    (const void *) ((const RwUInt8 *)
                                    listedWorld->world +
                                    listedWorld->memorySize);

                if ((const void *) sector < endOfWorld)
                {
                    /* It's in range, this is the world */
                    RWRETURN(listedWorld->world);
                }
            }
            /* Not this world */
        }
        else
        {
            /* Traverse the tree to find the sector (hopefully) */
            rpWorldFindSectorData findSectorData;

            findSectorData.sector = sector;
            findSectorData.foundSector = FALSE;
            RpWorldForAllWorldSectors(listedWorld->world,
                                      WorldFindSector,
                                      &findSectorData);

            if (findSectorData.foundSector)
            {
                /* It's this one */
                RWRETURN(listedWorld->world);
            }
        }

        cur = rwLLLinkGetNext(cur);
    }

    /* Not found the world */
    RWRETURN((RpWorld *)NULL);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
/**
 * \ingroup rpworldsub
 * \ref RpWorldGetBBox is used to retrieve the specified world's bounding
 * box. Use this function to determine the actual bounding box of the world
 * which will differ from that specified at creation time. For efficiency
 * reasons the bounding box is translated such that all coordinates within it
 * are positive. The world's offset, describing the extent of the translation,
 * can be used to bring objects defined relative to the original bounding box
 * into the bounding box actually used.
 *
 * Note the world offset is fixed at the time the world is created and
 * describes the translation required to move the world's bounding box back to
 * its original position.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns pointer to a RwBBox value equal to the world's bounding box
 * if successful or NULL if there is an error.
 *
 * \see RpWorldSectorGetBBox
 * \see RpWorldCreate
 * \see RpWorldStreamRead
 * \see RpWorldPluginAttach
 *
 */
const RwBBox *
RpWorldGetBBox(const RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetBBox"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(RpWorldGetBBoxMacro(world));
}

const RwV3d *
_rpWorldGetOrigin(const RpWorld *world)
{
    RWFUNCTION(RWSTRING("_rpWorldGetOrigin"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(_rpWorldGetOriginMacro(world));
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             Materials

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetNumMaterials is used to retrieve the number of different
 * materials in use by all static surfaces in the specified world. Materials
 * which are not referenced by static surfaces are not counted.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns a RwInt32 value equal to the number of materials if
 * successful or -1 if there is an error.
 *
 * \see RpWorldGetMaterial
 * \see RpWorldForAllMaterials
 * \see RpWorldStreamRead
 * \see RpWorldStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldGetNumMaterials(const RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetNumMaterials"));

    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWASSERT(0 <= RpWorldGetNumMaterialsMacro(world));

    RWRETURN(RpWorldGetNumMaterialsMacro(world));
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetMaterial is used to retrieve the material with the given
 * index from the specified world's material list. The material list only
 * includes materials used by static surfaces in the world.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param matNum  A RwInt32 value equal to the index of the material.
 *
 * \return Returns pointer to the material if successful or NULL if there is an
 * error or if the index is out of range.
 *
 * \see RpWorldGetNumMaterials
 * \see RpWorldForAllMaterials
 * \see RpWorldStreamRead
 * \see RpWorldStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RpMaterial *
RpWorldGetMaterial(const RpWorld *world, RwInt32 matNum)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetMaterial"));

    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWASSERT(world->matList.materials);
    RWASSERT((matNum >= 0) && (matNum < world->matList.numMaterials));

    RWRETURN(RpWorldGetMaterialMacro(world, matNum));
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetNumClumps is used to retrieve the number of clumps in
 * the specified world.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the clumps.
 *
 * \return Returns a RwInt32 value equal to the number of clumps if successful
 * or -1 if there is an error.
 *
 * \see RpWorldForAllClumps
 * \see RpWorldAddClump
 * \see RpWorldCreate
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldGetNumClumps(RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetNumClumps"));

    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWASSERT(0 <= RpWorldGetNumClumpsMacro(world));

    RWRETURN(RpWorldGetNumClumpsMacro(world));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                Rendering

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetRenderOrder is used to set the order in which
 * world sectors in the specified world are rendered with respect to the
 * camera's frustum, which can be either front to back or back to front.
 * On rendering a world sector, the contained atomics will not be sorted for
 * rendering.  If atomics contains transparencies, it is best to render them
 * separately..
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param renderOrder  Value indicating the rendering order to be used.  Supported values are:
 *          \li rpWORLDRENDERFRONT2BACK - World sectors will be rendered front to back.
 *          \li rpWORLDRENDERBACK2FRONT - World sectors will be rendered back to front.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldGetRenderOrder
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 *
 */
RpWorld *
RpWorldSetRenderOrder(RpWorld *world, RpWorldRenderOrder renderOrder)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSetRenderOrder"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(RpWorldSetRenderOrderMacro(world, renderOrder));
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetRenderOrder is used to retrieve the order in which
 * world sectors in the specified world are rendered with respect to the
 * camera's frustum, which can be either front to back or back to front.
 * Back to front produces the correct results with respect to alpha blending
 * at low memory cost but can lead to higher frame-buffer bandwidth requirements.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns value indicating the rendering order if successful or
 * rpWORLDRENDERNARENDERORDER if there is an error:
 *
 * \li rpWORLDRENDERNARENDERORDER - The world sector rendering order is not
 *                                  a supported type.
 * \li rpWORLDRENDERFRONT2BACK - World sectors will be rendered front to back.
 * \li rpWORLDRENDERBACK2FRONT - World sectors will be rendered back to front.
 *
 * \see RpWorldSetRenderOrder
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 *
 */
RpWorldRenderOrder
RpWorldGetRenderOrder(const RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetRenderOrder"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(RpWorldGetRenderOrderMacro(world));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpworldsub
 * \ref RpWorldRender is used to render all objects (static or otherwise)
 * in the specified world to the current camera's image raster.  Only objects
 * that have been added to the world will be rendered.
 *
 * \ref RpWorldRender is used to render both world sectors and atomics, but only
 * those that fall inside the current camera's view frustum will be rendered.
 *
 * This function should only be called between
 * \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate to ensure that
 * any rendering that takes place is directed towards an image raster
 * connected to a camera.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the camera.
 *
 * \return Returns pointer to the world if successful or
 * NULL if there is an error.
 *
 * \see RpWorldSetRenderOrder
 * \see RpWorldSetSectorRenderCallBack
 * \see RpWorldGetRenderOrder
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RpWorldSectorRender
 * \see RpClumpRender
 * \see RpAtomicRender
 * \see RpWorldAddAtomic
 * \see RpWorldAddClump
 * \see RwCameraShowRaster
 * \see RpWorldPluginAttach
 *
 */
RpWorld *
RpWorldRender(RpWorld *world)
{
    RwCamera         *camera;

    RWAPIFUNCTION(RWSTRING("RpWorldRender"));

    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    /* Render the sectors and contents */
    camera = (RwCamera *)RWSRCGLOBAL(curCamera);
    _rwCameraAssertWorld(camera, world);

    RwCameraForAllSectorsInFrustum(camera, WorldSectorRender, world);

    /* All done... */
    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldDestroy is used to destroy the specified world. Only those
 * parts of the world contained in static objects are destroyed.
 * Other objects such as clumps, cameras and lights are not destroyed and it is
 * recommended that they are removed from the world before calling this
 * function. If necessary, non-static objects can then be destroyed using the
 * appropriate functions.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpWorldCreate
 * \see RpWorldRemoveCamera
 * \see RpWorldRemoveLight
 * \see RpWorldRemoveClump
 * \see RpWorldRemoveAtomic
 * \see RpWorldStreamRead
 * \see RpWorldStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpWorldDestroy(RpWorld * world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldDestroy"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

#ifdef RWDEBUG
    /* Make sure all the objects are removed (we can do lights and clumps) */
    /* First the directional and ambient lights */
    if (!rwLinkListEmpty(&world->directionalLightList) ||
        !rwLinkListEmpty(&world->lightList))
    {
        RWMESSAGE((RWSTRING("Lights still exist in the world when world was destroyed")));
        RpWorldForAllLights(world, WorldRemoveLight, world);
    }
    if (!rwLinkListEmpty(&world->clumpList))
    {
        RWMESSAGE((RWSTRING("Clumps still exist in the world when world was destroyed")));
        RpWorldForAllClumps(world, WorldRemoveClump, world);
    }
#endif /* RWDEBUG */

    /* Remove it from the list of worlds */
    _rpWorldUnregisterWorld(world);

    /* Lock it so that there is no device dependent information */
    _rpWorldLock(world);

    /* Material list */
    _rpMaterialListDeinitialize(&world->matList);

    /* Destroy the BSP tree */
    if (rwObjectTestPrivateFlags(world, rpWORLDSINGLEMALLOC))
    {
        /* Its all in one big chunk */
        if (world->rootSector)
            _rpWorldSectorDeinstanceAll(world->rootSector);

        /* Call the destructor */
        rwPluginRegistryDeInitObject(&worldTKList, world);
        RwFree(world);
    }
    else
    {
        /* This calls destructors on sectors too */
        if (world->rootSector)
            _rpWorldSectorDestroyRecurse(world->rootSector);

        /* And finally the destructor on the world */
        rwPluginRegistryDeInitObject(&worldTKList, world);
        RwFree(world);
    }

    /* Done */
    RWRETURN(TRUE);
}


/**
 * \ingroup rpworldsub
 * \ref RpWorldSetSectorRenderCallBack is used to define the world
 * sector render callback function for the specified world. The callback
 * function is executed from \ref RpWorldSectorRender (and indirectly from
 * \ref RpWorldRender) when a world sector lies inside the current camera's view
 * frustum, giving the application the opportunity to alter the way in which
 * static objects in the world sector are rendered. The default callback
 * function will invoke the world rendering pipeline. The default function can
 * be reinstated by specifying NULL in the callback function parameter.
 *
 * The format of the callback function is:
 *
 * RpWorldSector * (*RpWorldSectorCallBackRender)(RpWorldSector *sect)
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param fpCallBack  Pointer to the render callback function.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldGetSectorRenderCallBack
 * \see RpWorldSectorRender
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldSetSectorRenderCallBack(RpWorld * world,
                               RpWorldSectorCallBackRender fpCallBack)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSetSectorRenderCallBack"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    if (NULL == fpCallBack)
    {
        fpCallBack = _rpSectorDefaultRenderCallBack;
    }

    world->renderCallBack = fpCallBack;

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetSectorRenderCallBack is used to retrieve the world
 * sector render callback function of the specified world. The render callback
 * is the function that is executed from \ref RpWorldSectorRender (and
 * indirectly from \ref RpWorldRender) when a world sector lies inside the
 * current camera's view frustum, giving the application the opportunity to
 * alter the way in which static objects in the world sector are rendered.
 *
 * The format of the callback function is:
 *
 * RpWorldSector * (*RpWorldSectorCallBackRender)(RpWorldSector *sect)
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns pointer to the callback function if successful or NULL
 * if there is an error.
 *
 * \see RpWorldSetSectorRenderCallBack
 * \see RpWorldSectorRender
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 *
 */
RpWorldSectorCallBackRender
RpWorldGetSectorRenderCallBack(const RpWorld * world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetSectorRenderCallBack"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(world->renderCallBack);
}
/******************************************************************************
 *
 * Dummy worlds are useful for rendering clumps directly
 *
 */

/**
 * \ingroup rpworldsub
 * \ref RpWorldCreate is used to create a new world which has the specified
 * bounding box. The newly created world contains no objects (static or
 * otherwise) and one world sector covering the whole of the bounding box.
 *
 * The world plugin must be attached before using this function.
 *
 * \param boundingBox  Pointer to a RwBBox value equal to the bounding box.
 *
 * \return Returns pointer to the world if successful or NULL if
 * there is an error.
 *
 * \see RpWorldDestroy
 * \see RpWorldGetBBox
 * \see RpWorldSectorGetBBox
 * \see RpWorldStreamRead
 * \see RpWorldRender
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldCreate(RwBBox * boundingBox)
{
    RpWorld            *world;
    RpWorldSector      *worldSector;

    RWAPIFUNCTION(RWSTRING("RpWorldCreate"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(boundingBox);
    RWASSERT(boundingBox->sup.x >= boundingBox->inf.x);
    RWASSERT(boundingBox->sup.y >= boundingBox->inf.y);
    RWASSERT(boundingBox->sup.z >= boundingBox->inf.z);

    world = (RpWorld *) RwMalloc(worldTKList.sizeOfStruct,
        rwID_WORLD | rwMEMHINTDUR_EVENT);
    if (!world)
    {
        RWERROR((E_RW_NOMEM, worldTKList.sizeOfStruct));
        RWRETURN((RpWorld *)NULL);
    }

    /* Setup the world type */
    rwObjectInitialize(world, rpWORLD, 0);
    _rpMaterialListInitialize(&world->matList);

    /* Set the default rendering order */
    RpWorldSetRenderOrder(world, rpWORLDRENDERBACK2FRONT);

    /* Don't need to set any flags, since we haven't got any thing
     * (geometry) to brag about - but zero them anyway
     */
    RpWorldSetFlags(world, 0);

    /* Set the atomic sector */
    worldSector = (RpWorldSector *) RwMalloc(sectorTKList.sizeOfStruct,
                                      rwID_WORLD | rwMEMHINTDUR_EVENT);

    if (!worldSector)
    {
        RWERROR((E_RW_NOMEM,sizeof(sectorTKList.sizeOfStruct)));
        RwFree(world);
        RWRETURN((RpWorld *)NULL);
    }

    /* !!!!!!!!!!!!!!!!!!! Create Atomic sector !!!!!!!!!!!!!!!!!!!!!!!!!! */

    /* Mark it as atomic */
    worldSector->type = rwSECTORATOMIC;

    /* Not instanced !!! */
    worldSector->repEntry = (RwResEntry *)NULL;
    worldSector->mesh = (RpMeshHeader *)NULL;

    /* Set so it contains nothing */
    rwLinkListInitialize(&worldSector->collAtomicsInWorldSector);
    rwLinkListInitialize(&worldSector->lightsInWorldSector);

    /* Copy over the information */
    worldSector->numVertices = 0;
    worldSector->numTriangles = 0;
    worldSector->vertices = (RwV3d *)NULL;
    worldSector->triangles = (RpTriangle *)NULL;
    worldSector->normals = (RpVertexNormal *)NULL;

    /* Set up texture coords */
    memset(worldSector->texCoords, 0, sizeof(RwTexCoords *) * rwMAXTEXTURECOORDS);

    worldSector->preLitLum = (RwRGBA *)NULL;
    worldSector->boundingBox.inf = boundingBox->inf;
    worldSector->boundingBox.sup = boundingBox->sup;
    worldSector->tightBoundingBox.inf = boundingBox->inf;
    worldSector->tightBoundingBox.sup = boundingBox->sup;

    /* Use the default world sector object pipeline */
    worldSector->pipeline = (RxPipeline *)NULL;

    /* Set the size of the atomic sector */
    world->rootSector = (RpSector *) worldSector;

    /* !!!!!!!!!!!!!!!!!!! Atomic sector Created !!!!!!!!!!!!!!!!!!!!!!!!!! */

    /* Set the number of texture coordinate sets in the world to zero */
    world->numTexCoordSets = 0;

    /* Set the world's offset to zero */
    world->worldOrigin.x =
        world->worldOrigin.y =
        world->worldOrigin.z = (RwReal)0.0f;

    /* Set the world's bounding boxes */
    world->boundingBox.inf = boundingBox->inf;
    world->boundingBox.sup = boundingBox->sup;

    /* There are no atomics or clumps in the world */
    rwLinkListInitialize(&world->clumpList);
    world->numClumpsInWorld = 0;
    world->currentClumpLink = rwLinkListGetTerminator(&world->clumpList);

    /* Initialize (non directional) lights linked list */
    rwLinkListInitialize(&world->lightList);

    /* Directional lights in world */
    rwLinkListInitialize(&world->directionalLightList);

    /* Reset its instance callback */
    RpWorldSetSectorRenderCallBack(world,
                                   (RpWorldSectorCallBackRender)NULL);

    /* use the default world sector object pipeline */
    world->pipeline = (RxPipeline *)NULL;

    /* Add it to the list of worlds */
    _rpWorldRegisterWorld(world, worldTKList.sizeOfStruct);

    /* Call the constructors */
    rwPluginRegistryInitObject(&worldTKList, world);
    rwPluginRegistryInitObject(&sectorTKList, worldSector);

    /* Give the device a look in */
    if (!_rpWorldUnlock(world))
    {
        /* Failed to unlock, fail the world creation */
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    /* Done */
    RWRETURN(world);
}

/* native.h will overload this for PS2 which does crezzee thiangs */
#if (!defined(_rpWorldSectorNativeFreeResEntry))
#define _rpWorldSectorNativeFreeResEntry(_sctr) \
    ((_sctr->repEntry)?(RwResourcesFreeResEntry(_sctr->repEntry)):(TRUE))
#endif /* (!defined(_rpWorldSectorNativeFreeResEntry)) */

static RpWorldSector *
WorldSectorInstance( RpWorldSector *sector,
                     void *data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("WorldSectorInstance"));

    /* Destroy the old resource entry if one exists.
     * (this macro is overloaded for PS2) */
    _rpWorldSectorNativeFreeResEntry(sector);

    /* Instance the sucker by executing the pipeline... */
    RpWorldSectorRender(sector);

    RWRETURN(sector);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldInstance is used to instance a world outside of the resource
 * arena. This means that the world will never be instanced again.
 *
 * Once this function has been called two representations of the world will
 * exist, one platform independent (PI) and one platform specific (PS).
 *
 * If the world is serialized, using \ref RpWorldStreamWrite after
 * \ref RpWorldInstance has been called, only the PS representation will be
 * saved. When the world is reloaded, using \ref RpWorldStreamRead, only the
 * PS representation will exist. This data can only be loaded and used on the
 * platform it was created on.
 *
 * All PI geometry data will be lost so certain things will no longer work.
 * For example, collision detection will not work as this relies on a PI data.
 * A second low resolution collision world can be used to solve this problem.
 *
 * The number of vertices in a world sector \ref RpWorldSectorGetNumVertices
 * and the number of triangles \ref RpWorldSectorGetNumTriangles are preserved.
 * Meshes \ref RpMesh are also preserved but the indices themselves are not
 * present.
 *
 * The correct rendering pipelines must be attached to the world sector and
 * materials before this function is called as those rendering pipelines may
 * introduce PS data that is required to give the desired rendering effect.
 *
 * Pre-instancing should not be attempted when PVS is enabled. PVS should be
 * disabled as world sectors are not pre-instanced, if culled by PVS.
 *
 * This function must be called between \ref RwCameraBeginUpdate and
 * \ref RwCameraEndUpdate as this function has to execute the rendering
 * pipelines to guarantee that all the needed data is generated. In essence
 * this function behaves exactly like RpWorldRender, the only difference being
 * that the instance data does not get created in the resource arena but is
 * allocated from the heap and is therefore persistent. All world sectors will
 * be processed and instanced even if they are not inside the camera frustum.
 *
 * For example:
 *
 * \verbatim

   RpWorldAddCamera(World, Camera);

   if (RwCameraBeginUpdate(Camera))
   {
       RpWorldInstance(World);

       RwCameraEndUpdate(Camera);
    }

   RpWorldRemoveCamera(World, Camera);

    \endverbatim
 *
 *
 * \note See the platform specific documentation to see if this feature is
 * not supported on your target platform.
 *
 * The data generated by this function should be considered as being highly
 * volatile and the binary format will change.
 *
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to a RpWorld to instance
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpWorldPluginAttach
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldSectorForAllMeshes
 * \see RpWorldStreamWrite
 * \see RpWorldStreamRead
 * \if gcn \see \ref preinstanceoverview \endif
 *
 */
RwBool
RpWorldInstance(RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldInstance"));
    RWASSERT(world);
    RWASSERT(!(rpWORLDNATIVE & RpWorldGetFlags(world)));

    if( rpWORLDNATIVE & RpWorldGetFlags(world) )
    {
        /* Already native */
        RWRETURN(TRUE);
    }

    /* Mark as persistent but needs instancing */
    RpWorldSetFlags(world, RpWorldGetFlags(world) | rpWORLDNATIVEINSTANCE);

    RpWorldForAllWorldSectors(world, WorldSectorInstance, (void *)NULL);

    /* Mark as persistent */
    RpWorldSetFlags(world, (~rpWORLDNATIVEINSTANCE & RpWorldGetFlags(world)) |
                           rpWORLDNATIVE);

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Iterating over objects in the world

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsub
 * \ref RpWorldForAllClumps is used to apply the given callback function
 * to all clumps in the specified world. The format of the callback function is:
 *
 * RpClump * (*RpClumpCallBack)(RpClump *clump, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure status
 * the iteration is terminated. However, \ref RpWorldForAllClumps will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the clumps.
 * \param fpCallBack  Pointer to the callback function to apply to each clump.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldForAllLights
 * \see RpWorldForAllMaterials
 * \see RpWorldGetNumClumps
 * \see RpWorldGetNumMaterials
 * \see RpWorldGetMaterial
 * \see RpWorldCreate
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldForAllClumps(RpWorld * world, RpClumpCallBack fpCallBack,
                    void *pData)
{
    RWAPIFUNCTION(RWSTRING("RpWorldForAllClumps"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    if (!rwLinkListEmpty(&world->clumpList))
    {
        RwLLLink           *cur, *term, *next;

        cur = rwLinkListGetFirstLLLink(&world->clumpList);
        term = rwLinkListGetTerminator(&world->clumpList);

        while (term != cur)
        {
            RpClump *clump;

            clump = rwLLLinkGetData(cur, RpClump, inWorldLink);

            next = rwLLLinkGetNext(cur);
            if (!fpCallBack(clump, pData))
            {
                /* Early out */
                RWRETURN(world);
            }

            cur = next;
        }
    }

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldForAllMaterials is used to apply the given callback
 * function to all materials used by static objects in the specified world. The
 * format of the callback function is:
 *
 * RpMaterial * (*RpMaterialCallBack)(RpMaterial *material, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure
 * status the iteration is terminated. However, \ref RpWorldForAllMaterials
 * will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the materials.
 * \param fpCallBack  Pointer to the callback function to apply to each material.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldForAllLights
 * \see RpWorldForAllClumps
 * \see RpWorldGetNumClumps
 * \see RpWorldGetNumMaterials
 * \see RpWorldGetMaterial
 * \see RpWorldCreate
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldForAllMaterials(RpWorld * world, RpMaterialCallBack fpCallBack,
                       void *pData)
{
    RwInt32             numMaterials;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RpWorldForAllMaterials"));
    RWASSERT(world);
    RWASSERT(fpCallBack);
    RWASSERT(worldModule.numInstances);

    numMaterials = RpWorldGetNumMaterials(world);
    for (i = 0; i < numMaterials; i++)
    {
        RpMaterial         *material;

        material = RpWorldGetMaterial(world, i);
        if (!fpCallBack(material, pData))
        {
            /* Early out */
            RWRETURN(world);
        }
    }

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldForAllLights is used to apply the given callback function
 * to all lights in the specified world. The format of the callback function
 * is:
 *
 * RpLight * (*RpLightCallBack)(RpLight *light, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure
 * status the iteration is terminated. However, \ref RpWorldForAllLights will
 * still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the lights.
 * \param fpCallBack  Pointer to the callback function to apply to each light.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldForAllClumps
 * \see RpWorldGetNumClumps
 * \see RpWorldForAllMaterials
 * \see RpWorldGetNumMaterials
 * \see RpWorldGetMaterial
 * \see RpWorldCreate
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldForAllLights(RpWorld * world, RpLightCallBack fpCallBack,
                    void *pData)
{
    RwLLLink           *cur, *term, *next;

    RWAPIFUNCTION(RWSTRING("RpWorldForAllLights"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    /* First the directional and ambient lights */
    if (!rwLinkListEmpty(&world->directionalLightList))
    {
        cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
        term = rwLinkListGetTerminator(&world->directionalLightList);

        while (term != cur)
        {
            RpLight *light = rwLLLinkGetData(cur, RpLight, inWorld);

            next = rwLLLinkGetNext(cur);
            if (!fpCallBack(light, pData))
            {
                /* Early out */
                RWRETURN(world);
            }

            cur = next;
        }
    }

    /* Then the others */
    if (!rwLinkListEmpty(&world->lightList))
    {
        cur = rwLinkListGetFirstLLLink(&world->lightList);
        term = rwLinkListGetTerminator(&world->lightList);

        while (term != cur)
        {
            RpLight *light = rwLLLinkGetData(cur, RpLight, inWorld);

            next = rwLLLinkGetNext(cur);
            if (!fpCallBack(light, pData))
            {
                /* Early out */
                RWRETURN(world);
            }

            cur = next;
        }
    }

    /* Both lists are empty */
    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldForAllWorldSectors is used to apply the given callback
 * function to all world sectors in the specified world. The format of the
 * callback function is:
 *
 * RpWorldSector *
 *  (*RpWorldSectorCallBack)(RpWorldSector *worldSector, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure
 * status the iteration is terminated. However, \ref RpWorldForAllWorldSectors
 * will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world containing the world sectors.
 * \param fpCallBack  Pointer to the callback function to apply to each world sector.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpWorldForAllLights
 * \see RpWorldForAllClumps
 * \see RpWorldForAllMaterials
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldForAllWorldSectors(RpWorld * world,
                          RpWorldSectorCallBack fpCallBack, void *pData)
{
    RpSector           *spSect;
    RpSector           *spaStack[rpWORLDMAXBSPDEPTH];
    RwInt32             nStack = 0;

    RWAPIFUNCTION(RWSTRING("RpWorldForAllWorldSectors"));
    RWASSERT(world);
    RWASSERT(fpCallBack);

    /* Start at the top */
    spSect = world->rootSector;

    do
    {
        if (spSect->type < 0)
        {
            /* Its an atomic sector */
            if (!fpCallBack((RpWorldSector *) spSect, pData))
            {
                /* Early out */
                RWRETURN(world);
            }
            spSect = spaStack[nStack--];
        }
        else
        {
            /* Its a plane */
            RpPlaneSector      *pspPlane = (RpPlaneSector *) spSect;

            /* Left then right */
            spSect = pspPlane->leftSubTree;
            spaStack[++nStack] = pspPlane->rightSubTree;
        }
    }
    while (nStack >= 0);

    RWRETURN(world);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetFlags is used to define the property flags for the
 * specified world. The flags only pertain to the static component of the
 * world's geometry.
 *
 * Note that this function replaces existing flags with the new ones, hence
 * any previously set flags are lost. Therefore, if any of the old flags are
 * still required they must be set again with this function.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param flags  A 32-bit integer bit-field specifying the world's properties:
 *      \li rpWORLDTEXTURED - World has textures applied. Texture coordinates
 *                             are specified on a per vertex basis.
 *      \li rpWORLDPRELIT - World has pre-light colors.
 *      \li rpWORLDNORMALS - World has vertex normals.
 *      \li rpWORLDLIGHT - World will be lit.
 *      \li rpWORLDTRISTRIP - World's static geometry can be rendered as triangle strips.
 *      \li rpWORLDMODULATEMATERIALCOLOR - World is rendered with material colors.
 *
 * \return Returns pointer to the world if successful or NULL if
 * there is an error.
 *
 * \see RpWorldGetFlags
 * \see RpWorldGetMaterial
 * \see RpWorldPluginAttach
 *
 */
RpWorld *
RpWorldSetFlags(RpWorld *world, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSetFlags"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(RpWorldSetFlagsMacro(world, flags));
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetFlags is used to retrieve the property flags from
 * the specified world. The flags only pertain to the static component of the
 * world's geometry.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns a 32-bit integer bit-field whose value is the bit-wise OR of
 * the following constants if successful or zero if there is an error or if
 * the atomic's flags are unset:
 * \li rpWORLDTEXTURED - World has textures applied. Texture coordinates
 *                         are specified on a per vertex basis.
 * \li rpWORLDPRELIT - World has pre-light colors.
 * \li rpWORLDNORMALS - World has vertex normals.
 * \li rpWORLDLIGHT - World is lit.
 * \li rpWORLDTRISTRIP - World's static geometry can be rendered as triangle strips.
 * \li rpWORLDMODULATEMATERIALCOLOR - World is rendered with material colors.
 *
 * \see RpWorldSetFlags
 * \see RpWorldGetMaterial
 * \see RpWorldGetNumMaterials
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpWorldGetFlags(const RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetFlags"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    RWRETURN(RpWorldGetFlagsMacro(world));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpworldsub
 * \ref RpWorldRegisterPlugin is used to register a plugin and
 * reserve some space within a world. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size      A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *                  to identify binary chunks).
 * \param constructCB Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB      Copy constructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * of memory reserved for this plugin or -1 if there is an error.
 *
 * \see RpWorldValidatePlugins
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldRegisterPlugin"));
    RWASSERT(!worldModule.numInstances);
    RWASSERT(size >= 0);


    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&worldTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered world plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is identified as being for
 *       this plugin.
 * \param writeCB  Callback used when a chunk should be written out for this plugin.
 * \param getSizeCB  Callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing).
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * of memory reserved for this plugin or -1 if there is an error.
 *
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldValidatePlugins
 * \see RpWorldRegisterPlugin
 * \see RpWorldGetPluginOffset
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */

    plug = rwPluginRegistryAddPluginStream(&worldTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetStreamAlwaysCallBack is used to associate a
 * binary stream callback with a previously registered world plugin. This
 * callback is called for all plugins after stream data reading has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * of memory reserved for this plugin or -1 if there is an error.
 *
 * \see RpWorldRegisterPluginStream
 * \see RpWorldValidatePlugins
 * \see RpWorldRegisterPlugin
 * \see RpWorldGetPluginOffset
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                    RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */

    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &worldTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetStreamRightsCallBack is used to associate a
 * binary stream callback with a previously registered world plugin. This
 * callback is called for the plugin with rights after stream data reading
 * has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param rightsCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * of memory reserved for this plugin or -1 if there is an error.
 *
 * \see RpWorldRegisterPluginStream
 * \see RpWorldValidatePlugins
 * \see RpWorldRegisterPlugin
 * \see RpWorldGetPluginOffset
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSetStreamRightsCallBack(RwUInt32 pluginID,
                               RwPluginDataChunkRightsCallBack rightsCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSetStreamRightsCallBack"));
    RWASSERT(rightsCB);

    /* Everything's cool, so pass it on */

    plug = _rwPluginRegistryAddPlgnStrmRightsCB(&worldTKList, pluginID,
                                                rightsCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetPluginOffset is used to get the offset of a previously
 * registered world plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldValidatePlugins
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpWorldGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&worldTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldValidatePlugins is used to validate the plugin memory
 * allocated within the specified world. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  world   Pointer to the world.
 *
 * \return Returns TRUE if the world data is valid or FALSE if there is an
 * error or if the world data has become corrupt.
 *
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpWorldValidatePlugins(RpWorld * world __RWUNUSEDRELEASE__)
{
    RwBool              valid = TRUE;
    RWAPIFUNCTION(RWSTRING("RpWorldValidatePlugins"));
    RWASSERT(worldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

#ifdef RWDEBUG
    valid = rwPluginRegistryValidateObject(&worldTKList, world);
#endif /* RWDEBUG */

    RWRETURN(valid);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldPluginAttach is used to attach the world plugin to the
 * RenderWare system to enable scene management. The world plugin must be
 * attached between initialising the system with \ref RwEngineInit and opening
 * it with \ref RwEngineOpen.
 *
 * Use this function if the application wishes to use the
 * following system functionality: worlds and world sectors, clumps, atomics,
 * geometry, lights, materials.
 *
 * The include file rpworld.h is also required and must be included by an
 * application wishing to use scene management
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 *
 */
RwBool
RpWorldPluginAttach(void)
{
    RwInt32             state;

    RWAPIFUNCTION(RWSTRING("RpWorldPluginAttach"));

    /* Add the world 'plugin' */
    state = 0;
    state |= RwEngineRegisterPlugin(sizeof(rpMaterialGlobals), rwID_MATERIALMODULE,
                                    _rpMaterialOpen, _rpMaterialClose);
    state |= RwEngineRegisterPlugin(sizeof(rpMeshGlobals), rwID_MESHMODULE,
                                    _rpMeshOpen, _rpMeshClose);
    state |= RwEngineRegisterPlugin(sizeof(rpGeometryGlobals), rwID_GEOMETRYMODULE,
                                    _rpGeometryOpen, _rpGeometryClose);
    state |= RwEngineRegisterPlugin(sizeof(rpClumpGlobals), rwID_CLUMPMODULE,
                                    _rpClumpOpen, _rpClumpClose);
    state |= RwEngineRegisterPlugin(sizeof(rpLightGlobals), rwID_LIGHTMODULE,
                                    _rpLightOpen, _rpLightClose);
    state |= RwEngineRegisterPlugin(0, rwID_SECTORMODULE,
                                    _rpSectorOpen, _rpSectorClose);
    state |= RwEngineRegisterPlugin(sizeof(rpWorldGlobals), rwID_WORLDMODULE,
                                    WorldOpen, WorldClose);
    state |= RwEngineRegisterPlugin(0, rwID_BINWORLDMODULE,
                                    _rpBinaryWorldOpen,
                                    _rpBinaryWorldClose); /* No globals */

    if (state < 0)
    {
        /* If any are negative, we've failed */
        RWRETURN(FALSE);
    }

    /* Hook up the objects to the world */
    if (!_rpWorldObjRegisterExtensions())
    {
        /* Failed to hook up objects */
        RWRETURN(FALSE);
    }

    /* Hook up clumpable objects to clumps */
    if (!_rpClumpRegisterExtensions())
    {
        /* Failed to hook up objects */
        RWRETURN(FALSE);
    }

    /* moved to here to be after the binmesh plugin.  There seems to be */
    /* another way to do post initialisation device specific stuff, but */
    /* I can't quite see how at the moment */
    if (!_rxWorldDevicePluginAttach())
    {
        RWRETURN(FALSE);
    }

    /* Hurrah */
    RWRETURN(TRUE);
}


