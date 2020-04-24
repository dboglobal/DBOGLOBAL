
#include <d3d9.h>

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toon.h"
#include "edgerender.h"

#include "toonvertexshader.h"
#include "toonvertexshaderflat.h"

#define RPTOON_MAX_VERTICES_BATCH    10000

typedef struct
{
    RwV3d pos;
    RwUInt32 color;
} EdgeVertex;

typedef struct
{
    RwV3d pos;
} EdgeVertexFlat;

typedef struct
{
    RwV3d pos;
    RwV3d normal;
    RwUInt32 color;
    RwV2d inkcoef;
} EdgeVertexShader;

typedef struct
{
    RwV3d pos;
    RwV3d normal;
    RwReal thickness;
} EdgeVertexShaderFlat;


static LPDIRECT3DINDEXBUFFER9 IndexBuffer = NULL;
static LPDIRECT3DVERTEXBUFFER9 VertexBuffer = NULL;
static RwUInt32 Offset = 0;
static RwUInt32 NumVertices = 0;
static RwUInt32 MaxVertices = 0;
static RwUInt32 MaxBatch = 0;

static EdgeVertex *VertexData = NULL;
static EdgeVertexFlat *VertexFlatData = NULL;
static EdgeVertexShader *VertexShaderData = NULL;
static EdgeVertexShaderFlat *VertexShaderFlatData = NULL;

static LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration = NULL;
static LPDIRECT3DVERTEXDECLARATION9 VertexDeclarationFlat = NULL;

static LPDIRECT3DVERTEXDECLARATION9 VertexShaderDeclaration = NULL;
static LPDIRECT3DVERTEXSHADER9 VertexShader = NULL;

static LPDIRECT3DVERTEXDECLARATION9 VertexShaderDeclarationFlat = NULL;
static LPDIRECT3DVERTEXSHADER9 VertexShaderFlat = NULL;

static RwUInt32 OldShadeMode;
static RwUInt32 OldDither;

/****************************************************************************
 _rpToonEdgeRenderInit
 */
