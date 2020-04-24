/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctquery.h
 *  Purpose: Collision partition tree queries
 */

#ifndef CTQUERY_H
#define CTQUERY_H

/******************************************************************************
 *  Constants
 */

#define rpCOLLISION_LINE_FEPS   1e-6f

/******************************************************************************
 *  Types
 */

/* RWPUBLIC */

#if !defined(DOXYGEN)

/* RpSector - BBox iterator */

typedef struct RpSectorBBoxItNodeInfo RpSectorBBoxItNodeInfo;
struct RpSectorBBoxItNodeInfo 
{
    RpSector   *sector; 
};

struct RpSectorBBoxIt
{
    RwBBox                  bbox;
    RpSectorBBoxItNodeInfo  stack[rpWORLDMAXBSPDEPTH+1];
    RpSectorBBoxItNodeInfo *curr;
};

/* RpCollSector - BBox iterator */

typedef struct RpCollSectorBBoxItNodeInfo RpCollSectorBBoxItNodeInfo;
struct RpCollSectorBBoxItNodeInfo 
{
    RpCollSector   *sector; 
    RwUInt32        clipFlags; /* RpCollBBoxClipFlags */
};

struct RpCollSectorBBoxIt
{
    RpCollTree                 *tree;
    RwBBox                      bbox; /* Used for rejection tests. Should be
                                           completely outside object. */
    RwBBox                      clipBBox;   /* Used for setting clip flags. Should 
                                           be completely inside object. */
    RpCollSector                dummy;
    RpCollSectorBBoxItNodeInfo  stack[rpCOLLTREE_MAXDEPTH+1];
    RpCollSectorBBoxItNodeInfo *curr;
};

/* RpSector - Line iterator */

typedef struct RpSectorLineItNodeInfo RpSectorLineItNodeInfo;
struct RpSectorLineItNodeInfo 
{
    RpSector   *sector; 
    RwReal      pa; /* Start parameter for segment of line in sector */
    RwReal      pb; /* End parameter for segment of line in sector */
};

struct RpSectorLineIt
{
    RwV3d                   start;
    RwV3d                   delta;
    RwV3d                   recip;
    RwV3d                   padding;
    RpSectorLineItNodeInfo  stack[rpWORLDMAXBSPDEPTH+1];
    RpSectorLineItNodeInfo *curr;
};

/* RpCollSector - Line iterator */

typedef struct RpCollSectorLineItNodeInfo RpCollSectorLineItNodeInfo;
struct RpCollSectorLineItNodeInfo
{
    RpCollSector   *sector;
    RwReal          pa; /* Start parameter for segment of line in sector */
    RwReal          pb; /* End parameter for segment of line in sector */
};

struct RpCollSectorLineIt
{
    RpCollTree                 *tree;
    RwV3d                       start;
    RwV3d                       delta;
    RwV3d                       recip;
    RwV3d                       padding;
    RpCollSector                dummy;
    RpCollSectorLineItNodeInfo  stack[rpCOLLTREE_MAXDEPTH+1];
    RpCollSectorLineItNodeInfo *curr;
};

#endif /* !defined(DOXYGEN) */


/**
 * \ingroup rpcollis
 * RpCollBBoxClipFlags are used to indicate the faces of an \ref RwBBox 
 * that clip a leaf sector of a collision tree (\ref RpCollTree). The shape 
 * of the leaf sector is also an axis aligned bounding box. If the flags are 
 * zero, then the sector is entirely inside the clip box.
 *
 * \see RpCollSectorBBoxIt
 */
enum RpCollBBoxClipFlags
{
    rpCOLLBBOXCLIP_INFX = 0x01, /**< Clipped by infimum  on x-axis */
    rpCOLLBBOXCLIP_SUPX = 0x02, /**< Clipped by supremum on x-axis */
    rpCOLLBBOXCLIP_INFY = 0x04, /**< Clipped by infimum  on y-axis */
    rpCOLLBBOXCLIP_SUPY = 0x08, /**< Clipped by supremum on y-axis */
    rpCOLLBBOXCLIP_INFZ = 0x10, /**< Clipped by infimum  on z-axis */
    rpCOLLBBOXCLIP_SUPZ = 0x20, /**< Clipped by supremum on z-axis */

