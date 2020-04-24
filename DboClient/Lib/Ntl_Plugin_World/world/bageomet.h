
/***************************************************************************
 *                                                                         *
 * Module  : bageomet.h                                                    *
 *                                                                         *
 * Purpose : Geometry handling                                             *
 *                                                                         *
 **************************************************************************/

#ifndef RWGEOMET_H
#define RWGEOMET_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "bamateri.h"
#include "bamatlst.h"
#include "bamesh.h"

/* RWPUBLIC */

/*
 * Handling atomic's geometry
 * Geometry describe objects, and are the building blocks for atomics
 *
 * Copyright (c) 1998 Criterion Software Ltd.
*/


/****************************************************************************
 Defines
 */

/* Type ID */
#define rpGEOMETRY 8

/**
 * \ingroup rpgeometry
 * RpGeometryFlag
 * Geometry type flags 
 *
 * When creating a geometry, these flags can be OR'ed together to
 * specify the format along with the rpGEOMETRYTEXCOORDSETS(n) macro if more
 * than two sets of texture coordinates are required. See \ref RpGeometryCreate
 * for more details.
 *
 * \see RpGeometryCreate().
 */
enum RpGeometryFlag
{
    rpGEOMETRYTRISTRIP  = 0x00000001, /**<This geometry's meshes can be
                                          rendered as strips.
                                          \ref RpMeshSetTriStripMethod is
                                          used to change this method.*/
    rpGEOMETRYPOSITIONS = 0x00000002, /**<This geometry has positions */  
    rpGEOMETRYTEXTURED  = 0x00000004, /**<This geometry has only one set of
                                          texture coordinates. Texture
                                          coordinates are specified on a per
                                          vertex basis */
    rpGEOMETRYPRELIT    = 0x00000008, /**<This geometry has pre-light colors */
    rpGEOMETRYNORMALS   = 0x00000010, /**<This geometry has vertex normals */
    rpGEOMETRYLIGHT     = 0x00000020, /**<This geometry will be lit */
    rpGEOMETRYMODULATEMATERIALCOLOR = 0x00000040, /**<Modulate material color 
                                                      with vertex colors
                                                      (pre-lit + lit) */

    rpGEOMETRYTEXTURED2 = 0x00000080, /**<This geometry has at least 2 sets of
                                          texture coordinates. */

    /*
     * These above flags were stored in the flags field in an RwObject, they
     * are now stored in the flags file of the RpGeometry.
     */

    rpGEOMETRYNATIVE            = 0x01000000,
    rpGEOMETRYNATIVEINSTANCE    = 0x02000000,

    rpGEOMETRYFLAGSMASK         = 0x000000FF,
    rpGEOMETRYNATIVEFLAGSMASK   = 0x0F000000,

    rpGEOMETRYFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpGeometryFlag RpGeometryFlag;

/* Multi texture coordinate format specifier for RpGeometryCreate() */
#define rpGEOMETRYTEXCOORDSETS(_num) ((_num & 0xff) << 16)

/**
 * \ingroup rpgeometry
 * RpGeometryLockMode
 * Geometry lock flags 
 */
enum RpGeometryLockMode
{
    rpGEOMETRYLOCKPOLYGONS   =   0x01, /**<Lock the polygons (triangle list) */
    rpGEOMETRYLOCKVERTICES   =   0x02, /**<Lock the vertex positional data */
    rpGEOMETRYLOCKNORMALS    =   0x04, /**<Lock the vertex normal data */
    rpGEOMETRYLOCKPRELIGHT   =   0x08, /**<Lock the pre-light values */
    rpGEOMETRYLOCKTEXCOORDS  =   0x10, /**<Lock the texture coordinates set 1*/
    rpGEOMETRYLOCKTEXCOORDS1 =   0x10, /**<Lock the texture coordinates set 1*/
    rpGEOMETRYLOCKTEXCOORDS2 =   0x20, /**<Lock the texture coordinates set 2*/
    rpGEOMETRYLOCKTEXCOORDS3 =   0x40, /**<Lock the texture coordinates set 3*/
    rpGEOMETRYLOCKTEXCOORDS4 =   0x80, /**<Lock the texture coordinates set 4*/
    rpGEOMETRYLOCKTEXCOORDS5 = 0x0100, /**<Lock the texture coordinates set 5*/
    rpGEOMETRYLOCKTEXCOORDS6 = 0x0200, /**<Lock the texture coordinates set 6*/
    rpGEOMETRYLOCKTEXCOORDS7 = 0x0400, /**<Lock the texture coordinates set 7*/
    rpGEOMETRYLOCKTEXCOORDS8 = 0x0800, /**<Lock the texture coordinates set 8*/
    rpGEOMETRYLOCKTEXCOORDSALL = 0x0ff0, /**<Lock all texture coordinate sets*/
    rpGEOMETRYLOCKALL        = 0x0fff, /**<Combination of all the above */

    rpGEOMETRYLOCKMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpGeometryLockMode RpGeometryLockMode;

/* Convenience macro for generating a tex coord lock flag */
#define rpGEOMETRYLOCKTEXCOORDSIDX(_idx) (rpGEOMETRYLOCKTEXCOORDS1 << (_idx))


/****************************************************************************
 Global Types
 */

typedef struct rpGeometryGlobals rpGeometryGlobals;

#if (!defined(DOXYGEN))
struct rpGeometryGlobals
{
        RwFreeList         *geomFreeList;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpgeometry
 * \struct RpGeometry 
 * Geometry object. This should be considered an opaque type. 
 * Use the RpGeometry API functions to access.
 */
typedef struct RpGeometry RpGeometry;

/**
 * \ingroup rpmorphtarget
 * \struct RpMorphTarget 
 * Morph target -- vertex positions and normals. 
 * This should be considered an opaque type. 
 * Use RpMorphTarget API functions to access.
 */
typedef struct RpMorphTarget RpMorphTarget;

#if (!defined(DOXYGEN))
struct RpMorphTarget
{
        RpGeometry *parentGeom; 
        RwSphere   boundingSphere;
        RwV3d      *verts;
        RwV3d      *normals;
};
#endif /* (!defined(DOXYGEN)) */

typedef struct RpTriangle RpTriangle;

/**
 * \ingroup rpgeomtry
 * \struct RpTriangle 
 * This type represents a triangle in a geometry specified 
 * by three indices into the geometry's vertex list (vertIndex)
 * together with an  index in to the geometry's 
 * material list (matIndex) (see API function \ref RpGeometryGetTriangles)
 */
struct RpTriangle
{
    RwUInt16            vertIndex[3];   /**< vertex indices */
    RwUInt16            matIndex;       /**< Index into material list */
};

#if (!defined(DOXYGEN))
struct RpGeometry
{
    RwObject            object;     /* Generic type */

    RwUInt32            flags;      /* Geometry flags */

    RwUInt16            lockedSinceLastInst; /* What has been locked since we last instanced - for re-instancing */
    RwInt16             refCount;   /* Reference count (for keeping track of atomics referencing geometry) */

    RwInt32             numTriangles; /* Quantity of various things (polys, verts and morph targets) */
    RwInt32             numVertices;
    RwInt32             numMorphTargets;
    RwInt32             numTexCoordSets;

    RpMaterialList      matList;

    RpTriangle         *triangles;  /* The triangles */

    RwRGBA             *preLitLum;  /* The pre-lighting values */

    RwTexCoords        *texCoords[rwMAXTEXTURECOORDS]; /* Texture coordinates */

    RpMeshHeader       *mesh;   /* The mesh - groups polys of the same material */

    RwResEntry         *repEntry;       /* Information for an instance */

    RpMorphTarget      *morphTarget;    /* The Morph Target */
};

typedef struct RpGeometryChunkInfo RpGeometryChunkInfo;
typedef struct RpGeometryChunkInfo _rpGeometry;

struct RpGeometryChunkInfo
{
    RwInt32             format;  /* Compression flags and number of texture coord sets */

    RwInt32             numTriangles;
    RwInt32             numVertices;

