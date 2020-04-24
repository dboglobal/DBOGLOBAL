/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctquery.c
 *  Purpose: Collision partition tree queries
 */

/******************************************************************************
 *  Include files
 */

#include "rwcore.h"
#include "rpdbgerr.h"
#include "rpworld.h"
#include "rtintsec.h"

#include "ctdata.h"
#include "ctquery.h"


/******************************************************************************
 *  Completion tests
 */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpcollis
 * \ref RpCollSectorBBoxItFinished  queries whether a collision sector
 * bounding box iteration process has finished.
 *
 * \param it Pointer to the iterator
 *
 * \return TRUE if iteration has finished.
 *
 * \note For efficiency, a macro implementation is used in release builds.
 *
 * \see RpCollSectorBBoxItInit
 * \see RpCollSectorBBoxItFindNextEntries
 * \see \ref rpcollisoverview
 */
RwBool
RpCollSectorBBoxItFinished(RpCollSectorBBoxIt *it)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorBBoxItFinished"));
    RWASSERT(it);
    RWASSERT(it->curr >= it->stack);
    RWRETURN(_rpCollSectorBBoxItFinishedMacro(it));
}

/**
 * \ingroup rpcollis
 * \ref RpCollSectorLineItFinished queries whether a collision sector
 * line iteration process has finished.
 *
 * \param it Pointer to the iterator
 *
 * \return TRUE if iteration has finished.
 *
 * \note For efficiency, a macro implementation is used in release builds.
 *
 * \see RpCollSectorLineItInit
 * \see RpCollSectorLineItFindNextEntries
 * \see RpCollSectorLineItSetFarClip
 * \see \ref rpcollisoverview
 */
RwBool
RpCollSectorLineItFinished(RpCollSectorLineIt *it)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorLineItFinished"));
    RWASSERT(it);
    RWASSERT(it->curr >= it->stack);
    RWRETURN(_rpCollSectorLineItFinishedMacro(it));
}

/**
 * \ingroup rpcollis
 * \ref RpSectorBBoxItFinished queries whether a world sector
 * bounding box iteration process has finished.
 *
 * \param it Pointer to the iterator
 *
 * \return TRUE if iteration has finished.
 *
 * \note For efficiency, a macro implementation is used in release builds.
 *
 * \see RpSectorBBoxItInit
 * \see RpSectorBBoxItFindNextWorldSector
 * \see \ref rpcollisoverview
 */
RwBool
RpSectorBBoxItFinished(RpSectorBBoxIt *it)
{
    RWAPIFUNCTION(RWSTRING("RpSectorBBoxItFinished"));
    RWASSERT(it);
    RWASSERT(it->curr >= it->stack);

    RWRETURN(_rpSectorBBoxItFinishedMacro(it));
}

/**
 * \ingroup rpcollis
 * \ref RpSectorLineItFinished queries whether a world sector
 * line iteration process has finished.
 *
 * \param it Pointer to the iterator
 *
 * \return TRUE if iteration has finished.
 *
 * \note For efficiency, a macro implementation is used in release builds.
 *
 * \see RpSectorLineItInit
 * \see RpSectorLineItFindNextWorldSector
 * \see RpSectorLineItSetFarClip
 * \see \ref rpcollisoverview
 */
