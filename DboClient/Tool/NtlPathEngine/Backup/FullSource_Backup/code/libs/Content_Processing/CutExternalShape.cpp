// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "platform_common/Header.h"
#include "CutExternalShape.h"
#include "ExternalShapeTraversal.h"
#include "iFaceIsOnTerrainCallBack.h"
#include "ExternalShapeInfo.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/AttributesBuilder.h"
#include "common/Containers/BitVector.h"
#include "common/TemplateMesh/External.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/Helper.h"
#include "common/interface/Error.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"

using std::vector;

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

static void
SplitFace_ClearIndices(tMesh& mesh, tEdge e1, tEdge e2)
{
    mesh.splitFace(e1, e2, tMesh::tEdgeData(e1->getZ()), tMesh::tEdgeData(e2->getZ()));
}

static void
InsertVertices(tMesh& mesh, cExternalShapeTraversal& traversal, const cMeshElement<tMesh>& traversalEnd)
{
    cMeshElement<tMesh> nullEl;
    nullEl.clear();
    cMeshElement<tMesh> el;
    do
    {
        el = traversal.advance();
        tPoint p;
        if(traversal.atShapeVertex(p))
        {
            if(el.isFace())
            {
                tMesh::tVertexData vertexData(p.getX(), p.getY());
                tFace toSplit = el.asFace();
                float z = toSplit->heightForF(p);
                tMesh::tEdgeData edgeData[6];
                tEdge edges[3];
                tEdge e = tMesh::beginEdge(toSplit);
                tSigned32 i;
                for(i = 0; i != 3; ++i)
                {
                    edges[i] = e++;
                    edgeData[i * 2]._z = z;
                    edgeData[i * 2 + 1]._z = e->getZ();
                }
                tVertex v = mesh.splitFaceWithVertex(toSplit, 3, vertexData, edgeData, edges);
                traversal.meshModified(nullEl, v);
            }
            else
            if(el.isEdge())
            {
                tEdge e = el.asEdge();
              // ** note the edge affected flags are not set here because behind portal edges should be able to survive being modified in this way
                tMesh::tVertexData vertexData(p.getX(), p.getY());
                tMesh::tEdgeData edgeData;
                if(e.face()->isExternal())
                {
                    edgeData._z = e.twin().face()->heightForF(p);
                }
                else
                {
                    edgeData._z = e.face()->heightForF(p);
                }
                mesh.splitEdge(e, vertexData, edgeData);
                if(!e.face()->isExternal())
                {
                    SplitFace_ClearIndices(mesh, e.next(), e.prev());
                }
                if(!e.twin().face()->isExternal())
                {
                    SplitFace_ClearIndices(mesh, e.twin(), e.twin().next().next());
                }
                traversal.meshModified(nullEl, e.next().vertex());
            }
        }
    }
    while(el != traversalEnd);
}


static void
FindConnectedFaces_NotExternal(tFace startFrom, cBitVector& isConnected)
{
    isConnected.setBit(startFrom.index(), true);
    tEdge e = tMesh::beginEdge(startFrom);
    do
    {
        if(e.twin().valid())
        {
            tFace f = e.twin().face();
            if(!f->isExternal() && !isConnected[f.index()])
            {
                FindConnectedFaces_NotExternal(f, isConnected);
            }
        }
    }
    while(e.toNextInFace());
}

static void
DeleteConnectedFaces(
        tMesh& mesh, tFace startFrom,
        bool& reportIfExitsTerrain,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        tSigned32 errorReportFace,
        tFace dontEatMe
        )
{
    cBitVector isConnected(mesh.faceSize());
    FindConnectedFaces_NotExternal(startFrom, isConnected);
    if(isConnected[dontEatMe.index()])
    {
        cAttributesBuilder attributes;
        attributes.add("faceIndex", errorReportFace);
        Error("NonFatal", "Geometry inside a building external shape bridges outside the shape cut boundary. Only faces touching the inside cut boundary will be deleted.", attributes.get());
        return;
    }
    tSigned32 i;
    for(i = 0; i != mesh.faceSize(); ++i)
    {
        if(!isConnected[i])
        {
            continue;
        }
        tFace f = mesh.faceAt(i);
        if(reportIfExitsTerrain && !faceIsOnTerrainCallBack.faceIsOnTerrain(f))
        {
            reportIfExitsTerrain = false;
            cAttributesBuilder attributes;
            attributes.add("faceIndex", errorReportFace);
            Error("NonFatal", "The building external shape being cut for a terrain portal exits the terrain. (Reported just once per cut.)", attributes.get());
        }
        tEdge e = tMesh::beginEdge(f);
        do
        {
            if(e.twin().valid())
            {
                tFace fTwin = e.twin().face();
                if(fTwin->isExternal())
                {
                    mesh.eraseFace(fTwin);
                }
            }
        }
        while(e.toNextInFace());
        mesh.eraseFace(f);
    }
}

