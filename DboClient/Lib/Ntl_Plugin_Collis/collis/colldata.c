/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    colldata.c
 *  Purpose: Collision data management
 */

/******************************************************************************
 *  Includes
 */

#include "rwcore.h"
#include "rpdbgerr.h"
#include "rpworld.h"

#include "rpcollis.rpe"
#include "ctdata.h"

#include "colldata.h"


/******************************************************************************
 *  Global vars
 */
#if (defined(RWDEBUG))
long    _rpCollisionStackDepth = 0;
#endif /* (defined(RWDEBUG)) */

RwInt32 _rpCollisionNumInstances = 0;
RwInt32 _rpCollisionGlobalRefOffset = 0;
RwInt32 _rpCollisionAtomicRefOffset = 0;
RwInt32 _rpCollisionGeometryDataOffset = 0;
RwInt32 _rpCollisionWorldSectorDataOffset = 0;

/******************************************************************************
 */


static void        *
CollisionOpen(void *instance,
              RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("CollisionOpen"));
    RWASSERT(instance);

    _rpCollisionNumInstances++;

    /* Init atomic collision reference counter */
    _rpCollisionGlobalRefMacro() = 0;

    RWRETURN(instance);
}


static void        *
CollisionClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("CollisionClose"));
    RWASSERT(instance);

    _rpCollisionNumInstances--;

    RWRETURN(instance);
}



/******************************************************************************
 *
 *  CollisionData plugin to world sectors and geometries
 *
 */

static void *
CollisionDataDestructor(void *object,
                        RwInt32 offset,
                        RwInt32 __RWUNUSED__ size)
{
    RpCollisionData *collData;

    RWFUNCTION(RWSTRING("CollisionDataDestructor"));
    RWASSERT(object);

    collData = RWPLUGINOFFSET(RpCollisionData, object, offset);
    if (collData->tree)
    {
        _rpCollTreeDestroy(collData->tree);
        collData->tree = (RpCollTree *)NULL;
    }

    RWRETURN(object);
}


static void *
CollisionDataConstructor(void *object,
                         RwInt32 offset,
                         RwInt32 __RWUNUSED__ size)
{
    RpCollisionData *collData;

    RWFUNCTION(RWSTRING("CollisionDataConstructor"));
    RWASSERT(object);

    collData = RWPLUGINOFFSET(RpCollisionData, object, offset);
    collData->tree = (RpCollTree *)NULL;

    RWRETURN(object);
}


static              RwInt32
CollisionDataStreamGetSize(const void *object,
                           RwInt32 offsetInObject,
                           RwInt32 __RWUNUSED__ sizeInObject)
{
    const RpCollisionData *collData;
    RwUInt32    binarySize;

    RWFUNCTION(RWSTRING("CollisionDataStreamGetSize"));
    RWASSERT(object);

    collData = RWPLUGINOFFSETCONST(RpCollisionData, object, offsetInObject);
    if (collData->tree)
    {
        binarySize = sizeof(RwUInt32)
            + rwCHUNKHEADERSIZE
            + _rpCollTreeStreamGetSize(collData->tree);
    }
    else
    {
        /* Specifying zero means that no plugin extension is written */
        binarySize = 0;
    }

    RWRETURN(binarySize);
}


static RwStream    *
CollisionDataStreamWrite(RwStream * stream,
                         RwInt32 __RWUNUSED__ binaryLength,
                         const void *object,
                         RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    const RpCollisionData *collData;

    RWFUNCTION(RWSTRING("CollisionDataStreamWrite"));
    RWASSERT(stream);
    RWASSERT(object);

    collData = RWPLUGINOFFSETCONST(RpCollisionData, object, offset);
    if (collData->tree)
    {
        RwUInt32  version = RwEngineGetVersion();

        if (!RwStreamWriteInt32(stream,
                (const RwInt32 *) &version, sizeof(RwInt32)))
        {
            RWRETURN((RwStream *)NULL);
        }

        if (!_rpCollTreeStreamWrite(collData->tree, stream))
        {
            RWRETURN((RwStream *)NULL);
        }
    }

    RWRETURN(stream);
}


