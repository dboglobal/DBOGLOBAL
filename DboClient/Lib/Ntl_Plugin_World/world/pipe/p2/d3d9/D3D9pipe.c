/****************************************************************************
 *                                                                          *
 * module : D3D9pipe.c                                                        *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include <d3d9.h>

#include "rwcore.h"
#include "baworld.h"

#include "D3D9pipe.h"


/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

#define RENDERFLAGS_HAS_TEXTURE  1
#define RENDERFLAGS_HAS_TFACTOR  2

/****************************************************************************
 local types
 */

/****************************************************************************
 local variables
 */
#define NUMPRIMTYPES    7
static const D3DPRIMITIVETYPE _RwD3D9PrimConvTable[NUMPRIMTYPES] =
{
    (D3DPRIMITIVETYPE)0,                      /* rwPRIMTYPENAPRIMTYPE */
    D3DPT_LINELIST,         /* rwPRIMTYPELINELIST */
    D3DPT_LINESTRIP,        /* rwPRIMTYPEPOLYLINE */
    D3DPT_TRIANGLELIST,     /* rwPRIMTYPETRILIST */
    D3DPT_TRIANGLESTRIP,    /* rwPRIMTYPETRISTRIP */
    D3DPT_TRIANGLEFAN,      /* rwPRIMTYPETRIFAN */
    D3DPT_POINTLIST
};

/****************************************************************************
 Functions
 */

/****************************************************************************
 _rwD3D9MeshGetNumVerticesMinIndex

 Purpose:   Calculates the number of vertices and the min index.

 On entry:  indices     - Pointer to an array of indices
            numIndices  - Number of indices
            numVertices - Pointer to the return value for max
            min         - Pointer to the return value for min

 On exit :  TRUE on success else FALSE.
*/
void
_rwD3D9MeshGetNumVerticesMinIndex(const RxVertexIndex *indices,
                                  RwUInt32 numIndices,
                                  RwUInt32 *numVertices,
                                  RwUInt32 *min)
{
    RWFUNCTION(RWSTRING("_rwD3D9MeshGetNumVerticesMinIndex"));

    if (numIndices)
    {
        RwUInt32    minVert;
        RwUInt32    maxVert;

        /* Find min and max vertex index */
        minVert = 0xFFFFFFFF;
        maxVert = 0x00000000;

        do
        {
            const RxVertexIndex index = *indices;

            minVert = min(minVert, index);
            maxVert = max(maxVert, index);

            indices++;
        }
        while (--numIndices);

        *numVertices = (maxVert - minVert) + 1;
        *min = minVert;
    }
    else
    {
        *numVertices = 0;
        *min = 0;
    }

    RWRETURNVOID();
}

/****************************************************************************
 SortTriangles

 Purpose:   Sort the triangles indices.

 On entry:  indices     - Pointer to an array of indices
            numIndices  - Number of indices
*/
static int
SortTriangles(const void *pA, const void *pB)
{
    const RxVertexIndex *pTA = (const RxVertexIndex *)pA;
    const RxVertexIndex *pTB = (const RxVertexIndex *)pB;

    RwUInt32 sortedIndexA[3];
    RwUInt32 sortedIndexB[3];
    RwUInt32 temp;


    RWFUNCTION( RWSTRING( "SortTriangles" ) );

    sortedIndexA[0] = pTA[0];
    sortedIndexA[1] = pTA[1];
    sortedIndexA[2] = pTA[2];

    if (sortedIndexA[0] > sortedIndexA[1])
    {
        temp = sortedIndexA[0];
        sortedIndexA[0] = sortedIndexA[1];
        sortedIndexA[1] = temp;
    }

    if (sortedIndexA[1] > sortedIndexA[2])
    {
        temp = sortedIndexA[1];
        sortedIndexA[1] = sortedIndexA[2];
        sortedIndexA[2] = temp;

        if (sortedIndexA[0] > sortedIndexA[1])
        {
            temp = sortedIndexA[0];
            sortedIndexA[0] = sortedIndexA[1];
            sortedIndexA[1] = temp;
        }
    }

    sortedIndexB[0] = pTB[0];
    sortedIndexB[1] = pTB[1];
    sortedIndexB[2] = pTB[2];

    if (sortedIndexB[0] > sortedIndexB[1])
    {
        temp = sortedIndexB[0];
        sortedIndexB[0] = sortedIndexB[1];
        sortedIndexB[1] = temp;
    }

    if (sortedIndexB[1] > sortedIndexB[2])
    {
        temp = sortedIndexB[1];
        sortedIndexB[1] = sortedIndexB[2];
        sortedIndexB[2] = temp;

        if (sortedIndexB[0] > sortedIndexB[1])
        {
            temp = sortedIndexB[0];
            sortedIndexB[0] = sortedIndexB[1];
            sortedIndexB[1] = temp;
        }
    }

    if (sortedIndexA[0] == sortedIndexB[0])
    {
        if (sortedIndexA[1] == sortedIndexB[1])
        {
            RWRETURN( sortedIndexA[2] - sortedIndexB[2] );
        }

        RWRETURN( sortedIndexA[1] - sortedIndexB[1] );
    }

    RWRETURN( sortedIndexA[0] - sortedIndexB[0] );
}

