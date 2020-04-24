//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "OperandsFromFaceVertex.h"
#include "tMesh_Simple3D.h"
#include "FacingDirection.h"
#include "AddVolumeToVerticalFace.h"
#include "platform_common/CoordinateRanges.h"
#include "common/TemplateMesh/Helper.h"
#include "i_pathengine.h"
#include "common/interface/Error.h"

typedef tMesh_Simple3D tMesh;
typedef tFace_Simple3D tFace;
typedef tEdge_Simple3D tEdge;
typedef tVertex_Simple3D tVertex;

typedef tMesh_Simple3D::tFaceData tFaceData;
typedef tMesh_Simple3D::tEdgeData tEdgeData;
typedef tMesh_Simple3D::tVertexData tVertexData;

static bool
IsAboveTest(tEdge_Simple3D e1, tEdge_Simple3D e2, tEdge_Simple3D eTest)
{
    tPoint p1 = GetVertexPoint(e1);
    tPoint p2 = GetVertexPoint(e2);
    tPoint pTest = GetVertexPoint(eTest);
    assertD((p2 - p1).side(p2 - p1) == SIDE_CENTRE);
    tSigned32 sExtents, sTest;
    if(p1.getX() == p2.getX())
    {
        sExtents = p2.getY() - p1.getY();
        assertD(sExtents != 0);
        sTest = pTest.getY() - p1.getY();
    }
    else
    {
        sExtents = p2.getX() - p1.getX();
        sTest = pTest.getX() - p1.getX();
    }
    if(sExtents < 0)
    {
        sExtents = -sExtents;
        sTest = -sTest;
    }
    tSigned32 zExtents = e2.vertex()->_p.getZ() - e1.vertex()->_p.getZ();
    tSigned32 zTest = eTest.vertex()->_p.getZ() - e1.vertex()->_p.getZ();
    tPoint::tDistance zExtents_MultipliedOut, zTest_MultipliedOut;
    tMultiplier::multiply(zTest, sExtents, zTest_MultipliedOut);
    tMultiplier::multiply(zExtents, sTest, zExtents_MultipliedOut);
    int comparison = tMultiplier::compare(zExtents_MultipliedOut, zTest_MultipliedOut);
    assertD(comparison != 0); // not expecting inline vertices during vertical face extrusion
    return comparison == 1;
}

static void
AdjustAndInsertIfNoExistingVerticalEdge(tMesh_Simple3D& mesh, tEdge_Simple3D& e)
{
    if(GetVertexPoint(e) == GetVertexPoint(e.prev()))
    {
    // already just after a vertical edge
        return;
    }
    if(GetVertexPoint(e) == GetVertexPoint(e.next()))
    {
        ++e;
        return;
    }
    // insert
    mesh.splitEdge(e.prev(), *e.vertex(), *e, *e);
}

static void
ExtrudeVerticalFace(tMesh_Simple3D& toExtrude, const tPoint3& extrudeOffset)
{
// find the furthest apart pair of vertices in x and y
// (this is the pair of vertices at which to split the face)

    tEdge_Simple3D e1, e2; // candidates
    tEdge_Simple3D bestE1, bestE2; // best so far

    bestE1 = toExtrude.beginEdge();
    bestE2 = bestE1.next();
    tPoint::tDistance bestDistance = (GetVertexPoint(bestE2) - GetVertexPoint(bestE1)).lengthSquared();

    e1 = toExtrude.beginEdge();
    do
    {
        e2 = e1;
        while(e2.toNextInMesh())
        {
            tPoint::tDistance distance = (GetVertexPoint(e2) - GetVertexPoint(e1)).lengthSquared();
            if(distance > bestDistance)
            {
                bestE1 = e1;
                bestE2 = e2;
                bestDistance = distance;
            }
        }
    }
    while(e1.toNextInMesh());

    e1 = bestE1;
    e2 = bestE2;

// add extra verts where there are not already vertical edges
// and adjust e1 and e2 to be exactly at start of upper and lower sequences respectively

    if(e2 != e1.next())
    {
    // test e1.next() to determine which sequence is upper and which is lower
        if(!IsAboveTest(e1, e2, e1.next()))
        {
            std::swap(e1, e2);
        }
    }
    else
    {
    // test e2.next() to determine which sequence is upper and which is lower
        if(IsAboveTest(e1, e2, e2.next()))
        {
            std::swap(e1, e2);
        }
    }

    AdjustAndInsertIfNoExistingVerticalEdge(toExtrude, e1);
    AdjustAndInsertIfNoExistingVerticalEdge(toExtrude, e2);

// go ahead and do the extrude

    tEdge_Simple3D e = e2;    
    do
    {
        e.vertex()->_p += extrudeOffset;
        ++e;
    }
    while(e != e1);
}

static void
FlipFace(tFace_Simple3D face)
{
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(face);
    assertD(CountEdgesAround(e) == 3);
    tMesh_Simple3D::tVertexData v = *e.next().vertex();
    *e.next().vertex() = *e.vertex();
    *e.vertex() = v;
}