static RwStream    *
CollisionDataStreamRead(RwStream *stream,
                        RwInt32 __RWUNUSED__ binaryLength,
                        void     *object,
                        RwInt32   offset,
                        RwInt32 __RWUNUSED__ size)
{
    RwUInt32            version;
    RpCollisionData    *collData;

    RWFUNCTION(RWSTRING("CollisionDataStreamRead"));
    RWASSERT(stream);
    RWASSERT(object);

    collData = RWPLUGINOFFSET(RpCollisionData, object, offset);

    /* Get version */
    if (!RwStreamReadInt(stream, (RwInt32 *)&version, sizeof(RwInt32)))
    {
        RWRETURN((RwStream *)NULL);
    }

    /* Because we aren't passed the version from the chunk header, we
     * need to resort to this slight hack to deal with old data that
     * doesn't contain the version number. In this case the value
     * we've just read is actually the number of leaves in the partition
     * tree (restricted to 16bit number so doesn't clash with versions).
     */
    if (version < 0x36001)
    {
        RwUInt32  numTriangles, numSplits;
        RwBool    failConv;
        RwBBox    bbox;

        if (!RwStreamReadInt(stream,
                (RwInt32 *)&numTriangles, sizeof(RwInt32)))
        {
            RWRETURN((RwStream *)NULL);
        }

        if (RwObjectGetType(object) == rpGEOMETRY)
        {
            RpMorphTarget *mt;
            RwSphere      *sph;

            mt = RpGeometryGetMorphTarget((RpGeometry *)object, 0);
            sph = RpMorphTargetGetBoundingSphere(mt);
            bbox.inf.x = sph->center.x - sph->radius;
            bbox.inf.y = sph->center.y - sph->radius;
            bbox.inf.z = sph->center.z - sph->radius;
            bbox.sup.x = sph->center.x + sph->radius;
            bbox.sup.y = sph->center.y + sph->radius;
            bbox.sup.z = sph->center.z + sph->radius;
        }
        else
        {
            bbox = *RpWorldSectorGetTightBBox((RpWorldSector *)object);
        }

        numSplits = version - 1;
        failConv = FALSE;
        collData->tree = _rpCollTreeStreamReadPre36001(
            stream, numTriangles, numSplits, &bbox, &failConv);

        if (failConv)
        {
            /* Conversion failed, but don't return NULL, since we need to
             * give the app a chance to read in the rest of the stream and
             * rebuild collision data. We don't rebuild automatically since
             * that would add an object file dependency which we don't want
             */
            RWRETURN(stream);
        }
    }
    else
    {
        if (!RwStreamFindChunk(stream, rwID_COLLTREE, NULL, NULL))
        {
            RWRETURN((RwStream *)NULL);
        }

        collData->tree = _rpCollTreeStreamRead(stream);
    }

    RWRETURN(collData->tree ? stream : (RwStream *)NULL);
}

static RwBool
CollisionGeometryPluginAttach(void)
{
    RwBool              result;
    RwInt32             offset;

    RWFUNCTION(RWSTRING("CollisionGeometryPluginAttach"));

    _rpCollisionGeometryDataOffset =
        RpGeometryRegisterPlugin(sizeof(RpCollisionData *),
                                 rwID_COLLISPLUGIN,
                                 CollisionDataConstructor,
                                 CollisionDataDestructor,
                                 (RwPluginObjectCopy)NULL);

    result = (_rpCollisionGeometryDataOffset >= 0);

    if (result)
    {

        offset =
            RpGeometryRegisterPluginStream(rwID_COLLISPLUGIN,
                                           CollisionDataStreamRead,
                                           CollisionDataStreamWrite,
                                           CollisionDataStreamGetSize);

        result = (offset >= 0);

    }

    RWRETURN(result);
}


