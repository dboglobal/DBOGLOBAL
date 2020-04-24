
/****************************************************************************
 *                                                                          *
 * module : native.c                                                        *
 *                                                                          *
 * purpose: Write/Read native data                                          *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include "rwcore.h"
#include "baworld.h"

#include "D3D9pipe.h"

#include "native.h"

extern D3DCAPS9 _RwD3D9DeviceCaps;

/****************************************************************************
 Local defines
 */

/****************************************************************************
 Local types
 */

/****************************************************************************
 Externed globals
 */

/****************************************************************************
 Local variables
 */

static RwPlatformID Id = rwID_PCD3D9;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                   GEOMETRY

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

RwStream *
_rpGeometryNativeWrite(RwStream *stream, const RpGeometry *geometry)
{
    RWFUNCTION(RWSTRING("_rpGeometryNativeWrite"));
    RWASSERT(stream);
    RWASSERT(geometry);

    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        RxD3D9ResEntryHeader    *resEntryHeader;
        D3DVERTEXELEMENT9       declaration[20];
        RwUInt32                numElements;
        RxD3D9InstanceData      *instancedData;
        RwInt32                 numMeshes;
        RwUInt32                size, n;

        RWASSERT(geometry->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + sizeof(RwInt32) + geometry->repEntry->size;

        resEntryHeader = (RxD3D9ResEntryHeader *) (geometry->repEntry + 1);

        IDirect3DVertexDeclaration9_GetDeclaration((LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
                                                   declaration,
                                                   &numElements);

        RWASSERT(numElements < 20);
        size += sizeof(RwUInt32);
        size += numElements * sizeof(D3DVERTEXELEMENT9);

        if (resEntryHeader->indexBuffer != NULL)
        {
            size += (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex);
        }

        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            size += sizeof(RxD3D9VertexStream);
            if (resEntryHeader->vertexStream[n].vertexBuffer != NULL)
            {
                size += (resEntryHeader->totalNumVertex) * (resEntryHeader->vertexStream[n].stride);
            }
        }

        /* Convert the material pointers to index for serialization */
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            instancedData->material = (RpMaterial *)
                _rpMaterialListFindMaterialIndex(&geometry->matList,
                                                 instancedData->material);
            instancedData++;
        }

        /* Write a chunk header so we get a VERSION NUMBER */
        if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, size))
        {
            RWRETURN(NULL);
        }

        /* Write a platform unique identifier */
        if (!RwStreamWrite(stream, (const void *) &Id, sizeof(RwPlatformID)))
        {
            RWRETURN(NULL);
        }

        /* Write header size */
        if (!RwStreamWrite(stream, (void *) &(geometry->repEntry->size), sizeof(RwInt32)))
        {
            RWRETURN(NULL);
        }

        /* Write header */
        if (!RwStreamWrite(stream, (void *) (geometry->repEntry + 1),
                           geometry->repEntry->size))
        {
            RWRETURN(NULL);
        }

        /* Write vertex declaration */
        if (!RwStreamWrite(stream, (void *) &numElements, sizeof(RwUInt32)))
        {
            RWRETURN(NULL);
        }

        if (!RwStreamWrite(stream, (void *) declaration, numElements * sizeof(D3DVERTEXELEMENT9)))
        {
            RWRETURN(NULL);
        }

        /* write index data */
        if (resEntryHeader->indexBuffer != NULL)
        {
            RxVertexIndex   *indexBuffer = NULL;
            void            *rv = NULL;

            IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)resEntryHeader->indexBuffer,
                                       0, 0, (RwUInt8 **)&indexBuffer, 0);

            /* Write index information */
            if (indexBuffer != NULL)
            {
                rv = RwStreamWrite(stream,
                                   (void *)indexBuffer,
                                   (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex));

                IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)resEntryHeader->indexBuffer);
            }

            if (rv == NULL)
            {
                RWRETURN(NULL);
            }
        }

        /* Write vertex information */
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            RxD3D9VertexStream  *vertexStream;
            RwUInt8 *vertexBuffer;
            void    *rv;

            vertexStream = &(resEntryHeader->vertexStream[n]);

            rv = RwStreamWrite(stream, (void *)vertexStream, sizeof(RxD3D9VertexStream));
            if (!rv)
            {
                RWRETURN(NULL);
            }

            if (vertexStream->vertexBuffer != NULL)
            {
                if (D3D_OK == IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                                    vertexStream->offset,
                                                    (resEntryHeader->totalNumVertex) * (vertexStream->stride),
                                                    &vertexBuffer,
                                                    D3DLOCK_NOSYSLOCK))
                {
                    rv = RwStreamWrite(stream, (void *)vertexBuffer, (resEntryHeader->totalNumVertex) * (vertexStream->stride));

                    IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)vertexStream->vertexBuffer);

                    if (!rv)
                    {
                        RWRETURN(NULL);
                    }
                }
                else
                {
                    RWRETURN(NULL);
                }
            }
        }

        /* Patch the material pointers back */
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            instancedData->material =
                _rpMaterialListGetMaterial(&geometry->matList,
                                           (RwInt32)
                                           (instancedData->material));
            instancedData++;
        }
    }

    RWRETURN(stream);
}

