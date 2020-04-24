/*****************************************************************************
 *                                                                           *
 * module : d3d9vertexdeclaration.c                                          *
 *                                                                           *
 * purpose: Compressed vertex support                                        *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 includes
 */

#include <d3d9.h>
#include <d3dx9.h>

#include <string.h>

#include <rwcore.h>

#include "d3d9vertexdeclaration.h"

extern D3DCAPS9 _RwD3D9DeviceCaps;


/*****************************************************************************
 local defines
 */

#define SUPPORT_FLOAT16x

#define SMALL_FLOAT 1e-12f

/*****************************************************************************
 local types
 */

/*****************************************************************************
 local global variables
 */

static const RwUInt32
D3D9VertexTypeSize[D3DDECLTYPE_UNUSED] =
{
    1 * 4,  /* D3DDECLTYPE_FLOAT1    =  0,  1D float expanded to (value, 0., 0., 1.) */
    2 * 4,  /* D3DDECLTYPE_FLOAT2    =  1,  2D float expanded to (value, value, 0., 1.) */
    3 * 4,  /* D3DDECLTYPE_FLOAT3    =  2,  3D float expanded to (value, value, value, 1.) */
    4 * 4,  /* D3DDECLTYPE_FLOAT4    =  3,  4D float */
    4,      /* D3DDECLTYPE_D3DCOLOR  =  4,  4D packed unsigned bytes mapped to 0. to 1. range. Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A) */
    4,      /* D3DDECLTYPE_UBYTE4    =  5,  4D unsigned byte */
    4,      /* D3DDECLTYPE_SHORT2    =  6,  2D signed short expanded to (value, value, 0., 1.) */
    2 * 4,  /* D3DDECLTYPE_SHORT4    =  7,  4D signed short */
    4,      /* D3DDECLTYPE_UBYTE4N   =  8,  Each of 4 bytes is normalized by dividing to 255.0 */
    4,      /* D3DDECLTYPE_SHORT2N   =  9,  2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1) */
    2 * 4,  /* D3DDECLTYPE_SHORT4N   = 10,  4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0) */
    4,      /* D3DDECLTYPE_USHORT2N  = 11,  2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1) */
    2 * 4,  /* D3DDECLTYPE_USHORT4N  = 12,  4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0) */
    4,      /* D3DDECLTYPE_UDEC3     = 13,  3D unsigned 10 10 10 format expanded to (value, value, value, 1) */
    4,      /* D3DDECLTYPE_DEC3N     = 14,  3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1) */
    4,      /* D3DDECLTYPE_FLOAT16_2 = 15,  Two 16-bit floating point values, expanded to (value, value, 0, 1) */
    2 * 4   /* D3DDECLTYPE_FLOAT16_4 = 16,  Four 16-bit floating point values */
};

/*****************************************************************************
 Functions
 */


