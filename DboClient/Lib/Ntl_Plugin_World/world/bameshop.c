/*
 * Purpose: Provide construction and enumeration facilities for meshes.
 *
 * For more on triangle strip-ification algorithms, see
 * o Stripe
 *   http://www.cs.sunysb.edu/~stripe/
 * o Fast and Effective Stripification of Polygonal Surface Models
 *   http://www.cosy.sbg.ac.at/~held/projects/strips/strips.html
 *   http://www.gvu.gatech.edu/gvu/i3dg/papers.html
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <rwcore.h>

#include "bamesh.h"
#include "baworld.h"
#include "bameshop.h"



/****************************************************************************
 Local Types
 */

typedef struct TriStripListEntry TriStripListEntry;
struct TriStripListEntry
{
    RwUInt16           *strip;
    RwUInt32            stripLen;
    RwUInt32            stripSize;
    TriStripListEntry  *next;
};

typedef struct TriStripList TriStripList;
struct TriStripList
{
    TriStripListEntry  *head;
};

typedef struct TriBinEntry TriBinEntry;

typedef struct Edge Edge;
struct Edge
{
    RwUInt16       v1, v2;      /* Two vertices involved in this edge       */
    TriBinEntry   *tri1;        /* Triangles sharing this edge              */
    TriBinEntry   *tri2;
    Edge          *next;        /* Next edge                                */
};

struct TriBinEntry
{
    RwUInt32       tri;
    Edge          *edge[3];
    TriBinEntry   *next;
    TriBinEntry   *prev;

    RwBool         used;
    RwBool         used2;

    RwUInt8        adjCount;
};

typedef struct TriBinList TriBinList;
struct TriBinList
{
    TriBinEntry   *head;
};

typedef struct MeshOpFreeLists MeshOpFreeLists;
struct MeshOpFreeLists
{
    RwFreeList    *binEntryFreeList;
    RwFreeList    *edgeFreeList;
};

typedef struct RpMeshopStatic RpMeshopStatic;
struct RpMeshopStatic
{
    RpMeshHeader            nullMeshHeader;
    RpTriStripMeshCallBack  meshTristripMethod;
    void                   *data;
};

typedef struct _rpTriStripTunnelStack _rpTriStripTunnelStack;
struct _rpTriStripTunnelStack
{
    RpTriStripEdge    *edge;
    RpTriStripPolygon *polygon;
    RwUInt32           length;
};

typedef struct _rpTriStripVertex _rpTriStripVertex;
struct _rpTriStripVertex
{
    RwUInt32 numEdges;
    RwLinkList triangleList;
};

typedef struct _rpTriStripEdge _rpTriStripEdge;
struct _rpTriStripEdge
{
    RpTriStripEdge *edge;
    RwLLLink inVertexLink;
};


/****************************************************************************
 Local (Static) Prototypes
 */

#if (defined(_WINDOWS))
#define  __RWCDECL   __cdecl
#endif /* (defined(_WINDOWS)) */

#if (!defined(__RWCDECL))
#define  __RWCDECL             /* No op */
#endif /* (!defined(__RWCDECL)) */

/****************************************************************************
 Local Defines
 */

#define RPMESHGLOBAL(var)                               \
   (RWPLUGINOFFSET(rpMeshGlobals,                       \
                   RwEngineInstance,                    \
                   meshModule.globalsOffset)->var)

#define RasterHasAlpha(_type)                           \
            ( ((_type) == rwRASTERFORMAT1555) ||        \
              ((_type) == rwRASTERFORMAT4444) ||        \
              ((_type) == rwRASTERFORMAT8888) )

#define JOINSTRIPS
#define TURNCORNERS
#define ODDTURNSx
#define TRISTRIPLOOKAHEADx

/****************************************************************************/

#define TESTVERTEXBIT(vertexIndex)      \
    (vertexTagBuffer[(vertexIndex) >> 5] & (1 << ((vertexIndex) & 31)))

#define SETVERTEXBIT(vertexIndex)       \
    vertexTagBuffer[(vertexIndex) >> 5] |= (1 << ((vertexIndex) & 31))

#define private_memset(_s, _c, _n)              \
MACRO_START                                     \
{                                               \
    char               *cs = (char *) (_s);     \
    char                c = (char) (_c);        \
    RwUInt32            count = _n;             \
                                                \
    while (count--)                             \
    {                                           \
        *cs++ = c;                              \
    }                                           \
}                                               \
MACRO_STOP

#define EdgeAdjCount(edge) (((edge)->tri1 && !((edge)->tri1->used)) +   \
                            ((edge)->tri2 && !((edge)->tri2->used)))

#define EdgeAdjCount2(edge) (((edge)->tri1 && !((edge)->tri1->used2)) +   \
                             ((edge)->tri2 && !((edge)->tri2->used2)))

/****************************************************************************
 Globals (across program)
 */


/****************************************************************************
 Local (static) Globals
 */

static RpMeshopStatic MeshopStatic =
{
    {0, 0, 0, 0, 0},
    RpBuildMeshGenerateDefaultTriStrip,
    FALSE
};

/****************************************************************************
 Local (static) Functions
 */


/****************************************************************************
 SortPolygons

 On entry   :
 On exit    :
 */
static int __RWCDECL
SortPolygons(const void *pA, const void *pB)
{
    const RwUInt32 transBIT = 16;
    const RwUInt32  rastBIT = 8;
    const RwUInt32  pipeBIT = 4;
    const RwUInt32  texBIT = 2;
    const RwUInt32  matBIT = 1;

    const RpBuildMeshTriangle * const * mtpA =
        (const RpBuildMeshTriangle * const *) pA;
    const RpBuildMeshTriangle * const * mtpB =
        (const RpBuildMeshTriangle * const *) pB;

    RpMaterial *materialA = (*mtpA)->material;
    RpMaterial *materialB = (*mtpB)->material;

    RwRaster   *rasterA   = (RwRaster *)NULL;
    RwRaster   *rasterB   = (RwRaster *)NULL;
    RwTexture  *textureA  = (RwTexture *)NULL;
    RwTexture  *textureB  = (RwTexture *)NULL;

    /* IMO use UInts for bitfields! Sign bits are nothing but trouble... */
    RwUInt32     orderA = 0;
    RwUInt32     orderB = 0;

    RWFUNCTION(RWSTRING("SortPolygons"));

    /* Easy case first */
    if (materialA == materialB)
    {
        RWRETURN(0);
    }

    /* We sort on:
     *   transparency > raster > pipeline > texture > material
     *
     * Transparency is required for correct alpha render ordering.
     * Raster upload is the greatest cost.
     * Pipeline swap might be a significant cost - vector code upload, CPU-side code cache miss.
     * Texture state changes might also hurt even with the same raster. (?)
     * Sorting things in memory order (i.e on RpMaterial pointer) is probably generally a good thing, pff...
     */

    if (materialA)
    {
        /* Place transparent materials after non transparent ones */
        if (materialA->texture)
        {
            textureA = materialA->texture;
            rasterA = RwTextureGetRaster(textureA);

            if (RasterHasAlpha
                (RwRasterGetFormat(rasterA) & (RwInt32)
                 rwRASTERFORMATPIXELFORMATMASK))
            {
                orderA |= transBIT;
            }
        }

        if (materialA->color.alpha != 0xff)
        {
            orderA |= transBIT;
        }
    }

    if (materialB)
    {
        /* Place transparent materials after non transparent ones */
        if (materialB->texture)
        {
            textureB = materialB->texture;
            rasterB = RwTextureGetRaster(textureB);

            if (RasterHasAlpha
                (RwRasterGetFormat(rasterB) & (RwInt32)
                 rwRASTERFORMATPIXELFORMATMASK))
            {
                orderB |= transBIT;
            }
        }

        if (materialB->color.alpha != 0xff)
        {
            orderB |= transBIT;
        }
    }

    orderA |= ((*mtpA)->rasterIndex > (*mtpB)->rasterIndex) ? rastBIT : 0;
    orderB |= ((*mtpA)->rasterIndex < (*mtpB)->rasterIndex) ? rastBIT : 0;

    orderA |= ((*mtpA)->pipelineIndex > (*mtpB)->pipelineIndex) ? pipeBIT : 0;
    orderB |= ((*mtpA)->pipelineIndex < (*mtpB)->pipelineIndex) ? pipeBIT : 0;

    orderA |= ((*mtpA)->textureIndex > (*mtpB)->textureIndex) ? texBIT : 0;
    orderB |= ((*mtpA)->textureIndex < (*mtpB)->textureIndex) ? texBIT : 0;

    orderA |= ((*mtpA)->matIndex > (*mtpB)->matIndex) ? matBIT : 0;
    orderB |= ((*mtpA)->matIndex < (*mtpB)->matIndex) ? matBIT : 0;

    RWRETURN(orderA - orderB);
}


/****************************************************************************
 SortPolygonsInTriListMesh

 On entry   :
 On exit    :
 */