void
_rwD3D9SortTriListIndices(RxVertexIndex *indices, RwUInt32 numIndices)
{
    RWFUNCTION(RWSTRING("_rwD3D9SortTriListIndices"));

    qsort(indices, (numIndices / 3), (3 * sizeof(RxVertexIndex)), SortTriangles);

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9ConvertToTriList

 Purpose:   Converts a triangle strip to a triangle list
*/
RwUInt32
_rwD3D9ConvertToTriList(RxVertexIndex *indexDst,
                        const RxVertexIndex *indexSrc,
                        RwUInt32 numIndices,
                        RwUInt32 minVert)
{
    RwUInt32        n, maxindex;

    RWFUNCTION(RWSTRING("_rwD3D9ConvertToTriList"));

    maxindex = numIndices - 2;
    numIndices = 0;

    /* Convert tri strip to tri list */
    if (minVert)
    {
        for (n = 0; n < maxindex; n++)
        {
            if (indexSrc[0] != indexSrc[1] &&
                indexSrc[0] != indexSrc[2] &&
                indexSrc[1] != indexSrc[2])
            {
                if (n & 0x1)
                {
                    indexDst[2] = (RxVertexIndex)(indexSrc[0] - (RxVertexIndex)minVert);
                    indexDst[1] = (RxVertexIndex)(indexSrc[1] - (RxVertexIndex)minVert);
                    indexDst[0] = (RxVertexIndex)(indexSrc[2] - (RxVertexIndex)minVert);
                }
                else
                {
                    indexDst[0] = (RxVertexIndex)(indexSrc[0] - (RxVertexIndex)minVert);
                    indexDst[1] = (RxVertexIndex)(indexSrc[1] - (RxVertexIndex)minVert);
                    indexDst[2] = (RxVertexIndex)(indexSrc[2] - (RxVertexIndex)minVert);
                }

                indexDst += 3;
                numIndices += 3;
            }

            indexSrc++;
        }
    }
    else
    {
        for (n = 0; n < maxindex; n++)
        {
            if (indexSrc[0] != indexSrc[1] &&
                indexSrc[0] != indexSrc[2] &&
                indexSrc[1] != indexSrc[2])
            {
                if (n & 0x1)
                {
                    indexDst[2] = indexSrc[0];
                    indexDst[1] = indexSrc[1];
                    indexDst[0] = indexSrc[2];
                }
                else
                {
                    indexDst[0] = indexSrc[0];
                    indexDst[1] = indexSrc[1];
                    indexDst[2] = indexSrc[2];
                }

                indexDst += 3;
                numIndices += 3;
            }

            indexSrc++;
        }
    }

    RWRETURN(numIndices);
}

/****************************************************************************
 _rwD3D9ResourceEntryInstanceDataDestroy

 Purpose:   Destroys the vertex and index buffers created by
            _rwD3D9InstanceDataCreate

 on entry:
 on exit :
*/
void
_rwD3D9ResourceEntryInstanceDataDestroy(RwResEntry *repEntry)
{
    RxD3D9ResEntryHeader    *resEntryHeader;
    RxD3D9VertexStream      *vertexStream;
    RwUInt32                n;

    RWFUNCTION(RWSTRING("_rwD3D9ResourceEntryInstanceDataDestroy"));

    resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

    if (repEntry->owner != NULL &&
        !RWD9DOBJECTISWORLDSECTOR(repEntry->owner))
    {
        /* Check if dynamic */
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            if (resEntryHeader->vertexStream[n].managed == FALSE)
            {
                if (RWD9DOBJECTISGEOMETRY(repEntry->owner))
                {
                    _rpD3D9RemoveDynamicGeometry((RpGeometry *)repEntry->owner);
                }
                else if (RWD9DOBJECTISATOMIC(repEntry->owner))
                {
                    RpAtomic *atomic = (RpAtomic *)repEntry->owner;

                    _rpD3D9RemoveDynamicGeometry(RpAtomicGetGeometry(atomic));
                }

                break;
            }
        }
    }

    /* Destroy the index buffer */
    if (resEntryHeader->indexBuffer != NULL)
    {
        IDirect3DIndexBuffer9_Release((LPINDEXBUFFER)resEntryHeader->indexBuffer);

        resEntryHeader->indexBuffer = NULL;
    }

    /* Destroy vertex declaration */
    if (resEntryHeader->vertexDeclaration != NULL)
    {
        RwD3D9DeleteVertexDeclaration(resEntryHeader->vertexDeclaration);

        resEntryHeader->vertexDeclaration = NULL;
    }

    /* Destroy the vertex buffer */
    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        vertexStream = &(resEntryHeader->vertexStream[n]);

        if (vertexStream->vertexBuffer != NULL)
        {
            if (vertexStream->managed)
            {
                const RwUInt32 stride = vertexStream->stride;

                RwD3D9DestroyVertexBuffer(stride, stride * (resEntryHeader->totalNumVertex),
                                            vertexStream->vertexBuffer,
                                            vertexStream->offset);
            }
            else
            {
                /* If offset not zero, assume it was a DynamicLock */
                if (vertexStream->offset == 0 &&
                    vertexStream->dynamicLock == FALSE)
                {
                    RwD3D9DynamicVertexBufferDestroy(vertexStream->vertexBuffer);
                }
            }

            vertexStream->vertexBuffer = NULL;
        }

        vertexStream->offset = 0;
        vertexStream->stride = 0;
        vertexStream->geometryFlags = 0;
        vertexStream->managed = FALSE;
        vertexStream->dynamicLock = FALSE;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9Instance

 Purpose:   To instance.

 On entry:

 On exit :
*/
RwResEntry *
_rxD3D9Instance(void *object,
                void *owner,
                RwUInt8 type,
                RwResEntry **resEntryPointer,
                RpMeshHeader *meshHeader,
                RxD3D9AllInOneInstanceCallBack instanceCallback,
                RwBool allocateNative)
{
    RxD3D9ResEntryHeader    *resEntryHeader;
    RxD3D9InstanceData      *instancedData;
    RwResEntry              *resEntry;
    const RpMesh            *mesh;
    RwInt32                 numMeshes;
    RwUInt32                size, n;
    RwUInt32                firstVert, startIndex;  /* Only for non-indexed geometries */
    RxVertexIndex           *indexBuffer;
    RwBool                  convertToTriList = FALSE;

    RWFUNCTION(RWSTRING("_rxD3D9Instance"));

    /*
     * Calculate the amount of memory to allocate
     */

    /* RxD3D9ResEntryHeader, stores serialNumber & numMeshes */
    size = sizeof(RxD3D9ResEntryHeader);

    /* RxD3D9InstanceData structures, one for each mesh */
    size += sizeof(RxD3D9InstanceData) * meshHeader->numMeshes;

    /*
     * Allocate the resource entry
     */
    if (allocateNative)
    {
        resEntry = RwMalloc(sizeof(RwResEntry) + size,
            rwMEMHINTDUR_EVENT | rwID_WORLDPIPEMODULE);
        *resEntryPointer = resEntry;

        /* Show its not in the arena lists */
        resEntry->link.next     = NULL;
        resEntry->link.prev     = NULL;
        resEntry->size          = size;
        resEntry->owner         = NULL;
        resEntry->ownerRef      = NULL;
        resEntry->destroyNotify = _rwD3D9ResourceEntryInstanceDataDestroy;
    }
    else
    {
        resEntry = RwResourcesAllocateResEntry(owner,
                                               resEntryPointer,
                                               size,
                                               _rwD3D9ResourceEntryInstanceDataDestroy);
    }

    RWASSERT(NULL != resEntry);

    /* Blank the RxD3D9ResEntryHeader & RxD3D9InstanceData's to '0' */
    memset((resEntry + 1), 0, size);

    /*
     * Initialize the RxD3D9ResEntryHeader
     */
    resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

    /* Set the serial number */
    resEntryHeader->serialNumber = meshHeader->serialNum;

    /* Set the number of meshes */
    resEntryHeader->numMeshes = meshHeader->numMeshes;

    /*
     * Create the index buffer if needed
     */
    indexBuffer = NULL;

    resEntryHeader->indexBuffer = NULL;
    resEntryHeader->totalNumIndex = 0;

    if ((meshHeader->flags & rpMESHHEADERUNINDEXED) == 0)
    {
        /* Calculate total num indices */
        mesh = (const RpMesh *)(meshHeader + 1);
        numMeshes = meshHeader->numMeshes;
        while (numMeshes--)
        {
            resEntryHeader->totalNumIndex += mesh->numIndices;

            mesh++;
        }

        /* Initialize the index buffers pointers */
        if (resEntryHeader->totalNumIndex)
        {
            /* check for inefficient triangle strips */
            if (RpMeshHeaderGetPrimType(meshHeader) == rwPRIMTYPETRISTRIP)
            {
                RwUInt32 numTriangles = 0xffffffff / 3;

                RWASSERT(object != NULL);

                if (type == (RwUInt8)rwSECTORATOMIC)
                {
                    RpWorldSector *sector = (RpWorldSector *)object;

                    numTriangles = RpWorldSectorGetNumTriangles(sector);
                }
                else if (type == rpATOMIC)
                {
                    RpAtomic *atomic = (RpAtomic *)object;
                    RpGeometry *geometry = (RpGeometry *)RpAtomicGetGeometry(atomic);

                    numTriangles = RpGeometryGetNumTriangles(geometry);
                }

                if (resEntryHeader->totalNumIndex > 3 * numTriangles)
                {
                    #ifdef RWDEBUG
                    RwChar buff[256];

                    rwsprintf(buff, "Detected inefficient triangle strip, converting to triangle list. Saved %d indices.", (resEntryHeader->totalNumIndex - (3 * numTriangles)));

                    RwDebugSendMessage(rwDEBUGMESSAGE,
                                       "_rxD3D9Instance",
                                       buff);
                    #endif

                    convertToTriList = TRUE;

                    resEntryHeader->totalNumIndex = 3 * numTriangles;
                }
            }

            /* Create index buffer */
            if (RwD3D9IndexBufferCreate(resEntryHeader->totalNumIndex, &(resEntryHeader->indexBuffer)))
            {
                IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)resEntryHeader->indexBuffer,
                                        0, 0, (RwUInt8 **)&indexBuffer, 0);
            }
        }
    }

    /* Primitive type */
    if (convertToTriList)
    {
        resEntryHeader->primType = D3DPT_TRIANGLELIST;
    }
    else
    {
        resEntryHeader->primType = _RwD3D9PrimConvTable[RpMeshHeaderGetPrimType(meshHeader)];
    }

    /* Initialize the vertex buffers pointers */
    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        resEntryHeader->vertexStream[n].vertexBuffer = NULL;
        resEntryHeader->vertexStream[n].offset = 0;
        resEntryHeader->vertexStream[n].stride = 0;
        resEntryHeader->vertexStream[n].geometryFlags = FALSE;
        resEntryHeader->vertexStream[n].managed = FALSE;
        resEntryHeader->vertexStream[n].dynamicLock = FALSE;
    }

    /* Vertex declaration */
    resEntryHeader->vertexDeclaration = NULL;

    /* Get the first RxD3D9InstanceData pointer */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

    firstVert = 0;
    startIndex = 0;

    mesh = (const RpMesh *)(meshHeader + 1);
    numMeshes = meshHeader->numMeshes;
    while (numMeshes--)
    {
        RwUInt32 numIndices = mesh->numIndices;

        /*
         * Number of vertices and Min vertex index,
         * (needed for instancing & reinstancing)
         */
        if (meshHeader->flags & rpMESHHEADERUNINDEXED)
        {
            instancedData->numVertices = numIndices;
            instancedData->minVert = firstVert;

            firstVert += instancedData->numVertices;
        }
        else
        {
            _rwD3D9MeshGetNumVerticesMinIndex(mesh->indices, numIndices,
                                    &instancedData->numVertices,
                                    &instancedData->minVert);
        }

        /* Material */
        instancedData->material = mesh->material;

        /* Vertex shader */
        instancedData->vertexShader = NULL;

        /* Initialize vertex alpha to FALSE */
        instancedData->vertexAlpha = FALSE;

        /*
         * copy indices if needed
         */
        if (indexBuffer != NULL)
        {
            instancedData->startIndex = startIndex;

            if (convertToTriList)
            {
                numIndices = _rwD3D9ConvertToTriList(indexBuffer,
                                                     mesh->indices,
                                                     numIndices,
                                                     instancedData->minVert);
            }
            else
            {
                if (instancedData->minVert)
                {
                    RxVertexIndex   *indexSrc;
                    RxVertexIndex   *indexDst;
                    RwUInt32        n;

                    indexSrc = mesh->indices;
                    indexDst = indexBuffer;

                    n = numIndices;
                    while (n--)
                    {
                        *indexDst = (RxVertexIndex)((*indexSrc) - (RxVertexIndex)instancedData->minVert);

                        indexSrc++;
                        indexDst++;
                    }
                }
                else
                {
                    memcpy(indexBuffer, mesh->indices, sizeof(RxVertexIndex) * numIndices);
                }
            }

            instancedData->numIndex = numIndices;

            if (resEntryHeader->primType == D3DPT_TRIANGLELIST)
            {
                _rwD3D9SortTriListIndices(indexBuffer, numIndices);
            }

            indexBuffer += numIndices;
            startIndex += numIndices;
        }
        else
        {
            instancedData->numIndex = 0;
            instancedData->startIndex = 0;
        }

        /* The number of primitives */
        switch (resEntryHeader->primType)
        {
            case D3DPT_LINELIST:
                {
                    RWASSERT(numIndices > 1);
                    instancedData->numPrimitives = numIndices / 2;
                    break;
                }

            case D3DPT_LINESTRIP:
                {
                    RWASSERT(numIndices > 1);
                    instancedData->numPrimitives = numIndices - 1;
                    break;
                }

            case D3DPT_TRIANGLELIST:
                {
                    RWASSERT(numIndices > 2);
                    instancedData->numPrimitives = numIndices / 3;
                    break;
                }

            case D3DPT_TRIANGLESTRIP:
                {
                    RWASSERT(numIndices > 2);
                    instancedData->numPrimitives = numIndices - 2;
                    break;
                }

            case D3DPT_TRIANGLEFAN:
                {
                    RWASSERT(numIndices > 2);
                    instancedData->numPrimitives = numIndices - 2;
                    break;
                }

            default:
                RWASSERT(FALSE && "Unknown primitive tipe.");
                instancedData->numPrimitives = 0;
                break;
        }

        instancedData->baseIndex = 0;

        instancedData++;
        mesh++;
    }

    if (indexBuffer != NULL)
    {
        IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)resEntryHeader->indexBuffer);

        RWASSERT(resEntryHeader->totalNumIndex == startIndex);
    }

    /*
     * Call the instance callback
     */
    if (instanceCallback)
    {
        if (!instanceCallback(object,
                              resEntryHeader,
                              FALSE))
        {
            /* Destroy resource entry */
            if(allocateNative)
            {
                RwFree(resEntry);
            }
            else
            {
                RwResourcesFreeResEntry(resEntry);
            }

            RWRETURN(NULL);
        }
    }

    RWRETURN(resEntry);
}

