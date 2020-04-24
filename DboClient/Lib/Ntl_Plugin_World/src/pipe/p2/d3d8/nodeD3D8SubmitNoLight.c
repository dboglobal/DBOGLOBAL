/*
 * Submitting object space triangles to the D3D8 custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */

/****************************************************************************
 *                                                                          *
 * module : nodeD3D8SubmitNoLight.c                                         *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include <d3d8.h>

#include "batypes.h"
#include "bapipe.h"
#include "pip2model.h"

#include "p2core.h"
#include "p2stdcls.h"

#include "drvfns.h"
#include "d3d8device.h"
#include "d3d8rendst.h"

#include "nodeD3D8SubmitNoLight.h"

/****************************************************************************
 local defines
 */

#define IM3D_OLD_FVF    (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))

#define IM3D_FVF        (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))
#define IM3D_NOTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

#define BUFFER_MAX_INDEX    65535

typedef struct _rxD3D8Im3DVertex RxD3D8Im3DVertex;
struct _rxD3D8Im3DVertex
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwUInt32 color;
    RwReal u;
    RwReal v;
};

typedef struct _rxD3D8Im3DVertexNoTex _rxD3D8Im3DVertexNoTex;
struct _rxD3D8Im3DVertexNoTex
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwUInt32 color;
};

/****************************************************************************
 Global variables
 */

/****************************************************************************
 Local static variables
 */

static const rwIm3DPool  *_rwD3D8ImmPool = NULL;

static const D3DPRIMITIVETYPE _RwD3D8PrimConv[6] =
{
    (D3DPRIMITIVETYPE)0,                      /* rwPRIMTYPENAPRIMTYPE */
    D3DPT_LINELIST,         /* rwPRIMTYPELINELIST */
    D3DPT_LINESTRIP,        /* rwPRIMTYPEPOLYLINE */
    D3DPT_TRIANGLELIST,     /* rwPRIMTYPETRILIST */
    D3DPT_TRIANGLESTRIP,    /* rwPRIMTYPETRISTRIP */
    D3DPT_TRIANGLEFAN       /* rwPRIMTYPETRIFAN */
};

static RwUInt32                 CurrentBaseIndex3D = 0;
static LPDIRECT3DVERTEXBUFFER8  CurrentVertexBuffer3D = NULL; /* Vertex buffer */

static RwUInt32      IB3DOffset = 0;
static LPDIRECT3DINDEXBUFFER8 IndexBuffer3D = NULL; /* Index buffer */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/****************************************************************************
 _rwD3D8Im3DRenderClose

 On entry   :
 On exit    :
 */
