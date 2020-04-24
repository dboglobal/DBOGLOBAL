#ifndef RPTOON_H
#define RPTOON_H

/**
 * \defgroup rptoon RpToon
 * \ingroup fxpack
 *
 * Toon Plugin for RenderWare.
 */

#include "rwcore.h"
#include "rpworld.h"

/*---- start: ./toonink.h----*/
/**
 * \ingroup rptoon
 * \struct RpToonInk
 *
 * Attributes describing how to draw the edges of toon objects are stored here.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonInk RpToonInk;

/**
 * \ingroup rptoon
 * \struct RpToonInkDictionary
 *
 * Collections of RpToonInks can be stored here and searched
 * for by name.  Behavior and functionality is similar to that
 * of \ref RwTexDictionary.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonInkDictionary RpToonInkDictionary;

typedef struct rpToonInkID      rpToonInkID;
/*****************************************************************************
 Function Pointers
 */
/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkCallBack
 * represents the function called from \ref RpToonInkDictionaryForAllInks
 * for all inks in a given paint dictionary. This function should
 * return the current ink to indicate success. The callback may return
 * NULL to terminate further callbacks on the ink dictionary.
 *
 * \param  ink   Pointer to the current ink.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current ink or NULL
 *
 * \see RpToonInkDictionaryForAllInks
 */
typedef RpToonInk * (RpToonInkCallBack)(RpToonInk *ink, void *data);

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct rpToonInkID
{
    RwChar  name[rwTEXTUREBASENAMELENGTH];
};

struct RpToonInk /* groups of edges point to one - per material? per atomic? */
{
    RwInt32 refCount;

    /* ---------- persistent stuff that's streamed -------------------------------- */

    RwChar name[rwTEXTUREBASENAMELENGTH];

    RwReal overallThickness;        /**/
    RwRGBA color;

    RwReal perspectiveScale;        /* vertices at front of object are scaled by perspectiveScale, back by 1 */

    RwReal farScale;                /* as bound sphere center approaches far clip, it's scaled by farScale */

    /*
    Other interesting features might be textures, exterior color,
    blending modes, stroke based properties... 
    */

    /* ---------- temporaries used during the rendering process ------------------- */

    /* stores factored coefficients for perspective scale effect */
    RwReal perspectiveScale_a, perspectiveScale_b, perspectiveScale_zMin;

    RwReal farScale_factor;
    RwReal farScale_creasefactor;
};
#endif /* (!defined(DOXYGEN)) */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/*****************************************************************************
 Function prototypes
 */
/* INK */
/* Create & Destroy */
extern RpToonInk *RpToonInkCreate(void);
extern void RpToonInkDestroy(RpToonInk *ink);

/* Add ref */
extern void RpToonInkAddRef(RpToonInk *ink);

/* Read & Write */
extern RpToonInk *RpToonInkStreamRead(RwStream *stream);
extern const RpToonInk *RpToonInkStreamWrite(const RpToonInk *ink,
                                             RwStream *stream);

/* Get & Set Thickness */
extern RwReal RpToonInkGetOverallThickness(const RpToonInk *ink);
extern void RpToonInkSetOverallThickness(RpToonInk *ink,
                                         RwReal thickness);

/* Get & Set Color */
extern RwRGBA RpToonInkGetColor(const RpToonInk *ink);
extern void RpToonInkSetColor(RpToonInk *ink, RwRGBA color);

/* Get & Set Name */
extern const RwChar *RpToonInkGetName(const RpToonInk *ink);
extern void RpToonInkSetName(RpToonInk *ink, const RwChar *name);

/* Get & Set Perspective Scale */
extern RwReal RpToonInkGetPerspectiveScale(const RpToonInk *ink);
extern void RpToonInkSetPerspectiveScale(RpToonInk *ink,
                                         RwReal perspectiveScale);

/* Get & Set Far Scale */
extern RwReal RpToonInkGetFarScale(const RpToonInk *ink);
extern void RpToonInkSetFarScale(RpToonInk *ink, RwReal farScale);

/* INK DICTIONARY */
/* Create & Destroy */
extern RpToonInkDictionary *RpToonInkDictionaryCreate(void);
extern void RpToonInkDictionaryDestroy(RpToonInkDictionary *inkDictionary);

/* Add & Remove Ink */
extern RpToonInkDictionary *
RpToonInkDictionaryAddInk(RpToonInkDictionary *inkDictionary,
                          RpToonInk *ink);
extern void
RpToonInkDictionaryRemoveInk(RpToonInkDictionary *inkDictionary,
                             RpToonInk *ink);

/* Find Named */
extern RpToonInk *
RpToonInkDictionaryFindNamedInk(const RpToonInkDictionary *inkDictionary,
                                const RwChar *name);

/* For all Inks */
extern void
RpToonInkDictionaryForAllInks(const RpToonInkDictionary *inkDictionary,
                              RpToonInkCallBack *inkCB,
                              void *data);

