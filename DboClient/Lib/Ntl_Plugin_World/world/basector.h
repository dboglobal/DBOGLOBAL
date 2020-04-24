/***************************************************************************
 *                                                                         *
 * Module  : basector.h                                                    *
 *                                                                         *
 * Purpose : handling of sectors                                           *
 *                                                                         *
 **************************************************************************/

#ifndef RWSECTOR_H
#define RWSECTOR_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "bamateri.h"
#include "bamatlst.h"
#include "bamesh.h"
#include "bageomet.h"

/* RWPUBLIC */

/*
 * Handling atomic sectors
 * Atomic sectors are use to divide up the world into manageable portions
 *
 * Copyright (c) 1998 Criterion Software Ltd.
*/

/****************************************************************************
 Defines
 */

/* Type ID */
#define rpWorldSector 0xff     /* Not a true 'type'! */

#define rpMINDISTANCEBETWEENVERTICES (RwReal)(0.0001)

#define RPV3DFROMVERTEXNORMAL(v, n) \
    (v).x = (((RwReal)((n).x)) * ( (RwReal)(1.0/128))); \
    (v).y = (((RwReal)((n).y)) * ( (RwReal)(1.0/128))); \
    (v).z = (((RwReal)((n).z)) * ( (RwReal)(1.0/128)))

#define RPVERTEXNORMALFROMRWV3D(n, v)           \
    {                                           \
        RwFixed naTmp[3];                       \
                                                \
        naTmp[0] = RwRealToFixed((v).x);        \
        naTmp[1] = RwRealToFixed((v).y);        \
        naTmp[2] = RwRealToFixed((v).z);        \
                                                \
        if (naTmp[0] >= RwIntToFixed(1))        \
        {                                       \
            naTmp[0] = RwIntToFixed(1)-1;       \
        }                                       \
        if (naTmp[0] <= RwIntToFixed(-1))       \
        {                                       \
            naTmp[0] = RwIntToFixed(-1)+1;      \
        }                                       \
        if (naTmp[1] >= RwIntToFixed(1))        \
        {                                       \
            naTmp[1] = RwIntToFixed(1)-1;       \
        }                                       \
        if (naTmp[1] <= RwIntToFixed(-1))       \
        {                                       \
            naTmp[1] = RwIntToFixed(-1)+1;      \
        }                                       \
        if (naTmp[2] >= RwIntToFixed(1))        \
        {                                       \
            naTmp[2] = RwIntToFixed(1)-1;       \
        }                                       \
        if (naTmp[2] <= RwIntToFixed(-1))       \
        {                                       \
            naTmp[2] = RwIntToFixed(-1)+1;      \
        }                                       \
                                                \
        (n).x = (RwInt8)(naTmp[0]>>9);          \
        (n).y = (RwInt8)(naTmp[1]>>9);          \
        (n).z = (RwInt8)(naTmp[2]>>9);          \
    }

/****************************************************************************
 Global types
 */

typedef struct RpVertexNormal RpVertexNormal;

#if (!defined(DOXYGEN))
struct RpVertexNormal
{
    RwInt8          x;
    RwInt8          y;
    RwInt8          z;
    RwUInt8         pad; /* pad character to force alignment */
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpworldsector
 * \struct RpWorldSector
 * World Sector object. This should be
 * considered an opaque type. Use the RpWorldSector API functions to access.
 */
typedef struct RpWorldSector RpWorldSector;

#if (!defined(DOXYGEN))
struct RpWorldSector
{
    RwInt32                 type;

    RpTriangle              *triangles;             /* Triangles */

    RwV3d                   *vertices;              /* Vertex positions */
    RpVertexNormal          *normals;               /* Vertex normals */

    RwTexCoords             *texCoords[rwMAXTEXTURECOORDS]; /* Texture coordinates */

    RwRGBA                  *preLitLum;             /* Pre lit luminances */

    /* Pointer to memory allocated for vertex and poly pointers */
    RwResEntry              *repEntry;

    /* Atomics in this sectors */
    /* The pointers are frigged so they look like they are pointing to
       Atomics when they are pointing to here */
    RwLinkList              collAtomicsInWorldSector;       /* Coll priority */

    /* Lights in an atomic sector */
    RwLinkList              lightsInWorldSector;

    /* Outer bounding box of sector based on BSP planes */
    RwBBox                  boundingBox;

    /* Bounding box tightly enclosing geometry */
    RwBBox                  tightBoundingBox;

    /* The mesh which groups same material triangles together */
    RpMeshHeader            *mesh;

    /* The WorldSector object pipeline for this WorldSector */
    RxPipeline    *pipeline;

    /* Material list window base
     * (triangles in a given sector can "see"
     * the 256 materials from
     * MatList[matListWindowBase] -> MatList[matListWindowBase + 255])
     */
    RwUInt16                matListWindowBase;

