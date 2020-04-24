/****************************************************************************
 *                                                                          *
 * module : nodeD3D9AtomicAllInOne.c                                        *
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

#include "rpcriter.h"

#include "D3D9pipe.h"
#include "D3D9lights.h"

#include "wrldpipe.h"

#include "d3d9vertexdeclaration.h"
#include "d3d9usage.h"
#include "d3d9vertexshader.h"
#include "d3d9vertexshaderutils.h"

#include "nodeD3D9AtomicAllInOne.h"

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
 _rwD3D9AtomicDefaultLightingCallback

 Purpose:   Enable lights.  If any lights are present they will be set and
            lighting will be enabled.

 On entry:  atomic - Pointer to the atomic to apply lighting to.
 .
 On exit :
*/
void
_rwD3D9AtomicDefaultLightingCallback(void *object)
{
    RpAtomic        *atomic;
    RpGeometryFlag  flags;

    RWFUNCTION(RWSTRING("_rwD3D9AtomicDefaultLightingCallback"));

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
                        _rwD3D9LightDirectionalEnable(light);

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
        if (lighting == FALSE)
        {
            RwD3D9SetRenderState(D3DRS_AMBIENT, 0);
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

                        RwV3dSubMacro(&distanceVector, &(sphere->center), pos);

                        distanceSquare = RwV3dDotProduct(&distanceVector, &distanceVector);

                        distanceCollision = (sphere->radius + RpLightGetRadius(lightTie->light));

                        if (distanceSquare < (distanceCollision * distanceCollision))
                        {
                            lighting |= _rwD3D9LightLocalEnable(lightTie->light);
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
        _rwD3D9LightsEnable(lighting, rpATOMIC);
    }
    else
    {
        /* Disable all lights */
        _rwD3D9LightsEnable(FALSE, rpATOMIC);
    }

    RWRETURNVOID();
}

/****************************************************************************
*  D3D9VertexElementSortCB
*/
static int
D3D9VertexElementSortCB(const void *data1, const void *data2)
{
    const D3DVERTEXELEMENT9 *elem1;
    const D3DVERTEXELEMENT9 *elem2;
    int value;

    RWFUNCTION(RWSTRING("D3D9VertexElementSortCB"));

    elem1 = (const D3DVERTEXELEMENT9 *)data1;
    elem2 = (const D3DVERTEXELEMENT9 *)data2;

    /* Sort by Stream */
    value = ( (RwInt32)(elem1->Stream) -
        (RwInt32)(elem2->Stream) );

    if (!value)
    {
        /* Sort by Offset */
        value = ( (RwInt32)(elem1->Offset) -
            (RwInt32)(elem2->Offset) );
    }

    RWRETURN(value);
}

/****************************************************************************
 D3D9AtomicDefaultInstanceCallback

 Purpose:   To write vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D9AtomicDefaultInstanceCallback(void *object,
                                  RxD3D9ResEntryHeader *resEntryHeader,
                                  RwBool reinstance)
{
    D3DVERTEXELEMENT9   declaration[20];
    const RpAtomic      *atomic;
    const RpGeometry    *geometry;
    RpGeometryFlag      flags;
    RpD3D9GeometryUsageFlag usageFlags;
    RwUInt32            numTextureCoords;
    RxD3D9VertexStream  *vertexStream;
    RwUInt32            numStreams, stream;
    RwUInt32            vbSize;
    RwUInt8             *lockedVertexBuffer[RWD3D9_MAX_VERTEX_STREAMS];
    RwInt32             startMT, endMT;
    RwReal              scale;
    RwUInt32            declarationIndex, n;
    RwUInt32            lockedSinceLastInst;
    RwUInt32            numMeshes;
    RxD3D9InstanceData  *instancedData;

    RWFUNCTION(RWSTRING("D3D9AtomicDefaultInstanceCallback"));

    atomic = (const RpAtomic *)object;
    geometry = (const RpGeometry *)RpAtomicGetGeometry(atomic);
    flags = (RpGeometryFlag)RpGeometryGetFlags(geometry);
    usageFlags = RpD3D9GeometryGetUsageFlags(geometry);

    /* Get number of texture coordinates */
    numTextureCoords = RpGeometryGetNumTexCoordSets(geometry);

    /*
     * Calculate the stride of the vertex
     */
    if (!reinstance)
    {
        RwBool useDinamic;

        resEntryHeader->totalNumVertex = geometry->numVertices;

        /* Destroy previous vertex buffers */
        for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
        {
            vertexStream = &(resEntryHeader->vertexStream[n]);

            if(vertexStream->vertexBuffer != NULL)
            {
                if (resEntryHeader->vertexStream[n].managed)
                {
                    RwD3D9DestroyVertexBuffer(vertexStream->stride,
                                              vertexStream->stride * (resEntryHeader->totalNumVertex),
                                              vertexStream->vertexBuffer,
                                              vertexStream->offset);
                }
                else
                {
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

        /* Currently, we only support 2 streams: 1 dinamic and 1 static */
        if( (geometry->numMorphTargets > 1 ||
             (usageFlags & rpD3D9GEOMETRYUSAGE_DYNAMICMASK) != 0) &&
            (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 &&
            (atomic->pipeline == NULL || atomic->pipeline->pluginId != rwID_TOONPLUGIN))
        {
            if (geometry->numMorphTargets > 1)
            {
                usageFlags |= rpD3D9GEOMETRYUSAGE_DYNAMICPOSITIONS |
                              rpD3D9GEOMETRYUSAGE_DYNAMICNORMALS;
            }

            useDinamic = TRUE;
            numStreams = 2;

            _rpD3D9AddDynamicGeometry((RpGeometry *)geometry);
        }
        else
        {
            useDinamic = FALSE;
            numStreams = 1;
        }

        /* Set declarations elements */
        declarationIndex = 0;
        vertexStream = resEntryHeader->vertexStream;

        /* Positions */
        if (usageFlags & rpD3D9GEOMETRYUSAGE_DYNAMICPOSITIONS)
        {
            stream = 0;
        }
        else
        {
            stream = (numStreams - 1);
        }

        declaration[declarationIndex].Stream = stream;
        declaration[declarationIndex].Offset = vertexStream[stream].stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        vertexStream[stream].stride = sizeof(RwV3d);
        vertexStream[stream].geometryFlags = rpGEOMETRYLOCKVERTICES;

        /* Normals */
        if (flags & rxGEOMETRY_NORMALS)
        {
            if (usageFlags & rpD3D9GEOMETRYUSAGE_DYNAMICNORMALS)
            {
                stream = 0;
            }
            else
            {
                stream = (numStreams - 1);
            }

            declaration[declarationIndex].Stream = stream;
            declaration[declarationIndex].Offset = vertexStream[stream].stride;
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                declaration[declarationIndex].Type = D3DDECLTYPE_DEC3N;
                vertexStream[stream].stride += sizeof(RwUInt32);
            }
            else
            {
                declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
                vertexStream[stream].stride += sizeof(RwV3d);
            }

            declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
            declaration[declarationIndex].Usage = D3DDECLUSAGE_NORMAL;
            declaration[declarationIndex].UsageIndex = 0;
            declarationIndex++;
            vertexStream[stream].geometryFlags |= rpGEOMETRYLOCKNORMALS;
        }

        /* Pre-lighting */
        if (flags & rxGEOMETRY_PRELIT)
        {
            if (usageFlags & rpD3D9GEOMETRYUSAGE_DYNAMICPRELIT)
            {
                stream = 0;
            }
            else
            {
                stream = (numStreams - 1);
            }

            declaration[declarationIndex].Stream = stream;
            declaration[declarationIndex].Offset = vertexStream[stream].stride;
            declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
            declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
            declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
            declaration[declarationIndex].UsageIndex = 0;
            declarationIndex++;
            vertexStream[stream].stride += sizeof(RwRGBA);
            vertexStream[stream].geometryFlags |= rpGEOMETRYLOCKPRELIGHT;
        }

        /* Texture coordinates */
        for (n = 0; n < numTextureCoords; n++)
        {
            if (usageFlags & (rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS1 << n))
            {
                stream = 0;
            }
            else
            {
                stream = (numStreams - 1);
            }

            declaration[declarationIndex].Stream = stream;
            declaration[declarationIndex].Offset = vertexStream[stream].stride;
            declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
            declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
            declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
            declaration[declarationIndex].UsageIndex = n;
            declarationIndex++;
            vertexStream[stream].stride += sizeof(RwV2d);
            vertexStream[stream].geometryFlags |= (rpGEOMETRYLOCKTEXCOORDS1 << n);
        }

        /* Tangents */
        if ( (usageFlags & rpD3D9WORLDSECTORUSAGE_CREATETANGENTS) != 0 &&
             numTextureCoords > 0 )
        {
            if (usageFlags & (rpD3D9GEOMETRYUSAGE_DYNAMICPOSITIONS | rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDSALL))
            {
                stream = 0;
            }
            else
            {
                stream = (numStreams - 1);
            }

            declaration[declarationIndex].Stream = stream;
            declaration[declarationIndex].Offset = vertexStream[stream].stride;
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                declaration[declarationIndex].Type = D3DDECLTYPE_DEC3N;
                vertexStream[stream].stride += sizeof(RwUInt32);
            }
            else
            {
                declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
                vertexStream[stream].stride += sizeof(RwV3d);
            }

            declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
            declaration[declarationIndex].Usage = D3DDECLUSAGE_TANGENT;
            declaration[declarationIndex].UsageIndex = 0;
            declarationIndex++;
        }

        while (numStreams > 1 &&
               vertexStream[numStreams - 1].stride == 0)
        {
            numStreams--;
        }

        RWASSERT(numStreams > 0);

        /* Sort by Stream */
        qsort(declaration,
              declarationIndex,
              sizeof(D3DVERTEXELEMENT9),
              D3D9VertexElementSortCB);

        /* End of declaration */
        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RwD3D9CreateVertexDeclaration(declaration,
                                      &(resEntryHeader->vertexDeclaration));

        /*
         * Create the vertex buffers
         */
        RWASSERT(numStreams > 0 && numStreams <= RWD3D9_MAX_VERTEX_STREAMS);

        for (n = 0; n < numStreams; n++)
        {
            vertexStream = &(resEntryHeader->vertexStream[n]);

            vbSize = (vertexStream->stride) * (resEntryHeader->totalNumVertex);

            if (useDinamic == FALSE)
            {
                vertexStream->managed = TRUE;

                if (FALSE == RwD3D9CreateVertexBuffer(vertexStream->stride, vbSize,
                                                      &(vertexStream->vertexBuffer),
                                                      &(vertexStream->offset)))
                {
                    RWRETURN(FALSE);
                }
            }
            else if (n == 0)
            {
                vertexStream->managed = FALSE;

                if (FALSE == RwD3D9DynamicVertexBufferCreate(vbSize,
                                                             &(vertexStream->vertexBuffer)))
                {
                    RWRETURN(FALSE);
                }
            }
            else
            {
                if (_RwD3D9DeviceCaps.DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)
                {
                    vertexStream->managed = TRUE;

                    if (FALSE == RwD3D9CreateVertexBuffer(vertexStream->stride, vbSize,
                                                          &(vertexStream->vertexBuffer),
                                                          &(vertexStream->offset)))
                    {
                        RWRETURN(FALSE);
                    }
                }
                else
                {
                    vertexStream->managed = FALSE;

                    if (FALSE == RwD3D9DynamicVertexBufferCreate(vbSize,
                                                                 &(vertexStream->vertexBuffer)))
                    {
                        RWRETURN(FALSE);
                    }
                }
            }
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
    }
    else
    {
        IDirect3DVertexDeclaration9_GetDeclaration((LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
                                                   declaration,
                                                   &n);

        RWASSERT(n < 20);

        numStreams = 0;
        n = 0;
        while (declaration[n].Type != D3DDECLTYPE_UNUSED)
        {
            if (declaration[n].Stream >= numStreams)
            {
                numStreams = declaration[n].Stream + 1;
            }

            n++;
        }

        RWASSERT(numStreams > 0 && numStreams <= RWD3D9_MAX_VERTEX_STREAMS);
    }

    /*
     * Lock the vertex buffer
     */
    for (n = 0; n < RWD3D9_MAX_VERTEX_STREAMS; n++)
    {
        lockedVertexBuffer[n] = NULL;
    }

    if (!reinstance)
    {
        lockedSinceLastInst = rpGEOMETRYLOCKALL;

        for (n = 0; n < numStreams; n++)
        {
            vertexStream = &(resEntryHeader->vertexStream[n]);

            vbSize = vertexStream->stride * (resEntryHeader->totalNumVertex);

            IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                vertexStream->offset, vbSize, &(lockedVertexBuffer[n]),
                                D3DLOCK_NOSYSLOCK);
        }
    }
    else
    {
        lockedSinceLastInst = geometry->lockedSinceLastInst;

        for (n = 0; n < numStreams; n++)
        {
            vertexStream = &(resEntryHeader->vertexStream[n]);

            if (rpGEOMETRYLOCKALL == lockedSinceLastInst ||
                (lockedSinceLastInst & vertexStream->geometryFlags) != 0)
            {
                vbSize = vertexStream->stride * (resEntryHeader->totalNumVertex);

                if(vertexStream->managed)
                {

                    IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                        vertexStream->offset, vbSize, &(lockedVertexBuffer[n]),
                                        D3DLOCK_NOSYSLOCK);
                }
                else
                {
                    lockedSinceLastInst |= vertexStream->geometryFlags;

                    IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                        vertexStream->offset, vbSize, &(lockedVertexBuffer[n]),
                                        D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                }
            }
        }
    }

    /* Check morphing parameters */
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

    /* INSTANCE */
    declarationIndex = 0;

    if (startMT == endMT || scale == 0.f)
    {
        /* Positions */
        if (lockedSinceLastInst & rpGEOMETRYLOCKVERTICES)
        {
            const RwV3d *pos;

            pos = geometry->morphTarget[startMT].verts;

            /* Find positions */
            declarationIndex = 0;
            while (declaration[declarationIndex].Usage != D3DDECLUSAGE_POSITION ||
                   declaration[declarationIndex].UsageIndex != 0)
            {
                declarationIndex++;
            }
            RWASSERT(declarationIndex < 16);

            _rpD3D9VertexDeclarationInstV3d(declaration[declarationIndex].Type,
                                            lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                            declaration[declarationIndex].Offset,
                                            pos,
                                            resEntryHeader->totalNumVertex,
                                            resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);
        }

        /* Normals */
        if (flags & rxGEOMETRY_NORMALS)
        {
            if (lockedSinceLastInst & rpGEOMETRYLOCKNORMALS)
            {
                const RwV3d *normal;

                normal = geometry->morphTarget[startMT].normals;

                /* Find normals */
                declarationIndex = 0;
                while (declaration[declarationIndex].Usage != D3DDECLUSAGE_NORMAL ||
                       declaration[declarationIndex].UsageIndex != 0)
                {
                    declarationIndex++;
                }
                RWASSERT(declarationIndex < 16);

                _rpD3D9VertexDeclarationInstV3d(declaration[declarationIndex].Type,
                                                lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                                declaration[declarationIndex].Offset,
                                                normal,
                                                resEntryHeader->totalNumVertex,
                                                resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);
            }
        }
    }
    else
    {
        const RwV3d *vertex1;
        const RwV3d *vertex2;

        vertex1 = geometry->morphTarget[startMT].verts;
        vertex2 = geometry->morphTarget[endMT].verts;

        /* Find positions */
        declarationIndex = 0;
        while (declaration[declarationIndex].Usage != D3DDECLUSAGE_POSITION ||
               declaration[declarationIndex].UsageIndex != 0)
        {
            declarationIndex++;
        }
        RWASSERT(declarationIndex < 16);

        _rpD3D9VertexDeclarationInstV3dMorph(declaration[declarationIndex].Type,
                                             lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                             declaration[declarationIndex].Offset,
                                             vertex1,
                                             vertex2,
                                             scale,
                                             resEntryHeader->totalNumVertex,
                                             resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);

        if (flags & rxGEOMETRY_NORMALS)
        {
            const RwV3d *normal1;
            const RwV3d *normal2;

            normal1 = geometry->morphTarget[startMT].normals;
            normal2 = geometry->morphTarget[endMT].normals;

            /* Find normals */
            declarationIndex = 0;
            while (declaration[declarationIndex].Usage != D3DDECLUSAGE_NORMAL ||
                   declaration[declarationIndex].UsageIndex != 0)
            {
                declarationIndex++;
            }
            RWASSERT(declarationIndex < 16);

            _rpD3D9VertexDeclarationInstV3dMorph(declaration[declarationIndex].Type,
                                                 lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                                 declaration[declarationIndex].Offset,
                                                 normal1,
                                                 normal2,
                                                 scale,
                                                 resEntryHeader->totalNumVertex,
                                                 resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);
        }
    }

    /* Pre-lighting */
    if (flags & rxGEOMETRY_PRELIT)
    {
        if (lockedSinceLastInst & rpGEOMETRYLOCKPRELIGHT)
        {
            const RwRGBA    *color;
            RwUInt32 stride;

            /* Find prelit */
            declarationIndex = 0;
            while (declaration[declarationIndex].Usage != D3DDECLUSAGE_COLOR ||
                   declaration[declarationIndex].UsageIndex != 0)
            {
                declarationIndex++;
            }
            RWASSERT(declarationIndex < 16);

            color = (const RwRGBA *)(geometry->preLitLum);
            stride = resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride;

            numMeshes = resEntryHeader->numMeshes;
            instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

            do
            {
                instancedData->vertexAlpha =
                _rpD3D9VertexDeclarationInstColor(lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                                declaration[declarationIndex].Offset +
                                                ((instancedData->minVert) * stride),
                                                color + instancedData->minVert,
                                                instancedData->numVertices,
                                                stride);


                instancedData++;
            }
            while (--numMeshes);
        }
    }
    else
    {
        if (!reinstance)
        {
            numMeshes = resEntryHeader->numMeshes;
            instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

            do
            {
                instancedData->vertexAlpha = FALSE;

                instancedData++;
            }
            while (--numMeshes);
        }
    }

    /* Texture coordinates */
    if (numTextureCoords)
    {
        RwUInt32    i;

        if (lockedSinceLastInst & rpGEOMETRYLOCKTEXCOORDSALL)
        {
            const RwV2d *texCoord;

            for (i = 0; i < numTextureCoords; i++)
            {
                if (lockedSinceLastInst & (rpGEOMETRYLOCKTEXCOORDS1 << i))
                {
                    texCoord = (const RwV2d *)(geometry->texCoords[i]);

                    /* Find tex coords */
                    declarationIndex = 0;
                    while (declaration[declarationIndex].Usage != D3DDECLUSAGE_TEXCOORD ||
                           declaration[declarationIndex].UsageIndex != i)
                    {
                        declarationIndex++;
                    }
                    RWASSERT(declarationIndex < 16);

                    _rpD3D9VertexDeclarationInstV2d(declaration[declarationIndex].Type,
                                                            lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                                            declaration[declarationIndex].Offset,
                                                            texCoord,
                                                            resEntryHeader->totalNumVertex,
                                                            resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);
                }
            }
        }
    }

    /* Tangents */
    if ((usageFlags & rpD3D9WORLDSECTORUSAGE_CREATETANGENTS) != 0 &&
        numTextureCoords > 0)
    {
        if (lockedSinceLastInst & (rpGEOMETRYLOCKVERTICES | rpGEOMETRYLOCKTEXCOORDSALL))
        {
            const RwV3d *pos;
            const RwV3d *normal;
            const RwTexCoords *texCoord;

            /* Find tangents */
            declarationIndex = 0;
            while (declaration[declarationIndex].Usage != D3DDECLUSAGE_TANGENT ||
                   declaration[declarationIndex].UsageIndex != 0)
            {
                declarationIndex++;
            }
            RWASSERT(declarationIndex < 16);

            pos = geometry->morphTarget[startMT].verts;

            normal = geometry->morphTarget[startMT].normals;

            texCoord = (const RwTexCoords *)(geometry->texCoords[numTextureCoords - 1]);

            _rpD3D9VertexDeclarationInstTangent(declaration[declarationIndex].Type,
                                                lockedVertexBuffer[declaration[declarationIndex].Stream] +
                                                declaration[declarationIndex].Offset,
                                                pos,
                                                normal,
                                                NULL,
                                                texCoord,
                                                resEntryHeader,
                                                resEntryHeader->vertexStream[declaration[declarationIndex].Stream].stride);
        }
    }

    /*
     * Unlock the vertex buffer
     */
    for (n = 0; n < numStreams; n++)
    {
        if (lockedVertexBuffer[n] != NULL)
        {
            IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)(resEntryHeader->vertexStream[n].vertexBuffer));
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9AtomicDefaultReinstanceCallback

 Purpose:   To update vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D9AtomicDefaultReinstanceCallback(void *object,
                                    RwResEntry *resEntry,
                                    RxD3D9AllInOneInstanceCallBack instanceCallback)
{
    const RpAtomic  *atomic;
    RpGeometry      *geometry;

    RWFUNCTION(RWSTRING("D3D9AtomicDefaultReinstanceCallback"));
    RWASSERT(NULL != object);
    RWASSERT(NULL != resEntry);

    atomic = (const RpAtomic *)object;

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(NULL != geometry);

    /* If were a morpth target, morph if were dirty */
    if ((RpGeometryGetNumMorphTargets(geometry) != 1))
    {
        const RpInterpolator *interp = &(atomic->interpolator);

         if ( (interp->flags & rpINTERPOLATORDIRTYINSTANCE) ||
              geometry->lockedSinceLastInst )
         {
            RpGeometryFlag          lockedFlags;
            RxD3D9ResEntryHeader    *resEntryHeader;

            lockedFlags = geometry->lockedSinceLastInst;

            geometry->lockedSinceLastInst |= (rpGEOMETRYLOCKVERTICES | rpGEOMETRYLOCKNORMALS);

            resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

            D3D9AtomicDefaultInstanceCallback(object,
                                              resEntryHeader,
                                              TRUE);

            geometry->lockedSinceLastInst = lockedFlags;
         }
    }
    else
    {
        /*
         * Call the instance callback
         */
        if (instanceCallback)
        {
            RxD3D9ResEntryHeader *resEntryHeader;

            resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

            if (!instanceCallback(object,
                                  resEntryHeader,
                                  TRUE))
            {
                RWRETURN(FALSE);
            }
        }
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9AtomicAllInOneNode
 */
static RwBool
D3D9AtomicAllInOneNode(RxPipelineNodeInstance *self,
                       const RxPipelineNodeParam *params)
{
    RpAtomic                *atomic;
    RpGeometry              *geometry;
    RwResEntry              *resEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                geomFlags;
    _rxD3D9InstanceNodeData *privateData;
    RwMatrix                *matrix;
    RwUInt32                lighting;

    RWFUNCTION(RWSTRING("D3D9AtomicAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    atomic = (RpAtomic *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != atomic);

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(NULL != geometry);

    privateData = (_rxD3D9InstanceNodeData *)self->privateData;

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
            RxD3D9ResEntryHeader    *resEntryHeader;

            resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);
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
            resEntry = _rxD3D9Instance((void *)atomic, owner, rpATOMIC,
                                       resEntryPointer, meshHeader,
                                       privateData->instanceCallback,
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

    RwD3D9SetTransformWorld(matrix);

    RwD3D9GetRenderState(D3DRS_LIGHTING, &lighting);

    if (lighting &&
        !rwMatrixTestFlags(matrix, (rwMATRIXTYPENORMAL | rwMATRIXINTERNALIDENTITY)))
    {
        const RwReal minlimit = 0.9f;
        const RwReal maxlimit = 1.1f;
        RwReal length;

        length = RwV3dDotProduct(&(matrix->right), &(matrix->right));

        if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
        {
            RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
        }
        else
        {
            length = RwV3dDotProduct(&(matrix->up), &(matrix->up));

            if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
            {
                RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
            }
            else
            {
                length = RwV3dDotProduct(&(matrix->at), &(matrix->at));

                if ( (FLOATASINT(length) > FLOATASINT(maxlimit)) || (FLOATASINT(length) < FLOATASINT(minlimit)) )
                {
                    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
                }
                else
                {
                    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
                }
            }
        }
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
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
 D3D9AtomicAllInOnePipelineInit
 */
static RwBool
D3D9AtomicAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D9AtomicAllInOnePipelineInit"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    /* Default instance callback */
    instanceData->instanceCallback = D3D9AtomicDefaultInstanceCallback;

    /* Default reinstance callback */
    instanceData->reinstanceCallback = D3D9AtomicDefaultReinstanceCallback;

    /* Default lighting callback */
    instanceData->lightingCallback = (RxD3D9AllInOneLightingCallBack)(_rwD3D9AtomicDefaultLightingCallback);

    /* Default render callback */
    instanceData->renderCallback = _rxD3D9DefaultRenderCallback;

    RWRETURN(TRUE);
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RxNodeDefinitionGetD3D9AtomicAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the Atomic version of PowerPipe. This is especially
 * useful if you overload the default pipeline with your
 * own.
 *
 * \return The \ref RxNodeDefinition pointer.
 *
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneGetLightingCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 * \see RxD3D9AllInOneGetRenderCallBack
 *
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D9AtomicAllInOne(void)
{

    static RwChar _AtomicInstance_csl[] = "nodeD3D9AtomicAllInOne.csl";

    static RxNodeDefinition nodeD3D9AtomicAllInOneCSL = { /* */
        _AtomicInstance_csl,                        /* Name */
        {                                           /* Nodemethods */
            D3D9AtomicAllInOneNode,              /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D9AtomicAllInOnePipelineInit,         /* +-- pipelinenodeinit */
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
        (RwUInt32)sizeof(_rxD3D9InstanceNodeData),  /* PipelineNodePrivateDataSize */
        (RxNodeDefEditable)FALSE,                   /* editable */
        0                                           /* inPipes */
    };

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9AtomicAllInOne"));

    RWRETURN(&nodeD3D9AtomicAllInOneCSL);
}

/****************************************************************************
 _rxD3D9VertexShaderAtomicAllInOneNode
 */
RwBool
_rxD3D9VertexShaderAtomicAllInOneNode(RxPipelineNodeInstance *self,
                                      const RxPipelineNodeParam *params)
{
    _rxD3D9VertexShaderInstanceNodeData *privateData;
    RpAtomic                *atomic;
    RpGeometry              *geometry;
    RwResEntry              *resEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                geomFlags;

    _rpD3D9VertexShaderDispatchDescriptor dispatch;
    _rpD3D9VertexShaderDescriptor         desc;

    RwMatrix                *matrix;
    RwV4d                   *shaderConstantPtr;
    RxD3D9ResEntryHeader    *resEntryHeader;
    RxD3D9InstanceData      *instancedData;
    RwInt32                 numMeshes;
    RwUInt32                shaderConstantCount;
    RwReal                  lastAmbientCoef = 1.f;
    RwReal                  lastDiffuseCoef = 1.f;
    RwUInt32                color;
    RwUInt8                 offsetMaterialColor = 0xff;
    RwUInt8                 offsetMorphing = 0xff;
    RwUInt8                 offsetFog = 0xff;

    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderAtomicAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    privateData = (_rxD3D9VertexShaderInstanceNodeData *)self->privateData;

    atomic = (RpAtomic *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != atomic);

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(NULL != geometry);

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

    /* If the geometry has more than one morph target the resEntry in the
     * atomic is used else the resEntry in the geometry */
    if (geometry->numMorphTargets != 1)
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
        resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);
        if (resEntryHeader->serialNumber != meshHeader->serialNum)
        {
            /* Destroy resources to force reinstance */
            RwResourcesFreeResEntry(resEntry);
            resEntry = NULL;
        }
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
            resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);
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
                /* If were a morpth target, morph if were dirty */
                if ((RpGeometryGetNumMorphTargets(geometry) != 1))
                {
                    const RpInterpolator *interp = &(atomic->interpolator);

                    if ((interp->flags & rpINTERPOLATORDIRTYINSTANCE) ||
                        (geometry->lockedSinceLastInst))
                    {
                        const RwUInt32 lockedFlags = geometry->lockedSinceLastInst;

                        geometry->lockedSinceLastInst |= (rpGEOMETRYLOCKVERTICES | rpGEOMETRYLOCKNORMALS);

                        resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

                        privateData->instanceCallback((void *)atomic,
                                                      resEntryHeader,
                                                      TRUE);

                        geometry->lockedSinceLastInst = lockedFlags;
                    }
                }
                else
                {
                    /*
                    * Call the instance callback
                    */
                    if (privateData->instanceCallback)
                    {
                        resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

                        if (!privateData->instanceCallback((void *)atomic,
                                                           resEntryHeader,
                                                           TRUE))
                        {
                            /* Destroy resource entry */
                            RwResourcesFreeResEntry(resEntry);

                            RWRETURN(FALSE);
                        }
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
            resEntry = _rxD3D9Instance((void *)atomic, owner, rpATOMIC,
                                       resEntryPointer, meshHeader,
                                       privateData->instanceCallback,
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
     * Dispatch geometry
     */
    RWASSERT(privateData->beginCallback != NULL);
    privateData->beginCallback(atomic, rpATOMIC, &desc);

    /* Set clipping */
    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    /* Prepare matrix calculations */
    matrix = RwFrameGetLTM(RpAtomicGetFrame(atomic));

    _rwD3D9VSSetActiveWorldMatrix(matrix);

    shaderConstantPtr = _rxD3D9VertexShaderConstants;

    _rwD3D9VSGetComposedTransformMatrix(shaderConstantPtr);
    shaderConstantPtr += 4;

    RWASSERT(privateData->lightingCallback != NULL);
    shaderConstantPtr = privateData->lightingCallback(atomic,
                                                      rpATOMIC,
                                                      shaderConstantPtr,
                                                      &desc);

    /* Do I need to remove lights in order to make it work? */
    shaderConstantCount = _rpD3D9GetNumConstantsUsed(&desc);

    if (shaderConstantCount > (_rwD3D9VertexShaderMaxConstants - 10)) /* -10 to be safe */
    {
        #ifdef RWDEBUG
        static RpAtomic *LastAtomic= NULL;

        if (LastAtomic != atomic)
        {
            RwChar buffer[512];

            LastAtomic = atomic;

            rwsprintf(buffer,
                        "\n\nThere are too many lights:\n"
                        "   Directional lights: %d\n"
                        "   Point lights: %d\n"
                        "   Spot lights: %d\n"
                        "   Fog: %s\n"
                        "   Material color: %s\n",
                        (RwUInt32)desc.numDirectionalLights,
                        (RwUInt32)desc.numPointLights,
                        (RwUInt32)desc.numSpotLights,
                        (desc.fogMode ? "On" : "Off"),
                        (desc.modulateMaterial ? "On" : "Off"));

            RwDebugSendMessage(rwDEBUGMESSAGE,
                            "_rxD3D9VertexShaderAtomicAllInOneNode",
                            buffer);
        }
        #endif

        do
        {
            if (desc.numSpotLights)
            {
                desc.numSpotLights--;
            }
            else if (desc.numPointLights)
            {
                desc.numPointLights--;
            }
            else if (desc.numDirectionalLights)
            {
                desc.numDirectionalLights--;
            }
            else if (desc.fogMode)
            {
                /* This is a bit too drastic */
                desc.fogMode = 0;
            }
            else
            {
                break;
            }

            shaderConstantCount = _rpD3D9GetNumConstantsUsed(&desc);
        }
        while (shaderConstantCount > (_rwD3D9VertexShaderMaxConstants - 10)); /* -10 to be safe */
    }

    /* upload shared vertex shader constants */
    RwD3D9SetVertexShaderConstant(0,
                                  _rxD3D9VertexShaderConstants,
                                  (shaderConstantPtr - _rxD3D9VertexShaderConstants));

    /* Get header */
    resEntryHeader = (RxD3D9ResEntryHeader *)(resEntry + 1);

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

    /* Get the instanced data */
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

    /* Init last color to only update color if needed */
    color = *((const RwUInt32 *)&(instancedData->material->color));

    /* Get the number of meshes */
    numMeshes = resEntryHeader->numMeshes;
    while (numMeshes--)
    {
        const RpMaterial    *material;
        const RwRGBA        *matcolor;

        RWASSERT(instancedData->material != NULL);

        material = instancedData->material;

        matcolor = &(material->color);

        if ((0xFF != matcolor->alpha) ||
            instancedData->vertexAlpha)
        {
            _rwD3D9RenderStateVertexAlphaEnable(TRUE);
        }
        else
        {
            _rwD3D9RenderStateVertexAlphaEnable(FALSE);
        }

        /* Get right vertex shader */
        RWASSERT(privateData->getmaterialshaderCallback != NULL);
        instancedData->vertexShader = privateData->getmaterialshaderCallback(material,
                                                                             &desc,
                                                                             &dispatch);

        if (FLOATASINT(lastAmbientCoef) != FLOATASINT(material->surfaceProps.ambient) ||
            FLOATASINT(lastDiffuseCoef) != FLOATASINT(material->surfaceProps.diffuse))
        {
            _rpD3D9VertexShaderUpdateLightsColors(_rxD3D9VertexShaderConstants + RWD3D9VSCONST_AMBIENT_OFFSET,
                                                  &desc,
                                                  (material->surfaceProps.ambient) / lastAmbientCoef,
                                                  (material->surfaceProps.diffuse) / lastDiffuseCoef);

            lastAmbientCoef = material->surfaceProps.ambient;
            lastDiffuseCoef = material->surfaceProps.diffuse;
        }

        /* Update material color if needed */
        if (offsetMaterialColor != dispatch.offsetMaterialColor ||
            color != *((const RwUInt32 *)matcolor))
        {
            offsetMaterialColor = dispatch.offsetMaterialColor;
            color = *((const RwUInt32 *)matcolor);

            _rpD3D9VertexShaderUpdateMaterialColor(matcolor, &dispatch);
        }

        /* Update fog if needed */
        if (offsetFog != dispatch.offsetFogRange)
        {
            offsetFog = dispatch.offsetFogRange;

            _rpD3D9VertexShaderUpdateFogData(&desc, &dispatch);
        }

        /* Update morphing if needed */
        if (offsetMorphing != dispatch.offsetMorphingCoef)
        {
            const RpInterpolator *interp;
            RwReal scale;

            offsetMorphing = dispatch.offsetMorphingCoef;

            interp = &(atomic->interpolator);

            scale = (interp->recipTime) * (interp->position);

            _rpD3D9VertexShaderUpdateMorphingCoef(scale, &dispatch);
        }

        RWASSERT(privateData->meshRenderCallback != NULL);
        privateData->meshRenderCallback(resEntryHeader, instancedData,
                                        &desc, &dispatch);

        /* Move onto the next instancedData */
        instancedData->vertexShader = NULL;

        instancedData++;
    }

    RWASSERT(privateData->endCallback != NULL);
    privateData->endCallback(atomic, rpATOMIC, &desc);

#ifdef RWMETRICS
    /* Now update our metrics statistics */
    RWSRCGLOBAL(metrics)->numVertices  += RpGeometryGetNumVertices(geometry);
    RWSRCGLOBAL(metrics)->numTriangles += RpGeometryGetNumTriangles(geometry);
#endif

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9VertexShaderAtomicAllInOnePipelineInit
 */
static RwBool
D3D9VertexShaderAtomicAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D9VertexShaderInstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D9VertexShaderAtomicAllInOnePipelineInit"));

    instanceData = (_rxD3D9VertexShaderInstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->instanceCallback = D3D9AtomicDefaultInstanceCallback;
    instanceData->beginCallback = _rxD3D9VertexShaderDefaultBeginCallBack;
    instanceData->lightingCallback = _rxD3D9VertexShaderDefaultLightingCallBack;
    instanceData->getmaterialshaderCallback = _rxD3D9VertexShaderDefaultGetMaterialShaderCallBack;
    instanceData->meshRenderCallback = _rxD3D9VertexShaderDefaultMeshRenderCallBack;
    instanceData->endCallback = _rxD3D9VertexShaderDefaultEndCallBack;

    RWRETURN(TRUE);
}

#ifdef RWD3D9_USE_VERTEXSHADER_PIPELINE
/**
 * \ingroup worldextensionsd3d9
 * \ref RxNodeDefinitionGetD3D9VertexShaderAtomicAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the Atomic version of PowerPipe using vertex shaders. This is especially
 * useful if you overload the default pipeline with your
 * own.
 *
 * \return The \ref RxNodeDefinition pointer.
 *
 * \see RxD3D9AllInOneSetLightingCallBack
 * \see RxD3D9AllInOneGetLightingCallBack
 * \see RxD3D9AllInOneSetRenderCallBack
 * \see RxD3D9AllInOneGetRenderCallBack
 *
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D9VertexShaderAtomicAllInOne(void)
{

    static RwChar _AtomicInstance_csl[] = "nodeD3D9VertexShaderAtomicAllInOne.csl";

    static RxNodeDefinition nodeD3D9VertexShaderAtomicAllInOneCSL = { /* */
        _AtomicInstance_csl,                        /* Name */
        {                                           /* Nodemethods */
            _rxD3D9VertexShaderAtomicAllInOneNode,  /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D9VertexShaderAtomicAllInOnePipelineInit, /* +-- pipelinenodeinit */
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
        (RwUInt32)sizeof(_rxD3D9VertexShaderInstanceNodeData),  /* PipelineNodePrivateDataSize */
        (RxNodeDefEditable)FALSE,                   /* editable */
        0                                           /* inPipes */
    };

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9VertexShaderAtomicAllInOne"));

    RWRETURN(&nodeD3D9VertexShaderAtomicAllInOneCSL);
}
#endif