void
_rpToonEdgeRenderInit(void)
{
    const D3DCAPS9      *d3dCaps;
    D3DVERTEXELEMENT9   declaration[20];
    RwUInt32            declarationIndex, stride;
    RwUInt16 *indices;
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpToonEdgeRenderInit"));

    /* Create index buffer */
    RwD3D9IndexBufferCreate(((RPTOON_MAX_VERTICES_BATCH / 4) * 6),
                            &IndexBuffer);
    RWASSERT(IndexBuffer != NULL);

    IDirect3DIndexBuffer9_Lock(IndexBuffer, 0, 0,
                               (RwUInt8 **)&indices,
                               D3DLOCK_NOSYSLOCK);


    for (i = 0; i < (RPTOON_MAX_VERTICES_BATCH / 4); i++)
    {
        indices[0] = i * 4 + 0;
        indices[1] = i * 4 + 1;
        indices[2] = i * 4 + 2;

        indices[3] = i * 4 + 2;
        indices[4] = i * 4 + 1;
        indices[5] = i * 4 + 3;

        indices += 6;
    }

    IDirect3DIndexBuffer9_Unlock(IndexBuffer);

    /* Create vertex declarations */
    declarationIndex = 0;

    declaration[declarationIndex].Stream = 0;
    declaration[declarationIndex].Offset = 0;
    declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
    declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
    declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
    declaration[declarationIndex].UsageIndex = 0;
    declarationIndex++;
    stride = sizeof(RwV3d);

    declaration[declarationIndex].Stream = 0;
    declaration[declarationIndex].Offset = stride;
    declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
    declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
    declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
    declaration[declarationIndex].UsageIndex = 0;
    declarationIndex++;
    stride += sizeof(RwRGBA);

    declaration[declarationIndex].Stream = 0xFF;
    declaration[declarationIndex].Offset = 0;
    declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
    declaration[declarationIndex].Method = 0;
    declaration[declarationIndex].Usage = 0;
    declaration[declarationIndex].UsageIndex = 0;

    RWASSERT(sizeof(EdgeVertex) == stride);

    RwD3D9CreateVertexDeclaration(declaration,
                                  &VertexDeclaration);

    /* Flat */
    declarationIndex = 0;

    declaration[declarationIndex].Stream = 0;
    declaration[declarationIndex].Offset = 0;
    declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
    declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
    declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
    declaration[declarationIndex].UsageIndex = 0;
    declarationIndex++;
    stride = sizeof(RwV3d);

    declaration[declarationIndex].Stream = 0xFF;
    declaration[declarationIndex].Offset = 0;
    declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
    declaration[declarationIndex].Method = 0;
    declaration[declarationIndex].Usage = 0;
    declaration[declarationIndex].UsageIndex = 0;

    RWASSERT(sizeof(EdgeVertexFlat) == stride);

    RwD3D9CreateVertexDeclaration(declaration,
                                  &VertexDeclarationFlat);

    /* Vertex shaders */
    d3dCaps = (const D3DCAPS9 *)RwD3D9GetCaps();

    if ((d3dCaps->VertexShaderVersion & 0xFFFF) >= 0x0101 &&
        (d3dCaps->PixelShaderVersion & 0xFFFF) >= 0x0101)
    {
        declarationIndex = 0;

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride = sizeof(RwV3d);

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_NORMAL;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride += sizeof(RwV3d);

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_D3DCOLOR;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_COLOR;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride += sizeof(RwRGBA);

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT2;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride += sizeof(RwV2d);

        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RWASSERT(sizeof(EdgeVertexShader) == stride);

        RwD3D9CreateVertexDeclaration(declaration,
                                    &VertexShaderDeclaration);

        RwD3D9CreateVertexShader(dwToonVertexShader, &VertexShader);

        /* Flat version */
        declarationIndex = 0;

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_POSITION;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride = sizeof(RwV3d);

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT3;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_NORMAL;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride += sizeof(RwV3d);

        declaration[declarationIndex].Stream = 0;
        declaration[declarationIndex].Offset = stride;
        declaration[declarationIndex].Type = D3DDECLTYPE_FLOAT1;
        declaration[declarationIndex].Method = D3DDECLMETHOD_DEFAULT;
        declaration[declarationIndex].Usage = D3DDECLUSAGE_TEXCOORD;
        declaration[declarationIndex].UsageIndex = 0;
        declarationIndex++;
        stride += sizeof(RwReal);

        declaration[declarationIndex].Stream = 0xFF;
        declaration[declarationIndex].Offset = 0;
        declaration[declarationIndex].Type = D3DDECLTYPE_UNUSED;
        declaration[declarationIndex].Method = 0;
        declaration[declarationIndex].Usage = 0;
        declaration[declarationIndex].UsageIndex = 0;

        RWASSERT(sizeof(EdgeVertexShaderFlat) == stride);

        RwD3D9CreateVertexDeclaration(declaration,
                                    &VertexShaderDeclarationFlat);

        RwD3D9CreateVertexShader(dwToonVertexShaderFlat, &VertexShaderFlat);
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderClean
 */
void
_rpToonEdgeRenderClean(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderClean"));

    if (IndexBuffer)
    {
        IDirect3DIndexBuffer9_Release(IndexBuffer);
        IndexBuffer = NULL;
    }

    if (VertexShaderFlat != NULL)
    {
        RwD3D9DeleteVertexShader(VertexShaderFlat);

        VertexShaderFlat = NULL;
    }

    if (VertexShaderDeclarationFlat != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexShaderDeclarationFlat);

        VertexShaderDeclarationFlat = NULL;
    }

    if (VertexShader != NULL)
    {
        RwD3D9DeleteVertexShader(VertexShader);

        VertexShader = NULL;
    }

    if (VertexShaderDeclaration != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexShaderDeclaration);

        VertexShaderDeclaration = NULL;
    }

    if (VertexDeclarationFlat != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexDeclarationFlat);

        VertexDeclarationFlat = NULL;
    }

    if (VertexDeclaration != NULL)
    {
        RwD3D9DeleteVertexDeclaration(VertexDeclaration);

        VertexDeclaration = NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPushState
 */
void
_rpToonEdgeRenderPushState(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPushState"));

    RwD3D9GetRenderState(D3DRS_SHADEMODE, &OldShadeMode);
    RwD3D9GetRenderState(D3DRS_DITHERENABLE, &OldDither);

    RwD3D9SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, FALSE);

    RwD3D9SetTexture(NULL, 0);

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPopState
 */
void
_rpToonEdgeRenderPopState(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPopState"));

    RwD3D9SetRenderState(D3DRS_SHADEMODE, OldShadeMode);
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, OldDither);

    RWRETURNVOID();
}

