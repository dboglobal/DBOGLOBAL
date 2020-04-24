/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctdata.h
 *  Purpose: Collision partition tree data management
 */

#ifndef CTDATA_H
#define CTDATA_H

/* RWPUBLIC */

/******************************************************************************
 *  Defines
 */

#define rpCOLLTREE_MAXDEPTH                 32

#define rpCOLLSPLIT_ALIGNMENT               16

/* Allow 16 special contents values */
#define rpCOLLSECTOR_CONTENTS_MAXCOUNT      0xef
#define rpCOLLSECTOR_CONTENTS_SPLIT         0xff

#define rpCOLLSECTOR_TYPE_NEG               0x01
#define rpCOLLSECTOR_TYPE_AXISMASK          (0x08 | 0x04)
#define rpCOLLSECTOR_TYPE_X                 0x00
#define rpCOLLSECTOR_TYPE_Y                 0x04
#define rpCOLLSECTOR_TYPE_Z                 0x08

/******************************************************************************
 *  Types
 */

/**
 * \ingroup rpcollis
 * \struct  RpCollSector
 * This opaque type represents a node of an \ref RpCollTree, and corresponds
 * to a region of space within an axis aligned bounding box. A branch sector
 * references a pair of overlapping sub-sectors, and a leaf sector references 
 * any entries that are contained within the region.
 */
typedef struct RpCollSector RpCollSector;


/**
 * \ingroup rpcollis
 * \struct  RpCollTree
 * This opaque type represents a collision tree, which uses overlapping
 * sectors to recursively divide a set of 'entries' into smaller groups.
 * The entries would normally be the triangles of a world 
 * sector or geometry, but in principle could be anything contained by a 
 * bounding box (hence the name 'entries' rather than 'triangles').
 *
 * The external entries may be sorted according to their location in the 
 * collision tree, such that any node of the tree corresponds to a contiguous
 * block of the ordered set. Alternatively, a remapping array is stored with 
 * the tree (see \ref RpCollTreeGetEntryMap). The leaf nodes of the tree 
 * reference the external entries that they contain by specifying a slice of
 * the ordered array.
 *
 * Functions are available for iterating over a collision tree to find all
 * leaf sectors that intersect a given line or bounding box.
 *
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionWorldSectorGetCollTree
 * \see RpCollisionGeometryGetCollTree
 * \see RpCollSectorLineItInit
 * \see RpCollSectorBBoxItInit
 * \see \ref rpcollisoverview
 */
typedef struct RpCollTree RpCollTree;


#if !defined(DOXYGEN)

enum RpCollTreeFlags
{
    rpCOLLTREE_USEMAP        = 0x01,

    rpCOLLTREEFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpCollTreeFlags RpCollTreeFlags;

struct RpCollSector
{
    RwUInt8         type;
    RwUInt8         contents;
    RwUInt16        index;
    RwReal          value;
};

typedef struct RpCollSplit RpCollSplit;
struct RpCollSplit
{
    RpCollSector    leftSector;
    RpCollSector    rightSector;
};

struct RpCollTree
{
    RwUInt32        flags;
    RwBBox          bbox;
    RwUInt16        numEntries;
    RwUInt16        numSplits;
    RpCollSplit    *splits;
    RwUInt16       *map;
};

#endif /* !defined(DOXYGEN) */


/******************************************************************************
 *  Macros
 */

#define RpCollTreeGetEntryMapMacro(_tree)  ((_tree)->map)

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
#define RpCollTreeGetEntryMap(_tree)  RpCollTreeGetEntryMapMacro(_tree)
#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/******************************************************************************
 *  Functions
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RpCollTree *
_rpCollTreeCreate(RwUInt16 numEntries,
                  RwUInt16 numSplits, 
                  RwBBox   *bbox,
                  RwUInt32  flags);

extern void
_rpCollTreeDestroy(RpCollTree *tree);

/* 
 *  Streaming
 */
extern RwUInt32
_rpCollTreeStreamGetSize(const RpCollTree *tree);

extern const RpCollTree *
_rpCollTreeStreamWrite(const RpCollTree *tree, RwStream *stream);

extern RpCollTree *
_rpCollTreeStreamRead(RwStream *stream);

/* RWPUBLICEND */

extern RpCollTree *
_rpCollTreeStreamReadPre36001(RwStream *stream, 
                              RwUInt32  numEntries, 
                              RwUInt32  numSplits,
                              RwBBox   *bbox,
                              RwBool   *failConv);

/* RWPUBLIC */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

extern RwUInt16 *
RpCollTreeGetEntryMap(RpCollTree *tree);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* CTDATA_H */
