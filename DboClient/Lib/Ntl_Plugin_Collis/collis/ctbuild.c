/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctbuild.h
 *  Purpose: Collision split tree generation
 */

/******************************************************************************
 *  Include files
 */

#include "rwcore.h"
#include "rpdbgerr.h"
#include "rpworld.h"

#include "rpcollis.rpe"
#include "ctdata.h"
#include "ctbuild.h"

/******************************************************************************
 *  Defines
 */

#define rpCOLLTREE_MIN_POLYGONS_FOR_SPLIT   4

#define CLIPVERTEXLEFT      (1)
#define CLIPVERTEXRIGHT     (2)
#define CLIPPOLYGONLEFT      (1)
#define CLIPPOLYGONRIGHT     (2)

/******************************************************************************
 *  Types
 */

/* Following types used just for build process */

typedef struct BuildSector BuildSector;
struct BuildSector
{
    RwInt32             type;   /* = -1 for poly sector, >=0 for plane sector */
};

typedef struct BuildPolySector BuildPolySector;
struct BuildPolySector
{
    RwInt32             type;   /* = -1 for poly sector */
    RwUInt16            numPolygons; /* num polys in sector */
    RwUInt16            iFirstPolygon; /* index of first poly in polyMap[] */
};

typedef struct BuildPlaneSector BuildPlaneSector;
struct BuildPlaneSector
{
    RwInt32             type;   /* plane type */
    BuildSector      *leftSubTree;
    BuildSector      *rightSubTree;
    RwReal              leftValue;
    RwReal              rightValue;
};

typedef struct BuildVertex BuildVertex;
struct BuildVertex
{
    RwV3d              *pos;
    RwUInt32            flags;
    RwReal              dist;
};

typedef struct BuildPoly BuildPoly;
struct BuildPoly
{
    RpTriangle         *poly;
    RwUInt32            flags;
};

typedef struct BuildData BuildData;
struct BuildData
{
    /* Data for whole tree */
    RwUInt32            numVertices;
    BuildVertex        *vertices;
    BuildPoly          *polygons; /* Sorted on completion */
    RpTriangle         *origPolys;

    /* Data for current sector in build process */
    RwUInt32            bspDepth;
    RwBBox              bbox;
    RwUInt32            numPolygons;
    RwUInt32            polygonOffset; /* Offset into polygons[] */

    /* Pointer array for sorting the subset of vertices for polygons in
     * current sector */
    RwUInt32            numSortVerts;
    BuildVertex       **sortVerts;
};

typedef struct ClipStats ClipStats;
struct ClipStats
{
    RwInt32             nLeft;
    RwInt32             nRight;
    RwReal              leftValue;
    RwReal              rightValue;
};

/******************************************************************************
 *  Macros
 */

#define SetClipCodes(numVert, sortVerts, plane, value)                   \
MACRO_START                                                              \
{                                                                        \
    RwUInt32     iVert;                                                  \
                                                                         \
    for (iVert=0; iVert<numVert; iVert++)                                \
    {                                                                    \
        RwReal test = GETCOORD(*(sortVerts[iVert]->pos), plane) - value; \
                                                                         \
        sortVerts[iVert]->flags = 0;                                     \
                                                                         \
        /*                                                               \
         *  NOTE: if the vertex is within the plane it                   \
         *  will be classifed as being on both sides of the plane,       \
         *  this is intentional.                                         \
         */                                                              \
        if (test <= ((RwReal)0))                                         \
        {                                                                \
            sortVerts[iVert]->flags |= CLIPVERTEXLEFT;                   \
        }                                                                \
                                                                         \
        if (test >= ((RwReal)0))                                         \
        {                                                                \
            sortVerts[iVert]->flags |= CLIPVERTEXRIGHT;                  \
        }                                                                \
    }                                                                    \
}                                                                        \
MACRO_STOP

/******************************************************************************
 *
 *  Get clip statistics.
 *
 *  Return stats:

    stats->leftValue        extent of left sector
    stats->rightValue       extent of right sector
    stats->nLeft            number of polys on left
    stats->nRight           number of polys on right
 */