/****************************************************************************
 GetScreenSpaceProjection
 */
void
GetScreenSpaceProjection(ScreenSpaceProjection *projection,
                         const RwMatrix *transform)
{
    RwCamera *camera;
    RwFrame *cameraFrame;

    RWFUNCTION(RWSTRING("GetScreenSpaceProjection"));

    camera = RwCameraGetCurrentCamera();
    RWASSERT(0 != camera);

    cameraFrame = RwCameraGetFrame(camera);
    RWASSERT(0 != cameraFrame);

    if (transform != NULL)
    {
        RwMatrix invCamLTM;

        RwMatrixInvert( &invCamLTM, RwFrameGetLTM(cameraFrame));

        RwMatrixMultiply( &projection->matrix, transform, &invCamLTM );
    }
    else
    {
        RwMatrixInvert( &projection->matrix, RwFrameGetLTM(cameraFrame));
    }

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPrepareSubmit
 */
void
_rpToonEdgeRenderPrepareSubmit(RwUInt32 maxNumEdges)
{
    RwCamera *camera;
    RwFrame *cameraFrame;

    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPrepareSubmit"));

    camera = RwCameraGetCurrentCamera();
    RWASSERT(0 != camera);

    cameraFrame = RwCameraGetFrame(camera);
    RWASSERT(0 != cameraFrame);

    /* World matrix */
    RwD3D9SetTransformWorld(RwFrameGetLTM(cameraFrame));

    /* Disable vertex alpha blending */
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

    /* Disable lighting */
    RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    /* Set color ops */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Set the vertex Declaration */
    RwD3D9SetVertexDeclaration(VertexDeclaration);

    /* Disable any vertex shader */
    RwD3D9SetVertexShader(NULL);

    /* Disable any pixel shader */
    RwD3D9SetPixelShader(NULL);

    RwD3D9SetIndices(IndexBuffer);

    NumVertices = 0;

    MaxVertices = maxNumEdges * 4;

    if (MaxVertices > RPTOON_MAX_VERTICES_BATCH)
    {
        MaxBatch = RPTOON_MAX_VERTICES_BATCH;
    }
    else if (MaxVertices > 1000)
    {
        MaxBatch = ((maxNumEdges / 4) * 4); /* We need a multiple of for */
    }

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertex),
                                      MaxBatch,
                                      (void**)&VertexBuffer,
                                      (RwUInt8 **)&VertexData,
                                      &Offset);
    }

    RWRETURNVOID();
}

void
_rpToonEdgeRenderSubmit(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderSubmit"));

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferUnlock(VertexBuffer);
    }

    if (NumVertices == 0)
    {
        /* early out to avoid crashing driver */
        RWRETURNVOID();
    }

    RWASSERT(NumVertices <= MaxBatch);

    RwD3D9SetStreamSource(0, VertexBuffer, 0, sizeof(EdgeVertex));

    RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                               Offset,
                               0, NumVertices,
                               0, (NumVertices / 2));


    RWRETURNVOID();
}

void
_rpToonEdgeRenderAppendQuad(const RwV3d *vIn0,
                            const RwV3d *vIn1,
                            const RwV2d *vOut0,
                            const RwV2d *vOut1,
                            RwRGBA color)
{
    RwUInt32 d3dcolor;

    RWFUNCTION( RWSTRING( "_rpToonEdgeRenderAppendQuad" ) );

    RWASSERT(NumVertices <= MaxVertices);

    if (NumVertices >= MaxBatch)
    {
        _rpToonEdgeRenderSubmit();

        NumVertices = 0;

        MaxVertices -= MaxBatch;

        MaxBatch = min(MaxVertices, RPTOON_MAX_VERTICES_BATCH);
        RWASSERT(MaxBatch > 0);

        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertex),
                                    MaxBatch,
                                    (void**)&VertexBuffer,
                                    (RwUInt8 **)&VertexData,
                                    &Offset);
    }

    d3dcolor = (((RwUInt32)color.alpha) << 24) |
                (((RwUInt32)color.red) << 16) |
                (((RwUInt32)color.green) << 8) |
                color.blue;

    VertexData->pos.x = vIn0->x;
    VertexData->pos.y = vIn0->y;
    VertexData->pos.z = vIn0->z;
    VertexData->color = d3dcolor;
    VertexData++;

    VertexData->pos.x = vIn1->x;
    VertexData->pos.y = vIn1->y;
    VertexData->pos.z = vIn1->z;
    VertexData->color = d3dcolor;
    VertexData++;

    VertexData->pos.x = vOut0->x;
    VertexData->pos.y = vOut0->y;
    VertexData->pos.z = vIn0->z;
    VertexData->color = d3dcolor;
    VertexData++;

    VertexData->pos.x = vOut1->x;
    VertexData->pos.y = vOut1->y;
    VertexData->pos.z = vIn1->z;
    VertexData->color = d3dcolor;
    VertexData++;

    NumVertices += 4;

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPrepareSubmit
 */
