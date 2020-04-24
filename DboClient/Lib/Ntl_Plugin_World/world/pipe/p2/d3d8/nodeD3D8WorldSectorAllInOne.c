/****************************************************************************
 *                                                                          *
 * module : nodeD3D8WorldSectorAllInOne.c                                   *
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

#include "D3D8pipe.h"
#include "D3D8lights.h"

#include "wrldpipe.h"

#include "D3D8VertexBufferManager.h"

#include "nodeD3D8WorldSectorAllInOne.h"

/****************************************************************************
 Local defines
 */

#define NUMCLUSTERSOFINTEREST   0
#define NUMOUTPUTS              0

#define COLORSCALAR 0.003921568627450980392156862745098f    /* 1.0f/ 255.0f */

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
 D3D8WorldSectorDefaultLightingCallback

 Purpose:   Enable lights.  If any lights are present they will be set and
            lighting will be enabled.

 On entry:  object - Pointer to the world sectors object to apply lighting to.
 .
 On exit :
*/
static void
D3D8WorldSectorDefaultLightingCallback(void *object)
{
    RwUInt32    flags;

    RWFUNCTION(RWSTRING("D3D8WorldSectorDefaultLightingCallback"));

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
        lighting |= _rwD3D8LightsGlobalEnable(rpLIGHTLIGHTWORLD);

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
                lighting |= _rwD3D8LightLocalEnable(lightTie->light);
            }

            /* Next */
            curLight = rwLLLinkGetNext(curLight);
        }

        /* Disable all unwanted lights and activate lighting if needed */
        _rwD3D8LightsEnable(lighting, (RwUInt32)rwSECTORATOMIC);
    }
    else
    {
        /* Disable all lights */
        _rwD3D8LightsEnable(FALSE, (RwUInt32)rwSECTORATOMIC);
    }

    RWRETURNVOID();
}

