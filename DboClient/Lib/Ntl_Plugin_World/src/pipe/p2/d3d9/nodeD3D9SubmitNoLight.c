/*
 * Submitting object space triangles to the D3D9 custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */

/****************************************************************************
 *                                                                          *
 * module : nodeD3D9SubmitNoLight.c                                         *
 *                                                                          *
 * purpose:                                                                 *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include <d3d9.h>

#include "batypes.h"
#include "bapipe.h"
#include "pip2model.h"

#include "p2core.h"
#include "p2stdcls.h"

#include "drvfns.h"
#include "d3d9device.h"
#include "d3d9rendst.h"
#include "d3d9vertexbuffer.h"

#include "nodeD3D9SubmitNoLight.h"

/****************************************************************************
 local defines
 */

#define BUFFER_MAX_INDEX    65535

typedef struct _rxD3D9Im3DVertex RxD3D9Im3DVertex;
struct _rxD3D9Im3DVertex
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwUInt32 color;
    RwReal u;
    RwReal v;
};

typedef struct _rxD3D9Im3DVertexNoTex _rxD3D9Im3DVertexNoTex;
struct _rxD3D9Im3DVertexNoTex
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

static const rwIm3DPool  *_rwD3D9ImmPool = NULL;

static const D3DPRIMITIVETYPE _RwD3D9PrimConv[6] =
{
    (D3DPRIMITIVETYPE)0,                      /* rwPRIMTYPENAPRIMTYPE */
    D3DPT_LINELIST,         /* rwPRIMTYPELINELIST */
    D3DPT_LINESTRIP,        /* rwPRIMTYPEPOLYLINE */
    D3DPT_TRIANGLELIST,     /* rwPRIMTYPETRILIST */
    D3DPT_TRIANGLESTRIP,    /* rwPRIMTYPETRISTRIP */
    D3DPT_TRIANGLEFAN       /* rwPRIMTYPETRIFAN */
};

static RwUInt32                 CurrentBaseIndex3D = 0;
static LPDIRECT3DVERTEXBUFFER9  CurrentVertexBuffer3D = NULL; /* Vertex buffer */

static RwUInt32      IB3DOffset = 0;
static LPDIRECT3DINDEXBUFFER9 IndexBuffer3D = NULL; /* Index buffer */

static LPDIRECT3DVERTEXDECLARATION9 VertexDeclIm3DOld = NULL;
static LPDIRECT3DVERTEXDECLARATION9 VertexDeclIm3D = NULL;
static LPDIRECT3DVERTEXDECLARATION9 VertexDeclIm3DNoTex = NULL;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/****************************************************************************
 _rwD3D9Im3DRenderClose

 On entry   :
 On exit    :
 */
void
_rwD3D9Im3DRenderClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D9Im3DRenderClose"));

    if (IndexBuffer3D)
    {
        IDirect3DIndexBuffer9_Release(IndexBuffer3D);
        IndexBuffer3D = NULL;
    }

    if (VertexDeclIm3DNoTex != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexDeclIm3DNoTex);
        VertexDeclIm3DNoTex = NULL;
    }

    if (VertexDeclIm3D != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexDeclIm3D);
        VertexDeclIm3D = NULL;
    }

    if (VertexDeclIm3DOld != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexDeclIm3DOld);
        VertexDeclIm3DOld = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D9Im3DRenderOpen

 On entry   :
 On exit    :
 */
RwBool
_rwD3D9Im3DRenderOpen(void)
{
    RwBool rv = FALSE;
    RwUInt32    usage;
    D3DPOOL     d3dPool;

    RWFUNCTION(RWSTRING("_rwD3D9Im3DRenderOpen"));

    IB3DOffset = 0;

    usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;

    if (_RwD3D9DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        d3dPool = D3DPOOL_DEFAULT;
    }
    else
    {
        d3dPool = D3DPOOL_SYSTEMMEM;
    }

    if ( D3D_OK == IDirect3DDevice9_CreateIndexBuffer(_RwD3DDevice,
                BUFFER_MAX_INDEX * sizeof(RwUInt16),
                usage, D3DFMT_INDEX16, d3dPool,
                &IndexBuffer3D,
                NULL) )
    {
        rv = TRUE;
    }

    if (!rv)
    {
        _rwD3D9Im3DRenderClose();
    }
    else
    {
        D3DVERTEXELEMENT9 declaration[5];
        RwUInt32 declarationIndex, offset;

        /*
         * Old Format
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

        /* Normals */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_NORMAL;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwV3d);

        /* Pre-lighting */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwRGBA);

        /* Texture coordinates */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwV2d);

        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RWASSERT(offset == sizeof(RwIm3DVertex));

        RwD3D9CreateVertexDeclaration(declaration,
                                      &VertexDeclIm3DOld);

        /*
         * New Format
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

        /* Pre-lighting */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwRGBA);

        /* Texture coordinates */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwV2d);

        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RWASSERT(offset == sizeof(RxD3D9Im3DVertex));

        RwD3D9CreateVertexDeclaration(declaration,
                                      &VertexDeclIm3D);


        /*
         * New Format no texture coords
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

        /* Pre-lighting */
        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = offset;
        declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        offset += sizeof(RwRGBA);

        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RWASSERT(offset == sizeof(_rxD3D9Im3DVertexNoTex));

        RwD3D9CreateVertexDeclaration(declaration,
                                      &VertexDeclIm3DNoTex);
    }

    RWRETURN(rv);
}