    RwUInt16                numVertices;            /* Num vertices */
    RwUInt16                numTriangles;           /* Num triangles */
    RwUInt16                pad;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorCallBack
 * represents the function called from \ref RpWorldForAllWorldSectors,
 * \ref RpAtomicForAllWorldSectors and \ref RpLightForAllWorldSectors for all
 * world sectors in a given world or world sectors a given atomic or light lies
 * in. This function should return a pointer to the current world sector to
 * indicate success. The callback may return NULL to terminate further
 * callbacks on the atomic or light.
 *
 * \param  sector   Pointer to the current world sector
 * \param  data     Pointer to developer-defined data structure.
 *
 * \return Pointer to the current world sector.
 *
 */
typedef RpWorldSector *(*RpWorldSectorCallBack)(RpWorldSector *worldSector, void *data);

typedef struct RpSector RpSector;

#if (!defined(DOXYGEN))
struct RpSector
{
    RwInt32                 type;
};

/* NOTE: The left and right pointers can point to an RpPlaneSector or
 *       an RpWorldSector
 * This is determined what the type is via the type value
 */

typedef struct RpPlaneSector RpPlaneSector;

struct RpPlaneSector
{
    RwInt32                 type;

    RwReal                  value;
    RpSector                *leftSubTree;   /* Sector 'left' (less) of the plane */
    RpSector                *rightSubTree;  /* Sector 'right' (more) of the plane */
    RwReal                  leftValue;
    RwReal                  rightValue;
};
#endif /* (!defined(DOXYGEN)) */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwPluginRegistry sectorTKList;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/****************************************************************************
    macro/inline functionality
 */

#define RpWorldSectorGetBBoxMacro(_sector)              \
    (&((_sector)->boundingBox))

#define RpWorldSectorGetTightBBoxMacro(_sector)         \
    (&((_sector)->tightBoundingBox))

#define RpWorldSectorGetNumVerticesMacro(_sector)       \
    ((_sector)->numVertices)

#define RpWorldSectorGetNumTrianglesMacro(_sector)      \
    ((_sector)->numTriangles)

#define RpWorldSectorGetTrianglesMacro(_sector)         \
    ((_sector)->triangles)

#define RpWorldSectorGetVerticesMacro(_sector)          \
    ((_sector)->vertices)

#if ((!defined(RWDEBUG)) && (!defined(RWSUPPRESSINLINE)))

#define RpWorldSectorGetBBox(_sector)                   \
    RpWorldSectorGetBBoxMacro(_sector)

#define RpWorldSectorGetTightBBox(_sector)              \
    RpWorldSectorGetTightBBoxMacro(_sector)

#define RpWorldSectorGetNumTriangles(_sector)           \
    RpWorldSectorGetNumTrianglesMacro(_sector)

#define RpWorldSectorGetNumVertices(_sector)            \
    RpWorldSectorGetNumVerticesMacro(_sector)

#define RpWorldSectorGetTriangles(_sector)              \
    RpWorldSectorGetTrianglesMacro(_sector)

#define RpWorldSectorGetVertices(_sector)               \
    RpWorldSectorGetVerticesMacro(_sector)

#endif /* ((!defined(RWDEBUG)) && (!defined(RWSUPPRESSINLINE))) */

/* Backward compatibility */
#define RpWorldSectorGetNumPolygons(_sector)            \
    RpWorldSectorGetNumTriangles(_sector)

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */
/* Opening and closing */
extern void *_rpSectorOpen(void *instance, RwInt32 offset, RwInt32 size);
extern void *_rpSectorClose(void *instance, RwInt32 offset, RwInt32 size);

/* RWPUBLIC */

/* Instancing and deinstancing sectors */
extern RpWorldSector * RpWorldSectorRender(RpWorldSector *worldSector);

extern const RpWorldSector *RpWorldSectorForAllMeshes(const RpWorldSector *sector,
                                                      RpMeshCallBack fpCallBack,
                                                      void *pData);


#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

extern RwInt32 RpWorldSectorGetNumTriangles(const RpWorldSector *Sector);
extern RwInt32 RpWorldSectorGetNumVertices(const RpWorldSector *Sector);

extern const RwV3d *
RpWorldSectorGetVertices(const RpWorldSector *sector);

extern const RpTriangle *
RpWorldSectorGetTriangles(const RpWorldSector *sector);

extern const RwBBox *RpWorldSectorGetBBox(const RpWorldSector *sector);
extern const RwBBox *RpWorldSectorGetTightBBox(const RpWorldSector *sector);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* Plugins */
extern RwInt32 RpWorldSectorRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                           RwPluginObjectConstructor constructCB,
                                           RwPluginObjectDestructor destructCB,
                                           RwPluginObjectCopy copyCB);
extern RwInt32 RpWorldSectorRegisterPluginStream(RwUInt32 pluginID,
                                                 RwPluginDataChunkReadCallBack readCB,
                                                 RwPluginDataChunkWriteCallBack writeCB,
                                                 RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwInt32 RpWorldSectorSetStreamAlwaysCallBack(
                    RwUInt32 pluginID,
                    RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwInt32 RpWorldSectorSetStreamRightsCallBack(RwUInt32 pluginID,
                                      RwPluginDataChunkRightsCallBack rightsCB);
extern RwInt32 RpWorldSectorGetPluginOffset(RwUInt32 pluginID);
extern RwBool RpWorldSectorValidatePlugins(const RpWorldSector *sector);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */
/* RWPUBLICEND */

#endif /* RWSECTOR_H */