static RwBool
CollisionWorldSectorPluginAttach(void)
{
    RwBool              result;
    RwInt32             offset;

    RWFUNCTION(RWSTRING("CollisionWorldSectorPluginAttach"));

    _rpCollisionWorldSectorDataOffset =
        RpWorldSectorRegisterPlugin(sizeof(RpCollisionData *),
                                    rwID_COLLISPLUGIN,
                                    CollisionDataConstructor,
                                    CollisionDataDestructor,
                                    (RwPluginObjectCopy)NULL);

    result = (_rpCollisionWorldSectorDataOffset >= 0);

    if (result)
    {

        offset = RpWorldSectorRegisterPluginStream(rwID_COLLISPLUGIN,
                                                   CollisionDataStreamRead,
                                                   CollisionDataStreamWrite,
                                                   CollisionDataStreamGetSize);

        result = (offset >= 0);

    }

    RWRETURN(result);
}


/******************************************************************************
 *
 *  Plugin to atomics
 *
 */

static void        *
CollisionAtomicInit(void *object,
                    RwInt32 __RWUNUSED__ offset,
                    RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("CollisionAtomicInit"));

    /*  Collision atomic extension - holds a collision reference which is
     *  updated when an atomic is intersected so that it can be eliminated if
     *  later found in the same intersection test (due to attachment to
     *  multiple world sectors).
     */

    _rpCollisionAtomicRefMacro(object) = 0;

    RWRETURN(object);
}



/****************************************************************************
 */


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  API functions
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


/*
 * ingroup rpcollis
 * _rpCollisionGeometrySetCollTree
 * attaches collision tree to a geometry.
 *
 * param geometry Pointer to the geometry.
 * param collTree Pointer to the collTree (can be NULL).
 *
 * return Pointer to the geometry.
 *
 * see RpCollisionGeometryGetData
 * see RpCollisionGeometryBuildData
 * see RpCollisionGeometryDestroyData
 * see RpCollisionGeometryQueryData
 */