    RwInt32             numMorphTargets;
};
#endif /* (!defined(DOXYGEN)) */

/* Callbacks */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryCallBack represents the simple callback function for the \ref RpGeometry object.
 * The callback may return NULL to terminate further callbacks on 
 * the geometry.
 * 
 * \param  geometry   Pointer to the current geometry, supplied by iterator.
 * \param  data       Pointer to developer-defined data structure.
 *
 * \return Pointer to the current geometry, or NULL If not found.
 */
typedef RpGeometry *(*RpGeometryCallBack)(RpGeometry *geometry, void *data);

/**
 * \ingroup rpgeometry
 * \ref RpGeometrySortByMaterialCallBack 
 * \ref RpGeometrySortByMaterialCallBack is used by
 * \ref RpGeometrySortByMaterial in order to preserve the validity of plugin
 * data when the vertices in an \ref RpGeometry are sorted (and some possibly
 * duplicated).
 *
 * A vertex map table is provided to the callback - this contains, for each
 * vertex in the new geometry, the index of the corresponding vertex in the
 * original geometry.
 *
 * \param  oldGeom           Pointer to the source geometry.
 * \param  newGeom           Pointer to the new, sorted geometry.
 * \param  remapTable        Pointer to the vertex map table
 * \param  numberOfEntries   Number of vertices in the new 
 *                           geometry (size of the map table)
 */
typedef void (*RpGeometrySortByMaterialCallBack)(const RpGeometry *oldGeom,
                                                 RpGeometry *newGeom,
                                                 RwUInt16 *remapTable,
                                                 RwUInt16 numberOfEntries);


/****************************************************************************
 <macro/inline functionality
 */

#define RpMorphTargetSetBoundingSphereMacro(_mt, _sphere)       \
    (RwSphereAssign(&((_mt)->boundingSphere), (_sphere)), (_mt))

#define RpMorphTargetGetBoundingSphereMacro(_mt)                \
    (&((_mt)->boundingSphere))

#define RpGeometryGetNumMorphTargetsMacro(_geometry)            \
    ((_geometry)->numMorphTargets)

#define RpGeometryGetMorphTargetMacro(_geometry, _index)        \
    (&((_geometry)->morphTarget[(_index)]))

#define RpGeometryGetPreLightColorsMacro(_geometry)             \
    ((_geometry)->preLitLum)

#define RpGeometryGetVertexTexCoordsMacro(_geometry, _uvIndex)    \
    ((_geometry)->texCoords[(_uvIndex) - 1])

#define RpGeometryGetNumTexCoordSetsMacro(_geometry)            \
    ((_geometry)->numTexCoordSets)

#define RpGeometryGetNumVerticesMacro(_geometry)                \
    ((_geometry)->numVertices)

#define RpMorphTargetGetVerticesMacro(_mt)                      \
    ((_mt)->verts)

#define RpMorphTargetGetVertexNormalsMacro(_mt)                 \
    ((_mt)->normals)

#define RpGeometryGetTrianglesMacro(_geometry)                  \
    ((_geometry)->triangles)

#define RpGeometryGetNumTrianglesMacro(_geometry)               \
    ((_geometry)->numTriangles)

#define RpGeometryGetMaterialMacro(_geometry, _num)             \
    (((_geometry)->matList.materials)[(_num)])

#define RpGeometryGetNumMaterialsMacro(_geometry)               \
    ((_geometry)->matList.numMaterials)

#define RpGeometryGetFlagsMacro(_geometry)                      \
    ((_geometry)->flags)

#define RpGeometrySetFlagsMacro(_geometry, _flags)              \
    (((_geometry)->flags = (_flags)), (_geometry))


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpMorphTargetSetBoundingSphere(_geometry, _sphere)      \
    RpMorphTargetSetBoundingSphereMacro(_geometry, _sphere)

#define RpMorphTargetGetBoundingSphere(_geometry)               \
    RpMorphTargetGetBoundingSphereMacro(_geometry)

#define RpGeometryGetNumMorphTargets(_geometry)                 \
    RpGeometryGetNumMorphTargetsMacro(_geometry)

#define RpGeometryGetMorphTarget(_geometry, _index)             \
    RpGeometryGetMorphTargetMacro(_geometry, _index)

#define RpGeometryGetPreLightColors(_geometry)                  \
    RpGeometryGetPreLightColorsMacro(_geometry)

#define RpGeometryGetVertexTexCoords(_geometry, _uvIndex)       \
    RpGeometryGetVertexTexCoordsMacro(_geometry, _uvIndex)

#define RpGeometryGetNumTexCoordSets(_geometry)                 \
    RpGeometryGetNumTexCoordSetsMacro(_geometry)

#define RpGeometryGetNumVertices(_geometry)                     \
    RpGeometryGetNumVerticesMacro(_geometry)

#define RpMorphTargetGetVertices(_mt)                           \
    RpMorphTargetGetVerticesMacro(_mt)

#define RpMorphTargetGetVertexNormals(_mt)                      \
    RpMorphTargetGetVertexNormalsMacro(_mt)

#define RpGeometryGetTriangles(_geometry)                       \
    RpGeometryGetTrianglesMacro(_geometry)

#define RpGeometryGetNumTriangles(_geometry)                    \
    RpGeometryGetNumTrianglesMacro(_geometry)

#define RpGeometryGetMaterial(_geometry, _num)                  \
    RpGeometryGetMaterialMacro(_geometry, _num)

#define RpGeometryGetNumMaterials(_geometry)                    \
    RpGeometryGetNumMaterialsMacro(_geometry)

#define RpGeometryGetFlags(_geometry)                           \
    RpGeometryGetFlagsMacro(_geometry)

#define RpGeometrySetFlags(_geometry, _flags)                   \
    RpGeometrySetFlagsMacro(_geometry, _flags)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Opening and closing the geometry */

extern void *
_rpGeometryClose(void *instance,
                 RwInt32 offset,
                 RwInt32 size);

extern void *
_rpGeometryOpen(void *instance,
                RwInt32 offset,
                RwInt32 size);

extern RwPluginRegEntry *
_rpGeometryGetTKListFirstRegEntry(void);

/* RWPUBLIC */

/* Transforms geometry morph target vertices */

extern RpGeometry  *
RpGeometryTransform(RpGeometry *geometry,
                    const RwMatrix *matrix);

/* Create geometry for a 'space' marker */

extern RpGeometry  *
RpGeometryCreateSpace(RwReal radius);

/* Morph targets - Accessing geometry contents */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

extern RpMorphTarget  *
RpMorphTargetSetBoundingSphere(RpMorphTarget *morphTarget,
                               const RwSphere *boundingSphere);

extern RwSphere  *
RpMorphTargetGetBoundingSphere(RpMorphTarget *morphTarget);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern const RpMorphTarget  *
RpMorphTargetCalcBoundingSphere(const RpMorphTarget *morphTarget,
                                RwSphere *boundingSphere);

extern RwInt32
RpGeometryAddMorphTargets(RpGeometry *geometry,
                          RwInt32 mtcount);

extern RwInt32
RpGeometryAddMorphTarget(RpGeometry *geometry);

extern RpGeometry  *
RpGeometryRemoveMorphTarget(RpGeometry *geometry,
                            RwInt32 morphTarget);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RwInt32
RpGeometryGetNumMorphTargets(const RpGeometry *geometry);

extern RpMorphTarget  *
RpGeometryGetMorphTarget(const RpGeometry *geometry,
                         RwInt32 morphTarget);

extern RwRGBA  *
RpGeometryGetPreLightColors(const RpGeometry *geometry);

extern RwTexCoords  *
RpGeometryGetVertexTexCoords(const RpGeometry *geometry,
                             RwTextureCoordinateIndex uvIndex);

extern RwInt32
RpGeometryGetNumTexCoordSets(const RpGeometry *geometry);

extern RwInt32
RpGeometryGetNumVertices (const RpGeometry *geometry);

extern RwV3d  *
RpMorphTargetGetVertices(const RpMorphTarget *morphTarget);

extern RwV3d  *
RpMorphTargetGetVertexNormals(const RpMorphTarget *morphTarget);

extern RpTriangle  *
RpGeometryGetTriangles(const RpGeometry *geometry);

extern RwInt32
RpGeometryGetNumTriangles(const RpGeometry *geometry);

extern RpMaterial  *
RpGeometryGetMaterial(const RpGeometry *geometry,
                      RwInt32 matNum);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern const RpGeometry  *
RpGeometryTriangleSetVertexIndices(const RpGeometry *geometry,
                                   RpTriangle *triangle,
                                   RwUInt16 vert1,
                                   RwUInt16 vert2,
                                   RwUInt16 vert3);

extern RpGeometry  *
RpGeometryTriangleSetMaterial(RpGeometry *geometry,
                              RpTriangle *triangle,
                              RpMaterial *material);

extern const RpGeometry  *
RpGeometryTriangleGetVertexIndices(const RpGeometry *geometry,
                                   const RpTriangle *triangle,
                                   RwUInt16 *vert1,
                                   RwUInt16 *vert2,
                                   RwUInt16 *vert3);

extern RpMaterial   *
RpGeometryTriangleGetMaterial(const RpGeometry *geometry,
                              const RpTriangle *triangle);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RwInt32
RpGeometryGetNumMaterials(const RpGeometry *geometry);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern RpGeometry  *
RpGeometryForAllMaterials(RpGeometry *geometry,
                          RpMaterialCallBack fpCallBack,
                          void *pData);

/* Accessing the inards of geometry */
extern RpGeometry  *
RpGeometryLock(RpGeometry *geometry,
               RwInt32 lockMode);

extern RpGeometry  *
RpGeometryUnlock(RpGeometry *geometry);

extern const RpGeometry  *
RpGeometryForAllMeshes(const RpGeometry *geometry,
                       RpMeshCallBack fpCallBack,
                       void *pData);

/* Creation and destruction */
extern RpGeometry  *
RpGeometryCreate(RwInt32 numVert,
                 RwInt32 numTriangles,
                 RwUInt32 format);

extern RwBool
RpGeometryDestroy(RpGeometry *geometry);

extern RpGeometry *
RpGeometryAddRef(RpGeometry *geometry);

/* Attaching toolkits */
extern RwInt32
RpGeometryRegisterPlugin(RwInt32 size,
                         RwUInt32 pluginID,
                         RwPluginObjectConstructor constructCB,
                         RwPluginObjectDestructor destructCB,
                         RwPluginObjectCopy copyCB);

extern RwInt32
RpGeometryRegisterPluginStream(RwUInt32 pluginID,
                               RwPluginDataChunkReadCallBack readCB,
                               RwPluginDataChunkWriteCallBack writeCB,
                               RwPluginDataChunkGetSizeCallBack getSizeCB);

extern RwInt32
RpGeometrySetStreamAlwaysCallBack(RwUInt32 pluginID,
                                  RwPluginDataChunkAlwaysCallBack alwaysCB);

extern RwInt32
RpGeometryGetPluginOffset(RwUInt32 pluginID);

extern RwBool
RpGeometryValidatePlugins(const RpGeometry *geometry);

/* Binary format */
extern RwUInt32
RpGeometryStreamGetSize(const RpGeometry *geometry);

extern const RpGeometry  *
RpGeometryStreamWrite(const RpGeometry *geometry,
                      RwStream *stream);

extern RpGeometry  *
RpGeometryStreamRead(RwStream *stream);

extern RpGeometryChunkInfo *
_rpGeometryChunkInfoRead(RwStream *stream,
                         RpGeometryChunkInfo *geometryChunkInfo,
                         RwInt32 *bytesRead);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
/* Flags */
extern RwUInt32
RpGeometryGetFlags(const RpGeometry *geometry);

extern RpGeometry  *
RpGeometrySetFlags(RpGeometry *geometry,
                   RwUInt32 flags);

#endif

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define rpGeometryAddRef(_geometry) \
    RpGeometryAddRef(_geometry) 

#define RpGeometryChunkInfoRead(stream, geometryChunkInfo, bytesRead) \
       _rpGeometryChunkInfoRead(stream, geometryChunkInfo, bytesRead)

/* RWPUBLICEND */

#endif                          /* RWGEOMET_H */

