/***************************************************************************
 *                                                                         *
 * Module  : drvmodel.h (D3D8)                                             *
 *                                                                         *
 * Purpose : Driver model description (device specific vertices/polys)     *
 *                                                                         *
 **************************************************************************/

/* RWPUBLIC */
#ifndef D3D8_DRVMODEL_H
#define D3D8_DRVMODEL_H

#if (defined(__ICL))
/* Avoid voluminous
 *   'warning #344: typedef name has already been declared (with same type)'
 * warnings from MS include files
 */
#pragma warning( disable : 344 )
#endif /* (defined(__ICL)) */


/* RWPUBLICEND */

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "baraster.h"
#include "bamatrix.h"

/* RWPUBLIC */
#if (defined(RWDEBUG))
#if (defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC))
#define _CRTDBG_MAP_ALLOC
#endif /* defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC)) */
#include <crtdbg.h>
#define ERR_WRAP(A) (_rwRePrintErrorDDD3D((A), __FILE__, __LINE__))
#endif /* (defined(RWDEBUG)) */

#if (!defined(ERR_WRAP))
#define ERR_WRAP(A) (A)
#endif /* (!defined(ERR_WRAP)) */

/****************************************************************************
 Defines
 */

/* Set true depth information (for fogging, eg) */
#define RwIm2DVertexSetCameraX(vert, camx)          /* Nothing */
#define RwIm2DVertexSetCameraY(vert, camy)          /* Nothing */
#define RwIm2DVertexSetCameraZ(vert, camz)          /* Nothing */

#define RwIm2DVertexSetRecipCameraZ(vert, recipz)   ((vert)->rhw = recipz)

#define RwIm2DVertexGetCameraX(vert)                (cause an error)
#define RwIm2DVertexGetCameraY(vert)                (cause an error)
#define RwIm2DVertexGetCameraZ(vert)                (cause an error)
#define RwIm2DVertexGetRecipCameraZ(vert)           ((vert)->rhw)

/* Set screen space coordinates in a device vertex */
#define RwIm2DVertexSetScreenX(vert, scrnx)         ((vert)->x = (scrnx))
#define RwIm2DVertexSetScreenY(vert, scrny)         ((vert)->y = (scrny))
#define RwIm2DVertexSetScreenZ(vert, scrnz)         ((vert)->z = (scrnz))
#define RwIm2DVertexGetScreenX(vert)                ((vert)->x)
#define RwIm2DVertexGetScreenY(vert)                ((vert)->y)
#define RwIm2DVertexGetScreenZ(vert)                ((vert)->z)

/* Set texture coordinates in a device vertex */
#define RwIm2DVertexSetU(vert, texU, recipz)        ((vert)->u = (texU))
#define RwIm2DVertexSetV(vert, texV, recipz)        ((vert)->v = (texV))
#define RwIm2DVertexGetU(vert)                      ((vert)->u)
#define RwIm2DVertexGetV(vert)                      ((vert)->v)

/* Modify the luminance stuff */
#define RwIm2DVertexSetRealRGBA(vert, red, green, blue, alpha)  \
    ((vert)->emissiveColor =                                    \
     (((RwFastRealToUInt32(alpha)) << 24) |                        \
      ((RwFastRealToUInt32(red)) << 16) |                          \
      ((RwFastRealToUInt32(green)) << 8) |                         \
      ((RwFastRealToUInt32(blue)))))

#define RwIm2DVertexSetIntRGBA(vert, red, green, blue, alpha)   \
    ((vert)->emissiveColor =                                    \
     ((((RwUInt32)(alpha)) << 24) |                             \
      (((RwUInt32)(red)) << 16) |                               \
      (((RwUInt32)(green)) << 8) |                              \
      (((RwUInt32)(blue)))))

#define RwIm2DVertexGetRed(vert)    \
    (((vert)->emissiveColor >> 16) & 0xFF)

#define RwIm2DVertexGetGreen(vert)  \
    (((vert)->emissiveColor >> 8) & 0xFF)

#define RwIm2DVertexGetBlue(vert)   \
    ((vert)->emissiveColor & 0xFF)

#define RwIm2DVertexGetAlpha(vert)  \
    (((vert)->emissiveColor >> 24) & 0xFF)

