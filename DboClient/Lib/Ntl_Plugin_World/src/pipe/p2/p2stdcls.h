/**
 *
 * Types for standard PowerPipe clusters
 *
 * Copyright (c) Criterion Software Limited
 */
/****************************************************************************
 *                                                                          *
 * module : p2stdcls.h                                                      *
 *                                                                          *
 * purpose: declare standard core pipeline clusters and associated types    *
 *                                                                          *
 ****************************************************************************/

#ifndef P2STDCLS_H
#define P2STDCLS_H

/****************************************************************************
 includes
 */

#include "bacolor.h"
#include "batextur.h" /* For RwTexture */
#include "drvmodel.h"
#include "bamatrix.h"

#include "p2core.h"

/****************************************************************************
 global types
 */

/* RWPUBLIC */

/*
 * Current:
 *
 * wait on Simon for instructions to do cluster renaming thing,
 * or go thru and  change all cluster type names and cluster
 * names (add CSL?)
 *
 */

/* CamVerts.csl */

/* clip flags */

/**
 * \ingroup rwcoregeneric
 * \ref RwClipFlag
 * Flags specifying the clipping status of a vertex
 */
enum RwClipFlag
{
    rwXLOCLIP   = 0x01, /**<The vertex is outside the low X clip-plane */
    rwXHICLIP   = 0x02, /**<The vertex is outside the high X clip-plane */
    rwXCLIP     = 0x03, /**<The vertex is outside an X clip-plane */

    rwYLOCLIP   = 0x04, /**<The vertex is outside the low Y clip-plane */
    rwYHICLIP   = 0x08, /**<The vertex is outside the high Z clip-plane */
    rwYCLIP     = 0x0C, /**<The vertex is outside a Y clip-plane */

    rwZLOCLIP   = 0x10, /**<The vertex is outside the low Z clip-plane */
    rwZHICLIP   = 0x20, /**<The vertex is outside the high Z clip-plane */
    rwZCLIP     = 0x30, /**<The vertex is outside a Z clip-plane */

    rwCLIPMASK  = 0x3F, /**<Mask covering all used bits in the clip flags
                         * in case a plugin  wants free ones to use (e.g RpGloss) */

    rwCLIPFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
/*
 * Typedef for RwClipFlag enumeration specifying the clipping status of a vertex
 */
typedef enum RwClipFlag RwClipFlag;



typedef struct RxCamSpace3DVertex RxCamSpace3DVertex;

/**
 * \ingroup rwcoregeneric
 * \struct RxCamSpace3DVertex
 * Structure describing a camera-space 3D vertex.
 */
struct RxCamSpace3DVertex
{
    /* Position in sheared camera-space (pre-projection) */
    RwV3d           cameraVertex; /**< \ref RwV3d camera-space position of the vertex */
    /* Clip flags on this vertex */
    RwUInt8         clipFlags;    /**< Clip flags for the vertex generated during transformation into camera-space, see \ref RwClipFlag */
    RwUInt8         pad[3];       /**< Alignment padding */
    /* Lit color */
    RwRGBAReal      col;          /**< Accumulated \ref RwReal light values (initialized to zero or prelight colors) */
    /* Only used by the clipper */
    RwReal          u;            /**< Texture U coordinate */
    RwReal          v;            /**< Texture V coordinate */
};

/* Supports pipeline1 apps: */
/**
 * \ingroup rwcoregeneric
 * \ref RwCameraVertex
 * typedef for a structure describing a camera-space 3D vertex.
 */
typedef RxCamSpace3DVertex RwCameraVertex;

/* (used during lighting) Doesn't use alpha yet, but it will */
#define RxCamSpace3DVertexSetRGBA(camvert, r, g, b, a)  \
MACRO_START                                             \
{                                                       \
    ((camvert)->col.red = (r));                         \
    ((camvert)->col.green = (g));                       \
    ((camvert)->col.blue = (b));                        \
    ((camvert)->col.alpha = (a));                       \
}                                                       \
MACRO_STOP

#define RxCamSpace3DVertexAddRGBA(camvert, r, g, b, a)  \
MACRO_START                                             \
{                                                       \
    ((camvert)->col.red += (r));                        \
    ((camvert)->col.green += (g));                      \
    ((camvert)->col.blue += (b));                       \
    ((camvert)->col.alpha += (a));                      \
}                                                       \
MACRO_STOP

/* LEGACY-SUPPORT macros */
#define RXCAMSPACE3DVERTEXSetRGBA(camvert, r, g, b, a)  \
    RxCamSpace3DVertexSetRGBA(camvert, r, g, b, a)
#define RXCAMSPACE3DVERTEXAddRGBA(camvert, r, g, b, a)  \
    RxCamSpace3DVertexAddRGBA(camvert, r, g, b, a)

/* MeshState.csl */

/* RWPUBLICEND
 * Geometry type flags, mirroring the flags in rwsdk/world/bageomet.h and
 * baworld.h so we can just copy them across. Hopefully, there'll only be
 * RxGeometryFlags eventually...
 * RWPUBLIC */

/**
 * \ingroup rwcoregeneric
 * \ref RxGeometryFlag
 * Flags describing geometry properties
 */
enum RxGeometryFlag
{
    rxGEOMETRY_TRISTRIP      = 0x01,  /**<This geometry's meshes can be rendered as tri-strips */
    rxGEOMETRY_POSITIONS     = 0x02,  /**<This geometry has positions */
    rxGEOMETRY_TEXTURED      = 0x04,  /**<This geometry has textures applied */
    rxGEOMETRY_PRELIT        = 0x08,  /**<This geometry has luminance values */
    rxGEOMETRY_NORMALS       = 0x10,  /**<This geometry has normals */
    rxGEOMETRY_LIGHT         = 0x20,  /**<This geometry will be lit */
    rxGEOMETRY_MODULATE      = 0x40,  /**<This geometry will modulate the material color with the vertex colors (prelit + lit) */
    rxGEOMETRY_TEXTURED2     = 0x80,  /**<This geometry has 2 set of texture coordinates */

