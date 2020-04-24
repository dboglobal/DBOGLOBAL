//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/AddInternalVertices.h"
//#include "interface/ObstacleInfo.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "platform_common/PointMultiplication.h"

static void
AddInternalVertex(cMeshWithTrackedTransform<tMesh>& mesh, tFace f, const tPoint& p, tMesh::tVertexData vertexData)
{
    cMeshElement<tMesh> originalElement = ElementForPointInOverlappingFace<tMesh, tPoint>(f, p);
    cMeshElement<tMesh> el = mesh.translate(originalElement, cPoint_PointLike(p));
    if(el.type() == bMeshElement::VERTEX)
    {
        return;
    }
    if(el.type() == bMeshElement::EDGE)
    {
        if(el.asEdge().twin().face()->isExternal())
        {
            return;
        }
        AddVertexBySplittingEdge(mesh, el.asEdge(), vertexData);
    }
    else
    {
        AddVertexInsideFace(mesh, el.asFace(), vertexData);
    }        
}

void
AddInternalVertex(cMeshWithTrackedTransform<tMesh>& mesh, tFace f, const tPoint& p)
{
    AddInternalVertex(mesh, f, p, tMesh::tVertexData(p));
}

static tPoint
OffsetOutsideCorner(const tPoint& axisBefore, const tPoint& axisAfter)
{
    tPoint candidate;
    candidate = tPoint(0, 1, 1);
    if(axisBefore.side(candidate) == SIDE_LEFT || axisAfter.side(candidate) == SIDE_LEFT)
    {
        return candidate;
    }
    candidate = tPoint(1, 0, 1);
    if(axisBefore.side(candidate) == SIDE_LEFT || axisAfter.side(candidate) == SIDE_LEFT)
    {
        return candidate;
    }
    candidate = tPoint(0, -1, 1);
    if(axisBefore.side(candidate) == SIDE_LEFT || axisAfter.side(candidate) == SIDE_LEFT)
    {
        return candidate;
    }
    candidate = tPoint(-1, 0, 1);
    assertD(axisBefore.side(candidate) == SIDE_LEFT || axisAfter.side(candidate) == SIDE_LEFT);
    return candidate;
}

static void
AttemptToOffsetOutsideCorner(const tPoint& axisBefore, const tPoint& axisAfter,
                             tFace& f, tPoint& p)
{
    tPoint offsetP = p + OffsetOutsideCorner(axisBefore, axisAfter);
    tFace offsetF = f;
    cPoint_PointLike target(offsetP);
    tEdge edgeCrossed;
    while(TraverseTowardsTarget_SI<tMesh>(offsetF, tLine(p, offsetP), target, edgeCrossed))
    {
    }
    if(!offsetF->isExternal())
    {
        f = offsetF;
        p = offsetP;
    }
}

void
AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
                  const tMeshCircuits& circuits, const int frequency)
{
    assertD(frequency > 0);
    int frequencyCount = 0;
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            const cCircuitElement* startE = (*j)->getStartElement();
            const cCircuitElement* e = startE;
            do
            {
                if(!e->enters() && e->getPrev()->refLine().side(e->refLine()) == SIDE_RIGHT)
                {
                    tIntersection intersection;
                    e->getIntersectionForCornerAtStart(intersection);
                    tPoint p;
                    if(intersection.isOnIntegerGrid(p))
                    {
                        if(frequencyCount == 0)
                        {
                            tFace f = e->faceCrossed(0);
                            AttemptToOffsetOutsideCorner(
                                            e->getPrev()->refLine().axis(),
                                            e->refLine().axis(),
                                            f, p);
                            AddInternalVertex(mesh, f, p);
                            frequencyCount = frequency;
                        }
                        frequencyCount--;
                    }
                }
                e = e->getNext();
            }
            while(e != startE);
        }
    }
}

static bool
OffsetInsideCorner(const tPoint& axisBefore, const tPoint& axisAfter, tPoint& result)
{
    result = tPoint(0, 1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(1, 1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(1, 0, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(1, -1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(0, -1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(-1, -1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(-1, 0, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    result = tPoint(-1, 1, 1);
    if(axisBefore.side(result) == SIDE_RIGHT && axisAfter.side(result) == SIDE_RIGHT)
    {
        return true;
    }
    return false;
}

static bool
AttemptToOffsetInsideCorner(const tPoint& axisBefore, const tPoint& axisAfter,
                             tFace& f, tPoint& p)
{
    tPoint offsetV;
    if(!OffsetInsideCorner(axisBefore, axisAfter, offsetV))
    {
        return false;
    }
    tPoint offsetP = p + offsetV;
    tFace offsetF = f;
    cPoint_PointLike target(offsetP);
    tEdge edgeCrossed;
    while(TraverseTowardsTarget_SI<tMesh>(offsetF, tLine(p, offsetP), target, edgeCrossed))
    {
    }
    if(!offsetF->isExternal())
    {
        f = offsetF;
        p = offsetP;
        return true;
    }
    return false;
}

void
AddInternalVerticesInsideObstructions(
                cMeshWithTrackedTransform<tMesh>& mesh,
                const tMeshCircuits& circuits, const int frequency
                )
{
    tSigned32 addedCount = 0; // just for debugging
    assertD(frequency > 0);
    int frequencyCount = 0;
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            const cCircuitElement* startE = (*j)->getStartElement();
            const cCircuitElement* e = startE;
            do
            {
                if(!e->enters() && e->getPrev()->refLine().side(e->refLine()) == SIDE_RIGHT)
                {
                    tIntersection intersection;
                    e->getIntersectionForCornerAtStart(intersection);
                    tPoint p;
                    if(intersection.isOnIntegerGrid(p))
                    {
                        if(frequencyCount == 0)
                        {
                            frequencyCount = frequency;

                            tFace f = e->faceCrossed(0);

                            if(AttemptToOffsetInsideCorner(
                                            e->getPrev()->refLine().axis(),
                                            e->refLine().axis(),
                                            f, p))
                            {
//                                assertD(collisionTester.testPointCollision(f, p));
                                AddInternalVertex(mesh, f, p);
                                addedCount++;
                            }
                        }
                        frequencyCount--;
                    }
                }
                e = e->getNext();
            }
            while(e != startE);
        }
    }

}

void
AddInternalVertices(cMeshWithTrackedTransform<tMesh>& mesh,
                  const std::vector<cInternalPosition>& positions,
                  const std::vector<tMesh::tVertexData>& vertexData
                  )
{
    assertD(positions.size() == vertexData.size());
    tSigned32 i;
    for(i = 0; i != SizeL(positions); ++i)
    {
        AddInternalVertex(mesh, positions[i].f, positions[i].p, vertexData[i]);
    }
}