void
_rwD3D8Im3DRenderClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8Im3DRenderClose"));

    if (IndexBuffer3D)
    {
        IDirect3DIndexBuffer8_Release(IndexBuffer3D);
        IndexBuffer3D = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8Im3DRenderOpen

 On entry   :
 On exit    :
 */
RwBool
_rwD3D8Im3DRenderOpen(void)
{
    RwBool rv = FALSE;
    RwUInt32    usage;
    D3DPOOL     d3dPool;

    RWFUNCTION(RWSTRING("_rwD3D8Im3DRenderOpen"));

    IB3DOffset = 0;

    usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;

    if (_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        d3dPool = D3DPOOL_DEFAULT;
    }
    else
    {
        d3dPool = D3DPOOL_SYSTEMMEM;
    }

    if ( D3D_OK == IDirect3DDevice8_CreateIndexBuffer(_RwD3DDevice,
                BUFFER_MAX_INDEX * sizeof(RwUInt16),
                usage, D3DFMT_INDEX16, d3dPool,
                &IndexBuffer3D) )
    {
        rv = TRUE;
    }

    if (!rv)
    {
        _rwD3D8Im3DRenderClose();
    }

    RWRETURN(rv);
}

/****************************************************************************
 rxD3D8SubmitNode
*/
static RwBool
rxD3D8SubmitNode(RxPipelineNodeInstance *self __RWUNUSED__,
                 const RxPipelineNodeParam *params __RWUNUSED__)
{
    const RwIm3DVertex      *verts;
    const _rwIm3DPoolStash  *stash;
    const RwImVertexIndex   *indices;
    RwBool              useTexCoords;
    RwUInt32            fvf;
    RwUInt32            stride;
    RwUInt32            numVerts;
    RwUInt32            numPrimitives;
    RwUInt32            numIndices;
    RwUInt32            primitiveType;
    HRESULT             hr = E_FAIL;

    RWFUNCTION(RWSTRING("rxD3D8SubmitNode"));

    stash = &_rwD3D8ImmPool->stash;

    verts = _rwD3D8ImmPool->elements;
    RWASSERT(NULL != verts);

    /* Check vertex type */
    if (stash->flags & rwIM3D_VERTEXUV)
    {
        useTexCoords = TRUE;

        fvf = IM3D_FVF;

        stride = sizeof(RxD3D8Im3DVertex);
    }
    else
    {
        useTexCoords = FALSE;

        fvf = IM3D_NOTEX_FVF;

        stride = sizeof(_rxD3D8Im3DVertexNoTex);
    }

    /* Set blending modes */
    if (stash->flags & rwIM3D_ALLOPAQUE)
    {
        _rwD3D8RenderStateVertexAlphaEnable(FALSE);
    }
    else
    {
        _rwD3D8RenderStateVertexAlphaEnable(TRUE);
    }

    /* World matrix */
    RwD3D8SetTransformWorld(stash->ltm);

    /* Disable lighting */
    RwD3D8SetRenderState(D3DRS_LIGHTING, FALSE);

    RwD3D8SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    /* Enable clipping if needed*/
    if (stash->flags & rwIM3D_NOCLIP)
    {
        RwD3D8SetRenderState(D3DRS_CLIPPING, FALSE);
    }
    else
    {
        RwD3D8SetRenderState(D3DRS_CLIPPING, TRUE);
    }

    /* Disable any pixel shader */
    RwD3D8SetPixelShader(0);

    /* Set the vertex shader */
    RwD3D8SetVertexShader(fvf);

    primitiveType = _RwD3D8PrimConv[stash->primType];

    numVerts = _rwD3D8ImmPool->numElements;

    if (numVerts > 0)
    {
        if ((NULL != stash->indices) && (stash->numIndices > 0))
        {
    #if (defined (RWDEBUG))
            RWASSERT((rwPRIMTYPELINELIST == stash->primType) ||
                     (rwPRIMTYPEPOLYLINE == stash->primType) ||
                     (rwPRIMTYPETRILIST == stash->primType) ||
                     (rwPRIMTYPETRISTRIP == stash->primType) ||
                     (rwPRIMTYPETRIFAN == stash->primType));
    #endif /* (defined (RWDEBUG)) */

            indices = stash->indices;
            RWASSERT(NULL != indices);

            numIndices = stash->numIndices;

            switch (primitiveType)
            {
                case D3DPT_LINELIST:
                    {
                        RWASSERT(numIndices > 1);
                        numPrimitives = numIndices / 2;
                        break;
                    }

                case D3DPT_LINESTRIP:
                    {
                        RWASSERT(numIndices > 1);
                        numPrimitives = numIndices - 1;
                        break;
                    }

                case D3DPT_TRIANGLELIST:
                    {
                        RWASSERT(numIndices > 2);
                        numPrimitives = numIndices / 3;
                        break;
                    }

                case D3DPT_TRIANGLESTRIP:
                    {
                        RWASSERT(numIndices > 2);
                        numPrimitives = numIndices - 2;
                        break;
                    }

                case D3DPT_TRIANGLEFAN:
                    {
                        RWASSERT(numIndices > 2);
                        numPrimitives = numIndices - 2;
                        break;
                    }
            }

            /* Check for inefficient index primitive */
            if (numIndices<=numVerts && numIndices<=8)
            {
                /* Use numIndices instead of numVerts */
                void    *bufferMem;

                /* Fill Vertex Buffer */
                if (RwD3D8DynamicVertexBufferLock( stride,
                                                   numIndices,
                                                   (void**)&CurrentVertexBuffer3D,
                                                   &bufferMem,
                                                   &CurrentBaseIndex3D))
                {
                    float *vertexDes = (float *)bufferMem;
                    RwUInt32 i;

                    if (useTexCoords)
                    {
                        for(i=0; i<numIndices; i++)
                        {
                            const float *vertexSrc = (const float *)(verts + indices[i]);

                            vertexDes[0] = vertexSrc[0];
                            vertexDes[1] = vertexSrc[1];
                            vertexDes[2] = vertexSrc[2];

                            ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                            vertexDes[4] = vertexSrc[7];
                            vertexDes[5] = vertexSrc[8];

                            vertexDes+=6;
                        }
                    }
                    else
                    {
                        for(i=0; i<numIndices; i++)
                        {
                            const float *vertexSrc = (const float *)(verts + indices[i]);

                            vertexDes[0] = vertexSrc[0];
                            vertexDes[1] = vertexSrc[1];
                            vertexDes[2] = vertexSrc[2];

                            ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                            vertexDes+=4;
                        }
                    }

                    RwD3D8DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                    RwD3D8SetStreamSource(0, CurrentVertexBuffer3D, stride);

                    _rwD3D8RenderStateFlushCache();

                    hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                                                (D3DPRIMITIVETYPE)primitiveType,
                                                                CurrentBaseIndex3D,
                                                                numPrimitives));
                }
            }
            else
            {
                /* Check index buffer */
                if (IndexBuffer3D && (numIndices <= BUFFER_MAX_INDEX))
                {
                    void    *bufferMem;

                    /* Fill Vertex Buffer */
                    if (RwD3D8DynamicVertexBufferLock( stride,
                                                       numVerts,
                                                       (void**)&CurrentVertexBuffer3D,
                                                       &bufferMem,
                                                       &CurrentBaseIndex3D))
                    {
                        float *vertexDes = (float *)bufferMem;
                        const float *vertexSrc = (const float *)(verts);
                        RwUInt32 i = numVerts;

                        if (useTexCoords)
                        {
                            do
                            {
                                vertexDes[0] = vertexSrc[0];
                                vertexDes[1] = vertexSrc[1];
                                vertexDes[2] = vertexSrc[2];

                                ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                                vertexDes[4] = vertexSrc[7];
                                vertexDes[5] = vertexSrc[8];

                                vertexDes+=6;
                                vertexSrc+=9;
                                i--;
                            }
                            while(i);
                        }
                        else
                        {
                            do
                            {
                                vertexDes[0] = vertexSrc[0];
                                vertexDes[1] = vertexSrc[1];
                                vertexDes[2] = vertexSrc[2];

                                ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                                vertexDes+=4;
                                vertexSrc+=9;
                                i--;
                            }
                            while(i);
                        }

                        RwD3D8DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                        /* Fill the Index Buffer */
                        if ((IB3DOffset + numIndices) > BUFFER_MAX_INDEX)
                        {
                            IB3DOffset = 0;

                            hr = IDirect3DIndexBuffer8_Lock(IndexBuffer3D,
                                                             0,
                                                             numIndices * sizeof(RwUInt16),
                                                             (RwUInt8 **)&bufferMem,
                                                             D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                        }
                        else
                        {
                            hr = IDirect3DIndexBuffer8_Lock(IndexBuffer3D,
                                                             IB3DOffset * sizeof(RwUInt16),
                                                             numIndices * sizeof(RwUInt16),
                                                             (RwUInt8 **)&bufferMem,
                                                             D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
                        }

                        if (SUCCEEDED(hr))
                        {
                            memcpy(bufferMem, indices, numIndices * sizeof(RwUInt16));

                            IDirect3DIndexBuffer8_Unlock(IndexBuffer3D);

                            RwD3D8SetStreamSource(0, CurrentVertexBuffer3D, stride);

                            RwD3D8SetIndices(IndexBuffer3D, CurrentBaseIndex3D);

                            _rwD3D8RenderStateFlushCache();

                            hr = DXCHECK(IDirect3DDevice8_DrawIndexedPrimitive(_RwD3DDevice,
                                                                               (D3DPRIMITIVETYPE)primitiveType,
                                                                               0, numVerts,
                                                                               IB3DOffset, numPrimitives));

                            IB3DOffset += numIndices;
                        }
                    }
                }
            }

            if (FAILED(hr))
            {
                RwD3D8SetVertexShader(IM3D_OLD_FVF);
                RwD3D8SetStreamSource(0, NULL, 0);
                RwD3D8SetIndices(NULL, 0);

                _rwD3D8RenderStateFlushCache();

                DXCHECK(IDirect3DDevice8_DrawIndexedPrimitiveUP(_RwD3DDevice,
                                                                (D3DPRIMITIVETYPE)primitiveType,
                                                                0,
                                                                numVerts,
                                                                numPrimitives,
                                                                indices, D3DFMT_INDEX16,
                                                                verts,
                                                                sizeof(RwIm3DVertex)));
            }
        }
        else
        {
            void    *bufferMem;

            switch (primitiveType)
            {
                case D3DPT_LINELIST:
                    {
                        RWASSERT(numVerts > 1);
                        numPrimitives = numVerts / 2;
                        break;
                    }

                case D3DPT_LINESTRIP:
                    {
                        RWASSERT(numVerts > 1);
                        numPrimitives = numVerts - 1;
                        break;
                    }

                case D3DPT_TRIANGLELIST:
                    {
                        RWASSERT(numVerts > 2);
                        numPrimitives = numVerts / 3;
                        break;
                    }

                case D3DPT_TRIANGLESTRIP:
                    {
                        RWASSERT(numVerts > 2);
                        numPrimitives = numVerts - 2;
                        break;
                    }

                case D3DPT_TRIANGLEFAN:
                    {
                        RWASSERT(numVerts > 2);
                        numPrimitives = numVerts - 2;
                        break;
                    }
            }


            /* Fill Vertex Buffer */
            if (RwD3D8DynamicVertexBufferLock( stride,
                                               numVerts,
                                               (void**)&CurrentVertexBuffer3D,
                                               &bufferMem,
                                               &CurrentBaseIndex3D))
            {
                float *vertexDes = (float *)bufferMem;
                const float *vertexSrc = (const float *)(verts);
                RwUInt32 i = numVerts;

                if (useTexCoords)
                {
                    do
                    {
                        vertexDes[0] = vertexSrc[0];
                        vertexDes[1] = vertexSrc[1];
                        vertexDes[2] = vertexSrc[2];

                        ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                        vertexDes[4] = vertexSrc[7];
                        vertexDes[5] = vertexSrc[8];

                        vertexDes+=6;
                        vertexSrc+=9;
                        i--;
                    }
                    while(i);
                }
                else
                {
                    do
                    {
                        vertexDes[0] = vertexSrc[0];
                        vertexDes[1] = vertexSrc[1];
                        vertexDes[2] = vertexSrc[2];

                        ((RwUInt32 *)vertexDes)[3] = ((const RwUInt32 *)vertexSrc)[6];

                        vertexDes+=4;
                        vertexSrc+=9;
                        i--;
                    }
                    while(i);
                }

                RwD3D8DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                RwD3D8SetStreamSource(0, CurrentVertexBuffer3D, stride);

                _rwD3D8RenderStateFlushCache();

                hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                                            (D3DPRIMITIVETYPE)primitiveType,
                                                            CurrentBaseIndex3D,
                                                            numPrimitives));
            }

            if (FAILED(hr))
            {
                RwD3D8SetVertexShader(IM3D_OLD_FVF);
                RwD3D8SetStreamSource(0, NULL, 0);
                RwD3D8SetIndices(NULL, 0);

                _rwD3D8RenderStateFlushCache();

                DXCHECK(IDirect3DDevice8_DrawPrimitiveUP(_RwD3DDevice,
                                                        (D3DPRIMITIVETYPE)primitiveType,
                                                         numPrimitives,
                                                         verts,
                                                         sizeof(RwIm3DVertex)));
            }
        }

        #if defined( RWMETRICS )
        switch ( primitiveType )
        {
            case D3DPT_TRIANGLELIST:
            case D3DPT_TRIANGLESTRIP:
            case D3DPT_TRIANGLEFAN:
            {
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
            }
            break;
        }
        #endif /* defined( RWMETRICS ) */
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 D3D8ImmInstanceNode
 */
static RwBool
D3D8ImmInstanceNode(RxPipelineNodeInstance *self __RWUNUSED__,
                    const RxPipelineNodeParam *params)
{
    RWFUNCTION(RWSTRING("D3D8ImmInstanceNode"));

    _rwD3D8ImmPool = (const rwIm3DPool *)RxPipelineNodeParamGetData(params);

    RWRETURN(TRUE);
}

/**
 * \ingroup cored3d8
 * \ref RxNodeDefinitionGetD3D8ImmInstance returns a pointer to a node.
 *
 * \return pointer to a node on success, NULL otherwise.
 *
 * \see RxNodeDefinitionGetD3D8ImmInstance
 */

/****************************************************************************
 RxNodeDefinitionGetD3D8ImmInstance
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D8ImmInstance(void)
{
    static RwChar   _ImmInstance_csl[] = RWSTRING("ImmInstance.csl");
    static RxNodeDefinition nodeImmInstanceCSL = {
        _ImmInstance_csl,           /* Name */
        {                           /* Nodemethods */
            D3D8ImmInstanceNode,    /* +-- nodebody */
            NULL,                   /* +-- nodeinit */
            NULL,                   /* +-- nodeterm */
            NULL,                   /* +-- pipelinenodeinit */
            NULL,                   /* +-- pipelineNodeTerm */
            NULL,                   /* +-- pipelineNodeConfig */
            NULL,                   /* +-- configMsgHandler */
        },
        {                           /* Io */
            0,                      /* +-- NumClustersOfInterest */
            NULL,                   /* +-- ClustersOfInterest */
            NULL,                   /* +-- InputRequirements */
            0,                      /* +-- NumOutputs */
            NULL                    /* +-- Outputs */
        },
        0,                          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,             /* editable */
        0                           /* inPipes */
    };

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D8ImmInstance"));

    RWRETURN((&nodeImmInstanceCSL));
}