RpGeometry *
_rpGeometryNativeRead(RwStream *stream __RWUNUSEDRELEASE__,
                      RpGeometry *geometry)
{
    RwUInt32                version;
    RwUInt32                size, n;
    RwUInt32                headerSize;
    RwPlatformID            id;
    RxD3D9ResEntryHeader    *resEntryHeader;
    D3DVERTEXELEMENT9       declaration[20];
    RwUInt32                numElements;
    RxD3D9InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  error;
    RwUInt32                numStreams;

    RWFUNCTION(RWSTRING("_rpGeometryNativeRead"));
    RWASSERT(stream);
    RWASSERT(geometry);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN(NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) ||
        (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN(NULL);
    }

    /* Read the platform unique identifier */
    if (RwStreamRead(stream, (void *) &id, sizeof(RwPlatformID)) !=
        sizeof(RwPlatformID))
    {
        RWRETURN(NULL);
    }

    /* Remove ID from size */
    size -= sizeof(RwPlatformID);

    /* Check this data is funky for this platfrom */
    if (rwID_PCD3D9 != id)
    {
        RWASSERT(rwID_PCD3D9 == id);

        RWRETURN(NULL);
    }

    /* Read the header size */
    if (RwStreamRead(stream, (void *) &headerSize, sizeof(RwInt32)) !=
        sizeof(RwInt32))
    {
        RWRETURN(NULL);
    }

    /* Remove header size from size */
    size -= sizeof(RwInt32);

    geometry->repEntry = (RwResEntry *)RwMalloc(sizeof(RwResEntry) + headerSize,
                                     rwMEMHINTDUR_EVENT | rwID_WORLDPIPEMODULE);
    RWASSERT(NULL != geometry->repEntry);

    /* We have an entry */
    geometry->repEntry->link.next = (RwLLLink *)NULL;
    geometry->repEntry->link.prev = (RwLLLink *)NULL;
    geometry->repEntry->owner = (void *)geometry;
    geometry->repEntry->size = headerSize;
    geometry->repEntry->ownerRef = &geometry->repEntry;
    geometry->repEntry->destroyNotify = _rwD3D9ResourceEntryInstanceDataDestroy;

    /* Read some native data */
    if (RwStreamRead(stream, (void *) (geometry->repEntry + 1), headerSize) != headerSize)
    {
        RWRETURN(NULL);
    }

    /* Remove header from size */
    size -= headerSize;

    /* Fix wrong pointers */
    resEntryHeader = (RxD3D9ResEntryHeader *) (geometry->repEntry + 1);

    resEntryHeader->indexBuffer = NULL;

    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        resEntryHeader->vertexStream[n].vertexBuffer = NULL;
    }

    resEntryHeader->vertexDeclaration = NULL;

    /* Read vertex declaration */
    if (RwStreamRead(stream, (void *) &numElements, sizeof(RwUInt32)) != sizeof(RwUInt32))
    {
        RWRETURN(NULL);
    }
    else
    {
        size -= sizeof(RwUInt32);
    }

    if (RwStreamRead(stream, (void *) declaration, numElements * sizeof(D3DVERTEXELEMENT9)) != (numElements * sizeof(D3DVERTEXELEMENT9)))
    {
        RWRETURN(NULL);
    }
    else
    {
        size -= numElements * sizeof(D3DVERTEXELEMENT9);
    }

    RwD3D9CreateVertexDeclaration(declaration, &(resEntryHeader->vertexDeclaration));

    /* Create index buffer */
    if (resEntryHeader->totalNumIndex)
    {
        if (RwD3D9IndexBufferCreate(resEntryHeader->totalNumIndex, &(resEntryHeader->indexBuffer)) == FALSE)
        {
            _rwD3D9ResourceEntryInstanceDataDestroy(geometry->repEntry);

            RWRETURN(NULL);
        }
    }

    if (resEntryHeader->indexBuffer != NULL)
    {
        RxVertexIndex   *indexBuffer = NULL;
        RwUInt32        bytesRead = 0;
        RwUInt32        ibSize;

        IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)resEntryHeader->indexBuffer,
                                   0, 0, (RwUInt8 **)&indexBuffer, 0);

        /* read index data */
        ibSize = (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex);

        if (indexBuffer != NULL)
        {
            bytesRead = RwStreamRead(stream, (void *)indexBuffer, ibSize);

            IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)resEntryHeader->indexBuffer);
        }

        if (bytesRead == ibSize)
        {
            size -= bytesRead;
        }
        else
        {
            _rwD3D9ResourceEntryInstanceDataDestroy(geometry->repEntry);

            RWRETURN(NULL);
        }
    }

    /* read vertex data */
    error = TRUE;
    numStreams = 0;

    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        RxD3D9VertexStream  *vertexStream;
        RwUInt32    vbSize;
        RwUInt8     *vertexBuffer;
        RwUInt32    bytesRead;

        vertexStream = &(resEntryHeader->vertexStream[n]);

        bytesRead = RwStreamRead(stream, (void *)vertexStream, sizeof(RxD3D9VertexStream));

        if (bytesRead != sizeof(RxD3D9VertexStream))
        {
            error = TRUE;
            break;
        }
        else
        {
            size -= bytesRead;
        }

        if (vertexStream->vertexBuffer != NULL)
        {
            numStreams++;

            vbSize = (resEntryHeader->totalNumVertex) * (vertexStream->stride);

            if (geometry->numMorphTargets > 1)
            {
                vertexStream->managed = FALSE;

                if (RwD3D9DynamicVertexBufferCreate(vbSize,
                                                    &(vertexStream->vertexBuffer)))
                {
                    error = FALSE;
                }
            }
            else
            {
                vertexStream->managed = TRUE;

                if (RwD3D9CreateVertexBuffer(vertexStream->stride, vbSize,
                                             &vertexStream->vertexBuffer,
                                             &vertexStream->offset))
                {
                    error = FALSE;
                }
            }

            if (!error)
            {
                error = TRUE;

                if(vertexStream->managed)
                {
                    IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                        vertexStream->offset,
                                        vbSize,
                                        &vertexBuffer,
                                        D3DLOCK_NOSYSLOCK);
                }
                else
                {
                    IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                        vertexStream->offset,
                                        vbSize,
                                        &vertexBuffer,
                                        D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                }

                bytesRead = RwStreamRead(stream, (void *)vertexBuffer, vbSize);

                IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)vertexStream->vertexBuffer);

                if (bytesRead == vbSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }
    }

    if (error)
    {
        _rwD3D9ResourceEntryInstanceDataDestroy(geometry->repEntry);

        RWRETURN(NULL);
    }

    /* Fix base index */
    if (numStreams > 1)
    {
        if (_RwD3D9DeviceCaps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)
        {
            resEntryHeader->useOffsets = TRUE;
        }

        numMeshes = resEntryHeader->numMeshes;
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

        do
        {
            instancedData->baseIndex = instancedData->minVert;

            instancedData++;
        }
        while (--numMeshes);
    }
    else
    {
        resEntryHeader->useOffsets = FALSE;

        numMeshes = resEntryHeader->numMeshes;
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

        do
        {
            instancedData->baseIndex = instancedData->minVert +
                                        (resEntryHeader->vertexStream[0].offset / resEntryHeader->vertexStream[0].stride);

            instancedData++;
        }
        while (--numMeshes);
    }

    /* Patch the material pointers back */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        instancedData->material =
            _rpMaterialListGetMaterial(&geometry->matList,
                                       (RwInt32)
                                       (instancedData->material));
        instancedData++;
    }

    RWASSERT(size == 0);

    RWRETURN(geometry);
}