/****************************************************************************
 D3D8WorldSectorDefaultInstanceCallback

 Purpose:   To write vertex data into a locked vertex buffer

 On entry:

 On exit :
*/
static RwBool
D3D8WorldSectorDefaultInstanceCallback(void *object,
                                        RxD3D8InstanceData *instancedData,
                                        RwBool reinstance __RWUNUSED__)
{
    const RpWorldSector *sector;
    RpGeometryFlag      flags;
    RwUInt32            numVertices;
    RwUInt32            vbSize;
    RwInt32             offset;
    RwUInt32            fvfFlags;
    RwUInt8             *lockedVertexBuffer;
    RwUInt8             *vertexBuffer;
    const RwV3d         *pos;

    RWFUNCTION(RWSTRING("D3D8WorldSectorDefaultInstanceCallback"));

    sector = (const RpWorldSector *)object;
    flags = (RpGeometryFlag)
        RpWorldGetFlags((RpWorld *)RWSRCGLOBAL(curWorld));

    /*
     * Calculate the stride of the vertex if set to the default value of -1
     */

    /* Positions */
    instancedData->stride = sizeof(RwV3d);
    fvfFlags = D3DFVF_XYZ;

    /* Normals */
    if (flags & rxGEOMETRY_NORMALS)
    {
        instancedData->stride += sizeof(RwV3d);
        fvfFlags |= D3DFVF_NORMAL;
    }

    /* Pre-lighting */
    if (flags & rxGEOMETRY_PRELIT)
    {
        instancedData->stride += sizeof(RwRGBA);
        fvfFlags |= D3DFVF_DIFFUSE;
    }

    /* Texture coordinates */
    if (flags & rxGEOMETRY_TEXTURED)
    {
        instancedData->stride += sizeof(RwTexCoords);
        fvfFlags |= (D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
    }
    else if (flags & rpWORLDTEXTURED2)
    {
        instancedData->stride += (sizeof(RwTexCoords) << 1);
        fvfFlags |= (D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1));
    }

    /*
     * Create the vertex buffer
     */
    vbSize = instancedData->stride * instancedData->numVertices;

    instancedData->managed = TRUE;

    if (FALSE == _rxD3D8VertexBufferManagerCreate(fvfFlags, vbSize,
                                                &instancedData->vertexBuffer,
                                                &instancedData->baseIndex))
    {
        RWRETURN(FALSE);
    }

    /*
     * Lock the vertex buffer
     */
    IDirect3DVertexBuffer8_Lock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer,
                         instancedData->baseIndex * instancedData->stride, vbSize, &lockedVertexBuffer, 0);

    /* Positions */
    pos = (const RwV3d *)&sector->vertices[instancedData->minVert];

    vertexBuffer = lockedVertexBuffer;
    numVertices = instancedData->numVertices;
    while (numVertices--)
    {
        *((RwV3d *)vertexBuffer) = *pos;
        vertexBuffer += instancedData->stride;
        pos++;
    }

    offset = sizeof(RwV3d);

    /* Normals */
    if (flags & rxGEOMETRY_NORMALS)
    {
        const RpVertexNormal  *normal;

        normal = (const RpVertexNormal *)(((const RwUInt8 *)(&sector->normals[instancedData->minVert])));

        vertexBuffer = lockedVertexBuffer + offset;
        numVertices = instancedData->numVertices;
        while (numVertices--)
        {
            RPV3DFROMVERTEXNORMAL(*((RwV3d *)vertexBuffer), *normal);
            vertexBuffer += instancedData->stride;
            normal++;
        }

        offset += sizeof(RwV3d);
    }

    /* Pre-lighting */
    if (flags & rxGEOMETRY_PRELIT)
    {
        const RwRGBA    *color;
        const RwRGBA    *matColor;
        RwUInt32        alpha;

        color = (const RwRGBA *)(((const RwUInt8 *)(&sector->preLitLum[instancedData->minVert])));

        matColor = (const RwRGBA *)RpMaterialGetColor(instancedData->material);

        vertexBuffer = lockedVertexBuffer + offset;
        numVertices = instancedData->numVertices;

        alpha = 0xff;

        if ( (flags & rxGEOMETRY_MODULATE) != 0 && (*((const RwUInt32 *)matColor) != 0xffffffff) )
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
                *((RwUInt32 *)vertexBuffer) = ((color->alpha << 24) |
                                               (color->red << 16) |
                                               (color->green << 8) |
                                               (color->blue));

                /* Does the pre-light contain alpha */
                alpha &= color->alpha;

                vertexBuffer += instancedData->stride;
                color++;
            }
        }

        instancedData->vertexAlpha = (alpha != 0xff);

        offset += sizeof(RwUInt32);
    }
    else
    {
        instancedData->vertexAlpha = FALSE;
    }

    /* Texture coordinates */
    if (flags & rxGEOMETRY_TEXTURED)
    {
        const RwTexCoords *texCoord;

        texCoord = (const RwTexCoords *)(((const RwUInt8 *)(&sector->texCoords[0][instancedData->minVert])));

        vertexBuffer = lockedVertexBuffer + offset;
        numVertices = instancedData->numVertices;
        while (numVertices--)
        {
            *((RwTexCoords *)vertexBuffer) = *texCoord;
            vertexBuffer += instancedData->stride;
            texCoord++;
        }
    }
    else if (flags & rpWORLDTEXTURED2)
    {
        const RwTexCoords *texCoord1;
        const RwTexCoords *texCoord2;

        texCoord1 = (const RwTexCoords *)(((const RwUInt8 *)(&sector->texCoords[0][instancedData->minVert])));
        texCoord2 = (const RwTexCoords *)(((const RwUInt8 *)(&sector->texCoords[1][instancedData->minVert])));

        vertexBuffer = lockedVertexBuffer + offset;
        numVertices = instancedData->numVertices;
        while (numVertices--)
        {
            *((RwTexCoords *)vertexBuffer) = *texCoord1;
            texCoord1++;

            *(((RwTexCoords *)vertexBuffer) + 1) = *texCoord2;
            texCoord2++;

            vertexBuffer += instancedData->stride;
        }
    }

    /*
     * Unlock the vertex buffer
     */
    IDirect3DVertexBuffer8_Unlock((LPDIRECT3DVERTEXBUFFER8)instancedData->vertexBuffer);

    /*
     * Set the vertex shader flags
     */
    instancedData->vertexShader = fvfFlags;

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8WorldSectorAllInOneNode
 */
