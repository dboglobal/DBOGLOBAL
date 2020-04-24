/****************************************************************************
 *                                                                          *
 * module : nodeD3D9WorldSectorAllInOne.c                                   *
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
#include "D3D9lights.h"

#include "wrldpipe.h"

#include "d3d9vertexdeclaration.h"
#include "d3d9usage.h"
#include "d3d9vertexshader.h"
#include "d3d9vertexshaderutils.h"

#include "nodeD3D9WorldSectorAllInOne.h"

/****************************************************************************
 Local defines
 */

#define NUMCLUSTERSOFINTEREST   0
#define NUMOUTPUTS              0

#define FLOATASINT(f) (*((const RwInt32 *)&(f)))

/****************************************************************************
 Global variables
 */

/****************************************************************************
 Local global variable
 */


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

 /****************************************************************************
 D3D9WorldSectorDefaultLightingCallback

 Purpose:   Enable lights.  If any lights are present they will be set and
            lighting will be enabled.

 On entry:  object - Pointer to the world sectors object to apply lighting to.
 .
 On exit :
*/
static void
D3D9WorldSectorDefaultLightingCallback(void *object)
{
    RwUInt32    flags;

    RWFUNCTION(RWSTRING("D3D9WorldSectorDefaultLightingCallback"));

    flags = RpWorldGetFlags((const RpWorld *)RWSRCGLOBAL(curWorld));
    if (flags & rxGEOMETRY_LIGHT)
    {
        RpWorldSector   *sector;
        RwLLLink        *curLight, *endLight;
        RwBool          lighting = FALSE;

        sector = (RpWorldSector *)object;

        /*
         * Global lights, (Directional & Ambient)
         */
        lighting |= _rwD3D9LightsGlobalEnable(rpLIGHTLIGHTWORLD);

        /*
         * Local Lights, (Point, Soft & Soft spot)
         */

        /* Increase the marker ! */
        RWSRCGLOBAL(lightFrame)++;

        /* Lights in the sector */
        curLight = rwLinkListGetFirstLLLink(&sector->lightsInWorldSector);
        endLight = rwLinkListGetTerminator(&sector->lightsInWorldSector);

        while (curLight != endLight)
        {
            RpLightTie *lightTie;

            lightTie = rwLLLinkGetData(curLight, RpLightTie, lightInWorldSector);

            /* NB lightTie may actually be a dummyTie from a enclosing ForAll */

            /* Check to see if the light is set to light worlds */
            if (lightTie->light &&
                (rwObjectTestFlags(lightTie->light, rpLIGHTLIGHTWORLD)))
            {
                /* Setup light */
                lighting |= _rwD3D9LightLocalEnable(lightTie->light);
            }

            /* Next */
            curLight = rwLLLinkGetNext(curLight);
        }

        /* Disable all unwanted lights and activate lighting if needed */
        _rwD3D9LightsEnable(lighting, (RwUInt32)rwSECTORATOMIC);
    }
    else
    {
        /* Disable all lights */
        _rwD3D9LightsEnable(FALSE, (RwUInt32)rwSECTORATOMIC);
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D9WorldSectorDefaultInstanceCallback

 Purpose:   To write vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D9WorldSectorDefaultInstanceCallback(void *object,
                                       RxD3D9ResEntryHeader *resEntryHeader,
                                       RwBool reinstance __RWUNUSED__)
{
    D3DVERTEXELEMENT9 declaration[20];
    RwUInt32 declarationIndex, offset;
    const RpWorldSector *sector;
    RpGeometryFlag      flags;
    RpD3D9WorldSectorUsageFlag usageFlags;
    RxD3D9VertexStream  *vertexStream;
    RwUInt32            vbSize;
    RwUInt8             *lockedVertexBuffer;
    const RwV3d         *pos;
    RwUInt32            numMeshes;
    RxD3D9InstanceData  *instancedData;

    RWFUNCTION(RWSTRING("D3D9WorldSectorDefaultInstanceCallback"));

    sector = (const RpWorldSector *)object;
    flags = (RpGeometryFlag)
        RpWorldGetFlags((RpWorld *)RWSRCGLOBAL(curWorld));

    usageFlags = RpD3D9WorldSectorGetUsageFlags(sector);

    resEntryHeader->totalNumVertex = sector->numVertices;

    vertexStream = &(resEntryHeader->vertexStream[0]);

    /*
     * Calculate the stride of the vertex if set to the default value of -1
     */
    declarationIndex = 0;
    offset = 0;

    /* Positions */
    declaration[declarationIndex].Stream = 0;
    declaration[declarationIndex].Offset = offset;
    declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
    declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
    declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
    declaration[declarationIndex].UsageIndex = 0;
    declarationIndex++;
    offset += sizeof(RwV3d);
    vertexStream->stride = sizeof(RwV3d);
    vertexStream->geometryFlags = rpGEOMETRYLOCKVERTICES;

    /* Normals */
    if (flags & rxGEOMETRY_NORMALS)
    {
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;

        if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
        {
            declaration[declarationIndex].Type = D3DDECLTYPE_DEC3N;
            offset += sizeof(RwUInt32);
            vertexStream->stride += sizeof(RwUInt32);
        }
        else
        {
            declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
            offset += sizeof(RwV3d);
            vertexStream->stride += sizeof(RwV3d);
        }

        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_NORMAL;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        vertexStream->geometryFlags |= rpGEOMETRYLOCKNORMALS;
    }

    /* Pre-lighting */
    if (flags & rxGEOMETRY_PRELIT)
    {
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwRGBA);
        vertexStream->stride += sizeof(RwRGBA);
        vertexStream->geometryFlags |= rpGEOMETRYLOCKPRELIGHT;
    }

    /* Texture coordinates */
    if (flags & (rxGEOMETRY_TEXTURED | rpWORLDTEXTURED2))
    {
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwV2d);
        vertexStream->stride += sizeof(RwV2d);
        vertexStream->geometryFlags |= rpGEOMETRYLOCKTEXCOORDS1;
    }

    if (flags & rpWORLDTEXTURED2)
    {
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 1;
        declarationIndex++;
        offset += sizeof(RwV2d);
        vertexStream->stride += sizeof(RwV2d);
        vertexStream->geometryFlags |= rpGEOMETRYLOCKTEXCOORDS2;
    }

    if (usageFlags & rpD3D9WORLDSECTORUSAGE_CREATETANGENTS)
    {
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;

        if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
        {
            declaration[declarationIndex].Type = D3DDECLTYPE_DEC3N;
            offset += sizeof(RwUInt32);
            vertexStream->stride += sizeof(RwUInt32);
        }
        else
        {
            declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
            offset += sizeof(RwV3d);
            vertexStream->stride += sizeof(RwV3d);
        }

        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TANGENT;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
    }

    declaration[declarationIndex].Stream = 0xFF;
    declaration[declarationIndex].Offset = 0;
    declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
    declaration[declarationIndex].Method = 0;
    declaration[declarationIndex].Usage = 0;
    declaration[declarationIndex].UsageIndex = 0;

    /*
     * Set the vertex shader flags
     */
    RwD3D9CreateVertexDeclaration(declaration,
                                    &(resEntryHeader->vertexDeclaration));

    /*
     * Create the vertex buffer
     */
    vbSize = (vertexStream->stride) * (resEntryHeader->totalNumVertex);

    vertexStream->managed = TRUE;

    if (FALSE == RwD3D9CreateVertexBuffer(vertexStream->stride, vbSize,
                                           &vertexStream->vertexBuffer,
                                           &vertexStream->offset))
    {
        RWRETURN(FALSE);
    }

    /* Fix base index */
    resEntryHeader->useOffsets = FALSE;

    numMeshes = resEntryHeader->numMeshes;
    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

    do
    {
        instancedData->baseIndex = instancedData->minVert +
                                   (vertexStream->offset / vertexStream->stride);

        instancedData++;
    }
    while (--numMeshes);

    /*
     * Lock the vertex buffer
     */
    IDirect3DVertexBuffer9_Lock((LPVERTEXBUFFER)vertexStream->vertexBuffer,
                                vertexStream->offset, vbSize, &lockedVertexBuffer, 0);

    declarationIndex = 0;
    offset = 0;

    /* Positions */
    pos = (const RwV3d *)(sector->vertices);

    offset += _rpD3D9VertexDeclarationInstV3d(declaration[declarationIndex].Type,
                                              lockedVertexBuffer + offset,
                                              pos,
                                              resEntryHeader->totalNumVertex,
                                              vertexStream->stride);

    declarationIndex++;

    /* Normals */
    if (flags & rxGEOMETRY_NORMALS)
    {
        const RpVertexNormal  *normal;

        normal = (const RpVertexNormal *)(sector->normals);

        offset += _rpD3D9VertexDeclarationInstV3dComp(declaration[declarationIndex].Type,
                                                      lockedVertexBuffer + offset,
                                                      normal,
                                                      resEntryHeader->totalNumVertex,
                                                      vertexStream->stride);

        declarationIndex++;
    }

    /* Pre-lighting */
    if (flags & rxGEOMETRY_PRELIT)
    {
        const RwRGBA    *color;
        RwUInt32 stride;

        color = (const RwRGBA *)(sector->preLitLum);
        stride = vertexStream->stride;

        numMeshes = resEntryHeader->numMeshes;
        instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

        do
        {
            instancedData->vertexAlpha =
            _rpD3D9VertexDeclarationInstColor(lockedVertexBuffer + offset +
                                              ((instancedData->minVert) * stride),
                                              color + instancedData->minVert,
                                              instancedData->numVertices,
                                              stride);


            instancedData++;
        }
        while (--numMeshes);

        declarationIndex++;
        offset += sizeof(RwUInt32);
    }
    else
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

    /* Texture coordinates */
    if (flags & (rxGEOMETRY_TEXTURED | rpWORLDTEXTURED2))
    {
        const RwV2d *texCoord;

        texCoord = (const RwV2d *)(sector->texCoords[0]);

        offset += _rpD3D9VertexDeclarationInstV2d(declaration[declarationIndex].Type,
                                                  lockedVertexBuffer + offset,
                                                  texCoord,
                                                  resEntryHeader->totalNumVertex,
                                                  vertexStream->stride);

        declarationIndex++;
    }

    if (flags & rpWORLDTEXTURED2)
    {
        const RwV2d *texCoord;

        texCoord = (const RwV2d *)(sector->texCoords[1]);

        offset += _rpD3D9VertexDeclarationInstV2d(declaration[declarationIndex].Type,
                                                  lockedVertexBuffer + offset,
                                                  texCoord,
                                                  resEntryHeader->totalNumVertex,
                                                  vertexStream->stride);

        declarationIndex++;
    }

    if (usageFlags & rpD3D9WORLDSECTORUSAGE_CREATETANGENTS)
    {
        const RpVertexNormal  *normal;
        const RwTexCoords *texCoord;

        pos = (const RwV3d *)(sector->vertices);

        normal = (const RpVertexNormal *)(sector->normals);

        if (flags & rpWORLDTEXTURED2)
        {
            texCoord = (const RwTexCoords *)(sector->texCoords[1]);
        }
        else
        {
            texCoord = (const RwTexCoords *)(sector->texCoords[0]);
        }

        offset += _rpD3D9VertexDeclarationInstTangent(declaration[declarationIndex].Type,
                                                      lockedVertexBuffer + offset,
                                                      pos,
                                                      NULL,
                                                      normal,
                                                      texCoord,
                                                      resEntryHeader,
                                                      vertexStream->stride);

        declarationIndex++;
    }

    RWASSERT(offset == vertexStream->stride);

    /*
     * Unlock the vertex buffer
     */
    IDirect3DVertexBuffer9_Unlock((LPVERTEXBUFFER)vertexStream->vertexBuffer);

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9WorldSectorAllInOneNode
 */
static RwBool
D3D9WorldSectorAllInOneNode(RxPipelineNodeInstance *self,
                             const RxPipelineNodeParam *params)
{
    RpWorldSector           *sector;
    RwResEntry              *repEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                worldFlags;
    _rxD3D9InstanceNodeData *privateData;
    RwInt32                 numMeshes;

    RWFUNCTION(RWSTRING("D3D9WorldSectorAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    sector = (RpWorldSector *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != sector);

    privateData = (_rxD3D9InstanceNodeData *)self->privateData;

    /* If there ain't vertices, we cain't make packets... */
    if (sector->numVertices <= 0)
    {
        /* Don't execute the rest of the pipeline */
        RWRETURN(TRUE);
    }

    meshHeader = sector->mesh;
    numMeshes = meshHeader->numMeshes;

    /* Early out if no meshes */
    if (numMeshes <= 0)
    {
        /* If the app wants to use plugin data to make packets, it
         * should use its own instancing function. If we have verts
         * here, we need meshes too in order to build a packet. */
        RWRETURN(TRUE);
    }

    repEntry = sector->repEntry;

    worldFlags = RpWorldGetFlags((RpWorld *)RWSRCGLOBAL(curWorld));

    /* if we have native data, don't even look at instancing */
    if (!(rpWORLDNATIVE & worldFlags))
    {
        /* If the meshes have changed we should re-instance */
        if (repEntry)
        {
            RxD3D9ResEntryHeader    *resEntryHeader;

            resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);
            if (resEntryHeader->serialNumber != meshHeader->serialNum)
            {
                /* Destroy resources to force reinstance */
                RwResourcesFreeResEntry(repEntry);
                repEntry = NULL;
            }
        }

        /* Check to see if a resource entry already exists */
        if (repEntry)
        {
            /* Nothing changed and we have a repEntry so use it */
            RwResourcesUseResEntry(repEntry);
        }
        else
        {
            /*
             * Create vertex buffers and instance
             */
            repEntry = _rxD3D9Instance((void *)sector, (void *)sector, (RwUInt8)rwSECTORATOMIC,
                                       &sector->repEntry,
                                       meshHeader, privateData->instanceCallback,
                                       (rpWORLDNATIVEINSTANCE & worldFlags) != 0);
            if (!repEntry)
            {
                RWRETURN(FALSE);
            }
        }
    }

    /*
     * Early out of rendering if we're really preinstancing
     * - might be doing this on a loading screen for example...
     */
    if (rpWORLDNATIVEINSTANCE & worldFlags)
    {
        RWRETURN(TRUE);
    }

    /*
     * Set up lights here, (for now at least)
     */
    if (privateData->lightingCallback)
    {
        privateData->lightingCallback((void *)sector);
    }

    /*
     * Set the world transform
     */

    /* Set an identity matrix */
    RwD3D9SetTransformWorld(NULL);

    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    /*
     * Render
     */
    if (privateData->renderCallback)
    {
        privateData->renderCallback(repEntry, (void *)sector, (RwUInt8)rwSECTORATOMIC, worldFlags);
    }

#ifdef RWMETRICS
    /* Now update our metrics statistics */
    RWSRCGLOBAL(metrics)->numVertices  += RpWorldSectorGetNumVertices(sector);
    RWSRCGLOBAL(metrics)->numTriangles += RpWorldSectorGetNumTriangles(sector);
#endif

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9WorldSectorAllInOnePipelineInit
 */
static RwBool
D3D9WorldSectorAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D9InstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D9WorldSectorAllInOnePipelineInit"));

    instanceData = (_rxD3D9InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    /* Default instance callback */
    instanceData->instanceCallback = (RxD3D9AllInOneInstanceCallBack)(D3D9WorldSectorDefaultInstanceCallback);

    /* Default reinstance callback */
    instanceData->reinstanceCallback = NULL;

    /* Default lighting callback */
    instanceData->lightingCallback = (RxD3D9AllInOneLightingCallBack)(D3D9WorldSectorDefaultLightingCallback);

    /* No execution callback */
    instanceData->renderCallback = (RxD3D9AllInOneRenderCallBack)(_rxD3D9DefaultRenderCallback);

    RWRETURN(TRUE);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup worldextensionsd3d9
 * \ref RxNodeDefinitionGetD3D9WorldSectorAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the world sector version of PowerPipe. This is especially
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
RxNodeDefinitionGetD3D9WorldSectorAllInOne(void)
{
    static RwChar _D3D9ObjWorldSectorAllInOne_csl[] = "nodeD3D9WorldSectorAllInOne.csl";

    static RxNodeDefinition nodeD3D9WorldSectorAllInOneCSL = { /* */
        _D3D9ObjWorldSectorAllInOne_csl,            /* Name */
        {                                           /* Nodemethods */
            D3D9WorldSectorAllInOneNode,         /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D9WorldSectorAllInOnePipelineInit,    /* +-- pipelinenodeinit */
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

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9WorldSectorAllInOne"));

    RWRETURN(&nodeD3D9WorldSectorAllInOneCSL);
}

/****************************************************************************
 _rxD3D9VertexShaderWorldSectorAllInOneNode
 */
RwBool
_rxD3D9VertexShaderWorldSectorAllInOneNode(RxPipelineNodeInstance *self,
                                           const RxPipelineNodeParam *params)
{
    _rxD3D9VertexShaderInstanceNodeData *privateData;
    RpWorldSector           *sector;
    RwResEntry              *repEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                worldFlags;
    RwInt32                 numMeshes;

    _rpD3D9VertexShaderDispatchDescriptor dispatch;
    _rpD3D9VertexShaderDescriptor         desc;

    RwV4d                   *shaderConstantPtr;
    RxD3D9ResEntryHeader    *resEntryHeader;
    RxD3D9InstanceData      *instancedData;
    RwUInt32                shaderConstantCount;
    RwReal                  lastAmbientCoef = 1.f;
    RwReal                  lastDiffuseCoef = 1.f;
    RwUInt32                color;
    RwUInt8                 offsetMaterialColor = 0xff;
    RwUInt8                 offsetMorphing = 0xff;
    RwUInt8                 offsetFog = 0xff;

    RWFUNCTION(RWSTRING("_rxD3D9VertexShaderWorldSectorAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    privateData = (_rxD3D9VertexShaderInstanceNodeData *)self->privateData;

    sector = (RpWorldSector *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != sector);

    /* If there ain't vertices, we cain't make packets... */
    if (sector->numVertices <= 0)
    {
        /* Don't execute the rest of the pipeline */
        RWRETURN(TRUE);
    }

    meshHeader = sector->mesh;
    numMeshes = meshHeader->numMeshes;

    /* Early out if no meshes */
    if (numMeshes <= 0)
    {
        /* If the app wants to use plugin data to make packets, it
         * should use its own instancing function. If we have verts
         * here, we need meshes too in order to build a packet. */
        RWRETURN(TRUE);
    }

    repEntry = sector->repEntry;

    worldFlags = RpWorldGetFlags((RpWorld *)RWSRCGLOBAL(curWorld));

    /* if we have native data, don't even look at instancing */
    if (!(rpWORLDNATIVE & worldFlags))
    {
        /* If the meshes have changed we should re-instance */
        if (repEntry)
        {
            resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);
            if (resEntryHeader->serialNumber != meshHeader->serialNum)
            {
                /* Destroy resources to force reinstance */
                RwResourcesFreeResEntry(repEntry);
                repEntry = NULL;
            }
        }

        /* Check to see if a resource entry already exists */
        if (repEntry)
        {
            /* Nothing changed and we have a repEntry so use it */
            RwResourcesUseResEntry(repEntry);
        }
        else
        {
            /*
             * Create vertex buffers and instance
             */
            repEntry = _rxD3D9Instance((void *)sector, (void *)sector, (RwUInt8)rwSECTORATOMIC,
                                       &sector->repEntry,
                                       meshHeader, privateData->instanceCallback,
                                       (rpWORLDNATIVEINSTANCE & worldFlags) != 0);
            if (!repEntry)
            {
                RWRETURN(FALSE);
            }
        }
    }

    /*
     * Early out of rendering if we're really preinstancing
     * - might be doing this on a loading screen for example...
     */
    if (rpWORLDNATIVEINSTANCE & worldFlags)
    {
        RWRETURN(TRUE);
    }

    /*
     * Dispatch sector
     */
    RWASSERT(privateData->beginCallback != NULL);
    privateData->beginCallback(sector, (RwUInt8)rwSECTORATOMIC, &desc);

    /* Set clipping */
    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    /* Prepare matrix calculations */
    _rwD3D9VSSetActiveWorldMatrix(NULL);

    shaderConstantPtr = _rxD3D9VertexShaderConstants;

    _rwD3D9VSGetComposedTransformMatrix(shaderConstantPtr);
    shaderConstantPtr += 4;

    RWASSERT(privateData->lightingCallback != NULL);
    shaderConstantPtr = privateData->lightingCallback(sector,
                                                      (RwUInt8)rwSECTORATOMIC,
                                                      shaderConstantPtr,
                                                      &desc);

    /* Do I need to remove lights in order to make it work? */
    shaderConstantCount = _rpD3D9GetNumConstantsUsed(&desc);

    if (shaderConstantCount > (_rwD3D9VertexShaderMaxConstants - 10)) /* 10 to be safe */
    {
        #ifdef RWDEBUG
        static RpWorldSector *LastSector= NULL;

        if (LastSector != sector)
        {
            RwChar buffer[512];

            LastSector = sector;

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
                            "_rxD3D9VertexShaderWorldSectorAllInOneNode",
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
        while (shaderConstantCount > (_rwD3D9VertexShaderMaxConstants - 10)); /* 10 to be safe */
    }

    /* upload shared vertex shader constants */
    RwD3D9SetVertexShaderConstant(0,
                                  _rxD3D9VertexShaderConstants,
                                  (shaderConstantPtr - _rxD3D9VertexShaderConstants));

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

        RWASSERT(privateData->meshRenderCallback != NULL);
        privateData->meshRenderCallback(resEntryHeader, instancedData,
                                        &desc, &dispatch);

        /* Remove vertex shader just in case */
        instancedData->vertexShader = NULL;

        /* Move onto the next instancedData */
        instancedData++;
    }

    RWASSERT(privateData->endCallback != NULL);
    privateData->endCallback(sector, (RwUInt8)rwSECTORATOMIC, &desc);

#ifdef RWMETRICS
    /* Now update our metrics statistics */
    RWSRCGLOBAL(metrics)->numVertices  += RpWorldSectorGetNumVertices(sector);
    RWSRCGLOBAL(metrics)->numTriangles += RpWorldSectorGetNumTriangles(sector);
#endif

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D9VertexShaderWorldSectorAllInOnePipelineInit
 */
static RwBool
D3D9VertexShaderWorldSectorAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D9VertexShaderInstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D9VertexShaderWorldSectorAllInOnePipelineInit"));

    instanceData = (_rxD3D9VertexShaderInstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    instanceData->instanceCallback = D3D9WorldSectorDefaultInstanceCallback;
    instanceData->beginCallback = _rxD3D9VertexShaderDefaultBeginCallBack;
    instanceData->lightingCallback = _rxD3D9VertexShaderDefaultLightingCallBack;
    instanceData->getmaterialshaderCallback = _rxD3D9VertexShaderDefaultGetMaterialShaderCallBack;
    instanceData->meshRenderCallback = _rxD3D9VertexShaderDefaultMeshRenderCallBack;
    instanceData->endCallback = _rxD3D9VertexShaderDefaultEndCallBack;

    RWRETURN(TRUE);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#ifdef RWD3D9_USE_VERTEXSHADER_PIPELINE
/**
 * \ingroup worldextensionsd3d9
 * \ref RxNodeDefinitionGetD3D9VertexShaderWorldSectorAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the world sector version of PowerPipe using vertex shaders. This is especially
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
RxNodeDefinitionGetD3D9VertexShaderWorldSectorAllInOne(void)
{
    static RwChar _D3D9ObjVertexShaderWorldSectorAllInOne_csl[] = "nodeD3D9VertexShaderWorldSectorAllInOne.csl";

    static RxNodeDefinition nodeD3D9VertexShaderWorldSectorAllInOneCSL = { /* */
        _D3D9ObjVertexShaderWorldSectorAllInOne_csl,            /* Name */
        {                                           /* Nodemethods */
            _rxD3D9VertexShaderWorldSectorAllInOneNode,         /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D9VertexShaderWorldSectorAllInOnePipelineInit,    /* +-- pipelinenodeinit */
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

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9VertexShaderWorldSectorAllInOne"));

    RWRETURN(&nodeD3D9VertexShaderWorldSectorAllInOneCSL);
}
#endif