RwBool
RpSectorLineItFinished(RpSectorLineIt *it)
{
    RWAPIFUNCTION(RWSTRING("RpSectorLineItFinished"));
    RWASSERT(it);
    RWASSERT(it->curr >= it->stack);

    RWRETURN(_rpSectorLineItFinishedMacro(it));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/*
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  RpSector iterators
 *
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


/**
 * \ingroup rpcollis
 * \ref RpSectorBBoxItInit initializes an \ref RpSectorBBoxIt iterator
 * object for traversing the RpSector tree of an \ref RpWorld
 * to find each \ref RpWorldSector (a leaf node of the tree) that intersects
 * the given bounding box.
 *
 * \param it     Pointer to the iterator structure.
 * \param world  Pointer to the world.
 * \param bbox   Pointer to the bounding box.
 *
 * The following code shows an example of usage
 *
 * \code

    RpSectorBBoxIt  it;

    RpSectorBBoxItInit(&it, world, bbox);
    while (!RpSectorBBoxItFinished(&it))
    {
        RpWorldSector *worldSector;

        worldSector = RpSectorBBoxItFindNextWorldSector(&it);

         ... code to process world sector ...
    }

 * \endcode
 *
 * \see RpSectorLineItInit
 * \see RpCollSectorBBoxItInit
 * \see RpCollSectorLineItInit
 * \see \ref rpcollisoverview
 */
void
RpSectorBBoxItInit(RpSectorBBoxIt *it,
                   RpWorld        *world,
                   RwBBox         *bbox)
{
    RWAPIFUNCTION(RWSTRING("RpSectorBBoxItInit"));
    RWASSERT(it);
    RWASSERT(world);
    RWASSERT(bbox);

    _rpSectorBBoxItInitMacro(it, world, bbox);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpSectorBBoxItFindNextWorldSector advances a world sector bounding
 * box iterator, returning the next \ref RpWorldSector that intersects
 * the bounding box.
 *
 * \param   it  Pointer to the iterator structure.
 *
 * \return A pointer to the next world sector.
 *
 * \see RpSectorBBoxItInit
 * \see RpSectorBBoxItFinished
 * \see \ref rpcollisoverview
 */
RpWorldSector *
RpSectorBBoxItFindNextWorldSector(RpSectorBBoxIt *it)
{
    RpWorldSector *worldSector;

    RWAPIFUNCTION(RWSTRING("RpSectorBBoxItFindNextWorldSector"));
    RWASSERT(it);
    RWASSERT(it->curr > it->stack);

    _rpSectorBBoxItFindNextWorldSectorMacro(it, &worldSector);
    RWRETURN(worldSector);
}


/**
 * \ingroup rpcollis
 * \ref RpSectorLineItInit initializes an \ref RpSectorLineIt iterator
 * object for traversing the RpSector tree of an \ref RpWorld
 * to find each \ref RpWorldSector (a leaf node of the tree) that intersects
 * the given line.
 *
 * \param it        Pointer to the iterator structure.
 * \param world     Pointer to the world.
 * \param line      Pointer to the line.
 * \param padding   Pointer to a vector specifying a padding amount on each axis. 
 *                  Use NULL for straightforward line queries.
 *
 * The following code shows an example of usage
 *
 * \code

    RpSectorLineIt  it;

    RpSectorLineItInit(&it, world, line, NULL);
    while (!RpSectorLineItFinished(&it))
    {
        RpWorldSector *worldSector;

        worldSector = RpSectorLineItFindNextWorldSector(&it);

         ... code to process world sector ...
    }

 * \endcode
 *
 * The iteration process proceeds from the start of the line to the end, but
 * due to the overlapping nature of the sectors, the ordering in terms of
 * distance is loosely defined. This complicates the process of finding the
 * nearest intersection along a line, but the function
 * \ref RpSectorLineItSetFarClip can be used to achieve this as efficiently
 * as possible.
 *
 * The padding values fatten the line in positive and negative directions
 * on each axis, so that it represents an axis aligned box swept along the 
 * line vector. An equivalent interpretation of this is that the nodes 
 * of the BSP tree (axis aligned boxes) are fattened by this amount, and
 * not the line. This could be used in parallel projection shadow calculations, 
 * by using the center of an occluder object as the start of the line, setting
 * the padding to bound the object, and setting the line end
 * point at some position away in the lighting direction.
 *
 * A small amount of padding will be used internally even if NULL is specified 
 * for the padding parameter to allow for floating point inaccuracies.
 *
 * \see RpSectorBBoxItInit
 * \see RpCollSectorBBoxItInit
 * \see RpCollSectorLineItInit
 * \see \ref rpcollisoverview
 */
void
RpSectorLineItInit(RpSectorLineIt  *it,
                   RpWorld         *world,
                   RwLine          *line,
                   RwV3d           *padding)
{
    RWAPIFUNCTION(RWSTRING("RpSectorLineItInit"));
    RWASSERT(it);
    RWASSERT(world);
    RWASSERT(line);

    _rpSectorLineItInitMacro(it, world, line, padding);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpSectorLineItFindNextWorldSector advances a world sector line
 * iterator, returning the next \ref RpWorldSector that intersects
 * the line.
 *
 * \param   it  Pointer to the iterator structure.
 *
 * \return Pointer to the next world sector intersected.
 *
 * \see RpSectorLineItInit
 * \see RpSectorLineItFinished
 * \see RpSectorLineItSetFarClip
 * \see \ref rpcollisoverview
 */
RpWorldSector *
RpSectorLineItFindNextWorldSector(RpSectorLineIt *it)
{
    RpWorldSector   *worldSector;

    RWAPIFUNCTION(RWSTRING("RpSectorLineItFindNextWorldSector"));
    RWASSERT(it);
    RWASSERT(it->curr > it->stack);

    _rpSectorLineItFindNextWorldSectorMacro(it, &worldSector);
    RWRETURN(worldSector);
}


/**
 * \ingroup rpcollis
 * \ref RpSectorLineItSetFarClip modifies an RpSector line
 * iterator to eliminate, from the iteration process, all sectors beyond
 * a given distance.
 *
 * \param it     Pointer to the iterator.
 * \param value  The far clip parameter in the range 0 to 1 (corresponding to
 *               the start and end of the line).
 *
 * This is intended for use when searching for the nearest intersection
 * along a line (eg triangle or atomic). Line iteration proceeds from the
 * start to the end of the line, finding each \ref RpWorldSector in the tree
 * that intersects the line. During this process, objects within each
 * \ref RpWorldSector may be tested for intersection with the line. If an
 * intersection is found, then it is likely to be the nearest, but this
 * cannot be guaranteed due to the overlapping nature of the sectors. By
 * calling this function with the parametric distance to the intersection,
 * any sectors that are further away can be eliminated from the iteration
 * process, leaving only those that potentially contain closer intersections
 * than the one already found. In many cases, this will immediately eliminate
 * all further processing and the next call to \ref RpSectorLineItFinished
 * will return TRUE.
 *
 * See \ref RpCollSectorLineItInit for a code example that demonstrates the
 * use of the related function \ref RpCollSectorLineItSetFarClip.
 *
 * \see RpSectorLineItInit
 * \see RpSectorLineItFindNextWorldSector
 * \see \ref rpcollisoverview
 */
void
RpSectorLineItSetFarClip(RpSectorLineIt *it, RwReal value)
{
    RWAPIFUNCTION(RWSTRING("RpSectorLineItSetFarClip"));
    RWASSERT(it);

    _rpCollLineItSetFarClipMacro(it, value);
    RWRETURNVOID();
}


/*
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  RpCollSector iterators
 *
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


/**
 * \ingroup rpcollis
 * \ref RpCollSectorLineItInit initializes an \ref RpCollSectorLineIt iterator
 * object for traversing an \ref RpCollTree to find each leaf sector
 * that intersects the given line.
 *
 * \param it        Pointer to the iterator structure.
 * \param tree      Pointer to the collision tree.
 * \param line      Pointer to the line.
 * \param padding   Pointer to a vector specifying a padding amount on each axis. 
 *                  Use NULL for straightforward line queries. See \ref
 *                  RpSectorLineItInit for more information.
 *
 * The following code shows an example of usage. This finds the first
 * triangle along a given line in a world sector.
 *
 * The iteration process proceeds from the start of the line to the end, but
 * due to the overlapping nature of the sectors, the ordering in terms of
 * distance is loosely defined. This complicates the process of finding the
 * nearest intersection, but the iterator modifier function
 * \ref RpCollSectorLineItSetFarClip is used below to achieve this as
 * efficiently as possible.
 *
 * The example also assumes that the triangles are not sorted into the
 * collision tree order so the entry map is used.
 *
 * \code

    const RpTriangle   *tris = RpWorldSectorGetTriangles(worldSector);
    const RwV3d        *verts = RpWorldSectorGetVertices(worldSector);
    RpCollTree         *tree = RpCollisionWorldSectorGetCollTree(worldSector);
    RwUInt16           *map = RpCollTreeGetEntryMap(tree);

    RwV3d               lineDelta;
    RpCollSectorLineIt  iterator;
    RwReal              minDist = RwRealMAXVAL;
    RpTriangle         *closestTri = NULL;

    RwV3dSubMacro(&lineDelta, &line->end, &line->start);

    RpCollSectorLineItInit(&iterator, tree, line, NULL);
    while (!RpCollSectorLineItFinished(&iterator))
    {
        RwUInt16  i, start, count;

        RpCollSectorLineItFindNextEntries(&iterator, &start, &count);
        for (i=start; i<(start+count); i++)
        {
            RwUInt16          triIndex = map[i];
            const RpTriangle *tri = tris + triIndex;
            const RwV3d      *v0 = verts + tri->vertIndex[0];
            const RwV3d      *v1 = verts + tri->vertIndex[1];
            const RwV3d      *v2 = verts + tri->vertIndex[2];
            RwReal            dist;

            if (RtIntersectionLineTriangle(
                        &line->start, &lineDelta, v0, v1, v2, &dist)
                && (dist < minDist))
            {
                minDist = dist;
                closestTri = tri;
                RpCollSectorLineItSetFarClip(&iterator, dist);
            }
        }
    }

 * \endcode
 *
 * \see RpSectorLineItInit
 * \see RpSectorBBoxItInit
 * \see RpCollSectorBBoxItInit
 * \see \ref rpcollisoverview
 */
void
RpCollSectorLineItInit(RpCollSectorLineIt  *it,
                       RpCollTree          *tree,
                       RwLine              *line,
                       RwV3d               *padding)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorLineItInit"));
    RWASSERT(it);
    RWASSERT(tree);
    RWASSERT(line);
    RWASSERT(tree->numSplits > 0
        || tree->numEntries <= rpCOLLSECTOR_CONTENTS_MAXCOUNT);

    _rpCollSectorLineItInitMacro(it, tree, line, padding);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpCollSectorLineItInitFromSectorIt initializes an
 * \ref RpCollSectorLineIt iterator object based on the current state
 * of an enclosing \ref RpSectorLineIt iteration loop.
 *
 * \param it     Pointer to the iterator structure.
 * \param tree   Pointer to the collision tree.
 * \param sectIt Pointer to the RpSector iterator.
 *
 * This function can be used instead of \ref RpCollSectorLineItInit, and
 * eliminates some of the work that it would otherwise do. The following code
 * shows an example.
 *
 * \code
    RpSectorLineIt      sectIt;
    RpCollSectorLineIt  collSectIt;

    RpSectorLineItInit(&sectIt, world, line, NULL);
    while (!RpSectorLineItFinished(&sectIt))
    {
        RpWorldSector *worldSector;
        RpCollTree    *collTree;

        worldSector = RpSectorLineItFindNextWorldSector(&sectIt);
        collTree = RpCollisionWorldSectorGetCollTree(worldSector);

        RpCollSectorLineItInitFromSectorIt(&collSectIt, collTree, &sectIt);
        while (!RpCollSectorLineItFinished(&collSectIt))
        {
            RwUInt16  start, count;

            RpCollSectorLineItFindNextEntries(&collSectIt, &start, &count);

            ... process triangles ...
        }
    }

 * \endcode
 *
 * \see \ref rpcollisoverview
 */
void
RpCollSectorLineItInitFromSectorIt(RpCollSectorLineIt  *it,
                                   RpCollTree          *tree,
                                   RpSectorLineIt      *sectIt)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorLineItInitFromSectorIt"));
    RWASSERT(it);
    RWASSERT(tree);
    RWASSERT(tree->numSplits > 0
        || tree->numEntries <= rpCOLLSECTOR_CONTENTS_MAXCOUNT);

    _rpCollSectorLineItInitFromSectorItMacro(it, tree, sectIt);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpCollSectorLineItFindNextEntries advances a collision sector line
 * iterator to the next leaf sector intersected, and returns a reference to
 * the entries contained in that sector ('entries' currently mean
 * the triangles of a world sector or geometry).
 *
 * \param   it      Pointer to the iterator structure.
 * \param   start   Pointer to a variable to receive the starting index
 *                  for the range of entries contained in the leaf sector.
 * \param   count   Pointer to a variable to receive the number of
 *                  entries in the range.
 *
 * The range indicated by the start and count values refers to the
 * collision ordered array of entries. In situations where the external array
 * of entries has not been ordered this way, the \ref RpCollTree contains
 * a remapping array (see \ref RpCollTreeGetEntryMap, and the example
 * code given in \ref RpCollSectorLineItInit).
 *
 * The entries themselves will lie close to the line but will not necessarily
 * intersect it. Further tests are necessary to determine this.
 *
 * \see RpCollSectorLineItFinished
 * \see RpCollSectorLineItSetFarClip
 * \see \ref rpcollisoverview
 */
void
RpCollSectorLineItFindNextEntries(RpCollSectorLineIt *it,
                                  RwUInt16           *start,
                                  RwUInt16           *count)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorLineItFindNextEntries"));
    RWASSERT(it);
    RWASSERT(start);
    RWASSERT(count);
    RWASSERT(it->curr > it->stack);
    RWASSERT(it->tree);

    _rpCollSectorLineItFindNextEntriesMacro(it, start, count);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpCollSectorLineItSetFarClip modifies a collision sector line
 * iterator to eliminate, from the iteration process, all sectors beyond
 * a given distance.
 *
 * \param it     Pointer to the iterator.
 * \param value  The far clip parameter in the range 0 to 1 (for
 *               the start and end of the line).
 *
 * This is analogous to the \ref RpSectorLineItSetFarClip function but
 * operates at the smaller scale of collision sectors. It is intended
 * for use when searching for the nearest intersection along
 * a line. See \ref RpCollSectorLineItInit for a code example.
 *
 * \see RpCollSectorLineItFinished
 * \see RpCollSectorLineItFindNextEntries
 * \see \ref rpcollisoverview
 */
void
RpCollSectorLineItSetFarClip(RpCollSectorLineIt *it, RwReal value)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorLineItSetFarClip"));
    RWASSERT(it);

    _rpCollLineItSetFarClipMacro(it, value);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpCollSectorBBoxItInit initializes a collision sector bounding box
 * iterator for traversing an \ref RpCollTree to find each sector
 * that intersects a given bounding box.
 *
 * \param it        Pointer to the iterator.
 * \param tree      Pointer to the collision tree.
 * \param outerBBox Pointer to a bounding box that completely encloses the
 *                  region of interest.
 * \param innerBBox Pointer to a bounding box that lies completely inside
 *                  the region of interest. This can be different to the outer
 *                  box when approximating collision regions of other shapes.
 *
 * The \p outerBBox is used for actually determining the \ref RpCollSector
 * intersections. The \p innerBBox is an optimization feature. The iterator
 * keeps track of \ref RpCollBBoxClipFlags as it traverses the tree, so that
 * they indicate whether the current sector is clipped by the \p innerBBox.
 * If the flags are zero then the sector must be completely
 * inside. The \ref RpCollSectorBBoxItFindNextEntries function returns
 * the clip flags relevant to each set of entries it finds.
 *
 * The following code shows an example of usage. This finds all triangles
 * in a world sector that lie inside a given bounding box (\c bbox). The
 * triangles are assumed not to be in the collision tree order, so the
 * entry map is used.
 *
 * \code

    const RpTriangle   *tris = RpWorldSectorGetTriangles(worldSector);
    const RwV3d        *verts = RpWorldSectorGetVertices(worldSector);
    RpCollTree         *tree = RpCollisionWorldSectorGetCollTree(worldSector);
    RwUInt16           *map = RpCollTreeGetEntryMap(tree);
    RpCollSectorBBoxIt  iterator;

    RpCollSectorBBoxItInit(&iterator, tree, bbox, bbox);
    while (!RpCollSectorBBoxItFinished(&iterator))
    {
        RwUInt16  i, start, count;
        RwUInt32  clipFlags;

        RpCollSectorBBoxItFindNextEntries(&it, &start, &count, &clipFlags);
        for (i=start; i<(start+count); i++)
        {
            RwUInt16          triIndex = map[i];
            const RpTriangle *tri = tris + triIndex;
            const RwV3d *v0 = verts + tri->vertIndex[0];
            const RwV3d *v1 = verts + tri->vertIndex[1];
            const RwV3d *v2 = verts + tri->vertIndex[2];

            if ((clipFlags == 0)
                || RtIntersectionBBoxTriangle(&bbox, v0, v1, v2))
            {
                ... process triangle ...
            }
        }
    }

 * \endcode
 *
 * \see \ref rpcollisoverview
 *
 */
void
RpCollSectorBBoxItInit(RpCollSectorBBoxIt *it,
                       RpCollTree         *tree,
                       RwBBox             *outerBBox,
                       RwBBox             *innerBBox)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorBBoxItInit"));
    RWASSERT(it);
    RWASSERT(tree);
    RWASSERT(outerBBox);
    RWASSERT(innerBBox);

    _rpCollSectorBBoxItInitMacro(it, tree, outerBBox, innerBBox);
    RWRETURNVOID();
}


/**
 * \ingroup rpcollis
 * \ref RpCollSectorBBoxItFindNextEntries advances a collision sector bounding
 * box iterator to the next leaf sector intersected, and returns a reference
 * to the entries contained in that sector ('entries' currently mean the
 * triangles of a world sector or geometry).
 *
 * \param it        Pointer to the iterator structure.
 * \param start     Pointer to a variable to receive the index
 *                  of the first entry in the range.
 * \param count     Pointer to a variable to receive the number
 *                  of entries in the range.
 * \param clipFlags Pointer to a variable that will receive the
 *                  \ref RpCollBBoxClipFlags for the sector containing the
 *                  entries. If zero, all entries are guaranteed to be
 *                  completely inside the inner bounding box that was passed
 *                  to \ref RpCollSectorBBoxItInit. If non-zero, then further
 *                  tests are required to determine whether each entry
 *                  intersects the region of interest.
 *
 * The range indicated by the start and count values refers to the
 * collision ordered array of entries. In situations where the external array
 * of entries has not been ordered this way, the \ref RpCollTree contains
 * a remapping array (see \ref RpCollTreeGetEntryMap).
 *
 * \see RpCollSectorBBoxItInit
 * \see RpCollSectorBBoxItFinished
 * \see \ref rpcollisoverview
 *
 */
void
RpCollSectorBBoxItFindNextEntries(RpCollSectorBBoxIt  *it,
                                  RwUInt16            *start,
                                  RwUInt16            *count,
                                  RwUInt32            *clipFlags)
{
    RWAPIFUNCTION(RWSTRING("RpCollSectorBBoxItFindNextEntries"));
    RWASSERT(it);
    RWASSERT(start);
    RWASSERT(count);
    RWASSERT(clipFlags);
    RWASSERT(it->curr > it->stack);
    RWASSERT(it->tree);

    _rpCollSectorBBoxItFindNextEntriesMacro(it, start, count, clipFlags);
    RWRETURNVOID();
}

