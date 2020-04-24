/***************************************************************************
 *                                                                         *
 * Module  : pip2model.h                                                   *
 *                                                                         *
 * Purpose : Pipe structure definition                                     *
 *                                                                         *
 **************************************************************************/


#ifndef PIP2MODEL_H
#define PIP2MODEL_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "bacolor.h"
#include "drvmodel.h"

/* RWPUBLIC */
/**
 * \defgroup corenull NULL
 * \ingroup corepowerpipe
 * 
 * Immediate mode rendering (platform-specific)
 * 
 * Copyright (c) Criterion Software Limited
 */

/**
 * \defgroup rwim3dnull RwIm3d
 * \ingroup corenull
 *
 * Platform specific
 */


/****************************************************************************
 Global Defines
 */

#define RXHEAPPLATFORMDEFAULTSIZE  (1 << 12) /* 4k */

/****************************************************************************
 Global Types
 */

typedef struct _RxObjSpace3DVertex RxObjSpace3DVertex;
struct _RxObjSpace3DVertex
{
        RwV3d          objVertex;              /* Object space vertex */
        union
        {
            RwRGBA     preLitColor;            /* Vertex pre-lighting values */
            RwRGBA     color;                  /* Immediate mode vertex color */
        }
        c;
        RwV3d          objNormal;              /* Object space normal */
        RwReal         u, v;                   /* Vertex tex coords */
};

/* The 3D immediate mode one is the same */
typedef RxObjSpace3DVertex RxObjSpace3DLitVertex;

/* This vertex format is non-truncatable */
#define RxObjSpace3DVertexNoUVsNoNormalsSize (offsetof(RxObjSpace3DVertex,objNormal))

#define RxObjSpace3DVertexNoUVsSize          (offsetof(RxObjSpace3DVertex,u))

#define RxObjSpace3DVertexFullSize           (sizeof(RxObjSpace3DVertex))

/* Defined in the driver: (eventually should be true of the above/below) */
typedef RwIm2DVertex RxScrSpace2DVertex;

/****************************************************************************
 Object-space 3D vertex macros
 */

/* Vertex positions */
#define RxObjSpace3DVertexGetPos(_vert, _pos)   \
    (*(_pos) = (_vert)->objVertex)

#define RxObjSpace3DVertexSetPos(_vert, _pos)   \
    ((_vert)->objVertex = *(_pos))

/* Pre-lighting colours */
#define RxObjSpace3DVertexGetPreLitColor(_vert, _col)   \
    (*(_col) = (_vert)->c.preLitColor)

#define RxObjSpace3DVertexSetPreLitColor(_vert, _col)   \
   ((_vert)->c.preLitColor = *(_col))

/* This uses the same slot as color (they are mutually exclusive) */
#define RxObjSpace3DVertexGetColor RxObjSpace3DVertexGetPreLitColor

/* Normals */
#define RxObjSpace3DVertexGetNormal(_vert, _normal)     \
    (*(_normal) = (_vert)->objNormal)

#define RxObjSpace3DVertexSetNormal(_vert, _normal)     \
    ((_vert)->objNormal = *(_normal))

/* Us and Vs */
#define RxObjSpace3DVertexGetU(_vert)           \
    ((_vert)->u)

#define RxObjSpace3DVertexGetV(_vert)           \
    ((_vert)->v)

#define RxObjSpace3DVertexSetU(_vert, _imu)     \
    ((_vert)->u = (_imu))

#define RxObjSpace3DVertexSetV(_vert, _imv)     \
    ((_vert)->v = (_imv))

#if (0)

/* LEGACY-SUPPORT for old objvert names - NB does NOT guarantee the
 * app will work, because the old IM3DVERTEX macros are NOT correctly
 * abstracted - 'Get' will return pointers to RwV3ds inside the
 * ObjVert, but you can't assume there are any RwV3ds inside an
 * opaque vertex type */

#define RwIm3DVertexSetPos(vert, imx, imy, imz) \
    ((vert)->objVertex.x = (imx));              \
    ((vert)->objVertex.y = (imy));              \
    ((vert)->objVertex.z = (imz))

#define RwIm3DVertexGetPos(vert)                                \
    (&((vert)->objVertex))

#define RwIm3DVertexSetRGBA(vert, r, g, b, a)   \
    ((vert)->color.red = (r));                  \
    ((vert)->color.green = (g));                \
    ((vert)->color.blue = (b));                 \
    ((vert)->color.alpha = (a))

#define RwIm3DVertexSetNormal(vert, imx, imy, imz)      \
    ((vert)->objNormal.x = (imx));                      \
    ((vert)->objNormal.y = (imy));                      \
    ((vert)->objNormal.z = (imz))

#endif /* (0) */


/**
 * \ingroup corenull
 * \ref RwIm3DVertex
 * Typedef for an RwIm3DVertex
 */
typedef RxObjSpace3DLitVertex RwIm3DVertex;

/* RWPUBLICEND
 * Correctly emulated ones
 * RWPUBLIC */
#define RwIm3DVertexSetU   RxObjSpace3DVertexSetU
#define RwIm3DVertexSetV   RxObjSpace3DVertexSetV
#define RwIm3DVertexGetNext(_vert)      ((_vert) + 1)

#define RwIm2DCameraVertexSetU(_devvert, _camvert, _u, _recipz) \
MACRO_START                                                     \
{                                                               \
    RwReal _uTmp = _u;                                          \
    camvert->u = _uTmp;                                         \
    RwIm2DVertexSetU(_devvert, _uTmp, recipz);                  \
}                                                               \
MACRO_STOP

#define RwIm2DCameraVertexSetV(_devvert, _camvert, _v, _recipz) \
MACRO_START                                                     \
{                                                               \
    RwReal _vTmp = _v;                                          \
    camvert->v = _vTmp;                                         \
    RwIm2DVertexSetV(_devvert, _vTmp, recipz);                  \
}                                                               \
MACRO_STOP

/* RWPUBLICEND
 * painfully emulated ones - option revert your macros to set-by-value
 * RWPUBLIC */
#define RwIm3DVertexSetPos(_vert, _imx, _imy, _imz)     \
MACRO_START                                             \
{                                                       \
    RwV3d _vTmp;                                        \
    _vTmp.x = _imx;                                     \
    _vTmp.y = _imy;                                     \
    _vTmp.z = _imz;                                     \
    RxObjSpace3DVertexSetPos(_vert, &_vTmp);            \
}                                                       \
MACRO_STOP

#define RwIm3DVertexSetRGBA(_vert, _r, _g, _b, _a)      \
MACRO_START                                             \
{                                                       \
    RwRGBA _col;                                        \
    _col.red = (_r);                                    \
    _col.green = (_g);                                  \
    _col.blue = (_b);                                   \
    _col.alpha = (_a);                                  \
    (_vert)->c.color = _col;                            \
}                                                       \
MACRO_STOP

#define RwIm3DVertexSetNormal(_vert, _imx, _imy, _imz)  \
MACRO_START                                             \
{                                                       \
    RwV3d _vTmp;                                        \
    _vTmp.x = _imx;                                     \
    _vTmp.y = _imy;                                     \
    _vTmp.z = _imz;                                     \
    RxObjSpace3DVertexSetNormal(_vert, &_vTmp);         \
}                                                       \
MACRO_STOP

/* RWPUBLICEND
 * Wrongly emulated ones !!! HAVE TO FIX !!!
 * RWPUBLIC */
#define RwIm3DVertexGetPos(_vert) (&((_vert)->objVertex))

/* RWPUBLICEND */
#endif /* PIP2MODEL_H */