static RwBool
D3D8WorldSectorAllInOneNode(RxPipelineNodeInstance *self,
                             const RxPipelineNodeParam *params)
{
    RpWorldSector           *sector;
    RwResEntry              *repEntry;
    RpMeshHeader            *meshHeader;
    RwUInt32                worldFlags;
    _rxD3D8InstanceNodeData *privateData;
    RwInt16                 numMeshes;

    RWFUNCTION(RWSTRING("D3D8WorldSectorAllInOneNode"));
    RWASSERT(NULL != self);
    RWASSERT(NULL != params);

    sector = (RpWorldSector *)RxPipelineNodeParamGetData(params);
    RWASSERT(NULL != sector);

    privateData = (_rxD3D8InstanceNodeData *)self->privateData;

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
            RxD3D8ResEntryHeader    *resEntryHeader;

            resEntryHeader = (RxD3D8ResEntryHeader *)(repEntry + 1);
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
            repEntry = _rxD3D8Instance((void *)sector, (void *)sector, &sector->repEntry,
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
    RwD3D8SetTransformWorld(NULL);

    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

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
 D3D8WorldSectorAllInOnePipelineInit
 */
static RwBool
D3D8WorldSectorAllInOnePipelineInit(RxPipelineNode *node)
{
    _rxD3D8InstanceNodeData *instanceData;

    RWFUNCTION(RWSTRING("D3D8WorldSectorAllInOnePipelineInit"));

    instanceData = (_rxD3D8InstanceNodeData *)node->privateData;

    /* Assert pipeline node private data pointer != NULL  */
    RWASSERT(NULL != instanceData);

    /* Default instance callback */
    instanceData->instanceCallback = (RxD3D8AllInOneInstanceCallBack)(D3D8WorldSectorDefaultInstanceCallback);

    /* Default reinstance callback */
    instanceData->reinstanceCallback = NULL;

    /* Default lighting callback */
    instanceData->lightingCallback = (RxD3D8AllInOneLightingCallBack)(D3D8WorldSectorDefaultLightingCallback);

    /* No execution callback */
    instanceData->renderCallback = (RxD3D8AllInOneRenderCallBack)(_rxD3D8DefaultRenderCallback);

    RWRETURN(TRUE);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   API functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup worldextensionsd3d8
 * \ref RxNodeDefinitionGetD3D8WorldSectorAllInOne returns a
 * pointer to the \ref RxNodeDefinition associated with
 * the world sector version of PowerPipe. This is especially
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
RxNodeDefinitionGetD3D8WorldSectorAllInOne(void)
{
    static RwChar _D3D8ObjWorldSectorAllInOne_csl[] = "nodeD3D8WorldSectorAllInOne.csl";

    static RxNodeDefinition nodeD3D8WorldSectorAllInOneCSL = { /* */
        _D3D8ObjWorldSectorAllInOne_csl,            /* Name */
        {                                           /* Nodemethods */
            D3D8WorldSectorAllInOneNode,         /* +-- nodebody */
            NULL,                                   /* +-- nodeinit */
            NULL,                                   /* +-- nodeterm */
            D3D8WorldSectorAllInOnePipelineInit,    /* +-- pipelinenodeinit */
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

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D8WorldSectorAllInOne"));

    RWRETURN(&nodeD3D8WorldSectorAllInOneCSL);
}