    rxGEOMETRY_COLORED       = 0x100, /**<This mesh specifies per-vertex colors. NB: ONLY USED IN IM3D */

    rxGEOMETRYFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
/*
 * Typedef for RxGeometryFlag enumeration describing geometry properties
 */
typedef enum RxGeometryFlag RxGeometryFlag;

typedef struct RxMeshStateVector RxMeshStateVector;

/**
 * \ingroup rwcoregeneric
 * \struct RxMeshStateVector
 * Structure describing a mesh-state vector,
 * used by the RxClMeshState cluster
 */
struct RxMeshStateVector
{
    RwInt32             Flags;              /**< \ref RxGeometryFlag Flags from the source geometry */
    /* RWPUBLICEND
     * SourceObject is the object which owns the *current* pipeline - in
     * immediate mode it points to a rwIm3DPool struct (an internal structure) and in
     * atomic/worldsector object or material pipelines it points to
     * a RpMaterial (yes I know atomics/worldsectors own instancing
     * pipelines but their pointers can be got from the "(void *)data"
     * passed into every node - a material's pointer can't and we may
     * need to get RpMaterial plugin data)
     * RWPUBLIC */
    void               *SourceObject;       /**< A void pointer. In immediate mode it points to an
                                             * internal structure and in atomic/world-sector object
                                             * or material pipelines it points to an \ref RpMaterial. */
    RwMatrix            Obj2World;          /**< \ref RwMatrix to transform from object-space to world-space */
    RwMatrix            Obj2Cam;            /**< \ref RwMatrix to transform from object-space to camera-space */
    RwSurfaceProperties SurfaceProperties;  /**< \ref RwSurfaceProperties */
    /* We can't necessarily reference an RpMaterial in here (i.e with Im3D),
     * because RpMaterials are defined in RpWorld not RwCore */
    RwTexture          *Texture;            /**< A pointer to a \ref RwTexture */
    RwRGBA              MatCol;             /**< \ref RwRGBA material color */
    RxPipeline         *Pipeline;           /**< A pointer to the material pipeline where appropriate */
    /* rwPRIMTYPETRILIST/TRIFAN/TRISTRIP/LINELIST/POLYLINE */
    RwPrimitiveType     PrimType;           /**< \ref RwPrimitiveType primitive type */
    /* Interpretation based on PrimType */
    RwUInt32            NumElements;        /**< \ref RwUInt32 number of elements (triangles, lines...) */
    RwUInt32            NumVertices;        /**< \ref RwUInt32 number of vertices */
    RwInt32             ClipFlagsOr;        /**< Boolean OR of the \ref RwClipFlag clip flags of all vertices in the mesh */
    RwInt32             ClipFlagsAnd;       /**< Boolean AND of the \ref RwClipFlag clip flags of all vertices in the mesh */
    void               *SourceMesh;         /**< A void pointer to the source \ref RpMesh */
    void               *DataObject;         /**< Mirrors the void data pointer of \ref RxPipelineExecute */
};

/* RxScatter.csl */
typedef struct RxScatter RxScatter;
/**
 * \ingroup rwcoregeneric
 * \struct RxScatter
 * Structure used by the RxClScatter cluster
 */
struct RxScatter
{
    RxPipeline    *pipeline; /**< \ref RxPipeline pointer, causes Scatter.csl
                              * to spit the packet out to the specified pipeline */
    RxPipelineNode *node;    /**< \ref RxPipelineNode pointer, causes Scatter.csl
                              * to send the packet to the specified PipelineNode
                              * (as long as it is actually one of its outputs!) */
};

/* RxInterpolants.csl */
typedef struct RxInterp RxInterp;
/**
 * \ingroup rwcoregeneric
 * \struct RxInterp
 * Structure used by the RxClInterp cluster
 */
struct RxInterp
{
    RxVertexIndex originalVert; /**< \ref RxVertexIndex index to the new vertex generated on the clipped edge */
    RxVertexIndex parentVert1;  /**< \ref RxVertexIndex index to the first vertex of the clipped edge */
    RxVertexIndex parentVert2;  /**< \ref RxVertexIndex index to the second vertex of the clipped edge */
    RwReal        interp;       /**< \ref RwReal interpolant along the clipped edge */
};

/* RxUVs.csl */
typedef struct RxUV RxUV;
/**
 * \ingroup rwcoregeneric
 * \struct RxUV
 * Structure used by the RxClUV cluster
 */
struct RxUV
{
    RwReal u; /**< \ref RwReal U texture coordinate */
    RwReal v; /**< \ref RwReal V texture coordinate */
};

/* RxTriPlanes.csl */
typedef struct RxTriPlane RxTriPlane;

/**
 * \ingroup rwcoregeneric
 * \struct RxTriPlane
 * for the packed plane structure used in RpWorlds and by the RxClTriPlane cluster
 * NOTE: this is currently not used in any nodes that ship with the SDK
 */
struct RxTriPlane
{
    /* Packed plane format used in RpWorlds */
    RwUInt32 N; /**< \ref RwUInt32 Packed normal */
    RwReal   w; /**< \ref RwReal distance from origin */
};

/* RxVSteps.csl */

#if (defined(_MSC_VER))
#  if (_MSC_VER>=1000)
#    pragma pack(push, 1)
#  endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */

typedef struct RxVStep RxVStep;
/* RWPUBLICEND */
/* TODO: RxVStep should just be a typedef, not a struct... */
/* RWPUBLIC */
/**
 * \ingroup rwcoregeneric
 * \struct RxVStep
 * Structure used by the RxClVStep cluster.
 * To use the step values in the RxClVStep cluster, start at the beginning
 * of the RxVStep and vertex arrays and proceed as follows: (a) Process one
 * vertex, (b) Skip 'step' vertices, (c) Increment the cursor of the RxClVStep
 * cluster. Repeat (a)-(c) until the entire vertex array has been processed.
 * If the RxVStep array contains valid data, you should not have to bounds-check
 * its cursor.
 */
struct RxVStep
{
    RwUInt8 step; /**< \ref RwUInt8 The number of vertices after the current one
                   * which can be skipped in lighting and other calculations because,
                   * for example, they belong only to back-facing triangles */
};

#if (defined(_MSC_VER))
#  if (_MSC_VER>=1000)
#    pragma pack(pop)
#  endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */

/* CamNorms.csl */
/**
 * \ingroup rwcoregeneric
 * \ref RxCamNorm
 * typedef for \ref RwV3d used by the RxClVStep cluster */
typedef RwV3d RxCamNorm;

/* RWPUBLICEND */

/****************************************************************************
 global prototypes
 */

/* RWPUBLIC */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Uses the RxObjSpace3DVertex type (see pipmodel.h) */
extern RxClusterDefinition RxClObjSpace3DVertices;
/* Uses the RxCamSpace3DVertex type */
extern RxClusterDefinition RxClCamSpace3DVertices;
/* Uses the RxScrSpace2DVertex type (see pipmodel.h) */
extern RxClusterDefinition RxClScrSpace2DVertices;
/* Uses the RxInterp type */
extern RxClusterDefinition RxClInterpolants;
/* Uses the RxMeshStateVector type */
extern RxClusterDefinition RxClMeshState;
/* Uses the RxRenderStateVector type (p2renderstate.c/h) */
extern RxClusterDefinition RxClRenderState;
/* Uses the RxVertexIndex type */
extern RxClusterDefinition RxClIndices;
/* Uses the RxScatter type */
extern RxClusterDefinition RxClScatter;
/* Uses the RxUV type */
extern RxClusterDefinition RxClUVs;
/* Uses the RxVStep type */
extern RxClusterDefinition RxClVSteps;
/* Uses the RwRGBAReal type */
extern RxClusterDefinition RxClRGBAs;
/* Uses the RxCamNorm type */
extern RxClusterDefinition RxClCamNorms;

/* Uses the RxTriPlane type.
 * NOTE: this is currently not used in any nodes that ship with the SDK */
extern RxClusterDefinition RxClTriPlanes;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* P2STDCLS_H */