/*****************************************************************************
 _rpD3D9VertexDeclarationGetSize

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationGetSize(RwUInt32 type)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationGetSize"));
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationGetStride

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationGetStride(const void *vertexDeclaration)
{
    const D3DVERTEXELEMENT9 *d3d9vertexDeclaration = (const D3DVERTEXELEMENT9 *)vertexDeclaration;
    RwUInt32    stride = 0;
    RwInt32     i = 0;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationGetStride"));
    RWASSERT(vertexDeclaration != NULL);

    while (d3d9vertexDeclaration[i].Type < D3DDECLTYPE_UNUSED)
    {
        RWASSERT(d3d9vertexDeclaration[i].Type < D3DDECLTYPE_UNUSED);

        stride += D3D9VertexTypeSize[d3d9vertexDeclaration[i].Type];

        i++;
    }

    RWRETURN(stride);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstWeights

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstWeights(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwV4d *src,
                                    RwInt32 numVerts,
                                    RwUInt32 stride)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstWeights"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != src);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                RwInt32 i;
                RwReal   *dstPosition;

                dstPosition = (RwReal *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *dstPosition = src->x;

                    src++;
                    dstPosition =
                        (RwReal *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                RwInt32 i;
                RwV2d   *dstPosition;

                dstPosition = (RwV2d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;

                    src++;
                    dstPosition =
                        (RwV2d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT3:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *dstPosition = *src;

                    src++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT4:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;
                    dstPosition->z = src->z;
                    dstPosition->w = src->w;

                    src++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    if (src->x >= 1.0f)
                    {
                        dstPosition[2] = 255;
                        dstPosition[1] = 0;
                        dstPosition[0] = 0;
                        dstPosition[3] = 0;
                    }
                    else
                    {
                        dstPosition[1] = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                        dstPosition[0] = (RwUInt8)RwFastRealToUInt32(src->z * 255.f);
                        dstPosition[3] = (RwUInt8)RwFastRealToUInt32(src->w * 255.f);

                        dstPosition[2] = (RwUInt8)(255 -
                                                   dstPosition[0] -
                                                   dstPosition[1] -
                                                   dstPosition[3]);
                    }

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src->z);
                    *(dstPosition + 3) = (RwUInt8)RwFastRealToUInt32(src->w);

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(src->z);
                    *(dstPosition + 3) = (RwInt16)RwFastRealToUInt32(src->w);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    if (src->x >= 1.0f)
                    {
                        dstPosition[0] = 255;
                        dstPosition[1] = 0;
                        dstPosition[2] = 0;
                        dstPosition[3] = 0;
                    }
                    else
                    {
                        dstPosition[1] = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                        dstPosition[2] = (RwUInt8)RwFastRealToUInt32(src->z * 255.f);
                        dstPosition[3] = (RwUInt8)RwFastRealToUInt32(src->w * 255.f);

                        dstPosition[0] = (RwUInt8)(255 -
                                                   dstPosition[1] -
                                                   dstPosition[2] -
                                                   dstPosition[3]);
                    }

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                RwInt32     i;
                RwInt16     *dstPosition;
                RwInt32     nx;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x * 32767.0f);
                    dstPosition[0] = (RwInt16)nx;
                    dstPosition[1] = (RwInt16)(32767 - nx);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                RwInt32     i;
                RwInt16     *dstPosition;
                RwInt32     ny, nz, nw;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    ny = RwFastRealToUInt32(src->y * 32767.0f);
                    nz = RwFastRealToUInt32(src->z * 32767.0f);
                    nw = RwFastRealToUInt32(src->w * 32767.0f);

                    dstPosition[1] = (RwInt16)ny;
                    dstPosition[2] = (RwInt16)nz;
                    dstPosition[3] = (RwInt16)nw;

                    dstPosition[0] = (RwInt16)(32767 - ny - nz - nw);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;
                RwUInt32    nx;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x * 65535.0f);
                    dstPosition[0] = (RwUInt16)nx;
                    dstPosition[1] = (RwUInt16)(65535 - nx);

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;
                RwUInt32    ny, nz, nw;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    ny = RwFastRealToUInt32(src->y * 65535.0f);
                    nz = RwFastRealToUInt32(src->z * 65535.0f);
                    nw = RwFastRealToUInt32(src->w * 65535.0f);

                    dstPosition[1] = (RwUInt16)ny;
                    dstPosition[2] = (RwUInt16)nz;
                    dstPosition[3] = (RwUInt16)nw;

                    dstPosition[0] = (RwUInt16)(65535 - ny - nz - nw);

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x) & 0x3ff;
                    ny = RwFastRealToUInt32(src->y) & 0x3ff;
                    nz = RwFastRealToUInt32(src->z) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    if (src->z < (1.0f / 511.0f))
                    {
                        nx = RwFastRealToUInt32(src->x * 511.0f) & 0x3ff;
                        ny = 511 - nx;

                        *(dstPosition) = ( (ny << 10) | nx );
                    }
                    else
                    {
                        ny = RwFastRealToUInt32(src->y * 511.0f) & 0x3ff;
                        nz = RwFastRealToUInt32(src->z * 511.0f) & 0x3ff;

                        nx = 511 - ny - nz;

                        *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );
                    }

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 2);

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 4);

                    src++;
                    dstPosition += stride;
                }
            }
            break;
#endif

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstV3d

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstV3d(RwUInt32 type,
                                RwUInt8 *mem,
                                const RwV3d *src,
                                RwInt32 numVerts,
                                RwUInt32 stride)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstV3d"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != src);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                RwInt32 i;
                RwReal   *dstPosition;

                dstPosition = (RwReal *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *dstPosition = src->x;

                    src++;
                    dstPosition =
                        (RwReal *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                RwInt32 i;
                RwV2d   *dstPosition;

                dstPosition = (RwV2d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;

                    src++;
                    dstPosition =
                        (RwV2d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT3:
            {
                if (stride == sizeof(RwV3d))
                {
                    memcpy(mem, src, numVerts * sizeof(RwV3d));
                }
                else
                {
                    RwInt32 i;
                    RwV3d   *dstPosition;

                    dstPosition = (RwV3d *)mem;
                    for (i = 0; i < numVerts; i++)
                    {
                        *dstPosition = *src;

                        src++;
                        dstPosition =
                            (RwV3d *)(((RwUInt8 *)dstPosition) + stride);
                    }
                }
            }
            break;

        case D3DDECLTYPE_FLOAT4:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;
                    dstPosition->z = src->z;
                    dstPosition->w = 1.0f;

                    src++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src->x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->z * 255.f);
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src->z);
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(src->z);
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src->z * 255.f);
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y * 32767.0f);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y * 32767.0f);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(src->z * 32767.0f);
                    *(dstPosition + 3) = (RwInt16)32767;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src->x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src->y * 65535.0f);

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src->x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src->y * 65535.0f);
                    *(dstPosition + 2) = (RwUInt16)RwFastRealToUInt32(src->z * 65535.0f);
                    *(dstPosition + 3) = (RwUInt16)65535;

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x) & 0x3ff;
                    ny = RwFastRealToUInt32(src->y) & 0x3ff;
                    nz = RwFastRealToUInt32(src->z) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x * 511.0f) & 0x3ff;
                    ny = RwFastRealToUInt32(src->y * 511.0f) & 0x3ff;
                    nz = RwFastRealToUInt32(src->z * 511.0f) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 2);

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 3);
                    ((RwUInt16 *)dstPosition)[3] = 0x3f80;

                    src++;
                    dstPosition += stride;
                }
            }
            break;
#endif

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstV3dComp

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstV3dComp(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RpVertexNormal *src,
                                    RwInt32 numVerts,
                                    RwUInt32 stride)
{
    RwV3d       temp;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstV3dComp"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != src);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                RwInt32 i;
                RwReal   *dstPosition;

                dstPosition = (RwReal *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *dstPosition = temp.x;

                    src++;
                    dstPosition =
                        (RwReal *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                RwInt32 i;
                RwV2d   *dstPosition;

                dstPosition = (RwV2d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    dstPosition->x = temp.x;
                    dstPosition->y = temp.y;

                    src++;
                    dstPosition =
                        (RwV2d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT3:
            {
                RwInt32 i;
                RwV3d   *dstPosition;

                dstPosition = (RwV3d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(*dstPosition, *src);

                    src++;
                    dstPosition =
                        (RwV3d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT4:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(*dstPosition, *src);

                    dstPosition->w = 1.0f;

                    src++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    /* We need to swap values [0] and [2] */
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(temp.x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(temp.y * 255.f);
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(temp.z * 255.f);
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(temp.x);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(temp.y);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(temp.z);
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(temp.x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(temp.y);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(temp.x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(temp.y);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(temp.z);
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(temp.x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(temp.y * 255.f);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(temp.z * 255.f);
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(temp.x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(temp.y * 32767.0f);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(temp.x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(temp.y * 32767.0f);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(temp.z * 32767.0f);
                    *(dstPosition + 3) = (RwInt16)32767;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(temp.x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(temp.y * 65535.0f);

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(temp.x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(temp.y * 65535.0f);
                    *(dstPosition + 2) = (RwUInt16)RwFastRealToUInt32(temp.z * 65535.0f);
                    *(dstPosition + 3) = (RwUInt16)65535;

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    nx = RwFastRealToUInt32(temp.x) & 0x3ff;
                    ny = RwFastRealToUInt32(temp.y) & 0x3ff;
                    nz = RwFastRealToUInt32(temp.z) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    nx = RwFastRealToUInt32(temp.x * 511.0f) & 0x3ff;
                    ny = RwFastRealToUInt32(temp.y * 511.0f) & 0x3ff;
                    nz = RwFastRealToUInt32(temp.z * 511.0f) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)&temp, 2);

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    RPV3DFROMVERTEXNORMAL(temp, *src);

                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)&temp, 3);
                    ((RwUInt16 *)dstPosition)[3] = 0x3f80;

                    src++;
                    dstPosition += stride;
                }
            }
            break;
#endif

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstV3dMorph

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstV3dMorph(RwUInt32 type,
                                     RwUInt8 *mem,
                                     const RwV3d *src1,
                                     const RwV3d *src2,
                                     RwReal scale,
                                     RwInt32 numVerts,
                                     RwUInt32 stride)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstV3dMorph"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != src1);
    RWASSERT(NULL != src2);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                RwInt32 i;
                RwReal   *dstPosition;

                dstPosition = (RwReal *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *dstPosition = src1->x + (src2->x - src1->x) * scale;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwReal *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                RwInt32 i;
                RwV2d   *dstPosition;

                dstPosition = (RwV2d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src1->x + (src2->x - src1->x) * scale;
                    dstPosition->y = src1->y + (src2->y - src1->y) * scale;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwV2d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT3:
            {
                RwInt32 i;
                RwV3d   *dstPosition;

                dstPosition = (RwV3d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src1->x + (src2->x - src1->x) * scale;
                    dstPosition->y = src1->y + (src2->y - src1->y) * scale;
                    dstPosition->z = src1->z + (src2->z - src1->z) * scale;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwV3d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT4:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src1->x + (src2->x - src1->x) * scale;
                    dstPosition->y = src1->y + (src2->y - src1->y) * scale;
                    dstPosition->z = src1->z + (src2->z - src1->z) * scale;
                    dstPosition->w = 1.0f;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwUInt8     *dstPosition;

                minusScale = (1.f - scale) * 255.0f;
                scale = scale * 255.0f;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src1->z * minusScale + src2->z * scale);
                    *(dstPosition + 3) = 0xff;

                    src1++;
                    src2++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src1->x + (src2->x - src1->x) * scale);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src1->y + (src2->y - src1->y) * scale);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src1->z + (src2->z - src1->z) * scale);
                    *(dstPosition + 3) = 1;

                    src1++;
                    src2++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src1->x + (src2->x - src1->x) * scale);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src1->y + (src2->y - src1->y) * scale);

                    src1++;
                    src2++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src1->x + (src2->x - src1->x) * scale);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src1->y + (src2->y - src1->y) * scale);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(src1->z + (src2->z - src1->z) * scale);
                    *(dstPosition + 3) = 1;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwUInt8     *dstPosition;

                minusScale = (1.f - scale) * 255.0f;
                scale = scale * 255.0f;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src1->z * minusScale + src2->z * scale);
                    *(dstPosition + 3) = 0xff;

                    src1++;
                    src2++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                RwReal      minusScale;
                RwInt32     i;
                RwInt16     *dstPosition;

                minusScale = (1.f - scale) * 32767.0f;
                scale = scale * 32767.0f;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);

                    src1++;
                    src2++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwInt16     *dstPosition;

                minusScale = (1.f - scale) * 32767.0f;
                scale = scale * 32767.0f;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);
                    *(dstPosition + 2) = (RwInt16)RwFastRealToUInt32(src1->z * minusScale + src2->z * scale);
                    *(dstPosition + 3) = (RwInt16)32767;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwUInt16     *dstPosition;

                minusScale = (1.f - scale) * 65535.0f;
                scale = scale * 65535.0f;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);

                    src1++;
                    src2++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwUInt16     *dstPosition;

                minusScale = (1.f - scale) * 65535.0f;
                scale = scale * 65535.0f;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src1->x * minusScale + src2->x * scale);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src1->y * minusScale + src2->y * scale);
                    *(dstPosition + 2) = (RwUInt16)RwFastRealToUInt32(src1->z * minusScale + src2->z * scale);
                    *(dstPosition + 3) = (RwUInt16)65535;

                    src1++;
                    src2++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src1->x + (src2->x - src1->x) * scale) & 0x3ff;
                    ny = RwFastRealToUInt32(src1->y + (src2->y - src1->y) * scale) & 0x3ff;
                    nz = RwFastRealToUInt32(src1->z + (src2->z - src1->z) * scale) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src1++;
                    src2++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwReal      minusScale;
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny, nz;

                minusScale = (1.f - scale) * 511.0f;
                scale = scale * 511.0f;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src1->x * minusScale + src2->x * scale) & 0x3ff;
                    ny = RwFastRealToUInt32(src1->y * minusScale + src2->y * scale) & 0x3ff;
                    nz = RwFastRealToUInt32(src1->z * minusScale + src2->z * scale) & 0x3ff;

                    *(dstPosition) = ( (nz << 20) | (ny << 10) | nx );

                    src1++;
                    src2++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                RwV3d       temp;
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    temp.x = src1->x + (src2->x - src1->x) * scale;
                    temp.y = src1->y + (src2->y - src1->y) * scale;
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)&temp, 2);

                    src1++;
                    src2++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                RwV3d       temp;
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    temp.x = src1->x + (src2->x - src1->x) * scale;
                    temp.y = src1->y + (src2->y - src1->y) * scale;
                    temp.z = src1->z + (src2->z - src1->z) * scale;
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)&temp, 3);
                    ((RwUInt16 *)dstPosition)[3] = 0x3f80;

                    src1++;
                    src2++;
                    dstPosition += stride;
                }
            }
            break;