/* Read & Write */
extern RpToonInkDictionary *
RpToonInkDictionaryStreamRead(RwStream *stream);
extern const RpToonInkDictionary *
RpToonInkDictionaryStreamWrite(const RpToonInkDictionary *inkDictionary,
                               RwStream *stream);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/*---- end: ./toonink.h----*/
/*---- start: ./toonpaint.h----*/
/*****************************************************************************
 Enums
 */
/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintType defines the different ways a toon paint can
 * be rendered. Once a paint has been created, it's type can be changed
 * with \ref RpToonPaintSetType and queried with \ref RpToonPaintGetType.
 */
enum RpToonPaintType
{
    RPTOON_PAINTTYPE_FLAT = 0,      /**<Flat shading, material and texture color is fully bright */
    RPTOON_PAINTTYPE_TOONSHADE,     /**<Toon shading, material and texture color blended against shading texture */
    RPTOON_PAINTTYPE_COUNT,         /**<Count of paint types */
    RPTOON_PAINTTYPE_FORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*****************************************************************************
 Typedef Enums
 */
typedef enum RpToonPaintType    RpToonPaintType;

/*****************************************************************************
 Typedef Structs
 */
typedef struct rpToonPaintID    rpToonPaintID;

/**
 * \ingroup rptoon
 * \struct RpToonPaint
 *
 * Attributes describing how to draw the fill/shading
 * of toon objects are stored here.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonPaint RpToonPaint;

/**
 * \ingroup rptoon
 * \struct RpToonPaintDictionary
 *
 * Collections of RpToonPaints can be stored here and searched
 * for by name.  Behavior and functionality is similar to that
 * of \ref RwTexDictionary.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonPaintDictionary RpToonPaintDictionary;

/*****************************************************************************
 Function Pointers
 */
/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintCallBack
 * represents the function called from \ref RpToonPaintDictionaryForAllPaints
 * for all paints in a given paint dictionary. This function should
 * return the current paint to indicate success. The callback may return
 * NULL to terminate further callbacks on the paint dictionary.
 *
 * \param  paint   Pointer to the current texture.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current paint or NULL
 *
 * \see RpToonPaintDictionaryForAllPaints
 */
typedef RpToonPaint * (RpToonPaintCallBack)(RpToonPaint *paint, void *data);

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct rpToonPaintID
{
    RwChar  name[rwTEXTUREBASENAMELENGTH];
};

struct RpToonPaint /* groups of triangles point to one. Store per material? per atomic? per light??? */
{
    RwInt32 refCount;

    RpToonPaintType type;

    RwChar name[rwTEXTUREBASENAMELENGTH];

    /* texture gradient - diffuse */
    RwTexture *gradient;

