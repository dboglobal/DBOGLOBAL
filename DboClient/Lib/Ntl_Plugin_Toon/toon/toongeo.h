/*****************************************************************************

    File: toongeo.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef TOONGEO_H
#define TOONGEO_H

/*****************************************************************************
 Includes
 */
#include "rwcore.h"
#include "rpworld.h"

#include "toonink.h"
#include "toonpaint.h"
#include "toongeo.h"

/* RWPUBLIC */
/*****************************************************************************
 Defines
 */
#define RPTOONGEOMETRYGETDATA(geometryInstance)         \
     ((RpToonGeo **)                                    \
      (((RwUInt8 *)geometryInstance) +                  \
       _rpToonGlobals.geometryOffset))

#define RPTOONGEOMETRYGETCONSTDATA(geometryInstance)    \
     ((const RpToonGeo * const *)                       \
      (((const RwUInt8 *)geometryInstance) +            \
       _rpToonGlobals.geometryOffset))

#define RPTOONSECTORGETDATA(geometryInstance)           \
     ((RpToonGeo **)                                    \
      (((RwUInt8 *)geometryInstance) +                  \
       _rpToonGlobals.sectorOffset))

#define RPTOONSECTORGETCONSTDATA(geometryInstance)      \
     ((const RpToonGeo * const *)                       \
      (((const RwUInt8 *)geometryInstance) +            \
       _rpToonGlobals.sectorOffset))

#define RPTOONEDGE_NONEIGHBOURFACE  ((RwUInt16)0xffff)
#define noNextEdge                  ((RwUInt16)0xffff)

/*****************************************************************************
 Enums
 */
/**
 * \ingroup rptoongeo
 * \ref RpToonInkType
 *
 * Describes how an \ref RpToonInk is interpreted by an \ref RpToonEdge.
 *
 */
enum RpToonInkType
{
    RPTOON_INKTYPE_SILHOUETTE = 0,  /**< Edge will use this ink only when it is a silhouette edge */
    RPTOON_INKTYPE_CREASE,          /**< Edge will become a crease edge and use this ink */
    RPTOON_INKTYPE_FORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*****************************************************************************
 Typedefs
 */
/*
UInt16 are ever so slightly faster (less cache thrashing I guess) and save quite a bit of space.
However this limits us to 65535 edges, so roughly (num triangles + num vertices) must be less than that.
*/
typedef RwUInt16    rpToonEdgeIndex;
typedef RwUInt16    rpToonVertexIndex;
typedef RwUInt16    rpToonFaceIndex;
typedef RwUInt8     rpToonGeoInkIndex;

/*****************************************************************************
 Typedef Enums
 */
typedef enum RpToonInkType RpToonInkType;

/*****************************************************************************
 Typedef Structs
 */
typedef struct rpToonEdgeInkID  rpToonEdgeInkID;

/**
 * \ingroup rptoongeo
 * \struct RpToonEdge
 *
 * Describes a toon edge.
 *
 */
typedef struct RpToonEdge       RpToonEdge;

typedef struct rpToonTriangle   rpToonTriangle;

/**
 * \ingroup rptoon
 * \struct RpToonGeo
 *
 * Geometry attributes required for toon rendering
 * (edges, connectivity information, etc) are stored here.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonGeo RpToonGeo;

/*****************************************************************************
 Function Pointers
 */
/**
 * \ingroup rptoongeo
 * \ref RpToonGeoForAllInkIDsCallBack
 * represents the function called from \ref RpToonGeoForAllInkIDs
 * for all ink IDs in a given \ref RpToonGeo. This function should
 * return the current ink ID to indicate success. The callback may return
 * NULL to terminate further callbacks on the \ref RpToonGeo.
 *
 * \param  inkId  The inks name.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current inkId or NULL
 *
 * \see RpToonGeoForAllInkIDs
 */
typedef const char *(RpToonGeoForAllInkIDsCallBack)(const char *inkId, void *data);

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoForAllPaintIDs
 * represents the function called from \ref RpToonGeoForAllPaintIDs
 * for all paint IDs in a given \ref RpToonGeo. This function should
 * return the current paint ID to indicate success. The callback may return
 * NULL to terminate further callbacks on the \ref RpToonGeo.
 *
 * \param  paintId  The paints name.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current paintId or NULL
 *
 * \see RpToonGeoForAllPaintIDs
 */
typedef const char *(RpToonGeoForAllPaintIDsCallBack)(const char *paintId, void *data);

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct rpToonEdgeInkID
{
    rpToonGeoInkIndex   inkId[2];
};
#endif /* (!defined(DOXYGEN)) */

struct RpToonEdge
{
    RwUInt16    v[2];           /**< index of vertex end points */
    RwUInt16    face[2];        /**< index of faces straddled, RPTOONEDGE_NONEIGHBOURFACE if one is missing */
    RwUInt16    edgefv[2][2];   /**< Internal use */
};

#if (!defined(DOXYGEN))
struct rpToonTriangle
{
    rpToonVertexIndex   vertIndex[3];   /* vertex indices */
};

/*
 * Extend geometries to hold info for silhoutte determination/tracking
 * (face normals, edge connectivity, previous silhoutte) vertex weighting for
 * lines user defined edges associations of inks to edges.
 */
struct RpToonGeo
{
    /* Summary of data from world sector/geometry so we don't need two sets of code to deal with it...  */
    RwInt32             numVerts;
    RwInt32             numTriangles;
    rpToonTriangle *    triangles; /* might be able to ditch this if we fixed rp world sector or did a nasty hack */