#endif

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstV2d

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstV2d(RwUInt32 type,
                     RwUInt8 *mem,
                     const RwV2d *src,
                     RwInt32 numVerts,
                     RwUInt32 stride)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstV2d"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != src);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                RwInt32 i;
                RwReal   *dstPosition;

                dstPosition = (RwReal *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *dstPosition = src->x;

                    src++;
                    dstPosition =
                        (RwReal *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                if (stride == sizeof(RwV2d))
                {
                    memcpy(mem, src, numVerts * sizeof(RwV2d));
                }
                else
                {
                    RwInt32 i;
                    RwV2d   *dstPosition;

                    dstPosition = (RwV2d *)mem;
                    for (i = 0; i < numVerts; i++)
                    {
                        dstPosition->x = src->x;
                        dstPosition->y = src->y;

                        src++;
                        dstPosition =
                            (RwV2d *)(((RwUInt8 *)dstPosition) + stride);
                    }
                }
            }
            break;

        case D3DDECLTYPE_FLOAT3:
            {
                RwInt32 i;
                RwV3d   *dstPosition;

                dstPosition = (RwV3d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;
                    dstPosition->z = 0.0f;

                    src++;
                    dstPosition =
                        (RwV3d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_FLOAT4:
            {
                RwInt32 i;
                RwV4d   *dstPosition;

                dstPosition = (RwV4d *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition->x = src->x;
                    dstPosition->y = src->y;
                    dstPosition->z = 0.0f;
                    dstPosition->w = 1.0f;

                    src++;
                    dstPosition =
                        (RwV4d *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    *(dstPosition + 2) = (RwUInt8)RwFastRealToUInt32(src->x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                    *(dstPosition + 0) = 0;
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = 0;
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y);
                    *(dstPosition + 2) = 0;
                    *(dstPosition + 3) = 1;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt8)RwFastRealToUInt32(src->x * 255.f);
                    *(dstPosition + 1) = (RwUInt8)RwFastRealToUInt32(src->y * 255.f);
                    *(dstPosition + 2) = 0;
                    *(dstPosition + 3) = 0xff;

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y * 32767.0f);

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)RwFastRealToUInt32(src->x * 32767.0f);
                    *(dstPosition + 1) = (RwInt16)RwFastRealToUInt32(src->y * 32767.0f);
                    *(dstPosition + 2) = 0;
                    *(dstPosition + 3) = (RwInt16)32767;

                    src++;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src->x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src->y * 65535.0f);

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                RwInt32     i;
                RwUInt16     *dstPosition;

                dstPosition = (RwUInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwUInt16)RwFastRealToUInt32(src->x * 65535.0f);
                    *(dstPosition + 1) = (RwUInt16)RwFastRealToUInt32(src->y * 65535.0f);
                    *(dstPosition + 2) = 0;
                    *(dstPosition + 3) = (RwUInt16)65535;

                    src++;
                    dstPosition =
                        (RwUInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x) & 0x3ff;
                    ny = RwFastRealToUInt32(src->y) & 0x3ff;

                    *(dstPosition) = ( (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32     i;
                RwUInt32    *dstPosition;
                RwUInt32    nx, ny;

                dstPosition = (RwUInt32 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    nx = RwFastRealToUInt32(src->x * 511.0f) & 0x3ff;
                    ny = RwFastRealToUInt32(src->y * 511.0f) & 0x3ff;

                    *(dstPosition) = ( (ny << 10) | nx );

                    src++;
                    dstPosition =
                        (RwUInt32 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 2);

                    src++;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    D3DXFloat32To16Array((D3DXFLOAT16 *)dstPosition, (const RwReal *)src, 2);
                    ((RwUInt16 *)dstPosition)[2] = 0;
                    ((RwUInt16 *)dstPosition)[3] = 0x3f80;

                    src++;
                    dstPosition += stride;
                }
            }
            break;
#endif

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstColor

 Purpose:

 On entry:
 On exit :
*/
RwBool
_rpD3D9VertexDeclarationInstColor(RwUInt8 *mem,
                       const RwRGBA *srcColor,
                       RwInt32 numVerts,
                       RwUInt32 stride)
{
    RwUInt8     alpha = 0xff;
    RwUInt32    *dstColor;
    RwInt32     i;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstColor"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != srcColor);

    dstColor = (RwUInt32 *)mem;
    for (i = 0; i < numVerts; i++)
    {
        *(dstColor) = ((srcColor[i].alpha << 24) |
                       (srcColor[i].red << 16) |
                       (srcColor[i].green << 8) |
                       (srcColor[i].blue));

        /* Does the pre-light contain alpha */
        alpha &= srcColor[i].alpha;

        dstColor = (RwUInt32 *)(((RwUInt8 *)dstColor) + stride);
    }

    RWRETURN(alpha != 0xff);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstIndices

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstIndices(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwUInt32 *indices,
                                    RwInt32 numVerts,
                                    RwUInt32 stride)
{
    const RwUInt8 *src = (const RwUInt8 *)indices;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstIndices"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != indices);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    *(dstPosition + 0) = src[2];
                    *(dstPosition + 1) = src[1];
                    *(dstPosition + 2) = src[0];
                    *(dstPosition + 3) = src[3];

                    src += 4;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = src[0];
                    *(dstPosition + 1) = src[1];
                    *(dstPosition + 2) = src[2];
                    *(dstPosition + 3) = src[3];

                    src += 4;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)src[0];
                    *(dstPosition + 1) = (RwInt16)src[1];

                    src += 4;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    *(dstPosition + 0) = (RwInt16)src[0];
                    *(dstPosition + 1) = (RwInt16)src[1];
                    *(dstPosition + 2) = (RwInt16)src[2];
                    *(dstPosition + 3) = (RwInt16)src[3];

                    src += 4;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstIndices

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstIndicesRemap(RwUInt32 type,
                                         RwUInt8 *mem,
                                         const RwUInt32 *indices,
                                         const RwUInt8 *remap,
                                         RwInt32 numVerts,
                                         RwUInt32 stride)
{
    const RwUInt8 *src = (const RwUInt8 *)indices;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstIndices"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != indices);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_D3DCOLOR:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    /* We need to swap values [0] and [2] */
                    dstPosition[0] = remap[src[2]];
                    dstPosition[1] = remap[src[1]];
                    dstPosition[2] = remap[src[0]];
                    dstPosition[3] = remap[src[3]];

                    src += 4;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                RwInt32     i;
                RwUInt8     *dstPosition;

                dstPosition = (RwUInt8 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition[0] = remap[src[0]];
                    dstPosition[1] = remap[src[1]];
                    dstPosition[2] = remap[src[2]];
                    dstPosition[3] = remap[src[3]];

                    src += 4;
                    dstPosition += stride;
                }
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition[0] = (RwInt16)remap[src[0]];
                    dstPosition[1] = (RwInt16)remap[src[1]];

                    src += 4;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                RwInt32     i;
                RwInt16     *dstPosition;

                dstPosition = (RwInt16 *)mem;
                for (i = 0; i < numVerts; i++)
                {
                    dstPosition[0] = (RwInt16)remap[src[0]];
                    dstPosition[1] = (RwInt16)remap[src[1]];
                    dstPosition[2] = (RwInt16)remap[src[2]];
                    dstPosition[3] = (RwInt16)remap[src[3]];

                    src += 4;
                    dstPosition =
                        (RwInt16 *)(((RwUInt8 *)dstPosition) + stride);
                }
            }
            break;

        default:
            break;
    }

    RWRETURN(D3D9VertexTypeSize[type]);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationInstTangent

 Purpose:

 On entry:
 On exit :
*/
RwUInt32
_rpD3D9VertexDeclarationInstTangent(RwUInt32 type,
                                    RwUInt8 *mem,
                                    const RwV3d *pos,
                                    const RwV3d *normal,
                                    const RpVertexNormal *packedNormal,
                                    const RwTexCoords *texCoord,
                                    RxD3D9ResEntryHeader *meshHeader,
                                    RwUInt32 stride)
{
    RwUInt32    numIndices, numVertices;
    RwUInt32    n, j;
    RwReal      unused;
    RwUInt32    i1, i2, i3;
    RwV3d       *tangent, *currentTangent;
    RwV3d       edge01, edge02, cp;
    RwUInt32            numMeshes;
    RxD3D9InstanceData  *instancedData;
    const RxVertexIndex *indexBufferOrigin;
    const RxVertexIndex *indexBuffer;
    RwUInt32    *tangentsRemapping;

    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationInstTangent"));
    RWASSERT(NULL != mem);
    RWASSERT(NULL != pos);
    RWASSERT(NULL != normal || NULL != packedNormal);
    RWASSERT(NULL != texCoord);
    RWASSERT(NULL != meshHeader);
    RWASSERT(NULL != meshHeader->indexBuffer);
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    numVertices = meshHeader->totalNumVertex;

    tangent = (RwV3d *)RwMalloc(sizeof(RwV3d) * numVertices,
        rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);
    memset(tangent , 0, sizeof(RwV3d) * numVertices);

    IDirect3DIndexBuffer9_Lock((LPINDEXBUFFER)meshHeader->indexBuffer,
                                0,
                                (meshHeader->totalNumIndex) * sizeof(RwUInt16),
                                (RwUInt8 **)&indexBufferOrigin, 0);

    numMeshes = meshHeader->numMeshes;
    instancedData = (RxD3D9InstanceData *)(meshHeader + 1);

    do
    {
        indexBuffer = indexBufferOrigin + instancedData->startIndex;

        numIndices = instancedData->numIndex;

        if (meshHeader->primType == D3DPT_TRIANGLESTRIP)
        {
            numIndices -= 2;
        }

        n = 0;
        while (n < numIndices)
        {
            const RwV3d *vert1;
            const RwV3d *vert2;
            const RwV3d *vert3;
            const RwTexCoords *uv1;
            const RwTexCoords *uv2;
            const RwTexCoords *uv3;
            RwV3d *tang1;
            RwV3d *tang2;
            RwV3d *tang3;

            if (meshHeader->primType == D3DPT_TRIANGLELIST)
            {
                i1 = indexBuffer[n + 0] + instancedData->minVert;
                i2 = indexBuffer[n + 1] + instancedData->minVert;
                i3 = indexBuffer[n + 2] + instancedData->minVert;
                n += 3;
            }
            else /* if (meshHeader->primType == D3DPT_TRIANGLESTRIP) */
            {
                if (n & 0x01)
                {
                    i1 = indexBuffer[n + 2] + instancedData->minVert;
                    i2 = indexBuffer[n + 1] + instancedData->minVert;
                    i3 = indexBuffer[n + 0] + instancedData->minVert;
                }
                else
                {
                    i1 = indexBuffer[n + 0] + instancedData->minVert;
                    i2 = indexBuffer[n + 1] + instancedData->minVert;
                    i3 = indexBuffer[n + 2] + instancedData->minVert;
                }
                n++;

                if (i1 == i2 || i1 == i3 || i2 == i3)
                {
                    continue;
                }
            }

            vert1 = pos + i1;
            vert2 = pos + i2;
            vert3 = pos + i3;

            uv1 = texCoord + i1;
            uv2 = texCoord + i2;
            uv3 = texCoord + i3;

            tang1 = (tangent + i1);
            tang2 = (tangent + i2);
            tang3 = (tangent + i3);

            /* x, s, t */
            edge01.x = (vert2->x - vert1->x);
            edge01.y = (uv2->u - uv1->u);
            edge01.z = (uv2->v - uv1->v);

            edge02.x = (vert3->x - vert1->x);
            edge02.y = (uv3->u - uv1->u);
            edge02.z = (uv3->v - uv1->v);

            RwV3dCrossProduct(&cp, &edge01, &edge02);
            if ( (RwReal)fabs(cp.x) > SMALL_FLOAT )
            {
                const RwReal invcpx = 1.f / cp.x;

                tang1->x += -cp.y * invcpx;

                tang2->x += -cp.y * invcpx;

                tang3->x += -cp.y * invcpx;
            }

            /* y, s, t */
            edge01.x = (vert2->y - vert1->y);

            edge02.x = (vert3->y - vert1->y);

            RwV3dCrossProduct(&cp, &edge01, &edge02);
            if ( (RwReal)fabs(cp.x) > SMALL_FLOAT )
            {
                const RwReal invcpx = 1.f / cp.x;

                tang1->y += -cp.y * invcpx;

                tang2->y += -cp.y * invcpx;

                tang3->y += -cp.y * invcpx;
            }

            /* z, s, t */
            edge01.x = (vert2->z - vert1->z);

            edge02.x = (vert3->z - vert1->z);

            RwV3dCrossProduct(&cp, &edge01, &edge02);
            if ( (RwReal)fabs(cp.x) > SMALL_FLOAT )
            {
                const RwReal invcpx = 1.f / cp.x;

                tang1->z += -cp.y * invcpx;

                tang2->z += -cp.y * invcpx;

                tang3->z += -cp.y * invcpx;
            }
        }

        instancedData++;
    }
    while (--numMeshes);

    IDirect3DIndexBuffer9_Unlock((LPINDEXBUFFER)meshHeader->indexBuffer);

    /* Fix for similar vertices tangents */
    tangentsRemapping = (RwUInt32 *)RwMalloc(sizeof(RwUInt32) * numVertices,
                                rwMEMHINTDUR_FUNCTION | rwID_WORLDPIPEMODULE);

    for (n = 0; n < numVertices; n++)
    {
        tangentsRemapping[n] = n;
    }

    /* Lock for similar vertex with a similar tangent */
    for (n = 0; n < numVertices; n++)
    {
        currentTangent = (tangent + n);

        for (j = n + 1; j < numVertices; j++)
        {
            if (tangentsRemapping[j] > n)
            {
                if (fabs(pos[n].x - pos[j].x) <= 0.000001f &&
                    fabs(pos[n].y - pos[j].y) <= 0.000001f &&
                    fabs(pos[n].z - pos[j].z) <= 0.000001f)
                {
                    /* Check same normal */
                    RwReal cosangle;

                    if (normal != NULL)
                    {
                        cosangle = RwV3dDotProduct((normal + n), (normal + j));
                    }
                    else
                    {
                        RPV3DFROMVERTEXNORMAL(edge01, packedNormal[n]);
                        RPV3DFROMVERTEXNORMAL(edge02, packedNormal[j]);

                        cosangle = RwV3dDotProduct(&edge01, &edge02);
                    }

                    if (cosangle >= 0.9999f)
                    {
                        /* Check similar tangents */
                        cosangle = RwV3dDotProduct(currentTangent, (tangent + j));
                        cosangle /= RwV3dLength(currentTangent);
                        cosangle /= RwV3dLength(tangent + j);

                        if (cosangle > 0.7f)
                        {
                            /* accumulate */
                            currentTangent->x += tangent[j].x;
                            currentTangent->y += tangent[j].y;
                            currentTangent->z += tangent[j].z;

                            tangentsRemapping[j] = n;
                        }
                    }
                }
            }
        }
    }

    /* Normalize the tangents vectors */
    for (n = 0; n < numVertices; n++)
    {
        if (tangentsRemapping[n] == n)
        {
            _rwV3dNormalizeMacro(unused, (tangent + n), (tangent + n));
        }
        else
        {
            tangent[n] = tangent[tangentsRemapping[n]];
        }
    }

    RwFree(tangentsRemapping);

    n = _rpD3D9VertexDeclarationInstV3d(type, mem, tangent,
                                        numVertices,
                                        stride);

    RwFree(tangent);

    RWRETURN(n);
}

/*****************************************************************************
 _rpD3D9VertexDeclarationUnInstV3d

 Purpose:

 On entry:
 On exit :
*/
void
_rpD3D9VertexDeclarationUnInstV3d(RwUInt32 type,
                                  RwV3d *dst,
                                  const RwUInt8 *src)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationUnInstV3d"));
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                dst->x = ((const RwV3d *)src)->x;
                dst->y = 0.0f;
                dst->z = 0.0f;
            }
            break;

        case D3DDECLTYPE_FLOAT2:
            {
                dst->x = ((const RwV3d *)src)->x;
                dst->y = ((const RwV3d *)src)->y;
                dst->z = 0.0f;
            }
            break;

        case D3DDECLTYPE_FLOAT3:
        case D3DDECLTYPE_FLOAT4:
            {
                dst->x = ((const RwV3d *)src)->x;
                dst->y = ((const RwV3d *)src)->y;
                dst->z = ((const RwV3d *)src)->z;
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                /* We need to swap values [0] and [2] */
                dst->x = (RwReal)(((const RwUInt8 *)src)[2]) / 255.f;
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]) / 255.f;
                dst->z = (RwReal)(((const RwUInt8 *)src)[0]) / 255.f;
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                dst->x = (RwReal)(((const RwUInt8 *)src)[0]);
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]);
                dst->z = (RwReal)(((const RwUInt8 *)src)[2]);
            }
            break;

        case D3DDECLTYPE_SHORT2:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]);
                dst->y = (RwReal)(((const RwInt16 *)src)[1]);
                dst->z = 0.0f;
            }
            break;

        case D3DDECLTYPE_SHORT4:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]);
                dst->y = (RwReal)(((const RwInt16 *)src)[1]);
                dst->z = (RwReal)(((const RwInt16 *)src)[2]);
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                dst->x = (RwReal)(((const RwUInt8 *)src)[0]) / 255.f;
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]) / 255.f;
                dst->z = (RwReal)(((const RwUInt8 *)src)[2]) / 255.f;
            }
            break;

        case D3DDECLTYPE_SHORT2N:
             {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 32767.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 32767.0f;
                dst->z = 0.0f;
            }
           break;

        case D3DDECLTYPE_SHORT4N:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 32767.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 32767.0f;
                dst->z = (RwReal)(((const RwInt16 *)src)[2]) / 32767.0f;
            }
            break;

        case D3DDECLTYPE_USHORT2N:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 65535.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 65535.0f;
                dst->z = 0.0f;
            }
            break;

        case D3DDECLTYPE_USHORT4N:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 65535.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 65535.0f;
                dst->z = (RwReal)(((const RwInt16 *)src)[2]) / 65535.0f;
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwUInt32    nx, ny, nz;

                nx = (((const RwUInt32 *)src)[0]) & 0x3ff;
                ny = ((((const RwUInt32 *)src)[0]) >> 10) & 0x3ff;
                nz = ((((const RwUInt32 *)src)[0]) >> 20) & 0x3ff;

                dst->x = (RwReal)(nx);
                dst->y = (RwReal)(ny);
                dst->z = (RwReal)(nz);
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32    nx, ny, nz;

                nx = (((const RwInt32 *)src)[0]) & 0x3ff;
                if (nx & 0x200)
                {
                    nx |= 0xFFFFFC00;
                }

                ny = ((((const RwInt32 *)src)[0]) >> 10) & 0x3ff;
                if (ny & 0x200)
                {
                    ny |= 0xFFFFFC00;
                }

                nz = ((((const RwInt32 *)src)[0]) >> 20) & 0x3ff;
                if (nz & 0x200)
                {
                    nz |= 0xFFFFFC00;
                }

                dst->x = (RwReal)(nx) / 511.0f;
                dst->y = (RwReal)(ny) / 511.0f;
                dst->z = (RwReal)(nz) / 511.0f;
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
            {
                D3DXFloat16To32Array((RwReal *)dst, (const D3DXFLOAT16 *)src, 2);
                dst->z = 0.0f;
            }
            break;

        case D3DDECLTYPE_FLOAT16_4:
            {
                D3DXFloat16To32Array((RwReal *)dst, (const D3DXFLOAT16 *)src, 3);
            }
            break;
