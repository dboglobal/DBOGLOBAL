/*
 * Manipulating world object custom pipelines
 * Copyright (c) Criterion Software Limited
 */

/**
 * \ingroup worldextensions
 * \page worldextensionsoverview World Extensions Overview
 *
 * Platform specific pipelines for rendering an RpAtomic or RpWorldSector.
 *
 */


/* Platform-specific documentation is in world/pipe/p2/[target]/wrldpipe.c */
/* #include <assert.h> */
#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rwcore.h"

#include "bageomet.h"
#include "balight.h"
#include "baworld.h"

/* The pipeline construction functions  -- device specific */
#include "wrldpipe.h"
#if (((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))
#include "fastim3d.h"
#endif /*(((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))*/

/* This file */
#include "bapipew.h"

/****************************************************************************
 Local defines
 */

/****************************************************************************
 Local types
 */

typedef struct RxMatIndexBounds RxMatIndexBounds;
struct RxMatIndexBounds
{
    RwUInt16            minIndex;
    RwUInt16            maxIndex;
};


/*******************************************************************
 * The below have been rehoused from
 * rwsdk/world/bageomet.c
 * to prevent consfusion to Autoduck
 */


/* Used to sort geoemtry vertices by material */
typedef struct SortVert SortVert;
struct SortVert
{
        RwInt32             faceIndex; /* Top 2 bits hold which vert of the face this was (0, 1, 2) */
        RwInt16             matIndex;
        RwUInt16            origIndex;
};

static int
SortVertsByMaterialCB(const void *data1, const void *data2)
{
    const SortVert     *vert1 = (const SortVert *) data1;
    const SortVert     *vert2 = (const SortVert *) data2;
    RwInt16             matIndex1, matIndex2;
    RwUInt16            origIndex1, origIndex2;

    RWFUNCTION(RWSTRING("SortVertsByMaterialCB"));
    RWASSERT(vert1 != NULL);
    RWASSERT(vert2 != NULL);

    matIndex1 = vert1->matIndex;
    matIndex2 = vert2->matIndex;

    /* Any verts found to be different (on material even if not original
     * vertIndex) will eventually end up as different verts in the new
     * geometry, so verts will be duplicated at material boundaries. This sort
     * takes place before the sort on indices, so verts should end up index-
     * sorted within material runs */
    if (matIndex1 > matIndex2)
    {
        RWRETURN(1);
    }
    else if (matIndex1 < matIndex2)
    {
        RWRETURN(-1);
    }

    /* Sort on vertIndex so SortVerts generated from the same original vertex
     * (i.e by all the triangles which share that vertex) are grouped and we
     * get verts sorted on index within materials (i.e when the above two tests
     * don't find a difference, we sort on index) */
    origIndex1 = vert1->origIndex;
    origIndex2 = vert2->origIndex;

    if (origIndex1 > origIndex2)
    {
        RWRETURN(1);
    }
    else if (origIndex1 < origIndex2)
    {
        RWRETURN(-1);
    }

    RWRETURN(0);
}
/**
 * \ingroup rpgeometry
 * \ref RpGeometryIsCorrectlySorted is used to check if the specified
 * geometry is sorted by material, with no shared vertices at material
 * boundaries.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  geometry   A pointer to an \ref RpGeometry.
 * \param  result   A pointer to an \ref RwBool to receive the result.
 *
 * \return Returns a pointer to the geometry if successful (and result will
 * receive a legitimate value) or NULL if there is an error (and result will
 * receive an unspecified value)
 *
 * \see RpGeometrySortByMaterial
 * \see RpWorldPluginAttach
 *
 */
