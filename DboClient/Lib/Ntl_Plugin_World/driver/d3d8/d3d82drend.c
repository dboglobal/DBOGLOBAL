/***************************************************************************
 *                                                                         *
 * Module  : dl2drend.c                                                    *
 *                                                                         *
 * Purpose : 2D primitive rendering functions.                             *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */
#include "batypes.h"
#include "badebug.h"
#include "batextur.h"
#include "baimage.h"
#include "baresour.h"
#include "bamatrix.h"
#include "baimmedi.h"
#include "barwtyp.h"
#include "drvmodel.h"
#include "drvfns.h"

#include "d3d8device.h"
#include "d3d8rendst.h"

#ifdef RWMETRICS
#include "d3d8metric.h"
#endif /* RWMETRICS */

#include "d3d82drend.h"

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

#define NUMPRIMTYPES    7
static const D3DPRIMITIVETYPE _RwD3D8PrimConvTable[NUMPRIMTYPES] =
{
    (D3DPRIMITIVETYPE)0,                      /* rwPRIMTYPENAPRIMTYPE */
    (D3DPRIMITIVETYPE)D3DPT_LINELIST,         /* rwPRIMTYPELINELIST */
    (D3DPRIMITIVETYPE)D3DPT_LINESTRIP,        /* rwPRIMTYPEPOLYLINE */
    (D3DPRIMITIVETYPE)D3DPT_TRIANGLELIST,     /* rwPRIMTYPETRILIST */
    (D3DPRIMITIVETYPE)D3DPT_TRIANGLESTRIP,    /* rwPRIMTYPETRISTRIP */
    (D3DPRIMITIVETYPE)D3DPT_TRIANGLEFAN,      /* rwPRIMTYPETRIFAN */
    (D3DPRIMITIVETYPE)D3DPT_POINTLIST
};

#define IM2D_FVF    (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0))

#define BUFFER_MAX_INDEX    10000

#define BUFFER_MAX_VERTEX_SIZE    (256 * 1024)
#define BUFFER_MAX_VERTEX_INDEX    (BUFFER_MAX_VERTEX_SIZE / sizeof(RwIm2DVertex))

static RwBool                   UseOwnVertexBuffer = FALSE;

static RwUInt32                 CurrentBaseIndex = 0;
static LPDIRECT3DVERTEXBUFFER8  CurrentVertexBuffer = NULL;

static RwUInt32                 IB2DOffset = 0;
static LPDIRECT3DINDEXBUFFER8   IndexBuffer2D = NULL; /* Index buffer */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                              Render functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwD3D8Im2DRenderOpen

 On exit    : TRUE on succes
 */