void
_rpToonEdgeRenderPrepareFlatSubmit(RwUInt32 maxNumEdges,
                                   RwRGBA color)
{
    RwCamera *camera;
    RwFrame *cameraFrame;
    RwUInt32 inkColor;

    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPrepareSubmit"));

    camera = RwCameraGetCurrentCamera();
    RWASSERT(0 != camera);

    cameraFrame = RwCameraGetFrame(camera);
    RWASSERT(0 != cameraFrame);

    /* World matrix */
    RwD3D9SetTransformWorld(RwFrameGetLTM(cameraFrame));

    /* Disable vertex alpha blending */
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

    /* Disable lighting */
    RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    inkColor = (((RwUInt32)color.alpha) << 24) |
                (((RwUInt32)color.red) << 16) |
                (((RwUInt32)color.green) << 8) |
                color.blue;

    RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, inkColor);

    /* Set color ops */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    /* Set the vertex Declaration */
    RwD3D9SetVertexDeclaration(VertexDeclarationFlat);

    /* Disable any vertex shader */
    RwD3D9SetVertexShader(NULL);

    /* Disable any pixel shader */
    RwD3D9SetPixelShader(NULL);

    RwD3D9SetIndices(IndexBuffer);

    NumVertices = 0;

    MaxVertices = maxNumEdges * 4;

    if (MaxVertices > RPTOON_MAX_VERTICES_BATCH)
    {
        MaxBatch = RPTOON_MAX_VERTICES_BATCH;
    }
    else if (MaxVertices > 1000)
    {
        MaxBatch = ((maxNumEdges / 4) * 4); /* We need a multiple of for */
    }

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexFlat),
                                      MaxBatch,
                                      (void**)&VertexBuffer,
                                      (RwUInt8 **)&VertexFlatData,
                                      &Offset);
    }

    RWRETURNVOID();
}

void
_rpToonEdgeRenderFlatSubmit(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderFlatSubmit"));

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferUnlock(VertexBuffer);
    }

    if (NumVertices == 0)
    {
        /* early out to avoid crashing driver */
        RWRETURNVOID();
    }

    RWASSERT(NumVertices <= MaxBatch);

    RwD3D9SetStreamSource(0, VertexBuffer, 0, sizeof(EdgeVertexFlat));

    RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                               Offset,
                               0, NumVertices,
                               0, (NumVertices / 2));


    RWRETURNVOID();
}

void
_rpToonEdgeRenderAppendFlatQuad(const RwV3d *vIn0,
                                const RwV3d *vIn1,
                                const RwV2d *vOut0,
                                const RwV2d *vOut1)
{
    RWFUNCTION( RWSTRING( "_rpToonEdgeRenderAppendFlatQuad" ) );

    RWASSERT(NumVertices <= MaxVertices);

    if (NumVertices >= MaxBatch)
    {
        _rpToonEdgeRenderFlatSubmit();

        NumVertices = 0;

        MaxVertices -= MaxBatch;

        MaxBatch = min(MaxVertices, RPTOON_MAX_VERTICES_BATCH);
        RWASSERT(MaxBatch > 0);

        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexFlat),
                                    MaxBatch,
                                    (void**)&VertexBuffer,
                                    (RwUInt8 **)&VertexFlatData,
                                    &Offset);
    }

    VertexFlatData->pos.x = vIn0->x;
    VertexFlatData->pos.y = vIn0->y;
    VertexFlatData->pos.z = vIn0->z;
    VertexFlatData++;

    VertexFlatData->pos.x = vIn1->x;
    VertexFlatData->pos.y = vIn1->y;
    VertexFlatData->pos.z = vIn1->z;
    VertexFlatData++;

    VertexFlatData->pos.x = vOut0->x;
    VertexFlatData->pos.y = vOut0->y;
    VertexFlatData->pos.z = vIn0->z;
    VertexFlatData++;

    VertexFlatData->pos.x = vOut1->x;
    VertexFlatData->pos.y = vOut1->y;
    VertexFlatData->pos.z = vIn1->z;
    VertexFlatData++;

    NumVertices += 4;

    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPrepareVertexShaderSubmit
 */
