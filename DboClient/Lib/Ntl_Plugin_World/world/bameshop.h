/***************************************************************************
 *                                                                         *
 * Module  : bameshop.h                                                    *
 *                                                                         *
 * Purpose : Mesh optimisation handling functions.                         *
 *                                                                         *
 **************************************************************************/

#ifndef RWMESHOP_H
#define RWMESHOP_H

/****************************************************************************
 Includes
 */

#include "bamesh.h"  /* Pick up RpMeshHeader/meshModule etc */

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#define RPTRISTRIPPOLYGONMAXEDGES 3

/**
 * \ingroup rpmesh
 * \ref RpTriStripMeshCallBack is the callback to generate triangle strips
 * when the triangle stripped geometries or world sectors are unlocked.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 *                  generated from.
 * \param data      pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 */
typedef RpMeshHeader *
(*RpTriStripMeshCallBack) (RpBuildMesh *buildMesh, void *data);



/****************************************************************************
 Global types
 */

typedef struct RpTriStripEdge RpTriStripEdge;

typedef struct RpTriStripPolygon RpTriStripPolygon;
/**
 * \ingroup rpmesh
 * \struct RpTriStripPolygon
 * This type represents a polygon.
 *
 * \see RpTriStripPolygonFollowStrip
 * \see RpBuildMeshGenerateTriStrip
 * \see RpTriStripMeshTunnel
 */
struct RpTriStripPolygon
{
    RwLLLink inEndLink;     /**< link for list of polygons joined by fewer than
                                 2 strip edges */
    RwLLLink inUsedLink;    /**< link for list of used polygons */
    RwLLLink inFreeLink;    /**< link for list of free polygons */
    RwUInt32 numEdges;      /**< number of edges */
    RpTriStripEdge *edges[RPTRISTRIPPOLYGONMAXEDGES];   /**< polygon edges */
    RxVertexIndex vertIndex[RPTRISTRIPPOLYGONMAXEDGES]; /**< polygon vertex 
                                                             indices */
    RwUInt32 testFrame;     /**< used to prevent a tri-strip being counted by
                                 the cost function more than once */
};

/**
 * \ingroup rpmesh
 * \struct RpTriStripEdge
 * This type represents a polygon edge which may be part of a tri-strip.
 *
 * \see RpTriStripPolygonFollowStrip
 * \see RpBuildMeshGenerateTriStrip
 * \see RpTriStripMeshTunnel
 */
struct RpTriStripEdge
{
    RwLLLink inUsedLink;        /**< link for list of used edges */      
    RwLLLink inFreeLink;        /**< link for list of free edges */
    RpTriStripPolygon *poly1;   /**< first polygon including this edge */
    RpTriStripPolygon *poly2;   /**< second polygon including this edge or NULL
                                     if this edge is only included in one */
    RxVertexIndex vert1;        /**< index of first vertex of edge */
    RxVertexIndex vert2;        /**< index of second vertex of edge */
    RwBool strip;               /**< TRUE if the edge is part of a tri-strip */
};

typedef struct RpTriStripMesh RpTriStripMesh;
/**
 * \ingroup rpmesh
 * \struct RpTriStripMesh
 * This type represents a mesh and the tri-strips it contains.
 *
 * \see RpBuildMeshGenerateTriStrip
 * \see RpTriStripMeshTunnel
 */
struct RpTriStripMesh
{
    RwUInt32 numPolygons;       /**< the number of polygons in the mesh */
    RwLinkList polygonEndList;  /**< linked list of polygons joined by fewer 
                                     than 2 strip edges. */
    RwLinkList polygonUsedList; /**< linked list of used polygons */
    RwLinkList polygonFreeList; /**< linked list of free polygons */
    RwLinkList edgeUsedList;    /**< linked list of used edges */
    RwLinkList edgeFreeList;    /**< linked list of free edges */
};

/**
 * \ingroup rpmesh
 * \ref RpTriStripMeshStripCallBack represents the callback function to
 * tri-strip a mesh.
 * 
 * \param  mesh Pointer to the mesh to tri-strip.
 * \param  data Pointer to user-supplied data.
 *
 * \return Returns a pointer to the current mesh if successful or NULL if an
 *         error occurred. 
 */
typedef RpTriStripMesh *
(*RpTriStripMeshStripCallBack) (RpTriStripMesh *mesh, void *data);

typedef struct RpTriStripData RpTriStripData;
/**
 * \ingroup rpmesh
 * \struct RpTriStripData
 * This type represents the data required by the 
 * \ref RpBuildMeshGenerateTriStrip function to tri-strip a mesh. This data must
 * be the user-supplied data passed into RpMeshSetTriStripMethod when using
 * \ref RpBuildMeshGenerateTriStrip.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 * \see RpTriStripMeshTunnel
 */
struct RpTriStripData
{
    RwBool ignoreWinding;                /**< TRUE to ignore winding order */
    RpTriStripMeshStripCallBack stripCB; /**< callback to tri-strip mesh */
    void *data;                          /**< data to supply to callback */
};

/**
 * \ingroup rpmesh
 * \ref RpTriStripCostCallBack represents the callback function used by the
 * tunnelling tri-stripper to determine the cost of a tri-strip.
 * 
 * \param  startPolygon Pointer to polygon at the start of the tri-strip.
 * \param  testFrame    Value that must be written to the testFrame of all 
 *                      polygons in the tri-strip
 * \param  data         Pointer to user-supplied data.
 *
 * \return Returns the cost of the tri-strip.
 */