static tPoint
IntersectionApproximatedToLeft(const tLine& line1, const tLine& line2)
{
    tPoint pointArray[4];
    tSigned32 points;
    tIntersection intersection(line1, line2);
    intersection.approximatePoints_IsOnAxis(points, pointArray);
    if(points == 1)
    {
        return pointArray[0];
    }
    if(points == 2)
    {
        eSide side = line1.side(pointArray[0]);
        if(side == SIDE_LEFT)
        {
            return pointArray[0];
        }
        if(side == SIDE_RIGHT)
        {
            return pointArray[1];
        }
        eDirection direction = line1.axis().direction_KnownToBeInline(pointArray[1] - pointArray[0]);
        if(direction == DIRECTION_FORWARD)
        {
            return pointArray[0];
        }
        assertD(direction == DIRECTION_FORWARD);
        return pointArray[1];
    }
  // actually returns the first approximation on the left of the line
  // (note that the points are guaranteed to be ordered clockwise in the case of 4 points)
    tSigned32 i;
    for(i = 0; i != points; ++i)
    {
        if(line1.side(pointArray[i]) == SIDE_RIGHT)
        {
            break;
        }
    }
    assertD(i != points);
    tSigned32 j;
    for(j = 0; j != points; ++j)
    {
        ++i;
        if(i == points)
        {
            i = 0;
        }
        if(line1.side(pointArray[i]) != SIDE_RIGHT)
        {
            return pointArray[i];
        }
    }
    invalid();
    return pointArray[0];
}

static tVertex
DoSplitEdge(tMesh& mesh, tEdge e, const tLine& splitLine, cGeometryOverlay<tMesh>& overlay)
{
    tLine edgeLine = overlay.getEdgeLine(e);
    tPoint p = IntersectionApproximatedToLeft(splitLine, edgeLine);
    tMesh::tVertexData vertexData(p.getX(), p.getY());
    tMesh::tEdgeData edgeData;
    edgeData._z = e.face()->heightForF(p);
    mesh.splitEdge(e, vertexData, edgeData);
    overlay.pushBack(e, edgeLine);
    overlay.pushBack(e.next(), edgeLine);
    edgeLine.reverse();
    overlay.pushBack(e.twin(), edgeLine);
    overlay.pushBack(e.next().twin(), edgeLine);
    return e.next().vertex();
}

static bool
FaceIsConvex_CanHaveMoreThanThreeEdges(tFace f)
{
    tEdge e = tMesh::beginEdge(f);
    assertD(CountEdgesAround(e) >= 3);
    do
    {
        tLine l = GetEdgeLine(e);
        tEdge e2 = e.next().next();
        do
        {
            if(l.side(GetVertexPoint(e2)) != SIDE_RIGHT)
            {
                return false;
            }
        }
        while((++e2) != e);
    }
    while(e.toNextInFace());
    return true;
}

static bool
VertexApproximationMakesBadFace(tVertex v, cGeometryOverlay<tMesh>& overlay)
{
    tPoint p = GetVertexPoint(v);
    tEdge e = tMesh::firstEdge(v);
    do
    {
//// note that approximated faces can be non-convex, but cannot be zero area or downward facing
//        assertD(CountEdgesAround(e) >= 3);
//        tEdge e2 = e.next();
//        do
//        {
//            if(overlay.getEdgeLine(e2).side(p) != SIDE_RIGHT)
//            {
//                return true;
//            }
//        }
//        while((++e2) != e.prev());

        if(!FaceIsConvex_CanHaveMoreThanThreeEdges(e.face()))
        {
            return true;
        }
    }
    while(e.toNextAroundVertex());
    return false;
}

static void
RemoveSplitVertex(tMesh& mesh, cGeometryOverlay<tMesh>& overlay, tVertex v)
{
    assertD(!v.isInternalVertex());
    tEdge firstE = tMesh::firstEdge(v);
    if(!firstE.prev().twin().valid())
    {
        eSide side = overlay.getEdgeAxis(firstE.prev()).side(overlay.getEdgeAxis(firstE));
        if(side == SIDE_CENTRE)
        {
          // internal edges for this vertex were joined during processing of other bad vertices
            mesh.unsplitEdge(firstE.prev());
            return;
        }
        assertD(side == SIDE_RIGHT);
        // corner split, where the cut shape entered through an external edge
        mesh.eraseFace(firstE.face());
        return;
    }
    do
    {
        mesh.joinFaces(firstE.prev());
    }
    while(firstE.prev().twin().valid());
    mesh.unsplitEdge(firstE.prev());
}