    rpCOLLBBOXCLIP_MASK = 0x3f,

    rpCOLLBBOXCLIPFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

typedef enum RpCollBBoxClipFlags RpCollBBoxClipFlags;


/**
 * \ingroup rpcollis
 * \struct  RpSectorBBoxIt
 * This opaque type is an iterator object used for traversing the RpSector 
 * tree of an \ref RpWorld to find each \ref RpWorldSector (a leaf 
 * node of the tree) that intersects a given \ref RwBBox.
 *
 * \see RpSectorBBoxItInit
 * \see RpSectorBBoxItFinished
 * \see RpSectorBBoxItFindNextWorldSector
 */
typedef struct RpSectorBBoxIt RpSectorBBoxIt;


/**
 * \ingroup rpcollis
 * \struct  RpCollSectorBBoxIt
 * This opaque type is an iterator object used for traversing the 
 * \ref RpCollSector nodes of an \ref RpCollTree to find the leaf nodes that 
 * intersect a given \ref RwBBox. 
 *
 * \see RpCollSectorBBoxItInit
 * \see RpCollSectorBBoxItFinished
 * \see RpCollSectorBBoxItFindNextEntries
 */
typedef struct RpCollSectorBBoxIt RpCollSectorBBoxIt;


/**
 * \ingroup rpcollis
 * \struct  RpSectorLineIt
 * This opaque type is an iterator object used for traversing the 
 * RpSector tree of an \ref RpWorld to find each \ref RpWorldSector
 * (a leaf node of the tree) that intersects a given line. 
 *
 * \see RpSectorLineItInit
 * \see RpSectorLineItFinished
 * \see RpSectorLineItFindNextWorldSector
 * \see RpSectorLineItSetFarClip
 */
typedef struct RpSectorLineIt RpSectorLineIt;


/**
 * \ingroup rpcollis
 * \struct  RpCollSectorLineIt
 * This opaque type is an iterator object used for traversing the 
 * \ref RpCollSector nodes of an \ref RpCollTree to find the leaf nodes
 * that intersect a given line.
 *
 * \see RpCollSectorLineItInit
 * \see RpCollSectorLineItInitFromSectorIt
 * \see RpCollSectorLineItFinished
 * \see RpCollSectorLineItFindNextEntries
 * \see RpCollSectorLineItSetFarClip
 */
typedef struct RpCollSectorLineIt RpCollSectorLineIt;


/******************************************************************************
 *  Macros
 */

/*
 *  Line and BBox Iterators - test for completion
 */

#define _rpSectorLineItFinishedMacro(_it)     ((_it)->curr <= (_it)->stack)
#define _rpSectorBBoxItFinishedMacro(_it)     ((_it)->curr <= (_it)->stack)
#define _rpCollSectorLineItFinishedMacro(_it) ((_it)->curr <= (_it)->stack)
#define _rpCollSectorBBoxItFinishedMacro(_it) ((_it)->curr <= (_it)->stack)

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpSectorLineItFinished(_it)     _rpSectorLineItFinishedMacro(_it)
#define RpSectorBBoxItFinished(_it)     _rpSectorBBoxItFinishedMacro(_it)
#define RpCollSectorLineItFinished(_it) _rpCollSectorLineItFinishedMacro(_it)
#define RpCollSectorBBoxItFinished(_it) _rpCollSectorBBoxItFinishedMacro(_it)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* RWPUBLICEND */

/******************************************************************************
 */

/*
 *  _rpCollSectorGetEntriesMacro
 *
 *  Get entries contained within any RpCollSector. It can be a branch node
 *  with several leaf sectors below it, but the entries will be in a single
 *  contiguous slice of the collision ordered array.
 */
#define _rpCollSectorGetEntriesMacro(_tree, _sector, _start, _count)        \
MACRO_START                                                                 \
{                                                                           \
    RpCollSplit  *split = (_tree)->splits + (_sector)->index;               \
    RpCollSector *leftLeaf, *rightLeaf;                                     \
                                                                            \
    /* Get leftmost leaf sector */                                          \
    leftLeaf = &split->leftSector;                                          \
    while (leftLeaf->contents == rpCOLLSECTOR_CONTENTS_SPLIT)               \
        leftLeaf = &(_tree)->splits[leftLeaf->index].leftSector;            \
                                                                            \
    /* Get rightmost leaf sector */                                         \
    rightLeaf = &split->rightSector;                                        \
    while (rightLeaf->contents == rpCOLLSECTOR_CONTENTS_SPLIT)              \
        rightLeaf = &(_tree)->splits[rightLeaf->index].rightSector;         \
                                                                            \
    *(_start) = leftLeaf->index;                                            \
    *(_count) = (RwUInt16)                                                  \
        (rightLeaf->index + rightLeaf->contents - leftLeaf->index);         \
}                                                                           \
MACRO_STOP

/*******************************************************************************
 *  Primitive intersection tests. (should be moved to rtintsec API)
 */

#define _rpCollBBoxBBoxTestMacro(_bb1, _bb2)                                \
    (    ((_bb1)->inf.x <= (_bb2)->sup.x)                                   \
      && ((_bb1)->inf.y <= (_bb2)->sup.y)                                   \
      && ((_bb1)->inf.z <= (_bb2)->sup.z)                                   \
      && ((_bb1)->sup.x >= (_bb2)->inf.x)                                   \
      && ((_bb1)->sup.y >= (_bb2)->inf.y)                                   \
      && ((_bb1)->sup.z >= (_bb2)->inf.z) )

#define _rpCollBBoxInBBoxClipFlagsMacro(_bb, _clipbb)                       \
    (   (((_bb)->inf.x < (_clipbb)->inf.x) ? rpCOLLBBOXCLIP_INFX:0)         \
      | (((_bb)->inf.y < (_clipbb)->inf.y) ? rpCOLLBBOXCLIP_INFY:0)         \
      | (((_bb)->inf.z < (_clipbb)->inf.z) ? rpCOLLBBOXCLIP_INFZ:0)         \
      | (((_bb)->sup.x > (_clipbb)->sup.x) ? rpCOLLBBOXCLIP_SUPX:0)         \
      | (((_bb)->sup.y > (_clipbb)->sup.y) ? rpCOLLBBOXCLIP_SUPY:0)         \
      | (((_bb)->sup.z > (_clipbb)->sup.z) ? rpCOLLBBOXCLIP_SUPZ:0) )

/*
 *  Test intersection of two intervals. (pa,pb) are parameters for
 *  the range of the first interval between start and end, and on completion
 *  give the range of the intersection.
 */
#define _rpCollIntervalTestMacro(_result, _start, _recip,                   \
                                 _inf, _sup, _pa, _pb)                      \