void
_rpToonEdgeRenderPrepareVertexShaderSubmit(RwUInt32 maxNumEdges)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPrepareVertexShaderSubmit"));

    /* Disable vertex alpha blending */
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

    /* Enable clipping */
    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    /* Set color ops */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Set the vertex Declaration */
    RwD3D9SetVertexDeclaration(VertexShaderDeclaration);

    /* Disable any vertex shader */
    RwD3D9SetVertexShader(VertexShader);

    /* Disable any pixel shader */
    RwD3D9SetPixelShader(NULL);

    RwD3D9SetIndices(IndexBuffer);

    NumVertices = 0;

    MaxVertices = maxNumEdges * 4;

    if (MaxVertices > RPTOON_MAX_VERTICES_BATCH)
    {
        MaxBatch = RPTOON_MAX_VERTICES_BATCH;
    }
    else if (MaxVertices > 1000)
    {
        MaxBatch = ((maxNumEdges / 4) * 4); /* We need a multiple of for */
    }

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexShader),
                                      MaxBatch,
                                      (void**)&VertexBuffer,
                                      (RwUInt8 **)&VertexShaderData,
                                      &Offset);
    }

    RWRETURNVOID();
}

void
_rpToonEdgeRenderAppendVertexShaderQuad(const RwV3d *vPos0,
                                        const RwV3d *vPos1,
                                        const RwV3d *vNormal0,
                                        const RwV3d *vNormal1,
                                        RwReal vertexThicknesses0,
                                        RwReal vertexThicknesses1,
                                        const RpToonInk *ink)
{
    RwUInt32 d3dcolor;

    RWFUNCTION( RWSTRING( "_rpToonEdgeRenderAppendVertexShaderQuad" ) );

    RWASSERT(NumVertices <= MaxVertices);

    if (NumVertices >= MaxBatch)
    {
        _rpToonEdgeRenderVertexShaderSubmit();

        NumVertices = 0;

        MaxVertices -= MaxBatch;

        MaxBatch = min(MaxVertices, RPTOON_MAX_VERTICES_BATCH);
        RWASSERT(MaxBatch > 0);

        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexShader),
                                    MaxBatch,
                                    (void**)&VertexBuffer,
                                    (RwUInt8 **)&VertexShaderData,
                                    &Offset);
    }

    d3dcolor = (((RwUInt32)ink->color.alpha) << 24) |
                (((RwUInt32)ink->color.red) << 16) |
                (((RwUInt32)ink->color.green) << 8) |
                ink->color.blue;

    VertexShaderData->pos.x = vPos0->x;
    VertexShaderData->pos.y = vPos0->y;
    VertexShaderData->pos.z = vPos0->z;
    VertexShaderData->normal.x = vNormal0->x;
    VertexShaderData->normal.y = vNormal0->y;
    VertexShaderData->normal.z = vNormal0->z;
    VertexShaderData->color = d3dcolor;
    VertexShaderData->inkcoef.x = 0.0f;
    VertexShaderData->inkcoef.y = 0.0f;
    VertexShaderData++;

    VertexShaderData->pos.x = vPos1->x;
    VertexShaderData->pos.y = vPos1->y;
    VertexShaderData->pos.z = vPos1->z;
    VertexShaderData->normal.x = vNormal1->x;
    VertexShaderData->normal.y = vNormal1->y;
    VertexShaderData->normal.z = vNormal1->z;
    VertexShaderData->color = d3dcolor;
    VertexShaderData->inkcoef.x = 0.0f;
    VertexShaderData->inkcoef.y = 0.0f;
    VertexShaderData++;

    if (ink->perspectiveScale_b != 0.0f)
    {
        RwReal auxCoef1, auxCoef2;

        auxCoef1 = (ink->farScale_factor) * (ink->perspectiveScale_b);
        auxCoef2 = (ink->farScale_factor) * ((ink->perspectiveScale_a) - (ink->perspectiveScale_b) * (ink->perspectiveScale_zMin));

        VertexShaderData->pos.x = vPos0->x;
        VertexShaderData->pos.y = vPos0->y;
        VertexShaderData->pos.z = vPos0->z;
        VertexShaderData->normal.x = vNormal0->x;
        VertexShaderData->normal.y = vNormal0->y;
        VertexShaderData->normal.z = vNormal0->z;
        VertexShaderData->color = d3dcolor;
        VertexShaderData->inkcoef.x = vertexThicknesses0 * auxCoef1;
        VertexShaderData->inkcoef.y =  vertexThicknesses0 * auxCoef2;
        VertexShaderData++;

        VertexShaderData->pos.x = vPos1->x;
        VertexShaderData->pos.y = vPos1->y;
        VertexShaderData->pos.z = vPos1->z;
        VertexShaderData->normal.x = vNormal1->x;
        VertexShaderData->normal.y = vNormal1->y;
        VertexShaderData->normal.z = vNormal1->z;
        VertexShaderData->color = d3dcolor;
        VertexShaderData->inkcoef.x = vertexThicknesses1 * auxCoef1;
        VertexShaderData->inkcoef.y =  vertexThicknesses1 * auxCoef2;
        VertexShaderData++;
    }
    else
    {
        RwReal auxCoef;

        auxCoef = (ink->farScale_factor) * (ink->perspectiveScale_a);

        VertexShaderData->pos.x = vPos0->x;
        VertexShaderData->pos.y = vPos0->y;
        VertexShaderData->pos.z = vPos0->z;
        VertexShaderData->normal.x = vNormal0->x;
        VertexShaderData->normal.y = vNormal0->y;
        VertexShaderData->normal.z = vNormal0->z;
        VertexShaderData->color = d3dcolor;
        VertexShaderData->inkcoef.x = 0.0f;
        VertexShaderData->inkcoef.y = vertexThicknesses0 * auxCoef;
        VertexShaderData++;

        VertexShaderData->pos.x = vPos1->x;
        VertexShaderData->pos.y = vPos1->y;
        VertexShaderData->pos.z = vPos1->z;
        VertexShaderData->normal.x = vNormal1->x;
        VertexShaderData->normal.y = vNormal1->y;
        VertexShaderData->normal.z = vNormal1->z;
        VertexShaderData->color = d3dcolor;
        VertexShaderData->inkcoef.x = 0.0f;
        VertexShaderData->inkcoef.y =  vertexThicknesses1 * auxCoef;
        VertexShaderData++;
    }

    NumVertices += 4;

    RWRETURNVOID();
}

