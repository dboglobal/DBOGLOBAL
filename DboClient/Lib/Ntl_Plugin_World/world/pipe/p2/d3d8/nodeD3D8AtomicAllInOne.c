/****************************************************************************
 *                                                                          *
 * module : nodeD3D8AtomicAllInOne.c                                        *
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

#include "rpcriter.h"

#include "D3D8pipe.h"
#include "D3D8lights.h"

#include "wrldpipe.h"

#include "D3D8VertexBufferManager.h"

#include "nodeD3D8AtomicAllInOne.h"

/****************************************************************************
 Global variables
 */

/****************************************************************************
 Local defines
 */

#define NUMCLUSTERSOFINTEREST   0
#define NUMOUTPUTS              0

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/****************************************************************************
 _rwD3D8AtomicDefaultLightingCallback

 Purpose:   Enable lights.  If any lights are present they will be set and
            lighting will be enabled.

 On entry:  atomic - Pointer to the atomic to apply lighting to.
 .
 On exit :
*/
void
_rwD3D8AtomicDefaultLightingCallback(void *object)
{
    RpAtomic        *atomic;
    RpGeometryFlag  flags;

    RWFUNCTION(RWSTRING("_rwD3D8AtomicDefaultLightingCallback"));

    atomic = (RpAtomic *)object;
    flags = (RpGeometryFlag)
        RpGeometryGetFlags(RpAtomicGetGeometry(atomic));

    if ((flags & rxGEOMETRY_LIGHT) && (NULL != RWSRCGLOBAL(curWorld)))
    {
        RpWorld     *world;
        RwLLLink    *cur, *end;
        RwBool      lighting = FALSE;

        /*
         * Global lights, (Directional & Ambient)
         */
        AmbientSaturated.red = 0.f;
        AmbientSaturated.green = 0.f;
        AmbientSaturated.blue = 0.f;
        AmbientSaturated.alpha = 1.f;

        world = (RpWorld *)RWSRCGLOBAL(curWorld);

        cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
        end = rwLinkListGetTerminator(&world->directionalLightList);
        while (cur != end)
        {
            RpLight *light;

            light = rwLLLinkGetData(cur, RpLight, inWorld);

            /* NB light may actually be a dummyTie from a enclosing ForAll */
            if (light && (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS)))
            {
                if (RpLightGetType(light) == rpLIGHTDIRECTIONAL)
                {
                    if (flags & rxGEOMETRY_NORMALS)
                    {
                        _rwD3D8LightDirectionalEnable(light);

                        lighting = TRUE;
                    }
                }
                else
                {
                    const RwRGBAReal    *color;

                    color = RpLightGetColor(light);

                    /* Accumulate ambient light color */
                    AmbientSaturated.red += color->red;
                    AmbientSaturated.green += color->green;
                    AmbientSaturated.blue += color->blue;

                    lighting = TRUE;
                }
            }

            /* Next */
            cur = rwLLLinkGetNext(cur);
        }

        /* Set the global ambient color to white if have lights */
        if(lighting)
        {
            RwD3D8SetRenderState(D3DRS_AMBIENT, 0xffffffff);
        }
        else
        {
            RwD3D8SetRenderState(D3DRS_AMBIENT, 0);
        }

        /*
         * Local Lights, (Point, Soft & Soft spot)
         */
        if (flags & rxGEOMETRY_NORMALS)
        {
            /* Increase the marker ! */
            RWSRCGLOBAL(lightFrame)++;

            /* For all sectors that this atomic lies in, apply all lights within */
            cur = rwLinkListGetFirstLLLink(&atomic->llWorldSectorsInAtomic);
            end = rwLinkListGetTerminator(&atomic->llWorldSectorsInAtomic);
            while (cur != end)
            {
                RpTie       *tpTie = rwLLLinkGetData(cur, RpTie, lWorldSectorInAtomic);
                RwLLLink    *curLight, *endLight;

                /* Lights in the sector */
                curLight = rwLinkListGetFirstLLLink(&tpTie->worldSector->lightsInWorldSector);
                endLight = rwLinkListGetTerminator(&tpTie->worldSector->lightsInWorldSector);

                while (curLight != endLight)
                {
                    RpLightTie  *lightTie;

                    lightTie = rwLLLinkGetData(curLight, RpLightTie, lightInWorldSector);

                    /* NB lightTie may actually be a dummyTie from a enclosing ForAll */

                    /* Check to see if the light has already been applied and is set to
                    * light atomics
                    */
                    if (lightTie->light &&
                        (lightTie->light->lightFrame != RWSRCGLOBAL(lightFrame)) &&
                        (rwObjectTestFlags(lightTie->light, rpLIGHTLIGHTATOMICS)))
                    {
                        const RwMatrix  *matrixLight;
                        const RwV3d     *pos;
                        const RwSphere  *sphere;
                        RwV3d           distanceVector;
                        RwReal          distanceSquare;
                        RwReal          distanceCollision;

                        /* don't light this atomic with the same light again! */
                        lightTie->light->lightFrame = RWSRCGLOBAL(lightFrame);

                        /* Does the light intersect the atomics bounding sphere */
                        matrixLight = RwFrameGetLTM(RpLightGetFrame(lightTie->light));

                        pos = &(matrixLight->pos);

                        sphere = RpAtomicGetWorldBoundingSphere(atomic);

                        RwV3dSub(&distanceVector, &(sphere->center), pos);

                        distanceSquare = RwV3dDotProduct(&distanceVector, &distanceVector);

                        distanceCollision = (sphere->radius + RpLightGetRadius(lightTie->light));

                        if (distanceSquare < (distanceCollision * distanceCollision))
                        {
                            lighting |= _rwD3D8LightLocalEnable(lightTie->light);
                        }
                    }

                    /* Next */
                    curLight = rwLLLinkGetNext(curLight);
                }

                /* Next one */
                cur = rwLLLinkGetNext(cur);
            }
        }

        /* Disable all unwanted lights and activate lighting if needed */
        _rwD3D8LightsEnable(lighting, rpATOMIC);
    }
    else
    {
        /* Disable all lights */
        _rwD3D8LightsEnable(FALSE, rpATOMIC);
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8InstanceDataMorphUpdate

 Purpose:   Modifies a vertex buffer to accomidate morph target animation.
            & Update data that has been changed since last lock

 On entry:  repEntry    - Pointer to a resource entry
            meshHeader  - Pointer to a mesh header
            geometry    - Pointer to the geometery to update morphs animation
            interp      - Pointer to the interpolator to use.

 On exit :  TRUE on success else FALSE.
*/
static void
D3D8InstanceDataMorphUpdate(RwResEntry *repEntry,
                             const RpMeshHeader *meshHeader,
                             const RpGeometry *geometry,
                             const RpInterpolator *interp)
{
    RwUInt32            numMeshes;
    RxD3D8InstanceData  *instancedData;
    const RpMesh        *mesh;
    RwInt32             startMT, endMT;
    RwReal              scale;
    RpGeometryFlag      flags;
    RwUInt32            numTextureCoords;

    RWFUNCTION(RWSTRING("D3D8InstanceDataMorphUpdate"));

    startMT = interp->startMorphTarget;
    endMT = interp->endMorphTarget;

    if ((startMT >= geometry->numMorphTargets) ||
        (endMT >= geometry->numMorphTargets))
    {
        /* Clamp to in range */
        startMT = endMT = 0;
    }

    scale = interp->recipTime * interp->position;

    flags = (RpGeometryFlag)
        RpGeometryGetFlags(geometry);

    numTextureCoords = RpGeometryGetNumTexCoordSets(geometry);

    /* Get the first RxD3D8InstanceData pointer */
    instancedData = (RxD3D8InstanceData *)(((RwUInt16 *)(repEntry + 1)) + 2);

    mesh = (const RpMesh *)(meshHeader + 1);
    numMeshes = meshHeader->numMeshes;
    while (numMeshes--)
    {
        RwUInt32        numVertices;
        RwUInt32        vbSize;
        RwInt32         stride;
        RwUInt8         *vertexData;
        RwUInt8         *data;
        RwBool          updateEverything;

        stride = instancedData->stride;
        vbSize = stride * instancedData->numVertices;

        if(instancedData->managed)
        {
            updateEverything = FALSE;

            IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                 instancedData->baseIndex * instancedData->stride, vbSize, &vertexData,
                                 D3DLOCK_NOSYSLOCK);
        }
        else
        {
            updateEverything = TRUE;

            IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                                 instancedData->baseIndex * instancedData->stride, vbSize, &vertexData,
                                 D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
        }

        /* Set the positions */
        data = vertexData;

        if (startMT == endMT || scale == 0.f)
        {
            const RpMorphTarget *morphTarget;
            const RwV3d         *vertex;

            morphTarget = &geometry->morphTarget[startMT];

            vertex = &morphTarget->verts[instancedData->minVert];
            numVertices = instancedData->numVertices;

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                const RwV3d *normal;

                normal = &morphTarget->normals[instancedData->minVert];
                while (numVertices--)
                {
                    *((RwV3d *)data) = *vertex;
                    *(((RwV3d *)data) + 1) = *normal;
                    data += stride;
                    vertex++;
                    normal++;
                }
            }
            else
            {
                while (numVertices--)
                {
                    *((RwV3d *)data) = *vertex;
                    data += stride;
                    vertex++;
                }
            }
        }
        else
        {
            const RwV3d *vertex1;
            const RwV3d *vertex2;
            RwV3d       *vertex;
            RwV3d       *normal;

            vertex1 = &geometry->morphTarget[startMT].verts[instancedData->minVert];
            vertex2 = &geometry->morphTarget[endMT].verts[instancedData->minVert];

            numVertices = instancedData->numVertices;

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                const RwV3d *normal1;
                const RwV3d *normal2;

                normal1 = &geometry->morphTarget[startMT].normals[instancedData->minVert];
                normal2 = &geometry->morphTarget[endMT].normals[instancedData->minVert];

                while (numVertices--)
                {
                    vertex = ((RwV3d *)data);
                    normal = ((RwV3d *)data) + 1;

                    vertex->x = vertex1->x + (vertex2->x - vertex1->x) * scale;
                    vertex->y = vertex1->y + (vertex2->y - vertex1->y) * scale;
                    vertex->z = vertex1->z + (vertex2->z - vertex1->z) * scale;

                    normal->x = normal1->x + (normal2->x - normal1->x) * scale;
                    normal->y = normal1->y + (normal2->y - normal1->y) * scale;
                    normal->z = normal1->z + (normal2->z - normal1->z) * scale;

                    data += stride;

                    vertex1++;
                    vertex2++;
                    normal1++;
                    normal2++;
                }
            }
            else
            {
                while (numVertices--)
                {
                    vertex = ((RwV3d *)data);

                    vertex->x = vertex1->x + (vertex2->x - vertex1->x) * scale;
                    vertex->y = vertex1->y + (vertex2->y - vertex1->y) * scale;
                    vertex->z = vertex1->z + (vertex2->z - vertex1->z) * scale;

                    data += stride;

                    vertex1++;
                    vertex2++;
                }
            }
        }

        /* update rest of the dirty information */
        if ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKPRELIGHT) ||
            (updateEverything && (flags & rxGEOMETRY_PRELIT)))
        {
            RwInt32         offset;
            const RwRGBA    *color;
            const RwRGBA    *matColor;
            RwUInt8         *vertexBuffer;
            RwInt32         alpha;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            color = (const RwRGBA *)(((const RwUInt8 *)(&geometry->preLitLum[instancedData->minVert])));
            matColor = RpMaterialGetColor(instancedData->material);

            vertexBuffer = vertexData + offset;
            numVertices = instancedData->numVertices;

            alpha = 0xff;

            if ((rpGEOMETRYMODULATEMATERIALCOLOR & RpGeometryGetFlags(geometry)) &&
                (*((const RwUInt32 *)matColor) != 0xffffffff))
            {
                while (numVertices--)
                {
                    *((RwUInt32 *)vertexBuffer) =
                        (((color->alpha * (matColor->alpha + 1)) & 0xff00) << (24-8)) |
                        (((color->red * (matColor->red + 1)) & 0xff00) << (16-8)) |
                        (((color->green * (matColor->green + 1)) & 0xff00) << (8-8)) |
                        ((color->blue * (matColor->blue + 1)) >> 8);

                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    vertexBuffer += instancedData->stride;
                    color++;
                }
            }
            else
            {
                while (numVertices--)
                {
                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    *((RwUInt32 *)vertexBuffer) = ((color->alpha << 24) |
                                                   (color->red << 16) |
                                                   (color->green << 8) |
                                                   (color->blue));

                    vertexBuffer += instancedData->stride;
                    color++;
                }
            }

            instancedData->vertexAlpha = (alpha != 0xff);
        }
        else
        {
            instancedData->vertexAlpha = FALSE;
        }

        /* Texture coordinates */
        if ( numTextureCoords &&
            ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKTEXCOORDSALL) || updateEverything) )
        {
            RwUInt32    offset;
            const RwTexCoords *texCoord;
            RwUInt32    i;
            RwUInt8     *vertexBuffer;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            if (rpGEOMETRYPRELIT & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwRGBA);
            }

            for (i = 0; i < numTextureCoords; i++)
            {
                if ((geometry->lockedSinceLastInst & (rpGEOMETRYLOCKTEXCOORDS1 << i)) || updateEverything)
                {
                    texCoord = (const RwTexCoords *)(((const RwUInt8 *)(&geometry->texCoords[i][instancedData->minVert])));

                    vertexBuffer = vertexData + offset;
                    numVertices = instancedData->numVertices;
                    while (numVertices--)
                    {
                        *((RwTexCoords *)vertexBuffer) = *texCoord;
                        vertexBuffer += instancedData->stride;
                        texCoord++;
                    }
                }

                offset += sizeof(RwTexCoords);
            }
        }

        /* Unlock the vertex buffer */
        IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

        /* Move onto the next instancedData & mesh */
        instancedData++;
        mesh++;
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8AtomicDefaultInstanceCallback

 Purpose:   To write vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D8AtomicDefaultInstanceCallback(void *object,
                                   RxD3D8InstanceData *instancedData,
                                   RwBool reinstance)
{
    RxVertexIndex       *indexBuffer = NULL;
    RxVertexIndex       *pRemapTable = NULL;
    RwInt32             n;
    const RpAtomic      *atomic;
    const RpGeometry    *geometry;
    RpGeometryFlag      flags;
    RwInt32             numVertices;
    RwUInt32            numTextureCoords;
    RwUInt32            vbSize;
    RwUInt32            stride;
    RwUInt8             *lockedVertexBuffer;
    RwUInt8             *vertexBuffer;

    RWFUNCTION(RWSTRING("D3D8AtomicDefaultInstanceCallback"));

    atomic = (const RpAtomic *)object;
    geometry = (const RpGeometry *)RpAtomicGetGeometry(atomic);
    flags = (RpGeometryFlag)RpGeometryGetFlags(geometry);

    /* Check max num of vertices and build remap table */
    if (geometry->numMorphTargets == 1 &&
        instancedData->indexBuffer != NULL)
    {
        if ( (!reinstance && instancedData->numIndices <= instancedData->numVertices) ||
             instancedData->remapped )
        {
            if (SUCCEEDED(IDirect3DIndexBuffer8_Lock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer,
                                                0, 0, (RwUInt8 **)&indexBuffer, 0)))
            {
                RxVertexIndex *pOld2NewTable = (RxVertexIndex *)RwMalloc(instancedData->numVertices * sizeof(RxVertexIndex),
                                                                         rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
                RwInt32       maxVertex = 0;

                memset(pOld2NewTable, 0xff, instancedData->numVertices * sizeof(RxVertexIndex));

                for(n = 0; n < instancedData->numIndices; n++)
                {
                    const RxVertexIndex index = indexBuffer[n];

                    if (pOld2NewTable[index] > maxVertex)
                    {
                        pOld2NewTable[index] = (RxVertexIndex)maxVertex;

                        maxVertex++;
                    }
                }

                if (maxVertex < instancedData->numVertices)
                {
                    instancedData->numVertices = maxVertex;

                    instancedData->remapped = TRUE;

                    pRemapTable = (RxVertexIndex *)RwMalloc(instancedData->numVertices * sizeof(RxVertexIndex),
                                                            rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);

                    for(n = 0; n < instancedData->numIndices; n++)
                    {
                        const RxVertexIndex index = indexBuffer[n];

                        indexBuffer[n] = pOld2NewTable[index];

                        pRemapTable[indexBuffer[n]] = index;
                    }
                }
                else
                {
                    instancedData->remapped = FALSE;
                }

                RwFree(pOld2NewTable);

                IDirect3DIndexBuffer8_Unlock((LPDIRECT3DINDEXBUFFER8)instancedData->indexBuffer);
            }
        }
    }

    /* Get number of texture coordinates */
    numTextureCoords = RpGeometryGetNumTexCoordSets(geometry);

    /*
     * Calculate the stride of the vertex
     */
    if (!reinstance)
    {
        RwUInt32 usage;

        /* Positions */
        instancedData->stride = sizeof(RwV3d);
        instancedData->vertexShader = D3DFVF_XYZ;

        /* Normals */
        if (flags & rxGEOMETRY_NORMALS)
        {
            instancedData->stride += sizeof(RwV3d);
            instancedData->vertexShader |= D3DFVF_NORMAL;
        }

        /* Pre-lighting */
        if (flags & rxGEOMETRY_PRELIT)
        {
            instancedData->stride += sizeof(RwRGBA);
            instancedData->vertexShader |= D3DFVF_DIFFUSE;
        }

        /* Texture coordinates */
        switch(numTextureCoords)
        {
            case 1:
                instancedData->stride += 1 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX1;
                break;
            case 2:
                instancedData->stride += 2 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX2;
                break;
            case 3:
                instancedData->stride += 3 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX3;
                break;
            case 4:
                instancedData->stride += 4 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX4;
                break;
            case 5:
                instancedData->stride += 5 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX5;
                break;
            case 6:
                instancedData->stride += 6 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX6;
                break;
            case 7:
                instancedData->stride += 7 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX7;
                break;
            case 8:
                instancedData->stride += 8 * sizeof(RwTexCoords);
                instancedData->vertexShader |= D3DFVF_TEX8;
                break;
        }

        /*
         * Create the vertex buffer
         */
        vbSize = instancedData->stride * instancedData->numVertices;

        if(instancedData->vertexBuffer != NULL)
        {
            if (instancedData->managed)
            {
                _rxD3D8VertexBufferManagerDestroy(instancedData->vertexShader, vbSize, instancedData->vertexBuffer, instancedData->baseIndex);
            }
            else
            {
                RwD3D8DynamicVertexBufferDestroy(instancedData->vertexBuffer);
            }

            instancedData->vertexBuffer = NULL;
        }

        usage = D3DUSAGE_WRITEONLY;

        if( geometry->numMorphTargets > 1 &&
            (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 &&
            (atomic->pipeline == NULL || atomic->pipeline->pluginId != rwID_TOONPLUGIN))
        {
            instancedData->managed = FALSE;

            if (FALSE == RwD3D8DynamicVertexBufferCreate(instancedData->vertexShader, vbSize,
                                                        &(instancedData->vertexBuffer)))
            {
                if (pRemapTable != NULL)
                {
                    RwFree(pRemapTable);
                    pRemapTable = NULL;
                }

                RWRETURN(FALSE);
            }
        }
        else
        {
            instancedData->managed = TRUE;

            if (FALSE == _rxD3D8VertexBufferManagerCreate(instancedData->vertexShader, vbSize,
                                                        &(instancedData->vertexBuffer),
                                                        &(instancedData->baseIndex)))
            {
                if (pRemapTable != NULL)
                {
                    RwFree(pRemapTable);
                    pRemapTable = NULL;
                }

                RWRETURN(FALSE);
            }
        }
    }
    else
    {
        vbSize = instancedData->stride * instancedData->numVertices;
    }

    stride = instancedData->stride;

    /*
     * Lock the vertex buffer
     */
    if(instancedData->managed || !reinstance)
    {
        IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                             instancedData->baseIndex * stride, vbSize, &lockedVertexBuffer,
                             D3DLOCK_NOSYSLOCK);
    }
    else
    {
        reinstance = FALSE;

        IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                             instancedData->baseIndex * stride, vbSize, &lockedVertexBuffer,
                             D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
    }

    if (pRemapTable == NULL)
    {
        RwInt32 startMT, endMT;
        RwReal  scale;

        if (geometry->numMorphTargets != 1)
        {
            const RpInterpolator *interp = &atomic->interpolator;

            startMT = interp->startMorphTarget;
            endMT = interp->endMorphTarget;

            if ((startMT >= geometry->numMorphTargets) ||
                (endMT >= geometry->numMorphTargets))
            {
                /* Clamp to in range */
                startMT = endMT = 0;
            }

            scale = interp->recipTime * interp->position;
        }
        else
        {
            startMT = endMT = 0;
            scale = 0.f;
        }

        if (startMT == endMT || scale == 0.f)
        {
            /* Positions */
            if ( (geometry->lockedSinceLastInst & rpGEOMETRYLOCKVERTICES) || !reinstance)
            {
                const RwV3d *pos;

                pos = &geometry->morphTarget[startMT].verts[instancedData->minVert];

                vertexBuffer = lockedVertexBuffer;
                numVertices = instancedData->numVertices;
                while (numVertices--)
                {
                    *((RwV3d *)vertexBuffer) = *pos;
                    vertexBuffer += stride;
                    pos++;
                }
            }

            /* Normals */
            if ( (flags & rxGEOMETRY_NORMALS) &&
                 ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKNORMALS) || !reinstance) )
            {
                RwUInt32    offset;
                const RwV3d *normal;

                offset = sizeof(RwV3d);

                normal = &geometry->morphTarget[startMT].normals[instancedData->minVert];

                vertexBuffer = lockedVertexBuffer + offset;
                numVertices = instancedData->numVertices;
                while (numVertices--)
                {
                    *((RwV3d *)vertexBuffer) = *normal;
                    vertexBuffer += stride;
                    normal++;
                }
            }
        }
        else
        {
            const RwV3d *vertex1;
            const RwV3d *vertex2;
            RwV3d       *vertex;
            RwV3d       *normal;

            vertex1 = &geometry->morphTarget[startMT].verts[instancedData->minVert];
            vertex2 = &geometry->morphTarget[endMT].verts[instancedData->minVert];

            vertexBuffer = lockedVertexBuffer;
            numVertices = instancedData->numVertices;

            if (flags & rxGEOMETRY_NORMALS)
            {
                const RwV3d *normal1;
                const RwV3d *normal2;

                normal1 = &geometry->morphTarget[startMT].normals[instancedData->minVert];
                normal2 = &geometry->morphTarget[endMT].normals[instancedData->minVert];

                while (numVertices--)
                {
                    vertex = ((RwV3d *)vertexBuffer);
                    normal = ((RwV3d *)vertexBuffer) + 1;

                    vertex->x = vertex1->x + (vertex2->x - vertex1->x) * scale;
                    vertex->y = vertex1->y + (vertex2->y - vertex1->y) * scale;
                    vertex->z = vertex1->z + (vertex2->z - vertex1->z) * scale;

                    normal->x = normal1->x + (normal2->x - normal1->x) * scale;
                    normal->y = normal1->y + (normal2->y - normal1->y) * scale;
                    normal->z = normal1->z + (normal2->z - normal1->z) * scale;

                    vertexBuffer += stride;

                    vertex1++;
                    vertex2++;
                    normal1++;
                    normal2++;
                }
            }
            else
            {
                while (numVertices--)
                {
                    vertex = ((RwV3d *)vertexBuffer);

                    vertex->x = vertex1->x + (vertex2->x - vertex1->x) * scale;
                    vertex->y = vertex1->y + (vertex2->y - vertex1->y) * scale;
                    vertex->z = vertex1->z + (vertex2->z - vertex1->z) * scale;

                    vertexBuffer += stride;

                    vertex1++;
                    vertex2++;
                }
            }
        }

        /* Pre-lighting */
        if ( (flags & rxGEOMETRY_PRELIT) &&
             ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKPRELIGHT) || !reinstance) )
        {
            RwUInt32        offset;
            const RwRGBA    *color;
            const RwRGBA    *matColor;
            RwInt32         alpha;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            color = (const RwRGBA *)(((const RwUInt8 *)(&geometry->preLitLum[instancedData->minVert])));
            matColor = RpMaterialGetColor(instancedData->material);

            vertexBuffer = lockedVertexBuffer + offset;
            numVertices = instancedData->numVertices;

            alpha = 0xff;

            if ((flags & rxGEOMETRY_MODULATE) != 0 &&
                (*((const RwUInt32 *)matColor) != 0xffffffff))
            {
                while (numVertices--)
                {
                    *((RwUInt32 *)vertexBuffer) =
                        (((color->alpha * (matColor->alpha + 1)) & 0xff00) << (24-8)) |
                        (((color->red * (matColor->red + 1)) & 0xff00) << (16-8)) |
                        (((color->green * (matColor->green + 1)) & 0xff00) << (8-8)) |
                        ((color->blue * (matColor->blue + 1)) >> 8);

                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    vertexBuffer += stride;
                    color++;
                }
            }
            else
            {
                while (numVertices--)
                {
                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    *((RwUInt32 *)vertexBuffer) = ((color->alpha << 24) |
                                                   (color->red << 16) |
                                                   (color->green << 8) |
                                                   (color->blue));

                    vertexBuffer += stride;
                    color++;
                }
            }

            instancedData->vertexAlpha = (alpha != 0xff);
        }
        else
        {
            instancedData->vertexAlpha = FALSE;
        }

        /* Texture coordinates */
        if ( numTextureCoords &&
            ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKTEXCOORDSALL) || !reinstance) )
        {
            RwUInt32            offset;
            const RwTexCoords   *texCoord;
            RwUInt32            i;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            if (rpGEOMETRYPRELIT & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwRGBA);
            }

            for (i = 0; i < numTextureCoords; i++)
            {
                if ((geometry->lockedSinceLastInst & (rpGEOMETRYLOCKTEXCOORDS1 << i)) || !reinstance)
                {
                    texCoord = (const RwTexCoords *)(((const RwUInt8 *)(&geometry->texCoords[i][instancedData->minVert])));

                    vertexBuffer = lockedVertexBuffer + offset;
                    numVertices = instancedData->numVertices;
                    while (numVertices--)
                    {
                        *((RwTexCoords *)vertexBuffer) = *texCoord;
                        vertexBuffer += stride;
                        texCoord++;
                    }
                }

                offset += sizeof(RwTexCoords);
            }
        }
    }
    else
    {
        /*
         * move used vertex to reduce space
         */

        /* Positions */
        if ( (geometry->lockedSinceLastInst & rpGEOMETRYLOCKVERTICES) || !reinstance)
        {
            const RwV3d *pos;

            pos = &geometry->morphTarget[0].verts[instancedData->minVert];

            vertexBuffer = lockedVertexBuffer;
            numVertices = instancedData->numVertices;
            for (n = 0; n < numVertices; n++)
            {
                *((RwV3d *)vertexBuffer) = pos[pRemapTable[n]];
                vertexBuffer += stride;
            }
        }

        /* Normals */
        if ( (flags & rxGEOMETRY_NORMALS) &&
             ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKNORMALS) || !reinstance) )
        {
            RwUInt32    offset;
            const RwV3d *normal;

            offset = sizeof(RwV3d);

            normal = (const RwV3d *)(((const RwUInt8 *)(&geometry->morphTarget[0].normals[instancedData->minVert])));

            vertexBuffer = lockedVertexBuffer + offset;
            numVertices = instancedData->numVertices;
            for (n = 0; n < numVertices; n++)
            {
                *((RwV3d *)vertexBuffer) = normal[pRemapTable[n]];
                vertexBuffer += stride;
            }
        }

        /* Pre-lighting */
        if ( (flags & rxGEOMETRY_PRELIT) &&
             ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKPRELIGHT) || !reinstance) )
        {
            RwUInt32        offset;
            const RwRGBA    *colorSrc;
            const RwRGBA    *matColor;
            RwInt32         alpha;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            colorSrc = (const RwRGBA *)(((const RwUInt8 *)(&geometry->preLitLum[instancedData->minVert])));
            matColor = RpMaterialGetColor(instancedData->material);

            vertexBuffer = lockedVertexBuffer + offset;
            numVertices = instancedData->numVertices;

            alpha = 0xff;

            if ((flags & rxGEOMETRY_MODULATE) != 0 &&
                (*((const RwUInt32 *)matColor) != 0xffffffff))
            {
                for (n = 0; n < numVertices; n++)
                {
                    const RwRGBA *color = &(colorSrc[pRemapTable[n]]);

                    *((RwUInt32 *)vertexBuffer) =
                        (((color->alpha * (matColor->alpha + 1)) & 0xff00) << (24-8)) |
                        (((color->red * (matColor->red + 1)) & 0xff00) << (16-8)) |
                        (((color->green * (matColor->green + 1)) & 0xff00) << (8-8)) |
                        ((color->blue * (matColor->blue + 1)) >> 8);

                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    vertexBuffer += stride;
                }
            }
            else
            {
                for (n = 0; n < numVertices; n++)
                {
                    const RwRGBA *color = &(colorSrc[pRemapTable[n]]);

                    /* Does the pre-light contain alpha */
                    alpha &= color->alpha;

                    *((RwUInt32 *)vertexBuffer) = ((color->alpha << 24) |
                                                   (color->red << 16) |
                                                   (color->green << 8) |
                                                   (color->blue));

                    vertexBuffer += stride;
                }
            }

            instancedData->vertexAlpha = (alpha != 0xff);
        }
        else
        {
            instancedData->vertexAlpha = FALSE;
        }

        /* Texture coordinates */
        if ( numTextureCoords &&
            ((geometry->lockedSinceLastInst & rpGEOMETRYLOCKTEXCOORDSALL) || !reinstance) )
        {
            RwUInt32            offset;
            const RwTexCoords   *texCoord;
            RwUInt32            i;

            offset = sizeof(RwV3d);

            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwV3d);
            }

            if (rpGEOMETRYPRELIT & RpGeometryGetFlags(geometry))
            {
                offset += sizeof(RwRGBA);
            }

            for (i = 0; i < numTextureCoords; i++)
            {
                if ((geometry->lockedSinceLastInst & (rpGEOMETRYLOCKTEXCOORDS1 << i)) || !reinstance)
                {
                    texCoord = (const RwTexCoords *)(((const RwUInt8 *)(&geometry->texCoords[i][instancedData->minVert])));

                    vertexBuffer = lockedVertexBuffer + offset;
                    numVertices = instancedData->numVertices;
                    for (n = 0; n < numVertices; n++)
                    {
                        *((RwTexCoords *)vertexBuffer) = texCoord[pRemapTable[n]];
                        vertexBuffer += stride;
                    }
                }

                offset += sizeof(RwTexCoords);
            }
        }
    }

    /*
     * Unlock the vertex buffer
     */
    IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

    /* Release remap table */
    if (pRemapTable != NULL)
    {
        RwFree(pRemapTable);
        pRemapTable = NULL;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8AtomicDefaultReinstanceCallback

 Purpose:   To update vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D8AtomicDefaultReinstanceCallback(void *object,
                                    RwResEntry *resEntry,
                                    const RpMeshHeader *meshHeader,
                                    RxD3D8AllInOneInstanceCallBack instanceCallback)
{
    const RpAtomic   *atomic;
    const RpGeometry *geometry;

    RWFUNCTION(RWSTRING("D3D8AtomicDefaultReinstanceCallback"));
    RWASSERT(NULL != object);
    RWASSERT(NULL != meshHeader);
    RWASSERT(NULL != resEntry);

    atomic = (const RpAtomic *)object;

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(NULL != geometry);

    /* If were a morpth target, morph if were dirty */
    if ((RpGeometryGetNumMorphTargets(geometry) != 1))
    {
        const RpInterpolator *interp = &(atomic->interpolator);

         if ((interp->flags & rpINTERPOLATORDIRTYINSTANCE) || (geometry->lockedSinceLastInst))
         {
            D3D8InstanceDataMorphUpdate(resEntry,
                                        meshHeader,
                                        geometry,
                                        interp);
         }
    }
    else
    {
        RxD3D8InstanceData  *instancedData;

        /* Get the first RxD3D8InstanceData pointer */
        instancedData = (RxD3D8InstanceData *)(((RxD3D8ResEntryHeader *)(resEntry + 1)) + 1);

        if (!_rxD3D8Reinstance(object, meshHeader, instancedData, instanceCallback))
        {
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8AtomicAllInOneNode
 */
static RwBool
D3D8AtomicAllInOneNode(RxPipelineNodeInstance *self,
                        const RxPipelineNodeParam *params)
{
    RpAtomic                *atomic;
    RpGeometry              *geometry;
    RwResEntry              *resEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                geomFlags;
    _rxD3D8InstanceNodeData *privateData;
    RwMatrix                *matrix;
    RwUInt32                lighting;

    RWFUNCTION(RWSTRING("D3D8AtomicAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    atomic = (RpAtomic *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != atomic);

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(NULL != geometry);

    privateData = (_rxD3D8InstanceNodeData *)self->privateData;

    /* If there ain't vertices, we cain't make packets... */
    if (geometry->numVertices <= 0)
    {
        /* Don't execute the rest of the pipeline */
        RWRETURN(TRUE);
    }

    meshHeader = geometry->mesh;

    /* Early out if no meshes */
    if (meshHeader->numMeshes <= 0)
    {
        /* If the app wants to use plugin data to make packets, it
         * should use its own instancing function. If we have verts
         * here, we need meshes too in order to build a packet. */
        RWRETURN(TRUE);
    }

    geomFlags = RpGeometryGetFlags(geometry);

    /* if we have native data, don't even look at instancing */
    if (!(rpGEOMETRYNATIVE & geomFlags))
    {
        /* If the geometry has more than one morph target the resEntry in the
         * atomic is used else the resEntry in the geometry */
        if (RpGeometryGetNumMorphTargets(geometry) != 1)
        {
            resEntry = atomic->repEntry;
        }
        else
        {
            resEntry = geometry->repEntry;
        }

        /* If the meshes have changed we should re-instance */
        if (resEntry)
        {
            RxD3D8ResEntryHeader    *resEntryHeader;

            resEntryHeader = (RxD3D8ResEntryHeader *)(resEntry + 1);
            if (resEntryHeader->serialNumber != meshHeader->serialNum)
            {
                /* Destroy resources to force reinstance */
                RwResourcesFreeResEntry(resEntry);
                resEntry = NULL;
            }
        }

        /* Check to see if a resource entry already exists */
        if (resEntry)
        {
            if (geometry->lockedSinceLastInst ||
                (RpGeometryGetNumMorphTargets(geometry) != 1))
            {
                if (privateData->reinstanceCallback)
                {
                    if ( !privateData->reinstanceCallback((void *)atomic,
                                                          resEntry,
                                                          meshHeader,
                                                          privateData->instanceCallback) )
                    {
                        /* Destroy resource entry */
                        RwResourcesFreeResEntry(resEntry);

                        RWRETURN(FALSE);
                    }
                }

                /* Just make sure the interpolator is no longer dirty */
                atomic->interpolator.flags &= ~rpINTERPOLATORDIRTYINSTANCE;

                /* The geometry is up to date */
                geometry->lockedSinceLastInst = 0;
            }

            /* We have a resEntry so use it */
            RwResourcesUseResEntry(resEntry);
        }
        else
        {
            RwResEntry  **resEntryPointer;
            void        *owner;

            meshHeader = geometry->mesh;

            if (RpGeometryGetNumMorphTargets(geometry) != 1)
            {
                owner = (void *)atomic;
                resEntryPointer = &atomic->repEntry;
            }
            else
            {
                owner = (void *)geometry;
                resEntryPointer = &geometry->repEntry;
            }

            /*
             * Create vertex buffers and instance
             */
            resEntry = _rxD3D8Instance((void *)atomic, owner, resEntryPointer,
                                     meshHeader, privateData->instanceCallback,
                                     (rpGEOMETRYNATIVEINSTANCE & geomFlags) != 0);
            if (!resEntry)
            {
                RWRETURN(FALSE);
            }

            /* The geometry is up to date */
            geometry->lockedSinceLastInst = 0;
        }
    }
    else
    {
        resEntry = geometry->repEntry;
    }

    /*
     * Early out of rendering if we're really preinstancing
     * - might be doing this on a loading screen for example...
     */
    if (rpGEOMETRYNATIVEINSTANCE & geomFlags)
    {
        RWRETURN(TRUE);
    }

    /*
     * Set up lights
     */
    if (privateData->lightingCallback)
    {
        privateData->lightingCallback((void *)atomic);
    }

    /*
     * Set the world transform
     */
    matrix = RwFrameGetLTM(RpAtomicGetFrame(atomic));

    RwD3D8SetTransformWorld(matrix);

    RwD3D8GetRenderState(D3DRS_LIGHTING, &lighting);

    if (lighting &&
        !rwMatrixTestFlags(matrix, (rwMATRIXTYPENORMAL | rwMATRIXINTERNALIDENTITY)))
    {
        const RwReal minlimit = 0.9f;
        const RwReal maxlimit = 1.1f;
        RwReal length;

        length = RwV3dDotProduct(&(matrix->right), &(matrix->right));

        if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
        {
            RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
        }
        else
        {
            length = RwV3dDotProduct(&(matrix->up), &(matrix->up));

            if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
            {
                RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
            }
            else
            {
                length = RwV3dDotProduct(&(matrix->at), &(matrix->at));

                if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
                {
                    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
                }
                else
                {
                    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
                }
            }
        }
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
    }

    /*
     * Render
     */
    if (privateData->renderCallback)
    {
        privateData->renderCallback(resEntry, (void *)atomic, rpATOMIC, geomFlags);
    }

#ifdef RWMETRICS
    /* Now update our metrics statistics */
    RWSRCGLOBAL(metrics)->numVertices  += RpGeometryGetNumVertices(geometry);
    RWSRCGLOBAL(metrics)->numTriangles += RpGeometryGetNumTriangles(geometry);
#endif

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8AtomicAllInOnePipelineInit
 */
static RwBool
D3D8AtomicAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D8AtomicAllInOnePipelineInit"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    /* Default instance callback */
    instanceData->instanceCallback = D3D8AtomicDefaultInstanceCallback;

    /* Default reinstance callback */
    instanceData->reinstanceCallback = D3D8AtomicDefaultReinstanceCallback;

    /* Default lighting callback */
    instanceData->lightingCallback = (RxD3D8AllInOneLightingCallBack)(_rwD3D8AtomicDefaultLightingCallback);

    /* Default render callback */
    instanceData->renderCallback = _rxD3D8DefaultRenderCallback;

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup worldextensionsd3d8
 * \ref RxNodeDefinitionGetD3D8AtomicAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the Atomic version of PowerPipe. This is especially
 * useful if you overload the default pipeline with your
 * own.
 *
 * \return The \ref RxNodeDefinition pointer.
 *
 * \see RxD3D8AllInOneSetLightingCallBack
 * \see RxD3D8AllInOneGetLightingCallBack
 * \see RxD3D8AllInOneSetRenderCallBack
 * \see RxD3D8AllInOneGetRenderCallBack
 *
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D8AtomicAllInOne(void)
{

    static RwChar _AtomicInstance_csl[] = "nodeD3D8AtomicAllInOne.csl";

    static RxNodeDefinition nodeD3D8AtomicAllInOneCSL = { /* */
        _AtomicInstance_csl,                        /* Name */
        {                                           /* Nodemethods */
            D3D8AtomicAllInOneNode,              /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D8AtomicAllInOnePipelineInit,         /* +-- pipelinenodeinit */
            NULL,                                   /* +-- pipelineNodeTerm */
            NULL,                                   /* +-- pipelineNodeConfig */
            NULL,                                   /* +-- configMsgHandler */
        },
        {                                           /* Io */
            NUMCLUSTERSOFINTEREST,                  /* +-- NumClustersOfInterest */
            NULL,                                   /* +-- ClustersOfInterest */
            NULL,                                   /* +-- InputRequirements */
            NUMOUTPUTS,                             /* +-- NumOutputs */
            NULL                                    /* +-- Outputs */
        },
        (RwUInt32)sizeof(_rxD3D8InstanceNodeData),  /* PipelineNodePrivateDataSize */
        (RxNodeDefEditable)FALSE,                   /* editable */
        0                                           /* inPipes */
    };

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D8AtomicAllInOne"));

    RWRETURN(&nodeD3D8AtomicAllInOneCSL);
}