MACRO_START                                                                 \
{                                                                           \
    if ((_recip) == 0.0f)                                                   \
    {                                                                       \
        (_result) = ((_start) >= (_inf) && (_start) <= (_sup));             \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RwReal p0 = (_recip) * ((_inf) - (_start));                         \
        RwReal p1 = (_recip) * ((_sup) - (_start));                         \
                                                                            \
        if (p1 < p0)                                                        \
        {                                                                   \
            RwReal temp = p0;                                               \
            p0 = p1;                                                        \
            p1 = temp;                                                      \
        }                                                                   \
                                                                            \
        (_result) = (*(_pa) <= p1) && (*(_pb) >= p0);                       \
        if (_result)                                                        \
        {                                                                   \
            *(_pa) = RwRealMax2(*(_pa), p0);                                \
            *(_pb) = RwRealMin2(*(_pb), p1);                                \
        }                                                                   \
    }                                                                       \
}                                                                           \
MACRO_STOP


/*
 *  Test intersection of line and bbox. (pa,pb) are parameters for a line
 *  segment and should be initialized, usually to (0,1). On completion, they
 *  contain parameters for the intersection region.
 */
#define _rpCollLineBBoxTestMacro(_result, _start, _recip, _bb, _pa, _pb)    \
MACRO_START                                                                 \
{                                                                           \
    _rpCollIntervalTestMacro(_result, (_start)->x, (_recip)->x,             \
        (_bb)->inf.x, (_bb)->sup.x, _pa, _pb);                              \
    if (_result)                                                            \
    {                                                                       \
        _rpCollIntervalTestMacro(_result, (_start)->y, (_recip)->y,         \
            (_bb)->inf.y, (_bb)->sup.y, _pa, _pb);                          \
        if (_result)                                                        \
        {                                                                   \
            _rpCollIntervalTestMacro(_result, (_start)->z, (_recip)->z,     \
                (_bb)->inf.z, (_bb)->sup.z, _pa, _pb);                      \
        }                                                                   \
    }                                                                       \
}                                                                           \
MACRO_STOP


