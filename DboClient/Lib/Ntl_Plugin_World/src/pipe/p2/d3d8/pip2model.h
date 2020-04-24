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
#include "p2core.h"

/* RWPUBLIC */





/****************************************************************************
 Global Defines
 */

#define RXHEAPPLATFORMDEFAULTSIZE  (1 << 12) /* 4k */

/****************************************************************************
 Global Types
 */

/* We use D3D8 formats for the instanced versions, to allow hardware T&L */

/**
 * \ingroup rwcoregeneric
 * \ref RxObjSpace3DVertex
 * Typedef for an RxObjSpace3DVertex.
 */
typedef struct RxObjSpace3DVertex RxObjSpace3DVertex;

/*
 * Structure representing object space vertex.
 */
#if (!defined(DOXYGEN))
struct RxObjSpace3DVertex
{
    RwV3d       objVertex;        /* position */
    RwV3d       objNormal;        /* normal */
    RwUInt32    color;            /* emissive color*/
    RwReal      u;                /* u */
    RwReal      v;                /* v */
};
#endif /* (!defined(DOXYGEN)) */

/* This vertex is non truncatable */
#define RxObjSpace3DVertexNoUVsNoNormalsSize (sizeof(RxObjSpace3DVertex))
#define RxObjSpace3DVertexNoUVsSize          (sizeof(RxObjSpace3DVertex))
#define RxObjSpace3DVertexFullSize           (sizeof(RxObjSpace3DVertex))

/**
 * \ingroup rwcoregeneric
 * \ref RxObjSpace3DLitVertex
 * Typedef for an RxObjSpace3DLitVertex.
 */
typedef RxObjSpace3DVertex RxObjSpace3DLitVertex;

/** 
 * \ingroup rwim3dvertex
 * \ref RwIm3DVertex
 * Typedef for an RwIm3DVertex.
 */
typedef RxObjSpace3DLitVertex RwIm3DVertex;

/* LEGACY-SUPPORT macro */
/**
 * \ingroup rwcoregeneric
 * \ref RxScrSpace2DVertex
 * Typedef for an RxScrSpace2DVertex structure
 */
typedef RwIm2DVertex RxScrSpace2DVertex;

/****************************************************************************
 Object-space 3D unlit vertex macros
 */

/* Vertex positions */
#define RxObjSpace3DVertexGetPos(_vert, _pos) \
    (*(_pos) = (_vert)->objVertex)
#define RxObjSpace3DVertexSetPos(_vert, _pos) \
    ((_vert)->objVertex = *(_pos))

/* Pre-lighting colours */
#define RxObjSpace3DVertexGetPreLitColor(_vert, _col) \
MACRO_START \
{ \
    (_col)->alpha = (RwUInt8)((_vert)->color >> 24) & 0xFF; \
    (_col)->red   = (RwUInt8)((_vert)->color >> 16) & 0xFF; \
    (_col)->green = (RwUInt8)((_vert)->color >>  8) & 0xFF; \
    (_col)->blue  = (RwUInt8)((_vert)->color      ) & 0xFF; \
} \
MACRO_STOP

#define RxObjSpace3DVertexSetPreLitColor(_vert, _col) \
    ((_vert)->color = (((RwUInt32)(_col)->alpha) << 24) | \
                              (((RwUInt32)(_col)->red)   << 16) | \
                              (((RwUInt32)(_col)->green) <<  8) | \
                              (((RwUInt32)(_col)->blue)       ))

/* This uses the same slot as color (they are mutually exclusive) */
#define RxObjSpace3DVertexGetColor RxObjSpace3DVertexGetPreLitColor

/* Normals */
#define RxObjSpace3DVertexGetNormal(_vert, _normal)             \
    (*(_normal) = (_vert)->objNormal)
#define RxObjSpace3DVertexSetNormal(_vert, _normal)             \
    ((_vert)->objNormal = *(_normal))

/* Us and Vs */
#define RxObjSpace3DVertexGetU(_vert)                          \
    ((_vert)->u)
#define RxObjSpace3DVertexGetV(_vert)                          \
    ((_vert)->v)
#define RxObjSpace3DVertexSetU(_vert, _imu)                     \
    ((_vert)->u = (_imu))
#define RxObjSpace3DVertexSetV(_vert, _imv)                     \
    ((_vert)->v = (_imv))

/****************************************************************************
 Object-space 3D lit vertex macros
 */