#endif

        default:
            dst->x = 0.0f;
            dst->y = 0.0f;
            dst->z = 0.0f;
            break;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rpD3D9VertexDeclarationUnInstV2d

 Purpose:

 On entry:
 On exit :
*/
void
_rpD3D9VertexDeclarationUnInstV2d(RwUInt32 type,
                                  RwV2d *dst,
                                  const RwUInt8 *src)
{
    RWFUNCTION(RWSTRING("_rpD3D9VertexDeclarationUnInstV2d"));
    RWASSERT(type < D3DDECLTYPE_UNUSED);

    switch (type)
    {
        case D3DDECLTYPE_FLOAT1:
            {
                dst->x = ((const RwV3d *)src)->x;
                dst->y = 0.0f;
            }
            break;

        case D3DDECLTYPE_FLOAT2:
        case D3DDECLTYPE_FLOAT3:
        case D3DDECLTYPE_FLOAT4:
            {
                dst->x = ((const RwV3d *)src)->x;
                dst->y = ((const RwV3d *)src)->y;
            }
            break;

        case D3DDECLTYPE_D3DCOLOR:
            {
                /* We need to swap values [0] and [2] */
                dst->x = (RwReal)(((const RwUInt8 *)src)[2]) / 255.f;
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]) / 255.f;
            }
            break;

        case D3DDECLTYPE_UBYTE4:
            {
                dst->x = (RwReal)(((const RwUInt8 *)src)[0]);
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]);
            }
            break;

        case D3DDECLTYPE_SHORT2:
        case D3DDECLTYPE_SHORT4:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]);
                dst->y = (RwReal)(((const RwInt16 *)src)[1]);
            }
            break;

        case D3DDECLTYPE_UBYTE4N:
            {
                dst->x = (RwReal)(((const RwUInt8 *)src)[0]) / 255.f;
                dst->y = (RwReal)(((const RwUInt8 *)src)[1]) / 255.f;
            }
            break;

        case D3DDECLTYPE_SHORT2N:
        case D3DDECLTYPE_SHORT4N:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 32767.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 32767.0f;
            }
            break;

        case D3DDECLTYPE_USHORT2N:
        case D3DDECLTYPE_USHORT4N:
            {
                dst->x = (RwReal)(((const RwInt16 *)src)[0]) / 65535.0f;
                dst->y = (RwReal)(((const RwInt16 *)src)[1]) / 65535.0f;
            }
            break;

        case D3DDECLTYPE_UDEC3:
            {
                RwUInt32    nx, ny;

                nx = (((const RwUInt32 *)src)[0]) & 0x3ff;
                ny = ((((const RwUInt32 *)src)[0]) >> 10) & 0x3ff;

                dst->x = (RwReal)(nx);
                dst->y = (RwReal)(ny);
            }
            break;

        case D3DDECLTYPE_DEC3N:
            {
                RwInt32    nx, ny;

                nx = (((const RwInt32 *)src)[0]) & 0x3ff;
                if (nx & 0x200)
                {
                    nx |= 0xFFFFFC00;
                }

                ny = ((((const RwInt32 *)src)[0]) >> 10) & 0x3ff;
                if (ny & 0x200)
                {
                    ny |= 0xFFFFFC00;
                }

                dst->x = (RwReal)(nx) / 511.0f;
                dst->y = (RwReal)(ny) / 511.0f;
            }
            break;