/****************************************************************************
 _rwD3D9EnableClippingIfNeeded
 Purpose:
 On entry:
 On exit :
*/
void
_rwD3D9EnableClippingIfNeeded(void *object,
                              RwUInt32 type)
{
    RwCamera    *camera;
    RwBool      fullyInsideFrustum;

    RWFUNCTION(RWSTRING("_rwD3D9EnableClippingIfNeeded"));
    RWASSERT(object != NULL);

    camera = RwCameraGetCurrentCamera();
    RWASSERT(camera != NULL);

    if (type == rpATOMIC)
    {
        RpAtomic    *atomic;

        atomic = (RpAtomic *)object;

        fullyInsideFrustum =
            RwD3D9CameraIsSphereFullyInsideFrustum(camera,
                                                   RpAtomicGetWorldBoundingSphere(atomic));
    }
    else
    {
        const RpWorldSector   *worldSector;

        RWASSERT(type == ((RwUInt8)rwSECTORATOMIC));

        worldSector = (const RpWorldSector *)object;

        fullyInsideFrustum =
            RwD3D9CameraIsBBoxFullyInsideFrustum(camera,
                                                 RpWorldSectorGetTightBBox(worldSector));
    }

    RwD3D9SetRenderState(D3DRS_CLIPPING, (fullyInsideFrustum == FALSE));

    RWRETURNVOID();
}

