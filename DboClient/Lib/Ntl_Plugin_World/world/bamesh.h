/***************************************************************************
 *                                                                         *
 * Module  : bamesh.h                                                      *
 *                                                                         *
 * Purpose : Mesh handling functions.                                      *
 *                                                                         *
 **************************************************************************/

#ifndef RWMESH_H
#define RWMESH_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "bamateri.h"
#include "bamatlst.h"

/****************************************************************************
 Global defines
 */

/* RWPUBLIC */

/*
 *
 * Purpose: Provide construction and enumeration facilities for meshes.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

#define RPMESHGLOBAL(var)                                   \
    (RWPLUGINOFFSET(rpMeshGlobals,                          \
                    RwEngineInstance,                       \
                    meshModule.globalsOffset)->var)

#define rwPRIMTYPEOR                            \
    (rwPRIMTYPELINELIST |                       \
      rwPRIMTYPEPOLYLINE |                      \
      rwPRIMTYPETRILIST |                       \
      rwPRIMTYPETRISTRIP |                      \
      rwPRIMTYPETRIFAN   |                      \
      rwPRIMTYPEPOINTLIST)

#define rpMESHHEADERPRIMTYPEOR                  \
    (0 /* rpMESHHEADERTRILIST*/ |               \
      rpMESHHEADERTRISTRIP |                    \
      rpMESHHEADERTRIFAN  |                     \
      rpMESHHEADERLINELIST |                    \
      rpMESHHEADERPOLYLINE |                    \
      rpMESHHEADERPOINTLIST)

/****************************************************************************
 Global variables
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwModuleInfo meshModule;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/****************************************************************************
 Global types
 */

/*
 * typedef for header structure listing all meshes
 * constituting a single RpGeometry or RpWorldSector
 */
typedef struct RpMeshHeader RpMeshHeader;

/**
 * \ingroup rpmesh
 * \ref RpMeshHeaderFlags
 * represents the different types of mesh.
 * \see RpMeshHeader
 */
enum RpMeshHeaderFlags
{
    /* NOTE: trilists are denoted by absence of any other
     *       primtype flags, so be careful that you test:
     *        (triListFlag == (flags&triListFlag))
     *       or:
     *        (0 == (flags&rpMESHHEADERPRIMMASK))
     *       and not:
     *        (flags&triListFlag)
     */
    rpMESHHEADERTRISTRIP  = 0x0001, /**< Render as tristrips */
    rpMESHHEADERTRIFAN    = 0x0002, /**< On PlayStation 2 these will be converted to trilists */
    rpMESHHEADERLINELIST  = 0x0004, /**< Render as linelists */
    rpMESHHEADERPOLYLINE  = 0x0008, /**< On PlayStation 2 these will be converted to linelists */
    rpMESHHEADERPOINTLIST = 0x0010, /**< Pointlists are supported only if rendered by
                                     *   custom pipelines; there is no default RenderWare
                                     *   way to render pointlists. */

