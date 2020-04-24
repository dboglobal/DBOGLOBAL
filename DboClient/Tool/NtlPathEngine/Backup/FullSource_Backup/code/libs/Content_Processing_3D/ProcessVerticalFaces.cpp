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
#include "ProcessVerticalFaces.h"
#include "tMesh_Dynamic.h"
#include "Plane.h"
#include "ValidateFaceDynamic.h"
#include "TriangulateDynamic.h"
#include "common/TemplateMesh/Helper.h"
#include "i_pathengine.h"

bool
IsVertical(tFace_Dynamic f, const std::vector<cPlane>& splitters)
{
    return splitters[f->_splitterIndex]._normal.z().isZero();
}

static bool
IsVertical(tEdge_Dynamic e, const cBSPVertices& vertices)
{
    const cDynamicPoint3& p1 = e.vertex()->dynamicPoint(vertices);
    const cDynamicPoint3& p2 = e.next().vertex()->dynamicPoint(vertices);
    int xComparison = p1.x().compare(p2.x());
    int yComparison = p1.y().compare(p2.y());
    return xComparison == 0 && yComparison == 0;
}

static bool
FaceNeedsToBeSplit_Component(
        const cBSPVertices& vertices,
        tEdge_Dynamic e1,
        int comparisonC1_C2,
        const cDynamicFraction& c1,
        const cDynamicFraction& c2,
        const cDynamicFraction& c3,
        tEdge_Dynamic& toSplit,
        cDynamicFraction& splitZ
        )
{
    int comparisonC2_C3 = c2.compare(c3);
    if(comparisonC2_C3 == 0)
    {
        return false;
    }
    const cDynamicFraction *start, *mid, *end;
    if(comparisonC2_C3 == comparisonC1_C2)
    {
        toSplit = e1.prev();
        start = &c3;
        mid = &c2;
        end = &c1;
    }
    else
    {
        int comparisonC1_C3 = c1.compare(c3);
        if(comparisonC1_C3 == 0)
        {
            return false;
        }
        if(comparisonC1_C3 == comparisonC1_C2)
        {
            toSplit = e1;
            start = &c1;
            mid = &c3;
            end = &c2;
        }
        else
        {
            toSplit = e1.next();
            start = &c2;
            mid = &c1;
            end = &c3;
        }
    }
    cDynamicFraction zRatio(*mid);
    zRatio -= *start;
    cDynamicFraction divisor(*end);
    divisor -= *start;
    zRatio /= divisor;
    assertD(!zRatio.isNegative());
    assertD(!zRatio.isZero());
    splitZ = toSplit.next().vertex()->dynamicPoint(vertices).z();
    splitZ -= toSplit.vertex()->dynamicPoint(vertices).z();
    splitZ *= zRatio;
    splitZ += toSplit.vertex()->dynamicPoint(vertices).z();
    return true;
}

static bool
FaceNeedsToBeSplit(
        cBSPVertices& vertices,
        tFace_Dynamic f,
        tEdge_Dynamic& toSplit,
        cDynamicFraction& splitZ
        )
{
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    assertD(CountEdgesAround(e) == 3);
    const cDynamicPoint3& p1 = e.vertex()->dynamicPoint(vertices);
    const cDynamicPoint3& p2 = e.next().vertex()->dynamicPoint(vertices);
    int xComparison = p1.x().compare(p2.x());
    int yComparison = p1.y().compare(p2.y());
    if(xComparison == 0 && yComparison == 0)
    {
        return false;
    }
    const cDynamicPoint3& p3 = e.prev().vertex()->dynamicPoint(vertices);
    if(xComparison == 0)
    {
        return FaceNeedsToBeSplit_Component(vertices, e, yComparison, p1.y(), p2.y(), p3.y(), toSplit, splitZ);
    }
    return FaceNeedsToBeSplit_Component(vertices, e, xComparison, p1.x(), p2.x(), p3.x(), toSplit, splitZ);    
}