    /* object this thing came from so we can find materials easily */
    RpGeometry    *     parentGeometry;
    RpWorldSector *     parentWorldSector;

    /* stored render callbacks for chaining */
    RpAtomicCallBackRender      defAtomicRenderCallback;
    RpWorldSectorCallBackRender defSectorRenderCallback;

    /*
    Evil hackery to allow D3D8 (and others perhaps?) to pick up skinned vertex positions for line drawing
    Set to TRUE by skintoon.lib pipeline attach, left FALSE by default.
    */
    RwBool              isSkinned;

    /*
    i.e. face normals will need to be recomputed since someone's messed with them and they're letting us know.
    Seems kind of dorky but I can't really think of a better way right now...
    */
    RwBool              vertexPositionsDirty;
    
    /* count of unique edges determine from mesh */
    RwInt32             numEdges;

    /* count of hard edges (determined from smoothing groups or user assigned) */
    RwInt32             numCreaseEdges;

    /*
    Edge connectivity data used by exporters & generic renderer
    All crease edges are stored at the front of the array so we can find them easily
    and check whether an edge is a crease edge or not without storing extra flags
    */
    RpToonEdge *        edges;

    /* one weight for each vertex in model to affect thickness of lines crossing it */
    RwReal *            vertexThicknesses;

    /* map ink names used on edges in this geometry to ink IDs (and Inks) - this bit's streamed */
    RwInt32             inkIDCount;
    rpToonInkID *       inkIDList;

    /* instances of inks matched to inkIDList above */
    RpToonInk **        inks;

    /* Ink IDs for each edge in geometry - streamed */
    rpToonEdgeInkID *   edgeInkIDs;

    /* so we can scream at exporter if it messes around with edges without optimizing them after */
    RwBool              edgeInkIDsOptimized;

    /* name of paint to use for whole object if materials don't specify otherwise - this is streamed */
    rpToonPaintID       defaultPaintID;

    /* instance of paint */
    RpToonPaint *       paint;

    /*
    rpToonGeoPlatformData platformData;
     */

    /* generic edge renderer stuff, should be removed for other platforms */

    /* store these for checking front/back facing silhoutte edge test */
    RwV3d *             faceNormals;

