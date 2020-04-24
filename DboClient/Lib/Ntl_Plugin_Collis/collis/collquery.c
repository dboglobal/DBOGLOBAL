/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    collquery.c
 *  Purpose: World/Geometry/Atomic collision queries
 */

/******************************************************************************
 *  Include files
 */

#include "rwcore.h"
#include "rpplugin.h"
#include "rpdbgerr.h"
#include "rpworld.h"
#include "rtintsec.h"

#include "ctdata.h"
#include "ctquery.h"
#include "colldata.h"
#include "collquery.h"


/******************************************************************************
 *  Constants
 */

/* Half edge for cube completely inside unit radius sphere */
#define CUBEINSPHEREFACTOR 0.57f


/******************************************************************************
 *  Macros
 */

#ifdef RWDEBUG

#define MESSAGE_ONCE(args)                                                  \
MACRO_START                                                                 \
{                                                                           \
    static RwBool first = TRUE;                                             \
                                                                            \
    if (first)                                                              \
    {                                                                       \
        RWMESSAGE(args);                                                    \
        first = FALSE;                                                      \
    }                                                                       \
}                                                                           \
MACRO_STOP

#else /* RWDEBUG */

#define MESSAGE_ONCE(args)

#endif /* RWDEBUG */


/* Line delta (and lengthSq) cannot be zero */
#define LineSphereTestMacro(_result, _lineStart, _lineDelta,            \
            _lengthSq, _sphere, _centDist, _offsetSq)                   \
MACRO_START                                                             \
{                                                                       \
    /* Distances scaled by length to keep maths simple */               \
    RwV3d   vcenter, temp;                                              \
    RwReal  perpSq;                                                     \
                                                                        \
    /* Projection onto line of vector to sphere center */               \
    RwV3dSub(&vcenter, &(_sphere)->center, _lineStart);                 \
    *(_centDist) = RwV3dDotProduct(&vcenter, _lineDelta);               \
                                                                        \
    /* Square of shortest distance from center to line.                 \
     * NO pythagoras here otherwise we can lose precision. */           \
    RwV3dScale(&temp, &vcenter, _lengthSq);                             \
    RwV3dIncrementScaled(&temp, _lineDelta, -*(_centDist));             \
    perpSq = RwV3dDotProduct(&temp, &vcenter);                          \
                                                                        \
    /* Squared offset from centDist to sphere surface */                \
    *(_offsetSq) =                                                      \
        (_sphere)->radius * (_sphere)->radius * (_lengthSq) - perpSq;   \
                                                                        \
    /* Does line trajectory hit the sphere? */                          \
    if (*(_offsetSq) >= 0)                                              \
    {                                                                   \
        if (*(_centDist) < 0)                                           \
        {                                                               \
            /* Center before line start */                              \
            (_result) = ((*(_centDist) * *(_centDist)) <= *(_offsetSq));\
        }                                                               \
        else if (*(_centDist) > (_lengthSq))                            \
        {                                                               \
            /* Center after line end */                                 \
            RwReal diff = *(_centDist) - (_lengthSq);                   \
            (_result) = ((diff * diff) <= *(_offsetSq));                \
        }                                                               \
        else                                                            \
        {                                                               \
            (_result) = TRUE;                                           \
        }                                                               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        (_result) = FALSE;                                              \
    }                                                                   \
}                                                                       \
MACRO_STOP

/*
 *  Test intersection of sphere and bbox by comparing shortest distance
 *  from sphere center to bbox boundary with the radius of the sphere.
 *  Also return square of the shortest distance.
 */

#define IntervalDistMacro(_v, _a, _b)   \
    (((_v)<(_a)) ? ((_a)-(_v)) : (((_v)>(_b)) ? ((_v)-(_b)) : 0))

#define SphereBBoxTestMacro(_result, _sph, _bb, _distSq)                    \
MACRO_START                                                                 \
{                                                                           \
    RwReal  dist;                                                           \
                                                                            \
    dist = IntervalDistMacro((_sph)->center.x, (_bb)->inf.x, (_bb)->sup.x); \
    *(_distSq) = dist*dist;                                                 \
    dist = IntervalDistMacro((_sph)->center.y, (_bb)->inf.y, (_bb)->sup.y); \
    *(_distSq) += dist*dist;                                                \
    dist = IntervalDistMacro((_sph)->center.z, (_bb)->inf.z, (_bb)->sup.z); \
    *(_distSq) += dist*dist;                                                \
                                                                            \
    (_result) = (*(_distSq) <= ((_sph)->radius * (_sph)->radius));          \
}                                                                           \
MACRO_STOP


#define SphereBBoxMacro(_bb, _center, _rad)                     \
MACRO_START                                                     \
{                                                               \
    (_bb)->inf = (_bb)->sup = *(_center);                       \
    (_bb)->inf.x -= (_rad);                                     \
    (_bb)->inf.y -= (_rad);                                     \
    (_bb)->inf.z -= (_rad);                                     \
    (_bb)->sup.x += (_rad);                                     \
    (_bb)->sup.y += (_rad);                                     \
    (_bb)->sup.z += (_rad);                                     \
}                                                               \
MACRO_STOP

#define TriangleNormalMacro(_normal, _v0, _v1, _v2)             \
MACRO_START                                                     \
{                                                               \
    RwV3d               vTmp, vTmp2;                            \
    RwReal              recipLength;                            \
    RwReal              lengthSq;                               \
                                                                \
    RwV3dSubMacro(&vTmp, (_v1), (_v0));                         \
    RwV3dSubMacro(&vTmp2, (_v2), (_v0));                        \
    RwV3dCrossProductMacro((_normal), &vTmp, &vTmp2);           \
    lengthSq = RwV3dDotProduct((_normal), (_normal));           \
    rwInvSqrtMacro(&recipLength, lengthSq);                     \
    RwV3dScaleMacro((_normal), (_normal), recipLength);         \
}                                                               \
MACRO_STOP

#define CollisionTriangleInitMacro(_ct, _ind, _v0, _v1, _v2)    \
MACRO_START                                                     \
{                                                               \
    (_ct)->point = *(_v0);                                      \
    (_ct)->index = (_ind);                                      \
    (_ct)->vertices[0] = (_v0);                                 \
    (_ct)->vertices[1] = (_v1);                                 \
    (_ct)->vertices[2] = (_v2);                                 \
    TriangleNormalMacro(&(_ct)->normal, _v0, _v1, _v2);         \
}                                                               \
MACRO_STOP

#define LineStartNotEqualToEnd(_line)                           \
   (    ((_line)->start.x != (_line)->end.x)                    \
    ||  ((_line)->start.y != (_line)->end.y)                    \
    ||  ((_line)->start.z != (_line)->end.z))

/******************************************************************************
 *
 *  Interpolation macros for morphed atomics
 *
 *  The _v0, _v1, _v2 should be local pointers that will point to the
 *  interpolated vertices.
 *
 */

typedef struct
{
    RwV3d               tri[3];  /* Interpolation buffer */
    RwV3d              *vertsA;
    RwV3d              *vertsB;
    RwReal              fac;

} TriLerp;

#define V3dLerp(_v, _va, _vb, _blend)                           \
MACRO_START                                                     \
{                                                               \
    RwV3dSub(_v, _vb, _va);                                     \
    RwV3dScale(_v, _v, _blend);                                 \
    RwV3dAdd(_v, _v, _va);                                      \
}                                                               \
MACRO_STOP


