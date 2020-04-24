
/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include <rwcore.h>

#include "basector.h"
#include "baworld.h"
#include "bamateri.h"
#include "bamatlst.h"
#include "babinwor.h"

/****************************************************************************
 Local Types
 */
#if !(rwLIBRARYVERSION36002 < rwLIBRARYBASEVERSION)
typedef struct rpPolygon rpPolygon;
struct rpPolygon
{
    RwUInt16        matIndex;
    RwUInt16        vertIndex[3];
};
#endif /* !(rwLIBRARYVERSION36002 < rwLIBRARYBASEVERSION) */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWSTREAMTYPE(_type)                                              \
  ( ( rwNASTREAM == (_type) ) ? "rwNASTREAM" :                           \
    ( ( rwSTREAMFILE == (_type) ) ? "rwSTREAMFILE" :                     \
      ( ( rwSTREAMFILENAME == (_type) ) ? "rwSTREAMFILENAME" :           \
        ( ( rwSTREAMMEMORY == (_type) ) ? "rwSTREAMMEMORY" : "Unknown" ) \
        ) ) )

/*
 * World format (as stored in the binary stream):-
 *  0xg000nngg, nn = num tex coords, gg = flags
 * See also baworld.h: rpWORLDTEXCOORDSETS(_num)
 *
 * To determine number of texture coordinate sets, the 'num tex coords'
 * field takes precedence, but if zero, we use the backward compatibility
 * flags.
 */
#define WorldFormatGetNumTexCoordSetsMacro(_fmt)            \
    (((_fmt) & 0xff0000) ? (((_fmt) & 0xff0000)>>16) :      \
        (((_fmt) & rpGEOMETRYTEXTURED2) ? 2 :               \
            (((_fmt) & rpGEOMETRYTEXTURED) ? 1 : 0)))

#define WorldFormatGetFlagsMacro(_fmt) ((_fmt) & 0xff

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo binWorldModule;


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Rights support

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwUInt32 lastSeenWorldRightsPluginId;
static RwUInt32 lastSeenWorldExtraData;

RwStream*
_rpReadWorldRights(RwStream *s,
                   RwInt32 len,
                   void *obj __RWUNUSED__,
                   RwInt32 off __RWUNUSED__,
                   RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpReadWorldRights"));

    RWASSERT((len == 4) || (len == 8));

    if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenWorldRightsPluginId),
                           sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (len == 8)
    {
        if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenWorldExtraData),
                               sizeof(RwInt32)))
        {
            RWRETURN((RwStream *) NULL);
        }
    }
    RWRETURN(s);
}

RwStream*
_rpWriteWorldRights(RwStream *s,
                    RwInt32 len __RWUNUSED__,
                    const void *obj,
                    RwInt32 off __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    const RpWorld *wrl;

    RWFUNCTION(RWSTRING("_rpWriteWorldRights"));
    RWASSERT(obj);

    wrl = (const RpWorld*)obj;
    RWASSERT(wrl->pipeline);
    RWASSERT(wrl->pipeline->pluginId);

    if (!RwStreamWriteInt32(s, (RwInt32*)&(wrl->pipeline->pluginId),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (!RwStreamWriteInt32(s, (RwInt32*)&(wrl->pipeline->pluginData),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }

    RWRETURN(s);
}

RwInt32
_rpSizeWorldRights(const void *obj,
                   RwInt32 off __RWUNUSED__,
                   RwInt32 size __RWUNUSED__)
{
    const RpWorld *wrl;

    RWFUNCTION(RWSTRING("_rpSizeWorldRights"));
    RWASSERT(obj);

    wrl = (const RpWorld*)obj;
    if ((wrl->pipeline) && (wrl->pipeline->pluginId))
    {
        RWRETURN(sizeof(RwUInt32) * 2);
    }
    else
    {
        RWRETURN(0);
    }
}

static RwUInt32 lastSeenSectRightsPluginId;
static RwUInt32 lastSeenSectExtraData;

RwStream*
_rpReadSectRights(RwStream *s,
                  RwInt32 len,
                  void *obj __RWUNUSED__,
                  RwInt32 off __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpReadSectRights"));

    RWASSERT((len == 4) || (len == 8));

    if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenSectRightsPluginId),
                           sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (len == 8)
    {
        if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenSectExtraData),
                               sizeof(RwInt32)))
        {
            RWRETURN((RwStream *) NULL);
        }
    }
    RWRETURN(s);
}