static void
CutExternalShape(
        tMesh& mesh, tEdge ePortal, tEdge ePortalInside, tEdge eShape,
        bool isTerrainShape, iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        tSigned32 errorReportFace
        )
{
    assertF(ValidateMeshConnections(mesh));

    cGeometryOverlay<tMesh> overlay;

    {
        cExternalShapeTraversal traversal(overlay, ePortal, ePortal.vertex(), eShape.next());
        InsertVertices(mesh, traversal, ePortal.vertex());
    }

    std::vector<tEdge> edgesToBreak;
    std::vector<tEdge> edgesToBreakTwins;
    std::vector<tVertex> splitVertices;
    {
        cExternalShapeTraversal traversal(overlay, ePortal, ePortal.vertex(), eShape.next());
        cMeshElement<tMesh> el1(ePortal.vertex());
        do
        {
            tLine splitLine = traversal.getCurrentLine();
            cMeshElement<tMesh> el2 = traversal.advance();
            cMeshElement<tMesh> el3 = traversal.advance();
            if(el2.isEdge())
            {
                tEdge e = el2.asEdgeFromVertex(el1.asVertex());
                if(!e.face()->isExternal())
                {
                    edgesToBreak.push_back(e);
                }
                el1 = el3;
                continue;
            }
            if(el2.isExternalFace())
            {
                assertD(el1.isVertex());
                el1 = el3;
                continue;
            }

            tEdge eVertex;
            if(!el1.isEdge())
            {
                eVertex = EdgeInFaceForVertex<tMesh>(el2.asFace(), el1.asVertex());
            }
            else
            {
                assertR(el1.asEdgeInFace(el2.asFace()).twin().face()->isExternal());
                eVertex = el1.asEdgeInFace(el2.asFace());
            }

            vector<tEdge> edgesToSplit;
            while(el3.isEdge() && !el3.asEdgeInFace(el2.asFace()).twin().face()->isExternal())
            {
                edgesToSplit.push_back(el3.asEdgeInFace(el2.asFace()));
                el2 = traversal.advance();
                el3 = traversal.advance();
            }

            if(el1.isEdge())
            {
                splitVertices.push_back(DoSplitEdge(mesh, eVertex, splitLine, overlay));
                ++eVertex;
            }

            tSigned32 i;
            for(i = 0; i != edgesToSplit.size(); ++i)
            {
                splitVertices.push_back(DoSplitEdge(mesh, edgesToSplit[i], splitLine, overlay));
                SplitFace_ClearIndices(mesh, eVertex, edgesToSplit[i].next());
                edgesToBreak.push_back(eVertex.prev().twin());
                assertD(edgesToBreak.back().valid());
                overlay.pushBack(eVertex.prev(), splitLine.reversed());
                overlay.pushBack(eVertex.prev().twin(), splitLine);
                eVertex = edgesToSplit[i].twin();
            }

            assertD(ValidateMeshConnections(mesh));

            tEdge eLastVertex;
            if(el3.isEdge())
            {
                eLastVertex = el3.asEdgeInFace(el2.asFace());
                splitVertices.push_back(DoSplitEdge(mesh, eLastVertex, splitLine, overlay));
                ++eLastVertex;
            }
            else
            {
                eLastVertex = EdgeInFaceForVertex<tMesh>(el2.asFace(), el3.asVertex());
            }

            SplitFace_ClearIndices(mesh, eVertex, eLastVertex);
            edgesToBreak.push_back(eVertex.prev().twin());
            assertD(edgesToBreak.back().valid());
            overlay.pushBack(eVertex.prev(), splitLine.reversed());
            overlay.pushBack(eLastVertex.prev(), splitLine);

            traversal.meshModified(eLastVertex.prev(), eLastVertex.vertex());
            el1 = eLastVertex.vertex();
        }
        while(el1 != ePortal.next().vertex());
    }

    RemoveExternalFaces(mesh);

    edgesToBreakTwins.resize(edgesToBreak.size());
    tSigned32 i;
    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        edgesToBreakTwins[i] = edgesToBreak[i].twin();
        mesh.breakEdge(edgesToBreak[i]);
    }

    {
        bool reportIfExitsTerrain = isTerrainShape;
        tFace dontEatMe = ePortalInside.face();
        DeleteConnectedFaces(mesh, ePortal.face(), reportIfExitsTerrain, faceIsOnTerrainCallBack, errorReportFace, dontEatMe);
    }

#ifdef ASSERTIONS_ON
    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        tEdge e = edgesToBreak[i];
        assertD(!e.entryIsDead()); // should not be possible for the fill in DeleteConnectedFaces to escape the cut boundary
    }
    for(i = 0; i != SizeL(splitVertices); ++i)
    {
        tVertex v = splitVertices[i];
        assertD(!v.entryIsDead());
    }
