//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "RemoveInlineExternalVertices.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/SquareRoot.h"

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

static float
DistanceFromInfiniteLine(const tLine& cutLine, const tPoint& testP)
{
    tPoint offset = testP - cutLine.start();
    tPoint::tDistance dp = cutLine.axis().right90().dotProduct(offset);
    tPoint::tDistance lengthSquared = cutLine.axis().lengthSquared();
    float length = SquareRoot(tMultiplier::asFloat(lengthSquared));
    float result = tMultiplier::asFloat(dp) / length;
    if(result < 0.f)
    {
        result = -result;
    }
    return result;
}

static bool
HorizontalMovementOK(const tLine& cutLine, const tPoint& originalPosition, float threshold)
{
    if(cutLine.left90().side(originalPosition) != SIDE_RIGHT)
    {
        return false;
    }
    if(cutLine.reversed().left90().side(originalPosition) != SIDE_RIGHT)
    {
        return false;
    }
    return DistanceFromInfiniteLine(cutLine, originalPosition) <= threshold;
}

static float
InterpolatedHeight(const tLine& cutLine, float startZ, float endZ, const tPoint& p)
{
    tPoint offset = p - cutLine.start();
    tPoint::tDistance lengthSquared = cutLine.axis().lengthSquared();
    tPoint::tDistance dp = cutLine.axis().dotProduct(offset);
    float ratio = tMultiplier::asFloat(dp) / tMultiplier::asFloat(lengthSquared);
    return startZ + (endZ - startZ) * ratio;
}

static bool
VerticalMovementOK(tEdge externalEdge, float rangeStart, float rangeEnd)
{
    tEdge e = externalEdge.twin().next();
    do
    {
        if(e->getZ() < rangeStart || e->getZ() > rangeEnd)
        {
            return false;
        }
        e = e.twin().next();
    }
    while(e != externalEdge);
    return true;
}

static bool
EdgeSequenceCanBeJoined(tEdge first, tEdge last,
                float horizontalThreshold,
                float verticalThreshold,
                iFacesCanBeJoinedCallBack& facesCallBack
                )
{
    assertD(first.face()->isExternal());
    assertD(first.face() == last.face());
    tLine cutLine(GetVertexPoint(first), GetVertexPoint(last.next()));
    float startZ = first.twin().next()->getZ();
    float endZ = last.twin()->getZ();
    while(first != last)
    {
        ++first;
        if(!HorizontalMovementOK(cutLine, GetVertexPoint(first), horizontalThreshold))
        {
            return false;
        }
        float interpolatedHeight = InterpolatedHeight(cutLine, startZ, endZ, GetVertexPoint(first));
        if(!VerticalMovementOK(first, interpolatedHeight - verticalThreshold, interpolatedHeight + verticalThreshold))
        {
            return false;
        }
        tEdge e = first.twin().next();
        while(e != first.prev().twin())
        {
            if(cutLine.side(GetVertexPoint(e.next())) != SIDE_LEFT)
            {
                return false;
            }
            if(!facesCallBack.facesCanBeJoined(e))
            {
                return false;
            }
            e = e.twin().next();
        }
    }
    return true;
}

static tEdge
JoinEdgeSequence(tMesh& mesh, tEdge first, tEdge last)
{
    assertD(first != last);
    if(first.twin().face() == last.twin().face())
    {
      // special case for removing an external edge that would otherwise result in a 2 edge face
        tFace behindF = first.twin().next().twin().face();
        if(behindF->isExternal())
        {
            // pathological case
            return last;
        }
        mesh.unsplitEdge(first);
        mesh.joinFaces(first.twin().next());
        bool success = TriangulateFace_NonConvex<tMesh>(mesh, first.twin().face());
        assertD(success);
        return first;
    }

    tEdge e;

  // join all faces along the inside mesh side of the cut

    e = first;
    while(e != last)
    {
        ++e;
        tEdge e2 = e.twin().next();
        while(e2 != e.prev().twin())
        {
            e2 = e2.twin().next();
            mesh.joinFaces(e2.prev());
        }
    }

  // join the sequence of external edges into a single edge

    while(last != first)
    {
        --last;
        mesh.unsplitEdge(last);
    }

  // and then triangulate the resulting internal polygon

    bool success = TriangulateFace_NonConvex<tMesh>(mesh, first.twin().face());
    assertD(success);

    return last;
}

void
RemoveInlineVerticesAroundExternalFace(
                tMesh& mesh,
                tFace& f,
                float horizontalThreshold,
                float verticalThreshold,
                iFacesCanBeJoinedCallBack& facesCallBack
                )
{
    tEdge veryFirst = tMesh::beginEdge(f);
    tEdge last = veryFirst;
    while(EdgeSequenceCanBeJoined(veryFirst.prev(), last, horizontalThreshold, verticalThreshold, facesCallBack))
    {
        assertD(veryFirst.prev() != last);
        --veryFirst;
    }
    tEdge first = veryFirst;
    do
    {
        while(last.next() != veryFirst && EdgeSequenceCanBeJoined(first, last.next(), horizontalThreshold, verticalThreshold, facesCallBack))
        {
            ++last;
        }
        if(first != last)
        {
            last = JoinEdgeSequence(mesh, first, last);
        }
        ++last;
        first = last;
    }
    while(first != veryFirst);
}

void
RemoveInlineExternalVertices(
                tMesh& mesh,
                float horizontalThreshold,
                float verticalThreshold,
                iFacesCanBeJoinedCallBack& facesCallBack
                )
{
    tFace f = mesh.beginFace();
    do
    {
        ++f;
    }
    while(!f->isExternal());
    do
    {
        RemoveInlineVerticesAroundExternalFace(mesh, f, horizontalThreshold, verticalThreshold, facesCallBack);
    }
    while((++f).valid());
}