/******************************************************************************
 *  Generic iterator macros that are used for both RpSector and RpCollSector
 *  iterators.
 */

/* Set iterator for immediate termination */
#define _rpCollItInitAsFinishedMacro(_it)  ((_it)->curr = (_it)->stack)


/*
 *  Line iteration
 */

#define _rwRealMaxAbs2(_a, _b) (RwRealMax2(RwRealAbs(_a), RwRealAbs(_b)))

/*
 *  Initializing line params
 *
 *  Reciprocal delta is defined in a slightly special way
 *  to allow the iterator step function to deal with axis aligned
 *  lines properly. 
 */
#define _rpCollLineItInitMacro(_it, _root, _line, _padding)                 \
MACRO_START                                                                 \
{                                                                           \
    (_it)->start = (_line)->start;                                          \
    RwV3dSub(&(_it)->delta, &(_line)->end, &(_line)->start);                \
                                                                            \
    (_it)->recip.x = ((_it)->delta.x != 0) ? (1/(_it)->delta.x) : 0;        \
    (_it)->recip.y = ((_it)->delta.y != 0) ? (1/(_it)->delta.y) : 0;        \
    (_it)->recip.z = ((_it)->delta.z != 0) ? (1/(_it)->delta.z) : 0;        \
                                                                            \
    if (_padding)                                                           \
    {                                                                       \
        RWASSERT(   (_padding)->x >= 0                                      \
                 && (_padding)->y >= 0                                      \
                 && (_padding)->z >= 0);                                    \
        (_it)->padding = *(_padding);                                       \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        (_it)->padding.x = _rwRealMaxAbs2((_line)->start.x, (_line)->end.x);\
        (_it)->padding.y = _rwRealMaxAbs2((_line)->start.y, (_line)->end.y);\
        (_it)->padding.z = _rwRealMaxAbs2((_line)->start.z, (_line)->end.z);\
        RwV3dScale(&(_it)->padding, &(_it)->padding, rpCOLLISION_LINE_FEPS);\
    }                                                                       \
                                                                            \
    (_it)->stack[0].sector = (_root);                                       \
    (_it)->stack[0].pa = 0;                                                 \
    (_it)->stack[0].pb = 1;                                                 \
    (_it)->curr = (_it)->stack + 1;                                         \
}                                                                           \
MACRO_STOP

#define _rpCollLineItInitRecursiveMacro(_it, _root, _parentIt)              \
MACRO_START                                                                 \
{                                                                           \
    (_it)->start = (_parentIt)->start;                                      \
    (_it)->delta = (_parentIt)->delta;                                      \
    (_it)->recip = (_parentIt)->recip;                                      \
    (_it)->padding = (_parentIt)->padding;                                  \
    (_it)->stack[0].sector = (_root);                                       \
    (_it)->stack[0].pa = (_parentIt)->curr->pa;                             \
    (_it)->stack[0].pb = (_parentIt)->curr->pb;                             \
    (_it)->curr = (_it)->stack + 1;                                         \
}                                                                           \
MACRO_STOP

#define _rpCollLineItClipBBoxMacro(_it, _bbox)                              \
MACRO_START                                                                 \
{                                                                           \
    RwBBox  padbb;                                                          \
    RwBool  result;                                                         \
                                                                            \
    RwV3dSub(&padbb.inf, &(_bbox)->inf, &(_it)->padding);                   \
    RwV3dAdd(&padbb.sup, &(_bbox)->sup, &(_it)->padding);                   \
                                                                            \
    _rpCollLineBBoxTestMacro(result, &(_it)->start, &(_it)->recip,          \
        &padbb, &(_it)->stack[0].pa, &(_it)->stack[0].pb);                  \
                                                                            \
    if (!result)                                                            \
    {                                                                       \
        (_it)->curr = (_it)->stack;                                         \
    }                                                                       \
}                                                                           \
MACRO_STOP

