/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    collbuild.c
 *  Purpose: Collision data generation
 */

/******************************************************************************
 *  Include files
 */

#include <string.h>

#include "rwcore.h"
#include "rpdbgerr.h"
#include "rpworld.h"

#include "ctdata.h"
#include "ctbuild.h"
#include "colldata.h"

#include "collbuild.h"


/******************************************************************************
 *  Functions
 */

/*
 * Generic Build function
 */
static RpCollTree *
BuildTriListCollTree(void        *object,
                     RwUInt32     numVerts,
                     RwV3d       *verts,
                     RwUInt32     numTris,
                     RpTriangle  *tris,
                     RpTriangle  *sortTris,
                     RpCollisionBuildParam *param)
{
    RpCollTree             *tree;
    RpCollisionBuildParam   defaultParam;

    RWFUNCTION(RWSTRING("BuildTriListCollTree"));
    RWASSERT(object);
    RWASSERT(numVerts);
    RWASSERT(numTris);
    RWASSERT(verts);
    RWASSERT(tris);
    RWASSERT(sortTris);

    /* Handle NULL param */
    if (!param)
    {
        RpCollisionBuildParamInitDefaults(&defaultParam);
        param = &defaultParam;
    }

    /* Are we sorting triangles? */
    if (rpCOLLISIONBUILD_SORTTRIANGLES & param->flags)
    {
        RwUInt32        size;
        RwUInt16       *map;
        RpTriangle     *copyTris;

        /* Create buffer for sort map and copy of originals */
        size = numTris * (sizeof(RwUInt16) + sizeof(RpTriangle));
        map = (RwUInt16 *)
            RwMalloc(size, rwID_COLLISPLUGIN | rwMEMHINTDUR_FUNCTION);
        if (!map)
        {
            RWERROR((E_RW_NOMEM, size));
            RWRETURN((RpCollTree *)NULL);
        }

        copyTris = (RpTriangle *)(map + numTris);
        memcpy(copyTris, sortTris, numTris*sizeof(RpTriangle));

        /* Create collision tree */
        tree = _rpCollTreeBuild(numVerts, verts, numTris, tris, map);
        if (tree)
        {
            RwUInt32    i;

            for (i=0; i<numTris; i++)
            {
                sortTris[i] = copyTris[map[i]];
            }

            if (param->sortCallBack)
            {
                param->sortCallBack(object, tree, map, param->sortCallBackData);
            }
        }

        RwFree(map);
    }
    else
    {
        /* Create collision tree */
        tree = _rpCollTreeBuild(numVerts, verts, numTris, tris, NULL);
    }

    RWRETURN(tree);
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionBuildParamInitDefaults initializes an
 * \ref RpCollisionBuildParam structure with the default values.
 *
 * \param param Pointer to the parameter block.
 *
 * \return Pointer to the parameter block.
 *
 * The defaults are
 *
 * \li \c flags = 0
 * \li \c sortCallBack = \c NULL
 * \li \c sortCallBackData = \c NULL
 *
 * After calling this function, modifications may be made in the application
 * before calling a collision data build function.
 *
 * \see RpCollisionWorldBuildData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionGeometryBuildData
 *
 */
RpCollisionBuildParam *
RpCollisionBuildParamInitDefaults(RpCollisionBuildParam *param)
{
    RWAPIFUNCTION(RWSTRING("RpCollisionBuildParamInitDefaults"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(param);

    param->flags = 0;
    param->sortCallBack = (RpCollisionBuildSortCallBack) NULL;
    param->sortCallBackData = (void *)NULL;

    RWRETURN(param);
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionGeometryBuildData builds collision extension data for a
 * geometry.
 *
 * \param geometry   Pointer to the geometry
 * \param param      Pointer to build parameters. If set to NULL
 *                   then defaults are used (see
 *                   \ref RpCollisionBuildParamInitDefaults)
 *
 * \return Pointer to the geometry if successful, NULL otherwise.
 *
 * The data contains a 'collision tree' which is used to do
 * efficient collision tests with the triangles of the geometry
 * (see \ref rpcollisoverview). It is only applicable to rigid geometries
 * with a single morph target and must be rebuilt if the geometry is modified.
 *
 * This function is intended for offline use. The collision data is
 * streamed with the geometry as plugin extension data.
 *
 * \see RpCollisionGeometryForAllIntersections
 * \see RpAtomicForAllIntersections
 * \see RpCollisionGeometryGetCollTree
 * \see RpCollisionGeometryDestroyData
 * \see RpCollisionGeometryQueryData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionPluginAttach
 */
RpGeometry *
RpCollisionGeometryBuildData(RpGeometry            *geometry,
                             RpCollisionBuildParam *param)
{
    RpCollTree  *oldTree, *collTree;

    RWAPIFUNCTION(RWSTRING("RpCollisionGeometryBuildData"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(geometry);

    collTree = BuildTriListCollTree(
        geometry,
        geometry->numVertices,
        geometry->morphTarget->verts,
        geometry->numTriangles,
        geometry->triangles,
        geometry->triangles, /*Triangles for sorting */
        param);

    if (!collTree)
    {
        RWRETURN((RpGeometry *)NULL);
    }

    /* Attach collision tree to geometry (removing any old one) */
    oldTree = RpCollisionGeometryGetCollTree(geometry);
    if (oldTree)
    {
        _rpCollTreeDestroy(oldTree);
    }

    _rpCollisionGeometrySetCollTree(geometry, collTree);

    RWRETURN(geometry);
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldSectorBuildData builds collision extension data for a
 * world sector.
 *
 * \param worldSector   Pointer to the world sector
 * \param param         Pointer to build parameters. If set to NULL
 *                      then defaults are used (see
 *                      \ref RpCollisionBuildParamInitDefaults)
 *
 * \return Pointer to the world sector if successful, NULL otherwise.
 *
 * The data contains a 'collision tree' which is used to do
 * efficient collision tests with the triangles of the world sector
 * (see \ref rpcollisoverview).
 *
 * The function \ref RpCollisionWorldBuildData may alternatively be used to
 * generate collision data for all world sectors of an \ref RpWorld.
 *
 * The build functions are intended for offline use in exporters and tools.
 *
 * \see RpCollisionWorldForAllIntersections
 * \see RpCollisionWorldSectorGetCollTree
 * \see RpCollisionWorldSectorQueryData
 * \see RpCollisionWorldSectorDestroyData
 * \see RpCollisionWorldBuildData
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionPluginAttach
 */
RpWorldSector *
RpCollisionWorldSectorBuildData(RpWorldSector *worldSector,
                                RpCollisionBuildParam *param)
{
    RWAPIFUNCTION(RWSTRING("RpCollisionWorldSectorBuildData"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(worldSector);

    /* Check that sector actually contains something */
    if (worldSector->numTriangles && worldSector->triangles)
    {
        RpCollTree     *oldTree, *collTree;

        /* Build collision tree */
        collTree = BuildTriListCollTree(
            worldSector,
            worldSector->numVertices,
            worldSector->vertices,
            worldSector->numTriangles,
            worldSector->triangles,
            worldSector->triangles, /* Array for sorting */
            param);

        if (!collTree)
        {
            RWRETURN((RpWorldSector *)NULL);
        }

        /* Attach collision tree to world sector (removing any old one) */
        oldTree = RpCollisionWorldSectorGetCollTree(worldSector);
        if (oldTree)
        {
            _rpCollTreeDestroy(oldTree);
        }

        _rpCollisionWorldSectorSetCollTree(worldSector, collTree);
    }

    RWRETURN(worldSector);
}


/******************************************************************************
 *
 *  RpCollisionWorldBuildData:-
 *
 *  worldsector callback and callback data.
 */
typedef struct buildCallBackData buildCallBackData;
struct buildCallBackData
{
    RpCollisionBuildParam *param;
    RwBool              error;
};

static RpWorldSector *
WorldSectorBuildDataCallBack(RpWorldSector *worldSector,
                             void *data)
{
    buildCallBackData  *buildData = (buildCallBackData *) data;

    RWFUNCTION(RWSTRING("WorldSectorBuildDataCallBack"));
    RWASSERT(worldSector);
    RWASSERT(buildData);

    /* Skip empty sectors */
    if (worldSector->numTriangles)
    {
        if (!RpCollisionWorldSectorBuildData(worldSector, buildData->param))
        {
            /* Failed to build collision tree */
            buildData->error = TRUE;
            RWRETURN((RpWorldSector *)NULL);
        }
    }

    RWRETURN(worldSector);
}

/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldBuildData is a convenience function which calls
 * \ref RpCollisionWorldSectorBuildData for all world sectors in a world.
 *
 * \param world   Pointer to the world
 * \param param   Pointer to the build parameters. If set to NULL
 *                then defaults are used (see
 *                 \ref RpCollisionBuildParamInitDefaults)
 *
 * \return Pointer to the world if successful, NULL otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionWorldForAllIntersections
 * \see RpCollisionWorldQueryData
 * \see RpCollisionWorldDestroyData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionGeometryBuildData
 */
RpWorld *
RpCollisionWorldBuildData(RpWorld *world,
                          RpCollisionBuildParam *param)
{
    buildCallBackData   buildData;

    RWAPIFUNCTION(RWSTRING("RpCollisionWorldBuildData"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(world);

    buildData.param = param;
    buildData.error = FALSE;

    /* Build collision tree for all sectors */
    RpWorldForAllWorldSectors(world, WorldSectorBuildDataCallBack, &buildData);

    /* Check for any errors */
    if (buildData.error)
    {
        RWRETURN((RpWorld *)NULL);
    }

    RWRETURN(world);
}