static bool
SplitFaceIfNecessary(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        tFace_Dynamic f
        )
{
    tEdge_Dynamic toSplit;
    cDynamicPoint3 splitPoint;
    if(!FaceNeedsToBeSplit(vertices, f, toSplit, splitPoint.z()))
    {
        return false;
    }

    const cDynamicPoint3& midP = toSplit.prev().vertex()->dynamicPoint(vertices);
    splitPoint.x() = midP.x();
    splitPoint.y() = midP.y();

//    bool propogate = toSplit.twin().valid() && IsVertical(toSplit.twin().face(), splitters);

  // do the split

    // note that we don't care about pair indices here, because this is after edge pair connection
    mesh.splitEdge(toSplit);
    *toSplit.next() = *toSplit;
    toSplit.next().vertex()->initForSplitPoint(vertices, splitPoint);

    mesh.splitFace(toSplit.next(), toSplit.prev());
    assertF(ValidateFaceDynamic(vertices, splitters[toSplit.face()->_splitterIndex], toSplit.face()));
    assertF(ValidateFaceDynamic(vertices, splitters[toSplit.next().twin().face()->_splitterIndex], toSplit.next().twin().face()));

  // triangulate face on the other side of the split edge

    if(toSplit.twin().valid())
    {
        tEdge_Dynamic e = toSplit.twin();
        *e = *e.prev(); // ensure _originalIndex remains valid, for inline vert removal
        bool mayNeedToPropagate = IsVertical(e.face(), splitters);
        mesh.splitFace(e, e.next().next());
        assertF(ValidateFaceDynamic(vertices, splitters[e.face()->_splitterIndex], e.face()));
        assertF(ValidateFaceDynamic(vertices, splitters[e.twin().face()->_splitterIndex], e.twin().face()));
        return mayNeedToPropagate;
    }
    return false;
}

static void
RemoveNonVerticalEdgesAndBreakVerticalEdges_Recursive(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        tEdge_Dynamic firstE,
        tEdge_Dynamic lastE
        )
{
    do
    {
        tEdge_Dynamic e = firstE;
        ++firstE;
        if(e.twin().valid() && IsVertical(e.twin().face(), splitters))
        {
            if(IsVertical(e, vertices))
            {
                mesh.breakEdge(e);
            }
            else
            {
                tEdge_Dynamic eBefore = e;
                --eBefore;
                tFace_Dynamic fToBeKept = e.face();
                mesh.joinFaces(e);
                assertD(firstE.face() == fToBeKept); // mesh operation guarantees this
                RemoveNonVerticalEdgesAndBreakVerticalEdges_Recursive(vertices, mesh, splitters, eBefore.next(), firstE.prev());
            }
        }
    }
    while(firstE != lastE.next());
}
static void
RemoveNonVerticalEdgesAndBreakVerticalEdges(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        tFace_Dynamic f
        )
{
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    RemoveNonVerticalEdgesAndBreakVerticalEdges_Recursive(vertices, mesh, splitters, e, e.prev());
}

static void
RemoveInlineVerts(
        tMesh_Dynamic& mesh,
        tFace_Dynamic f
        )
{
    bool faceAffected;
    do
    {
        faceAffected = false;
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        do
        {
            if(!e.twin().valid() && !e.next().twin().valid())
            {
                mesh.unsplitEdge(e);
                faceAffected = true;
                break;
            }
        }
        while(e.toNextInFace());
    }
    while(faceAffected);
}

void
GetVerticalFaceConnections(
        tFace_Dynamic f,
        tEdge_Dynamic& e1,
        tEdge_Dynamic& e2
        )
{
    e1 = tMesh_Dynamic::beginEdge(f);
    while(!e1.twin().valid())
    {
        e1.toNextInFace();
        assertD(e1.valid());
    }
    e2 = e1;
    e2.toNextInFace();
    assertD(e2.valid());
    while(!e2.twin().valid())
    {
        e2.toNextInFace();
        assertD(e2.valid());
    }
}