/****************************************************************************
 D3D9DefaultRenderBlack
 Purpose:
 On entry:
 On exit :
*/
static void
D3D9DefaultRenderBlack(RxD3D9ResEntryHeader *resEntryHeader,
                       RwBool hasTextureCoordinates)
{
    RxD3D9InstanceData *instancedData;
    RwUInt32    ditherEnable;
    RwUInt32    shadeMode;
    RwBool      useAlphaTexture;
    RwInt32     numMeshes;

    RWFUNCTION(RWSTRING("D3D9DefaultRenderBlack"));

    /* Save some renderstates */
    RwD3D9GetRenderState(D3DRS_DITHERENABLE, &ditherEnable);
    RwD3D9GetRenderState(D3DRS_SHADEMODE, &shadeMode);

    /* No dither, No Gouraud, No vertex alpha */
    _rwD3D9RenderStateVertexAlphaEnable(FALSE);
    RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xff000000);
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, FALSE);
    RwD3D9SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

    /* Always black color (only alpha channel may change) */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    RwD3D9SetTexture(NULL, 0);

    /* force refresh */
    useAlphaTexture = FALSE;

    /* Get the instanced data */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

    /* Get the number of meshes */
    numMeshes = resEntryHeader->numMeshes;
    while (numMeshes--)
    {
        RWASSERT(instancedData->material != NULL);

        /* this function check internally for alpha channel */
        if (hasTextureCoordinates &&
            instancedData->material->texture &&
            _rwD3D9TextureHasAlpha(instancedData->material->texture))
        {
            RwD3D9SetTexture(instancedData->material->texture, 0);

            if (useAlphaTexture == FALSE)
            {
                useAlphaTexture = TRUE;

                /* We only use the alpha channel from the texture */
                RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
            }
        }
        else
        {
            if (useAlphaTexture)
            {
                useAlphaTexture = FALSE;

                RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);

                RwD3D9SetTexture(NULL, 0);
            }
        }

        /*
         * Vertex shader
         */
        RwD3D9SetVertexShader(instancedData->vertexShader);

        /*
         * Render
         */
        if (resEntryHeader->indexBuffer != NULL)
        {
            RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                       instancedData->baseIndex,
                                       0, instancedData->numVertices,
                                       instancedData->startIndex, instancedData->numPrimitives);
        }
        else
        {
            RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                instancedData->baseIndex,
                                instancedData->numPrimitives);
        }

        /* Move onto the next instancedData */
        instancedData++;
    }

    /* Restore some renderstates */
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, ditherEnable);
    RwD3D9SetRenderState(D3DRS_SHADEMODE, shadeMode);

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D9DefaultRenderCallback
 Purpose:
 On entry:
 On exit :