#define RwIm2DVertexCopyRGBA(dst, src)  \
    ((dst)->emissiveColor = (src)->emissiveColor)

/* Clipper stuff */

#define RwIm2DVertexClipRGBA(o, i, n, f)                                \
MACRO_START                                                             \
{                                                                       \
    const RwInt32        _factor =                                      \
        (RwFastRealToUInt32(i * (RwReal)(255))) & 255;                  \
                                                                        \
    (o)->emissiveColor =                                                \
        (((((RwIm2DVertexGetAlpha(f) - RwIm2DVertexGetAlpha(n)) *       \
            _factor) >> 8) + RwIm2DVertexGetAlpha(n)) << 24) |          \
        (((((RwIm2DVertexGetRed(f) - RwIm2DVertexGetRed(n)) *           \
            _factor) >> 8) + RwIm2DVertexGetRed(n)) << 16) |            \
        (((((RwIm2DVertexGetGreen(f) - RwIm2DVertexGetGreen(n)) *       \
            _factor) >> 8) + RwIm2DVertexGetGreen(n)) << 8) |           \
        (((((RwIm2DVertexGetBlue(f) - RwIm2DVertexGetBlue(n)) *         \
            _factor) >> 8) + RwIm2DVertexGetBlue(n)));                  \
}                                                                       \
MACRO_STOP

/****************************************************************************
 Global Types
 */

/* We use RwD3D8Vertex to drive the hardware in 2D mode */

/*
 * D3D8 vertex structure definition for 2D geometry
 */
#if !defined(RWADOXYGENEXTERNAL)
typedef struct RwD3D8Vertex RwD3D8Vertex;
/**
 * \ingroup rwim2dd3d8
 * \struct RwD3D8Vertex
 * D3D8 vertex structure definition for 2D geometry
 */
struct RwD3D8Vertex
{
    RwReal      x;              /**< Screen X */
    RwReal      y;              /**< Screen Y */
    RwReal      z;              /**< Screen Z */
    RwReal      rhw;            /**< Reciprocal of homogeneous W */

    RwUInt32    emissiveColor;  /**< Vertex color */

    RwReal      u;              /**< Texture coordinate U */
    RwReal      v;              /**< Texture coordinate V */
};
#endif /* !defined(RWADOXYGENEXTERNAL) */

/* Define types used */

#if !defined(RWADOXYGENEXTERNAL)
/**
 * \ingroup rwim2dvertex
 * \ref RwIm2DVertex
 * Typedef for a RenderWare Graphics Immediate Mode 2D Vertex
 */
typedef RwD3D8Vertex    RwIm2DVertex;
#endif /* !defined(RWADOXYGENEXTERNAL) */

#if !defined(RWADOXYGENEXTERNAL)
/**
 * \ingroup rwcoregeneric
 * \ref RxVertexIndex
 *
 * Typedef for a RenderWare Graphics Powe   rPipe Immediate
 * Mode Vertex
 */
typedef RwUInt16        RxVertexIndex;
#endif /* !defined(RWADOXYGENEXTERNAL) */

#if !defined(RWADOXYGENEXTERNAL)
/**
 * \ingroup rwcoregeneric
 * \ref RwImVertexIndex
 * Typedef for a RenderWare Graphics Immediate Mode Vertex.
 */
typedef RxVertexIndex   RwImVertexIndex;
#endif /* !defined(RWADOXYGENEXTERNAL) */

#if !defined(RWADOXYGENEXTERNAL)
/**
 * \ingroup rwengined3d8
 * \struct RwD3D8Metrics
 * Structure containing metrics counters
 */
typedef struct
{
    RwUInt32    numRenderStateChanges;          /**< Number of Render States changed */
    RwUInt32    numTextureStageStateChanges;    /**< Number of Texture Stage States changed */
    RwUInt32    numMaterialChanges;             /**< Number of Material changes */
    RwUInt32    numLightsChanged;               /**< Number of Lights changed */
    RwUInt32    numVBSwitches;                  /**< Number of Vertex Buffer switches */
}
RwD3D8Metrics;
#endif /* !defined(RWADOXYGENEXTERNAL) */

#endif /* D3D8_DRVMODEL_H */
/* RWPUBLICEND */