#ifdef SUPPORT_FLOAT16
        case D3DDECLTYPE_FLOAT16_2:
        case D3DDECLTYPE_FLOAT16_4:
            {
                D3DXFloat16To32Array((RwReal *)dst, (const D3DXFLOAT16 *)src, 2);
            }
            break;
#endif

        default:
            dst->x = 0.0f;
            dst->y = 0.0f;
            break;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rpD3D9GetMinMaxValuesV3d
*/
void
_rpD3D9GetMinMaxValuesV3d(const RwV3d *src,
                          RwInt32 numElements,
                          RwV3d *min,
                          RwV3d *max)
{
    RwInt32 n;

    RWFUNCTION(RWSTRING("_rpD3D9GetMinMaxValuesV3d"));
    RWASSERT(src != NULL);
    RWASSERT(min != NULL);
    RWASSERT(max != NULL);
    RWASSERT(numElements > 0);

    min->x = src->x;
    min->y = src->y;
    min->z = src->z;

    max->x = src->x;
    max->y = src->y;
    max->z = src->z;

    for (n = 1; n < numElements; n++, src++)
    {
        if (min->x > src->x)
        {
            min->x = src->x;
        }
        else if (max->x < src->x)
        {
            max->x = src->x;
        }

        if (min->y > src->y)
        {
            min->y = src->y;
        }
        else if (max->y < src->y)
        {
            max->y = src->y;
        }

        if (min->z > src->z)
        {
            min->z = src->z;
        }
        else if (max->z < src->z)
        {
            max->z = src->z;
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rpD3D9GetMinMaxValuesV2d
*/
void
_rpD3D9GetMinMaxValuesV2d(const RwV2d *src,
                          RwInt32 numElements,
                          RwV2d *min,
                          RwV2d *max)
{
    RwInt32 n;

    RWFUNCTION(RWSTRING("_rpD3D9GetMinMaxValuesV2d"));
    RWASSERT(src != NULL);
    RWASSERT(min != NULL);
    RWASSERT(max != NULL);
    RWASSERT(numElements > 0);

    min->x = src->x;
    min->y = src->y;

    max->x = src->x;
    max->y = src->y;

    for (n = 1; n < numElements; n++, src++)
    {
        if (min->x > src->x)
        {
            min->x = src->x;
        }
        else if (max->x < src->x)
        {
            max->x = src->x;
        }

        if (min->y > src->y)
        {
            min->y = src->y;
        }
        else if (max->y < src->y)
        {
            max->y = src->y;
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 _rpD3D9FindFormatV3d
*/
RwUInt32
_rpD3D9FindFormatV3d(const RwV3d *src,
                     RwInt32 numElements)
{
    RwUInt32 type;
    RwV3d min, max;

    RWFUNCTION(RWSTRING("_rpD3D9FindFormatV3d"));
    RWASSERT(src != NULL);
    RWASSERT(numElements > 0);

    _rpD3D9GetMinMaxValuesV3d(src, numElements, &min, &max);

    type = D3DDECLTYPE_FLOAT3;

    if (max.x <= 1.0f &&
        max.y <= 1.0f &&
        max.z <= 1.0f)
    {
        if (min.x >= 0.0f &&
            min.y >= 0.0f &&
            min.z >= 0.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                type = D3DDECLTYPE_DEC3N;
            }
            else if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_USHORT4N)
            {
                type = D3DDECLTYPE_USHORT4N;
            }
        }
        else if (min.x >= -1.0f &&
                 min.y >= -1.0f &&
                 min.z >= -1.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                type = D3DDECLTYPE_DEC3N;
            }
            else if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_SHORT4N)
            {
                type = D3DDECLTYPE_SHORT4N;
            }
        }
#ifdef SUPPORT_FLOAT16
        else if (min.x >= -4.0f &&
                 min.y >= -4.0f &&
                 min.z >= -4.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_FLOAT16_4)
            {
                type = D3DDECLTYPE_FLOAT16_4;
            }
        }
#endif
    }
#ifdef SUPPORT_FLOAT16
    else if (max.x <= 4.0f &&
             max.y <= 4.0f &&
             max.z <= 4.0f)
    {
        if (min.x >= -4.0f &&
            min.y >= -4.0f &&
            min.z >= -4.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_FLOAT16_4)
            {
                type = D3DDECLTYPE_FLOAT16_4;
            }
        }
    }
#endif

    RWRETURN(type);
}