RwBool
_rwD3D8Im2DRenderOpen(void)
{
    RwBool      rv = FALSE;
    RwUInt32    usage;
    D3DPOOL     d3dPool;

    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderOpen"));

    /* Create dynamic vertex buffer for non T&L video cards */
    UseOwnVertexBuffer = FALSE;

    if((_RwD3D8DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0)
    {
        const RwUInt32 memAvailable =
                        IDirect3DDevice8_GetAvailableTextureMem(_RwD3DDevice);      

        if (memAvailable > (8 * 1024 * 1024))
        {
            usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC | D3DUSAGE_DONOTCLIP;

            d3dPool = D3DPOOL_DEFAULT;

            if ( D3D_OK == DXCHECK(IDirect3DDevice8_CreateVertexBuffer(_RwD3DDevice,
                        BUFFER_MAX_VERTEX_SIZE,
                        usage, IM2D_FVF, d3dPool,
                        &CurrentVertexBuffer)) )
            {
                UseOwnVertexBuffer = TRUE;

                CurrentBaseIndex = 0;
            }
        }
    }

    /* Create dynamic index buffer */
    IB2DOffset = 0;

    usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC | D3DUSAGE_DONOTCLIP;

    d3dPool = D3DPOOL_DEFAULT;

    if ( D3D_OK == DXCHECK(IDirect3DDevice8_CreateIndexBuffer(_RwD3DDevice,
                BUFFER_MAX_INDEX * sizeof(RwUInt16),
                usage, D3DFMT_INDEX16, d3dPool,
                &IndexBuffer2D)) )
    {
        rv = TRUE;
    }

    if(!rv)
    {
        _rwD3D8Im2DRenderClose();
    }

    RWRETURN(rv);
}

/****************************************************************************
 rxD3D8Im2DRenderFlush

  On exit    :  TRUE on succes
 */
static void
rxD3D8Im2DRenderFlush(void)
{
    RWFUNCTION(RWSTRING("rxD3D8Im2DRenderFlush"));

    /* Set up for Im2D Render */

    /* Set the vertex shader */
    RwD3D8SetVertexShader(IM2D_FVF);

    /* Disable any pixel shader */
    RwD3D8SetPixelShader(0);

    /* Disable clipping */
    RwD3D8SetRenderState(D3DRS_CLIPPING, FALSE);

    /* Disable lighting */
    RwD3D8SetRenderState(D3DRS_LIGHTING, FALSE);

    RwD3D8SetStreamSource(0, CurrentVertexBuffer, sizeof(RwIm2DVertex));

    _rwD3D8RenderStateFlushCache();

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8Im2DRenderClose

 On entry   : 
 On exit    :
 */
void
_rwD3D8Im2DRenderClose(void)
{
    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderClose"));

    if (UseOwnVertexBuffer)
    {
        if (CurrentVertexBuffer)
        {
            IDirect3DVertexBuffer8_Release(CurrentVertexBuffer);
            CurrentVertexBuffer = NULL;
        }
    }

    if (IndexBuffer2D)
    {
        IDirect3DIndexBuffer8_Release(IndexBuffer2D);
        IndexBuffer2D = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rwD3D8Im2DRenderLine

 On entry   : vertices, num vertices in array
            : index of line ends
 On exit    :
 */
RwBool
_rwD3D8Im2DRenderLine(RwIm2DVertex *verts,
                      RwInt32 numVerts __RWUNUSED__,
                      RwInt32 vert1,
                      RwInt32 vert2)
{
    const RwRaster  *raster;
    RwIm2DVertex    *bufferMem = NULL;

    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderLine"));
    RWASSERT(verts);
    RWASSERT(dgGGlobals.curCamera);

    raster = RwCameraGetRaster(dgGGlobals.curCamera);
    RWASSERT(raster);

    /* Fill Vertex Buffer */
    if (UseOwnVertexBuffer)
    {
        if ((CurrentBaseIndex + 2) > BUFFER_MAX_VERTEX_INDEX)
        {
            CurrentBaseIndex = 0;

            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        0,
                                        2 * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
        }
        else
        {
            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        CurrentBaseIndex * sizeof(RwIm2DVertex),
                                        2 * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
        }
    }
    else
    {
        RwD3D8DynamicVertexBufferLock(sizeof(RwIm2DVertex),
                                      2,
                                      (void**)&CurrentVertexBuffer,
                                      (void**)&bufferMem,
                                      &CurrentBaseIndex);
    }

    if (bufferMem != NULL)
    {
        const RwReal offsetX = (RwReal)(raster->nOffsetX);
        const RwReal offsetY = (RwReal)(raster->nOffsetY);
        HRESULT hr;

        bufferMem[0].x = verts[vert1].x + offsetX;
        bufferMem[0].y = verts[vert1].y + offsetY;
        bufferMem[0].z= verts[vert1].z;
        bufferMem[0].rhw= verts[vert1].rhw;
        bufferMem[0].emissiveColor= verts[vert1].emissiveColor;
        bufferMem[0].u= verts[vert1].u;
        bufferMem[0].v= verts[vert1].v;

        bufferMem[1].x = verts[vert2].x + offsetX;
        bufferMem[1].y = verts[vert2].y + offsetY;
        bufferMem[1].z= verts[vert2].z;
        bufferMem[1].rhw= verts[vert2].rhw;
        bufferMem[1].emissiveColor= verts[vert2].emissiveColor;
        bufferMem[1].u= verts[vert2].u;
        bufferMem[1].v= verts[vert2].v;

        IDirect3DVertexBuffer8_Unlock(CurrentVertexBuffer);

        rxD3D8Im2DRenderFlush();

        hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                            D3DPT_LINELIST,
                                            CurrentBaseIndex,
                                            1));

        #if defined( RWMETRICS )
        RWSRCGLOBAL(metrics)->numVertices += 2;
        #endif /* defined( RWMETRICS ) */

        CurrentBaseIndex += 2;

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D8Im2DRenderTriangle

 On entry   : vertices, num vertices in array
            : index of triangle vertices
 On exit    :
 */
RwBool
_rwD3D8Im2DRenderTriangle(RwIm2DVertex *verts,
                        RwInt32 unused2 __RWUNUSED__,
                        RwInt32 vert1,
                        RwInt32 vert2,
                        RwInt32 vert3)
{
    const RwRaster  *raster;
    RwIm2DVertex    *bufferMem = NULL;

    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderTriangle"));
    RWASSERT(verts);
    RWASSERT(dgGGlobals.curCamera);

    raster = RwCameraGetRaster(dgGGlobals.curCamera);
    RWASSERT(raster);

    /* Fill Vertex Buffer */
    if (UseOwnVertexBuffer)
    {
        if ((CurrentBaseIndex + 3) > BUFFER_MAX_VERTEX_INDEX)
        {
            CurrentBaseIndex = 0;

            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        0,
                                        3 * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
        }
        else
        {
            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        CurrentBaseIndex * sizeof(RwIm2DVertex),
                                        3 * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
        }
    }
    else
    {
        RwD3D8DynamicVertexBufferLock(sizeof(RwIm2DVertex),
                                      3,
                                      (void**)&CurrentVertexBuffer,
                                      (void**)&bufferMem,
                                      &CurrentBaseIndex);
    }

    if (bufferMem != NULL)
    {
        const RwReal offsetX = (RwReal)(raster->nOffsetX);
        const RwReal offsetY = (RwReal)(raster->nOffsetY);
        HRESULT hr;

        bufferMem[0].x = verts[vert1].x + offsetX;
        bufferMem[0].y = verts[vert1].y + offsetY;
        bufferMem[0].z= verts[vert1].z;
        bufferMem[0].rhw= verts[vert1].rhw;
        bufferMem[0].emissiveColor= verts[vert1].emissiveColor;
        bufferMem[0].u= verts[vert1].u;
        bufferMem[0].v= verts[vert1].v;

        bufferMem[1].x = verts[vert2].x + offsetX;
        bufferMem[1].y = verts[vert2].y + offsetY;
        bufferMem[1].z= verts[vert2].z;
        bufferMem[1].rhw= verts[vert2].rhw;
        bufferMem[1].emissiveColor= verts[vert2].emissiveColor;
        bufferMem[1].u= verts[vert2].u;
        bufferMem[1].v= verts[vert2].v;

        bufferMem[2].x = verts[vert3].x + offsetX;
        bufferMem[2].y = verts[vert3].y + offsetY;
        bufferMem[2].z= verts[vert3].z;
        bufferMem[2].rhw= verts[vert3].rhw;
        bufferMem[2].emissiveColor= verts[vert3].emissiveColor;
        bufferMem[2].u= verts[vert3].u;
        bufferMem[2].v= verts[vert3].v;

        IDirect3DVertexBuffer8_Unlock(CurrentVertexBuffer);

        rxD3D8Im2DRenderFlush();

        hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                            D3DPT_TRIANGLELIST,
                                            CurrentBaseIndex,
                                            1));

#if defined( RWMETRICS )
        RWSRCGLOBAL(metrics)->numVertices += 3;       
        RWSRCGLOBAL(metrics)->numTriangles += 1;
        RWSRCGLOBAL(metrics)->numProcTriangles += 1;
#endif /* defined( RWMETRICS ) */

        CurrentBaseIndex += 3;

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D8Im2DRenderPrimitive

 On entry   : Primitive type
            : Vertices
            : Num Vertices
 On exit    :
 */
RwBool
_rwD3D8Im2DRenderPrimitive(RwPrimitiveType primType,
                         RwIm2DVertex *verts,
                         RwInt32 numVerts)
{
    const RwRaster  *raster;
    RwInt32         numPrimitives;
    RwIm2DVertex    *bufferMem = NULL;

    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderPrimitive"));
    RWASSERT(verts);

    if (dgGGlobals.curCamera)
    {
        raster = RwCameraGetRaster(dgGGlobals.curCamera);
    }
    else
    {
        /*
         * This is only supported because RwRasterRender uses this function.
         */
        raster = RwRasterGetCurrentContext();
    }
    RWASSERT(raster);

    switch (primType)
    {
        case rwPRIMTYPELINELIST:
        {
            RWASSERT(numVerts > 1);
            numPrimitives = numVerts / 2;
            break;
        }

        case rwPRIMTYPEPOLYLINE:
        {
            RWASSERT(numVerts > 1);
            numPrimitives = numVerts - 1;
            break;
        }

        case rwPRIMTYPETRILIST:
        {
            RWASSERT(numVerts > 2);
            numPrimitives = numVerts / 3;
            break;
        }

        case rwPRIMTYPETRISTRIP:
        {
            RWASSERT(numVerts > 2);
            numPrimitives = numVerts - 2;
            break;
        }

        case rwPRIMTYPETRIFAN:
        {
            RWASSERT(numVerts > 2);
            numPrimitives = numVerts - 2;
            break;
        }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    /* Get a vertex buffer memory */
    if (UseOwnVertexBuffer)
    {
        if ((CurrentBaseIndex + numVerts) > BUFFER_MAX_VERTEX_INDEX)
        {
            CurrentBaseIndex = 0;

            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        0,
                                        numVerts * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
        }
        else
        {
            IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                        CurrentBaseIndex * sizeof(RwIm2DVertex),
                                        numVerts * sizeof(RwIm2DVertex),
                                        (RwUInt8 **)&bufferMem,
                                        D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
        }
    }
    else
    {
        RwD3D8DynamicVertexBufferLock(sizeof(RwIm2DVertex),
                                      numVerts,
                                      (void**)&CurrentVertexBuffer,
                                      (void**)&bufferMem,
                                      &CurrentBaseIndex);
    }

    /* Fill Vertex Buffer */
    if (bufferMem != NULL)
    {
        HRESULT hr;

        if (raster->nOffsetX || raster->nOffsetY)
        {
            const RwReal offsetX = (RwReal)(raster->nOffsetX);
            const RwReal offsetY = (RwReal)(raster->nOffsetY);
            RwInt32 i;

            for (i = 0; i < numVerts; i++)
            {
                bufferMem[i].x = verts[i].x + offsetX;
                bufferMem[i].y = verts[i].y + offsetY;
                bufferMem[i].z= verts[i].z;
                bufferMem[i].rhw= verts[i].rhw;
                bufferMem[i].emissiveColor= verts[i].emissiveColor;
                bufferMem[i].u= verts[i].u;
                bufferMem[i].v= verts[i].v;
            }
        }
        else
        {
            memcpy(bufferMem, verts, numVerts * sizeof(RwIm2DVertex));
        }

        IDirect3DVertexBuffer8_Unlock(CurrentVertexBuffer);

        rxD3D8Im2DRenderFlush();

        hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                            _RwD3D8PrimConvTable[primType],
                                            CurrentBaseIndex,
                                            numPrimitives));

#if defined( RWMETRICS )
        RWSRCGLOBAL(metrics)->numVertices += numVerts;
        
        switch ( primType )
        {               
        case rwPRIMTYPETRILIST:
        case rwPRIMTYPETRISTRIP:
        case rwPRIMTYPETRIFAN:
            {
                RWSRCGLOBAL(metrics)->numTriangles += numPrimitives;
                RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
            }
            break;
        }
#endif /* defined( RWMETRICS ) */

        CurrentBaseIndex += numVerts;

        RWRETURN(SUCCEEDED(hr));
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwD3D8Im2DRenderIndexedPrimitive

 On entry   : Primitive type
            : Vertices
            : Num Vertices
 On exit    :
 */
RwBool
_rwD3D8Im2DRenderIndexedPrimitive(RwPrimitiveType primType,
                                RwIm2DVertex *verts,
                                RwInt32 numVerts,
                                RwImVertexIndex *indices,
                                RwInt32 numIndices)
{
    const RwRaster  *raster;
    RwInt32         numPrimitives;
    RwIm2DVertex    *bufferMem = NULL;

    RWFUNCTION(RWSTRING("_rwD3D8Im2DRenderIndexedPrimitive"));
    RWASSERT(verts);
    RWASSERT(indices);
    RWASSERT(dgGGlobals.curCamera);

    raster = RwCameraGetRaster(dgGGlobals.curCamera);
    RWASSERT(raster);

    switch (primType)
    {
        case rwPRIMTYPELINELIST:
        {
            RWASSERT(numIndices > 1);
            numPrimitives = numIndices / 2;
            break;
        }

        case rwPRIMTYPEPOLYLINE:
        {
            RWASSERT(numIndices > 1);
            numPrimitives = numIndices - 1;
            break;
        }

        case rwPRIMTYPETRILIST:
        {
            RWASSERT(numIndices > 2);
            numPrimitives = numIndices / 3;
            break;
        }

        case rwPRIMTYPETRISTRIP:
        {
            RWASSERT(numIndices > 2);
            numPrimitives = numIndices - 2;
            break;
        }

        case rwPRIMTYPETRIFAN:
        {
            RWASSERT(numIndices > 2);
            numPrimitives = numIndices - 2;
            break;
        }

        default:
            RWASSERT(FALSE);
            numPrimitives = 0;
            break;
    }

    if (numIndices<=numVerts)
    {
        /* numIndices is used instead of numVerts */
        if (UseOwnVertexBuffer)
        {
            if ((CurrentBaseIndex + numIndices) > BUFFER_MAX_VERTEX_INDEX)
            {
                CurrentBaseIndex = 0;

                IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                            0,
                                            numIndices * sizeof(RwIm2DVertex),
                                            (RwUInt8 **)&bufferMem,
                                            D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
            }
            else
            {
                IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                            CurrentBaseIndex * sizeof(RwIm2DVertex),
                                            numIndices * sizeof(RwIm2DVertex),
                                            (RwUInt8 **)&bufferMem,
                                            D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
            }
        }
        else
        {
            RwD3D8DynamicVertexBufferLock(sizeof(RwIm2DVertex),
                                            numIndices,
                                            (void**)&CurrentVertexBuffer,
                                            (void**)&bufferMem,
                                            &CurrentBaseIndex);
        }

        /* Fill Vertex Buffer */
        if (bufferMem != NULL)
        {
            RwInt32 i;
            HRESULT hr;

            if (raster->nOffsetX || raster->nOffsetY)
            {
                const RwReal offsetX = (RwReal)(raster->nOffsetX);
                const RwReal offsetY = (RwReal)(raster->nOffsetY);

                for(i = 0; i < numIndices; i++)
                {
                    const RwIm2DVertex *currentVert = &(verts[indices[i]]);

                    bufferMem[i].x = currentVert->x + offsetX;
                    bufferMem[i].y = currentVert->y + offsetY;
                    bufferMem[i].z= currentVert->z;
                    bufferMem[i].rhw= currentVert->rhw;
                    bufferMem[i].emissiveColor= currentVert->emissiveColor;
                    bufferMem[i].u= currentVert->u;
                    bufferMem[i].v= currentVert->v;
                }
            }
            else
            {
                for(i = 0; i < numIndices; i++)
                {
                    const RwIm2DVertex *currentVert = &(verts[indices[i]]);

                    memcpy(bufferMem + i, currentVert, sizeof(RwIm2DVertex));
                }
            }

            IDirect3DVertexBuffer8_Unlock(CurrentVertexBuffer);

            rxD3D8Im2DRenderFlush();

            hr = DXCHECK(IDirect3DDevice8_DrawPrimitive(_RwD3DDevice,
                                                _RwD3D8PrimConvTable[primType],
                                                CurrentBaseIndex,
                                                numPrimitives));

            #if defined( RWMETRICS )
            RWSRCGLOBAL(metrics)->numVertices += numIndices;
        
            switch ( primType )
            {               
            case rwPRIMTYPETRILIST:
            case rwPRIMTYPETRISTRIP:
            case rwPRIMTYPETRIFAN:
                {
                    RWSRCGLOBAL(metrics)->numTriangles += numPrimitives;
                    RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                }
                break;
            }
            #endif /* defined( RWMETRICS ) */

            CurrentBaseIndex += numIndices;

            RWRETURN(SUCCEEDED(hr));
        }
    }
    else
    {
        /* Fill Vertex Buffer */
        if (UseOwnVertexBuffer)
        {
            if ((CurrentBaseIndex + numVerts) > BUFFER_MAX_VERTEX_INDEX)
            {
                CurrentBaseIndex = 0;

                IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                            0,
                                            numVerts * sizeof(RwIm2DVertex),
                                            (RwUInt8 **)&bufferMem,
                                            D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
            }
            else
            {
                IDirect3DVertexBuffer8_Lock(CurrentVertexBuffer,
                                            CurrentBaseIndex * sizeof(RwIm2DVertex),
                                            numVerts * sizeof(RwIm2DVertex),
                                            (RwUInt8 **)&bufferMem,
                                            D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
            }
        }
        else
        {
            RwD3D8DynamicVertexBufferLock(sizeof(RwIm2DVertex),
                                            numVerts,
                                            (void**)&CurrentVertexBuffer,
                                            (void**)&bufferMem,
                                            &CurrentBaseIndex);
        }

        if (bufferMem != NULL)
        {
            if (raster->nOffsetX || raster->nOffsetY)
            {
                const RwReal offsetX = (RwReal)(raster->nOffsetX);
                const RwReal offsetY = (RwReal)(raster->nOffsetY);
                RwInt32 i;

                for (i = 0; i < numVerts; i++)
                {
                    bufferMem[i].x = verts[i].x + offsetX;
                    bufferMem[i].y = verts[i].y + offsetY;
                    bufferMem[i].z= verts[i].z;
                    bufferMem[i].rhw= verts[i].rhw;
                    bufferMem[i].emissiveColor= verts[i].emissiveColor;
                    bufferMem[i].u= verts[i].u;
                    bufferMem[i].v= verts[i].v;
                }
            }
            else
            {
                memcpy(bufferMem, verts, numVerts * sizeof(RwIm2DVertex));
            }

            IDirect3DVertexBuffer8_Unlock(CurrentVertexBuffer);

            /* Fill the Index Buffer */
            if (IndexBuffer2D && (numIndices <= BUFFER_MAX_INDEX))
            {
                HRESULT hr;

                /* Blank the IB every time for now */
                if ((IB2DOffset + numIndices) > BUFFER_MAX_INDEX)
                {
                    IB2DOffset = 0;

                    hr = IDirect3DIndexBuffer8_Lock(IndexBuffer2D,
                                                     0,
                                                     numIndices * sizeof(RwUInt16),
                                                     (RwUInt8 **)&bufferMem,
                                                     D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK);
                }
                else
                {
                    hr = IDirect3DIndexBuffer8_Lock(IndexBuffer2D,
                                                     IB2DOffset * sizeof(RwUInt16),
                                                     numIndices * sizeof(RwUInt16),
                                                     (RwUInt8 **)&bufferMem,
                                                     D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK);
                }

                if(SUCCEEDED(hr))
                {
                    memcpy(bufferMem, indices, numIndices * sizeof(RwUInt16));

                    IDirect3DIndexBuffer8_Unlock(IndexBuffer2D);

                    rxD3D8Im2DRenderFlush();

                    RwD3D8SetIndices(IndexBuffer2D, CurrentBaseIndex);

                    hr = DXCHECK(IDirect3DDevice8_DrawIndexedPrimitive(_RwD3DDevice,
                                                _RwD3D8PrimConvTable[primType],
                                                0, numVerts,
                                                IB2DOffset, numPrimitives));

                    #if defined( RWMETRICS )
                    RWSRCGLOBAL(metrics)->numVertices += numVerts;
        
                    switch ( primType )
                    {               
                    case rwPRIMTYPETRILIST:
                    case rwPRIMTYPETRISTRIP:
                    case rwPRIMTYPETRIFAN:
                        {
                            RWSRCGLOBAL(metrics)->numTriangles += numPrimitives;
                            RWSRCGLOBAL(metrics)->numProcTriangles += numPrimitives;
                        }
                        break;
                    }
                    #endif /* defined( RWMETRICS ) */

                    IB2DOffset += numIndices;
                }

                CurrentBaseIndex += numVerts;

                RWRETURN(SUCCEEDED(hr));
            }
        }
    }

    RWRETURN(FALSE);
}