RwInt32
_rpGeometryNativeSize(const RpGeometry *geometry)
{
    RwUInt32            size = 0;

    RWFUNCTION(RWSTRING("_rpGeometryNativeSize"));
    RWASSERT(geometry);

    /* Calculate the native data size */
    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        RxD3D9ResEntryHeader    *resEntryHeader;
        D3DVERTEXELEMENT9   declaration[20];
        RwUInt32            numElements, n;

        /* Chunk header + resource entry size */
        RWASSERT(geometry->repEntry);

        RWASSERT(geometry->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + rwCHUNKHEADERSIZE + sizeof(RwInt32) + geometry->repEntry->size;

        resEntryHeader = (RxD3D9ResEntryHeader *) (geometry->repEntry + 1);

        IDirect3DVertexDeclaration9_GetDeclaration((LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
                                                   declaration,
                                                   &numElements);

        RWASSERT(numElements < 20);
        size += sizeof(RwUInt32);
        size += numElements * sizeof(D3DVERTEXELEMENT9);

        size += resEntryHeader->totalNumIndex * sizeof(RxVertexIndex);

        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            size += sizeof(RxD3D9VertexStream);
            if (resEntryHeader->vertexStream[n].vertexBuffer != NULL)
            {
                size += (resEntryHeader->totalNumVertex) * (resEntryHeader->vertexStream[n].stride);
            }
        }
    }

    RWRETURN(size);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                 WORLD SECTOR

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

RwStream *
_rpWorldSectorNativeWrite(RwStream *stream, const RpWorldSector *sector)
{
    RpWorld *world;

    RWFUNCTION(RWSTRING("_rpWorldSectorNativeWrite"));
    RWASSERT(NULL != stream);
    RWASSERT(NULL != sector);

    world = RpWorldSectorGetWorld(sector);

    /* Write some native data */
    if (rpWORLDNATIVE & RpWorldGetFlags(world))
    {
        RxD3D9ResEntryHeader    *resEntryHeader;
        D3DVERTEXELEMENT9       declaration[20];
        RwUInt32                numElements;
        RxD3D9InstanceData      *instancedData;
        RwInt32                 numMeshes;
        RwUInt32                size, n;

        RWASSERT(sector->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + sizeof(RwInt32) + sector->repEntry->size;

        resEntryHeader = (RxD3D9ResEntryHeader *) (sector->repEntry + 1);

        IDirect3DVertexDeclaration9_GetDeclaration((LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
                                                   declaration,
                                                   &numElements);

        RWASSERT(numElements < 20);
        size += sizeof(RwUInt32);
        size += numElements * sizeof(D3DVERTEXELEMENT9);

        if (resEntryHeader->indexBuffer != NULL)
        {
            size += (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex);
        }

        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            size += sizeof(RxD3D9VertexStream);
            if (resEntryHeader->vertexStream[n].vertexBuffer != NULL)
            {
                size += (resEntryHeader->totalNumVertex) * (resEntryHeader->vertexStream[n].stride);
            }
        }

        /* Convert the material pointers to index for serialization */
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            instancedData->material = (RpMaterial *)
                _rpMaterialListFindMaterialIndex(&world->matList,
                                                 instancedData->material);
            instancedData++;
        }

        /* Write a chunk header so we get a VERSION NUMBER */
        if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, size))
        {
            RWRETURN(NULL);
        }

        /* Write a platform unique identifier */
        if (!RwStreamWrite(stream, (const void *) &Id, sizeof(RwPlatformID)))
        {
            RWRETURN(NULL);
        }

        /* Write header size */
        if (!RwStreamWrite(stream, (void *) &(sector->repEntry->size), sizeof(RwInt32)))
        {
            RWRETURN(NULL);
        }

        /* Write header */
        if (!RwStreamWrite(stream, (void *) (sector->repEntry + 1),
                           sector->repEntry->size))
        {
            RWRETURN(NULL);
        }

        /* Write vertex declaration */
        if (!RwStreamWrite(stream, (void *) &numElements, sizeof(RwUInt32)))
        {
            RWRETURN(NULL);
        }

        if (!RwStreamWrite(stream, (void *) declaration, numElements * sizeof(D3DVERTEXELEMENT9)))
        {
            RWRETURN(NULL);
        }

        /* write index buffer */
        if (resEntryHeader->indexBuffer != NULL)
        {
            RxVertexIndex   *indexBuffer = NULL;
            void            *rv = NULL;

            IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)resEntryHeader->indexBuffer,
                                       0, 0, (RwUInt8 **)&indexBuffer, 0);


            if (indexBuffer != NULL)
            {
                rv = RwStreamWrite(stream, (void *)indexBuffer, (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex));
            }

            IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)resEntryHeader->indexBuffer);

            if (rv == NULL)
            {
                RWRETURN(NULL);
            }
        }

        /* Write vertex information */
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            RxD3D9VertexStream  *vertexStream;
            RwUInt8 *vertexBuffer;
            void    *rv;

            vertexStream = &(resEntryHeader->vertexStream[n]);

            rv = RwStreamWrite(stream, (void *)vertexStream, sizeof(RxD3D9VertexStream));
            if (!rv)
            {
                RWRETURN(NULL);
            }

            if (vertexStream->vertexBuffer != NULL)
            {
                if (D3D_OK == IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                                    vertexStream->offset,
                                                    (resEntryHeader->totalNumVertex) * (vertexStream->stride),
                                                    &vertexBuffer,
                                                    D3DLOCK_NOSYSLOCK))
                {
                    rv = RwStreamWrite(stream, (void *)vertexBuffer, (resEntryHeader->totalNumVertex) * (vertexStream->stride));

                    IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)vertexStream->vertexBuffer);

                    if (!rv)
                    {
                        RWRETURN(NULL);
                    }
                }
            }
        }

        /* Patch the material pointers back */
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            instancedData->material =
                _rpMaterialListGetMaterial(&world->matList,
                                           (RwInt32)
                                           (instancedData->material));
            instancedData++;
        }
    }

    RWRETURN(stream);
}