#define TriLerpInitMacro(_lerp, _geom, _interp, _v0, _v1, _v2)              \
MACRO_START                                                                 \
{                                                                           \
    RWASSERT((_interp)->startMorphTarget < (_geom)->numMorphTargets);       \
    RWASSERT((_interp)->endMorphTarget < (_geom)->numMorphTargets);         \
                                                                            \
    (_lerp)->vertsA =                                                       \
        (_geom)->morphTarget[(_interp)->startMorphTarget].verts;            \
    (_lerp)->vertsB =                                                       \
        (_geom)->morphTarget[(_interp)->endMorphTarget].verts;              \
    (_lerp)->fac = (_interp)->recipTime * (_interp)->position;              \
                                                                            \
    {                                                                       \
        /* Init pointers to interp buffer */                                \
        (_v0) = &(_lerp)->tri[0];                                           \
        (_v1) = &(_lerp)->tri[1];                                           \
        (_v2) = &(_lerp)->tri[2];                                           \
    }                                                                       \
}                                                                           \
MACRO_STOP


#define TriLerpUpdateMacro(_lerp, _triangle, _v0, _v1, _v2)                 \
MACRO_START                                                                 \
{                                                                           \
    if ((_lerp)->vertsA == (_lerp)->vertsB)                                 \
    {                                                                       \
        (_v0) = (_lerp)->vertsA + (_triangle)->vertIndex[0];                \
        (_v1) = (_lerp)->vertsA + (_triangle)->vertIndex[1];                \
        (_v2) = (_lerp)->vertsA + (_triangle)->vertIndex[2];                \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RwV3d  *va, *vb;                                                    \
                                                                            \
        va = (_lerp)->vertsA + (_triangle)->vertIndex[0];                   \
        vb = (_lerp)->vertsB + (_triangle)->vertIndex[0];                   \
        V3dLerp(&(_lerp)->tri[0], va, vb, (_lerp)->fac);                    \
                                                                            \
        va = (_lerp)->vertsA + (_triangle)->vertIndex[1];                   \
        vb = (_lerp)->vertsB + (_triangle)->vertIndex[1];                   \
        V3dLerp(&(_lerp)->tri[1], va, vb, (_lerp)->fac);                    \
                                                                            \
        va = (_lerp)->vertsA + (_triangle)->vertIndex[2];                   \
        vb = (_lerp)->vertsB + (_triangle)->vertIndex[2];                   \
        V3dLerp(&(_lerp)->tri[2], va, vb, (_lerp)->fac);                    \
    }                                                                       \
}                                                                           \
MACRO_STOP


/******************************************************************************
 *
 *  Iteration over atomics in world sector
 */

typedef struct
{
    RwLLLink *cur;
    RwLLLink *end;

} WorldSectorAtomicIt;

#define WorldSectorAtomicItInitMacro(_it, _sector)                          \
MACRO_START                                                                 \
{                                                                           \
    RwLinkList *list = &(_sector)->collAtomicsInWorldSector;                \
    (_it).end = rwLinkListGetTerminator(list);                              \
    (_it).cur = rwLinkListGetFirstLLLink(list);                             \
}                                                                           \
MACRO_STOP

#define WorldSectorAtomicItFinishedMacro(_it)  ((_it)->cur == (_it)->end)

#define WorldSectorAtomicItGetNextMacro(_it, _atomic)                       \
MACRO_START                                                                 \
{                                                                           \
    RpTie  *tie = rwLLLinkGetData((_it).cur, RpTie, lAtomicInWorldSector);  \
    (_atomic) = tie->apAtom;                                                \
    (_it).cur = rwLLLinkGetNext((_it).cur);                                 \
}                                                                           \
MACRO_STOP