*/
void
_rxD3D9DefaultRenderCallback(RwResEntry *repEntry,
                             void *object,
                             RwUInt8 type,
                             RwUInt32 flags)
{
    RxD3D9ResEntryHeader    *resEntryHeader;
    RxD3D9InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  lighting;
    RwBool                  vertexAlphaBlend;
    RwUInt32                lastRenderFlags;

    RWFUNCTION(RWSTRING("_rxD3D9DefaultRenderCallback"));

    /*
     * Set the Default Pixel shader
     */
    RwD3D9SetPixelShader(NULL);

    /* Set clipping */
    _rwD3D9EnableClippingIfNeeded(object, type);

    /* Get header */
    resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

    /*
     * Data shared between meshes
     */
    if (resEntryHeader->indexBuffer != NULL)
    {
        RwD3D9SetIndices(resEntryHeader->indexBuffer);
    }

    /* Set the stream sources */
    _rwD3D9SetStreams(resEntryHeader->vertexStream,
                      resEntryHeader->useOffsets);

    /*
    * Vertex Declaration
    */
    RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

    /* check lighting */
    RwD3D9GetRenderState(D3DRS_LIGHTING, &lighting);

    if (lighting == FALSE)
    {
        if ((flags & rxGEOMETRY_PRELIT) == 0)
        {
            D3D9DefaultRenderBlack(resEntryHeader,
                                   (flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0);

            RWRETURNVOID();
        }
    }

    /* Get vertex alpha Blend state */
    vertexAlphaBlend = _rwD3D9RenderStateIsVertexAlphaEnable();

    /* force refresh */
    lastRenderFlags = 0x80000000;

    /* Get the instanced data */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

    /* Get the number of meshes */
    numMeshes = resEntryHeader->numMeshes;
    while (numMeshes--)
    {
        const RpMaterial    *material;
        const RwRGBA        *matcolor;
        RwUInt32            currentRenderFlags = 0;

        RWASSERT(instancedData->material != NULL);

        material = instancedData->material;

        matcolor = &(material->color);

        if ((0xFF != matcolor->alpha) ||
            instancedData->vertexAlpha)
        {
            if (vertexAlphaBlend == FALSE)
            {
                vertexAlphaBlend = TRUE;

                _rwD3D9RenderStateVertexAlphaEnable(TRUE);
            }
        }
        else
        {
            if (vertexAlphaBlend != FALSE)
            {
                vertexAlphaBlend = FALSE;

                _rwD3D9RenderStateVertexAlphaEnable(FALSE);
            }
        }

        if (lighting)
        {
            RwD3D9SetSurfaceProperties(&(material->surfaceProps),
                                       matcolor,
                                       flags);
        }
        else
        {
            if (flags & rxGEOMETRY_MODULATE)
            {
                if (*((const RwUInt32 *)matcolor) != 0xffffffff)
                {
                    RwUInt32 tFactor;

                    currentRenderFlags |= RENDERFLAGS_HAS_TFACTOR;

                    tFactor =
                    ((((RwUInt32)matcolor->alpha)<<24)|(((RwUInt32)matcolor->red)<<16)|(((RwUInt32)matcolor->green)<<8)|((RwUInt32)matcolor->blue));

                    RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, tFactor);
                }
            }
        }

        if (material->texture != NULL &&
            (flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0)
        {
            currentRenderFlags |= RENDERFLAGS_HAS_TEXTURE;

            RwD3D9SetTexture(material->texture, 0);
        }
        else
        {
            RwD3D9SetTexture(NULL, 0);
        }

        if (currentRenderFlags != lastRenderFlags)
        {
            if (currentRenderFlags & RENDERFLAGS_HAS_TEXTURE)
            {
                if ((lastRenderFlags & RENDERFLAGS_HAS_TEXTURE) == 0)
                {
                    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                }

                if (currentRenderFlags & RENDERFLAGS_HAS_TFACTOR)
                {
                    RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
                    RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);

                    RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
                }
                else
                {
                    if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
                    {
                        RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
                        RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
                    }
                }
            }
            else
            {
                if (currentRenderFlags & RENDERFLAGS_HAS_TFACTOR)
                {
                    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
                    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
                }
                else
                {
                    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
                    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
                    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                }

                if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
                {
                    RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
                    RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
                }
            }

            lastRenderFlags = currentRenderFlags;
        }

        /*
         * Vertex shader
         */
        RwD3D9SetVertexShader(instancedData->vertexShader);

        /*
         * Render
         */
        if (resEntryHeader->indexBuffer != NULL)
        {
            RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                       instancedData->baseIndex,
                                       0, instancedData->numVertices,
                                       instancedData->startIndex, instancedData->numPrimitives);
        }
        else
        {
            RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                instancedData->baseIndex,
                                instancedData->numPrimitives);
        }
		
		/* Move onto the next instancedData */
        instancedData++;
    }

    if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
    {
        RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
        RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
    }

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneSetInstanceCallBack sets a callback
 * that will be called to instance.
 *
 * The callback is called once for the object being rendered.
 *
 * \param node The pipeline node to set the callback for.
 * \param callback The callback function.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 */