const RpGeometry *
RpGeometryIsCorrectlySorted(const RpGeometry *geometry, RwBool *result)
{
    RxMatIndexBounds    *matIndexBounds;
    RwInt32             sumOfMeshVertIndexRanges = 0;
    RwUInt16            vertIndex;
    RwInt16             matIndex;
    RwInt32             i, j;
    RwUInt32            bytes;

    /* For PowerPipe, we need to check that the data is in a valid arrangement.
     * A valid arrangement is such that for each mesh (group of triangles
     * referencing the same material) in the object, the vertices that its
     * triangles reference are in a contiguous block, disjoint with all such
     * blocks for other meshes in the object.
     * This test calculates the min/max indices for each mesh and then sums up
     * (1 + maxIndex - minIndex) for all meshes. This should be equal to
     * the total number of vertices if the data is arranged correctly. */
    RWAPIFUNCTION(RWSTRING("RpGeometryIsCorrectlySorted"));
    RWASSERT(geometry != NULL);

    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        bytes = ( geometry->matList.numMaterials * sizeof(RxMatIndexBounds) );

        matIndexBounds = (RxMatIndexBounds *) RwMalloc(bytes,
            rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);

        if (NULL == matIndexBounds)
        {
            RWRETURN((const RpGeometry *)NULL);
        }

        for (i = 0; i < geometry->matList.numMaterials; i++)
        {
            matIndexBounds[i].minIndex = (RwUInt16)~0;
            matIndexBounds[i].maxIndex = (RwUInt16) 0;
        }

        for (i = 0; i < geometry->numTriangles; i++)
        {
            matIndex = (geometry->triangles)[i].matIndex;
            RWASSERT(matIndex < geometry->matList.numMaterials);
            RWASSERT(matIndex >= 0);
            for (j = 0; j < 3; j++)
            {
                vertIndex = (geometry->triangles)[i].vertIndex[j];
                if (vertIndex > matIndexBounds[matIndex].maxIndex)
                {
                    matIndexBounds[matIndex].maxIndex = vertIndex;
                }
                if (vertIndex < matIndexBounds[matIndex].minIndex)
                {
                    matIndexBounds[matIndex].minIndex = vertIndex;
                }
            }
        }

        /* Is this geometry arranged ok? */
        for (i = 0; i < geometry->matList.numMaterials; i++)
        {
            sumOfMeshVertIndexRanges += 1 + matIndexBounds[i].maxIndex -
                matIndexBounds[i].minIndex;
        }

        RwFree(matIndexBounds);
    }

    if (sumOfMeshVertIndexRanges > geometry->numVertices)
    {
        /* Some meshes' ranges must overlap */
        *result = FALSE;
        RWRETURN(geometry);
    }
    else
    {
        /* This geometry is correctly sorted for rendering by PowerPipe */
        *result = TRUE;
        RWRETURN(geometry);
    }
}



/**
 * \ingroup rpgeometry
 * \ref RpGeometrySortByMaterial is used to create a modified clone of
 * the specified geometry, the modification entailing sorting vertices by
 * material and duplicating them along material boundaries. The new geometry
 * is returned in the unlocked state.
 *
 * If the source geometry contains any plugin data, the app should provide
 * an \ref RpGeometrySortByMaterialCallBack function to update this data as
 * appropriate. The callback will receive pointers to the original geometry,
 * the new geometry, a mapping array (for every vertex in the new geometry,
 * what was the index of the corresponding vertex in the source geometry?)
 * and the length of that array (the number of vertices in the new geometry).
 *
 * The world plugin must be attached before using this function.
 *
 * \param  source   A pointer to an \ref RpGeometry.
 * \param  callback   A pointer to a
 * \ref RpGeometrySortByMaterialCallBack function, or NULL if not required
 *
 * \return Returns pointer to the new, sorted geometry if successful or
 * NULL if there is an error.
 *
 * \see RpGeometryIsCorrectlySorted
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometrySortByMaterial(const RpGeometry * source,
                         RpGeometrySortByMaterialCallBack callback)
{
    RpGeometry         *newGeom = (RpGeometry *)NULL;
    RwPluginRegEntry   *entry = (RwPluginRegEntry *)NULL;
    RwBool              pluginData = FALSE;
    RwUInt32            flags = RpGeometryGetFlags(source);
    RwUInt32            format;
    RwBool              preLit, normals;
    SortVert           *sortVerts = (SortVert *)NULL;
    RwUInt16           *vertexMap = (RwUInt16 *)NULL;
    RwInt32             currentIndex;
    RwInt32             i, j;
    RwRGBA             *preLitSrc = (RwRGBA *)NULL, *preLitDest = (RwRGBA *)NULL;
    RwTexCoords        *uvSrc = (RwTexCoords *)NULL, *uvDest = (RwTexCoords *)NULL;
    RwV3d             **normalsSrc = (RwV3d **)NULL, **normalsDest = (RwV3d **)NULL;
    RwV3d             **posSrc = (RwV3d **)NULL, **posDest = (RwV3d **)NULL;
    RwInt32             numNewVerts;

    /* This sorts the vertices in a geometry by material, duplicating
     * vertices at material boundaries, and outputs a new RpGeometry */
    RWAPIFUNCTION(RWSTRING("RpGeometrySortByMaterial"));
    RWASSERT(source != NULL);

