
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

#include "D3D8pipe.h"
#include "D3D8VertexBufferManager.h"

#include "native.h"

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

static RwPlatformID Id = rwID_PCD3D8;

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
        RxD3D8ResEntryHeader    *resEntryHeader;
        RxD3D8InstanceData      *instancedData;
        RwInt32                 numMeshes;
        RwUInt32                size;

        RWASSERT(geometry->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + sizeof(RwInt32) + geometry->repEntry->size;

        resEntryHeader = (RxD3D8ResEntryHeader *) (geometry->repEntry + 1);

        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            size += instancedData->numIndices * sizeof(RxVertexIndex);
            size += instancedData->numVertices * instancedData->stride;

            instancedData++;
        }

        /* Convert the material pointers to index for serialization */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
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

        /* Write native mesh info */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            RxVertexIndex   *indexBuffer;
            RwUInt8         *vertexBuffer;

            /* Write index information */
            if (D3D_OK == IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                0, 0, (RwUInt8 **)&indexBuffer, 0))
            {
                void *rv;

                rv = RwStreamWrite(stream, (void *)indexBuffer, instancedData->numIndices * sizeof(RxVertexIndex));

                IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);

                if (!rv)
                {
                    RWRETURN(NULL);
                }
            }

            /* Write vertex information */
            if (D3D_OK == IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                                instancedData->baseIndex * instancedData->stride,
                                                instancedData->numVertices * instancedData->stride,
                                                &vertexBuffer,
                                                D3DLOCK_NOSYSLOCK))
            {
                void *rv;

                rv = RwStreamWrite(stream, (void *)vertexBuffer, instancedData->numVertices * instancedData->stride);

                IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

                if (!rv)
                {
                    RWRETURN(NULL);
                }
            }

            instancedData++;
        }

        /* Patch the material pointers back */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
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
    RwUInt32                size;
    RwUInt32                headerSize;
    RwPlatformID            id;
    RxD3D8ResEntryHeader    *resEntryHeader;
    RxD3D8InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  error;

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
    if (rwID_PCD3D8 != id)
    {
        RWASSERT(rwID_PCD3D8 == id);

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
                                      rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);
    RWASSERT(NULL != geometry->repEntry);

    /* We have an entry */
    geometry->repEntry->link.next = (RwLLLink *)NULL;
    geometry->repEntry->link.prev = (RwLLLink *)NULL;
    geometry->repEntry->owner = (void *)geometry;
    geometry->repEntry->size = headerSize;
    geometry->repEntry->ownerRef = &geometry->repEntry;
    geometry->repEntry->destroyNotify = _rwD3D8ResourceEntryInstanceDataDestroy;

    /* Read some native data */
    if (RwStreamRead(stream, (void *) (geometry->repEntry + 1), headerSize) != headerSize)
    {
        RWRETURN(NULL);
    }

    /* Remove header from size */
    size -= headerSize;

    /* Fix wrong pointers */
    resEntryHeader = (RxD3D8ResEntryHeader *) (geometry->repEntry + 1);

    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        instancedData->indexBuffer = NULL;
        instancedData->vertexBuffer = NULL;

        instancedData++;
    }

    /* Load index and vertex data */
    error = FALSE;

    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes-- && !error)
    {
        RwUInt32 bytesRead;

        error = TRUE;

        /* read index data */
        if (RwD3D8IndexBufferCreate(instancedData->numIndices, &(instancedData->indexBuffer)))
        {
            RwUInt32        ibSize;
            RxVertexIndex   *indexBuffer;

            ibSize = instancedData->numIndices * sizeof(RxVertexIndex);

            if (D3D_OK == IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                    0, 0, (RwUInt8 **)&indexBuffer, 0))
            {
                bytesRead = RwStreamRead(stream, (void *)indexBuffer, ibSize);

                IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);

                if (bytesRead == ibSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }

        /* read vertex data */
        if (!error)
        {
            RwUInt32        vbSize;
            RwUInt8         *vertexBuffer;

            error = TRUE;

            vbSize = instancedData->numVertices * instancedData->stride;

            if (geometry->numMorphTargets > 1)
            {
                instancedData->managed = FALSE;

                if (RwD3D8DynamicVertexBufferCreate(instancedData->vertexShader, vbSize,
                                                            &(instancedData->vertexBuffer)))
                {
                    error = FALSE;
                }
            }
            else
            {
                instancedData->managed = TRUE;

                if (_rxD3D8VertexBufferManagerCreate(instancedData->vertexShader, vbSize,
                                                            &instancedData->vertexBuffer,
                                                            &instancedData->baseIndex))
                {
                    error = FALSE;
                }
            }

            if (!error)
            {
                error = TRUE;

                if(instancedData->managed)
                {
                    IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                         instancedData->baseIndex * instancedData->stride,
                                         vbSize,
                                         &vertexBuffer,
                                         D3DLOCK_NOSYSLOCK);
                }
                else
                {
                    IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                         instancedData->baseIndex * instancedData->stride,
                                         vbSize,
                                         &vertexBuffer,
                                         D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                }

                bytesRead = RwStreamRead(stream, (void *)vertexBuffer, vbSize);

                IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

                if (bytesRead == vbSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }

        instancedData++;
    }

    if (error)
    {
        _rwD3D8ResourceEntryInstanceDataDestroy(geometry->repEntry);

        RWRETURN(NULL);
    }

    /* Patch the material pointers back */
    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        instancedData->material =
            _rpMaterialListGetMaterial(&geometry->matList,
                                       (RwInt32)
                                       (instancedData->material));
        instancedData++;
    }

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
        RxD3D8ResEntryHeader    *resEntryHeader;
        RxD3D8InstanceData      *instancedData;
        RwInt32                 numMeshes;

        /* Chunk header + resource entry size */
        RWASSERT(geometry->repEntry);

        RWASSERT(geometry->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + rwCHUNKHEADERSIZE + sizeof(RwInt32) + geometry->repEntry->size;

        resEntryHeader = (RxD3D8ResEntryHeader *) (geometry->repEntry + 1);

        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            size += instancedData->numIndices * sizeof(RxVertexIndex);
            size += instancedData->numVertices * instancedData->stride;

            instancedData++;
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
        RxD3D8ResEntryHeader    *resEntryHeader;
        RxD3D8InstanceData      *instancedData;
        RwInt32                 numMeshes;
        RwUInt32                size;

        RWASSERT(sector->repEntry);

        /* Calculate total size */
        size = sizeof(RwPlatformID) + sizeof(RwInt32) + sector->repEntry->size;

        resEntryHeader = (RxD3D8ResEntryHeader *) (sector->repEntry + 1);

        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            size += instancedData->numIndices * sizeof(RxVertexIndex);
            size += instancedData->numVertices * instancedData->stride;

            instancedData++;
        }

        /* Convert the material pointers to index for serialization */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
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

        /* Write native mesh info */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
        numMeshes = resEntryHeader->numMeshes;

        while (numMeshes--)
        {
            RxVertexIndex   *indexBuffer;
            RwUInt8         *vertexBuffer;

            /* Write index information */
            if (D3D_OK == IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                0, 0, (RwUInt8 **)&indexBuffer, 0))
            {
                void *rv;

                rv = RwStreamWrite(stream, (void *)indexBuffer, instancedData->numIndices * sizeof(RxVertexIndex));

                IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);

                if (!rv)
                {
                    RWRETURN(NULL);
                }
            }

            /* Write vertex information */
            if (D3D_OK == IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                                instancedData->baseIndex * instancedData->stride,
                                                instancedData->numVertices * instancedData->stride,
                                                &vertexBuffer,
                                                D3DLOCK_NOSYSLOCK))
            {
                void *rv;

                rv = RwStreamWrite(stream, (void *)vertexBuffer, instancedData->numVertices * instancedData->stride);

                IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

                if (!rv)
                {
                    RWRETURN(NULL);
                }
            }

            instancedData++;
        }

        /* Patch the material pointers back */
        instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
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
    RwUInt32                size;
    RwUInt32                headerSize;
    RwPlatformID            id;
    RxD3D8ResEntryHeader    *resEntryHeader;
    RxD3D8InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwBool                  error;

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
    if (rwID_PCD3D8 != id)
    {
        RWASSERT(rwID_PCD3D8 == id);

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
        rwID_WORLDPIPEMODULE | rwMEMHINTDUR_EVENT);
    RWASSERT(NULL != sector->repEntry);

    /* We have an entry */
    sector->repEntry->link.next = (RwLLLink *)NULL;
    sector->repEntry->link.prev = (RwLLLink *)NULL;
    sector->repEntry->owner = (void *)sector;
    sector->repEntry->size = headerSize;
    sector->repEntry->ownerRef = &sector->repEntry;
    sector->repEntry->destroyNotify = _rwD3D8ResourceEntryInstanceDataDestroy;

    /* Read some native data */
    if (RwStreamRead(stream, (void *) (sector->repEntry + 1), headerSize) != headerSize)
    {
        RWRETURN(NULL);
    }

    /* Remove header from size */
    size -= headerSize;

    /* Fix wrong pointers */
    resEntryHeader = (RxD3D8ResEntryHeader *) (sector->repEntry + 1);

    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes--)
    {
        instancedData->indexBuffer = NULL;
        instancedData->vertexBuffer = NULL;

        instancedData++;
    }

    /* Load index and vertex data */
    error = FALSE;

    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
    numMeshes = resEntryHeader->numMeshes;

    while (numMeshes-- && !error)
    {
        RwUInt32 bytesRead;

        error = TRUE;

        /* read index data */
        if (RwD3D8IndexBufferCreate(instancedData->numIndices, &(instancedData->indexBuffer)))
        {
            RwUInt32        ibSize;
            RxVertexIndex   *indexBuffer;

            ibSize = instancedData->numIndices * sizeof(RxVertexIndex);

            if (D3D_OK == IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                    0, 0, (RwUInt8 **)&indexBuffer, 0))
            {
                bytesRead = RwStreamRead(stream, (void *)indexBuffer, ibSize);

                IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);

                if (bytesRead == ibSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }

        /* read vertex data */
        if (!error)
        {
            RwUInt32        vbSize;
            RwUInt8         *vertexBuffer;

            error = TRUE;

            vbSize = instancedData->numVertices * instancedData->stride;

            instancedData->managed = TRUE;

            if (_rxD3D8VertexBufferManagerCreate(instancedData->vertexShader, vbSize,
                                                        &instancedData->vertexBuffer,
                                                        &instancedData->baseIndex))
            {
                error = FALSE;
            }

            if (!error)
            {
                error = TRUE;

                IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                     instancedData->baseIndex * instancedData->stride,
                                     vbSize,
                                     &vertexBuffer,
                                     D3DLOCK_NOSYSLOCK);

                bytesRead = RwStreamRead(stream, (void *)vertexBuffer, vbSize);

                IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

                if (bytesRead == vbSize)
                {
                    size -= bytesRead;

                    error = FALSE;
                }
            }
        }

        instancedData++;
    }

    if (error)
    {
        _rwD3D8ResourceEntryInstanceDataDestroy(sector->repEntry);

        RWRETURN(NULL);
    }

    /* Patch the material pointers back */
    instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
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

    if (NULL != sector->repEntry)
    {
        RpWorld *world;

        world = RpWorldSectorGetWorld(sector);

        if (rpWORLDNATIVE & RpWorldGetFlags(world))
        {
            RxD3D8ResEntryHeader    *resEntryHeader;
            RxD3D8InstanceData      *instancedData;
            RwInt32                 numMeshes;

            /* Calculate total size */

            /* Chunk header + resource entry size */
            size = sizeof(RwPlatformID) + rwCHUNKHEADERSIZE + sizeof(RwInt32) + sector->repEntry->size;

            resEntryHeader = (RxD3D8ResEntryHeader *) (sector->repEntry + 1);

            instancedData = (RxD3D8InstanceData *)(resEntryHeader + 1);
            numMeshes = resEntryHeader->numMeshes;

            while (numMeshes--)
            {
                size += instancedData->numIndices * sizeof(RxVertexIndex);
                size += instancedData->numVertices * instancedData->stride;

                instancedData++;
            }
        }
    }

    RWRETURN(size);
}