RpGeometry *
_rpCollisionGeometrySetCollTree(RpGeometry *geometry, RpCollTree *collTree)
{
    RpCollisionData  *collData;

    RWFUNCTION(RWSTRING("_rpCollisionGeometrySetCollTree"));
    RWASSERT(geometry);

    collData = _rpCollisionGeometryGetDataMacro(geometry);
    collData->tree = collTree;

    RWRETURN(geometry);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpcollis
 * \ref RpCollisionGeometryGetCollTree retrieves the collision
 * tree attached to a geometry.
 *
 * \param geometry   Pointer to the geometry
 *
 * \return Pointer to the collision tree or NULL if the geometry
 * has no collision tree attached.
 *
 * \note This function is a macro in release builds.
 *
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionGeometryDestroyData
 * \see RpCollisionGeometryQueryData
 * \see RpCollSectorLineItInit
 * \see RpCollSectorBBoxItInit
 * \see \ref rpcollisoverview
 */
RpCollTree *
RpCollisionGeometryGetCollTree(RpGeometry *geometry)
{
    RpCollisionData  *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionGeometryGetCollTree"));
    RWASSERT(geometry);

    collData = _rpCollisionGeometryGetDataMacro(geometry);
    RWRETURN(collData->tree);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/**
 * \ingroup rpcollis
 * \ref RpCollisionGeometryDestroyData is called to destroy
 * collision data for a geometry.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param geometry   Pointer to the geometry
 *
 * \return Pointer to the geometry if successful, NULL otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionGeometryQueryData
 * \see \ref rpcollisoverview
 *
 */
RpGeometry *
RpCollisionGeometryDestroyData(RpGeometry * geometry)
{
    RpCollisionData *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionGeometryDestroyData"));
    RWASSERT(geometry);

    /* Destroy any collision data */
    collData = _rpCollisionGeometryGetDataMacro(geometry);
    if (collData->tree)
    {
        _rpCollTreeDestroy(collData->tree);
        collData->tree = NULL;
    }

    RWRETURN(geometry);
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionGeometryQueryData is called to query if
 * collision data exists for the given geometry.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param geometry   Pointer to the geometry
 *
 * \return TRUE if collision data is present, FALSE otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionGeometryDestroyData
 * \see RpCollisionGeometryGetCollTree
 * \see \ref rpcollisoverview
 *
 */
RwBool
RpCollisionGeometryQueryData(RpGeometry *geometry)
{
    RpCollisionData *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionGeometryQueryData"));
    RWASSERT(geometry);

    collData = _rpCollisionGeometryGetDataMacro(geometry);
    RWRETURN(collData->tree != NULL);
}


/*
 * ingroup rpcollis
 * _rpCollisionWorldSectorSetCollTree
 * attaches collision tree to a world sector.
 *
 * param worldSector Pointer to the world sector
 * param collTree Pointer to the collTree (can be NULL).
 *
 * return Pointer to the world sector.
 *
 * see RpCollisionWorldSectorGetCollTree
 * see RpCollisionWorldSectorBuildData
 * see RpCollisionWorldSectorDestroyData
 * see RpCollisionWorldSectorQueryData
 */
RpWorldSector *
_rpCollisionWorldSectorSetCollTree(RpWorldSector *worldSector, RpCollTree *collTree)
{
    RpCollisionData  *collData;

    RWFUNCTION(RWSTRING("_rpCollisionWorldSectorSetCollTree"));
    RWASSERT(worldSector);

    collData = _rpCollisionWorldSectorGetDataMacro(worldSector);
    collData->tree = collTree;

    RWRETURN(worldSector);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldSectorGetCollTree retrieves the collision
 * tree attached to a world sector.
 *
 * \param worldSector   Pointer to the world sector
 *
 * \return Pointer to the collision tree or NULL if the world sector
 * has no collision tree attached.
 *
 * \note This function is a macro in release builds.
 *
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldSectorDestroyData
 * \see RpCollisionWorldSectorQueryData
 * \see RpCollSectorLineItInit
 * \see RpCollSectorBBoxItInit
 * \see \ref rpcollisoverview
 */
RpCollTree *
RpCollisionWorldSectorGetCollTree(RpWorldSector *worldSector)
{
    RpCollisionData  *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionWorldSectorGetCollTree"));
    RWASSERT(worldSector);

    collData = _rpCollisionWorldSectorGetDataMacro(worldSector);
    RWRETURN(collData->tree);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldSectorDestroyData is called to destroy
 * collision data for a world sector.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param worldSector   Pointer to the world sector
 *
 * \return Pointer to the world sector if successful, NULL otherwise.
 *
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldSectorQueryData
 * \see RpCollisionWorldBuildData
 * \see RpCollisionWorldDestroyData
 * \see RpCollisionWorldQueryData
 * \see RpCollisionPluginAttach
 * \see \ref rpcollisoverview
 *
 */
RpWorldSector *
RpCollisionWorldSectorDestroyData(RpWorldSector *worldSector)
{
    RpCollisionData  *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionWorldSectorDestroyData"));
    RWASSERT(worldSector);

    /* Destroy any collision data */
    collData = _rpCollisionWorldSectorGetDataMacro(worldSector);
    if (collData->tree)
    {
        _rpCollTreeDestroy(collData->tree);
        collData->tree = NULL;
    }

    RWRETURN(worldSector);
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldSectorQueryData is called to query if
 * collision data exists for the given world sector.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param worldSector   Pointer to the world sector
 *
 * \return TRUE if collision data is present, FALSE otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionWorldBuildData
 * \see RpCollisionWorldQueryData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldSectorGetCollTree
 * \see \ref rpcollisoverview
 *
 */
RwBool
RpCollisionWorldSectorQueryData(RpWorldSector *worldSector)
{
    RpCollisionData  *collData;

    RWAPIFUNCTION(RWSTRING("RpCollisionWorldSectorQueryData"));
    RWASSERT(worldSector);

    collData = _rpCollisionWorldSectorGetDataMacro(worldSector);
    RWRETURN(collData->tree != NULL);
}


static RpWorldSector *
WorldSectorDestroyDataCallBack( RpWorldSector * worldSector,
                                void * data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("WorldSectorDestroyDataCallBack"));
    RWASSERT(worldSector);

    /* Skip empty sectors */
    if (worldSector->numTriangles)
    {
        RpCollisionWorldSectorDestroyData(worldSector);
    }

    RWRETURN(worldSector);
}

/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldDestroyData is called to destroy the attached
 * collision data for the given world.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param world   Pointer to the world
 *
 * \return Pointer to the world if successful, NULL otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionWorldBuildData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldSectorDestroyData
 * \see \ref rpcollisoverview
 */

RpWorld *
RpCollisionWorldDestroyData(RpWorld *world)
{
    RWAPIFUNCTION(RWSTRING("RpCollisionWorldDestroyData"));
    RWASSERT(world);

    /* Destroy collision tree for all sectors */
    RpWorldForAllWorldSectors(world, WorldSectorDestroyDataCallBack, NULL);

    RWRETURN(world);
}



static RpWorldSector *
WorldSectorQueryDataCallBack(RpWorldSector * worldSector, void *data)
{
    RwBool    *result = (RwBool *) data;

    RWFUNCTION(RWSTRING("WorldSectorQueryDataCallBack"));
    RWASSERT(worldSector);

    /* Skip empty sectors */
    if (worldSector->numTriangles)
    {
        if (RpCollisionWorldSectorQueryData(worldSector))
        {
            *result = TRUE;
            RWRETURN(NULL);
        }
    }

    RWRETURN(worldSector);
}

/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldQueryData is called to query for collision data for
 * the given world.
 *
 * The result is TRUE if any world sectors contains collision data. FALSE if
 * no world sectors contains collision data.
 *
 * The collision and world plug-ins must be attached before using this
 * function. The header file rpcollis.h is required.
 *
 * \param world   Pointer to the world
 *
 * \return TRUE if collision data are present. FALSE otherwise.
 *
 * \see RpCollisionPluginAttach
 * \see RpCollisionWorldBuildData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldSectorQueryData
 * \see \ref rpcollisoverview
 */

RwBool
RpCollisionWorldQueryData(RpWorld *world)
{
    RwBool        result;

    RWAPIFUNCTION(RWSTRING("RpCollisionWorldQueryData"));
    RWASSERT(world);

    /* Assume no data. */
    result = FALSE;

    /* Query the collision tree for all sectors */
    RpWorldForAllWorldSectors(world, WorldSectorQueryDataCallBack, &result);

    RWRETURN(result);
}

/**
 * \ingroup rpcollis
 * \ref RpCollisionPluginAttach is called by the application to indicate
 * that the collision plugin should be used. The call
 * to this function should be placed between \ref RwEngineInit and
 * \ref RwEngineOpen and the world plugin must already be attached.
 *
 * The library rpcollis and the header file rpcollis.h are required.
 *
 * \return True on success, false otherwise
 *
 * \see RpWorldPluginAttach
 * \see RpWorldForAllWorldSectorIntersections
 * \see RpWorldForAllAtomicIntersections
 * \see RpCollisionWorldForAllIntersections
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldBuildData
 * \see RpAtomicForAllIntersections
 * \see RpCollisionGeometryForAllIntersections
 * \see RpCollisionGeometryBuildData
 * \see \ref rpcollisoverview
 */
RwBool
RpCollisionPluginAttach(void)
{
    RwBool              result;

    RWAPIFUNCTION(RWSTRING("RpCollisionPluginAttach"));

    /* Global instance data */
    _rpCollisionGlobalRefOffset =
        RwEngineRegisterPlugin(sizeof(RwUInt32),
                               rwID_COLLISPLUGIN,
                               CollisionOpen, CollisionClose);

    result = (_rpCollisionGlobalRefOffset > 0);

    /* Atomic plugin - for storing collision reference ID */
    if (result)
    {
        _rpCollisionAtomicRefOffset =
            RpAtomicRegisterPlugin(sizeof(RwInt32),
                                   rwID_COLLISPLUGIN,
                                   CollisionAtomicInit,
                                   (RwPluginObjectDestructor)NULL,
                                   (RwPluginObjectCopy)NULL);

        result = (_rpCollisionAtomicRefOffset > 0)
            && CollisionWorldSectorPluginAttach()
            && CollisionGeometryPluginAttach();
    }

    RWRETURN(result);
}