void
RxD3D9AllInOneSetInstanceCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneInstanceCallBack callback)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneSetInstanceCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->instanceCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneGetInstanceCallBack gets the currently set instance callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return An \ref RxD3D9AllInOneInstanceCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 *
 */
RxD3D9AllInOneInstanceCallBack
RxD3D9AllInOneGetInstanceCallBack(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneGetInstanceCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->instanceCallback);
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneSetReinstanceCallBack sets a callback
 * that will be called to reinstance.
 *
 * The callback is called each time the geometry of the current
 * atomic has been changed.
 *
 * \param node The pipeline node to set the callback for.
 * \param callback The callback function.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 */
void
RxD3D9AllInOneSetReinstanceCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneReinstanceCallBack callback)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneSetReinstanceCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->reinstanceCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneGetReinstanceCallBack gets the currently set reinstance
 * callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return An \ref RxD3D9AllInOneReinstanceCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 *
 */
RxD3D9AllInOneReinstanceCallBack
RxD3D9AllInOneGetReinstanceCallBack(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneGetReinstanceCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->reinstanceCallback);
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneSetLightingCallBack sets a callback
 * that will be called to set up lighting.
 *
 * The callback is called once for the object being rendered.
 *
 * \param node The \ref RxPipelineNode to set the callback for.
 * \param callback The \ref RxD3D9AllInOneLightingCallBack function.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetInstanceCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 * \ref RxD3D9AllInOneGetLightingCallBack
 */