typedef RwUInt32
(*RpTriStripCostCallBack) (RpTriStripPolygon *startPolygon, RwUInt32 testFrame, void *data);

typedef struct RpTriStripTunnelData RpTriStripTunnelData;
/**
 * \ingroup rpmesh
 * \struct RpTriStripTunnelData
 * This type represents the data required by the 
 * \ref RpTriStripMeshTunnel function to tri-strip a mesh. This data must
 * be the user-supplied data in the RpTriStripData structure passed into
 * \ref RpMeshSetTriStripMethod as the user-supplied data when using
 * \ref RpTriStripMeshTunnel.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 * \see RpTriStripMeshTunnel
 * \see RpTriStripDefaultCost
 */
struct RpTriStripTunnelData
{
    RwReal quality; /**< Values between 0.1 and 0.5 tend to be fairly quick but
        don't always give the best results. Values greater than 0.5 tend to 
        give good results useful for final artwork but can take much longer.
        Large values e.g. 5 allow the length limit to be the limiting factor
        instead. */
    RwUInt32 lengthLimit; /**< The maximum tunnel length to use. Values between
        10 and 50 tend to be fairly quick but don't always give the best
        results. Values greater than 50 tend to give good results for final
        artwork but can take much longer. Large values e.g. 10000 allow the
        quality to be the limiting factor. */
    RpTriStripCostCallBack costCB; /**< callback to determine tri-strip cost */
    void *data;                    /**< data to supply to callbacks */
};


/* RWPUBLICEND */
typedef struct RpTriStripTunnel RpTriStripTunnel;
struct RpTriStripTunnel
{
    RwUInt32 length;              /* number of edges in tunnel */
    RwUInt32 maxLength;           /* maximum length of tunnel */
    RpTriStripEdge **edges;
    RpTriStripPolygon **polygons;
};
/* RWPUBLIC */

/****************************************************************************
 Global Variables 
 */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Tri-strip manipulating functions */
extern RpTriStripPolygon *
RpTriStripPolygonFollowStrip(RpTriStripPolygon *curr, RpTriStripPolygon *prev);

/* Callback strip costing functions */
extern RwUInt32
RpTriStripDefaultCost(RpTriStripPolygon *startPolygon, RwUInt32 testFrame, 
                      void *data);

/* Callback mesh stripping functions */
extern RpTriStripMesh *
RpTriStripMeshTunnel(RpTriStripMesh *mesh, void *data);

extern RpTriStripMesh *
RpTriStripMeshQuick(RpTriStripMesh *mesh, void *data);

/* New callback mesh generating function */
extern RpMeshHeader *
RpBuildMeshGenerateTriStrip(RpBuildMesh *buildMesh, void *data);

/* Old callback mesh generating functions. Probably obsolete */
extern RpMeshHeader *
RpBuildMeshGenerateTrivialTriStrip(RpBuildMesh *buildMesh, void *data);

extern RpMeshHeader *
RpBuildMeshGenerateDefaultTriStrip(RpBuildMesh *buildmesh, void *data);

extern RpMeshHeader *
RpBuildMeshGeneratePreprocessTriStrip(RpBuildMesh *buildmesh, void *data);

extern RpMeshHeader *
RpBuildMeshGenerateExhaustiveTriStrip(RpBuildMesh *buildmesh, void *data);

extern RpMeshHeader *
RpBuildMeshGenerateDefaultIgnoreWindingTriStrip(RpBuildMesh *buildmesh, 
                                                void *data);

extern RpMeshHeader *
RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip(RpBuildMesh *buildmesh, 
                                                   void *data);

extern RpMeshHeader *
RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip(RpBuildMesh *buildmesh, 
                                                   void *data);

/* Functions to set and get the global mesh tri-strip algorithm */
extern RwBool
RpMeshSetTriStripMethod(RpTriStripMeshCallBack callback, void *data);

extern RwBool
RpMeshGetTriStripMethod(RpTriStripMeshCallBack *callback, void **data);


extern RpMeshHeader *
_rpTriListMeshGenerate(RpBuildMesh *buildMesh, void *data);

/* 
 * Optimise the mesh ordering 
 * (sort on material and place transparent materials last)
 */
extern RpMeshHeader *
_rpMeshOptimise(RpBuildMesh *buildmesh, RwUInt32 flags);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#define _rpTriStripMeshTrivialGenerate(_buildMesh, _data)      \
    RpBuildMeshGenerateTrivialTriStrip(_buildMesh, _data)

#define _rpTriStripMeshDefaultGenerate(_buildmesh, _data)      \
    RpBuildMeshGenerateDefaultTriStrip(_buildmesh, _data)

#define _rpTriStripMeshPreprocessGenerate(_buildmesh, _data)   \
    RpBuildMeshGeneratePreprocessTriStrip(_buildmesh, _data)

#define _rpTriStripMeshExhaustiveGenerate(_buildmesh, _data)   \
    RpBuildMeshGenerateExhaustiveTriStrip(_buildmesh, _data)

#define _rpMeshSetTristripMethod(_callback, _data)             \
    RpMeshSetTriStripMethod(_callback, _data)

#define _rpMeshGetTristripMethod(_callback, _data)             \
    RpMeshGetTriStripMethod(_callback, _data)


/* RWPUBLICEND */


#endif /* RWMESHOP_H */