static tMesh_Simple3D*
ExtrudedOperandFromFace(const iFaceVertexMesh& faceVertex, tSigned32 faceIndex, const tPoint3& extrudeOffset)
{
    tMesh_Simple3D* result = new tMesh_Simple3D;
    tEdgeData edges[4];
    tVertexData vertices[4];
    tSigned32 i;
    tEdge e;

  // build first face (which will be the top face of the result, if not vertical)

    for(i = 0; i != 3; ++i)
    {
        tSigned32 v = faceVertex.vertexIndex(faceIndex, i);
        tPoint3 p(faceVertex.vertexX(v), faceVertex.vertexY(v), static_cast<tSigned32>(faceVertex.vertexZ(v)), WORLD_RANGE);
        vertices[i] = tVertexData(p);
    }
    tFace topFace = result->addFace(tFaceData(), 3, edges, vertices);

    eSide facingDirection = FacingDirection(topFace);

    if(facingDirection == SIDE_CENTRE)
    {
      // vertical face
        ExtrudeVerticalFace(*result, extrudeOffset);
        AddVolumeToVerticalFace(*result);
        result->consolidate();
        return result;
    }

    if(facingDirection == SIDE_LEFT)
    {
      // top face is downward facing
        FlipFace(topFace);
    }

  // build bottom face
    e = tMesh::beginEdge(topFace);
    for(i = 0; i != 3; ++i)
    {
        vertices[i] = *e.vertex();
        vertices[i]._p += extrudeOffset;
        --e;
    }
    tFace bottomFace = result->addFace(tFaceData(), 3, edges, vertices);

  // build side faces, connect to top and bottom
    e = tMesh::beginEdge(topFace);
    tEdge bottomE = tMesh::beginEdge(bottomFace).prev();
    do
    {
        vertices[0] = *e.next().vertex();
        vertices[1] = *e.vertex();
        vertices[2] = vertices[1];
        vertices[2]._p += extrudeOffset;
        vertices[3] = vertices[0];
        vertices[3]._p += extrudeOffset;
        tFace sideFace = result->addFace(tFaceData(), 4, edges, vertices);
        result->connectEdges(e, tMesh::beginEdge(sideFace));
        result->connectEdges(bottomE, tMesh::beginEdge(sideFace).next().next());
        --bottomE;
    }
    while(e.toNextInFace());

  // connect side faces together
    e = tMesh::beginEdge(topFace);
    do
    {
        result->connectEdges(e.twin().next(), e.prev().twin().prev());
    }
    while(e.toNextInFace());

    result->consolidate();
    return result;
}

static bool
VertsIdentical(const iFaceVertexMesh& faceVertex, tSigned32 v1, tSigned32 v2)
{
    return faceVertex.vertexX(v1) == faceVertex.vertexX(v2)
            && faceVertex.vertexY(v1) == faceVertex.vertexY(v2)
            && static_cast<tSigned32>(faceVertex.vertexZ(v1)) == static_cast<tSigned32>(faceVertex.vertexZ(v2))
            ;
}
static bool
VertIsOutsideRange(const iFaceVertexMesh& faceVertex, tSigned32 v)
{
    return faceVertex.vertexX(v) < -WORLD_RANGE
        || faceVertex.vertexX(v) > WORLD_RANGE
        || faceVertex.vertexY(v) < -WORLD_RANGE
        || faceVertex.vertexY(v) > WORLD_RANGE
        || static_cast<tSigned32>(faceVertex.vertexZ(v)) < -WORLD_RANGE
        || static_cast<tSigned32>(faceVertex.vertexY(v)) > WORLD_RANGE
        ;
}
static bool
FaceIsOutsideRange(const iFaceVertexMesh& faceVertex, tSigned32 f)
{
    return VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 0))
        || VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 1))
        || VertIsOutsideRange(faceVertex, faceVertex.vertexIndex(f, 2))
        ;
}
static bool
FaceHasArea(const iFaceVertexMesh& faceVertex, tSigned32 f)
{
    tSigned32 v0 = faceVertex.vertexIndex(f, 0);
    tSigned32 v1 = faceVertex.vertexIndex(f, 1);
    tSigned32 v2 = faceVertex.vertexIndex(f, 2);
    if(VertsIdentical(faceVertex, v0, v1))
    {
        return false;
    }
    if(VertsIdentical(faceVertex, v0, v2))
    {
        return false;
    }
//    if(VertsIdentical(faceVertex, v1, v2))
//    {
//        return false;
//    }
    tPoint3 p0(faceVertex.vertexX(v0), faceVertex.vertexY(v0), static_cast<tSigned32>(faceVertex.vertexZ(v0)), WORLD_RANGE);
    tPoint3 p1(faceVertex.vertexX(v1), faceVertex.vertexY(v1), static_cast<tSigned32>(faceVertex.vertexZ(v1)), WORLD_RANGE);
    tPoint3 p2(faceVertex.vertexX(v2), faceVertex.vertexY(v2), static_cast<tSigned32>(faceVertex.vertexZ(v2)), WORLD_RANGE);
    tPoint3 axis = p1 - p0;
    if(axis.inLine(p2 - p0))
    {
        return false;
    }
    return true;
}

void
ExtrudedOperandsFromFaceVertex(
        const iFaceVertexMesh& faceVertex, const tPoint3& extrudeOffset,
        tSigned32 sourceMeshIndex,
        std::vector<tMesh_Simple3D*>& appendTo,
        std::vector<tSigned32>& meshAndFaceLookup
        )
{
    bool reportedFaceOutsideRange = false;
    tSigned32 i;
    for(i = 0; i != faceVertex.faces(); ++i)
    {
        if(FaceIsOutsideRange(faceVertex, i))
        {
            if(!reportedFaceOutsideRange)
            {
                Error("NonFatal", "OperandsFromFaceVertex() - Mesh contains one or more faces which are outside the supported coordinate range. These will be ignored.");
                reportedFaceOutsideRange = true;
            }
            continue;
        }
        if(!FaceHasArea(faceVertex, i))
        {
            continue;
        }
        appendTo.push_back(ExtrudedOperandFromFace(faceVertex, i, extrudeOffset));
        meshAndFaceLookup.push_back(sourceMeshIndex);
        meshAndFaceLookup.push_back(i);
    }
}