/*
 *  Macro for processing branch node of sector tree during line iteration
 *
 *  _it                 pointer to RpSectorLineIt or RpCollSectorLineIt
 *  _axis               0,4,8 for x,y,z
 *  _left, _right       pointers to left and right sectors
 *  _leftHi, _rightLo   values for left and right sector boundaries
 */
#define _rpCollLineItStepMacro(_it, _axis, _left, _right,                   \
                                            _leftHi, _rightLo)              \
MACRO_START                                                                 \
{                                                                           \
    RwReal del, sta, end, rec, pad, padLo, padHi;                           \
                                                                            \
    del = GETCOORD((_it)->delta, (_axis));                                  \
    sta = GETCOORD((_it)->start, (_axis));                                  \
    pad = GETCOORD((_it)->padding, (_axis));                                \
    end = sta + (_it)->curr->pb * del;                                      \
    sta = sta + (_it)->curr->pa * del;                                      \
    padLo = (_rightLo) - pad;                                               \
    padHi = (_leftHi) + pad;                                                \
                                                                            \
    if (del > 0)                                                            \
    {                                                                       \
        /* Line goes from left to right */                                  \
        if (padLo > end)                                                    \
        {                                                                   \
            /* Completely in left */                                        \
            (_it)->curr->sector = (_left);                                  \
        }                                                                   \
        else if (padHi < sta)                                               \
        {                                                                   \
            /* Completely in right */                                       \
            (_it)->curr->sector = (_right);                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            (_it)->curr++;                                                  \
            rec = GETCOORD((_it)->recip, (_axis));                          \
                                                                            \
            /* Go left (near sector) */                                     \
            (_it)->curr->sector = (_left);                                  \
            (_it)->curr->pa = ((_it)->curr - 1)->pa;                        \
            (_it)->curr->pb = ((_it)->curr - 1)->pb;                        \
            if (padHi < end)                                                \
            {                                                               \
                /* Clip end for near line segment */                        \
                (_it)->curr->pb += rec * (padHi - end);                     \
            }                                                               \
                                                                            \
            /* Stack right (far sector) */                                  \
            ((_it)->curr - 1)->sector = (_right);                           \
            if (padLo > sta)                                                \
            {                                                               \
                /* Clip start for far line segment */                       \
                ((_it)->curr - 1)->pa += rec * (padLo - sta);               \
            }                                                               \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        /* Line goes from right to left */                                  \
        if (padHi < end)                                                    \
        {                                                                   \
            /* Completely in right */                                       \
            (_it)->curr->sector = (_right);                                 \
        }                                                                   \
        else if (padLo > sta)                                               \
        {                                                                   \
            /* Completely in left */                                        \
            (_it)->curr->sector = (_left);                                  \
        }                                                                   \
        else                                                                \
        {                                                                   \
            (_it)->curr++;                                                  \
            rec = GETCOORD((_it)->recip, (_axis));                          \
                                                                            \
            /* Go right (near sector) */                                    \
            (_it)->curr->sector = (_right);                                 \
            (_it)->curr->pa = ((_it)->curr - 1)->pa;                        \
            (_it)->curr->pb = ((_it)->curr - 1)->pb;                        \
            if (padLo > end)                                                \
            {                                                               \
                /* Clip end for near line segment */                        \
                (_it)->curr->pb += rec * (padLo  - end);                    \
            }                                                               \
                                                                            \
            /* Stack left (far sector) */                                   \
            ((_it)->curr - 1)->sector = (_left);                            \
            if (padHi < sta)                                                \
            {                                                               \
                /* Clip start for far line segment */                       \
                ((_it)->curr - 1)->pa += rec * (padHi - sta);               \
            }                                                               \
        }                                                                   \
    }                                                                       \
}                                                                           \
MACRO_STOP

/*
 *  Stripping sector nodes from stack that are further than a 
 *  given distance:- 
 */
#define _rpCollLineItSetFarClipMacro(_it, _val)                             \
MACRO_START                                                                 \
{                                                                           \
    RwUInt32  i, iKeep = 0;                                                 \
                                                                            \
    for (i=0; (it->stack + i) < it->curr; i++)                              \
    {                                                                       \
        /* Is the segment for this sector in range? */                      \
        if (it->stack[i].pa <= (_val))                                      \
        {                                                                   \
            it->stack[iKeep] = it->stack[i];                                \
            if (it->stack[iKeep].pb > (_val))                               \
            {                                                               \
                it->stack[iKeep].pb = (_val);                               \
            }                                                               \
                                                                            \
            iKeep++;                                                        \
        }                                                                   \
    }                                                                       \
                                                                            \
    it->curr = it->stack + iKeep;                                           \
}                                                                           \
MACRO_STOP

/*
 *  Simple bbox iteration 
 */

#define _rpCollBBoxItInitMacro(_it, _root, _bbox)                           \
MACRO_START                                                                 \
{                                                                           \
    (_it)->bbox = *(_bbox);                                                 \
    (_it)->stack[0].sector = (_root);                                       \
    (_it)->curr = (_it)->stack + 1;                                         \
}                                                                           \
MACRO_STOP

#define _rpCollBBoxItStepMacro(_it, _axis, _left, _right,                   \
                                            _leftHi, _rightLo)              \