static void
ApplyStepConstraint(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tFace_Dynamic& f,
        const cDynamicFraction& maxStepHeight
        )
{
    assertD(!maxStepHeight.isNegative());

  // get top and bottom edges

    tEdge_Dynamic e1, e2;
    GetVerticalFaceConnections(f, e1, e2);

    const cDynamicPoint3* start1 = &e1.vertex()->dynamicPoint(vertices);
    const cDynamicPoint3* end1 = &e1.next().vertex()->dynamicPoint(vertices);
    const cDynamicPoint3* start2 = &e2.vertex()->dynamicPoint(vertices);
    const cDynamicPoint3* end2 = &e2.next().vertex()->dynamicPoint(vertices);
    assertD(start1->x() == end2->x());
    assertD(start1->y() == end2->y());
    assertD(start2->x() == end1->x());
    assertD(start2->y() == end1->y());

  // get start and end heights of polygon

    cDynamicFraction startHeight(start1->z());
    startHeight -= end2->z();
    cDynamicFraction endHeight(end1->z());
    endHeight -= start2->z();
    if(startHeight.isNegative() || endHeight.isNegative())
    {
        startHeight.negate();
        endHeight.negate();
    }
    assertD(!startHeight.isNegative());
    assertD(!endHeight.isNegative());

  // apply the constraint

    int startComparison = maxStepHeight.compare(startHeight);
    int endComparison = maxStepHeight.compare(endHeight);
    
    if(startComparison != 1 && endComparison != 1)
    {
        // already satisfies constraint
        return;
    }

    if(startComparison != -1 && endComparison != -1)
    {
        // discard whole face
        mesh.eraseFace(f);
        return;
    }

    if(startComparison == 1)
    {
        // normalise to situation where endHeight is larger
        startHeight.swap(endHeight);
        std::swap(e1, e2);
        std::swap(start1, start2);
        std::swap(end1, end2);
    }

    // work out coords for split points

    cDynamicFraction splitRatio(maxStepHeight);
    splitRatio -= startHeight;
    {
        cDynamicFraction divisor(endHeight);
        divisor -= startHeight;
        splitRatio /= divisor;
    }
    
    cDynamicFraction splitX(end1->x());
    splitX -= start1->x();
    splitX *= splitRatio;
    splitX += start1->x();
    cDynamicFraction splitY(end1->y());
    splitY -= start1->y();
    splitY *= splitRatio;
    splitY += start1->y();
    cDynamicFraction splitZ1(end1->z());
    splitZ1 -= start1->z();
    splitZ1 *= splitRatio;
    splitZ1 += start1->z();
    cDynamicFraction splitZ2(start2->z());
    splitZ2 -= end2->z();
    splitZ2 *= splitRatio;
    splitZ2 += end2->z();

    assertD(splitZ2 - splitZ1 == maxStepHeight || splitZ1 - splitZ2 == maxStepHeight);

    cDynamicPoint3 splitPoint1;
    splitPoint1.x() = splitX;
    splitPoint1.y() = splitY;
    splitPoint1.z() = splitZ1;
    cDynamicPoint3 splitPoint2;
    splitPoint2.x() = splitX;
    splitPoint2.y() = splitY;
    splitPoint2.z() = splitZ2;

    // split the face

    mesh.splitEdge(e1);
    *e1.next() = *e1;
    *e1.twin() = *e1.next().twin();
    ++e1;
    e1.vertex()->initForSplitPoint(vertices, splitPoint1);
    mesh.splitEdge(e2);
    *e2.next() = *e2;
    *e2.twin() = *e2.next().twin();
    ++e2;
    e2.vertex()->initForSplitPoint(vertices, splitPoint2);
    mesh.splitFace(e1, e2);

    // triangulate ground faces either side

    mesh.splitFace(e1.twin().next(), e1.twin().prev());
    mesh.splitFace(e2.twin().next(), e2.twin().prev());

    // delete the part that exceeds step constraint
    
    mesh.eraseFace(e1.face());
}