static void
GetClipStats(BuildData * data,
                  RwInt32 plane, RwReal value, ClipStats * stats)
{
    RwReal              overlapLeft;
    RwReal              overlapRight;
    RwInt32             numPolygons;
    BuildPoly       *polygons;
    const BuildVertex  *vertices;
    RwInt32             nLeft = 0;
    RwInt32             nRight = 0;

    RWFUNCTION(RWSTRING("GetClipStats"));
    RWASSERT(data);
    RWASSERT(stats);

    polygons = &data->polygons[data->polygonOffset];
    vertices = data->vertices;

    nLeft = nRight = 0;
    overlapLeft = overlapRight = (RwReal) 0;

    /* For each polygon */
    for (numPolygons = data->numPolygons; --numPolygons >=0; polygons++)
    {
        const RwUInt16     *const vertIndex = polygons->poly->vertIndex;
        RwUInt32            clip, iTriVert;
        RwReal              distLeft, distRight;

        clip = CLIPVERTEXLEFT | CLIPVERTEXRIGHT;
        distLeft = distRight = ((RwReal) 0);

        /* For each vertex */
        for (iTriVert = 0; iTriVert < 3; iTriVert++)
        {
            const RwUInt32      iVert = vertIndex[iTriVert];
            RwV3d * const       pos = vertices[iVert].pos;
            const RwReal        dist = GETCOORD(*pos, plane) - value;
            const RwReal        mdist = -dist;

            /* Mask clip flags for polygon */
            clip &= vertices[iVert].flags;

            /* Extent of poly either side of the plane */
            if (mdist > distLeft)
            {
                distLeft = mdist;
            }
            else if (dist > distRight)
            {
                distRight = dist;
            }
        }

        /* figure stats for this polygon */
        switch (clip & (CLIPVERTEXLEFT | CLIPVERTEXRIGHT))
        {
            case CLIPVERTEXLEFT:

                /* Poly is entirely on the left */
                polygons->flags = CLIPPOLYGONLEFT;
                nLeft++;
                break;

            case CLIPVERTEXLEFT | CLIPVERTEXRIGHT:
            case CLIPVERTEXRIGHT:

                /* Poly is entirely on the right */
                polygons->flags = CLIPPOLYGONRIGHT;
                nRight++;
                break;

            case 0:

                /* Decide which overlapping sector poly belongs to */
                if (distRight > distLeft)
                {
                    nRight++;
                    polygons->flags = CLIPPOLYGONRIGHT;
                    if (distLeft > overlapRight)
                    {
                        overlapRight = distLeft;
                    }
                }
                else
                {
                    nLeft++;
                    polygons->flags = CLIPPOLYGONLEFT;
                    if (distRight > overlapLeft)
                    {
                        overlapLeft = distRight;
                    }
                }
                break;
        }
    }

    /* All done */
    stats->nLeft = nLeft;
    stats->nRight = nRight;
    stats->leftValue = value + overlapLeft;
    stats->rightValue = value - overlapRight;

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  Calculate average depth of leaf nodes in a binary tree, assuming that
 *  all nodes have equal lookup probability, and the tree is as symmetric as
 *  possible.
 *
 *  In the case of choosing BSP sectors, this assumption gives maximum
 *  significance to the choice we are making at the current level of
 *  building the tree.
 */
static              RwReal
AverageTreeDepth(RwUInt32 numLeaves)
{
    RwReal              aveDepth;
    RwInt32             depth;
    RwUInt32            temp;

    RWFUNCTION(RWSTRING("AverageTreeDepth"));
    RWASSERT(numLeaves);

    /* Depth is MSB */
    depth = -1;
    temp = numLeaves;
    while (temp)
    {
        depth++;
        temp >>= 1;
    }

    /* Average over all leaf nodes */
    aveDepth =
        (RwReal) (depth + 2) - (RwReal) (1 << (depth + 1)) / numLeaves;

    RWRETURN(aveDepth);
}

/******************************************************************************
 *
 *  Score the division of the sector for current plane and value,
 *  based on the clip statistics.
 */
static              RwReal
DivisionScore(BuildData * data, ClipStats * stats, RwInt32 plane)
{
    RwReal              leftExtent, rightExtent, sectorExtent;
    RwReal              score;

    RWFUNCTION(RWSTRING("DivisionScore"));

    /*  These factors are proportional to the volumes of the left and
     *  right sectors, so that we effectively weight the scores for
     *  subsectors by volume, which makes sense for point-like collision
     *  tests. This also weights fairly against overlap.
     */
    sectorExtent = GETCOORD(data->bbox.sup, plane)
        - GETCOORD(data->bbox.inf, plane);
    leftExtent = stats->leftValue - GETCOORD(data->bbox.inf, plane);
    rightExtent = GETCOORD(data->bbox.sup, plane) - stats->rightValue;

    score = ((leftExtent * AverageTreeDepth(stats->nLeft) +
              rightExtent * AverageTreeDepth(stats->nRight)) /
             sectorExtent);

    RWRETURN(score);
}

/******************************************************************************
 *
 * Create list of pointers sortVerts[numSortVerts] to all vertices
 * referenced by polygons in data. This will, in general, be a subset
 * of vertices[numVertices].
 */
static void
SetSortVertices(BuildData * data)
{
    RwUInt32            iPoly;
    RwUInt32            iVert;
    RwUInt32            iSort;
    BuildPoly       *polygons;

    RWFUNCTION(RWSTRING("SetSortVertices"));

    polygons = data->polygons + data->polygonOffset;

    /* Reset flags for all vertices */
    for (iVert = 0; iVert < data->numVertices; iVert++)
    {
        data->vertices[iVert].flags = 0;
    }

    /* For each polygon */
    for (iPoly = 0; iPoly < data->numPolygons; iPoly++)
    {
        RwInt32             iPolyVert;

        /* For each polygon vertex */
        for (iPolyVert = 0; iPolyVert < 3; iPolyVert++)
        {
            /* Flag as required */
            data->vertices[polygons[iPoly].poly->vertIndex[iPolyVert]].
                flags++;
        }
    }

    /* Create a sort vertex pointer for each vertex required */
    iSort = 0;
    for (iVert = 0; iVert < data->numVertices; iVert++)
    {
        if (data->vertices[iVert].flags)
        {
            data->sortVerts[iSort] = &data->vertices[iVert];
            iSort++;
        }
    }

    data->numSortVerts = iSort;

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  Weight against splitting of sectors in smallest dimension. It is more
 *  efficient to have cube shaped sectors than slabs when testing lines.
 */
static              RwReal
FuzzyExtentScore(RwReal extent, RwReal maxExtent)
{
    RwReal              value;

    RWFUNCTION(RWSTRING("FuzzyExtentScore"));

    value = extent / maxExtent;
    if (value > ((RwReal) 0.5))
    {
        value = ((RwReal) 1);
    }
    else
    {
        /* bias against cutting small extent */
        value = ((RwReal) 0.5) + value;
    }

    value = (RwReal) 1 / value;
    RWRETURN(value);
}

/*****************************************************************************
 *
 *  Find the best plane that splits the current sets of polygons into two.
 *  This is based on the world sector cut plane algorithm.
 */

#define MAXCLOSESTCHECK 50
#define RANGEEPS 0.0001f

static int
BuildVertexCmp(const void *a, const void *b)
{
    volatile RwSplitBits diff;
    int                 result;

    RWFUNCTION(RWSTRING("BuildVertexCmp"));
    RWASSERT(a);
    RWASSERT(b);

    diff.nReal = ((*((const BuildVertex * const *) a))->dist -
                  (*((const BuildVertex * const *) b))->dist);
    result = diff.nInt;

    RWRETURN(result);
}

static              RwBool
FindDividingPlane(BuildData * data, RwInt32 * plane, RwReal * value)
{
    RwReal              bestScore = RwRealMAXVAL; /* Haven't found any */
    RwInt32             bestPlane = (RwInt32) 0;
    RwReal              bestValue = (RwReal) 0;
    ClipStats      bestStats;
    RwInt32             tryPlane, maxExtentPlane;
    RwV3d               vSize;

    RWFUNCTION(RWSTRING("FindDividingPlane"));

    /* Find the maximum extent for this sector */
    RwV3dSubMacro(&vSize, &data->bbox.sup, &data->bbox.inf);

    maxExtentPlane = 0;
    for (tryPlane = 0; tryPlane < 12; tryPlane += sizeof(RwReal))
    {
        if (GETCOORD(vSize, tryPlane) > GETCOORD(vSize, maxExtentPlane))
        {
            maxExtentPlane = tryPlane;
        }
    }

    /* Go through the planes -> try and find the best one */
    for (tryPlane = 0;
         tryPlane < (RwInt32) (3 * sizeof(RwReal));
         tryPlane += sizeof(RwReal))
    {
        RwReal              quantThresh;
        RwReal              median;
        RwReal              interQuartileRange;
        RwInt32             lastquantValue;
        RwUInt32            lowerQuartile;
        RwUInt32            upperQuartile;
        RwInt32             numUniq;
        RwUInt32            nI;
        RwReal              extentScore;

        extentScore = FuzzyExtentScore(GETCOORD(vSize, tryPlane),
                                       GETCOORD(vSize, maxExtentPlane));

        /* find median of population */
        for (nI = 0; nI < data->numSortVerts; nI++)
        {
            data->sortVerts[nI]->dist =
                GETCOORD(*(data->sortVerts[nI]->pos), tryPlane);
        }
        qsort((void *) data->sortVerts, data->numSortVerts,
              sizeof(BuildVertex *), BuildVertexCmp);

        /* value that bisects population in tryPlane axis */
        median = data->sortVerts[nI >> 1]->dist;
        lowerQuartile = nI >> 2; /* 25% */
        upperQuartile = (nI >> 1) + (nI >> 2); /* 75% */

        /* granularity by which we move plane around +/-25% of median */
        interQuartileRange = data->sortVerts[upperQuartile]->dist -
            data->sortVerts[lowerQuartile]->dist;

        /* Is inter-quartile range very small? */
        if (interQuartileRange <
            (RANGEEPS * GETCOORD(vSize, maxExtentPlane)))
        {
            /* Just test median value for split */
            lowerQuartile = upperQuartile = (nI >> 1);
            quantThresh = ((RwReal) 0);
        }
        else
        {
            quantThresh = MAXCLOSESTCHECK / interQuartileRange;

            /* sort just this subarray on abs dist from median */
            for (nI = lowerQuartile; nI <= upperQuartile; nI++)
            {
                RwReal              dist =
                    median - data->sortVerts[nI]->dist;

                if (dist < ((RwReal) 0))
                {
                    data->sortVerts[nI]->dist = -dist;
                }
                else
                {
                    data->sortVerts[nI]->dist = dist;
                }
            }
            qsort((void *) &data->sortVerts[lowerQuartile],
                  upperQuartile - lowerQuartile + 1,
                  sizeof(BuildVertex *), BuildVertexCmp);
        }

        /* score test planes  */
        lastquantValue = -1;
        numUniq = MAXCLOSESTCHECK;
        for (nI = lowerQuartile;
             nI <= upperQuartile && numUniq > 0; nI++)
        {
            RwReal              tryValue;
            RwInt32             quantValue;

            /* fetch the next unique distance */
            tryValue = GETCOORD(*(data->sortVerts[nI]->pos), tryPlane);
            quantValue = RwInt32FromRealMacro(tryValue * quantThresh);
            if (quantValue != lastquantValue)
            {
                ClipStats      stats;

                /* Set the clip codes */
                SetClipCodes(data->numSortVerts, data->sortVerts,
                             tryPlane, tryValue);

                /* Check what would be resultant polygons with this clip plane
                 * Including a look at overlaps */
                GetClipStats(data, tryPlane, tryValue, &stats);

                /* does it actually divide the geometry? */
                if (stats.nLeft && stats.nRight)
                {
                    RwReal              score;

                    score = DivisionScore(data, &stats, tryPlane);
                    score *= extentScore;

                    /* is it a winner? */
                    if (score < bestScore)
                    {
                        bestScore = score;
                        bestPlane = tryPlane;
                        bestValue = tryValue;
                        bestStats = stats;
                    }
                }

                /* onto next test plane */
                lastquantValue = quantValue;
                numUniq--;
            }
        }
    }

    /* Check if a suitable plane was found at all */
    if (bestScore < RwRealMAXVAL)
    {
        (*plane) = bestPlane;
        (*value) = bestValue;
        RWRETURN(TRUE);
    }

    RWRETURN(FALSE);
}

/******************************************************************************
 *
 *  Sort the polygons according to whether they are in left or right
 *  sector using flags set by GetClipStats(). On exit, the
 *  flags are no longer valid.
 */
static void
SortPolygons(BuildData * data)
{
    BuildPoly       *polygons = data->polygons + data->polygonOffset;
    RwUInt32            leftCount;
    RwUInt32            iPoly;

    RWFUNCTION(RWSTRING("SortPolygons"));

    leftCount = 0;

    /* For each polygon */
    for (iPoly = 0; iPoly < data->numPolygons; iPoly++)
    {
        /* Is polygon on the left? */
        if (polygons[iPoly].flags == CLIPPOLYGONLEFT)
        {
            /* Move it to first section of array. */
            if (iPoly > leftCount)
            {
                RpTriangle  *const swap = polygons[leftCount].poly;

                polygons[leftCount].poly = polygons[iPoly].poly;
                polygons[iPoly].poly = swap;
            }

            leftCount++;
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  BSP leaf node: Polygon sector.
 */
static BuildPolySector *
BuildPolySectorCreate(RwUInt32 numPolygons, RwUInt32 firstPolygon)
{
    BuildPolySector *sector;

    RWFUNCTION(RWSTRING("BuildPolySectorCreate"));

    sector = (BuildPolySector *)RwMalloc(sizeof(BuildPolySector), 0);
    if (!sector)
    {
        RWRETURN(NULL);
    }

    sector->type = -1;
    sector->numPolygons = (RwUInt16) numPolygons;
    sector->iFirstPolygon = (RwUInt16) firstPolygon;

    RWRETURN(sector);
}

/******************************************************************************
 *
 *  BSP tree node: Plane sector.
 */
static BuildPlaneSector *
BuildPlaneSectorCreate(RwInt32 type, RwReal leftValue, RwReal rightValue)
{
    BuildPlaneSector *sector;

    RWFUNCTION(RWSTRING("BuildPlaneSectorCreate"));

    sector = (BuildPlaneSector *)RwMalloc(sizeof(BuildPlaneSector), 0);
    if (!sector)
    {
        RWRETURN(NULL);
    }

    sector->type = type;
    sector->leftValue = leftValue;
    sector->rightValue = rightValue;

    RWRETURN(sector);
}

/******************************************************************************
 *
 *  Destroy the build tree. May be part built.
 */
static void
BuildTreeDestroy(BuildSector * sector)
{
    RWFUNCTION(RWSTRING("BuildTreeDestroy"));

    if (sector)
    {
        if (sector->type < 0)
        {
            /* Leaf node */
            RwFree(sector);
        }
        else
        {
            /* Branch node */
            BuildPlaneSector *planeSector = (BuildPlaneSector *) sector;

            BuildTreeDestroy(planeSector->leftSubTree);
            BuildTreeDestroy(planeSector->rightSubTree);

            RwFree(planeSector);
        }
    }

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  Split the sector recursively, returning a BSP tree.
 */
static BuildSector *
BuildTreeGenerate(BuildData * data)
{
    RwUInt32            plane;
    RwReal              value;
    ClipStats           stats;

    RWFUNCTION(RWSTRING("BuildTreeGenerate"));
    RWASSERT(data);

    if (data->numPolygons < rpCOLLTREE_MIN_POLYGONS_FOR_SPLIT)
    {
        /* Can stop splitting now */
        RWRETURN((BuildSector *)
            BuildPolySectorCreate(data->numPolygons, data->polygonOffset));
    }
    else if (data->bspDepth < rpCOLLTREE_MAXDEPTH)
    {
        /* Try finding a dividing plane, but first set up pointers to polygon
         * vertices ready for processing
         */
        SetSortVertices(data);

        if (FindDividingPlane(data, (RwInt32 *)&plane, &value))
        {
            /* Set the clip codes on the vertices */
            SetClipCodes(data->numSortVerts, data->sortVerts, plane, value);

            /* Check what would be resultant polygons with this clip plane.
             * This flags data->polygons ready for sorting */
            GetClipStats(data, plane, value, &stats);

            /* Sort polygons into bunch on left and bunch on right */
            SortPolygons(data);
        }
        else
        {
            /* If we failed to find a plane it probably means a bunch of
             * triangles are lying on top of each other. We can just put them
             * into a leaf node.
             */
            if (data->numPolygons <= rpCOLLSECTOR_CONTENTS_MAXCOUNT)
            {
                RWRETURN((BuildSector *)
                    BuildPolySectorCreate(data->numPolygons, data->polygonOffset));
            }
            else
            {
                /* There are too many for a leaf node! This should never happen,
                 * but we handle it just in case. We pretend to do a split, but
                 * it's actually 100% overlapping.
                 */
                plane = 0;
                stats.leftValue = data->bbox.sup.x;
                stats.rightValue = data->bbox.inf.x;
                stats.nLeft = data->numPolygons >> 1;
                stats.nRight = data->numPolygons - stats.nLeft;
            }
        }
    }
    else
    {
        /* We've reached maximum tree depth. If this happens then something
         * might be going wrong.
         */
        if (data->numPolygons <= rpCOLLSECTOR_CONTENTS_MAXCOUNT)
        {
            /* Put all remaining polys in this branch in a leaf node */
            RWRETURN((BuildSector *)
                BuildPolySectorCreate(data->numPolygons, data->polygonOffset));
        }
        else
        {
            RWMESSAGE((RWSTRING("Max tree depth reached with too many "
                "triangles remaining for a leaf node.")));

            RWERROR((E_RP_COLLIS_BUILD_FAILURE));
            RWRETURN(NULL);
        }
    }

    /* Make a plane sector */
    {
        BuildData           subdata;
        BuildPlaneSector *planeSector;

        /* Split space */
        data->bspDepth++;

        /* Create the plane sector */
        planeSector =
            BuildPlaneSectorCreate(plane, stats.leftValue, stats.rightValue);
        if (!planeSector)
        {
            RWRETURN(NULL);
        }

        /* Left polygons */
        subdata = *data;
        subdata.numPolygons = stats.nLeft;
        SETCOORD(subdata.bbox.sup, plane, stats.leftValue);
        planeSector->leftSubTree = BuildTreeGenerate(&subdata);
        if (!planeSector->leftSubTree)
        {
            RwFree(planeSector);
            RWRETURN(NULL);
        }

        /* Right polygons */
        subdata = *data;
        subdata.numPolygons = stats.nRight;
        subdata.polygonOffset = data->polygonOffset + stats.nLeft;
        SETCOORD(subdata.bbox.inf, plane, stats.rightValue);
        planeSector->rightSubTree = BuildTreeGenerate(&subdata);
        if (!planeSector->rightSubTree)
        {
            BuildTreeDestroy((BuildSector *) planeSector);
            RWRETURN(NULL);
        }

        RWRETURN((BuildSector *) planeSector);
    }
}

/******************************************************************************
 *
 *  Destroy the build data structure.
 */
static void
BuildDataDestroy(BuildData * data)
{
    RWFUNCTION(RWSTRING("BuildDataDestroy"));

    if (data)
    {
        if (data->vertices)
        {
            RwFree(data->vertices);
        }
        if (data->polygons)
        {
            RwFree(data->polygons);
        }
        if (data->sortVerts)
        {
            RwFree(data->sortVerts);
        }
        RwFree(data);
    }

    RWRETURNVOID();
}

/******************************************************************************
 *
 *  Create and initialize the temporary data structure used during
 *  the BSP build process.
 */
static BuildData   *
BuildDataCreate(RwInt32 numVertices,
                RwV3d * vertices,
                RwInt32 numPolygons,
                RpTriangle * polygons)
{
    RwInt32             iVert, iPoly;
    BuildData          *data;

    RWFUNCTION(RWSTRING("BuildDataCreate"));

    data = (BuildData *) RwMalloc(sizeof(BuildData), 0);
    if (!data)
    {
        BuildDataDestroy(data);
        RWRETURN(NULL);
    }

    data->vertices =
        (BuildVertex *) RwMalloc(numVertices * sizeof(BuildVertex), 0);
    if (!data->vertices)
    {
        BuildDataDestroy(data);
        RWRETURN(NULL);
    }

    data->sortVerts =
        (BuildVertex **) RwMalloc(numVertices * sizeof(BuildVertex *), 0);
    if (!data->sortVerts)
    {
        BuildDataDestroy(data);
        RWRETURN(NULL);
    }

    data->polygons =
        (BuildPoly *) RwMalloc(numPolygons * sizeof(BuildPoly), 0);
    if (!data->polygons)
    {
        BuildDataDestroy(data);
        RWRETURN(NULL);
    }

    /* Initialize data */
    data->numPolygons = numPolygons;
    data->polygonOffset = 0;
    data->numVertices = numVertices;
    data->bspDepth = 0;
    data->origPolys = polygons;

    /* Set up bounding box and
     * Initialize pointer tables for vertices and polygons
     */
    RwBBoxInitialize(&data->bbox, &vertices[0]);
    for (iVert = 0; iVert < numVertices; iVert++)
    {
        RwBBoxAddPoint(&data->bbox, &vertices[iVert]);
        data->vertices[iVert].pos = &vertices[iVert];
    }

    for (iPoly = 0; iPoly < numPolygons; iPoly++)
    {
        data->polygons[iPoly].poly = &polygons[iPoly];
    }

    RWRETURN(data);
}

/******************************************************************************
 *  Count the number of leaf nodes in the BSP tree
 */
static              RwInt32
BuildTreeCountLeafNodes(BuildSector * tree)
{
    BuildSector      *sector = tree;
    BuildSector      *sectorStack[rpCOLLTREE_MAXDEPTH + 1];
    RwInt32             nStack = 0;
    RwInt32             nLeaves = 0;

    RWFUNCTION(RWSTRING("BuildTreeCountLeafNodes"));

    sectorStack[0] = NULL;

    while (sector)
    {
        if (sector->type < 0)  /* Leaf node */
        {
            nLeaves++;
            sector = sectorStack[nStack];
            nStack--;
        }
        else                   /* Branch node */
        {
            BuildPlaneSector *planeSector =
                (BuildPlaneSector *) sector;

            /* Put right on stack, and go left */
            nStack++;
            sectorStack[nStack] = planeSector->rightSubTree;
            sector = planeSector->leftSubTree;
        }
    }

    RWRETURN(nLeaves);
}

/******************************************************************************
 */

static RpCollTree *
CreateCollTreeFromBuildTree(BuildData      *buildData,
                            BuildSector    *rootSector,
                            RwUInt16       *map)
{
    RwInt32             nStack;

    struct
    {
        RwUInt16       *ref;
        BuildSector    *sector;

    }   stack[rpCOLLTREE_MAXDEPTH + 1];

    RwUInt32            iSplit, iTri;
    RwUInt32            totalNumPolys;
    RwUInt16            dummy;
    RpCollTree         *collTree;

    RWFUNCTION(RWSTRING("CreateCollTreeFromBuildTree"));
    RWASSERT(rootSector);
    RWASSERT(buildData);

    /* Create collision tree */
    {
        RwUInt32  numSplits, flags;

        numSplits = BuildTreeCountLeafNodes(rootSector) - 1;
        flags = map ? 0 : rpCOLLTREE_USEMAP;

        RWASSERT(buildData->numPolygons <= 0xffff);
        RWASSERT(numSplits <= 0xffff);
        collTree = _rpCollTreeCreate(
            (RwUInt16)buildData->numPolygons,
            (RwUInt16)numSplits, &buildData->bbox, flags);

        if (!collTree)
        {
            RWRETURN((RpCollTree *)NULL);
        }
    }

    /* Build triangle sort map */
    if (!map)
        map = collTree->map;

    for (iTri = 0; iTri < buildData->numPolygons; iTri++)
    {
        map[iTri] =
            (RwUInt16)(buildData->polygons[iTri].poly - buildData->origPolys);
    }

    /* Handle case of simple leaf node with no splits */
    if (rootSector->type < 0)
    {
        RWRETURN(collTree);
    }

    /* Need to recurse tree and pickup array of splits */
    stack[0].ref = &dummy;
    stack[0].sector = rootSector;
    nStack = 0;
    iSplit = 0;
    totalNumPolys = 0;
    while (nStack >= 0)
    {
        BuildPlaneSector   *planeSector;
        BuildPolySector    *polySector;
        RpCollSplit        *split;

        split = collTree->splits + iSplit;
        planeSector = (BuildPlaneSector *) stack[nStack].sector;

        /* Shouldn't be a leaf node */
        RWASSERT(planeSector->type >= 0);

        /* Fill in the index of this split in its parent */
        *(stack[nStack].ref) = (RwUInt16) iSplit;

        split->rightSector.type = (RwUInt8) planeSector->type;
        split->leftSector.type =
            (RwUInt8) (planeSector->type | rpCOLLSECTOR_TYPE_NEG);
        split->leftSector.value = planeSector->leftValue;
        split->rightSector.value = planeSector->rightValue;

        /* When putting subtrees on the stack, we must end up processing in
         * order of left before right
         */

        if (planeSector->rightSubTree->type < 0)
        {
            polySector = (BuildPolySector *) planeSector->rightSubTree;
            RWASSERT(polySector->numPolygons <= rpCOLLSECTOR_CONTENTS_MAXCOUNT);
            split->rightSector.contents = (RwUInt8) polySector->numPolygons;
            split->rightSector.index = polySector->iFirstPolygon;
            totalNumPolys += split->rightSector.contents;
        }
        else
        {
            split->rightSector.contents = rpCOLLSECTOR_CONTENTS_SPLIT;
            stack[nStack].sector = planeSector->rightSubTree;
            stack[nStack].ref = &split->rightSector.index;
            nStack++;
        }

        if (planeSector->leftSubTree->type < 0)
        {
            polySector = (BuildPolySector *) planeSector->leftSubTree;
            RWASSERT(polySector->numPolygons <= rpCOLLSECTOR_CONTENTS_MAXCOUNT);
            split->leftSector.contents = (RwUInt8) polySector->numPolygons;
            split->leftSector.index = polySector->iFirstPolygon;
            totalNumPolys += split->leftSector.contents;
        }
        else
        {
            split->leftSector.contents = rpCOLLSECTOR_CONTENTS_SPLIT;
            stack[nStack].sector  = planeSector->leftSubTree;
            stack[nStack].ref = &split->leftSector.index;
            nStack++;
        }

        iSplit++;
        nStack--;
    }

    RWASSERT(iSplit == collTree->numSplits);
    RWASSERT(totalNumPolys == buildData->numPolygons);

    RWRETURN(collTree);
}


/*
 *  _rpCollTreeBuild
 *
 *  Generate collision tree from list of vertices and triangles
 *
 *  If map is non-NULL, then the remapping is stored there, otherwise the
 *  collision tree is created with a built in map (less efficient).
 *
 *  returns collision tree on success
 *
 */
RpCollTree *
_rpCollTreeBuild(RwInt32        numVerts,
                  RwV3d        *verts,
                  RwInt32       numTris,
                  RpTriangle   *tris,
                  RwUInt16     *map)
{
    RpCollTree     *collTree = NULL;
    BuildData      *BuildData;
    BuildSector  *rootSector;

    RWFUNCTION(RWSTRING("_rpCollTreeBuild"));
    RWASSERT(numVerts);
    RWASSERT(verts);
    RWASSERT(numTris);
    RWASSERT(tris);

    /* Create build data structure */
    BuildData = BuildDataCreate(numVerts, verts, numTris, tris);
    if (BuildData)
    {
        /* Make the build tree (split geometry recursively) */
        rootSector = BuildTreeGenerate(BuildData);
        if (rootSector)
        {
            /* Make the final working collision tree */
            collTree = CreateCollTreeFromBuildTree(BuildData, rootSector, map);
            BuildTreeDestroy(rootSector);
        }

        BuildDataDestroy(BuildData);
    }

    RWRETURN(collTree);
}