MACRO_START                                                                 \
{                                                                           \
    if ((_rightLo) > GETCOORD((_it)->bbox.sup, _axis))                      \
    {                                                                       \
        (_it)->curr->sector = (_left);                                      \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        (_it)->curr->sector = (_right);                                     \
        if ((_leftHi) >= GETCOORD((_it)->bbox.inf, _axis))                  \
        {                                                                   \
            (_it)->curr++;                                                  \
            (_it)->curr->sector = (_left);                                  \
        }                                                                   \
    }                                                                       \
}                                                                           \
MACRO_STOP

/*
 *  BBox with clip flags iteration
 */

#define _rpCollBBoxClipItInitMacro(                                         \
            _it, _root, _rootBB, _testBB, _clipBB, _flags)                  \
MACRO_START                                                                 \
{                                                                           \
    (_it)->bbox = *(_testBB);                                               \
    (_it)->clipBBox = *(_clipBB);                                           \
    (_it)->stack[0].sector = (_root);                                       \
    (_it)->stack[0].clipFlags =                                             \
        _rpCollBBoxInBBoxClipFlagsMacro(_rootBB, _clipBB);                  \
    (_it)->curr = (_it)->stack + 1;                                         \
}                                                                           \
MACRO_STOP

#define _rpCollBBoxClipItStepMacro(_it, _axis, _left, _right,               \
                                                _leftHi, _rightLo)          \
MACRO_START                                                                 \
{                                                                           \
    if ((_rightLo) > GETCOORD((_it)->bbox.sup, _axis))                      \
    {                                                                       \
        (_it)->curr->sector = (_left);                                      \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        (_it)->curr->sector = (_right);                                     \
        if ((_leftHi) >= GETCOORD((_it)->bbox.inf, _axis))                  \
        {                                                                   \
            (_it)->curr++;                                                  \
                                                                            \
            (_it)->curr->sector = (_left);                                  \
            (_it)->curr->clipFlags = ((_it)->curr - 1)->clipFlags;          \
            if ((_leftHi) <= GETCOORD((_it)->clipBBox.sup, _axis))          \
            {                                                               \
                (_it)->curr->clipFlags &=                                   \
                    ~(rpCOLLBBOXCLIP_SUPX << ((_axis)>>1));                 \
            }                                                               \
                                                                            \
            if ((_rightLo) >= GETCOORD((_it)->clipBBox.inf, _axis))         \
            {                                                               \
                ((_it)->curr - 1)->clipFlags &=                             \
                    ~(rpCOLLBBOXCLIP_INFX << ((_axis)>>1));                 \
            }                                                               \
        }                                                                   \
    }                                                                       \
}                                                                           \
MACRO_STOP


/*****************************************************************************
 *  RpSector Iterators
 */

#define _rpSectorLineItInitMacro(_it, _world, _line, _padding)              \
    _rpCollLineItInitMacro(_it, (_world)->rootSector, _line, _padding)