static RpMesh *
SortPolygonsInTriListMesh(RpMesh *mesh,
                          RpMeshHeader * meshHeader __RWUNUSED__,
                          void * pData __RWUNUSED__ )
{
    RwUInt32           *vertexTagBuffer;
    RwUInt32            maxVertex, i, numTriangles;
    RxVertexIndex      *indices;

    RWFUNCTION(RWSTRING("SortPolygonsInTriListMesh"));
    RWASSERT(mesh);

    indices = mesh->indices;
    numTriangles = mesh->numIndices / 3;

    /* Find the maximum vertex in a fairly rough and ready way */
    maxVertex = 0;
    for (i = 0; i < mesh->numIndices; i++)
    {
        maxVertex |= indices[i];
    }

    /* Allocate a vertex tag table (8 vertices per byte) round to next long */
    vertexTagBuffer = (RwUInt32 *) RwMalloc((maxVertex + 31) >> 3,
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    if (vertexTagBuffer)
    {
        /* Cool, now we can generate a new array of triangles,
         * copy the old ones to work from */
        RxVertexIndex    *oldIndices;
        RwUInt32            arraySize =

            sizeof(RxVertexIndex) * mesh->numIndices;

        oldIndices = (RxVertexIndex *) RwMalloc(arraySize,
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
        if (oldIndices)
        {
            RwUInt32            outIndex;

            memcpy(oldIndices, indices, arraySize);

            /* Copy runs of indices from the 'oldIndices' buffer back into
             * the 'indices' buffer, marking oldIndices as used by setting
             * all three vertex indices to 65535.
             */

            /* While we haven't copied all the triangles */
            outIndex = 0;
            while (outIndex < numTriangles)
            {
                RwUInt32            inIndex;

                /* Reset the vertex tag buffer, round to next long */
                private_memset(vertexTagBuffer, 0,
                               ((maxVertex + 31) >> 3));

                inIndex = 0;
                while (inIndex < numTriangles)
                {
                    RxVertexIndex    *testInds =

                        &oldIndices[inIndex * 3];

                    if ((testInds[0] & testInds[1] & testInds[2]) !=
                        65535)
                    {
                        /* This triangle hasn't been grabbed yet */
                        /* If none of the vertices are used,
                         * copy the triangles */
                        if ((!TESTVERTEXBIT(testInds[0])) &&
                            (!TESTVERTEXBIT(testInds[1])) &&
                            (!TESTVERTEXBIT(testInds[2])))
                        {
                            /* Setbits */
                            SETVERTEXBIT(testInds[0]);
                            SETVERTEXBIT(testInds[1]);
                            SETVERTEXBIT(testInds[2]);

                            /* Grabbit and markit */
                            *indices++ = testInds[0];
                            *indices++ = testInds[1];
                            *indices++ = testInds[2];
                            testInds[0] = testInds[1] = testInds[2] =
                                65535;

                            outIndex++;
                        }
                    }
                    inIndex++;
                }

                /* We've reached the end of the line, time to start a new scan
                 * if we don't have all the polys yet
                 */
            }

            /* Right, all done */
            RwFree(oldIndices);
        }

        /* Failed to allocate a triangle buffer
         *  - not much we can do with this mesh */
        RwFree(vertexTagBuffer);

        RWRETURN(mesh);
    }

    /* Failed to allocate a vertex tag table */
    RWRETURN(mesh);
}


/****************************************************************************
 TriStripAddEdge

 On entry:
 On exit:
 */
static Edge *
TriStripAddEdge(RwFreeList *edgeFreeList,
                Edge **edgelist,
                RwUInt16 v1,
                RwUInt16 v2,
                RwUInt32 tri,
                TriBinEntry **binEntryArray)
{
    Edge               *temp = *edgelist;
    Edge               *newEdge;

    RWFUNCTION(RWSTRING("TriStripAddEdge"));

    while (temp)
    {
        if (v1 == temp->v2 && v2 == temp->v1)
        {
            /* Can't have edge adjacency more than 2 */
            if (!temp->tri2)
            {
                /* Then adjcount must be equal to 1 */
                /* Up the adjacency on existing triangle on this edge */
                temp->tri1->adjCount++;

                /* Adjacency of triangle we are adding edge from goes up too */
                binEntryArray[tri]->adjCount++;
                temp->tri2 = binEntryArray[tri];
                RWRETURN(temp);
            }
        }
        temp = temp->next;
    }

    /* not in list so malloc a new edge */
    newEdge = (Edge *) RwFreeListAlloc(edgeFreeList,
        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    newEdge->v1 = v1;
    newEdge->v2 = v2;
    newEdge->tri1 = binEntryArray[tri];
    newEdge->tri2 = (TriBinEntry *)NULL;
    newEdge->next = *edgelist;

    *edgelist = newEdge;

    RWRETURN(newEdge);
}

static TriBinEntry **
TriStripBinEntryArrayDestroy(RwUInt32 numTris,
                             MeshOpFreeLists *meshOpFreeLists,
                             Edge *edge,
                             TriBinEntry **binEntryArray)
{
    RwUInt32            i;

    RWFUNCTION(RWSTRING("TriStripBinEntryArrayDestroy"));

    /*
     * Release meshOpFreeLists->edgeFreeList
     */
    while (edge)
    {
        Edge               *next = edge->next;

        RwFreeListFree(meshOpFreeLists->edgeFreeList, edge);
        edge = (Edge *)NULL;

        edge = next;
    }
    RwFreeListDestroy(meshOpFreeLists->edgeFreeList);
    meshOpFreeLists->edgeFreeList = (RwFreeList *)NULL;

    /*
     * Release meshOpFreeLists->binEntryFreeList
     */
    for (i = 0; i < numTris; i++)
    {
        /* free memory from meshOpFreeLists->binEntryFreeList */
        RwFreeListFree(meshOpFreeLists->binEntryFreeList,
                       binEntryArray[i]);
        binEntryArray[i] = (TriBinEntry *)NULL;
    }
    RwFreeListDestroy(meshOpFreeLists->binEntryFreeList);
    meshOpFreeLists->binEntryFreeList = (RwFreeList *)NULL;

    /*
     * Release binEntryArray
     */
    RwFree(binEntryArray);
    binEntryArray = (TriBinEntry **)NULL;

    RWRETURN(binEntryArray);
}


/****************************************************************************
 TriStripBinEntryArrayCreate

 On entry:
 On exit:
 */
static TriBinEntry **
TriStripBinEntryArrayCreate(RwUInt32 numTris,
                            MeshOpFreeLists *meshOpFreeLists,
                            Edge **edgelist,
                            RpBuildMeshTriangle *triList)
{
    TriBinEntry       **binEntryArray = (TriBinEntry **)NULL;
    RwUInt32            i;

    RWFUNCTION(RWSTRING("TriStripBinEntryArrayCreate"));

    binEntryArray = (TriBinEntry **)
        RwMalloc(sizeof(TriBinEntry *) * numTris,
        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    meshOpFreeLists->binEntryFreeList =
        RwFreeListCreate(sizeof(TriBinEntry), numTris, sizeof(RwUInt32),
                         rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    meshOpFreeLists->edgeFreeList =
        RwFreeListCreate(sizeof(Edge), (numTris / 2) + 1, sizeof(RwUInt32),
                         rwMEMHINTDUR_EVENT | rwID_MESHMODULE);

    for (i = 0; i < numTris; i++)
    {
        binEntryArray[i] = (TriBinEntry *)
            RwFreeListAlloc(meshOpFreeLists->binEntryFreeList,
                        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
        binEntryArray[i]->adjCount = 0;
        binEntryArray[i]->tri = i;
        binEntryArray[i]->prev = (TriBinEntry *)NULL;
        binEntryArray[i]->next = (TriBinEntry *)NULL;
        binEntryArray[i]->used = FALSE;
        binEntryArray[i]->edge[0] =
            TriStripAddEdge(meshOpFreeLists->edgeFreeList,
                            edgelist,
                            triList[i].vertIndex[0],
                            triList[i].vertIndex[1], i, binEntryArray);
        binEntryArray[i]->edge[1] =
            TriStripAddEdge(meshOpFreeLists->edgeFreeList, edgelist,
                            triList[i].vertIndex[1],
                            triList[i].vertIndex[2], i, binEntryArray);
        binEntryArray[i]->edge[2] =
            TriStripAddEdge(meshOpFreeLists->edgeFreeList, edgelist,
                            triList[i].vertIndex[2],
                            triList[i].vertIndex[0], i, binEntryArray);
    }

    RWRETURN(binEntryArray);
}


/****************************************************************************
 TriStripGetTriEdge

 On entry:
 On exit:
 */
static Edge *
TriStripGetTriEdge(TriBinEntry *binEntry,
                   RwUInt16 v1,
                   RwUInt16 v2 /*, RwInt32 currentAttempt */ )
{
    RWFUNCTION(RWSTRING("TriStripGetTriEdge"));
    RWASSERT(binEntry);


    if ((binEntry->edge[0]->v1 == v1 && binEntry->edge[0]->v2 == v2)
        || (binEntry->edge[0]->v1 == v2
            && binEntry->edge[0]->v2 == v1))
    {
        RWRETURN(binEntry->edge[0]);
    }
    if ((binEntry->edge[1]->v1 == v1 && binEntry->edge[1]->v2 == v2)
        || (binEntry->edge[1]->v1 == v2
            && binEntry->edge[1]->v2 == v1))
    {
        RWRETURN(binEntry->edge[1]);
    }
    if ((binEntry->edge[2]->v1 == v1 && binEntry->edge[2]->v2 == v2)
        || (binEntry->edge[2]->v1 == v2
            && binEntry->edge[2]->v2 == v1))
    {
        RWRETURN(binEntry->edge[2]);
    }

    RWRETURN((Edge *)NULL);
}


/****************************************************************************
 TriStripMarkTriUsed

 On entry:
 On exit:
 */
static void
TriStripMarkTriUsed(TriBinEntry *tri,
                    TriBinList *binListArray,
                    /* TriBinEntry **binEntryArray, */
                    RwInt32 currentAttempt)
{
    RwUInt8             i;
    TriBinEntry        *newTri;

    RWFUNCTION(RWSTRING("TriStripMarkTriUsed"));
    RWASSERT(tri);

    if (currentAttempt < 4)
    {
        /* mark used */
        tri->used2 = TRUE;

        /* no need to touch the bin lists or triangle adjacency counts
           in the temporary stripping tests */
    }
    else
    {
        /* mark used */
        tri->used = TRUE;

        /* so remove it from the bin lists */
        if (binListArray[tri->adjCount].head == tri)
        {
            /* remove from head of a list */
            binListArray[tri->adjCount].head =
                binListArray[tri->adjCount].head->next;
            if (binListArray[tri->adjCount].head)
            {
                binListArray[tri->adjCount].head->prev = (TriBinEntry *)NULL;
            }
        }
        else
        {
            /* remove it from the list */
            if (tri->next)
            {
                tri->next->prev = tri->prev;
            }
            if (tri->prev)
            {
                tri->prev->next = tri->next;
            }
        }

        for (i = 0; i < 3; i++)
        {
            newTri = FALSE;
            /* for each of the three edges find the other adjacent
               triangle if present and update it's adjacency counts
               and binList membership */
            if (tri->edge[i]->tri1 &&
                tri->edge[i]->tri1 != tri &&
                tri->edge[i]->tri1->used == FALSE)
            {
                newTri = tri->edge[i]->tri1;
            }
            else if (tri->edge[i]->tri2 &&
                     tri->edge[i]->tri2->used == FALSE)
            {
                newTri = tri->edge[i]->tri2;
            }

            if (newTri)
            {
                if (binListArray[newTri->adjCount].head == newTri)
                {
                    /* remove from head of a list */
                    binListArray[newTri->adjCount].head =
                        binListArray[newTri->adjCount].head->next;
                    if (binListArray[newTri->adjCount].head)
                    {
                        binListArray[newTri->adjCount].head->prev =
                            (TriBinEntry *)NULL;
                    }
                }
                else
                {
                    /* remove it from the list */
                    if (newTri->next)
                    {
                        newTri->next->prev = newTri->prev;
                    }
                    if (newTri->prev)
                    {
                        newTri->prev->next = newTri->next;
                    }
                }

                newTri->adjCount--;

                /* reinsert into the list */
                newTri->next = binListArray[newTri->adjCount].head;
                if (newTri->next)
                {
                    newTri->next->prev = newTri;
                }
                binListArray[newTri->adjCount].head = newTri;
                newTri->prev = (TriBinEntry *)NULL;
            }
        }
    }

    RWRETURNVOID();
}


#ifdef TRISTRIPLOOKAHEAD
/****************************************************************************
 TriStripIsLastTriInStrip

 On entry:
 On exit:
 */
static RwBool
TriStripIsLastTriInStrip(Edge *outgoingEdge,
                         TriBinEntry *currentTri,
                         TriBinEntry **binEntryArray,
                         RwInt32 currentAttempt)
{
    RwInt32             i;
    TriBinEntry        *nextTri = (TriBinEntry *)NULL;

    RWFUNCTION(RWSTRING("TriStripIsLastTriInStrip"));

    if (currentAttempt < 4)
    {
        if (outgoingEdge->tri1 &&
            outgoingEdge->tri1 != currentTri &&
            !outgoingEdge->tri1->used2)
        {
            nextTri = outgoingEdge->tri1;
        }
        else if (outgoingEdge->tri2 &&
                 outgoingEdge->tri2 != currentTri &&
                 !outgoingEdge->tri2->used2)
        {
            nextTri = outgoingEdge->tri2;
        }

        if (!nextTri)
        {
            RWRETURN(TRUE);
        }

        for (i = 0; i < 3; i++)
        {
            if (nextTri->edge[i] != outgoingEdge &&
                nextTri->edge[i] && EdgeAdjCount2(nextTri->edge[i]) > 1)
            {
                RWRETURN(FALSE);
            }
        }
    }
    else
    {
        if (outgoingEdge->tri1 &&
            outgoingEdge->tri1 != currentTri &&
            !outgoingEdge->tri1->used)
        {
            nextTri = outgoingEdge->tri1;
        }
        else if (outgoingEdge->tri2 &&
                 outgoingEdge->tri2 != currentTri &&
                 !outgoingEdge->tri2->used)
        {
            nextTri = outgoingEdge->tri2;
        }

        if (!nextTri)
        {
            RWRETURN(TRUE);
        }

        for (i = 0; i < 3; i++)
        {
            if (nextTri->edge[i] != outgoingEdge &&
                nextTri->edge[i] &&
                EdgeAdjCount(nextTri->edge[i]) > 1)
            {
                RWRETURN(FALSE);
            }
        }
    }

    RWRETURN(TRUE);
}
#endif /* TRISTRIPLOOKAHEAD */


/****************************************************************************
 TriStripFollow

 On entry:
 On exit:
 */
static RwUInt32
TriStripFollow(TriStripListEntry *strip,
               Edge *nextEdge,
               TriBinList *binListArray,
               RpBuildMeshTriangle *triList __RWUNUSED__,
               /* TriBinEntry **binEntryArray, */
               /* Edge *edgelist, */
               RwInt32 currentAttempt)
{
    RwUInt32            addedTris = 0;
    Edge               *prevEdge = (Edge *)NULL;
    Edge               *otherEdge = (Edge *)NULL;
    RwBool              nextIsLast = FALSE;
    RwBool              otherIsLast = FALSE;
    RwBool              turnResult;

    RWFUNCTION(RWSTRING("TriStripFollow"));

    while (nextEdge)
    {
        TriBinEntry *       bestTri = (TriBinEntry *)NULL;
        RwUInt16            v1;
        RwUInt16            v2;
        RwUInt16            v3 = 0;
        RwInt32             nextEdgeIndex = -1; /* index of nextEdge in bestTri */

        /* Order the vertices to get winding order right */
        if (strip->stripLen % 2)
        {
            v1 = strip->strip[strip->stripLen - 1];
            v2 = strip->strip[strip->stripLen - 2];
        }
        else
        {
            v1 = strip->strip[strip->stripLen - 2];
            v2 = strip->strip[strip->stripLen - 1];
        }

        /* Find next triangle from edge (go for high adjacency) */
        if (currentAttempt < 4)
        {
            if (nextEdge->tri1 &&
                nextEdge->tri1->used2 == FALSE)
            {
                bestTri = nextEdge->tri1;
            }
            else if (nextEdge->tri2 &&
                     nextEdge->tri2->used2 == FALSE)
            {
                bestTri = nextEdge->tri2;
            }
        }
        else
        {
            if (nextEdge->tri1 &&
                nextEdge->tri1->used == FALSE)
            {
                bestTri = nextEdge->tri1;
            }
            else if (nextEdge->tri2 &&
                     nextEdge->tri2->used == FALSE)
            {
                bestTri = nextEdge->tri2;
            }
        }

        if (!bestTri)
        {
            RWRETURN(addedTris);
        }

        addedTris++;

        /* sort out the adjacency counts */
        TriStripMarkTriUsed(bestTri, binListArray,
                            /* binEntryArray, */currentAttempt);

        /* work out the 3rd vertex */
        if (nextEdge == bestTri->edge[0])
        {
            nextEdgeIndex = 0;
        }
        else if (nextEdge == bestTri->edge[1])
        {
            nextEdgeIndex = 1;
        }
        else if (nextEdge == bestTri->edge[2])
        {
            nextEdgeIndex = 2;
        }

        RWASSERT(nextEdgeIndex != -1);

        /* now find the 3rd vertex from the next edge around the triangle */
        if (bestTri->edge[(nextEdgeIndex+1)%3]->tri1 == bestTri) /* edge direction */
        {
            v3 = bestTri->edge[(nextEdgeIndex+1)%3]->v2;
        }
        else
        {
            v3 = bestTri->edge[(nextEdgeIndex+1)%3]->v1;
        }

        prevEdge = nextEdge;
        nextEdge = TriStripGetTriEdge(bestTri,
                                      strip->strip[strip->stripLen -1],
                                      v3 /* ,  currentAttempt */ );
#ifdef TRISTRIPLOOKAHEAD
        nextIsLast =
            TriStripIsLastTriInStrip(nextEdge, bestTri, binEntryArray,
                                     currentAttempt);
#endif /* TRISTRIPLOOKAHEAD */

#ifdef TURNCORNERS
        if (currentAttempt < 4)
        {
            turnResult = !nextEdge || (EdgeAdjCount2(nextEdge) == 0)
                || nextIsLast;
        }
        else
        {
            turnResult = !nextEdge || (EdgeAdjCount(nextEdge) == 0)
                || nextIsLast;
        }
#else /* TURNCORNERS */
        turnResult = FALSE;
#endif /* TURNCORNERS */
        if (turnResult)
        {
            /* find the 3rd edge */
            if (bestTri->edge[0] != prevEdge &&
                bestTri->edge[0] != nextEdge)
            {
                otherEdge = bestTri->edge[0];
            }
            else if (bestTri->edge[1] != prevEdge &&
                     bestTri->edge[1] != nextEdge)
            {
                otherEdge = bestTri->edge[1];
            }
            else if (bestTri->edge[2] != prevEdge &&
                     bestTri->edge[2] != nextEdge)
            {
                otherEdge = bestTri->edge[2];
            }


#ifdef TRISTRIPLOOKAHEAD
            otherIsLast =
                TriStripIsLastTriInStrip(otherEdge, bestTri,
                                         binEntryArray, currentAttempt);
#endif /* TRISTRIPLOOKAHEAD */

            if (currentAttempt < 4)
            {
                turnResult = (otherEdge && EdgeAdjCount2(otherEdge)
                              && !(otherIsLast && nextIsLast));
            }
            else
            {
                turnResult = (otherEdge && EdgeAdjCount(otherEdge)
                              && !(otherIsLast && nextIsLast));
            }
            if (turnResult)
            {
                /* other edge has adjacencies */
                if (strip->stripLen % 2)
                {
                    /* normal vert */
                    strip->strip[strip->stripLen] = v3;
                    strip->stripLen++;
                    /* swapper */
#ifdef ODDTURNS                /* odd turns */
                    strip->strip[strip->stripLen] = v3;
                    strip->stripLen++;

                    /* swapper */
                    strip->strip[strip->stripLen] =
                        strip->strip[strip->stripLen - 4];
                    strip->stripLen++;
                    nextEdge = otherEdge;
#else /* ODDTURNS */
                    nextEdge = (Edge *)NULL;
#endif /* ODDTURNS */
                }
                else
                {
                    /* swapper */
                    strip->strip[strip->stripLen] =
                        strip->strip[strip->stripLen - 2];
                    strip->stripLen++;
                    /* now output the appropriate vertex */
                    strip->strip[strip->stripLen] = v3;
                    strip->stripLen++;
                    nextEdge = otherEdge;
                }
            }
            else
            {
                /* just output the appropriate vertex */
                strip->strip[strip->stripLen] = v3;
                strip->stripLen++;
                nextEdge = (Edge *)NULL;
            }
        }
        else
        {
            /* just output the appropriate vertex */
            strip->strip[strip->stripLen] = v3;
            strip->stripLen++;
        }
    }

    RWRETURN(addedTris);
}


/****************************************************************************
 TriStripStripTris

 On entry:
 On exit:
 */
static RwBool
TriStripStripTris(RpBuildMeshTriangle *triList,
                  RwUInt32 numTris,
                  TriStripList *stripList,
                  RwBool preprocess)
{
    /* build a face and edge list */
    Edge               *edgelist = (Edge *)NULL;
    TriStripListEntry  *newStrip, *buildStrip, *revBuildStrip;
    RwUInt32            i, j;
    RwUInt32            trisUsed = 0;
    RwUInt32            trisUsedTemp = 0;
    TriBinList          binListArray[4];
    TriBinEntry       **binEntryArray = (TriBinEntry **)NULL;
    Edge               *nextEdge = (Edge *)NULL;
    Edge               *firstEdge = (Edge *)NULL;
    RwUInt32            offset, cacheOffset;
    MeshOpFreeLists     meshOpFreeLists;
    RwInt32             bestOffset, currentAttempt;
    RwUInt32            bestSize = 0;
    RwBool              testResult;

    RWFUNCTION(RWSTRING("TriStripStripTris"));

    /* We build several lists based on triangle adjacency */
    binListArray[0].head = (TriBinEntry *)NULL;
    binListArray[1].head = (TriBinEntry *)NULL;
    binListArray[2].head = (TriBinEntry *)NULL;
    binListArray[3].head = (TriBinEntry *)NULL;

    meshOpFreeLists.binEntryFreeList = (RwFreeList *)NULL;
    meshOpFreeLists.edgeFreeList = (RwFreeList *)NULL;

    binEntryArray =
        TriStripBinEntryArrayCreate(numTris, &meshOpFreeLists,
                                    &edgelist, triList);

    /* Build the lists of similarly adjacent triangles */
    for (i = 0; i < numTris; i++)
    {
        binEntryArray[i]->next =
            binListArray[binEntryArray[i]->adjCount].head;

        if (binEntryArray[i]->next)
        {
            binEntryArray[i]->next->prev = binEntryArray[i];
        }
        binListArray[binEntryArray[i]->adjCount].head =
            binEntryArray[i];
        binEntryArray[i]->prev = (TriBinEntry *)NULL;
    }

    /* Allocate a buildstrip to avoid reallocs */
    buildStrip = (TriStripListEntry *)
        RwFreeListAlloc(RPMESHGLOBAL(triStripListEntryFreeList),
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    buildStrip->stripSize = (numTris * 2) + 2;
    buildStrip->stripLen = 0;
    buildStrip->strip = (RwUInt16 *)
        RwMalloc(sizeof(RwUInt32) * buildStrip->stripSize,
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    /* And another for backwards stripping */
    revBuildStrip = (TriStripListEntry  *)
        RwFreeListAlloc(RPMESHGLOBAL(triStripListEntryFreeList),
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    revBuildStrip->stripSize = (numTris * 2) + 2;
    revBuildStrip->stripLen = 0;
    revBuildStrip->strip = (RwUInt16 *)
        RwMalloc(sizeof(RwUInt32) * revBuildStrip->stripSize,
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    while (trisUsed < numTris)
    {
        bestOffset = 0;
        bestSize = 0;

        /* Do we preview the strips? If we do then we try more times than normal */
        if (preprocess)
        {
            /* Try each edge (0,1 and 2) */
            currentAttempt = 0;
        }
        else
        {
            /* Just go ahead and generate the strips first time */
            currentAttempt = 3;
        }

        /* Find a starter */
        if (binListArray[0].head)
        {
            /* Pull the first triangle off the list */
            RwUInt32            tri = binListArray[0].head->tri;

            /* If we have any separated tri's make into a strip */
            newStrip = (TriStripListEntry  *)
                RwFreeListAlloc(RPMESHGLOBAL
                                (triStripListEntryFreeList),
                                rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
            newStrip->next = stripList->head;
            stripList->head = newStrip;

            newStrip->stripSize = 3;
            newStrip->stripLen = 3;
            newStrip->strip = (RwUInt16 *)  RwMalloc(sizeof(RwUInt32) * 3,
                                     rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
            newStrip->strip[0] = triList[tri].vertIndex[0];
            newStrip->strip[1] = triList[tri].vertIndex[1];
            newStrip->strip[2] = triList[tri].vertIndex[2];
            binListArray[0].head->used = TRUE;
            binListArray[0].head->used2 = TRUE;
            binListArray[0].head = binListArray[0].head->next;
            if (binListArray[0].head)
            {
                binListArray[0].head->prev = (TriBinEntry *)NULL;
            }
            trisUsed++;
        }
        else
        {
            RpBuildMeshTriangle *source;

            /* Pick start triangle from lowest adjacency list */
            i = 1;
            while (!binListArray[i].head)
            {
                i++;
            }

            /* Then trace an edge */
            if (EdgeAdjCount(binListArray[i].head->edge[2]) >= 2 &&
                EdgeAdjCount(binListArray[i].head->edge[1]) >= 2)
            {
                offset = 1;
            }
            else if (EdgeAdjCount(binListArray[i].head->edge[0]) >= 2 &&
                     EdgeAdjCount(binListArray[i].head->edge[2]) >= 2)
            {
                offset = 2;
            }
            else if (EdgeAdjCount(binListArray[i].head->edge[1]) >= 2 &&
                     EdgeAdjCount(binListArray[i].head->edge[0]) >= 2)
            {
                offset = 0;
            }
            else
            {
                if (EdgeAdjCount(binListArray[i].head->edge[0]) >
                    EdgeAdjCount(binListArray[i].head->edge[1]))
                {
                    if (EdgeAdjCount(binListArray[i].head->edge[0]) >
                        EdgeAdjCount(binListArray[i].head->edge[2]))
                    {
                        offset = 2;
                    }
                    else
                    {
                        offset = 1;
                    }
                }
                else
                {
                    if (EdgeAdjCount(binListArray[i].head->edge[1]) >
                        EdgeAdjCount(binListArray[i].head->edge[2]))
                    {
                        offset = 0;
                    }
                    else
                    {
                        offset = 1;
                    }
                }
            }

            /* Cache the tris used */
            trisUsedTemp = trisUsed;
            cacheOffset = offset;
            bestOffset = offset;

            /* Jump back to here to restart the strip length attempt */
          labelRestart:

            /* restore the number of triangles currently used */
            trisUsed = trisUsedTemp;

            /* reset all adjCount2 vars to be the same as adjCount vars. The adjCount2 vars
             * are temporary - we use them to test the stripping from each edge */
            for (j = 0; j < numTris; j++)
            {
                binEntryArray[j]->used2 = binEntryArray[j]->used;
            }

            /* This is going to be attempted 3 times and we're going to pick the best attempt */
            switch (currentAttempt++)
            {
                case 0:
                    /* Edge order remains the same */
                    offset = (cacheOffset + 0) % 3;
                    break;
                case 1:
                    /* Try a different edge order */
                    offset = (cacheOffset + 1) % 3;
                    break;
                case 2:
                    /* Try the final edge order */
                    offset = (cacheOffset + 2) % 3;
                    break;
                default:
                    /* This is the final (and catch-all) case that we use */
                    offset = bestOffset;
                    break;
            }

            /* Now do it */
            switch (offset)
            {
                case 0:
                    nextEdge = binListArray[i].head->edge[1]; /* 1->2 */
                    firstEdge = binListArray[i].head->edge[0]; /* 0->1 */
                    break;
                case 1:
                    nextEdge = binListArray[i].head->edge[2]; /* 2->0 */
                    firstEdge = binListArray[i].head->edge[1]; /* 1->2 */
                    break;
                case 2:
                    nextEdge = binListArray[i].head->edge[0]; /* 0->1 */
                    firstEdge = binListArray[i].head->edge[2]; /* 2->0 */
                    break;
            }

            source = &triList[binListArray[i].head->tri];

            buildStrip->strip[0] = source->vertIndex[(0 + offset) % 3];
            buildStrip->strip[1] = source->vertIndex[(1 + offset) % 3];
            buildStrip->strip[2] = source->vertIndex[(2 + offset) % 3];
            buildStrip->stripLen = 3;

            TriStripMarkTriUsed(binListArray[i].head, binListArray,
                                /* binEntryArray, */ currentAttempt);
            trisUsed++;

            /* Follow along a strip from it */
            trisUsed += TriStripFollow(buildStrip, nextEdge,
                                       binListArray, triList,
                                       /* binEntryArray, edgelist, */
                                       currentAttempt);

            /* If we're trying to build the best strip then use the "temp" vars */
            if (currentAttempt < 4)
            {
                testResult = (EdgeAdjCount2(firstEdge) > 0);
            }
            else
            {
                testResult = (EdgeAdjCount(firstEdge) > 0);
            }
            if (testResult)
            {
                /* Now work back from the start */
                revBuildStrip->strip[0] = buildStrip->strip[1];
                revBuildStrip->strip[1] = buildStrip->strip[0];
                revBuildStrip->stripLen = 2;
                trisUsed += TriStripFollow(revBuildStrip, firstEdge,
                                           binListArray, triList,
                                           /* binEntryArray, edgelist, */
                                           currentAttempt);

                if (revBuildStrip->stripLen % 2)
                {
                    revBuildStrip->strip[revBuildStrip->stripLen] =
                        revBuildStrip->strip[revBuildStrip->stripLen -
                                             2];
                    revBuildStrip->stripLen++;
                }

                /* Check the length, record if necessary,
                 * reset trisUsed and jump back */
                if (trisUsed > bestSize)
                {
                    bestSize = trisUsed;
                    bestOffset = offset;
                }
                if (currentAttempt < 4)
                {
                    /* we're still trying to get the best strip length,
                     * so jump back */
                    goto labelRestart;
                }

                /* Finally copy it and add to the list */
                newStrip = (TriStripListEntry  *)
                    RwFreeListAlloc(RPMESHGLOBAL
                                    (triStripListEntryFreeList),
                                    rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                newStrip->next = stripList->head;
                stripList->head = newStrip;

                newStrip->stripSize =
                    buildStrip->stripLen + revBuildStrip->stripLen - 2;
                newStrip->stripLen = 0;
                newStrip->strip = (RwUInt16 *)
                    RwMalloc(sizeof(RwUInt32) * newStrip->stripSize,
                             rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                while (revBuildStrip->stripLen > 2)
                {
                    newStrip->strip[newStrip->stripLen] =
                        revBuildStrip->strip[revBuildStrip->stripLen -
                                             1];
                    newStrip->stripLen++;
                    revBuildStrip->stripLen--;
                }
                memcpy(&newStrip->strip[newStrip->stripLen],
                       buildStrip->strip,
                       sizeof(RwUInt32) * buildStrip->stripLen);
                newStrip->stripLen = newStrip->stripSize;
            }
            else
            {
                /* Check the length, record if necessary, reset trisUsed and jump back */
                if (trisUsed > bestSize)
                {
                    bestSize = trisUsed;
                    bestOffset = offset;
                }
                if (currentAttempt < 4)
                {
                    /* we're still trying to get the best strip length,
                     * so jump back */
                    goto labelRestart;
                }

                /* Finally copy it and add to the list */
                newStrip = (TriStripListEntry  *)
                    RwFreeListAlloc(RPMESHGLOBAL
                                    (triStripListEntryFreeList),
                                    rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                newStrip->next = stripList->head;
                stripList->head = newStrip;

                newStrip->stripSize = buildStrip->stripLen;
                newStrip->stripLen = buildStrip->stripLen;
                newStrip->strip = (RwUInt16 *)
                    RwMalloc(sizeof(RwUInt32) * buildStrip->stripLen,
                             rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                memcpy(newStrip->strip, buildStrip->strip, sizeof
                       (RwUInt32) * buildStrip->stripLen);
            }
        }
    }

    RwFree(revBuildStrip->strip);
    revBuildStrip->strip = (RwUInt16 *)  NULL;
    RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                   revBuildStrip);
    revBuildStrip = (TriStripListEntry *)NULL;

    RwFree(buildStrip->strip);
    buildStrip->strip = (RwUInt16 *)  NULL;
    RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList), buildStrip);
    buildStrip = (TriStripListEntry *)NULL;

    binEntryArray =
        TriStripBinEntryArrayDestroy(numTris, &meshOpFreeLists,
                                     edgelist, binEntryArray);

    RWRETURN(TRUE);
}


/****************************************************************************
 TriStripStripTrisExhaustive

 On entry:
 On exit:
 */
static RwBool
TriStripStripTrisExhaustive(RpBuildMeshTriangle *triList,
                            RwUInt32 numTris,
                            TriStripList *stripList)
{
    /* build a face and edge list */
    Edge               *edgelist = (Edge *)NULL;
    TriStripListEntry  *newStrip, *buildStrip, *revBuildStrip;
    RwUInt32            i;
    RwUInt32            trisUsed = 0;
    TriBinList          binListArray[4];
    TriBinEntry       **binEntryArray = (TriBinEntry **)NULL;
    Edge               *nextEdge = (Edge *)NULL;
    Edge               *firstEdge = (Edge *)NULL;
    RwUInt32            offset = 0;
    MeshOpFreeLists     meshOpFreeLists;
    TriBinEntry        *bestTri = (TriBinEntry *)NULL;
    RwInt32             bestLength = 0;

    RWFUNCTION(RWSTRING("TriStripStripTrisExhaustive"));

    /* We build several lists based on triangle adjacency */
    binListArray[0].head = (TriBinEntry *)NULL;
    binListArray[1].head = (TriBinEntry *)NULL;
    binListArray[2].head = (TriBinEntry *)NULL;
    binListArray[3].head = (TriBinEntry *)NULL;

    meshOpFreeLists.binEntryFreeList = (RwFreeList *)NULL;
    meshOpFreeLists.edgeFreeList = (RwFreeList *)NULL;

    binEntryArray =
        TriStripBinEntryArrayCreate(numTris, &meshOpFreeLists,
                                    &edgelist, triList);

    /* Build the lists of similarly adjacent triangles */
    for (i = 0; i < numTris; i++)
    {
        binEntryArray[i]->next =
            binListArray[binEntryArray[i]->adjCount].head;

        if (binEntryArray[i]->next)
        {
            binEntryArray[i]->next->prev = binEntryArray[i];
        }
        binListArray[binEntryArray[i]->adjCount].head =
            binEntryArray[i];
        binEntryArray[i]->prev = (TriBinEntry *)NULL;
    }

    /* Allocate a buildstrip to avoid reallocs */
    buildStrip = (TriStripListEntry  *)
        RwFreeListAlloc(RPMESHGLOBAL(triStripListEntryFreeList),
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    buildStrip->stripSize = (numTris * 2) + 2;
    buildStrip->stripLen = 0;
    buildStrip->strip = (RwUInt16 *)
        RwMalloc(sizeof(RwUInt32) * buildStrip->stripSize,
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    /* And another for backwards stripping */
    revBuildStrip = (TriStripListEntry  *)
        RwFreeListAlloc(RPMESHGLOBAL(triStripListEntryFreeList),
                        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    revBuildStrip->stripSize = (numTris * 2) + 2;
    revBuildStrip->stripLen = 0;
    revBuildStrip->strip = (RwUInt16 *)
        RwMalloc(sizeof(RwUInt32) * revBuildStrip->stripSize,
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    while (trisUsed < numTris)
    {
        /* If there are any 0 adjacency tris simply add them */
        if (binListArray[0].head)
        {
            /* Pull the first triangle off the list */
            RwUInt32            tri = binListArray[0].head->tri;

            /* If we have any separated tri's make into a strip */
            newStrip = (TriStripListEntry  *)
                RwFreeListAlloc(RPMESHGLOBAL
                                (triStripListEntryFreeList),
                                rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
            newStrip->next = stripList->head;
            stripList->head = newStrip;

            newStrip->stripSize = 3;
            newStrip->stripLen = 3;
            newStrip->strip = (RwUInt16 *)  RwMalloc(sizeof(RwUInt32) * 3,
                                    rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
            newStrip->strip[0] = triList[tri].vertIndex[0];
            newStrip->strip[1] = triList[tri].vertIndex[1];
            newStrip->strip[2] = triList[tri].vertIndex[2];
            binListArray[0].head->used = TRUE;
            binListArray[0].head = binListArray[0].head->next;
            if (binListArray[0].head)
            {
                binListArray[0].head->prev = (TriBinEntry *)NULL;
            }
            trisUsed++;
        }
        else
        {
            RpBuildMeshTriangle *source;
            RwInt32             startTrisUsed;

            /* Test all possible start triangles and edges */
            startTrisUsed = trisUsed;
            bestTri = (TriBinEntry *)NULL;
            /* the lengths can actually come out with negative scores for poor
               strips, hence to make sure we find one valid strip initialize
               bestLength to RwInt32MINVAL */
            bestLength = RwInt32MINVAL;
            for (i = 1; i < 4; i++)
            {
                TriBinEntry        *entry = binListArray[i].head;

                while (entry)
                {
                    RwUInt32            j;
                    RwInt32             length;

                    for (j = 0; j < 3; j++)
                    {
                        if (EdgeAdjCount(entry->edge[(j + 1) % 3]))
                        {
                            RwUInt32            k;

                            /* restore the number of triangles
                             * currently used */
                            trisUsed = startTrisUsed;

                            /* reset all adjCount2 vars to be the same as
                             * adjCount vars.
                             * The adjCount2 vars are temporary -
                             * we use them to test the stripping
                             * from each edge */
                            for (k = 0; k < numTris; k++)
                            {
                                binEntryArray[k]->used2 =
                                    binEntryArray[k]->used;
                            }

                            switch (j)
                            {
                                case 0:
                                    nextEdge = entry->edge[1]; /* 1->2 */
                                    firstEdge = entry->edge[0]; /* 0->1 */
                                    break;
                                case 1:
                                    nextEdge = entry->edge[2]; /* 2->0 */
                                    firstEdge = entry->edge[1]; /* 1->2 */
                                    break;
                                case 2:
                                    nextEdge = entry->edge[0]; /* 0->1 */
                                    firstEdge = entry->edge[2]; /* 2->0 */
                                    break;
                            }

                            source = &triList[entry->tri];

                            buildStrip->strip[0] =
                                source->vertIndex[(0 + j) % 3];
                            buildStrip->strip[1] =
                                source->vertIndex[(1 + j) % 3];
                            buildStrip->strip[2] =
                                source->vertIndex[(2 + j) % 3];
                            buildStrip->stripLen = 3;
                            TriStripMarkTriUsed(entry, binListArray,
                                                /* binEntryArray, */ 1);
                            trisUsed++;
                            /* Follow along a strip from it */
                            trisUsed +=
                                TriStripFollow(buildStrip, nextEdge,
                                               binListArray, triList,
                                               /* binEntryArray, edgelist, */
                                               1);
                            /* we don't do any back stripping since by testing
                             * all starts we'll hit the start of each possible
                             * strip */

                            length =
                                (trisUsed - startTrisUsed) -
                                (buildStrip->stripLen -
                                 (trisUsed - startTrisUsed));
                            /* (length = (trisUsed - startTrisUsed); */

                            if (length > bestLength)
                            {
                                bestLength = length;
                                bestTri = entry;
                                offset = j;
                            }
                        }
                    }

                    entry = entry->next;
                }
            }
            trisUsed = startTrisUsed;
#if 0
            RWMESSAGE(("%d", bestLength));
#endif

            /* Now we know the best tri and offset, so add that strip */
            switch (offset)
            {
                case 0:
                    nextEdge = bestTri->edge[1]; /* 1->2 */
                    firstEdge = bestTri->edge[0]; /* 0->1 */
                    break;
                case 1:
                    nextEdge = bestTri->edge[2]; /* 2->0 */
                    firstEdge = bestTri->edge[1]; /* 1->2 */
                    break;
                case 2:
                    nextEdge = bestTri->edge[0]; /* 0->1 */
                    firstEdge = bestTri->edge[2]; /* 2->0 */
                    break;
            }

            source = &triList[bestTri->tri];

            buildStrip->strip[0] = source->vertIndex[(0 + offset) % 3];
            buildStrip->strip[1] = source->vertIndex[(1 + offset) % 3];
            buildStrip->strip[2] = source->vertIndex[(2 + offset) % 3];
            buildStrip->stripLen = 3;
            TriStripMarkTriUsed(bestTri, binListArray,
                                /* binEntryArray, */ 4);
            trisUsed++;
            /* Follow along a strip from it */
            trisUsed += TriStripFollow(buildStrip, nextEdge,
                                       binListArray, triList,
                                       /* binEntryArray, edgelist, */
                                       4);
            if (EdgeAdjCount(firstEdge))
            {
                /* Now work back from the start */
                revBuildStrip->strip[0] = buildStrip->strip[1];
                revBuildStrip->strip[1] = buildStrip->strip[0];
                revBuildStrip->stripLen = 2;
                trisUsed += TriStripFollow(revBuildStrip, firstEdge,
                                           binListArray, triList,
                                           /* binEntryArray, edgelist, */
                                           4);

                if (revBuildStrip->stripLen % 2)
                {
                    revBuildStrip->strip[revBuildStrip->stripLen] =
                        revBuildStrip->strip[revBuildStrip->stripLen -
                                             2];
                    revBuildStrip->stripLen++;
                }
                /* Finally copy it and add to the list */
                newStrip = (TriStripListEntry  *)
                    RwFreeListAlloc(RPMESHGLOBAL
                                    (triStripListEntryFreeList),
                                    rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                newStrip->next = stripList->head;
                stripList->head = newStrip;

                newStrip->stripSize = buildStrip->stripLen +
                    revBuildStrip->stripLen - 2;
                newStrip->stripLen = 0;
                newStrip->strip = (RwUInt16 *)
                    RwMalloc(sizeof(RwUInt32) * newStrip->stripSize,
                             rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                while (revBuildStrip->stripLen > 2)
                {
                    newStrip->strip[newStrip->stripLen] =
                        revBuildStrip->strip[revBuildStrip->stripLen -
                                             1];
                    newStrip->stripLen++;
                    revBuildStrip->stripLen--;
                }
                memcpy(&newStrip->strip[newStrip->stripLen],
                       buildStrip->strip,
                       sizeof(RwUInt32) * buildStrip->stripLen);
                newStrip->stripLen = newStrip->stripSize;
            }
            else
            {
                /* Finally copy it and add to the list */
                newStrip = (TriStripListEntry  *)
                    RwFreeListAlloc(RPMESHGLOBAL
                                    (triStripListEntryFreeList),
                                    rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                newStrip->next = stripList->head;
                stripList->head = newStrip;

                newStrip->stripSize = buildStrip->stripLen;
                newStrip->stripLen = buildStrip->stripLen;
                newStrip->strip = (RwUInt16 *)
                    RwMalloc(sizeof(RwUInt32) * buildStrip->stripLen,
                             rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
                memcpy(newStrip->strip, buildStrip->strip, sizeof
                       (RwUInt32) * buildStrip->stripLen);
            }
        }
    }

    RwFree(revBuildStrip->strip);
    revBuildStrip->strip = (RwUInt16 *)  NULL;
    RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                   revBuildStrip);
    revBuildStrip = (TriStripListEntry *)NULL;

    RwFree(buildStrip->strip);
    buildStrip->strip = (RwUInt16 *)  NULL;
    RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList), buildStrip);
    buildStrip = (TriStripListEntry *)NULL;

    binEntryArray =
        TriStripBinEntryArrayDestroy(numTris, &meshOpFreeLists,
                                     edgelist, binEntryArray);

    RWRETURN(TRUE);
}


#ifdef JOINSTRIPS
/****************************************************************************
 TriStripJoin

 On entry:
 On exit:
 */
static RwBool
TriStripJoin(TriStripList *stripList, RwBool maintainWinding)
{
    RwUInt32            i, j;
    TriStripListEntry  *newStrip = (TriStripListEntry *)NULL;
    TriStripListEntry  *stripPtr;
    TriStripListEntry  *next;

    RWFUNCTION(RWSTRING("TriStripJoin"));
    RWASSERT(stripList);

    if (!stripList->head)
    {
        RWRETURN(FALSE);
    }

    newStrip =  (TriStripListEntry  *)
        RwFreeListAlloc(RPMESHGLOBAL(triStripListEntryFreeList),
                        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    newStrip->stripLen = 0;
    newStrip->stripSize = 0;

    /* calc a max new strip length */
    stripPtr = stripList->head;
    while (stripPtr)
    {
        /* sum the strips + max 6 joining indices */
        newStrip->stripSize += stripPtr->stripLen + 6;
        stripPtr = stripPtr->next;
    }
    newStrip->strip = (RwUInt16 *)  RwMalloc(sizeof(RwUInt32) * newStrip->stripSize,
                                             rwMEMHINTDUR_EVENT | rwID_MESHMODULE);

    stripPtr = stripList->head;
    for (i = 0; i < stripPtr->stripLen; i++)
    {
        newStrip->strip[newStrip->stripLen] = stripPtr->strip[i];
        newStrip->stripLen++;
    }

    RwFree(stripPtr->strip);
    stripPtr->strip = (RwUInt16 *)  NULL;

    next = stripPtr->next;
    RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList), stripPtr);
    stripPtr = (TriStripListEntry *)NULL;
    stripPtr = next;


    /* this algorithm allows us to ignore the winding order
       of triangles in tristrips resulting in less degenerate
       triangles when joining strips. Winding can only be ignored where
       backface culling is disabled. The algorithm also searches for and
       joins strips which start with vertices used at the end of
       the previous strip. */
    while (stripPtr)
    {
        TriStripListEntry  *tempStrip = stripPtr;

        /* can we find a strip starting with the vertex the last one
           ended with ? */
        while(tempStrip)
        {
            if (newStrip->strip[newStrip->stripLen-1] ==
                tempStrip->strip[0])
            {
                if ((!(newStrip->stripLen % 2)) || (!maintainWinding))
                {
                    /* no extra verts in this case */
                    newStrip->stripLen += 0;
                }
                else
                {
                    newStrip->strip[newStrip->stripLen] =
                        tempStrip->strip[0];
                    newStrip->stripLen += 1;
                }
                break;
            }
            tempStrip = tempStrip->next;
        }

        if (!tempStrip)
        {
            /* if we didn't find a strip can we find a strip starting with
               the last but one vertex the last strip ended with ? */
            tempStrip = stripPtr;

            while(tempStrip)
            {
                if (newStrip->strip[newStrip->stripLen-2] ==
                    tempStrip->strip[0])
                {
                    if ((!(newStrip->stripLen % 2)) && maintainWinding)
                    {
                        newStrip->strip[newStrip->stripLen] =
                            tempStrip->strip[0];
                        newStrip->strip[newStrip->stripLen + 1] =
                            tempStrip->strip[0];
                        newStrip->stripLen += 2;
                    }
                    else
                    {
                        newStrip->strip[newStrip->stripLen] =
                            tempStrip->strip[0];
                        newStrip->stripLen += 1;
                    }
                    break;
                }
                tempStrip = tempStrip->next;
            }
        }

        if (!tempStrip)
        {
            /* if we haven't found a strip yet use the next one in the
               list */
            tempStrip = stripPtr;

            if ((!(newStrip->stripLen % 2)) || (!maintainWinding))
            {
                newStrip->strip[newStrip->stripLen] =
                    newStrip->strip[newStrip->stripLen - 1];
                newStrip->strip[newStrip->stripLen + 1] =
                    tempStrip->strip[0];
                newStrip->stripLen += 2;
            }
            else
            {
                newStrip->strip[newStrip->stripLen] =
                    newStrip->strip[newStrip->stripLen - 1];
                newStrip->strip[newStrip->stripLen + 1] =
                    tempStrip->strip[0];
                newStrip->strip[newStrip->stripLen + 2] =
                    tempStrip->strip[0];
                newStrip->stripLen += 3;
            }
        }

        /* copy over the verts in the strip */
        for (j = 0; j < tempStrip->stripLen; j++)
        {
            newStrip->strip[newStrip->stripLen] = tempStrip->strip[j];
            newStrip->stripLen++;
        }
        RwFree(tempStrip->strip);
        tempStrip->strip = (RwUInt16 *)NULL;

        if (stripPtr == tempStrip)
        {
            stripPtr = stripPtr->next;
            RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                           tempStrip);
        }
        else
        {
            TriStripListEntry  *tempStrip2 = stripPtr;

            while (tempStrip2->next != tempStrip)
            {
                tempStrip2 = tempStrip2->next;
            }
            tempStrip2->next = tempStrip->next;
            RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                           tempStrip);
        }

    }

    stripList->head = newStrip;
    newStrip->next = (TriStripListEntry *)NULL;

    RWRETURN(TRUE);
}
#endif /* JOINSTRIPS */


/****************************************************************************
 TriStripMeshGenerate

 On entry:
 On exit:
 */
static RpMeshHeader *
TriStripMeshGenerate(RpBuildMesh *mesh,
                     RwBool preprocess,
                     RwBool maintainWinding)
{
    RpMeshHeader         *result;
    RpBuildMeshTriangle **triPointers;
    RpMesh               *meshEl;
    RwUInt32              i;
    RwUInt32              j;
    RwUInt32              numMats;
    RwUInt32              meshSize;
    RwUInt32              triPointerIndex;
    RpBuildMeshTriangle **tempTriPtr;
    RpBuildMeshTriangle  *triList;
    TriStripList          stripList;
    TriStripListEntry    *stripPtr;
    RwUInt16              numOutMeshes = 0;
    RwUInt16              numAllocatedOutMeshes = 0;
    RxVertexIndex        *stripMeshInds;
    RpMesh              **outMeshes = (RpMesh **)NULL;
    RpMesh               *outMeshInfo = (RpMesh *)NULL;
    RwUInt32              totalIndices;

    RWFUNCTION(RWSTRING("TriStripMeshGenerate"));
    RWASSERT(mesh);

    triPointers = (RpBuildMeshTriangle **)
        RwMalloc(mesh->numTriangles * sizeof(RpBuildMeshTriangle *),
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    if (!triPointers)
    {
        RWRETURN((RpMeshHeader *)NULL);
    }

    /* Fill in pointers so that we can sort */
    for (i = 0; i < mesh->numTriangles; i++)
    {
        triPointers[i] = &(mesh->meshTriangles[i]);
    }

    /* Now sort 'em */
    qsort(triPointers, mesh->numTriangles, sizeof(RpBuildMeshTriangle
                                                  *), SortPolygons);

    /* Figure out how many materials there are */
    numMats = 1;
    if (mesh->numTriangles >= 2)
    {
        RpMaterial         *lastMat = triPointers[0]->material;

        for (i = 1; i < mesh->numTriangles; i++)
        {
            if (triPointers[i]->material != lastMat)
            {
                /* We found another material */
                lastMat = triPointers[i]->material;
                numMats++;
            }
        }
    }

    /* Allocate an outMeshes array */
    outMeshes = (RpMesh **)
        RwMalloc(sizeof(RpMesh *) * numMats,
            rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE |
            rwMEMHINTFLAG_RESIZABLE);
    outMeshInfo = (RpMesh *)
        RwMalloc(sizeof(RpMesh) * numMats,
            rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    numAllocatedOutMeshes = (RwUInt16) numMats;

    /* Fill in the materials and use numIndices
     * to indicate triangle run length */
    numMats = 1;
    outMeshInfo[0].material = triPointers[0]->material;
    outMeshInfo[0].numIndices = 0;
    outMeshInfo[0].indices = (RxVertexIndex *)NULL;
    if (mesh->numTriangles >= 2)
    {
        for (i = 0; i < mesh->numTriangles - 1; i++)
        {
            if (triPointers[i]->material !=
                triPointers[i + 1]->material)
            {
                outMeshInfo[numMats].material =
                    triPointers[i + 1]->material;
                outMeshInfo[numMats].numIndices = (i + 1);
                outMeshInfo[numMats].indices = (RxVertexIndex *)NULL;
                outMeshInfo[numMats - 1].numIndices = (i + 1) -
                    outMeshInfo[numMats - 1].numIndices;
                numMats++;
            }
        }
    }
    outMeshInfo[numMats - 1].numIndices = mesh->numTriangles -
        outMeshInfo[numMats - 1].numIndices;

    /* Now lets strip them */
    RPMESHGLOBAL(triStripListEntryFreeList) =
        RwFreeListCreate(sizeof(TriStripListEntry),
                         (mesh->numTriangles / 10) + 5, sizeof(RwUInt32),
                         rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    triPointerIndex = 0;
    tempTriPtr = triPointers;
    stripList.head = (TriStripListEntry *)NULL;

    for (i = 0; i < numMats; i++)
    {
        /* build a tri list for this mesh */
        triList = (RpBuildMeshTriangle *)
            RwMalloc(sizeof(RpBuildMeshTriangle) *
                     outMeshInfo[i].numIndices,
                     rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
        for (j = 0; j < outMeshInfo[i].numIndices; j++)
        {
            triList[j] = *tempTriPtr[0];
            tempTriPtr++;
        }

        /* Build a set of strips */
        TriStripStripTris(triList, outMeshInfo[i].numIndices,
                          &stripList, preprocess);

#ifdef JOINSTRIPS
        /* Join the strips with degenerate triangles */
        TriStripJoin(&stripList, maintainWinding);
#endif

        stripPtr = stripList.head;
        while (stripPtr)
        {
            RwUInt32            bytes =
                sizeof(RpMesh) +

                (sizeof(RxVertexIndex) * stripPtr->stripLen);

            /* build this into a mesh (this allocated structure is aligned) */
            meshEl = (RpMesh *) RwMalloc(bytes,
                rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
            meshEl->material = outMeshInfo[i].material;
            meshEl->numIndices = stripPtr->stripLen;
            meshEl->indices = (RxVertexIndex *) (meshEl + 1);

            for (j = 0; j < meshEl->numIndices; j++)
            {
                meshEl->indices[j] =
                    (RxVertexIndex) stripPtr->strip[j];
            }
#ifndef JOINSTRIPS
            if (numOutMeshes == numAllocatedOutMeshes)
            {
                outMeshes = RwRealloc(outMeshes,
                                      sizeof(RpMesh *) *
                                      (numOutMeshes * 2),
                                      rwMEMHINTDUR_EVENT |
                                      rwID_MESHMODULE |
                                      rwMEMHINTFLAG_RESIZABLE);
                numAllocatedOutMeshes = numOutMeshes * 2;
            }
#endif /* JOINSTRIPS */
            outMeshes[numOutMeshes] = meshEl;
            numOutMeshes++;

            stripPtr = stripPtr->next;
        }

        while (NULL != stripList.head)
        {
            stripPtr = stripList.head;
            stripList.head = stripPtr->next;

            RwFree(stripPtr->strip);
            stripPtr->strip = (RwUInt16 *)  NULL;

            RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                           stripPtr);
            stripPtr = (TriStripListEntry *)NULL;

        }
        RwFree(triList);
        triList = (RpBuildMeshTriangle *)NULL;
    }

    RwFreeListDestroy(RPMESHGLOBAL(triStripListEntryFreeList));
    RPMESHGLOBAL(triStripListEntryFreeList) = (RwFreeList *)NULL;

    /* now build the whole mesh */
    meshSize = sizeof(RpMeshHeader);
    totalIndices = 0;
    for (i = 0; i < numOutMeshes; i++)
    {
        RwUInt32            indexSize;

        indexSize =
            (sizeof(RxVertexIndex) * outMeshes[i]->numIndices);
        meshSize += sizeof(RpMesh) + indexSize;

        totalIndices += outMeshes[i]->numIndices;
    }

    result = _rpMeshHeaderCreate(meshSize);
    result->flags = rpMESHHEADERTRISTRIP;
    result->numMeshes = numOutMeshes;
    result->serialNum = RPMESHGLOBAL(nextSerialNum);
    result->firstMeshOffset = 0;
    result->totalIndicesInMesh = totalIndices;
    RPMESHGLOBAL(nextSerialNum)++;

    meshEl = (RpMesh *) (result + 1);
    stripMeshInds = (RxVertexIndex *) (meshEl + numOutMeshes);
    for (i = 0; i < numOutMeshes; i++)
    {
        /* Add in the next mesh */
        meshEl->indices = stripMeshInds;
        meshEl->numIndices = outMeshes[i]->numIndices;
        meshEl->material = outMeshes[i]->material;

        /* And the indices */
        meshSize = (sizeof(RxVertexIndex) * outMeshes[i]->numIndices);
        memcpy(stripMeshInds, outMeshes[i]->indices, meshSize);

        /* Skip to next */
        stripMeshInds += meshEl->numIndices;
        meshEl++;

        /* Don't need this any more */
        RwFree(outMeshes[i]);
        outMeshes[i] = (RpMesh *)NULL;
    }

#if 0
    RWMESSAGE(("%d meshes, %d indices", numOutMeshes, totalIndices));
#endif

    RwFree(triPointers);
    triPointers = (RpBuildMeshTriangle **)NULL;

    RwFree(outMeshes);
    outMeshes = (RpMesh **)NULL;

    RwFree(outMeshInfo);
    outMeshInfo = (RpMesh *)NULL;

    RWRETURN(result);
}


/****************************************************************************
 _rpTriStripPolygonSharedVertex4

 On entry:
 On exit:   TRUE if a vertex is ahared by all four polygons
 */
static RwBool
_rpTriStripPolygonSharedVertex4(RpTriStripPolygon *poly1,
                                RpTriStripPolygon *poly2,
                                RpTriStripPolygon *poly3,
                                RpTriStripPolygon *poly4)
{
    RwUInt32 i, j, k, l;

    RWFUNCTION( RWSTRING( "_rpTriStripPolygonSharedVertex4" ) );

    for (i = 0; i < poly1->numEdges; i++)
    {
        for (j = 0; j < poly2->numEdges; j++)
        {
            if (poly1->vertIndex[i] == poly2->vertIndex[j])
            {
                break;
            }
        }

        if (j < poly2->numEdges)
        {
            for (k = 0; k < poly3->numEdges; k++)
            {
                if (poly1->vertIndex[i] == poly3->vertIndex[k])
                {
                    break;
                }
            }

            if (k < poly3->numEdges)
            {
                for (l = 0; l < poly4->numEdges; l++)
                {
                    if (poly1->vertIndex[i] == poly4->vertIndex[l])
                    {
                        break;
                    }
                }

                if (l < poly3->numEdges)
                {
                    RWRETURN( TRUE );
                }
            }
        }
    }

    RWRETURN( FALSE );
}


/****************************************************************************
 _rpTriStripPolygonSharedVertex3

 On entry:
 On exit:  The index of a vertex shared by all three polygons
 */
static RxVertexIndex
_rpTriStripPolygonSharedVertex3(RpTriStripPolygon *polygon1,
                                RpTriStripPolygon *polygon2,
                                RpTriStripPolygon *polygon3)
{
    RwUInt32 i, j, k;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonSharedVertex3"));
    RWASSERT(polygon1 && polygon2 && polygon3);

    for (i = 0; i < polygon1->numEdges; i++)
    {
        for (j = 0; j < polygon2->numEdges; j++)
        {
            if (polygon1->vertIndex[i] == polygon2->vertIndex[j])
            {
                for (k = 0; k < polygon3->numEdges; k++)
                {
                    if (polygon1->vertIndex[i] == polygon3->vertIndex[k])
                    {
                        goto found;
                    }
                }
            }
        }
    }

found:
    RWASSERT(i < polygon1->numEdges);
    RWRETURN(polygon1->vertIndex[i]);
}


/****************************************************************************
 _rpTriStripPolygonSharedVertex2

 On entry:
 On exit:  The index of a vertex shared by both polygons
 */
static RxVertexIndex
_rpTriStripPolygonSharedVertex2(RpTriStripPolygon *polygon1,
                                RpTriStripPolygon *polygon2)
{
    RwUInt32 i, j;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonSharedVertex2"));
    RWASSERT(polygon1 && polygon2);

    for (i = 0; i < polygon1->numEdges; i++)
    {
        for (j = 0; j < polygon2->numEdges; j++)
        {
            if (polygon1->vertIndex[i] == polygon2->vertIndex[j])
            {
                goto found;
            }
        }
    }

found:
    RWASSERT(i < polygon1->numEdges);
    RWRETURN(polygon1->vertIndex[i]);
}


/****************************************************************************
 _rpTriStripPolygonOtherSharedVertex2

 On entry:
 On exit:  The index of another vertex shared by both polygons
 */
static RxVertexIndex
_rpTriStripPolygonOtherSharedVertex2(RpTriStripPolygon *polygon1,
                                RpTriStripPolygon *polygon2,
                                RxVertexIndex index)
{
    RwUInt32 i, j;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonOtherSharedVertex2"));
    RWASSERT(polygon1 && polygon2);

    for (i = 0; i < polygon1->numEdges; i++)
    {
        if (index != polygon1->vertIndex[i])
        {
            for (j = 0; j < polygon2->numEdges; j++)
            {
                if (polygon1->vertIndex[i] == polygon2->vertIndex[j])
                {
                    goto found;
                }
            }
        }
    }

found:
    RWASSERT(i < polygon1->numEdges);
    RWRETURN(polygon1->vertIndex[i]);
}


/****************************************************************************
 _rpTriStripPolygonOtherVertex

 On entry:
 On exit:  The index of another vertex of the polygon
 */
static RxVertexIndex
_rpTriStripPolygonOtherVertex(RpTriStripPolygon *polygon,
                              RxVertexIndex index1,
                              RxVertexIndex index2)
{
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonOtherVertex"));
    RWASSERT(polygon);

    for (i = 0; i < polygon->numEdges; i++)
    {
        if (index1 != polygon->vertIndex[i]
            && index2 != polygon->vertIndex[i])
        {
            break;
        }
    }

    RWASSERT(i < polygon->numEdges);
    RWRETURN(polygon->vertIndex[i]);
}


/****************************************************************************
 _rpTriStripTunnelEdges

 On entry:
 On exit:  The edges of the tunnel are complemented.
 */
static void
_rpTriStripTunnelEdges(RpTriStripTunnel *tunnel)
{
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripTunnelEdges"));

    if (tunnel->length == 6)
    {
        i = 0;
    }

    for (i = 0; i < tunnel->length; i++)
    {
        tunnel->edges[i]->strip = !tunnel->edges[i]->strip;
    }

    RWRETURNVOID();
}


/****************************************************************************
 _rpTriStripPolygonNumStripEdges

 On entry:
 On exit:  The number of strip edges the polygon has.
 */
static RwUInt32
_rpTriStripPolygonNumStripEdges(RpTriStripPolygon *polygon)
{
    RwUInt32 numStripEdges;
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonNumStripEdges"));

    numStripEdges = 0;
    for (i = 0; i < polygon->numEdges; i++)
    {
        if (polygon->edges[i]->strip)
        {
            numStripEdges++;
        }
    }

    RWASSERT(numStripEdges < 3);
    RWRETURN(numStripEdges);
}


/****************************************************************************
 _rpTriStripMeshComplementTunnel

 On entry:
 On exit:  The edges of the tunnel are complemented and the strip end polygon
           list is updated.
 */
static void
_rpTriStripMeshComplementTunnel(RpTriStripMesh *mesh,
                                RpTriStripTunnel *tunnel)
{
    RpTriStripPolygon *polygon;
    RwUInt32 numStripEdges;
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripMeshComplementTunnel"));

    _rpTriStripTunnelEdges(tunnel);

    for (i = 0; i <= tunnel->length; i++)
    {
        polygon = tunnel->polygons[i];
        numStripEdges = _rpTriStripPolygonNumStripEdges(polygon);

        if (rwLLLinkAttached(&polygon->inEndLink))
        {
            if (numStripEdges > 1)
            {
                rwLinkListRemoveLLLink(&polygon->inEndLink);
                rwLLLinkInitialize(&polygon->inEndLink);
                RWASSERT(rwLinkListGetTerminator(&mesh->polygonEndList)
                    != rwLinkListGetFirstLLLink(&mesh->polygonEndList));
            }
        }
        else
        {
            if (numStripEdges < 2)
            {
                rwLinkListAddLLLink(&mesh->polygonEndList,
                    &polygon->inEndLink);
            }
        }
    }

    RWRETURNVOID();
}


/****************************************************************************
 _rpTriStripTunnelCheckStrips

 On entry:
 On exit:  TRUE if the tunnel edges can be complemented without generating
           any loops.
 */
static RwBool
_rpTriStripTunnelCheckStrips(RpTriStripTunnel *tunnel)
{
    RpTriStripPolygon *polygon, *curr, *next, *temp;
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripTunnelCheckStrips"));

    /* check for loops */
    for (i = 0; i < tunnel->length; i += 2)
    {
        polygon = tunnel->polygons[i];
        curr = tunnel->polygons[i+1];
        next = RpTriStripPolygonFollowStrip(curr, polygon);

        while (NULL != next && polygon != next)
        {
            temp = RpTriStripPolygonFollowStrip(next, curr);
            curr = next;
            next = temp;
        }

        if (NULL != next)
        {
            RWRETURN(FALSE);
        }
    }

    RWRETURN(TRUE);
}


/****************************************************************************
 _rpTriStripTunnelCost

 On entry:
 On exit:  The cost of all the tristrips the tunnel affects.
 */
static RwUInt32
_rpTriStripTunnelCost(RpTriStripTunnel *tunnel,
                      RpTriStripTunnelData *tunnelData)
{
    RwUInt32 i;
    RwUInt32 cost;
    RpTriStripPolygon *polygon, *next, *next2;
    static RwUInt32 testFrame = 1;

    RWFUNCTION(RWSTRING("_rpTriStripTunnelCost"));

    cost = 0;
    for (i = 0; i <= tunnel->length; i++)
    {
        /* find start of strip */
        polygon = tunnel->polygons[i];

        if (testFrame != polygon->testFrame)
        {
            next = RpTriStripPolygonFollowStrip(polygon, NULL);
            while (NULL != next)
            {
                next2 = RpTriStripPolygonFollowStrip(next, polygon);
                polygon = next;
                next = next2;
            }
            cost += tunnelData->costCB(polygon, testFrame, tunnelData->data);
        }
    }
    testFrame++;

    RWRETURN(cost);
}


/****************************************************************************
 _rpTriStripTunnelValid

 On entry:
 On exit:  TRUE if the tunnel is valid and improves the stripification of the
           mesh.
 */
static RwBool
_rpTriStripTunnelValid(RpTriStripTunnel *tunnel,
                       RpTriStripTunnelData *tunnelData)
{
    RwBool result;

    RWFUNCTION(RWSTRING("_rpTriStripTunnelValid"));
    RWASSERT(tunnel);
    RWASSERT(tunnelData);

    if (tunnel->length & 1
        && !rwLLLinkAttached(&tunnel->polygons[tunnel->length]->inEndLink))
    {
        RWRETURN(FALSE);
    }

    _rpTriStripTunnelEdges(tunnel);
    result = _rpTriStripTunnelCheckStrips(tunnel);

    if (!result || tunnel->length < 2)
    {
        _rpTriStripTunnelEdges(tunnel);
    }
    else
    {
        RwUInt32 oldCost, newCost;

        newCost = _rpTriStripTunnelCost(tunnel, tunnelData);
        _rpTriStripTunnelEdges(tunnel);
        oldCost = _rpTriStripTunnelCost(tunnel, tunnelData);

        result = oldCost > newCost;
    }

    RWRETURN(result);
}


/****************************************************************************
 _rpTriStripTunnelFind

 On entry:
 On exit:  Finds the shortest valid tunnel of length less than
           tunnel->maxLength
 */
static RwBool
_rpTriStripTunnelFind(RpTriStripTunnel *tunnel,
                      RpTriStripTunnelData *tunnelData,
                      _rpTriStripTunnelStack *stack)
{
    _rpTriStripTunnelStack *top;
    RpTriStripPolygon *polygon;
    RpTriStripEdge *edge;
    RwBool strip;
    RwUInt32 i, j;

    RWFUNCTION(RWSTRING("_rpTriStripTunnelFind"));
    RWASSERT(tunnel);
    RWASSERT(tunnelData);
    RWASSERT(stack);

    /* put initial edges on stack */
    top = stack;
    for (i = 0; i < tunnel->polygons[0]->numEdges; i++)
    {
        edge = tunnel->polygons[0]->edges[i];

        if (!edge->strip)
        {
            polygon = tunnel->polygons[0] == edge->poly1
                ? edge->poly2 : edge->poly1;

            if (NULL != polygon)
            {
                top->edge = edge;
                top->polygon = polygon;
                top->length = 1;
                top++;
            }
        }
    }

    while (stack < top)
    {
        /* get next edge to test from stack */
        top--;
        RWASSERT(top->polygon);
        RWASSERT(top->edge);
        tunnel->length = top->length;
        tunnel->edges[tunnel->length - 1] = top->edge;
        tunnel->polygons[tunnel->length] = top->polygon;
        strip = tunnel->length & 1;

        /* test for complete valid tunnel */
        if (_rpTriStripTunnelValid(tunnel, tunnelData))
        {
            RWRETURN(TRUE);
        }

        /* add possible continuations to stack */
        if (tunnel->length < tunnel->maxLength)
        {
            polygon = tunnel->polygons[tunnel->length];

            for (i = 0; i < polygon->numEdges; i++)
            {
                edge = polygon->edges[i];

                if (strip == edge->strip
                    && edge != tunnel->edges[tunnel->length - 1])
                {
                    top->polygon = polygon == edge->poly1
                        ? edge->poly2 : edge->poly1;
                    if (NULL != top->polygon)
                    {

                        for (j = 0; j <= tunnel->length; j++)
                        {
                            if (top->polygon == tunnel->polygons[j])
                            {
                                break;
                            }
                        }

                        if (j > tunnel->length)
                        {
                            top->length = tunnel->length + 1;
                            top->edge = edge;
                            top++;
                        }
                    }
                }
            }
        }
    }

    RWRETURN(FALSE);
}


/****************************************************************************
 _rpTriStripPolygonWindingCorrect

 On entry:
 On exit:
 */
static RwBool
_rpTriStripPolygonWindingCorrect(RpTriStripPolygon *polygon,
                                 RxVertexIndex index1,
                                 RxVertexIndex index2,
                                 RwUInt32 winding)
{
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonWindingCorrect"));
    RWASSERT(polygon);

    for (i = 0; i < polygon->numEdges; i++)
    {
        if (index1 == polygon->vertIndex[i])
        {
            break;
        }
    }
    RWASSERT(i < polygon->numEdges);

    if (index2 == polygon->vertIndex[(i + 1) % polygon->numEdges])
    {
        RWRETURN(1 == winding);
    }
    else
    {
        RWRETURN(2 == winding);
    }
}


/****************************************************************************
 _rpTriStripPolygonStripIndex

 On entry:
 On exit:  TRUE if the polygon uses index2 (i.e. the tristrip can be continued
           without fan vertices). index points to the next vertex index.
 */
static RwBool
_rpTriStripPolygonStripIndex(RpTriStripPolygon *polygon,
                             RxVertexIndex index1,
                             RxVertexIndex index2,
                             RxVertexIndex index3,
                             RxVertexIndex *index)
{
    RwUInt32 i;
    RwBool tristrip = FALSE;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonStripIndex"));
    RWASSERT(polygon);
    RWASSERT(3 == polygon->numEdges);
    RWASSERT(index);
    RWASSERT(index1 != index2 && index2 != index3 && index3 != index1);
#ifdef RWDEBUG
    {
        RwUInt32 numShared;

        numShared = 0;
        for (i = 0; i < polygon->numEdges; i++)
        {
            if (index1 == polygon->vertIndex[i]
                || index2 == polygon->vertIndex[i]
                || index3 == polygon->vertIndex[i])
            {
                numShared++;
            }
        }
        RWASSERT(2 <= numShared);
    }
#endif

    *index = index1;
    for (i = 0; i < polygon->numEdges; i++)
    {
        if (index1 != polygon->vertIndex[i]
            && index2 != polygon->vertIndex[i]
            && index3 != polygon->vertIndex[i])
        {
            *index = polygon->vertIndex[i];
        }
        else if (index2 == polygon->vertIndex[i])
        {
            tristrip = TRUE;
        }
    }

    RWRETURN(tristrip);
}


/****************************************************************************
 _rpTriStripPolygonSharedVertices

 On entry:
 On exit:
 */
static RwUInt32
_rpTriStripPolygonSharedVertices(RpTriStripPolygon *polygon1,
                                 RpTriStripPolygon *polygon2)
{
    RwUInt32 numShared;
    RwUInt32 i, j;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonSharedVertices"));
    RWASSERT(polygon1);
    RWASSERT(polygon2);

    numShared = 0;
    for (i = 0; i < polygon1->numEdges; i++)
    {
        for (j = 0; j < polygon2->numEdges; j++)
        {
            if (polygon1->vertIndex[i] == polygon2->vertIndex[j])
            {
                numShared++;
                break;
            }
        }
    }

    RWRETURN(numShared);
}


/****************************************************************************
 _rpTriStripPolygonAdjCount

 On entry:
 On exit:
 */
static RwUInt32
_rpTriStripPolygonAdjCount(RpTriStripPolygon *polygon)
{
    RwUInt32 i, adj;
    RpTriStripEdge *edge;

    RWFUNCTION(RWSTRING("_rpTriStripPolygonAdjCount"));

    adj = 0;
    for (i = 0; i < polygon->numEdges; i++)
    {
        edge = polygon->edges[i];

        if ((NULL != edge->poly1 && polygon != edge->poly1
            && rwLLLinkAttached(&edge->poly1->inFreeLink))
            || (NULL != edge->poly2 && polygon != edge->poly2
            && rwLLLinkAttached(&edge->poly2->inFreeLink)))
        {
            adj++;
        }
    }

    RWRETURN(adj);
}


/****************************************************************************
 _rpTriStripEdgeFindNext

 On entry:
 On exit:
 */
static RpTriStripEdge *
_rpTriStripEdgeFindNext(RpTriStripEdge *edge, RpTriStripPolygon *polygon,
                        RxVertexIndex otherIndex, RwUInt32 testFrame)
{
    /* initialized to prevent incorrect compiler warning */
    RpTriStripEdge *testEdge = NULL;
    RpTriStripPolygon *testPolygon;
    RwUInt32 i;

    RWFUNCTION(RWSTRING("_rpTriStripEdgeFindNext"));

    RWASSERT(3 == polygon->numEdges);

    for (i = 0; i < 3; i++)
    {
        testEdge = polygon->edges[i];

        if (edge != testEdge
            && otherIndex != testEdge->vert1
            && otherIndex != testEdge->vert2)
        {
            testPolygon = polygon == testEdge->poly1
                ? testEdge->poly2 : testEdge->poly1;

            if (NULL != testPolygon && testFrame != testPolygon->testFrame
                && rwLLLinkAttached(&testPolygon->inFreeLink))
            {
                break;
            }
        }
    }

    if (3 == i)
    {
        for (i = 0; i < 3; i++)
        {
            testEdge = polygon->edges[i];

            if (edge != testEdge)
            {
                testPolygon = polygon == testEdge->poly1
                    ? testEdge->poly2 : testEdge->poly1;

                if (NULL != testPolygon && testFrame != testPolygon->testFrame
                    && rwLLLinkAttached(&testPolygon->inFreeLink))
                {
                    break;
                }
            }
        }
    }

    if (3 == i)
    {
        RWRETURN(NULL);
    }
    else
    {
        RWRETURN(testEdge);
    }
}


/****************************************************************************
 _rpTriStripMeshCreate

 On entry:
 On exit:
 */
static RpTriStripMesh *
_rpTriStripMeshCreate(RpBuildMeshTriangle **triangles, RwUInt32 numTriangles)
{
    RwUInt32 i, j;
    RpTriStripMesh *mesh;
    RpTriStripEdge *edge, *testEdge;
    RpTriStripPolygon *poly;
    _rpTriStripVertex *vertices;
    _rpTriStripEdge *edges, *nextEdge;
    RwUInt32 numVertices;

    RWFUNCTION(RWSTRING("_rpTriStripMeshCreate"));
    RWASSERT(triangles);
    RWASSERT(numTriangles);

    numVertices = 0;
    for (i = 0; i < numTriangles; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (triangles[i]->vertIndex[j] > numVertices)
            {
                numVertices = triangles[i]->vertIndex[j];
            }
        }
    }

    vertices = RwMalloc(numVertices * sizeof(_rpTriStripVertex),
        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    for (i = 0; i < numVertices; i++)
    {
        rwLinkListInitialize(&vertices[i].triangleList);
    }
    nextEdge = edges = RwMalloc(3 * numTriangles * sizeof(_rpTriStripEdge),
                                rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    mesh = RwMalloc(sizeof(RpTriStripMesh)
        + numTriangles * (sizeof(RpTriStripPolygon)
        + 3 * sizeof(RpTriStripEdge)),
        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);

    mesh->numPolygons = numTriangles;
    rwLinkListInitialize(&mesh->polygonEndList);
    rwLinkListInitialize(&mesh->polygonFreeList);
    rwLinkListInitialize(&mesh->polygonUsedList);
    rwLinkListInitialize(&mesh->edgeFreeList);
    rwLinkListInitialize(&mesh->edgeUsedList);

    poly = (RpTriStripPolygon *)(mesh + 1);
    edge = (RpTriStripEdge *)(poly + numTriangles);

    for (i = 0; i < numTriangles; i++)
    {
        /* setup polygon */
        poly->vertIndex[0] = triangles[i]->vertIndex[0];
        poly->vertIndex[1] = triangles[i]->vertIndex[1];
        poly->vertIndex[2] = triangles[i]->vertIndex[2];

        /* triangles should not be degenerate */
        RWASSERT(poly->vertIndex[0] != poly->vertIndex[1]
            && poly->vertIndex[1] != poly->vertIndex[2]
            && poly->vertIndex[2] != poly->vertIndex[0]);

        rwLinkListAddLLLink(&mesh->polygonUsedList, &poly->inUsedLink);
        rwLinkListAddLLLink(&mesh->polygonEndList, &poly->inEndLink);
        poly->numEdges = 3;
        poly->testFrame = 0;

        /* find or create edges */
        for (j = 0; j < 3; j++)
        {
            RxVertexIndex index1, index2;
            _rpTriStripVertex *vertex;
            RwLLLink *curr, *end;

            index1 = poly->vertIndex[j];
            index2 = poly->vertIndex[(j + 1) % 3];
            if (index1 > index2)
            {
                RxVertexIndex temp;

                temp = index1;
                index1 = index2;
                index2 = temp;
            }
            vertex = vertices + index1;

            curr = rwLinkListGetFirstLLLink(&vertex->triangleList);
            end = rwLinkListGetTerminator(&vertex->triangleList);

            while (curr != end)
            {
                testEdge = rwLLLinkGetData(curr, _rpTriStripEdge, inVertexLink)->edge;

                if (index2 == testEdge->vert2
                    && NULL == testEdge->poly2
                    && 2 == _rpTriStripPolygonSharedVertices(poly, testEdge->poly1))
                {
                    break;
                }

                curr = rwLLLinkGetNext(curr);
            }

            if (curr != end)
            {
                poly->edges[j] = testEdge;
                testEdge->poly2 = poly;
            }
            else
            {
                rwLinkListAddLLLink(&mesh->edgeUsedList, &edge->inUsedLink);
                poly->edges[j] = edge;
                edge->poly1 = poly;
                edge->poly2 = NULL;
                edge->vert1 = index1;
                edge->vert2 = index2;
                edge->strip = FALSE;
                nextEdge->edge = edge;
                rwLinkListAddLLLink(&vertex->triangleList, &nextEdge->inVertexLink);
                nextEdge++;
                edge++;
            }

        }

        poly++;
    }

    RwFree(edges);
    RwFree(vertices);

    RWRETURN(mesh);
}


/****************************************************************************
 _rpTriStripGetStrip

 On entry:
 On exit:
 */
static void
_rpTriStripGetStrip(RwLLLink *link,
                    RpTriStripPolygon **polygon1,
                    RpTriStripPolygon **polygon2,
                    RpTriStripPolygon **polygon3,
                    RxVertexIndex *index1,
                    RxVertexIndex *index2,
                    RxVertexIndex *index3)
{
    RWFUNCTION(RWSTRING("_rpTriStripGetStrip"));

    *polygon1 = rwLLLinkGetData(link, RpTriStripPolygon, inEndLink);
    RWASSERT(3 == (*polygon1)->numEdges);

    *polygon2 = RpTriStripPolygonFollowStrip(*polygon1, NULL);
    if (NULL == *polygon2)
    {
        *index1 = (*polygon1)->vertIndex[0];
        *index2 = (*polygon1)->vertIndex[1];
        *index3 = (*polygon1)->vertIndex[2];
        *polygon3 = NULL;
    }
    else
    {
        RWASSERT(3 == (*polygon2)->numEdges);
        *polygon3 = RpTriStripPolygonFollowStrip(*polygon2, *polygon1);
        if (NULL == *polygon3)
        {
            *index3 = _rpTriStripPolygonSharedVertex2(*polygon1, *polygon2);
        }
        else
        {
            RWASSERT(3 == (*polygon3)->numEdges);
            *index3 = _rpTriStripPolygonSharedVertex3(*polygon1, *polygon2,
                *polygon3);
        }
        *index2 = _rpTriStripPolygonOtherSharedVertex2(*polygon1, *polygon2,
            *index3);
        *index1 = _rpTriStripPolygonOtherVertex(*polygon1, *index2, *index3);
    }

    RWRETURNVOID();
}


/****************************************************************************
 _rpTriStripMeshCreateOutput

 On entry:
 On exit:
 */
static void
_rpTriStripMeshCreateOutput(RpTriStripMesh *inMesh, RpMesh *outMesh,
                            RpTriStripData *data)
{
    RwLLLink *curr, *end;
    RpTriStripPolygon *polygon1, *polygon2, *polygon3;
    RwUInt32 winding; /* 0 ignoring, 1 anticlockwise, 2 clockwise */
    RxVertexIndex *indices;
    RxVertexIndex index1, index2, index3;
    RxVertexIndex index, testIndex;

    RWFUNCTION(RWSTRING("_rpTriStripMeshCreateOutput"));
    RWASSERT(inMesh);
    RWASSERT(outMesh);
    RWASSERT(data);

    winding = data->ignoreWinding ? 0 : 1;
    indices = outMesh->indices;
    curr = end = rwLinkListGetTerminator(&inMesh->polygonEndList);

    do
    {
        if (curr == end)
        {
            curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);
            while (curr != end)
            {
                _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

                if (!winding || _rpTriStripPolygonWindingCorrect(polygon1, index1, index2, winding))
                {
                    if (winding)
                    {
                        winding ^= 3;
                    }
                    if (indices != outMesh->indices)
                    {
                        indices[0] = indices[-1];
                        indices[1] = index1;
                        indices += 2;
                    }
                    break;
                }

                curr = rwLLLinkGetNext(curr);
            }
        }

        if (curr == end)
        {
            curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);

            if (curr == end)
            {
                break;
            }

            _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

            if (indices != outMesh->indices)
            {
                indices[0] = indices[-1];
                indices[1] = index1;
                indices += 2;
            }
            *indices++ = index1;
        }

        *indices++ = index1;
        *indices++ = index2;
        *indices++ = index3;
        RWASSERT(indices[-3] != indices[-2]
            && indices[-2] != indices[-1]
            && indices[-1] != indices[-3]);
        RWASSERT(!winding || _rpTriStripPolygonWindingCorrect(
            polygon1, indices[-3], indices[-2], winding ^ 3));

        while (NULL != polygon2)
        {
            polygon1 = polygon2;
            polygon2 = polygon3;
            polygon3 = NULL != polygon2
                ? RpTriStripPolygonFollowStrip(polygon2, polygon1) : NULL;

            if (rwLLLinkAttached(&polygon1->inEndLink))
            {
                rwLinkListRemoveLLLink(&polygon1->inEndLink);
                rwLLLinkInitialize(&polygon1->inEndLink);
            }

            /* test whether fanning is required */
            if (_rpTriStripPolygonStripIndex(polygon1, indices[-3],
                indices[-2], indices[-1], &index))
            {
                if (winding)
                {
                    if (_rpTriStripPolygonWindingCorrect(polygon1, indices[-2],
                        indices[-1], winding))
                    {
                        winding ^= 3;
                    }
                    else
                    {
                        indices[0] = indices[-2];
                        indices[1] = indices[-2];
                        indices[2] = indices[-1];
                        indices += 3;
                    }
                }
                *indices++ = index;
            }
            /* look at next index to determine which way to fan */
            else if (NULL == polygon2 || _rpTriStripPolygonStripIndex(
                polygon2, indices[-3], indices[-1], index, &testIndex))
            {
                indices[0] = indices[-1];
                indices[-1] = indices[-3];
                indices++;
                if (winding)
                {
                    if (!_rpTriStripPolygonWindingCorrect(polygon1,
                        indices[-2], indices[-1], winding ^ 3))
                    {
                        indices[1] = indices[-2];
                        indices[2] = indices[-1];
                        indices[0] = indices[-1];
                        indices[-2] = indices[-1];
                        indices += 3;
                        winding ^= 3;
                    }
                }
                *indices++ = index;
            }
            /* look at next index to determine which way to fan */
            else if (NULL == polygon3 || _rpTriStripPolygonStripIndex(
                polygon3, indices[-3], index, testIndex, &testIndex))
            {
                index1 = indices[-3];
                indices[0] = indices[-1];
                indices++;
                if (winding)
                {
                    if (_rpTriStripPolygonWindingCorrect(polygon1, indices[-1],
                        index1, winding))
                    {
                        winding ^= 3;
                    }
                    else
                    {
                        indices[0] = indices[-1];
                        indices++;
                    }
                }
                *indices++ = index1;
                *indices++ = index;
            }
            else
            {
                index1 = indices[-3];
                indices[0] = indices[-1];
                indices++;
                if (winding)
                {
                    if (_rpTriStripPolygonWindingCorrect(polygon1, indices[-1],
                        index, winding))
                    {
                        winding ^= 3;
                    }
                    else
                    {
                        indices[0] = indices[-1];
                        indices++;
                    }
                }
                *indices++ = index;
                *indices++ = index1;
            }

            RWASSERT(indices[-3] != indices[-2]
                && indices[-2] != indices[-1]
                && indices[-1] != indices[-3]);
            RWASSERT(!winding || _rpTriStripPolygonWindingCorrect(
                polygon1, indices[-3], indices[-2], winding ^ 3));
        }

        rwLinkListRemoveLLLink(curr);
        rwLLLinkInitialize(curr);

        curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);
        while (curr != end)
        {
            _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

            if (index1 == indices[-1])
            {
                if (!winding || _rpTriStripPolygonWindingCorrect(polygon1, index1, index2, winding))
                {
                    if (winding)
                    {
                        winding ^= 3;
                    }
                    break;
                }
            }

            curr = rwLLLinkGetNext(curr);
        }

        if (winding && curr == end)
        {
            curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);
            while (curr != end)
            {
                _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

                if (index1 == indices[-1])
                {
                    *indices++ = index1;
                    break;
                }

                curr = rwLLLinkGetNext(curr);
            }
        }

        if (curr == end)
        {
            curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);
            while (curr != end)
            {
                _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

                if (index1 == indices[-2])
                {
                    if (!winding || _rpTriStripPolygonWindingCorrect(polygon1, index1, index2, winding ^ 3))
                    {
                        *indices++ = index1;
                        break;
                    }
                }
                if (index2 == indices[-1])
                {
                    if (!winding || _rpTriStripPolygonWindingCorrect(polygon1, index1, index2, winding ^ 3))
                    {
                        *indices++ = index2;
                        break;
                    }
                }

                curr = rwLLLinkGetNext(curr);
            }
        }

        if (winding && curr == end)
        {
            curr = rwLinkListGetFirstLLLink(&inMesh->polygonEndList);
            while (curr != end)
            {
                _rpTriStripGetStrip(curr, &polygon1, &polygon2, &polygon3, &index1, &index2, &index3);

                if (index1 == indices[-2])
                {
                    winding ^= 3;
                    *indices++ = index1;
                    *indices++ = index1;
                    break;
                }

                if (index2 == indices[-1])
                {
                    winding ^= 3;
                    *indices++ = index2;
                    *indices++ = index2;
                    break;
                }

                curr = rwLLLinkGetNext(curr);
            }
        }
    } while (1);

    outMesh->numIndices = indices - outMesh->indices;
    RWASSERT(outMesh->numIndices);

    RWRETURNVOID();
}


/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateTrivialTriStrip generates singleton triangle
 * strips.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateTrivialTriStrip(RpBuildMesh *buildMesh,
                                   void * data __RWUNUSED__)
{
    RpMeshHeader        *result = (RpMeshHeader *) NULL;
    RpBuildMeshTriangle *meshTriangles = (RpBuildMeshTriangle *) NULL;
    RwUInt32            numIndices;
    RwUInt32            numTriangles;
    RwInt32             numStrips;
    RwUInt32            meshSize;
    RxVertexIndex      *FillStripInds = (RxVertexIndex *) NULL;
    RpMesh             *FillStripEl = (RpMesh *) NULL;

    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateTrivialTriStrip"));
    RWASSERT(buildMesh);

    /* Read data */
    meshTriangles = buildMesh->meshTriangles;
    numTriangles = buildMesh->numTriangles;
    numStrips = numTriangles;

    /* Build the composite mesh */
    numIndices = numTriangles * 3;

    meshSize = ((sizeof(RpMeshHeader)) +
                (sizeof(RpMesh) * numStrips) +
                (sizeof(RxVertexIndex) * numIndices));

    result = _rpMeshHeaderCreate(meshSize);
    memset(result, rwFREELISTCLEANLANDFILL, meshSize);

    result->flags = rpMESHHEADERTRISTRIP;
    result->numMeshes = (RwUInt16)numStrips;
    result->serialNum = 0;
    result->totalIndicesInMesh = numIndices;
    result->firstMeshOffset = 0;

    FillStripEl = (RpMesh *) & result[1];
    FillStripInds = (RxVertexIndex *) & FillStripEl[numStrips];

    while (--numStrips >= 0)
    {
        /* Fill in the current output mesh ... */
        FillStripEl->indices = FillStripInds;
        FillStripEl->numIndices = 3;
        FillStripEl->material = meshTriangles->material;

        /* ... and its indices */
        *FillStripInds++ = (meshTriangles->vertIndex[0]);
        *FillStripInds++ = (meshTriangles->vertIndex[1]);
        *FillStripInds++ = (meshTriangles->vertIndex[2]);

        /* Move to next mesh */
        meshTriangles++;
        FillStripEl++;
    }

    RWRETURN (result);
}


/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateDefaultTriStrip generates a triangle strip by
 * selecting a starting triangle with the lowest adjacency and builds the
 * strip off it. This is repeated until the mesh is stripped.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateDefaultTriStrip(RpBuildMesh *buildMesh,
                                   void * data __RWUNUSED__)
{
    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateDefaultTriStrip"));
    RWASSERT(buildMesh);

    RWRETURN(TriStripMeshGenerate(buildMesh, FALSE, TRUE));
}

/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateDefaultIgnoreWindingTriStrip generates a triangle
 * strip by selecting a starting triangle with the lowest adjacency and builds
 * the strip off it. This is repeated until the mesh is stripped.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateDefaultIgnoreWindingTriStrip(RpBuildMesh *buildMesh,
                                   void * data __RWUNUSED__)
{
    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateDefaultIgnoreWindingTriStrip"));
    RWASSERT(buildMesh);

    RWRETURN(TriStripMeshGenerate(buildMesh, FALSE, FALSE));
}

/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGeneratePreprocessTriStrip generates a triangle strip by
 * selecting a starting triangle with the lowest adjacency. It then builds a
 * strip off each side of triangle and chooses the longest (not counting
 * degenerates). This is repeated until the mesh is stripped.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGeneratePreprocessTriStrip(RpBuildMesh *buildMesh,
                                      void * data __RWUNUSED__)
{
    RWAPIFUNCTION(RWSTRING("RpBuildMeshGeneratePreprocessTriStrip"));
    RWASSERT(buildMesh);

    RWRETURN(TriStripMeshGenerate(buildMesh, TRUE, TRUE));
}

/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip generates a triangle
 * strip by selecting a starting triangle with the lowest adjacency. It then
 * builds a strip off each side of triangle and chooses the longest (not
 * counting degenerates). This is repeated until the mesh is stripped.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip(RpBuildMesh *buildMesh,
                                      void * data __RWUNUSED__)
{
    RWAPIFUNCTION(RWSTRING("RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip"));
    RWASSERT(buildMesh);

    RWRETURN(TriStripMeshGenerate(buildMesh, TRUE, FALSE));
}


static RpMeshHeader *
BuildMeshGenerateExhaustiveTriStrip(RpBuildMesh *buildMesh,
                                    void * data __RWUNUSED__,
                                    RwBool maintainWinding)
{
    RpMeshHeader         *result;
    RpBuildMeshTriangle **triPointers;
    RpMesh               *meshEl;
    RwUInt32              i;
    RwUInt32              j;
    RwUInt32              numMats;
    RwUInt32              meshSize;
    RwUInt32              triPointerIndex;
    RpBuildMeshTriangle **tempTriPtr;
    RpBuildMeshTriangle  *triList;
    TriStripList          stripList;
    TriStripListEntry    *stripPtr;
    RwUInt16              numOutMeshes = 0;
    RwUInt16              numAllocatedOutMeshes = 0;
    RxVertexIndex        *stripMeshInds;
    RpMesh              **outMeshes = (RpMesh **)NULL;
    RpMesh               *outMeshInfo = (RpMesh *)NULL;
    RwUInt32              totalIndices;

    RWFUNCTION(RWSTRING("BuildMeshGenerateExhaustiveTriStrip"));
    RWASSERT(buildMesh);

    triPointers = (RpBuildMeshTriangle **)
        RwMalloc(buildMesh->numTriangles * sizeof(RpBuildMeshTriangle *),
                 rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    if (!triPointers)
    {
        RWRETURN((RpMeshHeader *)NULL);
    }

    /* Fill in pointers so that we can sort */
    for (i = 0; i < buildMesh->numTriangles; i++)
    {
        triPointers[i] = &(buildMesh->meshTriangles[i]);
    }

    /* Now sort 'em */
    qsort(triPointers,
          buildMesh->numTriangles,
          sizeof(RpBuildMeshTriangle *),
          SortPolygons);

    /* Figure out how many materials there are */
    numMats = 1;
    if (buildMesh->numTriangles >= 2)
    {
        RpMaterial         *lastMat = triPointers[0]->material;

        for (i = 1; i < buildMesh->numTriangles; i++)
        {
            if (triPointers[i]->material != lastMat)
            {
                /* We found another material */
                lastMat = triPointers[i]->material;
                numMats++;
            }
        }
    }

    /* Allocate an outMeshes array */
    outMeshes = (RpMesh **)  RwMalloc(sizeof(RpMesh *) * numMats,
                    rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE |
                    rwMEMHINTFLAG_RESIZABLE);
    outMeshInfo = (RpMesh *) RwMalloc(sizeof(RpMesh) * numMats,
                              rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    numAllocatedOutMeshes = (RwUInt16) numMats;

    /* Fill in the materials and use numIndices
     * to indicate triangle run length */
    numMats = 1;
    outMeshInfo[0].material = triPointers[0]->material;
    outMeshInfo[0].numIndices = 0;
    outMeshInfo[0].indices = (RxVertexIndex *) NULL;
    if (buildMesh->numTriangles >= 2)
    {
        for (i = 0; i < buildMesh->numTriangles - 1; i++)
        {
            if (triPointers[i]->material !=
                triPointers[i + 1]->material)
            {
                outMeshInfo[numMats].material =
                    triPointers[i + 1]->material;
                outMeshInfo[numMats].numIndices = (i + 1);
                outMeshInfo[numMats].indices = (RxVertexIndex *)NULL;
                outMeshInfo[numMats - 1].numIndices = (i + 1) -
                    outMeshInfo[numMats - 1].numIndices;
                numMats++;
            }
        }
    }
    outMeshInfo[numMats - 1].numIndices = buildMesh->numTriangles -
        outMeshInfo[numMats - 1].numIndices;

    /* Now lets strip them */
    RPMESHGLOBAL(triStripListEntryFreeList) =
        RwFreeListCreate(sizeof(TriStripListEntry),
                         (buildMesh->numTriangles / 10) + 5, sizeof(RwUInt32),
                         rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    triPointerIndex = 0;
    tempTriPtr = triPointers;
    stripList.head = (TriStripListEntry *)NULL;

    for (i = 0; i < numMats; i++)
    {
        /* build a tri list for this mesh */
        triList = (RpBuildMeshTriangle *)
            RwMalloc(sizeof(RpBuildMeshTriangle) *
                     outMeshInfo[i].numIndices,
                     rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

        for (j = 0; j < outMeshInfo[i].numIndices; j++)
        {
            triList[j] = *tempTriPtr[0];
            tempTriPtr++;
        }

        /* Build a set of strips */
        TriStripStripTrisExhaustive(triList,
                                    outMeshInfo[i].numIndices,
                                    &stripList);

#ifdef JOINSTRIPS
        /* Join the strips with degenerate triangles */
        TriStripJoin(&stripList, maintainWinding);
#endif

        stripPtr = stripList.head;
        while (stripPtr)
        {
            RwUInt32            bytes =
                sizeof(RpMesh) +

                (sizeof(RxVertexIndex) * stripPtr->stripLen);

            /* build this into a mesh (this allocated structure is aligned) */
            meshEl = (RpMesh *) RwMalloc(bytes,
                rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
            meshEl->material = outMeshInfo[i].material;
            meshEl->numIndices = stripPtr->stripLen;
            meshEl->indices = (RxVertexIndex *) (meshEl + 1);

            for (j = 0; j < meshEl->numIndices; j++)
            {
                meshEl->indices[j] =
                    (RxVertexIndex) stripPtr->strip[j];
            }
#ifndef JOINSTRIPS
            if (numOutMeshes == numAllocatedOutMeshes)
            {
                outMeshes = RwRealloc(outMeshes, sizeof(RpMesh *) *
                                      (numOutMeshes * 2),
                                      rwMEMHINTDUR_FUNCTION |
                                      rwID_MESHMODULE |
                                      rwMEMHINTFLAG_RESIZABLE);
                numAllocatedOutMeshes = numOutMeshes * 2;
            }
#endif /* JOINSTRIPS */
            outMeshes[numOutMeshes] = meshEl;
            numOutMeshes++;

            stripPtr = stripPtr->next;
        }

        while (NULL != stripList.head)
        {
            stripPtr = stripList.head;
            stripList.head = stripPtr->next;

            RwFree(stripPtr->strip);
            stripPtr->strip = (RwUInt16 *)  NULL;

            RwFreeListFree(RPMESHGLOBAL(triStripListEntryFreeList),
                           stripPtr);
            stripPtr = (TriStripListEntry *)NULL;

        }
        RwFree(triList);
        triList = (RpBuildMeshTriangle *)NULL;
    }

    RwFreeListDestroy(RPMESHGLOBAL(triStripListEntryFreeList));
    RPMESHGLOBAL(triStripListEntryFreeList) = (RwFreeList *)NULL;

    /* now build the whole mesh */
    meshSize = sizeof(RpMeshHeader);
    totalIndices = 0;
    for (i = 0; i < numOutMeshes; i++)
    {
        RwUInt32            indexSize;

        indexSize =
            (sizeof(RxVertexIndex) * outMeshes[i]->numIndices);
        meshSize += sizeof(RpMesh) + indexSize;

        totalIndices += outMeshes[i]->numIndices;
    }

    result = _rpMeshHeaderCreate(meshSize);
    result->flags = rpMESHHEADERTRISTRIP;
    result->numMeshes = numOutMeshes;
    result->serialNum = RPMESHGLOBAL(nextSerialNum);
    result->firstMeshOffset = 0;
    result->totalIndicesInMesh = totalIndices;
    RPMESHGLOBAL(nextSerialNum)++;

    meshEl = (RpMesh *) (result + 1);
    stripMeshInds = (RxVertexIndex *) (meshEl + numOutMeshes);
    for (i = 0; i < numOutMeshes; i++)
    {
        /* Add in the next mesh */
        meshEl->indices = stripMeshInds;
        meshEl->numIndices = outMeshes[i]->numIndices;
        meshEl->material = outMeshes[i]->material;

        /* And the indices */
        meshSize = (sizeof(RxVertexIndex) * outMeshes[i]->numIndices);
        memcpy(stripMeshInds, outMeshes[i]->indices, meshSize);

        /* Skip to next */
        stripMeshInds += meshEl->numIndices;
        meshEl++;

        /* Don't need this any more */
        RwFree(outMeshes[i]);
        outMeshes[i] = (RpMesh *)NULL;
    }

#if 0
    RWMESSAGE(("%d meshes, %d indices", numOutMeshes, totalIndices));
#endif

    RwFree(triPointers);
    triPointers = (RpBuildMeshTriangle **)NULL;

    RwFree(outMeshes);
    outMeshes = (RpMesh **)NULL;

    RwFree(outMeshInfo);
    outMeshInfo = (RpMesh *)NULL;

    RWRETURN(result);
}


/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateExhaustiveTriStrip generates a triangle strip
 * similarly to \ref RpBuildMeshGeneratePreprocessTriStrip but repeats the
 * process starting on every triangle and chooses the longest (not counting
 * degenerates). It should be noted that presently this algorithm is not fully
 * exhaustive.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateExhaustiveTriStrip(RpBuildMesh *buildMesh,
                                      void *data)
{
    RpMeshHeader         *result = (RpMeshHeader *)NULL;

    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateExhaustiveTriStrip"));
    RWASSERT(buildMesh);

    result = BuildMeshGenerateExhaustiveTriStrip(buildMesh, data, TRUE);

    RWRETURN(result);
}

/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip generates a triangle
 * strip similarly to \ref RpBuildMeshGeneratePreprocessTriStrip but repeats
 * the process starting on every triangle and chooses the longest (not counting
 * degenerates). It should be noted that presently this algorithm is not fully
 * exhaustive.
 *
 * \param buildMesh pointer to the mesh which the triangle strip will be
 * generated from.
 * \param data  pointer to user-supplied data to pass to the callback
 * function.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip(RpBuildMesh *buildMesh,
                                                   void *data)
{
    RpMeshHeader         *result = (RpMeshHeader *)NULL;

    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip"));
    RWASSERT(buildMesh);

    result = BuildMeshGenerateExhaustiveTriStrip(buildMesh, data, FALSE);

    RWRETURN(result);
}


/**
 * \ingroup rpmesh
 * \ref RpTriStripDefaultCost calculates the cost of a tristrip as the number
 * of fans contained in the strip.
 *
 * \param startPolygon pointer to the polygon at the start of the strip.
 * \param testFrame Value that is written to the testFrame of all polygons in
 *        the tri-strip
 * \param data pointer to RpTriStripTunnelData supplied as the user data in the
 * RpTriStripData structure passed into \ref RpMeshSetTriStripMethod as the
 * user supplied data.
 *
 * \return the cost of the strip (the number of fans it contains).
 *
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RwUInt32
RpTriStripDefaultCost(RpTriStripPolygon *startPolygon, RwUInt32 testFrame,
                      void *data __RWUNUSED__)
{
    RwUInt32 cost;
    RpTriStripPolygon *polygon, *next, *next2, *next3;
    RwUInt32 beforeFan, afterFan;

    RWAPIFUNCTION(RWSTRING("RpTriStripDefaultCost"));

    cost = 40000;
    beforeFan = 1;
    afterFan = 0;
    polygon = startPolygon;
    polygon->testFrame = testFrame;
    next = RpTriStripPolygonFollowStrip(polygon, NULL);

    if (NULL != next)
    {
        next->testFrame = testFrame;
        next2 = RpTriStripPolygonFollowStrip(next, polygon);

        if (NULL != next2)
        {
            next2->testFrame = testFrame;
            next3 = RpTriStripPolygonFollowStrip(next2, next);
        }
        else
        {
            next3 = NULL;
        }
    }
    else
    {
        next2 = NULL;
        next3 = NULL;
    }

    while (NULL != next3)
    {
        next3->testFrame = testFrame;

        /* check for fan */
        if (_rpTriStripPolygonSharedVertex4(polygon, next, next2, next3))
        {
            afterFan = 1;
            cost += 10000;
        }
        else if (afterFan)
        {
            afterFan++;
        }
        else
        {
            beforeFan++;
        }

        polygon = next;
        next = next2;
        next2 = next3;
        next3 = RpTriStripPolygonFollowStrip(next2, next);
    }

    if (afterFan > beforeFan)
    {
        RWRETURN(cost + beforeFan);
    }
    else
    {
        RWRETURN(cost + afterFan);
    }
}


/**
 * \ingroup rpmesh
 * \ref RpTriStripMeshTunnel uses a tunneling algorithm to tristrip the
 * supplied mesh.
 *
 * \param mesh pointer to the mesh to tristrip.
 * \param data pointer to RpTriStripData structure passed into
 * \ref RpMeshSetTriStripMethod as the user supplied data.
 *
 * \return the mesh.
 *
 * \see RpTriStripMeshQuick
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpTriStripMesh *
RpTriStripMeshTunnel(RpTriStripMesh *mesh, void *data)
{
    RpTriStripTunnelData *tunnelData = (RpTriStripTunnelData *)data;
    RpTriStripTunnel *tunnel;
    RwLLLink *curr, *end;
    _rpTriStripTunnelStack *stack;
    RwUInt32 numTunnels;
    RwReal coeff;
    RwReal minCoeff;

    RWAPIFUNCTION(RWSTRING("RpTriStripMeshTunnel"));

    if (tunnelData->lengthLimit)
    {
        /* create tunnel and stack */
        tunnel = RwMalloc(sizeof(RpTriStripTunnel)
            + tunnelData->lengthLimit * sizeof(RpTriStripEdge *)
            + (1 + tunnelData->lengthLimit) * sizeof(RpTriStripPolygon *),
            rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
        tunnel->edges = (RpTriStripEdge **)(tunnel + 1);
        tunnel->polygons = (RpTriStripPolygon **)
            (tunnel->edges + tunnelData->lengthLimit);
        tunnel->length = 0;
        tunnel->maxLength = 1;
        stack = RwMalloc(3 * tunnelData->lengthLimit
            * sizeof(_rpTriStripTunnelStack),
            rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
        numTunnels = 0;
        coeff = 0.0f;
        minCoeff = (RwReal)RwPow(0.001f, tunnelData->quality);

        /* perform breadth first search for tunnels */
        do
        {
            end = rwLinkListGetTerminator(&mesh->polygonEndList);
            curr = rwLinkListGetFirstLLLink(&mesh->polygonEndList);
            RWASSERT(curr != end);

            while (curr != end)
            {
                tunnel->polygons[0] = rwLLLinkGetData(curr, RpTriStripPolygon,
                    inEndLink);

                if (_rpTriStripTunnelFind(tunnel, tunnelData, stack))
                {
                    break;
                }

                curr = rwLLLinkGetNext(curr);
            }

            if (curr == end)
            {
                coeff = 0.5f * coeff + (RwReal)numTunnels / mesh->numPolygons;
                tunnel->maxLength++;
                numTunnels = 0;

                if (coeff < minCoeff
                    || tunnel->maxLength >= tunnelData->lengthLimit)
                {
                    break;
                }
            }
            else
            {
                _rpTriStripMeshComplementTunnel(mesh, tunnel);
                tunnel->length = 0;
                numTunnels++;
            }
        } while (1);

        /* tidy up */
        RwFree(stack);
        RwFree(tunnel);
    }

    RWRETURN(mesh);
}


/**
 * \ingroup rpmesh
 * \ref RpTriStripMeshQuick generates a triangle strip by selecting a starting
 * triangle with the lowest adjacency. It then builds a strip off each side of
 * triangle and chooses the longest (not counting degenerates). This is
 * repeated until the mesh is stripped.
 *
 * \param mesh pointer to the mesh to tristrip.
 * \param data pointer to user supplied data (ignored).
 *
 * \return the mesh.
 *
 * \see RpTriStripMeshTunnel
 * \see RpBuildMeshGenerateTriStrip
 *
 */
RpTriStripMesh *
RpTriStripMeshQuick(RpTriStripMesh *mesh,
                    void *data __RWUNUSED__)
{
    RwLLLink *curr, *end;
    RwLinkList adjList[4];
    RwUInt32 i, j;
    RpTriStripPolygon *polygon;
    RwUInt32 bestLength, bestDir;
    RwUInt32 testFrame;

    RWAPIFUNCTION(RWSTRING("RpTriStripMeshQuick"));

    rwLinkListInitialize(&adjList[0]);
    rwLinkListInitialize(&adjList[1]);
    rwLinkListInitialize(&adjList[2]);
    rwLinkListInitialize(&adjList[3]);
    curr = rwLinkListGetFirstLLLink(&mesh->polygonUsedList);
    end = rwLinkListGetTerminator(&mesh->polygonUsedList);
    while (curr != end)
    {
        RwUInt32 adj;
        RpTriStripEdge *edge;

        polygon = rwLLLinkGetData(curr, RpTriStripPolygon, inUsedLink);

        adj = 0;
        for (i = 0; i < polygon->numEdges; i++)
        {
            edge = polygon->edges[i];

            if ((NULL != edge->poly1 && polygon != edge->poly1)
                || (NULL != edge->poly2 && polygon != edge->poly2))
            {
                adj++;
            }
        }

        rwLinkListAddLLLink(&adjList[adj], &polygon->inFreeLink);

        curr = rwLLLinkGetNext(curr);
    }

    testFrame = 0;
    i = 0;
    do
    {
        curr = rwLinkListGetFirstLLLink(&adjList[i]);
        end = rwLinkListGetTerminator(&adjList[i]);

        if (curr != end)
        {
            RpTriStripEdge *edge, *next;
            RpTriStripPolygon *test;
            RxVertexIndex oldIndex;

            bestLength = 0;
            bestDir = 0;
            polygon = rwLLLinkGetData(curr, RpTriStripPolygon, inFreeLink);

            for (j = 0; j < 6; j++)
            {
                RwUInt32 length;

                length = 1;
                edge = polygon->edges[j % 3];
                test = polygon == edge->poly1 ? edge->poly2 : edge->poly1;
                if (NULL != test && rwLLLinkAttached(&test->inFreeLink))
                {
                    oldIndex = j < 3 ? edge->vert1 : edge->vert2;
                    testFrame++;
                    if (!testFrame)
                    {
                        testFrame = 1;
                    }
                    polygon->testFrame = testFrame;

                    while (NULL != test && testFrame != test->testFrame)
                    {
                        test->testFrame = testFrame;
                        length++;

                        next = _rpTriStripEdgeFindNext(edge, test, oldIndex, testFrame);

                        if (NULL != next)
                        {
                            if (edge->vert1 == next->vert1 || edge->vert1 == next->vert2)
                            {
                                oldIndex = edge->vert1;
                            }
                            else
                            {
                                RWASSERT(edge->vert2 == next->vert1 || edge->vert2 == next->vert2);
                                oldIndex = edge->vert2;
                            }
                            edge = next;
                            test = test == edge->poly1 ? edge->poly2 : edge->poly1;
                        }
                        else
                        {
                            test = NULL;
                        }
                    }
                }

                if (length > bestLength)
                {
                    bestDir = j;
                    bestLength = length;
                }
            }

            /* create strip and remove strip polygons from adjacency lists */
            rwLinkListRemoveLLLink(&polygon->inFreeLink);
            rwLLLinkInitialize(&polygon->inFreeLink);

            edge = polygon->edges[bestDir % 3];
            test = polygon == edge->poly1 ? edge->poly2 : edge->poly1;
            if (NULL != test && rwLLLinkAttached(&test->inFreeLink))
            {
                oldIndex = bestDir < 3 ? edge->vert1 : edge->vert2;

                while (NULL != test && 0 != test->testFrame)
                {
                    RWASSERT(!edge->strip);
                    RWASSERT(2 > _rpTriStripPolygonNumStripEdges(edge->poly1));
                    RWASSERT(2 > _rpTriStripPolygonNumStripEdges(edge->poly2));
                    edge->strip = TRUE;
                    test->testFrame = 0;

                    rwLinkListRemoveLLLink(&test->inFreeLink);
                    rwLLLinkInitialize(&test->inFreeLink);

                    next = _rpTriStripEdgeFindNext(edge, test, oldIndex, 0);

                    if (NULL != next)
                    {
                        if (edge->vert1 == next->vert1 || edge->vert1 == next->vert2)
                        {
                            oldIndex = edge->vert1;
                        }
                        else
                        {
                            RWASSERT(edge->vert2 == next->vert1 || edge->vert2 == next->vert2);
                            oldIndex = edge->vert2;
                        }
                        edge = next;

                        RWASSERT(rwLLLinkAttached(&test->inEndLink));
                        rwLinkListRemoveLLLink(&test->inEndLink);
                        rwLLLinkInitialize(&test->inEndLink);
                        test = test == edge->poly1 ? edge->poly2 : edge->poly1;
                    }
                    else
                    {
                        test = NULL;
                    }
                }
            }

            /* update adjacency lists */
            {
                RpTriStripPolygon *prev, *curr, *next;

                RWASSERT(1 >= _rpTriStripPolygonNumStripEdges(polygon));
                curr = polygon;
                prev = NULL;
                next = RpTriStripPolygonFollowStrip(curr, prev);

                while (NULL != next)
                {
                    RWASSERT(2 >= _rpTriStripPolygonNumStripEdges(next));
                    prev = curr;
                    curr = next;
                    next = RpTriStripPolygonFollowStrip(curr, prev);
                }

                while (NULL != curr)
                {
                    for (j = 0; j < 3; j++)
                    {
                        RpTriStripPolygon *temp;

                        temp = curr == curr->edges[j]->poly1 ? curr->edges[j]->poly2 : curr->edges[j]->poly1;

                        if (NULL != temp && rwLLLinkAttached(&temp->inFreeLink))
                        {
                            rwLinkListRemoveLLLink(&temp->inFreeLink);
                            rwLLLinkInitialize(&temp->inFreeLink);
                            rwLinkListAddLLLink(&adjList[_rpTriStripPolygonAdjCount(temp)], &temp->inFreeLink);
                        }
                    }

                    next = curr;
                    curr = prev;
                    prev = NULL != curr ? RpTriStripPolygonFollowStrip(curr, next) : NULL;
                }
            }

            i = 0;
        }
        else
        {
            i++;
        }
    } while (i < 4);

    RWRETURN(mesh);
}


/**
 * \ingroup rpmesh
 * \ref RpTriStripPolygonFollowStrip returns the next polygon of the tristrip.
 *
 * \param curr pointer to current polygon of the tristrip.
 * \param prev pointer to the previous polygon of the tristrip. If this is NULL
 * then any polygon joined to the current polygon by a strip edge will be
 * returned.
 *
 * \return pointer to the next polygon of the tristrip.
 *
 */
RpTriStripPolygon *
RpTriStripPolygonFollowStrip(RpTriStripPolygon *curr, RpTriStripPolygon *prev)
{

    RwUInt32 i;
    RpTriStripPolygon *next;

    RWAPIFUNCTION(RWSTRING("RpTriStripPolygonFollowStrip"));
    RWASSERT(curr);

    next = NULL;
    for (i = 0; i < curr->numEdges; i++)
    {
        if (curr->edges[i]->strip && prev != curr->edges[i]->poly1
            && prev != curr->edges[i]->poly2)
        {
            next = curr == curr->edges[i]->poly1
                ? curr->edges[i]->poly2 : curr->edges[i]->poly1;
            break;
        }
    }

    RWRETURN(next);
}


/**
 * \ingroup rpmesh
 * \ref RpBuildMeshGenerateTriStrip generates a triangle strip using the
 * tristripping callback supplied in the RpTriStripData structure passed into
 * \ref RpMeshSetTriStripMethod as user supplied data.
 *
 * \return a pointer to the constructed mesh header.
 *
 * \see RpTriStripMeshTunnel
 * \see RpMeshGetTriStripMethod
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 *
 */
RpMeshHeader *
RpBuildMeshGenerateTriStrip(RpBuildMesh *buildMesh, void *data)
{
    RpBuildMeshTriangle **triangles;
    RpBuildMeshTriangle  *triangle;
    RpTriStripMesh      **meshes;
    RwUInt32              i;
    RwUInt32              numTriangles;
    RwUInt32              numMaterials;
    RwUInt32              materialNum;
    RwUInt32              materialStart;
    RpTriStripData       *triStripData = (RpTriStripData *)data;
    RpMeshHeader         *header;
    RpMesh               *mesh;
    RxVertexIndex        *indices;

    RWAPIFUNCTION(RWSTRING("RpBuildMeshGenerateTriStrip"));
    RWASSERT(buildMesh);
    RWASSERT(triStripData);
    RWASSERT(triStripData->stripCB);

    /* sort non degenerate triangles into material order */
    numTriangles = 0;
    triangles = RwMalloc(buildMesh->numTriangles * sizeof(RpBuildMeshTriangle *),
                         rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);
    for (i = 0; i < (RwUInt32)buildMesh->numTriangles; i++)
    {
        triangle = &buildMesh->meshTriangles[i];
        if (triangle->vertIndex[0] != triangle->vertIndex[1]
            && triangle->vertIndex[1] != triangle->vertIndex[2]
            && triangle->vertIndex[2] != triangle->vertIndex[0])
        {
            triangles[numTriangles++] = triangle;
        }
    }
    RWASSERT(numTriangles > 0);
    qsort(triangles, numTriangles, sizeof(RpBuildMeshTriangle *),
        SortPolygons);

    /* count number of materials */
    numMaterials = 1;
    for (i = 1; i < numTriangles; i++)
    {
        if (triangles[i-1]->material != triangles[i]->material)
        {
            numMaterials++;
        }
    }
    meshes = RwMalloc(numMaterials * sizeof(RpTriStripMesh *),
        rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    /* create a mesh for each material and tristrip it */
    materialNum = 0;
    materialStart = 0;
    for (i = 1; i < numTriangles; i++)
    {
        if (triangles[i-1]->material != triangles[i]->material)
        {
            meshes[materialNum] =
                _rpTriStripMeshCreate(triangles + materialStart, i - materialStart);
            (triStripData->stripCB)(meshes[materialNum], triStripData->data);

            materialNum++;
            materialStart = i;
        }
    }
    meshes[materialNum] =
        _rpTriStripMeshCreate(triangles + materialStart, i - materialStart);
    (triStripData->stripCB)(meshes[materialNum], triStripData->data);

    /* create output mesh header */
    header = RwMalloc(sizeof(RpMeshHeader)
        + numMaterials * sizeof(RpMesh)
        + 6 * numTriangles * sizeof(RxVertexIndex),
        rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    header->flags = rpMESHHEADERTRISTRIP;
    header->numMeshes = numMaterials;
    header->serialNum = 0;
    header->totalIndicesInMesh = 0;
    header->firstMeshOffset = 0;

    /* create output meshes */
    mesh = (RpMesh *)(header + 1);
    indices = (RxVertexIndex *)(mesh + numMaterials);
    materialNum = 0;
    for (i = 1; i < numTriangles; i++)
    {
        if (triangles[i-1]->material != triangles[i]->material)
        {
            mesh->indices = indices;
            mesh->material = triangles[i-1]->material;
            _rpTriStripMeshCreateOutput(meshes[materialNum], mesh,
                triStripData);
            indices += mesh->numIndices;
            header->totalIndicesInMesh += mesh->numIndices;
            mesh++;
            materialNum++;
        }
    }
    mesh->indices = indices;
    mesh->material = triangles[i-1]->material;
    _rpTriStripMeshCreateOutput(meshes[materialNum], mesh, triStripData);
    indices += mesh->numIndices;
    header->totalIndicesInMesh += mesh->numIndices;
    mesh++;

    /* resize header */
    header = RwRealloc(header, (RwUInt8 *)indices - (RwUInt8 *)header,
                       rwMEMHINTDUR_EVENT | rwID_MESHMODULE);
    mesh = (RpMesh *)(header + 1);
    indices = (RxVertexIndex *)(mesh + numMaterials);
    for (i = 0; i < numMaterials; i++)
    {
        mesh->indices = indices;
        indices += mesh->numIndices;
        mesh++;
    }

    /* tidy up */
    for (i = 0; i < numMaterials; i++)
    {
        RwFree(meshes[i]);
    }
    RwFree(meshes);
    RwFree(triangles);

    RWRETURN(header);
}


/**
 * \ingroup rpmesh
 * \ref RpMeshSetTriStripMethod is used to register the specified
 * triangle strip generation function. The specified callback
 * function is executed when the the geometry or world sectors
 * are unlocked. If NULL is specified for the callback, then
 * \ref RpBuildMeshGenerateDefaultTriStrip is registered as the default
 * function and the user-supplied data is set to NULL.
 *
 * The format of the callback is
 * \verbatim
   RpMeshHeader *
   (*RpTriStripMeshCallBack)(RpBuildMesh *buildMesh, void *data);
   \endverbatim
 *
 * \param callback  pointer to the callback function.
 * \param data  pointer to user-supplied data to pass to callback function.
 * \return TRUE.
 *
 * \see RpMeshGetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 */
RwBool
RpMeshSetTriStripMethod(RpTriStripMeshCallBack callback,
                        void * data)
{
    RWAPIFUNCTION(RWSTRING("RpMeshSetTriStripMethod"));

    if (callback)
    {
        MeshopStatic.meshTristripMethod = callback;
        MeshopStatic.data = data;
    }
    else
    {
        MeshopStatic.meshTristripMethod = RpBuildMeshGenerateDefaultTriStrip;
        MeshopStatic.data = NULL;
    }

    RWRETURN(TRUE);
}


/**
 * \ingroup rpmesh
 * \ref RpMeshGetTriStripMethod is used to query the specified
 * triangle strip generation function and user-supplied data.
 *
 * The format of the callback is
 * \verbatim
   RpMeshHeader *
   (*RpTriStripMeshCallBack)(RpBuildMesh *buildMesh,
                             void *data);
   \endverbatim
 *
 * \param callback  pointer to receive the callback function. If NULL then
 * ignored.
 * \param data  pointer to recieve the user-supplied data which is passed
 * to the callback function. If NULL then ignored.
 * \return TRUE.
 *
 * \see RpMeshSetTriStripMethod
 * \see RpBuildMeshGenerateTriStrip
 * \see RpBuildMeshGenerateDefaultTriStrip
 * \see RpBuildMeshGeneratePreprocessTriStrip
 * \see RpBuildMeshGenerateExhaustiveTriStrip
 * \see RpBuildMeshGenerateTrivialTriStrip
 * \see RpBuildMeshGenerateDefaultIgnoreWindingTriStrip
 * \see RpBuildMeshGeneratePreprocessIgnoreWindingTriStrip
 * \see RpBuildMeshGenerateExhaustiveIgnoreWindingTriStrip
 *
 */
RwBool
RpMeshGetTriStripMethod(RpTriStripMeshCallBack * callback,
                        void ** data)
{
    RWAPIFUNCTION(RWSTRING("RpMeshGetTriStripMethod"));

    if (callback)
    {
        *callback = MeshopStatic.meshTristripMethod;
    }

    if (data)
    {
        *data = MeshopStatic.data;
    }

    RWRETURN(TRUE);
}


/*
 * Stealth functions
 *
 * No longer seems to crash the Intel compiler !!!
 */

/****************************************************************************
 *
 * Turn off optimization for _rpTriListMeshGenerate for Intel compilers. For
 * unknown reasons it caused the compiler to crash in opti builds.
 *
 ***************************************************************************/
#if (0 && defined(__ICL))
#if (450==__ICL)
#pragma message ( __FILE__ ":" RW_STRINGIFY_EXPANDED(__LINE__) ":" RW_STRINGIFY_EXPANDED(__ICL) "==__ICL - turning off optimization")
#pragma optimize("", off)
#endif /* (450==__ICL) */
#endif /* (0 && defined(__ICL)) */

/****************************************************************************
 _rpTriListMeshGenerate

 On entry   :
 On exit    :
 */
RpMeshHeader *
_rpTriListMeshGenerate(RpBuildMesh * buildMesh,
                       void * data __RWUNUSED__)
{
    RpBuildMeshTriangle **triPointers;

    RWFUNCTION(RWSTRING("_rpTriListMeshGenerate"));
    RWASSERT(buildMesh);

    triPointers = (RpBuildMeshTriangle **)
        RwMalloc(buildMesh->numTriangles *
                 sizeof(RpBuildMeshTriangle *),
                 rwMEMHINTDUR_FUNCTION | rwID_MESHMODULE);

    if (triPointers)
    {
        RpMeshHeader       *result;
        RwUInt32            i;
        RwUInt32            numMats;
        RwUInt32            meshSize;
        RpMesh             *meshEl;
        RxVertexIndex    *meshTriInds;

        /* Fill in pointers so that we can sort */
        for (i = 0; i < buildMesh->numTriangles; i++)
        {
            triPointers[i] = &(buildMesh->meshTriangles[i]);
        }

        /* Now sort the pointers by material */
        qsort(triPointers, buildMesh->numTriangles,
              sizeof(RpBuildMeshTriangle *), SortPolygons);

        /* Figure out how many different materials there are */
        numMats = 1;
        if (buildMesh->numTriangles >= 2)
        {
            RpMaterial         *lastMat = triPointers[0]->material;

            for (i = 1; i < buildMesh->numTriangles; i++)
            {
                if (triPointers[i]->material != lastMat)
                {
                    /* We found another different one */
                    lastMat = triPointers[i]->material;
                    numMats++;
                }
            }
        }

        /* And generate an output mesh
         * (allow a bit per material for alignment) */
        meshSize =
            (sizeof(RpMeshHeader)) +
            (sizeof(RpMesh) * numMats) +
            (sizeof(RxVertexIndex) * 3 * buildMesh->numTriangles);

        result = _rpMeshHeaderCreate(meshSize);
        if (!result)
        {
            RwFree(triPointers);
            RWERROR((E_RW_NOMEM, meshSize));
            RWRETURN((RpMeshHeader *)NULL);
        }

        result->flags = 0;
        result->numMeshes = 1;
        result->serialNum = RPMESHGLOBAL(nextSerialNum);
        result->firstMeshOffset = 0;
        /* Nothing clever done for tri lists re connecting triangles */
        result->totalIndicesInMesh = buildMesh->numTriangles * 3;
        RPMESHGLOBAL(nextSerialNum)++;

        meshEl = (RpMesh *) (result + 1);
        meshTriInds = (RxVertexIndex *) (meshEl + numMats);

        /* Start here */
        meshEl->numIndices = 0;
        meshEl->material = triPointers[0]->material;
        meshEl->indices = meshTriInds;
        i = 0;
        do
        {
            if (triPointers[i]->material != meshEl->material)
            {
                meshEl++;

                meshEl->numIndices = 0;
                meshEl->material = triPointers[i]->material;
                meshEl->indices = meshTriInds;

                result->numMeshes++;
            }

            *meshTriInds++ =
                (RxVertexIndex) triPointers[i]->vertIndex[0];
            *meshTriInds++ =
                (RxVertexIndex) triPointers[i]->vertIndex[1];
            *meshTriInds++ =
                (RxVertexIndex) triPointers[i]->vertIndex[2];

            meshEl->numIndices += 3;

            i++;
        }
        while (i < buildMesh->numTriangles);

        /* We can do more to optimise this mesh.  By separating polygons which
         * share vertices, we can dispatch more polygons in one go when we have
         * a situation where polygons are supplying the texture coordinates,
         * eg.
         */

        _rpMeshHeaderForAllMeshes(result,
                                  SortPolygonsInTriListMesh, NULL);

        /* Free the workspace */
        RwFree(triPointers);

        /* Return the new mesh */
        RWRETURN(result);
    }

    /* Failed to allocate workspace */
    RWERROR(
            (E_RW_NOMEM,
             buildMesh->numTriangles * sizeof(RpBuildMeshTriangle *)));
    RWRETURN((RpMeshHeader *)NULL);
}

/****************************************************************************
 *
 * Re-enable optimization after _rpTriListMeshGenerate for Intel compilers.
 * For unknown reasons it caused the compiler to crash in opti builds.
 *
 ***************************************************************************/
#if (0 && defined(__ICL))
#if (450==__ICL)
#pragma message ( __FILE__ ":" RW_STRINGIFY_EXPANDED(__LINE__) ":" RW_STRINGIFY_EXPANDED(__ICL) "==__ICL - turning on optimization")
#pragma optimize("", on)
#endif /* (450==__ICL) */
#endif /* (0 && defined(__ICL)) */

/****************************************************************************/


/****************************************************************************
 _rpMeshOptimise

 On entry   : Mesh to optimise, flags indicating type of mesh to build
 On exit    : New mesh list on success, or NULL on failure
 */
RpMeshHeader *
_rpMeshOptimise(RpBuildMesh * mesh, RwUInt32 flags)
{
    RpTriStripMeshCallBack  func = (RpTriStripMeshCallBack)NULL;
    void                   *data = NULL;

    RWFUNCTION(RWSTRING("_rpMeshOptimise"));
    RWASSERT(mesh);

    if (mesh)
    {
        if (mesh->numTriangles == 0)
        {
            /* Flags don't matter, with 0 triangles, it makes no difference! */
            _rpBuildMeshDestroy(mesh);
            RWRETURN(&MeshopStatic.nullMeshHeader);
        }
        else
        {
            RpMeshHeader *newMesh;

            if (flags & rpMESHHEADERTRISTRIP)
            {
                /* How have we been told to tristrip the meshes? */
                func = MeshopStatic.meshTristripMethod;
                data = MeshopStatic.data;
            }
            else
            {
                func = _rpTriListMeshGenerate;
                data = NULL;
            }

            RWASSERT(((RpTriStripMeshCallBack) NULL) != func);

            newMesh = func(mesh, data);

            if (newMesh)
            {
                _rpBuildMeshDestroy(mesh);

                RWRETURN(newMesh);
            }

            /* Failed to generate the mesh, error already output */
        }
    }

    RWRETURN((RpMeshHeader *)NULL);
}
