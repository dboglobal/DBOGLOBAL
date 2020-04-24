/****************************************************************************
 *                                                                          *
 * module : D3D8pipe.c                                                        *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include <d3d8.h>

#include "rwcore.h"
#include "baworld.h"

#include "D3D8VertexBufferManager.h"

#include "D3D8pipe.h"


/****************************************************************************
 Global variables
 */

/****************************************************************************
 local defines
 */

/****************************************************************************
 local types
 */

/****************************************************************************
 local variables
 */
#define NUMPRIMTYPES    7
static const D3DPRIMITIVETYPE _RwD3D8PrimConvTable[NUMPRIMTYPES] =
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
 _rwD3D8MeshGetNumVerticesMinIndex

 Purpose:   Calculates the number of vertices and the min index.

 On entry:  indices     - Pointer to an array of indices
            numIndices  - Number of indices
            numVertices - Pointer to the return value for max
            min         - Pointer to the return value for min

 On exit :  TRUE on success else FALSE.
*/
void
_rwD3D8MeshGetNumVerticesMinIndex(RxVertexIndex *indices,
                        RwUInt32 numIndices,
                        RwInt32 *numVertices,
                        RwUInt32 *min)
{
    RWFUNCTION(RWSTRING("_rwD3D8MeshGetNumVerticesMinIndex"));

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
 _rwD3D8SortTriListIndices

 Purpose:   Sort the triangles indices.

 On entry:  indices     - Pointer to an array of indices
            numIndices  - Number of indices
*/
static int
SortTriangles(const void *pA, const void *pB)
{
    const RxVertexIndex *pTA = (const RxVertexIndex *)pA;
    const RxVertexIndex *pTB = (const RxVertexIndex *)pB;

    RwInt32 sortedIndexA[3];
    RwInt32 sortedIndexB[3];
    RwInt32 temp;


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
_rwD3D8SortTriListIndices(RxVertexIndex *indices, RwUInt32 numIndices)
{
    RWFUNCTION(RWSTRING("_rwD3D8SortTriListIndices"));

    if (indices[0] != 0 &&
        indices[1] != 0 &&
        indices[2] != 0)
    {
        qsort(indices, (numIndices / 3), (3 * sizeof(RxVertexIndex)), SortTriangles);
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8ResourceEntryInstanceDataDestroy

 Purpose:   Destroys the vertex and index buffers created by
            _rwD3D8InstanceDataCreate

 on entry:
 on exit :
*/
void
_rwD3D8ResourceEntryInstanceDataDestroy(RwResEntry *repEntry)
{
    RwUInt16                numMeshes;
    RxD3D8InstanceData      *instancedData;
    RxD3D8ResEntryHeader    *resEntryHeader;

    RWFUNCTION(RWSTRING("_rwD3D8ResourceEntryInstanceDataDestroy"));

    resEntryHeader = (RxD3D8ResEntryHeader *)(repEntry + 1);

    /* Get the instanced data structures */
    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);

    /* Get the number of meshes */
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        /* Destroy the index buffer */
        if (instancedData->indexBuffer != NULL)
        {
            IDirect3DIndexBuffer8_Release((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);

            instancedData->indexBuffer = NULL;
        }

        /* Destroy the vertex buffer */
        if (instancedData->vertexBuffer != NULL)
        {
            if (instancedData->managed)
            {
                if (instancedData->vertexShader & D3DFVF_RESERVED0)
                {
                    _rxD3D8VertexBufferManagerDestroyNoFVF(instancedData->stride,
                                                        instancedData->stride * instancedData->numVertices,
                                                        instancedData->vertexBuffer,
                                                        instancedData->baseIndex);
                }
                else
                {
                    _rxD3D8VertexBufferManagerDestroy(instancedData->vertexShader,
                                                        instancedData->stride * instancedData->numVertices,
                                                        instancedData->vertexBuffer,
                                                        instancedData->baseIndex);
                }
            }
            else
            {
                RwD3D8DynamicVertexBufferDestroy(instancedData->vertexBuffer);
            }

            instancedData->vertexBuffer = NULL;
        }

        /* On to the next RxD3D8InstanceData */
        instancedData++;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rxD3D8Instance

 Purpose:   To instance.

 On entry:

 On exit :
*/
RwResEntry *
_rxD3D8Instance(void *object,
              void *owner,
              RwResEntry **resEntryPointer,
              RpMeshHeader *meshHeader,
              RxD3D8AllInOneInstanceCallBack instanceCallback,
              RwBool allocateNative)
{
    RxD3D8ResEntryHeader    *resEntryHeader;
    RxD3D8InstanceData      *instancedData;
    RwResEntry              *resEntry;
    RpMesh                  *mesh;
    RwInt16                 numMeshes;
    RwUInt32                size;
    RwUInt32                firstVert;  /* Only for non-indexed geometries */

    RWFUNCTION(RWSTRING("_rxD3D8Instance"));

    /*
     * Calculate the amount of memory to allocate
     */

    /* RxD3D8ResEntryHeader, stores serialNumber & numMeshes */
    size = sizeof(RxD3D8ResEntryHeader);

    /* RxD3D8InstanceData structures, one for each mesh */
    size += sizeof(RxD3D8InstanceData) * meshHeader->numMeshes;

    /*
     * Allocate the resource entry
     */
    if (allocateNative)
    {
        resEntry = RwMalloc(sizeof(RwResEntry) + size,
            rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);
        *resEntryPointer = resEntry;

        /* Show its not in the arena lists */
        resEntry->link.next     = NULL;
        resEntry->link.prev     = NULL;
        resEntry->size          = size;
        resEntry->owner         = NULL;
        resEntry->ownerRef      = NULL;
        resEntry->destroyNotify = _rwD3D8ResourceEntryInstanceDataDestroy;
    }
    else
    {
        resEntry = RwResourcesAllocateResEntry(owner,
                                               resEntryPointer,
                                               size,
                                               _rwD3D8ResourceEntryInstanceDataDestroy);
    }

    RWASSERT(NULL != resEntry);

    /* Blank the RxD3D8ResEntryHeader & RxD3D8InstanceData's to '0' */
    memset((resEntry + 1), 0, size);

    /*
     * Initialize the RxD3D8ResEntryHeader
     */
    resEntryHeader = (RxD3D8ResEntryHeader *)(resEntry + 1);

    /* Set the serial number */
    resEntryHeader->serialNumber = meshHeader->serialNum;

    /* Set the number of meshes */
    resEntryHeader->numMeshes = meshHeader->numMeshes;

    /* Get the first RxD3D8InstanceData pointer */
    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);

    firstVert = 0;

    mesh = (RpMesh *)(meshHeader + 1);
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
            _rwD3D8MeshGetNumVerticesMinIndex(mesh->indices, numIndices,
                                    &instancedData->numVertices,
                                    &instancedData->minVert);
        }

        /* Primitive type */
        instancedData->primType = _RwD3D8PrimConvTable[RpMeshHeaderGetPrimType(meshHeader)];

        /* The number of indices */
        instancedData->numIndices = numIndices;

        /* Material */
        instancedData->material = mesh->material;

        /* Vertex shader */
        instancedData->vertexShader = 0;

        /* The vertex format stride */
        instancedData->stride = 0;

        /* Initialize the vertex buffers pointers */
        instancedData->vertexBuffer = NULL;

        /* Initialize vertex buffer managed to FALSE */
        instancedData->baseIndex = 0;

        /* Initialize vertex alpha to FALSE */
        instancedData->vertexAlpha = FALSE;

        /* Initialize to not managed */
        instancedData->managed = FALSE;

        /* Initialize to not remapped */
        instancedData->remapped = FALSE;

        /*
         * Create the index buffer if needed
         */
        if (meshHeader->flags & rpMESHHEADERUNINDEXED)
        {
            instancedData->indexBuffer = NULL;
        }
        else
        {
            /* Initialize the index buffers pointers */
            if (RwD3D8IndexBufferCreate(numIndices, &(instancedData->indexBuffer)))
            {
                RxVertexIndex   *indexBuffer;

                if (D3D_OK == IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                    0, 0, (RwUInt8 **)&indexBuffer, 0))
                {
                    if(instancedData->minVert)
                    {
                        RxVertexIndex   *indexSrc;
                        RxVertexIndex   *indexDst;

                        indexSrc = mesh->indices;
                        indexDst = indexBuffer;

                        while (numIndices--)
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

                    if (instancedData->primType == D3DPT_TRIANGLELIST)
                    {
                        _rwD3D8SortTriListIndices(indexBuffer, instancedData->numIndices);
                    }

                    IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);
                }
            }
        }

        /*
         * Call the instance callback
         */
        if (instanceCallback)
        {
            if (!instanceCallback(object, instancedData, FALSE))
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

        instancedData++;
        mesh++;
    }

    RWRETURN(resEntry);
}

/****************************************************************************
 _rxD3D8Reinstance

 Purpose:   To instance.

 On entry:

 On exit :
*/
RwBool
_rxD3D8Reinstance( void *object,
                   const RpMeshHeader *meshHeader,
                   RxD3D8InstanceData *instancedData,
                   RxD3D8AllInOneInstanceCallBack instanceCallback)
{
    RWFUNCTION(RWSTRING("_rxD3D8Reinstance"));

    if (instanceCallback)
    {
        RwInt32 numMeshes = meshHeader->numMeshes;

        while (numMeshes--)
        {
            /*
             * Call the instance callback
             */
            if (!instanceCallback(object, instancedData, TRUE))
            {
                RWRETURN(FALSE);
            }

            instancedData++;
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rxD3D8DefaultRenderCallback

 Purpose:

 On entry:

 On exit :
*/
void
_rxD3D8DefaultRenderCallback(RwResEntry *repEntry,
                           void *object,
                           RwUInt8 type,
                           RwUInt32 flags)
{
    RxD3D8ResEntryHeader    *resEntryHeader;
    RxD3D8InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  lighting;
    RwBool                  vertexAlphaBlend;
    RwBool                  forceBlack;
    RwUInt32                ditherEnable;
    RwUInt32                shadeMode;
    void                    *lastVertexBuffer;

    RWFUNCTION(RWSTRING("_rxD3D8DefaultRenderCallback"));

    /* Get lighting state */
    RwD3D8GetRenderState(D3DRS_LIGHTING, &lighting);

    forceBlack = FALSE;

    if (lighting)
    {
        if (flags & rxGEOMETRY_PRELIT)
        {
            /* Emmisive color from the vertex colors */
            RwD3D8SetRenderState(D3DRS_COLORVERTEX, TRUE);
            RwD3D8SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_COLOR1);
        }
        else
        {
            /* Emmisive color from material, set to black in the submit node */
            RwD3D8SetRenderState(D3DRS_COLORVERTEX, FALSE);
            RwD3D8SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
        }
    }
    else
    {
        if ((flags & rxGEOMETRY_PRELIT) == 0)
        {
            forceBlack = TRUE;

            RwD3D8GetRenderState(D3DRS_DITHERENABLE, &ditherEnable);
            RwD3D8GetRenderState(D3DRS_SHADEMODE, &shadeMode);

            RwD3D8SetRenderState(D3DRS_TEXTUREFACTOR, 0xff000000);
            RwD3D8SetRenderState(D3DRS_DITHERENABLE, FALSE);
            RwD3D8SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
        }
    }

    /* Enable clipping */
    if (type == rpATOMIC)
    {
        RpAtomic                *atomic;
        RwCamera                *cam;

        atomic = (RpAtomic *)object;

        cam = RwCameraGetCurrentCamera();
        RWASSERT(cam);

        if (RwD3D8CameraIsSphereFullyInsideFrustum(cam, RpAtomicGetWorldBoundingSphere(atomic)))
        {
            RwD3D8SetRenderState(D3DRS_CLIPPING, FALSE);
        }
        else
        {
            RwD3D8SetRenderState(D3DRS_CLIPPING, TRUE);
        }
    }
    else
    {
        RpWorldSector   *worldSector;
        RwCamera        *cam;

        worldSector = (RpWorldSector *)object;

        cam = RwCameraGetCurrentCamera();
        RWASSERT(cam);

        if (RwD3D8CameraIsBBoxFullyInsideFrustum(cam, RpWorldSectorGetTightBBox(worldSector)))
        {
            RwD3D8SetRenderState(D3DRS_CLIPPING, FALSE);
        }
        else
        {
            RwD3D8SetRenderState(D3DRS_CLIPPING, TRUE);
        }
    }

    /* Set texture to NULL if hasn't any texture flags */
    if ( (flags & (rxGEOMETRY_TEXTURED | rpGEOMETRYTEXTURED2)) == 0)
    {
        RwD3D8SetTexture(NULL, 0);

        if (forceBlack)
        {
            RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
            RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

            RwD3D8SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
        }
    }

    /* Get vertex alpha Blend state */
    vertexAlphaBlend = _rwD3D8RenderStateIsVertexAlphaEnable();

    /* Set Last vertex buffer to force the call */
    lastVertexBuffer = (void *)0xffffffff;

    /* Get the instanced data */
    resEntryHeader = (RxD3D8ResEntryHeader *)(repEntry + 1);
    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);

    /*
     * Data shared between meshes
     */

    /*
     * Set the Default Pixel shader
     */
    RwD3D8SetPixelShader(0);

    /*
     * Vertex shader
     */
    RwD3D8SetVertexShader(instancedData->vertexShader);

    /* Get the number of meshes */
    numMeshes = resEntryHeader->numMeshes;
    while (numMeshes--)
    {
        RWASSERT(instancedData->material != NULL);

        if ( (flags & (rxGEOMETRY_TEXTURED | rpGEOMETRYTEXTURED2)) )
        {
            RwD3D8SetTexture(instancedData->material->texture, 0);

            if (forceBlack)
            {
                /* Only change the colorop, we need to use the texture alpha channel */
                RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
                RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
            }
        }

        if (instancedData->vertexAlpha ||
            (0xFF != instancedData->material->color.alpha))
        {
            if (!vertexAlphaBlend)
            {
                vertexAlphaBlend = TRUE;

                _rwD3D8RenderStateVertexAlphaEnable(TRUE);
            }
        }
        else
        {
            if (vertexAlphaBlend)
            {
                vertexAlphaBlend = FALSE;

                _rwD3D8RenderStateVertexAlphaEnable(FALSE);
            }
        }

        if (lighting)
        {
            if (instancedData->vertexAlpha)
            {
                RwD3D8SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
            }
            else
            {
                RwD3D8SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
            }

            RwD3D8SetSurfaceProperties(&instancedData->material->color,
                                        &instancedData->material->surfaceProps,
                                        (flags & rxGEOMETRY_MODULATE));
        }

        /*
         * Render
         */

        /* Set the stream source */
        if (lastVertexBuffer != instancedData->vertexBuffer)
        {
            RwD3D8SetStreamSource(0, instancedData->vertexBuffer, instancedData->stride);

            lastVertexBuffer = instancedData->vertexBuffer;
        }

        /* Set the Index buffer */
        if (instancedData->indexBuffer != NULL)
        {
            RwD3D8SetIndices(instancedData->indexBuffer, instancedData->baseIndex);

            /* Draw the indexed primitive */
            RwD3D8DrawIndexedPrimitive((D3DPRIMITIVETYPE)instancedData->primType,
                                       0, instancedData->numVertices,
                                       0, instancedData->numIndices);
        }
        else
        {
            RwD3D8DrawPrimitive((D3DPRIMITIVETYPE)instancedData->primType,
                                instancedData->baseIndex,
                                instancedData->numVertices);
        }

        /* Move onto the next instancedData */
        instancedData++;
    }

    if (forceBlack)
    {
        RwD3D8SetRenderState(D3DRS_DITHERENABLE, ditherEnable);
        RwD3D8SetRenderState(D3DRS_SHADEMODE, shadeMode);

        if (_rwD3D8RWGetRasterStage(0))
        {
            RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
            RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        }
        else
        {
            RwD3D8SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
            RwD3D8SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        }
    }

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneSetInstanceCallBack sets a callback
 * that will be called to instance.
 *
 * The callback is called once for the object being rendered.
 *
 * \param node The pipeline node to set the callback for.
 * \param callback The callback function.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetLightingCallBack
 * \see RxD3D8AllInOneSetRenderCallBack
 */
void
RxD3D8AllInOneSetInstanceCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneInstanceCallBack callback)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneSetInstanceCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->instanceCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneGetInstanceCallBack gets the currently set instance callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return An \ref RxD3D8AllInOneInstanceCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 *
 */
RxD3D8AllInOneInstanceCallBack
RxD3D8AllInOneGetInstanceCallBack(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneGetInstanceCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->instanceCallback);
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneSetReinstanceCallBack sets a callback
 * that will be called to reinstance.
 *
 * The callback is called each time the geometry of the current
 * atomic has been changed.
 *
 * \param node The pipeline node to set the callback for.
 * \param callback The callback function.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetLightingCallBack
 * \see RxD3D8AllInOneSetRenderCallBack
 */
void
RxD3D8AllInOneSetReinstanceCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneReinstanceCallBack callback)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneSetReinstanceCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->reinstanceCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneGetReinstanceCallBack gets the currently set reinstance
 * callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return An \ref RxD3D8AllInOneReinstanceCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 *
 */
RxD3D8AllInOneReinstanceCallBack
RxD3D8AllInOneGetReinstanceCallBack(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneGetReinstanceCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->reinstanceCallback);
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneSetLightingCallBack sets a callback
 * that will be called to set up lighting.
 *
 * The callback is called once for the object being rendered.
 *
 * \param node The \ref RxPipelineNode to set the callback for.
 * \param callback The \ref RxD3D8AllInOneLightingCallBack function.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetInstanceCallBack
 * \see RxD3D8AllInOneSetRenderCallBack
 * \ref RxD3D8AllInOneGetLightingCallBack
 */
void
RxD3D8AllInOneSetLightingCallBack(RxPipelineNode *node,
                                  RxD3D8AllInOneLightingCallBack callback)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneSetLightingCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->lightingCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneGetLightingCallBack gets the currently
 * set lighting callback. Useful if you plan to overload the
 * RenderWare lighting pipeline with your own.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return A \ref RxD3D8AllInOneLightingCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetLightingCallBack
 * \see RxD3D8AllInOneSetRenderCallBack
 * \see RxD3D8AllInOneGetRenderCallBack
 *
 */
RxD3D8AllInOneLightingCallBack
RxD3D8AllInOneGetLightingCallBack(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneGetLightingCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->lightingCallback);
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneSetRenderCallBack sets a callback that
 * will be called to dispatch vertices for rendering for every
 * material in the given object.
 *
 * \param node The \ref RxPipelineNode to set the callback for.
 * \param callback The \ref RxD3D8AllInOneRenderCallBack function.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetInstanceCallBack
 * \see RxD3D8AllInOneSetLightingCallBack
 * \ref RxD3D8AllInOneGetRenderCallBack
 */
void
RxD3D8AllInOneSetRenderCallBack(RxPipelineNode *node,
                                RxD3D8AllInOneRenderCallBack callback)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneSetRenderCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->renderCallback = callback;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d8
 * \ref RxD3D8AllInOneGetRenderCallBack gets the currently set
 * render callback. Useful if you plan to overload the default
 * RenderWare render callback.
 *
 * \param node The \ref RxPipelineNode to get the callback for.
 *
 * \return A \ref RxD3D8AllInOneRenderCallBack function pointer.
 *
 * \see RxNodeDefinitionGetD3D8AtomicAllInOne
 * \see RxNodeDefinitionGetD3D8WorldSectorAllInOne
 * \see RxD3D8AllInOneSetInstanceCallBack
 * \see RxD3D8AllInOneSetLightingCallBack
 * \ref RxD3D8AllInOneSetRenderCallBack
 */
RxD3D8AllInOneRenderCallBack
RxD3D8AllInOneGetRenderCallBack(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWAPIFUNCTION(RWSTRING("RxD3D8AllInOneGetRenderCallBack"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    RWRETURN(instanceData->renderCallback);
}