/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *      WorldForAllSectors....
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
WorldForAllSectorsIntsecLine(RpWorld *world,
                             RpIntersection *intersection,
                             RpIntersectionCallBackWorldSector callBack,
                             void *data)
{
    RpSectorLineIt  it;
    RpWorldSector  *sector;

    RWFUNCTION(RWSTRING("WorldForAllSectorsIntsecLine"));

    /* Iterate over sectors */
    _rpSectorLineItInitMacro(&it, world, &intersection->t.line, (RwV3d *)NULL);
    while (!RpSectorLineItFinished(&it))
    {
        _rpSectorLineItFindNextWorldSectorMacro(&it, &sector);
        if (!callBack(intersection, sector, data))
        {
            /* Early out */
            RWRETURNVOID();
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
WorldForAllSectorsIntsecBBox(RpWorld *world,
                             RwBBox *bbox,
                             RpIntersection *intersection,
                             RpIntersectionCallBackWorldSector callBack,
                             void *data)
{
    RpSectorBBoxIt  it;
    RpWorldSector  *sector;

    RWFUNCTION(RWSTRING("WorldForAllSectorsIntsecBBox"));

    /* Iterate over sectors */
    _rpSectorBBoxItInitMacro(&it, world, bbox);
    while (!RpSectorBBoxItFinished(&it))
    {
        _rpSectorBBoxItFindNextWorldSectorMacro(&it, &sector);
        if (!callBack(intersection, sector, data))
        {
            /* Early out */
            RWRETURNVOID();
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
WorldForAllSectorsIntsecSphere(RpWorld *world,
                               const RwSphere *sphere,
                               RpIntersection *intersection,
                               RpIntersectionCallBackWorldSector callBack,
                               void *data)
{
    RwBBox          bbox;
    RpSectorBBoxIt  it;

    RWFUNCTION(RWSTRING("WorldForAllSectorsIntsecSphere"));

    /* Get sphere, and enclosing bbox */
    SphereBBoxMacro(&bbox, &sphere->center, sphere->radius);

    /* Iterate over sectors */
    _rpSectorBBoxItInitMacro(&it, world, &bbox);
    while (!RpSectorBBoxItFinished(&it))
    {
        RpWorldSector  *sector;
        RwReal          distSq;
        RwBool          result;

        _rpSectorBBoxItFindNextWorldSectorMacro(&it, &sector);

        /* Check the sphere intersects the sector */
        SphereBBoxTestMacro(result, sphere, &sector->boundingBox, &distSq);
        if (result)
        {
            if (!callBack(intersection, sector, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}



/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *      WorldForAllAtomics....
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
WorldForAllAtomicsIntsecLine(RpWorld *world,
                             RpIntersection *is,
                             RpIntersectionCallBackAtomic callBack,
                             void *data)
{
    RwLine                 *line;
    RwV3d                   delta;
    RwReal                  lengthSq;
    RwUInt32                ref;
    RpSectorLineIt          sectIt;
    RpWorldSector          *sector;
    WorldSectorAtomicIt     atomicIt;
    RpAtomic               *atomic;

    RWFUNCTION(RWSTRING("WorldForAllAtomicsIntsecLine"));

    /* Line params */
    line = &is->t.line;
    RwV3dSubMacro(&delta, &line->end, &line->start);
    lengthSq = RwV3dDotProduct(&delta, &delta);

    /* Increment the global collision reference */
    ref = ++(_rpCollisionGlobalRefMacro());

    /* Iterate over sectors */
    _rpSectorLineItInitMacro(&sectIt, world, line, (RwV3d *)NULL);
    while (!RpSectorLineItFinished(&sectIt))
    {
        _rpSectorLineItFindNextWorldSectorMacro(&sectIt, &sector);

        /* Iterate over atomics */
        WorldSectorAtomicItInitMacro(atomicIt, sector);
        while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
        {
            WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

            /* Check that atomic hasn't already been tested via another
             * sector and that it is flagged for collision tests. */
            if (atomic
                && (_rpCollisionAtomicRefMacro(atomic) != ref)
                && (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
            {
                const RwSphere *sphere;
                RwReal          dist, offsetSq;
                RwBool          result;

                /* Mark it */
                _rpCollisionAtomicRefMacro(atomic) = ref;

                /* Test for intersection */
                sphere = RpAtomicGetWorldBoundingSphere(atomic);
                LineSphereTestMacro(result,
                    &line->start, &delta, lengthSq, sphere, &dist, &offsetSq);

                if (result)
                {
                    /* Return parametric distance */
                    dist /= lengthSq;
                    if (!callBack(is, sector, atomic, dist, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
WorldForAllAtomicsIntsecBBox(RpWorld *world,
                             RpIntersection *is,
                             RpIntersectionCallBackAtomic callBack,
                             void *data)
{
    RwBBox                 *bbox;
    RwUInt32                ref;
    RpSectorBBoxIt          sectIt;
    RpWorldSector          *sector;
    WorldSectorAtomicIt     atomicIt;
    RpAtomic               *atomic;

    RWFUNCTION(RWSTRING("WorldForAllAtomicsIntsecBBox"));

    /* Increment global collision reference */
    ref = ++(_rpCollisionGlobalRefMacro());

    /* Iterate over sectors */
    bbox = &is->t.box;
    _rpSectorBBoxItInitMacro(&sectIt, world, bbox);
    while (!RpSectorBBoxItFinished(&sectIt))
    {
        _rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &sector);

        /* Iterate over atomics */
        WorldSectorAtomicItInitMacro(atomicIt, sector);
        while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
        {
            WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

            /* Check that atomic hasn't already been tested via another
             * sector and that it is flagged for collision tests. */
            if (atomic
                && (_rpCollisionAtomicRefMacro(atomic) != ref)
                && (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
            {
                const RwSphere *sphere;
                RwReal          distSq;
                RwBool          result;

                /* Mark it */
                _rpCollisionAtomicRefMacro(atomic) = ref;

                /* Test for intersection */
                sphere = RpAtomicGetWorldBoundingSphere(atomic);
                SphereBBoxTestMacro(result, sphere, bbox, &distSq);
                if (result)
                {
                    if (!callBack(is, sector, atomic, 0.0f, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }

    RWRETURNVOID();
}


/*****************************************************************************
 */

static void
WorldForAllAtomicsIntsecSphere(RpWorld *world,
                               const RwSphere *sphere,
                               RpIntersection *is,
                               RpIntersectionCallBackAtomic callBack,
                               void *data)
{
    RwBBox                  bbox;
    RpAtomic               *sphereAtomic;
    RwUInt32                ref;
    RpSectorBBoxIt          sectIt;
    RpWorldSector          *sector;
    WorldSectorAtomicIt     atomicIt;
    RpAtomic               *atomic;

    RWFUNCTION(RWSTRING("WorldForAllAtomicsIntsecSphere"));

    /* Sphere info */
    SphereBBoxMacro(&bbox, &sphere->center, sphere->radius);
    sphereAtomic = (is->type == rpINTERSECTATOMIC) ?
                        (RpAtomic *)is->t.object : (RpAtomic *)NULL;

    /* Increment global collision reference */
    ref = ++(_rpCollisionGlobalRefMacro());

    /* Iterator over sectors */
    _rpSectorBBoxItInitMacro(&sectIt, world, &bbox);
    while (!RpSectorBBoxItFinished(&sectIt))
    {
        RwReal          distSq;
        RwBool          result;
        const RwBBox   *sectorBB;

        _rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &sector);

        /* Eliminate sectors not intersecting sphere */
        sectorBB = RpWorldSectorGetBBox(sector);
        SphereBBoxTestMacro(result, sphere, sectorBB, &distSq);
        if (!result)
            continue;

        /* Iterate over atomics */
        WorldSectorAtomicItInitMacro(atomicIt, sector);
        while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
        {
            WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

            /* Check that we're not colliding atomic with itself, that
             * we haven't already tested it via another sector and
             * that it is actually flagged for collision tests. */
            if (atomic
                && (atomic != sphereAtomic)
                && (_rpCollisionAtomicRefMacro(atomic) != ref)
                && (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
            {
                const RwSphere *atomSphere;
                RwV3d   sep;
                RwReal  sumRadSq;

                /* Mark it */
                _rpCollisionAtomicRefMacro(atomic) = ref;

                /* Test for intersection */
                atomSphere = RpAtomicGetWorldBoundingSphere(atomic);
                RwV3dSubMacro(&sep, &sphere->center, &atomSphere->center);
                distSq = RwV3dDotProduct(&sep, &sep);
                sumRadSq = sphere->radius + atomSphere->radius;
                sumRadSq = sumRadSq * sumRadSq;

                if (distSq <= sumRadSq)
                {
                    /* Normalize distance */
                    RwReal dist = distSq / sumRadSq;
                    rwSqrt(&dist, dist);

                    if (!callBack(is, sector, atomic, dist, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *      WorldForAllTriangles....
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
WorldForAllTriIntsecLine(RpWorld *world,
                         RpIntersection *is,
                         RpIntersectionCallBackWorldTriangle callBack,
                         void *data)
{
    RpSectorLineIt      sectIt;

    RWFUNCTION(RWSTRING("WorldForAllTriIntsecLine"));

    /* Iterator over the RpSectors */
    _rpSectorLineItInitMacro(&sectIt, world, &is->t.line, (RwV3d *)NULL);
    while (!RpSectorLineItFinished(&sectIt))
    {
        RpWorldSector      *worldSector;
        RpCollTree         *collTree;
        RpTriangle         *triangles;
        RwV3d              *verts;
        RpCollSectorLineIt  collIt;

        /* Get world sector data */
        _rpSectorLineItFindNextWorldSectorMacro(&sectIt, &worldSector);
        collTree = RpCollisionWorldSectorGetCollTree(worldSector);
        if (!collTree)
            continue;

        RWASSERT(worldSector->triangles);
        RWASSERT(worldSector->vertices);
        triangles = worldSector->triangles;
        verts = worldSector->vertices;

        /* Iterate over the RpCollSectors */
        _rpCollSectorLineItInitFromSectorItMacro(&collIt, collTree, &sectIt);
        while (!RpCollSectorLineItFinished(&collIt))
        {
            RwUInt16    i, start, count;

            /* Get next slice of polygons */
            _rpCollSectorLineItFindNextEntriesMacro(&collIt, &start, &count);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    index = collTree->map ? collTree->map[i] : i;
                RpTriangle *tri = triangles + index;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];
                RwReal      dist;
                RwBool      result;
                RpCollisionTriangle collTri;

                RtIntersectionLineTriangleMacro(
                    result, &sectIt.start, &sectIt.delta, v0, v1, v2, &dist);

                if (result)
                {
                    CollisionTriangleInitMacro(&collTri, index, v0, v1, v2);
                    if (!callBack(is, worldSector, &collTri, dist, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
WorldForAllTriIntsecBBox(RpWorld *world,
                         RpIntersection *is,
                         RpIntersectionCallBackWorldTriangle callBack,
                         void *data)
{
    RwBBox             *bbox = &is->t.box;
    RpSectorBBoxIt      sectIt;

    RWFUNCTION(RWSTRING("WorldForAllTriIntsecBBox"));

    /* Iterator over the RpSectors */
    _rpSectorBBoxItInitMacro(&sectIt, world, bbox);
    while (!RpSectorBBoxItFinished(&sectIt))
    {
        RpWorldSector      *worldSector;
        RpCollTree         *collTree;
        RpTriangle         *triangles;
        RwV3d              *verts;
        RpCollSectorBBoxIt  collIt;

        /* Get world sector data */
        _rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &worldSector);
        collTree = RpCollisionWorldSectorGetCollTree(worldSector);
        if (!collTree)
            continue;

        RWASSERT(worldSector->triangles);
        RWASSERT(worldSector->vertices);
        triangles = worldSector->triangles;
        verts = worldSector->vertices;

        /* Iterate over the RpCollSectors */
        _rpCollSectorBBoxItInitMacro(&collIt, collTree, bbox, bbox);
        while (!RpCollSectorBBoxItFinished(&collIt))
        {
            RwUInt16    i, start, count;
            RwUInt32    clip;

            /* Get next slice of polygons */
            _rpCollSectorBBoxItFindNextEntriesMacro(
                &collIt, &start, &count, &clip);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    index = collTree->map ? collTree->map[i] : i;
                RpTriangle *tri = triangles + index;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];
                RpCollisionTriangle collTri;

                /* Test individual triangle only if bbox clips them */
                if (!clip || RtIntersectionBBoxTriangle(bbox, v0, v1, v2))
                {
                    /* Note: distance not defined */
                    CollisionTriangleInitMacro(&collTri, index, v0, v1, v2);
                    if (!callBack(is, worldSector, &collTri, 0.0f, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }

    RWRETURNVOID();
}


/******************************************************************************
 */


static void
WorldForAllTriIntsecSphere(RpWorld *world,
                           const RwSphere *sphere,
                           RpIntersection *is,
                           RpIntersectionCallBackWorldTriangle callBack,
                           void *data)
{
    RwReal              recipRadius, radSq, innerSize;
    RwBBox              bbox, innerBBox;
    RpSectorBBoxIt      sectIt;

    RWFUNCTION(RWSTRING("WorldForAllTriIntsecSphere"));

    /* Get sphere, and inner/outer bboxes. The inner bbox used for clip test
     * must fit entirely inside sphere */
    radSq = sphere->radius * sphere->radius;
    recipRadius = 1 / sphere->radius;
    innerSize = CUBEINSPHEREFACTOR * sphere->radius;
    SphereBBoxMacro(&bbox, &sphere->center, sphere->radius);
    SphereBBoxMacro(&innerBBox, &sphere->center, innerSize);

    /* Iterator over the RpSectors */
    _rpSectorBBoxItInitMacro(&sectIt, world, &bbox);
    while (!RpSectorBBoxItFinished(&sectIt))
    {
        RpWorldSector      *worldSector;
        RwReal              distSq;
        RpCollTree         *collTree;
        RpTriangle         *triangles;
        RwV3d              *verts;
        RpCollSectorBBoxIt  collIt;
        RwBool              result;

        /* Get world sector */
        _rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &worldSector);
        collTree = RpCollisionWorldSectorGetCollTree(worldSector);
        if (!collTree)
            continue;

        /* Skip sectors not touching sphere */
        SphereBBoxTestMacro(result, sphere, &collTree->bbox, &distSq);
        if (!result)
            continue;

        /* Get sector data */
        RWASSERT(worldSector->triangles);
        RWASSERT(worldSector->vertices);
        triangles = worldSector->triangles;
        verts = worldSector->vertices;

        /* Iterate over the RpCollSectors */
        _rpCollSectorBBoxItInitMacro(&collIt, collTree, &bbox, &innerBBox);
        while (!RpCollSectorBBoxItFinished(&collIt))
        {
            RwUInt16    i, start, count;
            RwUInt32    clip;

            /* Get next slice of polygons */
            _rpCollSectorBBoxItFindNextEntriesMacro(
                &collIt, &start, &count, &clip);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    iPoly = collTree->map ? collTree->map[i] : i;
                RpTriangle *tri = triangles + iPoly;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];
                RwReal      dist;
                RpCollisionTriangle collTri;

                if (!clip)
                {
                    /* Triangle is entirely inside inner bbox */
                    RwV3d  offset;

                    TriangleNormalMacro(&collTri.normal, v0, v1, v2);
                    RwV3dSubMacro(&offset, &sphere->center, v0);
                    dist = RwV3dDotProduct(&offset, &collTri.normal);
                }
                else if (!RtIntersectionSphereTriangle(
                            sphere, v0, v1, v2, &collTri.normal, &dist))
                {
                    continue;
                }

                /* Normalize distance */
                dist *= recipRadius;

                collTri.point = (*v0);
                collTri.index = iPoly;
                collTri.vertices[0] = v0;
                collTri.vertices[1] = v1;
                collTri.vertices[2] = v2;

                if (!callBack(is, worldSector, &collTri, dist, data))
                {
                    /* Early out */
                    RWRETURNVOID();
                }
            }
        }
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *          Geometry without collision trees (slow...)
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
SimpleGeomForAllTriIntsecLine(RpGeometry *geometry,
                              RwLine *line,
                              RpIntersection *is,
                              RpIntersectionCallBackGeometryTriangle callBack,
                              void *data)
{
    RwV3d               delta;
    RwV3d              *verts;
    RpTriangle         *tri;
    RwInt32             i;

    RWFUNCTION(RWSTRING("SimpleGeomForAllTriIntsecLine"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    /* Line param */
    RwV3dSubMacro(&delta, &line->end, &line->start);

    /* Iterate over all triangles */
    verts = geometry->morphTarget->verts;
    for (i=0, tri=geometry->triangles; i < geometry->numTriangles; i++, tri++)
    {
        RwV3d *v0 = verts + tri->vertIndex[0];
        RwV3d *v1 = verts + tri->vertIndex[1];
        RwV3d *v2 = verts + tri->vertIndex[2];
        RwBool              result;
        RwReal              dist;
        RpCollisionTriangle collTri;

        RtIntersectionLineTriangleMacro(
            result, &line->start, &delta, v0, v1, v2, &dist);

        if (result)
        {
            CollisionTriangleInitMacro(&collTri, i, v0, v1, v2);
            if (!callBack(is, &collTri, dist, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}


/******************************************************************************
 */

static void
SimpleGeomForAllTriIntsecBBox(RpGeometry *geometry,
                              RpIntersection *is,
                              RpIntersectionCallBackGeometryTriangle callBack,
                              void *data)
{
    RwBBox             *bbox;
    RwV3d              *verts;
    RpTriangle         *tri;
    RwInt32             i;

    RWFUNCTION(RWSTRING("SimpleGeomForAllTriIntsecBBox"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    /* BBox */
    bbox = &is->t.box;

    /* Iterate over all triangles */
    verts = geometry->morphTarget->verts;
    for (i=0, tri=geometry->triangles; i < geometry->numTriangles; i++, tri++)
    {
        RwV3d *v0 = verts + tri->vertIndex[0];
        RwV3d *v1 = verts + tri->vertIndex[1];
        RwV3d *v2 = verts + tri->vertIndex[2];

        /* Test for intersection */
        if (RtIntersectionBBoxTriangle(bbox, v0, v1, v2))
        {
            RpCollisionTriangle collTri;

            /* Note: distance not defined */
            CollisionTriangleInitMacro(&collTri, i, v0, v1, v2);
            if (!callBack(is, &collTri, 0.0f, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
SimpleGeomForAllTriIntsecSphere(RpGeometry *geometry,
                                const RwSphere *sphere,
                                RpIntersection *is,
                                RpIntersectionCallBackGeometryTriangle callBack,
                                void *data)
{
    RwReal              recipRadius;
    RwV3d              *verts;
    RpTriangle         *tri;
    RwInt32             i;

    RWFUNCTION(RWSTRING("SimpleGeomForAllTriIntsecSphere"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    /* Sphere params */
    recipRadius = 1/sphere->radius;

    /* Iterate over all triangles */
    verts = geometry->morphTarget->verts;
    for (i=0, tri=geometry->triangles; i < geometry->numTriangles; i++, tri++)
    {
        RwV3d *v0 = verts + tri->vertIndex[0];
        RwV3d *v1 = verts + tri->vertIndex[1];
        RwV3d *v2 = verts + tri->vertIndex[2];
        RpCollisionTriangle collTri;
        RwReal              dist;

        if (RtIntersectionSphereTriangle(
                sphere, v0, v1, v2, &collTri.normal, &dist))
        {
            /* Normalize distance */
            dist *= recipRadius;

            collTri.point = (*v0);
            collTri.index = i;
            collTri.vertices[0] = v0;
            collTri.vertices[1] = v1;
            collTri.vertices[2] = v2;

            if (!callBack(is, &collTri, dist, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *          Geometry Collisions
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
GeometryForAllTriIntsecLine(RpGeometry *geometry,
                            RwLine *line,
                            RpIntersection *is,
                            RpIntersectionCallBackGeometryTriangle callBack,
                            void *data)
{
    RpCollTree         *tree;

    RWFUNCTION(RWSTRING("GeometryForAllTriIntsecLine"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    tree = RpCollisionGeometryGetCollTree(geometry);
    if (tree)
    {
        RwV3d               delta;
        RwV3d              *verts;
        RpTriangle         *tris;
        RpCollSectorLineIt  it;

        /* Line param */
        RwV3dSubMacro(&delta, &line->end, &line->start);

        /* Geometry data */
        verts = geometry->morphTarget->verts;
        tris = geometry->triangles;

        /* Iterate over collision sectors */
        _rpCollSectorLineItInitMacro(&it, tree, line, (RwV3d *)NULL);
        while (!RpCollSectorLineItFinished(&it))
        {
            RwUInt16    i, start, count;

            /* Get next slice of triangles */
            _rpCollSectorLineItFindNextEntriesMacro(&it, &start, &count);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    index = tree->map ? tree->map[i] : i;
                RpTriangle *tri = tris + index;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];
                RwBool      result;
                RwReal      dist;

                RtIntersectionLineTriangleMacro(
                    result, &line->start, &delta, v0, v1, v2, &dist);

                if (result)
                {
                    RpCollisionTriangle collTri;

                    CollisionTriangleInitMacro(&collTri, index, v0, v1, v2);
                    if (!callBack(is, &collTri, dist, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }
    else
    {
        /* No collision tree */
        MESSAGE_ONCE((RWSTRING(
            "Geometry has no collision tree, so every triangle will be tested. "
            "RpCollisionGeometryBuildData can be used to improve performance. "
            "This message will not be repeated.")));

        SimpleGeomForAllTriIntsecLine(geometry, line, is, callBack, data);
    }

    RWRETURNVOID();
}


/******************************************************************************
 */

static void
GeometryForAllTriIntsecBBox(RpGeometry *geometry,
                            RpIntersection *is,
                            RpIntersectionCallBackGeometryTriangle callBack,
                            void *data)
{
    RpCollTree         *tree;

    RWFUNCTION(RWSTRING("GeometryForAllTriIntsecBBox"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    tree = RpCollisionGeometryGetCollTree(geometry);
    if (tree)
    {
        RwBBox             *bbox;
        RwV3d              *verts;
        RpTriangle         *tris;
        RpCollSectorBBoxIt  it;

        /* BBox */
        bbox = &is->t.box;

        /* Geometry data */
        verts = geometry->morphTarget->verts;
        tris = geometry->triangles;

        /* Iterate over collision sectors */
        _rpCollSectorBBoxItInitMacro(&it, tree, bbox, bbox);
        while (!RpCollSectorBBoxItFinished(&it))
        {
            RwUInt16    i, start, count;
            RwUInt32    clip;

            /* Get next slice of triangles */
            _rpCollSectorBBoxItFindNextEntriesMacro(
                &it, &start, &count, &clip);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    index = tree->map ? tree->map[i] : i;
                RpTriangle *tri = tris + index;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];

                /* Test individual triangle only if bbox clips them */
                if (!clip || RtIntersectionBBoxTriangle(bbox, v0, v1, v2))
                {
                    RpCollisionTriangle collTri;

                    /* Note: distance not defined */
                    CollisionTriangleInitMacro(&collTri, index, v0, v1, v2);
                    if (!callBack(is, &collTri, 0.0f, data))
                    {
                        /* Early out */
                        RWRETURNVOID();
                    }
                }
            }
        }
    }
    else
    {
        /* No collision tree */
        MESSAGE_ONCE((RWSTRING(
            "Geometry has no collision tree, so every triangle will be tested. "
            "RpCollisionGeometryBuildData can be used to improve performance. "
            "This message will not be repeated.")));

        SimpleGeomForAllTriIntsecBBox(geometry, is, callBack, data);
    }

    RWRETURNVOID();
}

/******************************************************************************
 */

static void
GeometryForAllTriIntsecSphere(RpGeometry *geometry,
                              const RwSphere *sphere,
                              RpIntersection *is,
                              RpIntersectionCallBackGeometryTriangle callBack,
                              void *data)
{
    RpCollTree         *tree;

    RWFUNCTION(RWSTRING("GeometryForAllTriIntsecSphere"));
    RWASSERT(geometry->triangles || geometry->numTriangles == 0);

    tree = RpCollisionGeometryGetCollTree(geometry);
    if (tree)
    {
        RwReal              recipRadius, innerSize, distSq;
        RwBBox              bbox;
        RwBBox              innerBBox;
        RwV3d              *verts;
        RpTriangle         *tris;
        RpCollSectorBBoxIt  it;
        RwBool              result;

        /* Check for overlap with bbox bounding geometry */
        SphereBBoxTestMacro(result, sphere, &tree->bbox, &distSq);
        if (!result)
        {
            RWRETURNVOID();
        }

        /* Sphere params */
        recipRadius = 1/sphere->radius;
        innerSize = CUBEINSPHEREFACTOR * sphere->radius;
        SphereBBoxMacro(&bbox, &sphere->center, sphere->radius);
        SphereBBoxMacro(&innerBBox, &sphere->center, innerSize);

        /* Geometry data */
        verts = geometry->morphTarget->verts;
        tris = geometry->triangles;

        /* Iterate over collision sectors */
        _rpCollSectorBBoxItInitMacro(&it, tree, &bbox, &innerBBox);
        while (!RpCollSectorBBoxItFinished(&it))
        {
            RwUInt16    i, start, count;
            RwUInt32    clip;

            /* Get next slice of triangles */
            _rpCollSectorBBoxItFindNextEntriesMacro(
                &it, &start, &count, &clip);
            for (i = start; i < (start + count); i++)
            {
                RwUInt32    index = tree->map ? tree->map[i] : i;
                RpTriangle *tri = tris + index;
                RwV3d      *v0 = verts + tri->vertIndex[0];
                RwV3d      *v1 = verts + tri->vertIndex[1];
                RwV3d      *v2 = verts + tri->vertIndex[2];
                RwReal      dist;
                RpCollisionTriangle collTri;

                if (!clip)
                {
                    /* Triangle is entirely inside inner bbox */
                    RwV3d  offset;

                    TriangleNormalMacro(&collTri.normal, v0, v1, v2);
                    RwV3dSubMacro(&offset, &sphere->center, v0);
                    dist = RwV3dDotProduct(&offset, &collTri.normal);
                }
                else if (!RtIntersectionSphereTriangle(
                            sphere, v0, v1, v2, &collTri.normal, &dist))
                {
                    continue;
                }

                /* Normalize distance */
                dist *= recipRadius;

                collTri.point = (*v0);
                collTri.index = index;
                collTri.vertices[0] = v0;
                collTri.vertices[1] = v1;
                collTri.vertices[2] = v2;

                if (!callBack(is, &collTri, dist, data))
                {
                    /* Early out */
                    RWRETURNVOID();
                }
            }
        }
    }
    else
    {
        /* No collision tree */
        MESSAGE_ONCE((RWSTRING(
            "Geometry has no collision tree, so every triangle will be tested. "
            "RpCollisionGeometryBuildData can be used to improve performance. "
            "This message will not be repeated.")));

        SimpleGeomForAllTriIntsecSphere(geometry, sphere, is, callBack, data);
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *          Morph geometry intersections (slow...)
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
MorphGeomForAllTriIntsecLine(RpGeometry     *geom,
                             RpInterpolator *interp,
                             RwLine         *line,
                             RpIntersection *is,
                             RpIntersectionCallBackGeometryTriangle callBack,
                             void           *data)
{
    TriLerp         lerp;
    RwV3d           delta;
    RwInt32         i;
    RwV3d          *v0, *v1, *v2;
    RpTriangle     *tri;

    RWFUNCTION(RWSTRING("MorphGeomForAllTriIntsecLine"));
    RWASSERT(geom);
    RWASSERT(interp);
    RWASSERT(geom->triangles || geom->numTriangles == 0);

    /* Store line delta */
    RwV3dSubMacro(&delta, &line->end, &line->start);

    /* Initialize triangle interpolation */
    TriLerpInitMacro(&lerp, geom, interp, v0, v1, v2);

    /* Iterate over all triangles */
    for (i=0, tri=geom->triangles; i < geom->numTriangles; i++, tri++)
    {
        RwReal  dist;
        RwBool  result;
        RpCollisionTriangle collTri;

        /* Interpolate next triangle */
        TriLerpUpdateMacro(&lerp, tri, v0, v1, v2);

        /* Test for intersection */
        RtIntersectionLineTriangleMacro(
            result, &line->start, &delta, v0, v1, v2, &dist);

        if (result)
        {
            CollisionTriangleInitMacro(&collTri, i, v0, v1, v2);
            if (!callBack(is, &collTri, dist, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}


/******************************************************************************
 */

static void
MorphGeomForAllTriIntsecSphere(RpGeometry     *geom,
                               RpInterpolator *interp,
                               RwSphere       *sphere,
                               RpIntersection *is,
                               RpIntersectionCallBackGeometryTriangle callBack,
                               void           *data)
{
    RwReal          recipRad;
    TriLerp         lerp;
    RwV3d          *v0, *v1, *v2;
    RwInt32         i;
    RpTriangle     *tri;

    RWFUNCTION(RWSTRING("MorphGeomForAllTriIntsecSphere"));
    RWASSERT(geom);
    RWASSERT(interp);
    RWASSERT(geom->triangles || geom->numTriangles == 0);

    /* Cache reciprocal radius */
    recipRad = 1.0f / sphere->radius;

    /* Initialize triangle interpolation */
    TriLerpInitMacro(&lerp, geom, interp, v0, v1, v2);

    /* Iterate over all triangles */
    for (i=0, tri=geom->triangles; i < geom->numTriangles; i++, tri++)
    {
        RwReal  dist;
        RpCollisionTriangle collTri;

        /* Interpolate next triangle */
        TriLerpUpdateMacro(&lerp, tri, v0, v1, v2);

        /* Test for intersection */
        if (RtIntersectionSphereTriangle(
                sphere, v0, v1, v2, &collTri.normal, &dist))
        {
            collTri.point = (*v0);
            collTri.index = i;
            collTri.vertices[0] = v0;
            collTri.vertices[1] = v1;
            collTri.vertices[2] = v2;

            /* Normalize distance (0 - 1) */
            dist *= recipRad;
            if (!callBack(is, &collTri, dist, data))
            {
                /* Early out */
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *          Atomic intersections
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */


static void
AtomicForAllTriIntsecLine(RpAtomic *atomic,
                          RpIntersection *is,
                          RpIntersectionCallBackGeometryTriangle callBack,
                          void *data)
{
    RwLine         *line;
    RpGeometry     *geom;
    RwMatrix        inverseLTM;
    RwLine          localLine;

    RWFUNCTION(RWSTRING("AtomicForAllTriIntsecLine"));

    line = &is->t.line;
    geom = atomic->geometry;
    RWASSERT(geom);

    /* Transform the line into the frame of the atomic */
    RwMatrixInvert(&inverseLTM, RwFrameGetLTM(RpAtomicGetFrame(atomic)));
    RwV3dTransformPoints(&localLine.start, &line->start, 2, &inverseLTM);

    /* Now collide with geometry */
    if (geom->numMorphTargets > 1)
    {
        MorphGeomForAllTriIntsecLine(
            geom, &atomic->interpolator, &localLine, is, callBack, data);
    }
    else
    {
        GeometryForAllTriIntsecLine(geom, &localLine, is, callBack, data);
    }

    RWRETURNVOID();
}


/******************************************************************************
 */

static void
AtomicForAllTriIntsecSphere(RpAtomic *atomic,
                            const RwSphere  *sphere,
                            RpIntersection *is,
                            RpIntersectionCallBackGeometryTriangle callBack,
                            void *data)
{
    RpGeometry         *geom;
    RwSphere            localSphere;
    RwMatrix            inverseLTM;
    RwReal              scaleSq, scale;

    RWFUNCTION(RWSTRING("AtomicForAllTriIntsecSphere"));

    /* Get geometry */
    geom = atomic->geometry;
    RWASSERT(geom);

    /* Get transform from world space to frame of atomic */
    RwMatrixInvert(&inverseLTM, RwFrameGetLTM(RpAtomicGetFrame(atomic)));

    /* Transform the sphere position into object space */
    RwV3dTransformPoints(&localSphere.center, &sphere->center, 1, &inverseLTM);

    /* Get sphere radius in object space - assume uniform scaling */
    scaleSq = RwV3dDotProduct(&inverseLTM.at, &inverseLTM.at);
    rwSqrt(&scale, scaleSq);
    localSphere.radius = scale * sphere->radius;

    /* Now collide with geometry */
    if (geom->numMorphTargets > 1)
    {
        MorphGeomForAllTriIntsecSphere(
            geom, &atomic->interpolator, &localSphere, is, callBack, data);
    }
    else
    {
        GeometryForAllTriIntsecSphere(geom, &localSphere, is, callBack, data);
    }

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *      API INTERFACE
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

/**
 * \ingroup rpcollis
 * \ref RpWorldForAllWorldSectorIntersections returns, via a callback
 * function, all world sectors in a world that intersect a given primitive.
 * The callback may return NULL to terminate intersection testing.
 *
 * \param world         Pointer to the world.
 * \param intersection  Pointer to an \ref RpIntersection value describing
 *                      the intersection primitive. Supported types are:
 *                      \li rpINTERSECTPOINT    Point intersections.
 *                      \li rpINTERSECTLINE     Line intersections (the length
 *                                              should not be zero).
 *                      \li rpINTERSECTSPHERE   Sphere intersections.
 *                      \li rpINTERSECTBOX      Box intersections.
 *                      \li rpINTERSECTATOMIC   Atomic intersections (based on
 *                                              the atomic's bounding sphere).
 * \param callBack      Pointer to the \ref RpIntersectionCallBackWorldSector
 *                      callback function.
 * \param data          Pointer to the user supplied data to pass to the
 *                      callback function.
 *
 * \return  Returns a pointer to the world (even if no intersections were
 *          found).
 *
 * The intersection volume of a world sector is equivalent to its bounding box
 * (see \ref RpWorldSectorGetBBox).
 *
 * An alternative to this function is to use the lower-level RpSector
 * iterator API (\ref RpSectorLineIt, \ref RpSectorBBoxIt) to find world
 * sectors that intersect a line or bounding box.
 *
 * \see RpWorldForAllAtomicIntersections
 * \see RpCollisionWorldForAllIntersections
 * \see \ref rpcollisoverview
 */
RpWorld *
RpWorldForAllWorldSectorIntersections(
    RpWorld *world,
    RpIntersection *intersection,
    RpIntersectionCallBackWorldSector callBack,
    void *data)
{
    RWAPIFUNCTION(RWSTRING("RpWorldForAllWorldSectorIntersections"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(world);
    RWASSERT(intersection);
    RWASSERT(callBack);

    switch (intersection->type)
    {
    case rpINTERSECTPOINT:
        {
            RwBBox  bbox;

            bbox.inf = intersection->t.point;
            bbox.sup = intersection->t.point;

            WorldForAllSectorsIntsecBBox(
                world, &bbox, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTLINE:
        {
            RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
            WorldForAllSectorsIntsecLine(
                world, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTSPHERE:
        {
            WorldForAllSectorsIntsecSphere(
                world, &intersection->t.sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTBOX:
        {
            WorldForAllSectorsIntsecBBox(
                world, &intersection->t.box, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTATOMIC:
        {
            RpAtomic *atomic = (RpAtomic *)intersection->t.object;
            RwSphere sphere;

            RWASSERT(atomic);
            RWASSERTISTYPE(atomic, rpATOMIC);
            sphere = *RpAtomicGetWorldBoundingSphere(atomic);

            WorldForAllSectorsIntsecSphere(
                world, &sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    default:
        {
            RWASSERT(0);
            RWMESSAGE(("Unsupported or invalid intersection type."));
            RWRETURN(world);
        }
    }
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionWorldForAllIntersections returns, via a callback function,
 * all triangles in the static geometry of a world that intersect a given
 * primitive. The call back function may return NULL to terminate
 * intersection testing.
 *
 * \param world         Pointer to the world.
 * \param intersection  Pointer to an \ref RpIntersection value describing
 *                      the intersection primitive. Supported types are:
 *                      \li rpINTERSECTLINE     Line intersections (the length
 *                          should not be zero).
 *                      \li rpINTERSECTSPHERE   Sphere intersections.
 *                      \li rpINTERSECTBOX      Box intersections.
 *                      \li rpINTERSECTATOMIC   Atomic intersections. Only
 *                          the atomic’s world-space bounding sphere is tested
 *                          against triangles in the world. A fine-grained
 *                          intersection test between the atomic’s
 *                          triangles and the world is not performed.
 * \param callBack      Pointer to the \ref RpIntersectionCallBackWorldTriangle
 *                      callback function.
 * \param data          Pointer to the user supplied data to pass to the
 *                      callback function.
 *
 * \return  Returns a pointer to the world (even if no intersections were
 *          found).
 *
 * \note Collision data must exist for the world (see
 * \ref RpCollisionWorldBuildData). The intersection test uses the world BSP
 * structure and the additional collision data in each world sector to
 * quickly isolate triangles that potentially intersect the primitive before
 * individually testing them.
 *
 * For line queries, the triangles are returned in approximate order of
 * distance along the line. It should not be assumed that the first triangle
 * returned is the nearest.
 *
 * A more flexible way to construct intersection tests similar to this function
 * is to use the iterator API to query the world's BSP tree, then each
 * resulting world sector's collision tree before doing precise triangle tests.
 * (see \ref RpSectorLineIt, \ref RpSectorBBoxIt,
 * \ref RpCollisionWorldSectorGetCollTree, \ref RpCollSectorLineIt,
 * \ref RpCollSectorBBoxIt, \ref rtintersectionoverview). In particular, the
 * iterator API provides a method to efficiently determine the first
 * intersection along a line.
 *
 * \see RpWorldForAllWorldSectorIntersections
 * \see RpWorldForAllAtomicIntersections
 * \see RpAtomicForAllIntersections
 * \see RpCollisionGeometryForAllIntersections
 * \see \ref rpcollisoverview
 */
RpWorld *
RpCollisionWorldForAllIntersections(
    RpWorld *world,
    RpIntersection *intersection,
    RpIntersectionCallBackWorldTriangle callBack,
    void *data)
{
    RWAPIFUNCTION(RWSTRING("RpCollisionWorldForAllIntersections"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(world);
    RWASSERT(intersection);
    RWASSERT(callBack);

    switch (intersection->type)
    {
    case rpINTERSECTLINE:
        {
            RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
            WorldForAllTriIntsecLine(
                world, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTSPHERE:
        {
            WorldForAllTriIntsecSphere(
                world, &intersection->t.sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    case rpINTERSECTBOX:
        {
            WorldForAllTriIntsecBBox(
                world, intersection, callBack, data);
            RWRETURN(world);
        }

    case (rpINTERSECTATOMIC):
        {
            RpAtomic *atomic = (RpAtomic *)intersection->t.object;
            RwSphere sphere;

            RWASSERT(atomic);
            RWASSERTISTYPE(atomic, rpATOMIC);
            sphere = *RpAtomicGetWorldBoundingSphere(atomic);

            WorldForAllTriIntsecSphere(
                world, &sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    default:
        {
            RWASSERT(0);
            RWMESSAGE(("Unsupported or invalid intersection type."));
            RWRETURN(world);
        }
    }
}

/**
 * \ingroup rpcollis
 * \ref RpWorldForAllAtomicIntersections returns, via a callback function,
 * all collision atomics in a world whose bounding spheres intersect a given
 * primitive. The callback may return NULL to terminate intersection tests.
 *
 * \param world         Pointer to the world containing the atomics.
 * \param intersection  Pointer to an \ref RpIntersection value describing the
 *                      intersection primitive. Supported types are:
 *                      \li rpINTERSECTPOINT    Point intersections.
 *                      \li rpINTERSECTLINE     Line intersections (the
 *                                              length should not be zero).
 *                      \li rpINTERSECTSPHERE   Sphere intersections.
 *                      \li rpINTERSECTATOMIC   Atomic intersections (based on
 *                                              the atomic's bounding sphere).
 *                      \li rpINTERSECTBOX      Box intersections.
 * \param callBack      Pointer to the \ref RpIntersectionCallBackAtomic
 *                      callback function.
 * \param data          Pointer to the user supplied data to pass to the
 *                      callback function.
 *
 * \return  Returns a pointer to the world (even if no intersections were
 *          found).
 *
 * Note that tests are only made on collision atomics, that is, those which
 * have their collision flag set (see \ref RpAtomicFlag).
 *
 * For a more precise intersection test with the triangles of an atomic's
 * geometry, use \ref RpAtomicForAllIntersections or
 * \ref RpCollisionGeometryForAllIntersections.
 *
 * \see RpWorldForAllWorldSectorIntersections
 * \see RpCollisionWorldForAllIntersections
 * \see RpWorldSectorForAllCollisionAtomics
 * \see \ref rpcollisoverview
 */
RpWorld *
RpWorldForAllAtomicIntersections(RpWorld *world,
                                 RpIntersection *intersection,
                                 RpIntersectionCallBackAtomic callBack,
                                 void *data)
{
    RWAPIFUNCTION(RWSTRING("RpWorldForAllAtomicIntersections"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(world);
    RWASSERT(intersection);
    RWASSERT(callBack);

    /* Temporary fix: we resynch all dirty frames in case users do simulation
     * updates inside a camera update. Ideally (eventually), we will move
     * towards a system where the users do simulation updates whenever they
     * like and frame resynching is entirely their responsibility. */
    _rwFrameSyncDirty();

    switch (intersection->type)
    {
    case (rpINTERSECTPOINT):
        {
            RwSphere sphere;

            sphere.center = intersection->t.point;
            sphere.radius = 0;

            WorldForAllAtomicsIntsecSphere(
                world, &sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    case (rpINTERSECTLINE):
        {
            RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
            WorldForAllAtomicsIntsecLine(
                world, intersection, callBack, data);
            RWRETURN(world);
        }

    case (rpINTERSECTSPHERE):
        {
            WorldForAllAtomicsIntsecSphere(
                world, &intersection->t.sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    case (rpINTERSECTBOX):
        {
            WorldForAllAtomicsIntsecBBox(
                world, intersection, callBack, data);
            RWRETURN(world);
        }

    case (rpINTERSECTATOMIC):
        {
            RpAtomic *atomic = (RpAtomic *)intersection->t.object;
            RwSphere sphere;

            RWASSERT(atomic);
            RWASSERTISTYPE(atomic, rpATOMIC);
            sphere = *RpAtomicGetWorldBoundingSphere(atomic);

            WorldForAllAtomicsIntsecSphere(
                world, &sphere, intersection, callBack, data);
            RWRETURN(world);
        }

    default:
        {
            RWASSERT(0);
            RWMESSAGE(("Unsupported or invalid intersection type."));
            RWRETURN(world);
        }
    }
}


/**
 * \ingroup rpcollis
 * \ref RpCollisionGeometryForAllIntersections returns, via a callback
 * function, all triangles in a geometry that intersect a given primitive.
 * The callback may return NULL to terminate intersection testing.
 *
 * \param geometry      Pointer to the geometry.
 * \param intersection  Pointer to an \ref RpIntersection describing
 *                      the intersection primitive, which must be specified
 *                      in the <b>local coordinate system</b> of the geometry
 *                      (this may require transformation from world
 *                      coordinates using the inverse LTM of the parent
 *                      atomic). Supported intersection types are:
 *                      \li rpINTERSECTLINE      Line intersections
 *                          (the length should not be zero).
 *                      \li rpINTERSECTSPHERE    Sphere intersections.
 *                      \li rpINTERSECTBOX       Box intersections.
 *                      \li rpINTERSECTATOMIC    Atomic intersections (based
 *                          on the atomic's bounding sphere).
 * \param callBack      Pointer to the
 *                      \ref RpIntersectionCallBackGeometryTriangle
 *                      callback function.
 * \param data          Pointer to the user supplied data to pass to the
 *                      callback function.
 *
 * \return  Returns a pointer to the geometry (even if no intersections
 *          were found).
 *
 * This function is compatible with a geometry whether or not it has
 * collision extension data, but performance will be significantly improved
 * for complex models if the data is present. It can be generated using
 * \ref RpCollisionGeometryBuildData (this may commonly be done when exporting
 * the model).
 *
 * If the geometry has multiple morph targets, only the first is tested for
 * intersections. To test for intersections with a morphed atomic, use
 * \ref RpAtomicForAllIntersections.
 *
 * For line queries, the triangles are returned in approximate order of
 * distance along the line. It should not be assumed that the first triangle
 * returned is the nearest.
 *
 * A more flexible way to construct intersection tests similar to this function
 * is to query a geometry's collision tree with the iterator API before
 * doing precise triangle tests. (see \ref RpCollisionGeometryGetCollTree,
 * \ref RpCollSectorLineIt, \ref RpCollSectorBBoxIt,
 * \ref rtintersectionoverview). In particular, the iterator API provides a
 * method to efficiently determine the first intersection along a line.
 *
 * \see RpAtomicForAllIntersections
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionWorldForAllIntersections
 * \see \ref rpcollisoverview
 */
RpGeometry *
RpCollisionGeometryForAllIntersections(
    RpGeometry *geometry,
    RpIntersection *intersection,
    RpIntersectionCallBackGeometryTriangle callBack,
    void *data)
{
    RWAPIFUNCTION(RWSTRING("RpCollisionGeometryForAllIntersections"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(geometry);
    RWASSERT(intersection);
    RWASSERT(callBack);

    switch (intersection->type)
    {
    case rpINTERSECTLINE:
        {
            RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
            GeometryForAllTriIntsecLine(
                geometry, &intersection->t.line, intersection, callBack, data);
            RWRETURN(geometry);
        }

    case rpINTERSECTSPHERE:
        {
            GeometryForAllTriIntsecSphere(geometry,
                &intersection->t.sphere, intersection, callBack, data);
            RWRETURN(geometry);
        }

    case rpINTERSECTBOX:
        {
            GeometryForAllTriIntsecBBox(
                geometry, intersection, callBack, data);
            RWRETURN(geometry);
        }

    case rpINTERSECTATOMIC:
        {
            RpAtomic *atomic = (RpAtomic *) intersection->t.object;
            RwSphere sphere;

            RWASSERT(atomic);
            RWASSERTISTYPE(atomic, rpATOMIC);
            sphere = *RpAtomicGetWorldBoundingSphere(atomic);

            GeometryForAllTriIntsecSphere(
                geometry, &sphere, intersection, callBack, data);
            RWRETURN(geometry);
        }

    default:
        {
            RWASSERT(0);
            RWMESSAGE(("Unsupported or invalid intersection type."));
            RWRETURN(geometry);
        }
    }
}


/**
 * \ingroup rpcollis
 * \ref RpAtomicForAllIntersections returns, via a callback function,
 * all triangles in an atomic that intersect a given primitive.
 * The callback may return NULL to terminate intersection testing.
 *
 * \param atomic        Pointer to the atomic.
 * \param intersection  Pointer to an \ref RpIntersection value describing
 *                      the intersection primitive. This must be specified
 *                      in world-space coordinates. Supported types are:
 *                      \li rpINTERSECTLINE      Line intersections
 *                          (the length should not be zero).
 *                      \li rpINTERSECTSPHERE    Sphere intersections.
 *                      \li rpINTERSECTATOMIC    Atomic intersections (based
 *                          on the atomic's bounding sphere).
 * \param callBack      Pointer to the
 *                      \ref RpIntersectionCallBackGeometryTriangle
 *                      callback function.
 * \param data          Pointer to the user supplied data to pass to the
 *                      callback function.
 *
 * \return  Returns a pointer to the atomic (even if no intersections were
 *          found).
 *
 * If the atomic's geometry has a single morph target with collision
 * extension data, then this will be used for faster results (see
 * \ref RpCollisionGeometryBuildData). Otherwise, every triangle of the
 * geometry is tested individually.
 *
 * If the geometry has multiple morph targets, then the triangles
 * are interpolated according to the current \ref RpInterpolator value
 * before being tested for intersection.
 *
 * Note that a bounding sphere test is not done before testing the
 * triangles of the atomic, as this usually takes place as part of a first
 * pass collision stage, e.g. as in \ref RpWorldForAllAtomicIntersections.
 *
 * \see RpCollisionGeometryForAllIntersections
 * \see RpCollisionGeometryBuildData
 * \see RpWorldForAllAtomicIntersections
 * \see RpCollisionWorldForAllIntersections
 * \see \ref rpcollisoverview
 */
RpAtomic *
RpAtomicForAllIntersections(RpAtomic * atomic,
                            RpIntersection * intersection,
                            RpIntersectionCallBackGeometryTriangle callBack,
                            void *data)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicForAllIntersections"));
    RWASSERT(_rpCollisionNumInstances);
    RWASSERT(atomic);
    RWASSERT(intersection);
    RWASSERT(callBack);

    /* This doesn't need the frame hierarchies to be synchronized.
     * The world space bounding sphere of the atomic is not used,
     * and the individual frames are synchronized as necessary with
     * RwFrameGetLtm().
     */

    switch (intersection->type)
    {
    case rpINTERSECTLINE:
        {
            RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
            AtomicForAllTriIntsecLine(
                atomic, intersection, callBack, data);
            RWRETURN(atomic);
        }

    case rpINTERSECTSPHERE:
        {
            AtomicForAllTriIntsecSphere(
                atomic, &intersection->t.sphere, intersection, callBack, data);
            RWRETURN(atomic);
        }

    case rpINTERSECTATOMIC:
        {
            RpAtomic *intsecAtomic = (RpAtomic *)intersection->t.object;
            RwSphere sphere;

            RWASSERT(intsecAtomic);
            RWASSERTISTYPE(intsecAtomic, rpATOMIC);
            sphere = *RpAtomicGetWorldBoundingSphere(intsecAtomic);

            AtomicForAllTriIntsecSphere(
                atomic, &sphere, intersection, callBack, data);
            RWRETURN(atomic);
        }

    default:
        {
            RWASSERT(0);
            RWMESSAGE(("Unsupported or invalid intersection type."));
            RWRETURN(atomic);
        }
    }
}