RwStream*
_rpWriteSectRights(RwStream *s,
                   RwInt32 len __RWUNUSED__,
                   const void *obj,
                   RwInt32 off __RWUNUSED__,
                   RwInt32 size __RWUNUSED__)
{
    const RpWorldSector *sect;

    RWFUNCTION(RWSTRING("_rpWriteSectRights"));
    RWASSERT(obj);

    sect = (const RpWorldSector*)obj;
    RWASSERT(sect->pipeline);
    RWASSERT(sect->pipeline->pluginId);

    if (!RwStreamWriteInt32(s, (RwInt32*)&(sect->pipeline->pluginId),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (!RwStreamWriteInt32(s, (RwInt32*)&(sect->pipeline->pluginData),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }

    RWRETURN(s);
}

RwInt32
_rpSizeSectRights(const void *obj,
                  RwInt32 off __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    const RpWorldSector *sect;

    RWFUNCTION(RWSTRING("_rpSizeSectRights"));
    RWASSERT(obj);

    sect = (const RpWorldSector*)obj;
    if ((sect->pipeline) && (sect->pipeline->pluginId))
    {
        RWRETURN(sizeof(RwUInt32) * 2);
    }
    else
    {
        RWRETURN(0);
    }
}

/****************************************************************************
 Functions
 */

/****************************************************************************
 WorldSectorStreamGetSizeActual

 On entry   : world sector
 On exit    : size of world sector actual (without extension chunks)
 */

static RwUInt32
WorldSectorStreamGetSizeActual(const RpWorldSector *worldSector,
                               const RpWorld *world,
                               RwUInt32 flags)
{
    RwUInt32    size;

    RWFUNCTION(RWSTRING("WorldSectorStreamGetSizeActual"));
    RWASSERT(worldSector);

    /* Structure header */
    size = sizeof(_rpWorldSector);

    if (!(rpWORLDNATIVE & RpWorldGetFlags(world)))
    {
        /* Vertices */
        size += sizeof(RwV3d) * worldSector->numVertices;

        /* Normals */
        if (flags & rpWORLDNORMALS)
        {
            size += sizeof(RpVertexNormal) * worldSector->numVertices;
        }

        /* PreLitLums */
        if (flags & rpWORLDPRELIT)
        {
            size += sizeof(RwRGBA) * worldSector->numVertices;
        }

        /* Texture coordinates  - written as floats */
        size += sizeof(RwTexCoords) * worldSector->numVertices
                * world->numTexCoordSets;

        /* Polygons */
        size += sizeof(RpTriangle) * worldSector->numTriangles;
    }

    RWRETURN(size);
}

/****************************************************************************
 WorldSectorStreamGetSize

 On entry   :
 On exit    : Size of Binary worldSector
 */

static RwUInt32
WorldSectorStreamGetSize(const RpWorldSector * worldSector, const
                         RpWorld * world, RwUInt32 flags)
{
    RwUInt32            size;

    RWFUNCTION(RWSTRING("WorldSectorStreamGetSize"));
    RWASSERT(worldSector);

    size = WorldSectorStreamGetSizeActual(worldSector, world,
                                          flags) + rwCHUNKHEADERSIZE;

    /* Plugin data size */
    size +=
        rwPluginRegistryGetSize(&sectorTKList,
                                worldSector) + rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/****************************************************************************
 WorldSectorStreamWrite

 On entry   : Stream to write to
 On exit    :
 */

static const RpWorldSector *
WorldSectorStreamWrite(const RpWorldSector *worldSector,
                       RwStream *stream,
                       const RpWorld *world,
                       RwUInt32 flags)
{
    _rpWorldSector  as;

    RWFUNCTION(RWSTRING("WorldSectorStreamWrite"));
    RWASSERT(worldSector);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_ATOMICSECT,
                                  WorldSectorStreamGetSize
                                  (worldSector, world, flags)))
    {
        RWRETURN((const RpWorldSector *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT,
                                  WorldSectorStreamGetSizeActual
                                  (worldSector, world, flags)))
    {
        RWRETURN((const RpWorldSector *)NULL);
    }

    /* Fill it - don't forget fancy way of assigning floats */
    as.matListWindowBase = worldSector->matListWindowBase;
    as.numTriangles = worldSector->numTriangles;
    as.numVertices = worldSector->numVertices;
    RwV3dAssign(&as.inf, &worldSector->tightBoundingBox.inf);
    RwV3dAssign(&as.sup, &worldSector->tightBoundingBox.sup);

    /* 'Loose' bounding boxes not saved. They are dynamically generated
     * on load.
     */

    as.unused = FALSE;

    /* Convert it */
    (void)RwMemRealToFloat32(&as.inf, sizeof(as.inf));
    (void)RwMemRealToFloat32(&as.sup, sizeof(as.sup));
    (void)RwMemLittleEndian32(&as, sizeof(as));

    /* Write it */
    if (!RwStreamWrite(stream, &as, sizeof(as)))
    {
        RWRETURN((const RpWorldSector *)NULL);
    }

    if (!(rpWORLDNATIVE & RpWorldGetFlags(world)))
    {
        if (worldSector->numVertices)
        {
            RwInt32 vertexSize = sizeof(RwV3d) * worldSector->numVertices;

            /* Write off vertices - we always have these */
            /* Don't forget they are reals - they need converting */
            if (!RwStreamWriteReal(stream, (RwReal *)
                                   worldSector->vertices, vertexSize))
            {
                RWRETURN((const RpWorldSector *)NULL);
            }

            /* Write off normals - conditional on whether the world has these */
            if (flags & rpWORLDNORMALS)
            {
                RwInt32 normalSize = sizeof(RpVertexNormal) *
                                     worldSector->numVertices;

                /* These are made of chars - don't need converting */
                if (!RwStreamWrite(stream, worldSector->normals, normalSize))
                {
                    RWRETURN((const RpWorldSector *)NULL);
                }
            }

            /* Write off prelighting info -
             * conditional on whether the world has these */
            if (flags & rpWORLDPRELIT)
            {
                RwInt32 preLitLumSize = sizeof(RwRGBA) *
                                        worldSector->numVertices;

                /* Effectively RwUint8 [] */
                if (!RwStreamWrite(stream, (void *)
                                   worldSector->preLitLum, preLitLumSize))
                {
                    RWRETURN((const RpWorldSector *)NULL);
                }
            }

            /* Write off vertex texture coordinates
             * - conditional on whether the world has these */
            if (world->numTexCoordSets > 0)
            {
                RwInt32 i;
                RwInt32 vertexTexCoordSize;

                vertexTexCoordSize = sizeof(RwTexCoords) *
                                     worldSector->numVertices;

                for (i = 0; i < world->numTexCoordSets; i++)
                {
                    RWASSERT(worldSector->texCoords[i]);

                    if (!RwStreamWriteReal(stream,
                                           (RwReal *)worldSector->texCoords[i],
                                           vertexTexCoordSize))
                    {
                        RWRETURN((const RpWorldSector *)NULL);
                    }
                }
            }
        }

        /* Write off triangles */
        if (worldSector->numTriangles)
        {
            RwInt32 triangleSize = sizeof(RpTriangle) * worldSector->numTriangles;

            /* Write off triangles - we always have these */
            /* RpTriangle's consist of an array of 3 RwUInt16's and a RwUInt16 */
            if (!RwStreamWriteInt16(stream,
                                    (RwInt16 *)worldSector->triangles,
                                    triangleSize))
            {
                RWRETURN((const RpWorldSector *)NULL);
            }
        }
    }

    /* write out the plugin info */
    if (!rwPluginRegistryWriteDataChunks(&sectorTKList, stream, worldSector))
    {
        /* Failed to read extension data */
        RWRETURN((const RpWorldSector *)NULL);
    }

    RWRETURN(worldSector);
}

/****************************************************************************
 BinaryWorldMalloc

 This selects whether to draw memory form a malloc pool,
 or to RwMalloc some memory

 On entry   : Address binary world memory pointer, size of memory to allocate
 On exit    : Pointer to memory allocated
 */

static void        *
BinaryWorldMalloc(RwUInt8 ** binaryWorldMallocAddr, RwInt32 size)
{
    void               *pMemory;

    RWFUNCTION(RWSTRING("BinaryWorldMalloc"));

    /* We should allocate from the pool of pre-allocated memory */
    pMemory = (void *) (*binaryWorldMallocAddr);
    *binaryWorldMallocAddr += size;

    RWRETURN(pMemory);
}

#ifdef RWDEBUG

static const RpWorldSector *
WorldSectorIsCorrectlySorted(const RpWorldSector * sector, RwBool * result)
{
    /* For PowerPipe, we need to check that the data is in a valid arrangement.
     * A valid arrangement is such that for each mesh (group of triangles
     * referencing the same material) in the object, the vertices that its
     * triangles reference are in a contiguous block, disjoint with all such
     * blocks for other meshes in the object.
     * This test calculates the min/max indices for each mesh and then sums up
     * (1 + maxIndex - minIndex) for all meshes. This should be equal to
     * the total number of vertices if the data is arranged correctly. */
    RWFUNCTION(RWSTRING("WorldSectorIsCorrectlySorted"));
    RWASSERT(sector != NULL);

    if (sector != NULL)
    {
        struct MatIndexBounds
        {
                RwUInt16            minIndex;
                RwUInt16            maxIndex;
        };
        struct MatIndexBounds *matIndexBounds;
        RwInt32             sumOfMeshVertIndexRanges;
        RwUInt16            vertIndex;
        RwInt16             matIndex;
        RwInt32             i, j;
        RwUInt32            bytes;
        RwInt32             numMaterials;
        RpWorld            *world;

        world = RpWorldSectorGetWorld(sector);
        RWASSERT(world);

        numMaterials = RpWorldGetNumMaterials(world);
        if (numMaterials <= 1)
        {
            *result = TRUE;
            RWRETURN(sector);
        }

        bytes = numMaterials * sizeof(struct MatIndexBounds);

        matIndexBounds = (struct MatIndexBounds *) RwMalloc(bytes,
            rwMEMHINTDUR_FUNCTION | rwID_BINWORLDMODULE);

        if (NULL == matIndexBounds)
        {
            RWERROR((E_RW_NOMEM, (bytes)));
            RWRETURN((const RpWorldSector *)NULL);
        }

        for (i = 0; i < numMaterials; i++)
        {
            matIndexBounds[i].minIndex = 65535;
            matIndexBounds[i].maxIndex = 0;
        }

        for (i = 0; i < sector->numTriangles; i++)
        {
            matIndex = (sector->triangles)[i].matIndex;
            RWASSERT(matIndex < numMaterials);
            for (j = 0; j < 3; j++)
            {
                vertIndex = (sector->triangles)[i].vertIndex[j];
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
        sumOfMeshVertIndexRanges = 0;
        for (i = 0; i < numMaterials; i++)
        {
            sumOfMeshVertIndexRanges += 1 + matIndexBounds[i].maxIndex -
                matIndexBounds[i].minIndex;
        }

        RwFree(matIndexBounds);

        if (sumOfMeshVertIndexRanges > sector->numVertices)
        {
            /* Some meshes' ranges must overlap */
            *result = FALSE;
            RWRETURN(sector);
        }
        else
        {
            /* This geometry is correctly sorted for rendering by PowerPipe */
            *result = TRUE;
            RWRETURN(sector);
        }
    }
    else
    {
        RWRETURN((const RpWorldSector *)NULL);
    }
}

#endif /* RWDEBUG */


/****************************************************************************
 WorldSectorStreamRead

 On entry   : Stream to read from
 On exit    : worldSector created
 */
static RpWorldSector *
WorldSectorStreamRead(RwStream * stream,
                      RwUInt8 ** binaryWorldMallocAddr,
                      RpWorld * world,
                      RwUInt32  flags)
{
    RpWorldSector      *worldSector;
    _rpWorldSector      as;
    RwUInt32            version;
    RwInt32             i;
    RwBool              result;

    RWFUNCTION(RWSTRING("WorldSectorStreamRead"));
    RWASSERT(stream);

    result = TRUE;

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           (RwUInt32 *)NULL, &version))
    {
        RWRETURN((RpWorldSector *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpWorldSector *)NULL);
    }

    /* Read it */
    if (RwStreamRead(stream, &as, sizeof(as)) != sizeof(as))
    {
        RWRETURN((RpWorldSector *)NULL);
    }

    /* Convert it */
    (void)RwMemNative32(&as, sizeof(as));
    (void)RwMemFloat32ToReal(&as.inf, sizeof(as.inf));
    (void)RwMemFloat32ToReal(&as.sup, sizeof(as.sup));

    /* Grab some memory */
    worldSector = (RpWorldSector *)
        BinaryWorldMalloc(binaryWorldMallocAddr,
                          sectorTKList.sizeOfStruct);
    if (!worldSector)
    {
        RWERROR((E_RW_NOMEM, (sectorTKList.sizeOfStruct)));
        RWRETURN((RpWorldSector *)NULL);
    }

    /* Copy over values */
    worldSector->type = rwSECTORATOMIC;
    worldSector->tightBoundingBox.inf = as.inf;
    worldSector->tightBoundingBox.sup = as.sup;

    /* The 'Loose' bounding boxes are dynamically generated
     * from the BSP planes at a later stage.
     */

    worldSector->matListWindowBase = (RwInt16) as.matListWindowBase;
    worldSector->numTriangles = (RwInt16) as.numTriangles;
    worldSector->numVertices = (RwInt16) as.numVertices;
    worldSector->triangles = (RpTriangle *)NULL;
    worldSector->vertices = (RwV3d *)NULL;
    worldSector->preLitLum = (RwRGBA *)NULL;
    worldSector->normals = (RpVertexNormal *)NULL;
    memset(worldSector->texCoords, 0, sizeof(RwTexCoords *) * rwMAXTEXTURECOORDS);
    worldSector->pipeline = (RxPipeline *)NULL;

    /* Not instanced !!! */
    worldSector->repEntry = (RwResEntry *)NULL;
    worldSector->mesh = (RpMeshHeader *)NULL;

    /* Set so it contains nothing */
    rwLinkListInitialize(&worldSector->collAtomicsInWorldSector);
    rwLinkListInitialize(&worldSector->lightsInWorldSector);

    if (!(rpWORLDNATIVE & RpWorldGetFlags(world)))
    {
        if (as.numVertices)
        {
            RwInt32             vertexSize = sizeof(RwV3d) * as.numVertices;

            /* Memory allocation draws from the pre-allocated pool, or RwMallocs
             * a separate buffer as necessary
             */
            worldSector->vertices = (RwV3d *)
                BinaryWorldMalloc(binaryWorldMallocAddr, vertexSize);
            if (!worldSector->vertices)
            {
                /* Deallocation is performed in one block when we return */
                RWERROR((E_RW_NOMEM, vertexSize));
                result = FALSE;
            }

            /* Read off vertices */
            /* Don't forget they are reals, so need converting */
            if (result && (!RwStreamReadReal(stream, (RwReal *)
                                  worldSector->vertices, vertexSize)))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }

            if (result && (flags & rpWORLDNORMALS))
            {
                RwInt32             normalSize =

                    sizeof(RpVertexNormal) * as.numVertices;

                worldSector->normals = (RpVertexNormal *)
                    BinaryWorldMalloc(binaryWorldMallocAddr, normalSize);
                if (!worldSector->normals)
                {
                    /* Deallocation is performed in one block when we return */
                    RWERROR((E_RW_NOMEM, normalSize));
                    result = FALSE;
                }

                /* These are made of chars - don't need converting */
                if (result && (RwStreamRead(stream, worldSector->normals, normalSize)
                    != (RwUInt32) normalSize))
                {
                    /* Deallocation is performed in one block when we return */
                    result = FALSE;
                }
            }

            /* Read off Prelighting info
             * - conditional on whether the world has these */
            if (result && (flags & rpWORLDPRELIT))
            {
                RwInt32             preLitLumSize =

                    sizeof(RwRGBA) * as.numVertices;

                worldSector->preLitLum = (RwRGBA *)
                    BinaryWorldMalloc(binaryWorldMallocAddr, preLitLumSize);
                if (!worldSector->preLitLum)
                {
                    /* Deallocation is performed in one block when we return */
                    RWERROR((E_RW_NOMEM, preLitLumSize));
                    result = FALSE;
                }

                if (result && (!RwStreamRead
                    (stream, (void *) worldSector->preLitLum, preLitLumSize)))
                {
                    /* Deallocation is performed in one block when we return */
                    result = FALSE;
                }
            }

            /* Read off vertex texture cooords
             * - conditional on whether the world has these */
            if (result && (world->numTexCoordSets > 0))
            {
                RwInt32     texCoordSize;

                texCoordSize = sizeof(RwTexCoords) * worldSector->numVertices;

                for (i = 0; i < world->numTexCoordSets; i++)
                {
                    worldSector->texCoords[i] = (RwTexCoords *)
                        BinaryWorldMalloc(binaryWorldMallocAddr, texCoordSize);
                    if (result && (!worldSector->texCoords[i]))
                    {
                        /* Deallocation is performed in one block when we return */
                        RWERROR((E_RW_NOMEM, texCoordSize));
                        result = FALSE;
                        break;
                    }

                    if (result && (!RwStreamReadReal(stream,
                            (RwReal *) worldSector->texCoords[i], texCoordSize)))
                    {
                        /* Deallocation is performed in one block when we return */
                        result = FALSE;
                        break;
                    }
                }
            }
        }

        if (result && as.numTriangles)
        {
            RwInt32 triangleSize = sizeof(RpTriangle) * as.numTriangles;

            /* Allocate all the memory first */
            worldSector->triangles = (RpTriangle *)
                BinaryWorldMalloc(binaryWorldMallocAddr, triangleSize);
            if (!worldSector->triangles)
            {
                /* Deallocation is performed in one block when we return */
                RWERROR((E_RW_NOMEM, triangleSize));
                result = FALSE;
            }

            /* Read off triangles */
            if (result && (!RwStreamReadInt16(stream, (RwInt16 *)worldSector->triangles,
                                   (RwUInt32)triangleSize)))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }

            if (result && (version < rwLIBRARYVERSION36002))
            {
                for (i = 0; i < as.numTriangles; i++)
                {
                    rpPolygon   poly;

                    poly = *((rpPolygon *)&worldSector->triangles[i]);

                    worldSector->triangles[i].vertIndex[0] = poly.vertIndex[0];
                    worldSector->triangles[i].vertIndex[1] = poly.vertIndex[1];
                    worldSector->triangles[i].vertIndex[2] = poly.vertIndex[2];
                    worldSector->triangles[i].matIndex = poly.matIndex;
                }
            }

#ifdef RWDEBUG
            /*
             * For PowerPipe we have to do more work. We need to get vertices
             * sorted by material and duplicated at material boundaries.
             * Inform the app if this has not been done.
             */
            if (result)
            {
                RwBool  b;

                if (WorldSectorIsCorrectlySorted(worldSector, &b) == NULL)
                {
                    /* Out of memory most probably */

                    result = FALSE;
                }

                if (result && (b == FALSE))
                {
                    RWMESSAGE((RWSTRING
                               ("Warning: World sector is in an invalid format for "
                                "RxPipeline rendering. There may be visible artifacts "
                                "and/or decreased performance.")));
                }
            }
#endif /* RWDEBUG */
        }
    }

    /* Call the constructor as late as we can */
    if (result)
    {
        rwPluginRegistryInitObject(&sectorTKList, worldSector);

        lastSeenSectRightsPluginId = 0;
        lastSeenSectExtraData = 0;

        /* Read in the plugin info */
        if (result && (!rwPluginRegistryReadDataChunks(&sectorTKList, stream, worldSector)))
        {
            /* Failed to read extension data */
            result = FALSE;
        }

        if (result && (lastSeenSectRightsPluginId))
        {
            /* Silently ignore errors if any */
            _rwPluginRegistryInvokeRights(&sectorTKList, lastSeenSectRightsPluginId,
                                        worldSector, lastSeenSectExtraData);
        }
    }

    /* Was there an error ? */
    if (result == FALSE)
    {
#ifdef RWDEBUG
        /* Place validation code to avoid incorrect assertions during plugin destroy. */
        _rwPluginRegistryPlaceValidationCodes(&sectorTKList, worldSector);
#endif

        if (rwObjectTestPrivateFlags(world, rpWORLDSINGLEMALLOC))
        {
            _rpWorldSectorDeinstanceAll((RpSector *) worldSector);
        }
        else
        {
            _rpWorldSectorDestroyRecurse((RpSector *) worldSector);
        }

        RWRETURN((RpWorldSector *) NULL);
    }

    RWRETURN(worldSector);
}

/****************************************************************************
 PlaneSectorStreamGetSize

 On entry   :
 On exit    : Size of Binary PlaneSector
 */

static RwUInt32
PlaneSectorStreamGetSize(const RpPlaneSector * planeSector, const
                         RpWorld * world, RwUInt32 flags)
{
    RwUInt32            size;

    RWFUNCTION(RWSTRING("PlaneSectorStreamGetSize"));
    RWASSERT(planeSector);

    size = sizeof(_rpPlaneSector) + rwCHUNKHEADERSIZE;

    /* Left side first */
    if (planeSector->leftSubTree->type < 0)
    {
        /* Atomic sector */
        size += WorldSectorStreamGetSize((RpWorldSector *)
                                         planeSector->leftSubTree,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }
    else
    {
        /* Plane sector - recurse */
        size += PlaneSectorStreamGetSize((RpPlaneSector *)
                                         planeSector->leftSubTree,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }

    /* Right side next */
    if (planeSector->rightSubTree->type < 0)
    {
        /* Atomic sector */
        size += WorldSectorStreamGetSize((RpWorldSector *)
                                         planeSector->rightSubTree,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }
    else
    {
        /* Plane sector - recurse */
        size += PlaneSectorStreamGetSize((RpPlaneSector *)
                                         planeSector->rightSubTree,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }
    RWRETURN(size);
}

/****************************************************************************
 PlaneSectorStreamWrite

 On entry   : Stream to write to
 On exit    :
 */
static const RpPlaneSector *
PlaneSectorStreamWrite(const RpPlaneSector * planeSector, RwStream
                       * stream, const RpWorld * world, RwUInt32 flags)
{
    _rpPlaneSector      ps;

    RWFUNCTION(RWSTRING("PlaneSectorStreamWrite"));
    RWASSERT(planeSector);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_PLANESECT,
                                  PlaneSectorStreamGetSize
                                  (planeSector, world, flags)))
    {
        RWRETURN((const RpPlaneSector *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader
        (stream, rwID_STRUCT, sizeof(_rpPlaneSector)))
    {
        RWRETURN((const RpPlaneSector *)NULL);
    }

    /* Fill it in */
    ps.type = planeSector->type;
    ps.value = planeSector->value;
    ps.leftIsWorldSector = ((planeSector->leftSubTree)->type < 0);
    ps.rightIsWorldSector = ((planeSector->rightSubTree)->type < 0);
    ps.leftValue = planeSector->leftValue;
    ps.rightValue = planeSector->rightValue;

    /* Convert it */
    (void)RwMemRealToFloat32(&ps.value, sizeof(ps.value));
    (void)RwMemRealToFloat32(&ps.leftValue, sizeof(ps.leftValue));
    (void)RwMemRealToFloat32(&ps.rightValue, sizeof(ps.rightValue));
    (void)RwMemLittleEndian32(&ps, sizeof(ps));

    /* And write it */
    if (!RwStreamWrite(stream, &ps, sizeof(ps)))
    {
        RWRETURN((const RpPlaneSector *)NULL);
    }

    /* Left first */
    if (planeSector->leftSubTree->type < 0)
    {
        /* Atomic */
        if (!WorldSectorStreamWrite((RpWorldSector *)
                                    planeSector->leftSubTree,
                                    stream, world, flags))
        {
            RWRETURN((const RpPlaneSector *)NULL);
        }
    }
    else
    {
        /* Plane */
        if (!PlaneSectorStreamWrite((RpPlaneSector *)
                                    planeSector->leftSubTree,
                                    stream, world, flags))
        {
            RWRETURN((const RpPlaneSector *)NULL);
        }
    }

    /* Right second */
    if (planeSector->rightSubTree->type < 0)
    {
        /* Atomic */

        if (!WorldSectorStreamWrite((RpWorldSector *)
                                    planeSector->rightSubTree,
                                    stream, world, flags))
        {
            RWRETURN((const RpPlaneSector *)NULL);
        }
    }
    else
    {
        /* Plane */
        if (!PlaneSectorStreamWrite((RpPlaneSector *)
                                    planeSector->rightSubTree,
                                    stream, world, flags))
        {
            RWRETURN((const RpPlaneSector *)NULL);
        }
    }

    RWRETURN(planeSector);
}

/****************************************************************************
 PlaneSectorStreamRead

 On entry   : Stream to read from
 On exit    : PlaneSector created
 */
static RpPlaneSector *
PlaneSectorStreamRead(RwStream * stream, RwUInt8 **
                      binaryWorldMallocAddr, RpWorld * world,
                      RwUInt32 flags)
{
    RpPlaneSector      *planeSector;
    _rpPlaneSector      ps;
    RwUInt32            size, version;
    RwBool              result;

    RWFUNCTION(RWSTRING("PlaneSectorStreamRead"));
    RWASSERT(stream);

    result = TRUE;

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpPlaneSector *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpPlaneSector *)NULL);
    }

    /* The size read from the stream is the size stored,
     * no question about it */

    /* Read it */
    RWASSERT(size <= sizeof(ps));
    memset(&ps, 0, sizeof(ps));
    if (RwStreamRead(stream, &ps, size) != size)
    {
        RWRETURN((RpPlaneSector *)NULL);
    }

    /* Convert it */
    (void)RwMemNative32(&ps, sizeof(ps));
    (void)RwMemFloat32ToReal(&ps.value, sizeof(ps.value));
    (void)RwMemFloat32ToReal(&ps.leftValue, sizeof(ps.leftValue));
    (void)RwMemFloat32ToReal(&ps.rightValue, sizeof(ps.rightValue));

    planeSector = (RpPlaneSector *)
        BinaryWorldMalloc(binaryWorldMallocAddr, sizeof(RpPlaneSector));
    if (!planeSector)
    {
        RWERROR((E_RW_NOMEM, (sizeof(RpPlaneSector))));
        RWRETURN((RpPlaneSector *)NULL);
    }

    /* Use contents */
    planeSector->type = ps.type;
    planeSector->value = ps.value;
    if (flags & rpWORLDSECTORSOVERLAP)
    {
        planeSector->leftValue = ps.leftValue;
        planeSector->rightValue = ps.rightValue;
    }
    else
    {
        planeSector->leftValue = ps.value;
        planeSector->rightValue = ps.value;
    }

    /* Left */
    if (ps.leftIsWorldSector)
    {
        if (!RwStreamFindChunk(stream, (RwUInt32)rwID_ATOMICSECT,
                               (RwUInt32 *)NULL, &version))
        {
            /* Deallocation is performed in one block when we return */
            result = FALSE;
        }

        if (result && ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION)))
        {
            RWERROR((E_RW_BADVERSION));
            result = FALSE;
        }

        if (result && (!(planeSector->leftSubTree =
              (RpSector *) WorldSectorStreamRead(stream,
                                        binaryWorldMallocAddr, world, flags))))
        {
            /* Deallocation is performed in one block when we return */
            result = FALSE;
        }
    }
    else
    {
        if (!RwStreamFindChunk(stream, (RwUInt32)rwID_PLANESECT,
                               (RwUInt32 *)NULL, &version))
        {
            /* Deallocation is performed in one block when we return */
            result = FALSE;
        }

        if (result && ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION)))
        {
            RWERROR((E_RW_BADVERSION));
            result = FALSE;
        }

        if (result && (!(planeSector->leftSubTree =
              (RpSector *) PlaneSectorStreamRead(stream,
                                         binaryWorldMallocAddr, world, flags))))
        {
            /* Deallocation is performed in one block when we return */
            result = FALSE;
        }
    }

    /* Right */
    if (result)
    {
        if (ps.rightIsWorldSector)
        {
            if (!RwStreamFindChunk(stream, (RwUInt32)rwID_ATOMICSECT,
                                (RwUInt32 *)NULL, &version))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }

            if (result && ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION)))
            {
                RWERROR((E_RW_BADVERSION));
                result = FALSE;
            }

            if (result && (!(planeSector->rightSubTree =
                (RpSector *) WorldSectorStreamRead(stream,
                                            binaryWorldMallocAddr, world, flags))))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }
        }
        else
        {
            if (!RwStreamFindChunk(stream, (RwUInt32)rwID_PLANESECT,
                                (RwUInt32 *)NULL, &version))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }

            if (result && ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION)))
            {
                RWERROR((E_RW_BADVERSION));
                result = FALSE;
            }

            if (result && (!(planeSector->rightSubTree =
                (RpSector *) PlaneSectorStreamRead(stream,
                                            binaryWorldMallocAddr, world, flags))))
            {
                /* Deallocation is performed in one block when we return */
                result = FALSE;
            }
        }
    }

    /* Was there an error ? */
    if (result == FALSE)
    {
        if (planeSector->leftSubTree)
        {
            if (rwObjectTestPrivateFlags(world, rpWORLDSINGLEMALLOC))
            {
                _rpWorldSectorDeinstanceAll(planeSector->leftSubTree);
            }
            else
            {
                _rpWorldSectorDestroyRecurse(planeSector->leftSubTree);
            }

            planeSector->leftSubTree = NULL;
        }

        RWRETURN((RpPlaneSector *) NULL);
    }

    RWRETURN(planeSector);
}