void
_rpToonEdgeRenderVertexShaderSubmit(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderVertexShaderSubmit"));

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferUnlock(VertexBuffer);
    }

    if (NumVertices == 0)
    {
        /* early out to avoid crashing driver */
        RWRETURNVOID();
    }

    RWASSERT(NumVertices <= MaxBatch);

    RwD3D9SetStreamSource(0, VertexBuffer, 0, sizeof(EdgeVertexShader));

    RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                               Offset,
                               0, NumVertices,
                               0, (NumVertices / 2));


    RWRETURNVOID();
}

/****************************************************************************
 _rpToonEdgeRenderPrepareVertexShaderFlatSubmit
 */
void
_rpToonEdgeRenderPrepareVertexShaderFlatSubmit(RwUInt32 maxNumEdges,
                                               RwRGBA color)
{
    RwUInt32 inkColor;

    RWFUNCTION(RWSTRING("_rpToonEdgeRenderPrepareVertexShaderFlatSubmit"));

    /* Disable vertex alpha blending */
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

    /* Enable clipping */
    RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

    inkColor = (((RwUInt32)color.alpha) << 24) |
                (((RwUInt32)color.red) << 16) |
                (((RwUInt32)color.green) << 8) |
                color.blue;

    RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, inkColor);

    /* Set color ops */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    /* Set the vertex Declaration */
    RwD3D9SetVertexDeclaration(VertexShaderDeclarationFlat);

    /* Disable any vertex shader */
    RwD3D9SetVertexShader(VertexShaderFlat);

    /* Disable any pixel shader */
    RwD3D9SetPixelShader(NULL);

    RwD3D9SetIndices(IndexBuffer);

    NumVertices = 0;

    MaxVertices = maxNumEdges * 4;

    if (MaxVertices > RPTOON_MAX_VERTICES_BATCH)
    {
        MaxBatch = RPTOON_MAX_VERTICES_BATCH;
    }
    else if (MaxVertices > 1000)
    {
        MaxBatch = ((maxNumEdges / 4) * 4); /* We need a multiple of for */
    }

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexShaderFlat),
                                      MaxBatch,
                                      (void**)&VertexBuffer,
                                      (RwUInt8 **)&VertexShaderFlatData,
                                      &Offset);
    }

    RWRETURNVOID();
}