void
ProcessVerticalFaces(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        const std::vector<int>& projectionTypes,
        const cDynamicFraction& maxStepHeight,
        iProgressCallBack* progressCallBack
        )
{
  // store original edge indices - these will be used to remove spurious inline vertices,
  // after removal of faces for step height constraint

    {
        tEdge_Dynamic e = mesh.beginEdge();
        do
        {
            e->_originalIndex = e.index();
        }
        while(e.toNextInMesh());
    }

  // split up and down from vertices in vertical faces, wherever possible

    {
        bool reloop;
        bool firstLoop = true;
        do
        {
            reloop = false;
            mesh.consolidateFaces();
            tSigned32 i;
            tSigned32 startingFaces = mesh.faces();
            for(i = 0; i != startingFaces; ++i)
            {
                tFace_Dynamic f = mesh.faceAt(i);
                if(!IsVertical(f, splitters) || f->wasDiscarded())
                {
                    continue;
                }
                bool mayNeedToPropagate = SplitFaceIfNecessary(vertices, mesh, splitters, f);
                if(mayNeedToPropagate)
                {
                    reloop = true;
                }
                if(progressCallBack && firstLoop)
                {
                    progressCallBack->updateProgressL("Splitting vertical faces", i, startingFaces);
                }
            }
            firstLoop = false;
        }
        while(reloop);
    }

  // remove non-vertical edges and break vertical edges

    {
        mesh.consolidateFaces();
        tSigned32 i;
        tSigned32 startingFaces = mesh.faces();
        for(i = 0; i != startingFaces; ++i)
        {
            tFace_Dynamic f = mesh.faceAt(i);
            if(f.entryIsDead())
            {
                continue;
            }
            if(!IsVertical(f, splitters) || f->wasDiscarded())
            {
                continue;
            }
            RemoveNonVerticalEdgesAndBreakVerticalEdges(vertices, mesh, splitters, f);
            if(progressCallBack)
            {
                progressCallBack->updateProgressL("Removing non-vertical edges", i, startingFaces);
            }
        }
    }

  // remove any faces that don't connect to two others
  // (i.e. faces that don't connect two ground pieces)
    {
        tFace_Dynamic f = mesh.beginFace();
        do
        {
            tFace_Dynamic toProcess = f;
            ++f;
            if(!IsVertical(toProcess, splitters) || toProcess->wasDiscarded())
            {
                continue;
            }
            tSigned32 connections = 0;
            tEdge_Dynamic e = tMesh_Dynamic::beginEdge(toProcess);
            do
            {
                if(e.twin().valid())
                {
                    connections++;
                }
            }
            while(e.toNextInFace());
            assertD(connections >= 0 && connections <= 2);
            if(connections != 2)
            {
                mesh.eraseFace(toProcess);
            }
        }
        while(f.valid());
    }

    if(mesh.empty())
    {
        return;
    }

  // remove inline verts
  // note that we can assume that any two disconnected edges in sequence are inline
  // because faces not connecting to two ground pieces have been stripped, above

    {
        tFace_Dynamic f = mesh.beginFace();
        do
        {
            if(!IsVertical(f, splitters) || f->wasDiscarded())
            {
                continue;
            }
            RemoveInlineVerts(mesh, f);
        }
        while((++f).valid());
    }

  // apply step constraint

    {
        tSigned32 i;
        mesh.consolidateFaces();
        tSigned32 startingFaces = mesh.faces();
        for(i = 0; i != startingFaces; ++i)
        {
            tFace_Dynamic f = mesh.faceAt(i);
            if(!IsVertical(f, splitters) || f->wasDiscarded())
            {
                continue;
            }
            ApplyStepConstraint(vertices, mesh, f, maxStepHeight);
        }
    }

  // remove spurious inline edge verts on ground, resulting from the previous face removal step

//    return;//...
    {
        mesh.consolidateVertices();
        tSigned32 vertsAtStart = mesh.vertices();
        tSigned32 i;
        for(i = 0; i != vertsAtStart; ++i)
        {
            tVertex_Dynamic v = mesh.vertAt(i);
            assertD(!v.entryIsDead());
            if(v.isInternalVertex())
            {
                continue;
            }
            tEdge_Dynamic eAfter = tMesh_Dynamic::firstEdge(v);
            tEdge_Dynamic eBefore = tMesh_Dynamic::lastEdge(v).prev();
            if(eAfter->_originalIndex != eBefore->_originalIndex)
            {
                continue;
            }
            assertD(GetEdgeAxis3(vertices, eBefore).inLine(GetEdgeAxis3(vertices, eAfter)));
            assertD(GetEdgeAxis3(vertices, eBefore).direction_KnownToBeInline(GetEdgeAxis3(vertices, eAfter)) == DIRECTION_FORWARD);
            tSigned32 splitterIndex = eAfter.face()->_splitterIndex;
            assertD(splitterIndex >= 0);
            assertD(splitterIndex == eBefore.face()->_splitterIndex);
            tSigned32 vertsBefore = mesh.vertices();
            RemoveInlineEdgeVertex_NoExternalFaces_Dynamic(mesh, v, vertices, projectionTypes[splitterIndex]);
            assertD(mesh.vertices() == vertsBefore - 1);
            assertD(v.entryIsDead());
        }
    }

// note that remaining vertical faces which are quads are left this way, because these will subsequently be collapsed during approximation
}