    /*
    Other interesting features might be specular reflection, god knows what...
    keep an open mind.
    */
};
#endif /* (!defined(DOXYGEN)) */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/*****************************************************************************
 Function prototypes
 */
/* PAINT */
/* Create & Destroy */
extern RpToonPaint * RpToonPaintCreate(void);
extern void RpToonPaintDestroy(RpToonPaint *paint);

/* Add ref */
extern void RpToonPaintAddRef(RpToonPaint *paint);

/* Get & Set Type */
extern RpToonPaintType RpToonPaintGetType(const RpToonPaint *paint);
extern void RpToonPaintSetType(RpToonPaint *paint, RpToonPaintType type);

/* Get & Set Name */
extern const RwChar *RpToonPaintGetName(const RpToonPaint *paint);
extern void RpToonPaintSetName(RpToonPaint *paint, const RwChar *name);

/* Get & Set Gradient Texture */
extern RwTexture *RpToonPaintGetGradientTexture(const RpToonPaint *paint);
extern void RpToonPaintSetGradientTexture(RpToonPaint *paint, RwTexture *texture);

/* Read & Write */
extern RpToonPaint *RpToonPaintStreamRead(RwStream *stream);
extern const RpToonPaint *RpToonPaintStreamWrite(const RpToonPaint *paint, RwStream *stream);


/* PAINT DICTIONARY */
/* Create & Destroy */
extern RpToonPaintDictionary *RpToonPaintDictionaryCreate(void);
extern void RpToonPaintDictionaryDestroy(RpToonPaintDictionary *paintDictionary);

/* Add & Remove Paint */
extern RpToonPaintDictionary *
RpToonPaintDictionaryAddPaint(RpToonPaintDictionary *paintDictionary,
                              RpToonPaint *paint);
extern void
RpToonPaintDictionaryRemovePaint(RpToonPaintDictionary *paintDictionary,
                                 RpToonPaint *paint);

/* Find named */
extern RpToonPaint *
RpToonPaintDictionaryFindNamedPaint(const RpToonPaintDictionary *paintDictionary,
                                    const RwChar *name);

/* For all Paints */
extern void
RpToonPaintDictionaryForAllPaints(const RpToonPaintDictionary *paintDictionary,
                                  RpToonPaintCallBack *paintCB,
                                  void *data);

/* Read & Write */
extern RpToonPaintDictionary *
RpToonPaintDictionaryStreamRead(RwStream *stream);
extern const RpToonPaintDictionary *
RpToonPaintDictionaryStreamWrite(const RpToonPaintDictionary *paintDictionary,
                                 RwStream *stream);
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/*---- end: ./toonpaint.h----*/
/*---- start: ./toonmaterial.h----*/
/*****************************************************************************
 Defines
 */
#define RPTOONMATERIALGETDATA(materialInstance)         \
     ((RpToonMaterial **)                               \
      (((RwUInt8 *)materialInstance) +                  \
       _rpToonGlobals.materialOffset))

#define RPTOONMATERIALGETCONSTDATA(materialInstance)    \
     ((const RpToonMaterial * const *)                  \
      (((const RwUInt8 *)materialInstance) +            \
       _rpToonGlobals.materialOffset))

/*****************************************************************************
 Enums
 */

/*****************************************************************************
 Typedef Enums
 */

/*****************************************************************************
 Typedef Structs
 */
/**
 * \ingroup rptoonmaterial
 * \struct RpToonMaterial
 *
 * RpToon keeps a \ref RpToonPaint name here to identify paints associated with a material
 * for streaming.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonMaterial RpToonMaterial;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct RpToonMaterial
{
    RwBool      overrideGeometryPaint;
    RwChar      paintName[rwTEXTUREBASENAMELENGTH];
    RpToonPaint *paint;
    RwInt32     silhouetteInkID;
};
#endif /* (!defined(DOXYGEN)) */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/*****************************************************************************
 Global Variables
 */

/*****************************************************************************
 Function prototypes
 */
extern RpToonMaterial *RpToonMaterialCreate(void);
extern void RpToonMaterialDestroy(RpToonMaterial *mat);

extern void RpToonMaterialSetPaintID(RpMaterial *mat, const RwChar *name);
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/*---- end: ./toonmaterial.h----*/
/*---- start: ./toongeo.h----*/
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
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/*****************************************************************************
 Function prototypes
 */
extern RpToonGeo *_rpToonGeoInit(RwInt32 numVerts, RwInt32 numTriangles);
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
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/*---- end: ./toongeo.h----*/
/*---- start: ./toon.h----*/
/*****************************************************************************
 Enums
 */
enum RpToonType
{
    rpNATOONTYPE        = 0,
    rpTOONTYPEGENERIC   = 1,
    /* rpTOONTYPESKIN      = 2, */
    rpTOONTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*****************************************************************************
 Typedef Enums
 */
typedef enum RpToonType RpToonType;

/*****************************************************************************
 Typedef Structs
 */
typedef struct ToonGlobals  ToonGlobals;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct ToonGlobals
{
    /* Offsets */
    RwInt32         engineOffset;
    RwInt32         atomicOffset;
    RwInt32         geometryOffset;
    RwInt32         sectorOffset;
    RwInt32         materialOffset;

    /* Plugin data */
    RwFreeList      *freeList;
    RwModuleInfo    module;

    /* Platform global data */
};
#endif /* (!defined(DOXYGEN)) */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/*****************************************************************************
 Global Variables
 */
extern ToonGlobals  _rpToonGlobals;

/*****************************************************************************
 Function prototypes
 */
/* Z bias */
extern void RpToonSetCreaseZBias(RwReal zBias);
extern RwReal RpToonGetCreaseZBias(void);

/* Create */
extern RpToonGeo *
RpToonGeometryCreateToonGeo(RpGeometry *geometry,
                            RwBool generateCreaseEdgesFromSmoothingGroups);
RpToonGeo *
RpToonWorldSectorCreateToonGeo(RpWorldSector *sector,
                               RwBool generateCreaseEdgesFromSmoothingGroups);

/* Enable */
extern RwBool
RpToonAtomicEnable(RpAtomic *atomic);
extern RwBool
RpToonWorldSectorEnable(RpWorldSector *sector);

/* Get */
extern RpToonGeo *
RpToonGeometryGetToonGeo(RpGeometry *geometry);
extern RpToonGeo *
RpToonWorldSectorGetToonGeo(RpWorldSector *sector);

/* Plugin */
extern RwBool
RpToonPluginAttach(void);
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/*---- end: ./toon.h----*/

/* Platform specific */
/*---- start: c:/RW/Graphics/rwsdk/plugin/toon/d3d9/toonplatform.h----*/
#ifndef TOONPLATFORM_H
#define TOONPLATFORM_H
/*****************************************************************************
 Function prototypes
 */

#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern RpAtomic *
RpD3D9ToonFastSilhouetteAtomicRenderCallback(RpAtomic *atomic);

extern RpWorldSector *
RpD3D9ToonFastSilhouetteWorldSectorRenderCallback(RpWorldSector *sector);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
#endif /* TOONPLATFORM_H */
/*---- end: c:/RW/Graphics/rwsdk/plugin/toon/d3d9/toonplatform.h----*/

#endif /* RPTOON_H */