void
RxD3D9AllInOneSetLightingCallBack(RxPipelineNode *node,
                                  RxD3D9AllInOneLightingCallBack callback)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneSetLightingCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->lightingCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneGetLightingCallBack gets the currently
 * set lighting callback. Useful if you plan to overload the
 * RenderWare lighting pipeline with your own.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return A \ref RxD3D9AllInOneLightingCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 * \see RxD3D9AllInOneGetRenderCallBack
 *
 */
RxD3D9AllInOneLightingCallBack
RxD3D9AllInOneGetLightingCallBack(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneGetLightingCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->lightingCallback);
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneSetRenderCallBack sets a callback that
 * will be called to dispatch vertices for rendering for every
 * material in the given object.
 *
 * \param node The \ref RxPipelineNode to set the callback for.
 * \param callback The \ref RxD3D9AllInOneRenderCallBack function.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetInstanceCallBack
 * \see RxD3D9AllInOneSetLightingCallBack
 * \ref RxD3D9AllInOneGetRenderCallBack
 */
void
RxD3D9AllInOneSetRenderCallBack(RxPipelineNode *node,
                                RxD3D9AllInOneRenderCallBack callback)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneSetRenderCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->renderCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxD3D9AllInOneGetRenderCallBack gets the currently set
 * render callback. Useful if you plan to overload the default
 * RenderWare render callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return A \ref RxD3D9AllInOneRenderCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D9AtomicAllInOne
 * \see RxNodeDefinitionGetD3D9WorldSectorAllInOne
 * \see RxD3D9AllInOneSetInstanceCallBack
 * \see RxD3D9AllInOneSetLightingCallBack
 * \ref RxD3D9AllInOneSetRenderCallBack
 */
RxD3D9AllInOneRenderCallBack
RxD3D9AllInOneGetRenderCallBack(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D9AllInOneGetRenderCallBack"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->renderCallback);
}