    /*
    store these for direction to extrude silhoutte edges along,
    can look nicer than ordinary vertex normals
    */
    RwV3d *             extrusionVertexNormals;
};
#endif /* (!defined(DOXYGEN)) */
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/* RWPUBLICEND */

/*****************************************************************************
 Global Variables
 */

/* RWPUBLIC */
/*****************************************************************************
 Function prototypes
 */
/* RWPUBLICEND */

extern void _rpToonUpdateWorldPerspectiveScale(RpToonGeo *geo);
extern void _rpToonUpdateAtomicPerspectiveScale(RpToonGeo *geo, RpAtomic *atomic);

/* RWPUBLIC */
extern RpToonGeo *_rpToonGeoInit(RwInt32 numVerts, RwInt32 numTriangles);
/* RWPUBLICEND */
extern RpToonGeo *_rpToonGeoDestroy(RpToonGeo *toonGeo);

#if defined(PLATFORM_NATIVE)
extern RwInt32 _rpToonGeoNativeSize(const RpToonGeo *toonGeo);
extern RwStream *_rpToonGeoNativeWrite(RwStream *stream, const RpToonGeo *toonGeo);
extern RwStream *_rpToonGeoNativeRead(RwStream *stream, RpToonGeo **toonGeo);
#endif /* defined(PLATFORM_NATIVE) */

extern RwInt32 _rpToonGeoSize(const RpToonGeo *toonGeo);
extern RwStream *_rpToonGeoRead(RwStream *stream, RpToonGeo **toonGeoPtr);
extern RwStream *_rpToonGeoWrite(RwStream *stream, const RpToonGeo *toonGeo);

/* RWPUBLIC */
/* INK */
/* Get & Set Silhouette Ink */
extern RpToonInk *RpToonGeoGetSilhouetteInk(const RpToonGeo *toonGeo);
extern void RpToonGeoSetSilhouetteInk(RpToonGeo *toonGeo, RpToonInk *ink);

/* Get & Set Crease Ink */
extern RpToonInk *RpToonGeoGetCreaseInk(const RpToonGeo *toonGeo);
extern void RpToonGeoSetCreaseInk(RpToonGeo *toonGeo, RpToonInk *ink);

/* For all Ink IDs */
extern void
RpToonGeoForAllInkIDs(const RpToonGeo *toonGeo,
                      RpToonGeoForAllInkIDsCallBack *forAllInkIDsCallBack,
                      void *data);

/* Apply Inks in Ink dictionary */
extern void
RpToonGeoApplyInksInInkDictionary(RpToonGeo *toonGeo,
                                  const RpToonInkDictionary *inkDictionary);



/* PAINT */
/* Get & Set Paint */
extern RpToonPaint *
RpToonGeoGetPaint(RpToonGeo *toonGeo);
extern void
RpToonGeoSetPaint(RpToonGeo *toonGeo,
                  RpToonPaint *paint);

/* For all Paint IDs */
extern void RpToonGeoForAllPaintIDs(const RpToonGeo *toonGeo,
                                    RpToonGeoForAllPaintIDsCallBack *forAllPaintIDsCallBack,
                                    void *data);

/* Apply Paints in Paint dictionary */
extern void
RpToonGeoApplyPaintsInPaintDictionary(RpToonGeo *toonGeo,
                                      const RpToonPaintDictionary *paintDictionary);

extern void
RpToonGeoNotifyVertexPositionsDirty(RpToonGeo *toonGeo);

/* */
/* Get Vertex Thinkness */
extern const RwReal *
RpToonGeoGetVertexThicknesses(RpToonGeo *toonGeo);

/* Lock & Unlock Vertex Thickness */
extern RwReal *RpToonGeoLockVertexThicknesses(RpToonGeo *toonGeo);
extern void RpToonGeoUnlockVertexThicknesses(RpToonGeo *toonGeo);
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* TOONGEO_H */