RpWorldSector *
_rpWorldSectorNativeRead(RwStream *stream, RpWorldSector *sector)
{
    RpWorld                 *world;
    RwUInt32                version;
    RwUInt32                size, n;
    RwUInt32                headerSize;
    RwPlatformID            id;
    RxD3D9ResEntryHeader    *resEntryHeader;
    D3DVERTEXELEMENT9       declaration[20];
    RwUInt32                numElements;
    RxD3D9InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  error;
    RwUInt32                numStreams;

    RWFUNCTION(RWSTRING("_rpWorldSectorNativeRead"));
    RWASSERT(stream);
    RWASSERT(sector);

    world = RpWorldSectorGetWorld(sector);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN(NULL);
    }

    if ((version < rwLIBRARYBASEVERSION) ||
        (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN(NULL);
    }

    /* Read the platform unique identifier */
    if (RwStreamRead(stream, (void *) &id, sizeof(RwPlatformID)) !=
        sizeof(RwPlatformID))
    {
        RWRETURN(NULL);
    }

    /* Remove ID from size */
    size -= sizeof(RwPlatformID);

    /* Check this data is funky for this platfrom */
    if (rwID_PCD3D9 != id)
    {
        RWASSERT(rwID_PCD3D9 == id);

        RWRETURN(NULL);
    }

    /* Read the header size */
    if (RwStreamRead(stream, (void *) &headerSize, sizeof(RwInt32)) !=
        sizeof(RwInt32))
    {
        RWRETURN(NULL);
    }

    /* Remove header size from size */
    size -= sizeof(RwInt32);

    sector->repEntry = (RwResEntry *)RwMalloc(sizeof(RwResEntry) + headerSize,
                                   rwMEMHINTDUR_EVENT | rwID_WORLDPIPEMODULE);
    RWASSERT(NULL != sector->repEntry);

    /* We have an entry */
    sector->repEntry->link.next = (RwLLLink *)NULL;
    sector->repEntry->link.prev = (RwLLLink *)NULL;
    sector->repEntry->owner = (void *)sector;
    sector->repEntry->size = headerSize;
    sector->repEntry->ownerRef = &sector->repEntry;
    sector->repEntry->destroyNotify = _rwD3D9ResourceEntryInstanceDataDestroy;

    /* Read some native data */
    if (RwStreamRead(stream, (void *) (sector->repEntry + 1), headerSize) != headerSize)
    {
        RWRETURN(NULL);
    }

    /* Remove header from size */
    size -= headerSize;

    /* Fix wrong pointers */
    resEntryHeader = (RxD3D9ResEntryHeader *) (sector->repEntry + 1);

    resEntryHeader->indexBuffer = NULL;

    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        resEntryHeader->vertexStream[n].vertexBuffer = NULL;
    }

    resEntryHeader->vertexDeclaration = NULL;

    /* Read vertex declaration */
    if (RwStreamRead(stream, (void *) &numElements, sizeof(RwUInt32)) != sizeof(RwUInt32))
    {
        RWRETURN(NULL);
    }
    else
    {
        size -= sizeof(RwUInt32);
    }

    if (RwStreamRead(stream, (void *) declaration, numElements * sizeof(D3DVERTEXELEMENT9)) != (numElements * sizeof(D3DVERTEXELEMENT9)))
    {
        RWRETURN(NULL);
    }
    else
    {
        size -= numElements * sizeof(D3DVERTEXELEMENT9);
    }

    RwD3D9CreateVertexDeclaration(declaration, &(resEntryHeader->vertexDeclaration));

    /* Create index buffer */
    if (resEntryHeader->totalNumIndex)
    {
        if (RwD3D9IndexBufferCreate(resEntryHeader->totalNumIndex, &(resEntryHeader->indexBuffer)) == FALSE)
        {
            _rwD3D9ResourceEntryInstanceDataDestroy(sector->repEntry);

            RWRETURN(NULL);
        }
    }

    if (resEntryHeader->indexBuffer != NULL)
    {
        RxVertexIndex   *indexBuffer = NULL;
        RwUInt32        bytesRead = 0;
        RwUInt32        ibSize;

        IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)resEntryHeader->indexBuffer,
                                0, 0, (RwUInt8 **)&indexBuffer, 0);

        ibSize = (resEntryHeader->totalNumIndex) * sizeof(RxVertexIndex);

        if (indexBuffer != NULL)
        {
            bytesRead = RwStreamRead(stream, (void *)indexBuffer, ibSize);

            IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)resEntryHeader->indexBuffer);
        }

        if (bytesRead == ibSize)
        {
            size -= bytesRead;
        }
        else
        {
            _rwD3D9ResourceEntryInstanceDataDestroy(sector->repEntry);

            RWRETURN(NULL);
        }
    }

    /* read vertex data */
    error = TRUE;
    numStreams = 0;

    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        RxD3D9VertexStream  *vertexStream;
        RwUInt32        bytesRead;
        RwUInt32        vbSize;
        RwUInt8         *vertexBuffer;

        vertexStream = &(resEntryHeader->vertexStream[n]);

        bytesRead = RwStreamRead(stream, (void *)vertexStream, sizeof(RxD3D9VertexStream));

        if (bytesRead != sizeof(RxD3D9VertexStream))
        {
            error = TRUE;
            break;
        }

        if (vertexStream->vertexBuffer != NULL)
        {
            numStreams++;

            vbSize = (resEntryHeader->totalNumVertex) * (vertexStream->stride);

            vertexStream->managed = TRUE;

            if (RwD3D9CreateVertexBuffer(vertexStream->stride, vbSize,
                                            &vertexStream->vertexBuffer,
                                            &vertexStream->offset))
            {
                error = FALSE;
            }

            if (!error)
            {
                error = TRUE;

                IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                    vertexStream->offset,
                                    vbSize,
                                    &vertexBuffer,
                                    D3DLOCK_NOSYSLOCK);

                bytesRead = RwStreamRead(stream, (void *)vertexBuffer, vbSize);

                IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)vertexStream->vertexBuffer);

                if (bytesRead == vbSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }
    }

    if (error)
    {
        _rwD3D9ResourceEntryInstanceDataDestroy(sector->repEntry);

        RWRETURN(NULL);
    }

    /* Fix base index */
    if (numStreams > 1)
    {
        if (_RwD3D9DeviceCaps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)
        {
            resEntryHeader->useOffsets = TRUE;
        }

        numMeshes = resEntryHeader->numMeshes;
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

        do
        {
            instancedData->baseIndex = instancedData->minVert;

            instancedData++;
        }
        while (--numMeshes);
    }
    else
    {
        resEntryHeader->useOffsets = FALSE;

        numMeshes = resEntryHeader->numMeshes;
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

        do
        {
            instancedData->baseIndex = instancedData->minVert +
                                        (resEntryHeader->vertexStream[0].offset / resEntryHeader->vertexStream[0].stride);

            instancedData++;
        }
        while (--numMeshes);
    }

    /* Patch the material pointers back */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        instancedData->material =
            _rpMaterialListGetMaterial(&world->matList,
                                       (RwInt32)
                                       (instancedData->material));
        instancedData++;
    }

    RWRETURN(sector);
}