    rpMESHHEADERPRIMMASK  = 0x00FF, /**< All bits reserved for specifying primitive type */
    rpMESHHEADERUNINDEXED = 0x0100, /**< Topology is defined implicitly by vertex
                                     *   order, ergo the mesh contains no indices */
    rpMESHHEADERFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*
 * Typedef for RpMeshHeaderFlags enumeration
 * representing the different types of mesh
 */
typedef enum RpMeshHeaderFlags RpMeshHeaderFlags;

typedef struct rpMeshGlobals rpMeshGlobals;

#if (!defined(DOXYGEN))
struct rpMeshGlobals
{
    RwInt16             nextSerialNum;
    RwFreeList         *triStripListEntryFreeList;
    RwUInt8             meshFlagsToPrimType[rpMESHHEADERPRIMTYPEOR];
    RwUInt8             primTypeToMeshFlags[rwPRIMTYPEOR];
};
#endif /* (!defined(DOXYGEN)) */

typedef struct RpBuildMeshTriangle RpBuildMeshTriangle;

/**
 * \ingroup rpmesh
 * \struct RpBuildMeshTriangle
 * This type represents an array of indices into
 * the object vertex array. Used during the construction
 * of tristrips.
 *
 * See API functions
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * and
 * \see RpMeshSetTriStripMethod
 * \see RpMeshGetTriStripMethod
 */
struct RpBuildMeshTriangle
{
    RwUInt16            vertIndex[3]; /**< indices into object vertex
                                       *   array. */
    RpMaterial         *material;     /**< pointer to material used to
                                       *   render the mesh. */
    RwUInt16            matIndex;     /**< unique index for material used */
    RwUInt16            textureIndex; /**< unique index for texture used */
    RwUInt16            rasterIndex;  /**< unique index for raster used */
    RwUInt16            pipelineIndex;/**< unique index for pipeline used */
};

typedef struct RpBuildMesh RpBuildMesh;

/**
 * \ingroup rpmesh
 * \struct RpBuildMesh
 * This type represents a mesh ready for tri stripping.
 *
 * See API functions
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * and
 * \see RpMeshSetTriStripMethod
 * \see RpMeshGetTriStripMethod
 */
struct RpBuildMesh
{
    RwUInt32            triangleBufferSize; /**< number of triangles
                                             *   space has been allocated
                                             *   for. */
    RwUInt32            numTriangles;       /**< number of triangles to be
                                             *   tristripped. */
    RpBuildMeshTriangle *meshTriangles;     /**< pointer to build mesh
                                             *   triangles. */
};

typedef struct RpMesh RpMesh;

/**
 * \ingroup rpmesh
 * \struct RpMesh
 * This type represents a single polygon mesh.
 * A mesh is defined as a collection of triangles derived from an RpGeometry
 * or RpWorldSector which have a common material.
 *
 * See API functions \see RpGeometryForAllMeshes and
 * \see RpWorldSectorForAllMeshes and
 * the corresponding function callback types:
 */
struct RpMesh
{
    RxVertexIndex      *indices;    /**< vertex indices defining the mesh */
    RwUInt32            numIndices; /**< number of vertices in mesh */
    RpMaterial         *material;   /**< pointer to material used to
                                     *   render the mesh. */
};

/**
 * \ingroup rpmesh
 * \struct RpMeshHeader
 * Header for all meshes that constitute a single RpGeometry or RpWorldSector
 */
struct RpMeshHeader
{
    RwUInt32            flags;    /**< \see RpMeshHeaderFlags */
    RwUInt16            numMeshes; /**< Number of meshes in object */
    RwUInt16            serialNum; /**< Determine if mesh has changed
                                    * since last instance */
    RwUInt32            totalIndicesInMesh; /**< Total triangle index
                                             * count in all meshes
                                             */
    RwUInt32            firstMeshOffset; /**< Offset in bytes from end this
                                          * structure RpMeshHeader
                                          * to the first mesh
                                          */
};

/**
 * \ingroup rpmesh
 * \ref RpMeshCallBack is the callback
 * function supplied to \ref RpGeometryForAllMeshes and
 * \ref RpWorldSectorForAllMeshes for all meshes in a given geometry.
 *
 * This function should return a pointer to the current mesh to indicate
 * success. The callback may return NULL to terminate further callbacks
 * on the meshes.
 *
 * \param  mesh        Pointer to the current mesh, supplied by
 *                     iterator.
 * \param  meshHeader  Pointer to the meshes header
 * \param  data        Pointer to developer-defined data structure.
 *
 * \return Returns a pointer to the current mesh if successful or NULL 
 * if an error occurred.
 */
typedef RpMesh     *(*RpMeshCallBack) (RpMesh * mesh,
                                       RpMeshHeader * meshHeader,
                                       void *pData);

/****************************************************************************
 Function prototypes
 */

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpMeshHeaderGetPrimType(_mshHdr)                                        \
    ( (RwPrimitiveType)RPMESHGLOBAL(meshFlagsToPrimType)[(_mshHdr)->flags &     \
                                                         rpMESHHEADERPRIMMASK] )

#define RpMeshHeaderSetPrimType(_mshHdr, _prmTyp)                   \
    ( (_mshHdr)->flags =                                            \
          ((_mshHdr)->flags & ~rpMESHHEADERPRIMMASK) |              \
          (rpMESHHEADERPRIMMASK &                                   \
           RPMESHGLOBAL(primTypeToMeshFlags)[(_prmTyp) &            \
                                           rpMESHHEADERPRIMMASK]),  \
      (_mshHdr) )

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Opening and closing module */
extern void        *_rpMeshOpen(void *instance, RwInt32 offset,
                                RwInt32 size);
extern void        *_rpMeshClose(void *instance, RwInt32 offset,
                                 RwInt32 size);

extern RwInt16      _rpMeshGetNextSerialNumber(void);

/* Create a build mesh with nothing in */
extern RpBuildMesh *_rpBuildMeshCreate(RwUInt32 bufferSize);

/* Destroy a build mesh */
extern RwBool       _rpBuildMeshDestroy(RpBuildMesh * mesh);

/* Destroy a build mesh */
extern RwBool       _rpMeshDestroy(RpMeshHeader * mesh);

/* Add a triangle to a mesh */
extern RpBuildMesh *_rpBuildMeshAddTriangle(RpBuildMesh * mesh,
                                            RpMaterial * material,
                                            RwInt32 vert1,
                                            RwInt32 vert2,
                                            RwInt32 vert3,
                                            RwUInt16 matIndex,
                                            RwUInt16 textureIndex,
                                            RwUInt16 rasterIndex,
                                            RwUInt16 pipelineIndex);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/* Get primtype from a mesh header */
extern RwPrimitiveType RpMeshHeaderGetPrimType(RpMeshHeader *
                                               meshHeader);

/* Set primtype for a mesh header */
extern RpMeshHeader *RpMeshHeaderSetPrimType(RpMeshHeader *
                                             meshHeader,
                                             RwPrimitiveType
                                             primType);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* Enumerate meshes within a mesh header */
extern RpMeshHeader *_rpMeshHeaderForAllMeshes(RpMeshHeader *
                                               meshHeader,
                                               RpMeshCallBack
                                               fpCallBack,
                                               void *pData);

/* Mesh serialisation functions */
extern RwStream    *_rpMeshWrite(const RpMeshHeader * meshHeader,
                                 const void *object,
                                 RwStream * stream,
                                 const RpMaterialList * matList);
extern RpMeshHeader *_rpMeshRead(RwStream * stream,
                                 const void *object,
                                 const RpMaterialList * matList);
extern RwInt32      _rpMeshSize(const RpMeshHeader *meshHeader,
                                const void *object);
/* Mesh header create/destroy functions */
extern void          _rpMeshHeaderDestroy(RpMeshHeader * meshHeader);
extern RpMeshHeader * _rpMeshHeaderCreate(RwUInt32 size);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif                          /* RWMESH_H */