/****************************************************************************
 rxD3D9SubmitNode
*/
static RwBool
rxD3D9SubmitNode(RxPipelineNodeInstance *self __RWUNUSED__,
                 const RxPipelineNodeParam *params __RWUNUSED__)
{
    const RwIm3DVertex      *verts;
    const _rwIm3DPoolStash  *stash;
    const RwImVertexIndex   *indices;
    RwBool              useTexCoords;
    LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
    RwUInt32            stride;
    RwUInt32            numVerts;
    RwUInt32            numPrimitives;
    RwUInt32            numIndices;
    RwUInt32            primitiveType;
    HRESULT             hr = E_FAIL;

    RWFUNCTION(RWSTRING("rxD3D9SubmitNode"));

    stash = &_rwD3D9ImmPool->stash;

    verts = _rwD3D9ImmPool->elements;
    RWASSERT(NULL != verts);

    /* Check vertex type */
    if (stash->flags & rwIM3D_VERTEXUV)
    {
        useTexCoords = TRUE;

        vertexDecl = VertexDeclIm3D;

        stride = sizeof(RxD3D9Im3DVertex);
    }
    else
    {
        useTexCoords = FALSE;

        vertexDecl = VertexDeclIm3DNoTex;

        stride = sizeof(_rxD3D9Im3DVertexNoTex);
    }

    /* Set blending modes */
    if (stash->flags & rwIM3D_ALLOPAQUE)
    {
        _rwD3D9RenderStateVertexAlphaEnable(FALSE);
    }
    else
    {
        _rwD3D9RenderStateVertexAlphaEnable(TRUE);
    }

    /* World matrix */
    RwD3D9SetTransformWorld(stash->ltm);

    /* Disable lighting */
    RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

    RwD3D9SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

    /* Enable clipping if needed*/
    if (stash->flags & rwIM3D_NOCLIP)
    {
        RwD3D9SetRenderState(D3DRS_CLIPPING, FALSE);
    }
    else
    {
        RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);
    }

    /* Set color ops */
    if (_rwD3D9RWGetRasterStage(0) == NULL)
    {
        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }
    else
    {
        RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    /* Set the vertex Declaration */
    RwD3D9SetVertexDeclaration(vertexDecl);

    /* Disable any vertex shader */
    RwD3D9SetVertexShader(NULL);

    /* Disable any pixel shader */
    RwD3D9SetPixelShader(NULL);

    primitiveType = _RwD3D9PrimConv[stash->primType];

    numVerts = _rwD3D9ImmPool->numElements;

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
                if (RwD3D9DynamicVertexBufferLock( stride,
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

                    RwD3D9DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                    RwD3D9SetStreamSource(0, CurrentVertexBuffer3D, 0, stride);

                    _rwD3D9RenderStateFlushCache();

                    hr = DXCHECK(IDirect3DDevice9_DrawPrimitive(_RwD3DDevice,
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
                    if (RwD3D9DynamicVertexBufferLock( stride,
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

                        RwD3D9DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                        /* Fill the Index Buffer */
                        if ((IB3DOffset + numIndices) > BUFFER_MAX_INDEX)
                        {
                            IB3DOffset = 0;

                            hr = IDirect3DIndexBuffer9_Lock(IndexBuffer3D,
                                                             0,
                                                             numIndices * sizeof(RwUInt16),
                                                             (RwUInt8 **)&bufferMem,
                                                             D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                        }
                        else
                        {
                            hr = IDirect3DIndexBuffer9_Lock(IndexBuffer3D,
                                                             IB3DOffset * sizeof(RwUInt16),
                                                             numIndices * sizeof(RwUInt16),
                                                             (RwUInt8 **)&bufferMem,
                                                             D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
                        }

                        if (SUCCEEDED(hr))
                        {
                            memcpy(bufferMem, indices, numIndices * sizeof(RwUInt16));

                            IDirect3DIndexBuffer9_Unlock(IndexBuffer3D);

                            RwD3D9SetStreamSource(0, CurrentVertexBuffer3D, 0, stride);

                            RwD3D9SetIndices(IndexBuffer3D);

                            _rwD3D9RenderStateFlushCache();

                            hr = DXCHECK(IDirect3DDevice9_DrawIndexedPrimitive(_RwD3DDevice,
                                                                               (D3DPRIMITIVETYPE)primitiveType,
                                                                               CurrentBaseIndex3D,
                                                                               0, numVerts,
                                                                               IB3DOffset, numPrimitives));

                            IB3DOffset += numIndices;
                        }
                    }
                }
            }

            if (FAILED(hr))
            {
                RwD3D9SetVertexDeclaration(VertexDeclIm3DOld);
                RwD3D9SetStreamSource(0, NULL, 0, 0);
                RwD3D9SetIndices(NULL);

                _rwD3D9RenderStateFlushCache();

                DXCHECK(IDirect3DDevice9_DrawIndexedPrimitiveUP(_RwD3DDevice,
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
            if (RwD3D9DynamicVertexBufferLock( stride,
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

                RwD3D9DynamicVertexBufferUnlock(CurrentVertexBuffer3D);

                RwD3D9SetStreamSource(0, CurrentVertexBuffer3D, 0, stride);

                _rwD3D9RenderStateFlushCache();

                hr = DXCHECK(IDirect3DDevice9_DrawPrimitive(_RwD3DDevice,
                                                            (D3DPRIMITIVETYPE)primitiveType,
                                                            CurrentBaseIndex3D,
                                                            numPrimitives));
            }

            if (FAILED(hr))
            {
                RwD3D9SetVertexDeclaration(VertexDeclIm3DOld);
                RwD3D9SetStreamSource(0, NULL, 0, 0);

                _rwD3D9RenderStateFlushCache();

                DXCHECK(IDirect3DDevice9_DrawPrimitiveUP(_RwD3DDevice,
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
 D3D9ImmInstanceNode
 */
static RwBool
D3D9ImmInstanceNode(RxPipelineNodeInstance *self __RWUNUSED__,
                    const RxPipelineNodeParam *params)
{
    RWFUNCTION(RWSTRING("D3D9ImmInstanceNode"));

    _rwD3D9ImmPool = (const rwIm3DPool *)RxPipelineNodeParamGetData(params);

    RWRETURN(TRUE);
}

/**
 * \ingroup cored3d9
 * \ref RxNodeDefinitionGetD3D9ImmInstance returns a pointer to a node.
 *
 * \return pointer to a node on success, NULL otherwise.
 *
 * \see RxNodeDefinitionGetD3D9ImmInstance
 */

/****************************************************************************
 RxNodeDefinitionGetD3D9ImmInstance
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D9ImmInstance(void)
{
    static RwChar   _ImmInstance_csl[] = RWSTRING("ImmInstance.csl");
    static RxNodeDefinition nodeImmInstanceCSL = {
        _ImmInstance_csl,           /* Name */
        {                           /* Nodemethods */
            D3D9ImmInstanceNode,    /* +-- nodebody */
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

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9ImmInstance"));

    RWRETURN((&nodeImmInstanceCSL));
}

/**
 * \ingroup cored3d9
 * \ref RxNodeDefinitionGetD3D9SubmitNoLight returns a pointer to a node
 * to dispatch polygons using D3D9 transform facilities.
 *
 * The node supports the submission of line lists and strips, and of triangle
 * lists, strips and fans.
 *
 * \return pointer to a node to submit triangles to the rasterizer on success,
 * NULL otherwise.
 *
 * \see RxNodeDefinitionGetD3D9ImmInstance
 */
RxNodeDefinition *
RxNodeDefinitionGetD3D9SubmitNoLight(void)
{
    static RwChar _SubmitNoLight_csl[] = "nodeD3D9SubmitNoLight.csl";
    static RxNodeDefinition nodeD3D9SubmitNoLight = {
        _SubmitNoLight_csl,         /* Name */
        {                           /* Nodemethods */
            rxD3D9SubmitNode,       /* +-- nodebody */
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

    RWAPIFUNCTION(RWSTRING("RxNodeDefinitionGetD3D9SubmitNoLight"));

    RWRETURN(&nodeD3D9SubmitNoLight);
}