#define _rpSectorLineItFindNextWorldSectorMacro(_it, _sector)               \
MACRO_START                                                                 \
{                                                                           \
    (_it)->curr--;                                                          \
    while ((_it)->curr->sector->type >= 0)                                  \
    {                                                                       \
        RpPlaneSector  *plane = (RpPlaneSector *)(_it)->curr->sector;       \
        _rpCollLineItStepMacro(_it, plane->type, plane->leftSubTree,        \
            plane->rightSubTree, plane->leftValue, plane->rightValue);      \
    }                                                                       \
                                                                            \
    *(_sector) = (RpWorldSector *)(_it)->curr->sector;                      \
}                                                                           \
MACRO_STOP


#define _rpSectorBBoxItInitMacro(_it, _world, _bbox)                        \
    _rpCollBBoxItInitMacro(_it, (_world)->rootSector, _bbox);

#define _rpSectorBBoxItFindNextWorldSectorMacro(_it, _sector)               \
MACRO_START                                                                 \
{                                                                           \
    (_it)->curr--;                                                          \
    while ((_it)->curr->sector->type >= 0)                                  \
    {                                                                       \
        RpPlaneSector *plane = (RpPlaneSector *)(_it)->curr->sector;        \
        _rpCollBBoxItStepMacro(_it, plane->type, plane->leftSubTree,        \
            plane->rightSubTree, plane->leftValue, plane->rightValue);      \
    }                                                                       \
                                                                            \
    *(_sector) = (RpWorldSector *)(_it)->curr->sector;                      \
}                                                                           \
MACRO_STOP


/*****************************************************************************
 *  RpCollSector iterators
 */

#define _rpCollSectorItInitCollTreeMacro(_it, _tree)                        \
{                                                                           \
    (_it)->tree = (_tree);                                                  \
    (_it)->dummy.index = 0;                                                 \
    (_it)->dummy.contents = rpCOLLSECTOR_CONTENTS_SPLIT;                    \
    if ((_tree)->numSplits == 0)                                            \
    {                                                                       \
        (_it)->dummy.contents = (RwUInt8) (_tree)->numEntries;              \
    }                                                                       \
}

#define _rpCollSectorLineItInitMacro(_it, _tree, _line, _padding)           \
MACRO_START                                                                 \
{                                                                           \
    _rpCollSectorItInitCollTreeMacro(_it, _tree);                           \
    _rpCollLineItInitMacro(_it, &(_it)->dummy, _line, _padding);            \
    _rpCollLineItClipBBoxMacro(_it, &(_tree)->bbox);                        \
}                                                                           \
MACRO_STOP

#define _rpCollSectorLineItInitFromSectorItMacro(_it, _tree, _sectIt)       \
MACRO_START                                                                 \
{                                                                           \
    _rpCollSectorItInitCollTreeMacro(_it, _tree);                           \
    _rpCollLineItInitRecursiveMacro(_it, &(_it)->dummy, _sectIt);           \
}                                                                           \
MACRO_STOP

#define _rpCollSectorLineItFindNextEntriesMacro(_it, _start, _count)        \
MACRO_START                                                                 \
{                                                                           \
    (_it)->curr--;                                                          \
    while ((_it)->curr->sector->contents == rpCOLLSECTOR_CONTENTS_SPLIT)    \
    {                                                                       \
        RpCollSplit     *split;                                             \
        RwUInt32        axis;                                               \
                                                                            \
        split = (_it)->tree->splits + (_it)->curr->sector->index;           \
        axis = split->leftSector.type & rpCOLLSECTOR_TYPE_AXISMASK;         \
        _rpCollLineItStepMacro(_it, axis,                                   \
            &split->leftSector, &split->rightSector,                        \
            split->leftSector.value, split->rightSector.value);             \
    }                                                                       \
                                                                            \
    *(_start) = (_it)->curr->sector->index;                                 \
    *(_count) = (_it)->curr->sector->contents;                              \
}                                                                           \
MACRO_STOP

/*
 *  BBoxes
 */

#define _rpCollSectorBBoxItInitMacro(_it, _tree, _bbox, _clipBBox)          \
MACRO_START                                                                 \
{                                                                           \
    if (_rpCollBBoxBBoxTestMacro(&(_tree)->bbox, _bbox))                    \
    {                                                                       \
        _rpCollSectorItInitCollTreeMacro(_it, _tree);                       \
        _rpCollBBoxClipItInitMacro(                                         \
            _it, &(_it)->dummy, &(_tree)->bbox, _bbox, _clipBBox, flags);   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        _rpCollItInitAsFinishedMacro(_it);                                  \
    }                                                                       \
}                                                                           \
MACRO_STOP