#if (0)
    numNewVerts = (RwInt32) (1.25f * source->numVertices);
#endif /* (0) */

    normalsSrc = (RwV3d **)
        RwMalloc(source->numMorphTargets * sizeof(RwV3d *),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    normalsDest = (RwV3d **)
        RwMalloc(source->numMorphTargets * sizeof(RwV3d *),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    posSrc = (RwV3d **)
        RwMalloc(source->numMorphTargets * sizeof(RwV3d *),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    posDest =(RwV3d **)
        RwMalloc(source->numMorphTargets * sizeof(RwV3d *),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    sortVerts = (SortVert *)
        RwMalloc(3 * source->numTriangles * sizeof(SortVert),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);

    if ((((flags & rpGEOMETRYNORMALS) == rpGEOMETRYNORMALS) &&
         ((normalsSrc == NULL) || (normalsDest == NULL))) ||
        (posSrc == NULL) || (posDest == NULL) || (sortVerts == NULL))
    {
        if (sortVerts != NULL)
            RwFree(sortVerts);
        if (posDest != NULL)
            RwFree(posDest);
        if (posSrc != NULL)
            RwFree(posSrc);
        if (normalsDest != NULL)
            RwFree(normalsDest);
        if (normalsSrc != NULL)
            RwFree(normalsSrc);
        RWRETURN((RpGeometry *)NULL);
    }

    /* Set up the sortVerts */
    currentIndex = 0;
    for (i = 0; i < source->numTriangles; i++)
    {
        RwInt16             matIndex = (source->triangles[i]).matIndex;

        for (j = 0; j < 3; j++)
        {
            sortVerts[currentIndex].matIndex = matIndex;
            sortVerts[currentIndex].origIndex =
                (source->triangles[i]).vertIndex[j];
            sortVerts[currentIndex].faceIndex = i | (j << 30);
            currentIndex++;
        }
    }

    /* Sort the sortVerts by material and then index (so sortVerts coming
         * from the same initial vertex are grouped together (they're created
         * by the triangles that share this vertex) and verts are sorted on
         * index within materials) */
    qsort(sortVerts, 3 * source->numTriangles, sizeof(SortVert),
          SortVertsByMaterialCB);

    /* Find out how many unique vertices there are (verts having been
     * duplicated at material boundaries) so we can allocate the new
     * geometry */
    currentIndex = -1;
    for (i = 0; i < 3 * source->numTriangles; i++)
    {
        SortVert           *curSortVert = &(sortVerts[i]);

        if ((i == 0) ||
            (SortVertsByMaterialCB
             ((void *) (curSortVert - 1), (void *) curSortVert) != 0))
        {
            /* We've found a new unique vertex */
            currentIndex++;
        }
    }
    numNewVerts = currentIndex + 1;

    vertexMap = (RwUInt16 *)
        RwMalloc((RwUInt32) (numNewVerts * sizeof(RwUInt16)),
                 rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    format = flags | rpGEOMETRYTEXCOORDSETS(source->numTexCoordSets);
    newGeom = RpGeometryLock(RpGeometryCreate(numNewVerts,
                                              source->numTriangles,
                                              format),
                             rpGEOMETRYLOCKALL);
    if ((vertexMap == NULL) || (newGeom == NULL))
    {
        RwFree(sortVerts);
        RwFree(posDest);
        RwFree(posSrc);
        if (vertexMap != NULL)
            RwFree(vertexMap);
        if (newGeom != NULL)
            RpGeometryDestroy(newGeom);

        RWRETURN((RpGeometry *)NULL);
    }

    /* Map new triangle faces to new vertices and generate a vertex map
         * to map new to old (to get at the original data when constructing
         * the new geometry) */
    currentIndex = -1;
    for (i = 0; i < 3 * source->numTriangles; i++)
    {
        RpTriangle         *triangle;
        SortVert           *curSortVert;
        RwUInt32            faceIndex, triVert;

        curSortVert = &(sortVerts[i]);

        /* The top 2 bits of sortVert->faceIndex contain (0, 1, 2), which
         * is the index into triangle->vertIndex[] */
        faceIndex = curSortVert->faceIndex;
        triVert = faceIndex & (3 << 30);
        faceIndex = faceIndex & ~triVert;
        triVert >>= 30;

        if ((i == 0) ||
            (SortVertsByMaterialCB
             ((void *) (curSortVert - 1), (void *) curSortVert) != 0))
        {
            /* We've found a new unique vertex */
            currentIndex++;
        }
        triangle = &(newGeom->triangles[faceIndex]);
        /* Remap the triangle to the new vertices */
        triangle->vertIndex[triVert] = (RwUInt16) currentIndex;
        triangle->matIndex = source->triangles[faceIndex].matIndex;
        vertexMap[currentIndex] = curSortVert->origIndex;
    }
    RwFree(sortVerts);
    sortVerts = (SortVert *)NULL;

    for (i = 0; i < (source->numMorphTargets - 1); i++)
    {
        RpGeometryAddMorphTarget(newGeom);
    }
    if (newGeom->numMorphTargets != source->numMorphTargets)
    {
        RpGeometryDestroy(newGeom);

        RwFree(posDest);
        RwFree(posSrc);
        RwFree(vertexMap);

        RWRETURN((RpGeometry *)NULL);
    }

    for (i = 0; i < source->numMorphTargets; i++)
    {
        posSrc[i] =
            RpMorphTargetGetVertices(RpGeometryGetMorphTarget(source, i));
        posDest[i] =
            RpMorphTargetGetVertices(RpGeometryGetMorphTarget(newGeom, i));
    }
    if ((flags & rpGEOMETRYNORMALS) == rpGEOMETRYNORMALS)
    {
        normals = TRUE;
        for (i = 0; i < source->numMorphTargets; i++)
        {
            normalsSrc[i] =
                RpMorphTargetGetVertexNormals
                (RpGeometryGetMorphTarget(source, i));
            normalsDest[i] =
                RpMorphTargetGetVertexNormals
                (RpGeometryGetMorphTarget(newGeom, i));
        }
    }
    else
    {
        normals = FALSE;
    }
    if ((flags & rpGEOMETRYPRELIT) == rpGEOMETRYPRELIT)
    {
        preLit = TRUE;
        preLitSrc = RpGeometryGetPreLightColors(source);
        preLitDest = RpGeometryGetPreLightColors(newGeom);
    }
    else
    {
        preLit = FALSE;
    }

    /* Copy data across using the remapping array */
    for (i = 0; i < numNewVerts; i++)
    {
        RwUInt32            origVertex = vertexMap[i];

        /* Copy data common to all keyframes */
        if (preLit)
        {
            preLitDest[i] = preLitSrc[origVertex];
        }

        for (j = 0; j < source->numMorphTargets; j++)
        {
            /* Copy data per morphtarget */
            posDest[j][i] = posSrc[j][origVertex];
            if (normals)
            {
                normalsDest[j][i] = normalsSrc[j][origVertex];
            }
        }
    }

    /* Copy across any texture coordinates */
    for (i = 0; i < source->numTexCoordSets; i++)
    {
        uvSrc = source->texCoords[i];
        uvDest = newGeom->texCoords[i];

        RWASSERT(uvSrc);
        RWASSERT(uvDest);

        for (j = 0; j < numNewVerts; j++)
        {
            uvDest[j] = uvSrc[vertexMap[j]];
        }
    }

    /* Calculate bounding spheres for newGeom's morphtarget(s) */
    for (i = 0; i < newGeom->numMorphTargets; i++)
    {
        RpMorphTarget      *curTarget;
        RwSphere            sphere;

        curTarget = RpGeometryGetMorphTarget(newGeom, i);
        RpMorphTargetCalcBoundingSphere(curTarget, &sphere);
        RpMorphTargetSetBoundingSphere(curTarget, &sphere);
    }

    RwFree(posDest);
    RwFree(posSrc);
    RwFree(normalsDest);
    RwFree(normalsSrc);
    /*RwFree(vertexMap); */

/*        newGeom->surfaceProps = source->surfaceProps;*/
    if (rpMaterialListCopy
        (&(newGeom->matList), &(source->matList)) == NULL)
    {
        RwFree(vertexMap);
        RpGeometryDestroy(newGeom);
        RWRETURN((RpGeometry *)NULL);
    }

    entry = _rpGeometryGetTKListFirstRegEntry();
    while (entry)
    {
        if (entry->size > 0)
        {
            pluginData = TRUE;
            memcpy(((RwUInt8 *) newGeom) + entry->offset,
                   ((const RwUInt8 *) source) + entry->offset,
                   entry->size);
        }
        entry = entry->nextRegEntry;
    }

#if (0)
    if (pluginData)
    {
        /* Since the vertex order/number and triangle indices in newGeom are
         * different from those in source, the plugin data in source may
         * not be correct when copied to newGeom */
        /* removed - as anyone calling this function
         * will know that things are going to be changed */

        RwDebugSendMessage(rwDEBUGMESSAGE,
                           "RpGeometryStreamWrite",
                           "Warning: sorting performed on geometry (vertex order and number changed, per-triangle vertex indices changed) and hence geometry plugin data not necessarily saved correctly. Reload sorted geometry from disk, correct its plugin data and export again if necessary.");
    }
#endif /* (0) */

    /* Call the user supplied callback function if one was given */
    if (callback != NULL)
    {
        callback(source, newGeom, vertexMap, (RwUInt16) numNewVerts);
    }

    RwFree(vertexMap);

    if (RpGeometryUnlock(newGeom) != NULL)
    {
        RWRETURN(newGeom);
    }
    else
    {
        /* So near and yet... */
        RpGeometryDestroy(newGeom);
        RWRETURN((RpGeometry *)NULL);
    }
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetDefaultSectorPipeline defines the global
 * default world sector object pipeline. The pipeline is executed from the
 * default render callback, unless overridden by a non-NULL world sector's
 * pipeline or world's pipeline, and indirectly from \ref RpWorldSectorRender
 * and \ref RpWorldRender) when a world sector enters the current camera's view
 * frustum, giving the application the opportunity to alter the way in which
 * static objects in the world sector are rendered.
 *
 * If the parameter is NULL then the default world sector object
 * pipeline will be reinstated (this is platform-specific).
 *
 * World sectors will use their specified pipeline, unless it is set to NULL, in
 * which case they will use their world's specified pipeline.. If the world's
 * pipeline is set to NULL then the global default world sector pipeline will
 * be used.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pipeline  Pointer to the pipeline or NULL.
 *
 * \return Returns pointer to the pipeline if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RxPipeline        *
RpWorldSetDefaultSectorPipeline(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSetDefaultSectorPipeline"));
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    if (NULL == pipeline)
    {
        if (NULL != RXPIPELINEGLOBAL(platformWorldSectorPipeline))
        {
            pipeline = RXPIPELINEGLOBAL(platformWorldSectorPipeline);
        }
        else
        {
            pipeline = NULL;
        }
    }

    RXPIPELINEGLOBAL(currentWorldSectorPipeline) = pipeline;
    RWASSERT(NULL != RXPIPELINEGLOBAL(currentWorldSectorPipeline));
    RWRETURN(pipeline);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetDefaultSectorPipeline retrieves the
 * global default world sector object pipeline. The pipeline is
 * executed from the default sector render callback, and indirectly from
 * \ref RpWorldSectorRender and \ref RpWorldRender) when a world sector
 * enters the current camera's view frustum.
 *
 * World sectors will use their specified pipeline, unless it is set to NULL,
 * in which case they will use their world's specified pipeline. If the world's
 * pipeline is set to NULL then the global default world sector pipeline will
 * be used.
 *
 * The generic form of the default world-sector object pipeline is
 * detailed in \ref RpWorldGetGenericSectorPipeline and
 * platform-specific versions in
 * \ref RpWorldGetDefaultSectorPipelineplatform
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns pointer to the pipeline if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 */
RxPipeline *
RpWorldGetDefaultSectorPipeline(void)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetDefaultSectorPipeline"));
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /*  (defined(DUNROAMIN)) */

    RWRETURN(RpWorldGetDefaultSectorPipelineMacro());
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldSetSectorPipeline defines the default
 * world sector object pipeline for the specified world. The pipeline
 * is executed from the default render callback and indirectly from
 * \ref RpWorldSectorRender and \ref RpWorldRender) when a world sector enters
 * the current camera's view frustum, giving the application the opportunity
 * to alter the way in which static objects in the world sector are rendered.
 *
 * World sectors in the current world will use this pipeline by default,
 * unless they have their own pipeline specified. If the world's pipeline
 * is set to NULL then the global default world sector pipeline will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param pipeline  Pointer to the pipeline.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 *
 */
RpWorld *
RpWorldSetSectorPipeline(RpWorld *world, RxPipeline *pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSetSectorPipeline"));
    RWASSERT(world);
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpWorldSetSectorPipelineMacro(world, pipeline));
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldGetSectorPipeline returns the default world
 * sector object pipeline for the specified world. The pipeline
 * is executed from the default render callback and indirectly from
 * \ref RpWorldSectorRender and \ref RpWorldRender) when a world sector enters
 * the current camera's view frustum.
 *
 * World sectors in the current world will use this pipeline by default,
 * unless they have their own pipeline specified. If the world's pipeline
 * is NULL, this signifies that the global default world sector pipeline
 * will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 * \param pipeline  Pipeline pointer to receive the world's pipeline pointer.
 *
 * \return Returns pointer to the world if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
  *
 */
RpWorld *
RpWorldGetSectorPipeline(RpWorld *world, RxPipeline **pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpWorldGetSectorPipeline"));
    RWASSERT(world);
    RWASSERT(pipeline);
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpWorldGetSectorPipelineMacro(world, pipeline));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorSetPipeline defines the object
 * pipeline for the specified world sector. The pipeline is executed from the
 * default render callback and indirectly from \ref RpWorldSectorRender and
 * \ref RpWorldRender) when a world sector enters the current camera's view
 * frustum, giving the application the opportunity to alter the way in which
 * static objects in the world sector are rendered.
 *
 * The world sector will use this pipeline, unless it is set to NULL, in
 * which case the current world's pipeline will be used. If the world's
 * pipeline is set to NULL then the global default world sector pipeline
 * will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 * \param pipeline  Pointer to the pipeline.
 *
 * \return Returns pointer to the world sector if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 */
RpWorldSector *
RpWorldSectorSetPipeline(RpWorldSector *sector, RxPipeline *pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorSetPipeline"));
    RWASSERT(sector);
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWASSERTISTYPE(sector, rpWorldSector); /* Is this valid?? */

    RWRETURN(RpWorldSectorSetPipelineMacro(sector, pipeline));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetPipeline returns the object pipeline
 * for the specified world sector. The pipeline is executed from the default
 * render callback and indirectly from \ref RpWorldSectorRender and
 * \ref RpWorldRender) when a world sector enters the current camera's view
 * frustum.
 *
 * The world sector will use this pipeline, unless it is set to NULL, in
 * which case the current world's pipeline will be used. If the world's
 * pipeline is set to NULL then the global default world sector pipeline
 * will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 * \param pipeline  Pointer to receive the pipeline.
 *
 * \return Returns pointer to the world sector if successful or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 */
RpWorldSector *
RpWorldSectorGetPipeline(RpWorldSector *sector, RxPipeline **pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetPipeline"));
    RWASSERT(sector);
    RWASSERT(pipeline);
#if (defined(DUNROAMIN))
    RWASSERT(worldModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWASSERTISTYPE(sector, rpWorldSector); /* Is this valid?? */

    RWRETURN(RpWorldSectorGetPipelineMacro(sector, pipeline));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetDefaultPipeline determines the default
 * object pipeline used by atomics. The pipeline has a modular structure
 * and contains operations necessary for taking potentially compressed
 * objects within RenderWare and decompressing them and caching this
 * decompressed (instanced) representation for use over multiple frames.
 *
 * If the parameter is NULL, the default default atomic object pipeline
 * is reinstated.
 *
 * If an atomic's pipeline is NULL, the default Atomic pipeline will be used.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pipeline  Pointer to the pipeline or NULL.
 *
 * \return Returns pointer to the pipeline if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RxPipeline        *
RpAtomicSetDefaultPipeline(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetDefaultPipeline"));
#if (defined(DUNROAMIN))
    RWASSERT(clumpModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    if (NULL == pipeline)
    {
        if (RXPIPELINEGLOBAL(platformAtomicPipeline) != NULL)
        {
            pipeline = RXPIPELINEGLOBAL(platformAtomicPipeline);
        }
        else
        {
            pipeline = NULL;
        }
    }

    RXPIPELINEGLOBAL(currentAtomicPipeline) = pipeline;
    RWASSERT(NULL != RXPIPELINEGLOBAL(currentAtomicPipeline));
    RWRETURN(pipeline);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetDefaultPipeline retrieves the default
 * object pipeline used by atomics. The pipeline has a modular structure
 * and contains operations necessary for taking potentially compressed
 * objects within RenderWare and decompressing them and caching this
 * decompressed (instanced) representation for use over multiple frames.
 *
 * If an atomic's pipeline is NULL, the default Atomic pipeline will be used.
 *
 * The generic form of the default atomic object pipeline is
 * detailed in \ref RpAtomicGetGenericPipeline and
 * platform-specific versions in
 * \ref RpAtomicGetDefaultPipeline
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns pointer to the pipeline if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RxPipeline *
RpAtomicGetDefaultPipeline(void)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetDefaultPipeline"));
#if (defined(DUNROAMIN))
    RWASSERT(clumpModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpAtomicGetDefaultPipelineMacro());
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetPipeline determines the object
 * pipeline for the specified atomic. The pipeline has a modular structure
 * and contains operations necessary for taking potentially compressed
 * objects within RenderWare and decompressing them and caching this
 * decompressed (instanced) representation for use over multiple frames.
 *
 * If the atomic's pipeline is NULL, the default Atomic pipeline will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param pipeline  Pointer to the pipeline.
 *
 * \return Returns pointer to the atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RpAtomic *
RpAtomicSetPipeline(RpAtomic *atomic, RxPipeline *pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetPipeline"));
    RWASSERT(atomic);
#if (defined(DUNROAMIN))
    RWASSERT(clumpModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(RpAtomicSetPipelineMacro(atomic, pipeline));
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetPipeline retrieves the object
 * pipeline for the specified atomic. The pipeline has a modular structure
 * and contains operations necessary for taking potentially compressed
 * objects within RenderWare and decompressing them and caching this
 * decompressed (instanced) representation for use over multiple frames.
 *
 * If the atomic's pipeline is NULL, the default Atomic pipeline will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param pipeline  Pointer to receive the pipeline.
 *
 * \return Returns pointer to the atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
const RpAtomic *
RpAtomicGetPipeline(const RpAtomic *const atomic,
                    RxPipeline **pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetPipeline"));
    RWASSERT(atomic);
    RWASSERT(pipeline);
#if (defined(DUNROAMIN))
    RWASSERT(clumpModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(RpAtomicGetPipelineMacro(atomic, pipeline));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetDefaultPipeline determines the default
 * pipeline for materials. This is the pipeline which will be
 * used to render objects referencing a given material when that
 * material's pipeline pointer is set to NULL.
 *
 * If the parameter is NULL, the default default material pipeline is
 * reinstated.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pipeline  Pointer to the pipeline or NULL.
 *
 * \return Returns a pointer to the pipeline is successful, or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RxPipeline        *
RpMaterialSetDefaultPipeline(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialSetDefaultPipeline"));

#if (defined(DUNROAMIN))
    RWASSERT(materialModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    if(NULL == pipeline)
    {
        if (NULL != RXPIPELINEGLOBAL(platformMaterialPipeline))
        {
            pipeline = RXPIPELINEGLOBAL(platformMaterialPipeline);
        }
        else
        {
            pipeline = NULL;
        }
    }

    RXPIPELINEGLOBAL(currentMaterialPipeline) = pipeline;
    RWASSERT(NULL != RXPIPELINEGLOBAL(currentMaterialPipeline));
    RWRETURN(pipeline);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetDefaultPipeline retrieves the default
 * pipeline for materials. This is the pipeline which will be
 * used to render objects referencing a given material when that
 * material's pipeline pointer is set to NULL.
 *
 * The generic form of the default material pipeline is
 * detailed in \ref RpMaterialGetGenericPipeline and
 * platform-specific versions in
 * \ref RpMaterialGetDefaultPipeline
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns a pointer to the pipeline is successful, or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldSetDefaultSectorPipeline
 */
RxPipeline *
RpMaterialGetDefaultPipeline(void)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetDefaultPipeline"));
#if (defined(DUNROAMIN))
    RWASSERT(materialModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpMaterialGetDefaultPipelineMacro());
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetPipeline determines the pipeline for the
 * specified material. This is the pipeline which will be
 * used to render objects referencing the material. If the material's
 * pipeline pointer is set to NULL, the default material pipeline
 * will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 * \param pipeline  Pointer to the pipeline.
 *
 * \return Returns a pointer to the material is successful, or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialGetPipeline
 * \see RpMaterialSetDefaultPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RpMaterial *
RpMaterialSetPipeline(RpMaterial *material,
                      RxPipeline *pipeline)

{
    RWAPIFUNCTION(RWSTRING("RpMaterialSetPipeline"));
    RWASSERT(material);
#if (defined(DUNROAMIN))
    RWASSERT(materialModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpMaterialSetPipelineMacro(material, pipeline));
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetPipeline retrieves the pipeline for the
 * specified material. This is the pipeline which will
 * be used to render objects referencing the material. If the material's
 * pipeline pointer is set to NULL, the default material pipeline
 * will be used.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 * \param pipeline  Pointer to receive the material's pipeline.
 *
 * \return Returns a pointer to the material is successful, or NULL if there
 * is an error.
 *
 * \see RpAtomicGetGenericPipeline
 * \see RpAtomicGetDefaultPipeline
 * \see \ref RpAtomicGetDefaultPipelineplatform
 * \see RpAtomicGetPipeline
 * \see RpAtomicSetDefaultPipeline
 * \see RpAtomicSetPipeline
 * \see RpMaterialGetGenericPipeline
 * \see RpMaterialGetDefaultPipeline
 * \see \ref RpMaterialGetDefaultPipelineplatform
 * \see RpMaterialSetDefaultPipeline
 * \see RpMaterialSetPipeline
 * \see RpWorldGetGenericSectorPipeline
 * \see RpWorldGetDefaultSectorPipeline
 * \see \ref RpWorldGetDefaultSectorPipelineplatform
 * \see RpWorldGetSectorPipeline
 * \see RpWorldSectorGetPipeline
 * \see RpWorldSectorSetPipeline
 * \see RpWorldSetDefaultSectorPipeline
 * \see RpWorldSetSectorPipeline
 *
 */
RpMaterial *
RpMaterialGetPipeline(RpMaterial *material,
                      RxPipeline **pipeline)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetPipeline"));
    RWASSERT(material);
    RWASSERT(pipeline);
#if (defined(DUNROAMIN))
    RWASSERT(materialModule.numInstances);
#endif /* (defined(DUNROAMIN)) */

    RWRETURN(RpMaterialGetPipelineMacro(material, pipeline));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/****************************************************************************
 _rpWorldPipelineClose
 */
void
_rpWorldPipelineClose(void)
{
    RWFUNCTION(RWSTRING("_rpWorldPipelineClose"));

#if (((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))
    _rwfastIm3DPipesClose();
#endif /*(((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))*/

    _rpDestroyPlatformWorldSectorPipelines();
    _rpDestroyPlatformAtomicPipelines();
    _rpDestroyPlatformMaterialPipelines();

    RWRETURNVOID();
}

/****************************************************************************
 _rpWorldPipelineOpen
 */
RwBool
_rpWorldPipelineOpen(void)
{
    RwBool success = TRUE;

    RWFUNCTION(RWSTRING("_rpWorldPipelineOpen"));

    /* NOTE: these create-default funcs are device specific.
     *       (e.g the PS2 build creates several different versions
     *       of atomic/worldsector pipe) */

    /* These funcs initialise the global pipe pointers as appropriate,
       start by initializing them all to NULL */
    RXPIPELINEGLOBAL(currentAtomicPipeline) = NULL;
    RXPIPELINEGLOBAL(currentWorldSectorPipeline) = NULL;
    RXPIPELINEGLOBAL(currentMaterialPipeline) = NULL;
    RXPIPELINEGLOBAL(genericAtomicPipeline) = NULL;
    RXPIPELINEGLOBAL(genericWorldSectorPipeline) = NULL;
    RXPIPELINEGLOBAL(genericMaterialPipeline) = NULL;
    RXPIPELINEGLOBAL(platformAtomicPipeline) = NULL;
    RXPIPELINEGLOBAL(platformWorldSectorPipeline) = NULL;
    RXPIPELINEGLOBAL(platformMaterialPipeline) = NULL;

    /* These funcs may set up platform-specific pipelines and
     * set them as the defaults */
    success = _rpCreatePlatformMaterialPipelines();
    if (success != FALSE) success = _rpCreatePlatformAtomicPipelines();
    if (success != FALSE) success = _rpCreatePlatformWorldSectorPipelines();

#if (((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))
    /* Temporary overloading of Im3D pipes to use PS2All
     * (interim quicken-up-ening solution - this and PS2All
     * will end up in the core, not RpWorld) */
    if (success != FALSE) success = _rwfastIm3DPipesOpen();
#endif /*(((defined(SKY) || defined(SKY2_DRVMODEL_H)) && !defined(NULLSKY_DRVMODEL_H)))*/

    if (success != FALSE)
    {
        RWRETURN(TRUE);
    }

    RwDebugSendMessage(rwDEBUGMESSAGE, "_rpWorldPipelineOpen",
                       "Failed to create platform-specific default atomic/world-sector/material pipelines");

    /* This ought to safely clean up */
    _rpWorldPipelineClose();

    RWRETURN(FALSE);
}

/****************************************************************************
 rpWorldPipeAttach
 */
RwBool
_rpWorldPipeAttach(void)
{
    RWFUNCTION(RWSTRING("_rpWorldPipeAttach"));

    RWRETURN(TRUE);
}