#endif

    for(i = 0; i != SizeL(edgesToBreakTwins); ++i)
    {
        tEdge e = edgesToBreakTwins[i];
        if(!e.valid() || e.entryIsDead())
        {
            continue;
        }
        mesh.eraseFace(e.face()); // should only get here in the case where DeleteConnectedFaces is aborted
    }

    {
        std::vector<tVertex> badVertices;
        for(i = 0; i != SizeL(splitVertices); ++i)
        {
            tVertex v = splitVertices[i];
            assertD(!v.entryIsDead());
            if(VertexApproximationMakesBadFace(v, overlay))
            {
                badVertices.push_back(v);
            }
        }
        for(i = 0; i != SizeL(badVertices); ++i)
        {
            tVertex v = badVertices[i];
            if(v.entryIsDead())
            {
            // vertex was deleted together with a face previously erased for a bad vertex at a corner split
                continue;
            }
            RemoveSplitVertex(mesh, overlay, v); // this results in non-convex faces
        }
    }

    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        tEdge e = edgesToBreak[i];
        if(e.entryIsDead())
        {
            continue;
        }
        assertD(CountEdgesAround(e) >= 3);
        if(CountEdgesAround(e) != 3)
        {
            TriangulateFace_NonConvex(mesh, e.face()); // overlay is no longer valid after this point
        }
    }

    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        tEdge e = edgesToBreak[i];
        if(e.entryIsDead())
        {
            continue;
        }
        assertD(CountEdgesAround(e) == 3);
        assertD(GetEdgeLine(e.prev()).side(GetVertexPoint(e.next())) == SIDE_RIGHT);
//        if(GetEdgeLine(e.prev()).side(GetVertexPoint(e.next())) != SIDE_RIGHT)
//        {
//            tEdge eNext = e.next();
//            *eNext.vertex() = *e.vertex();
//
//            assertF(ValidateMeshConnections(mesh));
//
//            mesh.collapseEdge(e);
//
//            assertF(ValidateMeshConnections(mesh));
//
//            if(CountEdgesAround(eNext) == 2)
//            {
//                if(eNext.twin().valid())
//                {
//                    mesh.joinFaces(eNext);
//                }
//                else
//                {
//                    mesh.eraseFace(eNext.face());
//                }
//            }
//            edgesToBreak[i].clear();
//        }
    }

    AddExternalFaces(mesh);
}

static bool
PortalAffected(
        tEdge e,
        const tPoint* points
        )
{
    if(e.entryIsDead())
    {
        return true;
    }
    if(GetVertexPoint(e) != points[0] || GetVertexPoint(e.next()) != points[1])
    {
        return true;
    }
    if(!e.twin().face()->isExternal())
    {
        return true;
    }
    return false;
}
static bool
PortalAffected_InsideEdge(
        tEdge e,
        const tPoint* points
        )
{
    if(e.entryIsDead())
    {
        return true;
    }
    if(GetVertexPoint(e) != points[0] || GetVertexPoint(e.next()) != points[1])
    {
        return true;
    }
    return false;
}

bool
CutExternalShape(
        tMesh& mesh,
        cExternalShapeInfo& shape,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack
        )
{
    tFace& shapeExternalFace = shape._shapeExternalFace;
    const std::vector<tEdge>& edgeBehindPortal = shape._behindPortalEdges;
    const std::vector<tPoint>& edgeBehindPortalPoints = shape._behindPortalEdgePoints;

    bool portalsAffected = false;
    tSigned32 i;
    for(i = 0; i != edgeBehindPortal.size(); ++i)
    {
        tEdge e = edgeBehindPortal[i];
        if(e.valid())
        {
            if(PortalAffected(e, &edgeBehindPortalPoints[i * 2]))
            {
                portalsAffected = true;
                break;
            }
            tEdge insideE = shape._insideEdgeOnTransformed[i];
            assertD(insideE.valid());
            if(PortalAffected_InsideEdge(insideE, &edgeBehindPortalPoints[i * 2]))
            {
                portalsAffected = true;
                break;
            }
        }
    }
    tEdge eShape = tMesh::beginEdge(shapeExternalFace);
    for(i = 0; i != edgeBehindPortal.size(); ++i, --eShape)
    {
        tEdge e = edgeBehindPortal[i];
        if(!e.valid())
        {
            continue;
        }
        if(PortalAffected(e, &edgeBehindPortalPoints[i * 2]))
        {
            continue;
        }
        tEdge insideE = shape._insideEdgeOnTransformed[i];
        if(PortalAffected_InsideEdge(insideE, &edgeBehindPortalPoints[i * 2]))
        {
            continue;
        }
        CutExternalShape(mesh, e, insideE, eShape, shape._portalResolvesToTerrain[i], faceIsOnTerrainCallBack, shape._portalFaceIndices[i]);
    }
    return portalsAffected;
}