#define _rpCollSectorBBoxItFindNextEntriesMacro(_it, _start, _count, _clip) \
MACRO_START                                                                 \
{                                                                           \
    (_it)->curr--;                                                          \
    for ( ;; )                                                              \
    {                                                                       \
        RpCollSector *sector = (_it)->curr->sector;                         \
        RpCollSplit  *split;                                                \
        RwUInt32      axis;                                                 \
                                                                            \
        /* Do we have a leaf sector? */                                     \
        if (sector->contents != rpCOLLSECTOR_CONTENTS_SPLIT)                \
        {                                                                   \
            *(_start) = sector->index;                                      \
            *(_count) = sector->contents;                                   \
            *(_clip) = (_it)->curr->clipFlags;                              \
            break;                                                          \
        }                                                                   \
                                                                            \
        /* Can do a shortcut if completely inside clip bbox */              \
        if ((_it)->curr->clipFlags == 0)                                    \
        {                                                                   \
            _rpCollSectorGetEntriesMacro(                                   \
                (_it)->tree, sector, _start, _count);                       \
            *(_clip) = 0;                                                   \
            break;                                                          \
        }                                                                   \
                                                                            \
        /* Deal with branch sector */                                       \
        split = (_it)->tree->splits + sector->index;                        \
        axis = split->leftSector.type & rpCOLLSECTOR_TYPE_AXISMASK;         \
        _rpCollBBoxClipItStepMacro(_it, axis,                               \
            &split->leftSector, &split->rightSector,                        \
            split->leftSector.value, split->rightSector.value);             \
    }                                                                       \
}                                                                           \
MACRO_STOP

/* RWPUBLIC */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

extern RwBool
RpSectorBBoxItFinished(RpSectorBBoxIt *it);

extern RwBool
RpSectorLineItFinished(RpSectorLineIt *it);

extern RwBool
RpCollSectorBBoxItFinished(RpCollSectorBBoxIt *it);

extern RwBool
RpCollSectorLineItFinished(RpCollSectorLineIt *it);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/*
 *  RpSector iterators 
 */

extern void
RpSectorBBoxItInit(RpSectorBBoxIt *it, 
                   RpWorld        *world,
                   RwBBox         *bbox);

extern RpWorldSector *
RpSectorBBoxItFindNextWorldSector(RpSectorBBoxIt *it);

extern void
RpSectorLineItInit(RpSectorLineIt *it, 
                   RpWorld        *world,
                   RwLine         *line,
                   RwV3d          *padding);

extern RpWorldSector *
RpSectorLineItFindNextWorldSector(RpSectorLineIt *it);

extern void
RpSectorLineItSetFarClip(RpSectorLineIt *it,
                         RwReal          value);

/*
 *  RpCollSector iterators 
 */

extern void
RpCollSectorBBoxItInit(RpCollSectorBBoxIt *it, 
                       RpCollTree         *tree,
                       RwBBox             *outerBBox,
                       RwBBox             *innerBBox);

extern void
RpCollSectorBBoxItFindNextEntries(RpCollSectorBBoxIt  *it, 
                                  RwUInt16            *start, 
                                  RwUInt16            *count,
                                  RwUInt32            *clipFlags);

extern void
RpCollSectorLineItInit(RpCollSectorLineIt  *it, 
                       RpCollTree          *tree,
                       RwLine              *line,
                       RwV3d               *padding);

extern void
RpCollSectorLineItInitFromSectorIt(RpCollSectorLineIt  *it, 
                                   RpCollTree          *tree,
                                   RpSectorLineIt      *sectIt);

extern void
RpCollSectorLineItFindNextEntries(RpCollSectorLineIt *it, 
                                  RwUInt16            *start, 
                                  RwUInt16            *count);

extern void
RpCollSectorLineItSetFarClip(RpCollSectorLineIt *it,
                             RwReal             value);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* CTQUERY_H */