RwInt32
_rpWorldSectorNativeSize(const RpWorldSector *sector)
{
    RwInt32 size = 0;

    RWFUNCTION(RWSTRING("_rpWorldSectorNativeSize"));
    RWASSERT(NULL != sector);

    /*
     * Calculate the native data size
     */
    if (NULL != sector->repEntry)
    {
        RpWorld *world;

        world = RpWorldSectorGetWorld(sector);

        if (rpWORLDNATIVE & RpWorldGetFlags(world))
        {
            RxD3D9ResEntryHeader    *resEntryHeader;
            D3DVERTEXELEMENT9       declaration[20];
            RwUInt32                numElements, n;

            /* Calculate total size */

            /* Chunk header + resource entry size */
            size = sizeof(RwPlatformID) + rwCHUNKHEADERSIZE + sizeof(RwInt32) + sector->repEntry->size;

            resEntryHeader = (RxD3D9ResEntryHeader *) (sector->repEntry + 1);

            IDirect3DVertexDeclaration9_GetDeclaration((LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
                                                    declaration,
                                                    &numElements);

            RWASSERT(numElements < 20);
            size += sizeof(RwUInt32);
            size += numElements * sizeof(D3DVERTEXELEMENT9);

            size += resEntryHeader->totalNumIndex * sizeof(RxVertexIndex);

            for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
            {
                size += sizeof(RxD3D9VertexStream);
                if (resEntryHeader->vertexStream[n].vertexBuffer != NULL)
                {
                    size += (resEntryHeader->totalNumVertex) * (resEntryHeader->vertexStream[n].stride);
                }
            }
        }
    }

    RWRETURN(size);
}