/****************************************************************************
 WorldFindSize

 Finds the block size to hold the whole world

 On entry   : World, pointer to numbers to receive various stats
 On exit    : Size of the world in bytes
 */

static void
WorldFindSize(const RpWorld * world, RwInt32 * numTriangles,
              RwInt32 * numVertices,
              RwInt32 * numPlaneSectors,
              RwInt32 * numWorldSectors)
{
    RwInt32             nStack = 0;
    RpSector           *spaStack[rpWORLDMAXBSPDEPTH];
    RpSector           *spSect = world->rootSector;

    RWFUNCTION(RWSTRING("WorldFindSize"));
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    *numTriangles = 0;
    *numVertices = 0;
    *numPlaneSectors = 0;
    *numWorldSectors = 0;

    do
    {
        if (spSect->type < 0)
        {
            /* Atomic sector */
            RpWorldSector      *worldSector = (RpWorldSector *) spSect;

            /* One more world sector */
            (*numWorldSectors)++;

            /* Add on the vertices and triangles */
            (*numTriangles) += worldSector->numTriangles;
            (*numVertices) += worldSector->numVertices;

            /* Try next */
            spSect = spaStack[nStack--];
        }
        else
        {
            RpPlaneSector      *pspPlane = (RpPlaneSector *) spSect;

            /* One more plane sector */
            (*numPlaneSectors)++;

            /* Go left, stack right */
            spSect = pspPlane->leftSubTree;
            spaStack[++nStack] = pspPlane->rightSubTree;
        }
    }
    while (nStack >= 0);

    RWRETURNVOID();
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldStreamGetSize is used to  determine the size in bytes
 * of the binary representation of the given world. Only static objects in
 * the world are included. This is used in
 * the binary chunk header to indicate the size of the chunk. The size does
 * not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param world  Pointer to the world.
 *
 * \return Returns a RwUInt32 value equal to the chunk size of
 * the specified world in bytes if successful or zero if there is an error.
 *
 * \see RpWorldStreamRead
 * \see RpWorldStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpWorldStreamGetSize(const RpWorld * world)
{
    RwUInt32            size;
    RwUInt32            flags;
    RWAPIFUNCTION(RWSTRING("RpWorldStreamGetSize"));

    RWASSERT(binWorldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);

    flags = RpWorldGetFlags(world);
    size = sizeof(_rpWorld) + rwCHUNKHEADERSIZE;

    /* Size of material list */
    size += _rpMaterialListStreamGetSize(&world->matList) + rwCHUNKHEADERSIZE;

    if (world->rootSector->type < 0)
    {
        /* Atomic sector */

        size += WorldSectorStreamGetSize((RpWorldSector *)
                                         world->rootSector,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }
    else
    {
        /* Plane sector */

        size += PlaneSectorStreamGetSize((RpPlaneSector *)
                                         world->rootSector,
                                         world, flags) + rwCHUNKHEADERSIZE;
    }

    /* Plugin data size */
    size +=
        (rwPluginRegistryGetSize(&worldTKList, world) + rwCHUNKHEADERSIZE);

    RWRETURN(size);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldStreamWrite is used to write the specified world to the
 * given binary stream. Only those parts of the world contained in static
 * objects are written to the stream. Note that the stream must be opened
 * prior to this function call.
 *
 * The world plugin must be attached before using this function.
 *
 * For example:
  \code
    RwStream *streamOut;
    RpWorld *worldOut;

    // Write object

    streamOut = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, "world.rws");

    RpWorldStreamWrite(worldOut, streamOut);

    RwStreamClose(streamOut, NULL);
  \endcode

 * \param world  Pointer to the world.
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the specified world if successful or NULL if
 * there is an error.
 *
 * \see RpWorldStreamRead
 * \see RpWorldStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */
const RpWorld      *
RpWorldStreamWrite(const RpWorld * world, RwStream * stream)
{
    _rpWorld            w;
    RwInt32             numTriangles, numVertices;
    RwInt32             numPlaneSectors, numWorldSectors;
    RwUInt32            flags;

    RWAPIFUNCTION(RWSTRING("RpWorldStreamWrite"));

    RWASSERT(binWorldModule.numInstances);
    RWASSERT(world);
    RWASSERTISTYPE(world, rpWORLD);
    RWASSERT(stream);

    flags = RpWorldGetFlags(world);

    if (!RwStreamWriteChunkHeader(stream, rwID_WORLD,
                                  RpWorldStreamGetSize(world)))
    {
        RWRETURN((const RpWorld *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(_rpWorld)))
    {
        RWRETURN((const RpWorld *)NULL);
    }

    /* Find out how big the world is */
    WorldFindSize(world, &numTriangles, &numVertices,
                  &numPlaneSectors, &numWorldSectors);

    /* Verify texture flags */
    RWASSERT((world->numTexCoordSets == 1) == ((flags & rpWORLDTEXTURED) > 0));
    RWASSERT((world->numTexCoordSets > 1) == ((flags & rpWORLDTEXTURED2) > 0));

    /* Fill it */
    w.format = flags
        | rpWORLDTEXCOORDSETS(world->numTexCoordSets)
        | rpWORLDSECTORSOVERLAP;
    w.rootIsWorldSector = (world->rootSector->type < 0);
    RwV3dScale(&w.invWorldOrigin, &world->worldOrigin, (RwReal) (-1.0f));

    w.numVertices = numVertices;
    w.numTriangles = numTriangles;
    w.numPlaneSectors = numPlaneSectors;
    w.numWorldSectors = numWorldSectors;
    w.colSectorSize = 0;                 /* unused but retain to keep same struct size */
    w.boundingBox = world->boundingBox;

    /* Convert it */
    (void)RwMemRealToFloat32(&w.invWorldOrigin, sizeof(w.invWorldOrigin));
    (void)RwMemRealToFloat32(&w.boundingBox, sizeof(w.boundingBox));
    (void)RwMemLittleEndian32(&w, sizeof(w));

    /* Write it */
    if (!RwStreamWrite(stream, &w, sizeof(w)))
    {
        RWRETURN((const RpWorld *)NULL);
    }

    /* Write out all the materials */
    if (!_rpMaterialListStreamWrite(&world->matList, stream))
    {
        RWRETURN((const RpWorld *)NULL);
    }

    /* Check what sector type to write */
    if (world->rootSector->type < 0)
    {
        /* Atomic sector */
        if (!WorldSectorStreamWrite
            ((RpWorldSector *) world->rootSector, stream, world, flags))
        {
            RWRETURN((const RpWorld *)NULL);
        }
    }
    else
    {
        /* Plane sector */
        if (!PlaneSectorStreamWrite
            ((RpPlaneSector *) world->rootSector, stream, world, flags))
        {
            RWRETURN((const RpWorld *)NULL);
        }
    }

    /* write out the plugin info */
    if (!rwPluginRegistryWriteDataChunks(&worldTKList, stream, world))
    {
        /* Failed to read extension data */
        RWRETURN((const RpWorld *)NULL);
    }

    RWRETURN(world);
}

/**
 * \ingroup rpworldsub
 * \ref RpWorldStreamRead is used to read a world from the specified
 * binary stream. Note that prior to this function call a binary world chunk
 * must be found in the stream using the \ref RwStreamFindChunk API function.
 * Note also that the world created by this function only contains static
 * objects.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read a world from a binary stream is
 * as follows:
 * \code
   RwStream *stream;
   RpWorld *newWorld;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_WORLD, NULL, NULL) )
       {
           newWorld = RpWorldStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
   \endcode
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the world if successful or NULL if
 * there is an error.
 *
 * \see RpWorldStreamWrite
 * \see RpWorldStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 *
 */
RpWorld            *
RpWorldStreamRead(RwStream * stream)
{
    RpWorld            *world;
    _rpWorld            w;
    RwInt32             i;
    RwUInt8            *binaryWorldMallocAddr;
    RwInt32             worldSize;
    RwUInt32            size, worldVersion, version;
    RwInt32             flags;
    RwUInt32            numTexCoordSets;

    RWAPIFUNCTION(RWSTRING("RpWorldStreamRead"));

    RWASSERT(binWorldModule.numInstances);
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &worldVersion))
    {
        RWRETURN((RpWorld *)NULL);
    }

    if ((worldVersion < rwLIBRARYBASEVERSION) || (worldVersion > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpWorld *)NULL);
    }

    memset(&w, 0, sizeof(w));

    /* Read it */
    /* For pre-34003 binaries, this does not include the bounding box,
       which must be calculated later */
    if (RwStreamRead(stream, &w, size) != size)
    {
        RWRETURN((RpWorld *)NULL);
    }

    if (w.colSectorSize > 0)
    {
        RWERROR((E_RW_BADVERSION));
        RWMESSAGE((RWSTRING(
            "The BSP has not been exported since RW3.03, and contains "
            "collision data that is incompatible with the current version."
            )));
        RWRETURN((RpWorld *)NULL);
    }

    /* Convert it */
    (void)RwMemNative32(&w, sizeof(w));
    (void)RwMemFloat32ToReal(&w.boundingBox, sizeof(w.boundingBox));
    (void)RwMemFloat32ToReal(&w.invWorldOrigin, sizeof(w.invWorldOrigin));

    /* Figure out how big it is */
    worldSize = worldTKList.sizeOfStruct;
    worldSize += w.numPlaneSectors * sizeof(RpPlaneSector);
    worldSize += w.numWorldSectors * sectorTKList.sizeOfStruct;

    /* The format contains the flags, so just copy */
    flags = w.format;

    numTexCoordSets = WorldFormatGetNumTexCoordSetsMacro(w.format);
    RWASSERT(numTexCoordSets <= rwMAXTEXTURECOORDS);

    if (!(rpWORLDNATIVE & flags))
    {
        worldSize += w.numVertices * sizeof(RwV3d);

        if (flags & (RwInt32) rpWORLDNORMALS)
        {
            worldSize += w.numVertices * sizeof(RpVertexNormal);
        }

        if (flags & (RwInt32) rpWORLDPRELIT)
        {
            worldSize += w.numVertices * sizeof(RwRGBA);
        }

        if (numTexCoordSets > 0)
        {
            /* Allow space for texture coordinates and pointer arrays in sectors */
            worldSize += numTexCoordSets * w.numVertices * sizeof(RwTexCoords);
        }

        worldSize += w.numTriangles * sizeof(RpTriangle);
    }

    /* Can just allocate for the whole world's memory */
    world = (RpWorld *)RwMalloc(worldSize,
        rwMEMHINTDUR_EVENT | rwID_WORLD);
    if (!world)
    {
        RWERROR((E_RW_NOMEM, (worldSize)));
        RWRETURN(FALSE);
    }

    /* Clear it to zero. This would set pointers to NULL */
    memset(world, 0, worldSize);

    binaryWorldMallocAddr = (RwUInt8 *) world + worldTKList.sizeOfStruct;
    rwObjectInitialize(world, rpWORLD, 0);
    rwObjectSetPrivateFlags(world, rpWORLDSINGLEMALLOC);

    /* Add to the list */
    _rpWorldRegisterWorld(world, worldSize);

    /* Set the worlds flags */
    RpWorldSetFlags(world, flags);

    /* Set the default rendering order */
    RpWorldSetRenderOrder(world, rpWORLDRENDERBACK2FRONT);

    /* Set the world offset */
    RwV3dScale(&world->worldOrigin, &w.invWorldOrigin, (RwReal) (-1.0f));

    /* Set number of texture coordinates used in this world */
    world->numTexCoordSets = numTexCoordSets;

    /* No atomic sectors yet */
    world->rootSector = (RpSector *)NULL;

    /* use the default world sector object pipeline */
    world->pipeline = (RxPipeline *)NULL;

    /* Initialise the link list. */
    rwLinkListInitialize(&world->clumpList);
    rwLinkListInitialize(&world->lightList);
    rwLinkListInitialize(&world->directionalLightList);

    /* Initialise the plugins */
#ifdef RWDEBUG
    _rwPluginRegistryPlaceValidationCodes(&worldTKList, world);
#endif /* RWDEBUG */

    /* Read all of the materials */
    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_MATLIST,
                           (RwUInt32 *)NULL, &version))
    {
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    if (!_rpMaterialListStreamRead(stream, &world->matList))
    {
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    /* Create all of the sectors */
    if (w.rootIsWorldSector)
    {
        /* We have an atomic sector */
        if (!RwStreamFindChunk(stream, (RwUInt32)rwID_ATOMICSECT,
                               (RwUInt32 *)NULL, &version))
        {
            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }

        if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
        {
            RWERROR((E_RW_BADVERSION));

            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }

        if (!(world->rootSector = (RpSector *)
              WorldSectorStreamRead(stream,
                                    &binaryWorldMallocAddr, world, flags)))
        {
            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }
    }
    else
    {
        /* We have a plane sector */
        if (!RwStreamFindChunk(stream, (RwUInt32)rwID_PLANESECT,
                               (RwUInt32 *)NULL, &version))
        {
            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }

        if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
        {
            RWERROR((E_RW_BADVERSION));

            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }

        if (!(world->rootSector = (RpSector *)
              PlaneSectorStreamRead(stream,
                                    &binaryWorldMallocAddr, world, flags)))
        {
            RpWorldDestroy(world);
            RWRETURN((RpWorld *)NULL);
        }
    }

    /* Check that our single malloc area hasn't been overrun */
    RWASSERT(((RwUInt32)worldSize) == ((RwUInt32)binaryWorldMallocAddr - (RwUInt32)world));

    world->numClumpsInWorld = 0;
    world->currentClumpLink = rwLinkListGetTerminator(&world->clumpList);

    /* Set up the bbox */
    world->boundingBox = w.boundingBox;

    /* Set up the sector bounding boxes based on the BSP planes. These are
     * not the same as the tight bounding boxes that were streamed in
     */
    _rpWorldSetupSectorBoundingBoxes(world);

    /* Reset all its callback functions */
    RpWorldSetSectorRenderCallBack(world,
                                   (RpWorldSectorCallBackRender)NULL);

    /* Go through the mat list - make textured correct */
    for (i = 0; i < world->matList.numMaterials; i++)
    {
        RwTexture          *tex;
        RpMaterial         *mat = (world->matList).materials[i];

        tex = RpMaterialGetTexture(mat);

        if (tex)
        {
            RpMaterialSetTexture(mat, tex);
        }
    }

    /* Call the constructor on the world as late as
     * we can (so world sectors are in place)
     */
    rwPluginRegistryInitObject(&worldTKList, world);

    lastSeenWorldRightsPluginId = 0;
    lastSeenWorldExtraData = 0;

    /* Read in the plugin info */
    if (!rwPluginRegistryReadDataChunks(&worldTKList, stream, world))
    {
        /* Failed to read extension data */
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    if (lastSeenWorldRightsPluginId)
    {
        /* Silently ignore errors if any */
        _rwPluginRegistryInvokeRights(&worldTKList, lastSeenWorldRightsPluginId,
                                      world, lastSeenWorldExtraData);
    }

    /* Release the world so platform specific info can be set up */
    if (!_rpWorldUnlock(world))
    {
        /* Failed to unlock, fail the world read */
        RpWorldDestroy(world);
        RWRETURN((RpWorld *)NULL);
    }

    RWRETURN(world);
}

RpWorldSectorChunkInfo *
_rpWorldSectorChunkInfoRead(RwStream * stream,
                            RpWorldSectorChunkInfo *
                            worldSectorChunkInfo,
                            RwInt32 * bytesRead)
{
    RwUInt32            size;
    RwUInt32            readSize;
    RWFUNCTION(RWSTRING("_rpWorldSectorChunkInfoRead"));

    RWASSERT(stream);
    RWASSERT(worldSectorChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((RpWorldSectorChunkInfo *)NULL);
    }

    RWASSERT(size >= sizeof(RpWorldSectorChunkInfo));
    readSize = sizeof(RpWorldSectorChunkInfo);
    memset(worldSectorChunkInfo, 0, readSize);
    if (RwStreamRead(stream, worldSectorChunkInfo, readSize) != readSize)
    {
        RWRETURN((RpWorldSectorChunkInfo *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    /* Convert it */
    (void)RwMemNative32(worldSectorChunkInfo, sizeof(RpWorldSectorChunkInfo));
    (void)RwMemFloat32ToReal(&worldSectorChunkInfo->inf,
                       sizeof(worldSectorChunkInfo->inf));
    (void)RwMemFloat32ToReal(&worldSectorChunkInfo->sup,
                       sizeof(worldSectorChunkInfo->sup));

    RWRETURN(worldSectorChunkInfo);
}

RpPlaneSectorChunkInfo *
_rpPlaneSectorChunkInfoRead(RwStream * stream,
                            RpPlaneSectorChunkInfo * planeSectorChunkInfo,
                            RwInt32 * bytesRead)
{
    RwUInt32            size;
    RwUInt32            readSize;
    RWFUNCTION(RWSTRING("_rpPlaneSectorChunkInfoRead"));

    RWASSERT(stream);
    RWASSERT(planeSectorChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((RpPlaneSectorChunkInfo *)NULL);
    }

    /* The size read from the stream is the size stored,
     * no question about it */

    /* Read it */
    RWASSERT(size <= sizeof(RpPlaneSectorChunkInfo));
    readSize = sizeof(RpPlaneSectorChunkInfo);
    memset(planeSectorChunkInfo, 0, readSize);
    if (RwStreamRead(stream, planeSectorChunkInfo, readSize) != readSize)
    {
        RWRETURN((RpPlaneSectorChunkInfo *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    /* Convert it */
    (void)RwMemNative32(planeSectorChunkInfo, sizeof(RpPlaneSectorChunkInfo));
    (void)RwMemFloat32ToReal(&planeSectorChunkInfo->value,
                       sizeof(planeSectorChunkInfo->value));
    (void)RwMemFloat32ToReal(&planeSectorChunkInfo->leftValue,
                       sizeof(planeSectorChunkInfo->leftValue));
    (void)RwMemFloat32ToReal(&planeSectorChunkInfo->rightValue,
                       sizeof(planeSectorChunkInfo->rightValue));

    RWRETURN(planeSectorChunkInfo);
}

RpWorldChunkInfo   *
_rpWorldChunkInfoRead(RwStream * stream,
                      RpWorldChunkInfo * worldChunkInfo,
                      RwInt32 * bytesRead)
{
    RwUInt32            size;
    RwUInt32            version;
    RWFUNCTION(RWSTRING("_rpWorldChunkInfoRead"));

    RWASSERT(stream);
    RWASSERT(worldChunkInfo);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpWorldChunkInfo *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) || (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpWorldChunkInfo *)NULL);
    }

    memset(worldChunkInfo, 0, sizeof(RpWorldChunkInfo));

    /* Read it */
    /* For pre-34003 binaries, this does not include the bounding box */
    if (RwStreamRead(stream, worldChunkInfo, size) != size)
    {
        RWRETURN((RpWorldChunkInfo *)NULL);
    }

    /* Convert it */
    (void)RwMemNative32(worldChunkInfo, sizeof(RpWorldChunkInfo));
    (void)RwMemFloat32ToReal(&worldChunkInfo->boundingBox, sizeof(RwBBox));
    (void)RwMemFloat32ToReal(&worldChunkInfo->invWorldOrigin, sizeof(RwV3d));

    *bytesRead = size + rwCHUNKHEADERSIZE;

    RWRETURN(worldChunkInfo);
}


/****************************************************************************
 _rpBinaryWorldClose

 On entry   : instance, offset, size
 On exit    : instance on success
 */

void               *
_rpBinaryWorldClose(void *instance,
                    RwInt32 __RWUNUSED__ offset,
                    RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpBinaryWorldClose"));

    /* One less instance */
    binWorldModule.numInstances--;

    /* All done */
    RWRETURN(instance);
}

/****************************************************************************
 _rpBinaryWorldOpen

 On entry   : instance, offset, size
 On exit    : instance on success
 */

void               *
_rpBinaryWorldOpen(void *instance,
                   RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rpBinaryWorldOpen"));

    /* One more instance */
    binWorldModule.numInstances++;

    /* All done */
    RWRETURN(instance);
}