void
_rpToonEdgeRenderAppendVertexShaderFlatQuad(const RwV3d *vPos0,
                                            const RwV3d *vPos1,
                                            const RwV3d *vNormal0,
                                            const RwV3d *vNormal1,
                                            RwReal vertexThicknesses0,
                                            RwReal vertexThicknesses1)
{
    RWFUNCTION( RWSTRING( "_rpToonEdgeRenderAppendVertexShaderFlatQuad" ) );

    RWASSERT(NumVertices <= MaxVertices);

    if (NumVertices >= MaxBatch)
    {
        _rpToonEdgeRenderVertexShaderFlatSubmit();

        NumVertices = 0;

        MaxVertices -= MaxBatch;

        MaxBatch = min(MaxVertices, RPTOON_MAX_VERTICES_BATCH);
        RWASSERT(MaxBatch > 0);

        RwD3D9DynamicVertexBufferLock(sizeof(EdgeVertexShaderFlat),
                                    MaxBatch,
                                    (void**)&VertexBuffer,
                                    (RwUInt8 **)&VertexShaderFlatData,
                                    &Offset);
    }

    VertexShaderFlatData->pos.x = vPos0->x;
    VertexShaderFlatData->pos.y = vPos0->y;
    VertexShaderFlatData->pos.z = vPos0->z;
    VertexShaderFlatData->normal.x = 0.0f;
    VertexShaderFlatData->normal.y = 0.0f;
    VertexShaderFlatData->normal.z = 1.0f;
    VertexShaderFlatData->thickness = 0.0f;
    VertexShaderFlatData++;

    VertexShaderFlatData->pos.x = vPos1->x;
    VertexShaderFlatData->pos.y = vPos1->y;
    VertexShaderFlatData->pos.z = vPos1->z;
    VertexShaderFlatData->normal.x = 0.0f;
    VertexShaderFlatData->normal.y = 0.0f;
    VertexShaderFlatData->normal.z = 1.0f;
    VertexShaderFlatData->thickness = 0.0f;
    VertexShaderFlatData++;

    VertexShaderFlatData->pos.x = vPos0->x;
    VertexShaderFlatData->pos.y = vPos0->y;
    VertexShaderFlatData->pos.z = vPos0->z;
    VertexShaderFlatData->normal.x = vNormal0->x;
    VertexShaderFlatData->normal.y = vNormal0->y;
    VertexShaderFlatData->normal.z = vNormal0->z;
    VertexShaderFlatData->thickness =  vertexThicknesses0;
    VertexShaderFlatData++;

    VertexShaderFlatData->pos.x = vPos1->x;
    VertexShaderFlatData->pos.y = vPos1->y;
    VertexShaderFlatData->pos.z = vPos1->z;
    VertexShaderFlatData->normal.x = vNormal1->x;
    VertexShaderFlatData->normal.y = vNormal1->y;
    VertexShaderFlatData->normal.z = vNormal1->z;
    VertexShaderFlatData->thickness =  vertexThicknesses1;
    VertexShaderFlatData++;

    NumVertices += 4;

    RWRETURNVOID();
}

void
_rpToonEdgeRenderVertexShaderFlatSubmit(void)
{
    RWFUNCTION(RWSTRING("_rpToonEdgeRenderVertexShaderFlatSubmit"));

    if (MaxBatch)
    {
        RwD3D9DynamicVertexBufferUnlock(VertexBuffer);
    }

    if (NumVertices == 0)
    {
        /* early out to avoid crashing driver */
        RWRETURNVOID();
    }

    RWASSERT(NumVertices <= MaxBatch);

    RwD3D9SetStreamSource(0, VertexBuffer, 0, sizeof(EdgeVertexShaderFlat));

    RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                               Offset,
                               0, NumVertices,
                               0, (NumVertices / 2));


    RWRETURNVOID();
}