/* Vertex positions */
#define RxObjSpace3DLitVertexGetPos(_vert, _pos) \
    (*(_pos) = (_vert)->objVertex)
#define RxObjSpace3DLitVertexSetPos(_vert, _pos) \
    ((_vert)->objVertex = *(_pos))

/* Vertex colours */
#define RxObjSpace3DLitVertexGetColor(_vert, _col) \
MACRO_START \
{ \
    (_col)->red   = ((_vert)->color >> 16) & 0xFF; \
    (_col)->green = ((_vert)->color >>  8) & 0xFF; \
    (_col)->blue  = ((_vert)->color      ) & 0xFF; \
    (_col)->alpha = ((_vert)->color >> 24) & 0xFF; \
} \
MACRO_STOP
#define RxObjSpace3DLitVertexSetColor(_vert, _col) \
    ((_vert)->color = (((RwUInt32)(_col)->alpha) << 24) | \
                              (((RwUInt32)(_col)->red)   << 16) | \
                              (((RwUInt32)(_col)->green) <<  8) | \
                              (((RwUInt32)(_col)->blue)       ))

/* Us and Vs */
#define RxObjSpace3DLitVertexGetU(_vert) \
    ((_vert)->u)
#define RxObjSpace3DLitVertexGetV(_vert) \
    ((_vert)->v)
#define RxObjSpace3DLitVertexSetU(_vert, _imu) \
    ((_vert)->u = (_imu))
#define RxObjSpace3DLitVertexSetV(_vert, _imv) \
    ((_vert)->v = (_imv))

/* LEGACY-SUPPORT for old objvert names - NB does NOT guarantee the
 * app will work, because the old IM3DVERTEX macros are NOT correctly
 * abstracted - 'Get' will return pointers to RwV3ds inside the
 * ObjVert, but you can't assume there are any RwV3ds inside an
 * opaque vertex type */

/* RWPUBLICEND
 * Correctly emulated ones
 * RWPUBLIC */
#define RwIm3DVertexSetU   RxObjSpace3DLitVertexSetU
#define RwIm3DVertexSetV   RxObjSpace3DLitVertexSetV
#define RwIm3DVertexGetNext(_vert)      ((_vert) + 1)

#define RwIm2DCameraVertexSetU(_devvert, _camvert, _u, _recipz) \
MACRO_START                                                     \
{                                                               \
    RwReal _uTmp = _u;                                          \
    _camvert->u = _uTmp;                                        \
    RwIm2DVertexSetU(_devvert, _uTmp, _recipz);                 \
}                                                               \
MACRO_STOP
#define RwIm2DCameraVertexSetV(_devvert, _camvert, _v, _recipz) \
MACRO_START                                                     \
{                                                               \
    RwReal _vTmp = _v;                                          \
    _camvert->v = _vTmp;                                        \
    RwIm2DVertexSetV(_devvert, _vTmp, _recipz);                 \
}                                                               \
MACRO_STOP

/* RWPUBLICEND
 * painfully emulated ones - option revert your macros to set-by-value
 * RWPUBLIC */
#define RwIm3DVertexSetPos(_vert, _imx, _imy, _imz) \
MACRO_START \
{ \
    (_vert)->objVertex.x = _imx; \
    (_vert)->objVertex.y = _imy; \
    (_vert)->objVertex.z = _imz; \
} \
MACRO_STOP

#define RwIm3DVertexSetNormal(vert, imx, imy, imz)         \
MACRO_START                                                \
{                                                          \
    RwV3d packed;                                          \
    packed.x = imx;                                        \
    packed.y = imy;                                        \
    packed.z = imz;                                        \
    RxObjSpace3DVertexSetNormal(vert, &packed);            \
}                                                          \
MACRO_STOP

#define RwIm3DVertexSetRGBA(_vert, _r, _g, _b, _a) \
MACRO_START \
{ \
    ((_vert)->color = ((_a) << 24) | \
                              ((_r) << 16) | \
                              ((_g) << 8) | \
                              ((_b))); \
} \
MACRO_STOP

/* RWPUBLICEND
 * wrongly emulated ones !!! HAVE TO FIX !!!
 * RWPUBLIC */
#define RwIm3DVertexGetPos(_vert) (&((_vert)->objVertex))
#define RwIm3DVertexGetNormal(vert) (&((vert)->objNormal))

#define RwIm3DVertexCopyRGBA(_dst, _src) (((_dst)->color) = ((_src)->color))

/* RWPUBLICEND */
#endif /* PIP2MODEL_H */