/**
 * \ingroup cored3d8
 * \ref RxNodeDefinitionGetD3D8SubmitNoLight returns a pointer to a node
 * to dispatch polygons using D3D8 transform facilities.
 *
 * The node supports the submission of line lists and strips, and of triangle
 * lists, strips and fans.
 *
 * \return pointer to a node to submit triangles to the rasterizer on success,
 * NULL otherwise.
 *
 * \see RxNodeDefinitionGetD3D8ImmInstance
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D8SubmitNoLight(void)
{
    static RwChar _SubmitNoLight_csl[] = "nodeD3D8SubmitNoLight.csl";
    static RxNodeDefinition nodeD3D8SubmitNoLight = {
        _SubmitNoLight_csl,         /* Name */
        {                           /* Nodemethods */
            rxD3D8SubmitNode,       /* +-- nodebody */
            NULL,                   /* +-- nodeinit */
            NULL,                   /* +-- nodeterm */
            NULL,                   /* +-- pipelinenodeinit */
            NULL,                   /* +-- pipelineNodeTerm */
            NULL,                   /* +-- pipelineNodeConfig */
            NULL,                   /* +-- configMsgHandler */
        },
        {                           /* Io */
            0,                      /* +-- NumClustersOfInterest */
            NULL,                   /* +-- ClustersOfInterest */
            NULL,                   /* +-- InputRequirements */
            0,                      /* +-- NumOutputs */
            NULL                    /* +-- Outputs */
        },
        0,                          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,             /* editable */
        0                           /* inPipes */
    };

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D8SubmitNoLight"));

    RWRETURN(&nodeD3D8SubmitNoLight);
}