/*****************************************************************************
 _rpD3D9FindFormatV2d
*/
RwUInt32
_rpD3D9FindFormatV2d(const RwV2d *src,
                     RwInt32 numElements)
{
    RwUInt32 type;
    RwV2d min, max;

    RWFUNCTION(RWSTRING("_rpD3D9FindFormatV2d"));
    RWASSERT(src != NULL);
    RWASSERT(numElements > 0);

    _rpD3D9GetMinMaxValuesV2d(src, numElements, &min, &max);

    type = D3DDECLTYPE_FLOAT2;

    if (max.x <= 1.0f &&
        max.y <= 1.0f)
    {
        if (min.x >= 0.0f &&
            min.y >= 0.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_USHORT2N)
            {
                type = D3DDECLTYPE_USHORT2N;
            }
            else if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                type = D3DDECLTYPE_DEC3N;
            }
            else
            {
                type = D3DDECLTYPE_D3DCOLOR;
            }
        }
        else if (min.x >= -1.0f &&
                 min.y >= -1.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_SHORT2N)
            {
                type = D3DDECLTYPE_SHORT2N;
            }
            else if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_DEC3N)
            {
                type = D3DDECLTYPE_DEC3N;
            }
        }
#ifdef SUPPORT_FLOAT16
        else if (min.x >= -4.0f &&
                 min.y >= -4.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_FLOAT16_2)
            {
                type = D3DDECLTYPE_FLOAT16_2;
            }
        }
#endif
    }
#ifdef SUPPORT_FLOAT16
    else if (max.x <= 4.0f &&
             max.y <= 4.0f)
    {
        if (min.x >= -4.0f &&
            min.y >= -4.0f)
        {
            if (_RwD3D9DeviceCaps.DeclTypes & D3DDTCAPS_FLOAT16_2)
            {
                type = D3DDECLTYPE_FLOAT16_2;
            }
        }
    }
#endif

    RWRETURN(type);
}
