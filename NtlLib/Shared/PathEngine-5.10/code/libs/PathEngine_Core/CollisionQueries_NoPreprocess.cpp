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
#include "interface/CollisionQueries_NoPreprocess.h"
#include "interface/OverlappingPolygon.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/CompoundContextCollisionPreprocess.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "common/Containers/BitVector.h"

static bool
ObstaclesOverlap(cQueryContext& qc, const cLocalisedConvexShape& obstacle1, const cLocalisedConvexShape& obstacle2)
{
    tIntersection intersection;
    if(!obstacle1.poly().overlaps2D(obstacle2.poly(), intersection))
    {
        return false;
    }
    const std::vector<tSigned32>& facesOverlapped = obstacle1.refFacesOverlapped();
    tSigned32 k;
    for(k = 0; k != SizeL(facesOverlapped); ++k)
    {
        if(!obstacle2.overlapsFace(facesOverlapped[k]))
        {
            continue;
        }
        tFace f = qc.refMesh().faceAt(facesOverlapped[k]);
        if(PointIsInFace<tMesh>(f, intersection))
        {
            return true;
        }
    }
    return false;
}

static bool
ShapeCollidesWithObstacles(
        cQueryContext& qc,
        const cLocalisedConvexShape& lcs,
        const cLocalisedConvexShapeSet& obstacles
        )
{
    const std::vector<tSigned32>& facesOverlapped = lcs.refFacesOverlapped();
    cBitVector obstaclesChecked(obstacles.size(), false);
    tSigned32 i;
    for(i = 0; i != SizeL(facesOverlapped); ++i)
    {
        tSigned32 fIndex = facesOverlapped[i];
        {
            tSigned32 obstacleIndex;
            const cLocalisedConvexShape* obstaclePtr;
            cLocalisedConvexShapeSet::tShapesInFaceIterator j;
            obstacles.initShapesInFaceIterator(fIndex, j);
            while(obstacles.nextShapeInFace(j, obstacleIndex, obstaclePtr))
            {
                if(obstaclesChecked[obstacleIndex])
                {
                    continue;
                }
                obstaclesChecked.setBit(obstacleIndex, true);
                if(ObstaclesOverlap(qc, lcs, *obstaclePtr))
                {
                    return true;
                }
            }
        }
    }
    return false;    
}

static bool
ShapeCollidesWithObstacles(
        cQueryContext& qc,
        const cLocalisedConvexShape& lcs,
        const cCompoundContextCollisionPreprocess& obstacles,
        const cAgent* agentToIgnore
        )
{
    const std::vector<tSigned32>& facesOverlapped = lcs.refFacesOverlapped();
    cBitVector obstaclesChecked(obstacles.size(), false);
    tSigned32 i;
    for(i = 0; i != SizeL(facesOverlapped); ++i)
    {
        tSigned32 fIndex = facesOverlapped[i];
        {
            tSigned32 obstacleIndex;
            const cLocalisedConvexShape* obstaclePtr;
            cAgent* agentPtr;
            cCompoundContextCollisionPreprocess::tShapesInFaceIterator j;
            obstacles.initShapesInFaceIterator(fIndex, j);
            while(obstacles.nextShapeInFace(j, obstacleIndex, obstaclePtr, agentPtr))
            {
                if(obstaclesChecked[obstacleIndex])
                {
                    continue;
                }
                obstaclesChecked.setBit(obstacleIndex, true);
                assertD(agentPtr);
                if(agentPtr == agentToIgnore)
                {
                    continue;
                }
                if(ObstaclesOverlap(qc, lcs, *obstaclePtr))
                {
                    return true;
                }
            }
        }
    }
    return false;    
}

static bool
ShapeCollidesWithObstaclesButNotAtStart(
        cQueryContext& qc,
        const cLocalisedConvexShape& lcsStart,
        const cLocalisedConvexShape& lcsExtruded,
        const cCompoundContextCollisionPreprocess& obstacles,
        const cAgent* agentToIgnore
        )
{
    const std::vector<tSigned32>& facesOverlapped = lcsExtruded.refFacesOverlapped();
    cBitVector obstaclesChecked(obstacles.size(), false);
    tSigned32 i;
    for(i = 0; i != SizeL(facesOverlapped); ++i)
    {
        tSigned32 fIndex = facesOverlapped[i];
        {
            tSigned32 obstacleIndex;
            const cLocalisedConvexShape* obstaclePtr;
            cAgent* agentPtr;
            cCompoundContextCollisionPreprocess::tShapesInFaceIterator j;
            obstacles.initShapesInFaceIterator(fIndex, j);
            while(obstacles.nextShapeInFace(j, obstacleIndex, obstaclePtr, agentPtr))
            {
                if(obstaclesChecked[obstacleIndex])
                {
                    continue;
                }
                obstaclesChecked.setBit(obstacleIndex, true);
                assertD(agentPtr);
                if(agentPtr == agentToIgnore)
                {
                    continue;
                }
                if(!ObstaclesOverlap(qc, lcsExtruded, *obstaclePtr))
                {
                    continue;
                }
                if(ObstaclesOverlap(qc, lcsStart, *obstaclePtr))
                {
                    continue;
                }
                return true;
            }
        }
    }
    return false;    
}

bool
TestPointCollision_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& p,
        const cAgent* contextAgentToIgnore
        )
{
    cConvexPoly placedShape(shape);
    placedShape += p.p;
    if(ShapeCollidesWithMeshEdge(qc, p.f, placedShape))
    {
        return true;
    }
    if(baseObstacles || contextObstacles)
    {
        cLocalisedConvexShape lcs(qc, placedShape, p);
        if(baseObstacles && ShapeCollidesWithObstacles(qc, lcs, *baseObstacles))
        {
            return true;
        }
        if(contextObstacles && ShapeCollidesWithObstacles(qc, lcs, *contextObstacles, contextAgentToIgnore))
        {
            return true;
        }
    }
    return false;
}

bool
TestLineCollision_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& start,
        const cInternalPosition& end,
        const cAgent* contextAgentToIgnore
        )
{
    if(start.p == end.p)
    {
        return false;
    }
    tLine l(start.p, end.p);
    tFace endF = FaceAtEndOfLine<tMesh>(start.f, l);
    if(endF != end.f)
    {
        return true;
    }
    cConvexPoly extruded;
    shape.extrude(start.p, end.p, extruded);
    if(ShapeCollidesWithMeshEdge(qc, start.f, extruded))
    {
        return true;
    }
    if(baseObstacles || contextObstacles)
    {
        cLocalisedConvexShape lcsExtruded(qc, extruded, start);
        if(baseObstacles && ShapeCollidesWithObstacles(qc, lcsExtruded, *baseObstacles))
        {
            return true;
        }
        cConvexPoly placedShape;
        shape.offset(start.p, placedShape);
        cLocalisedConvexShape lcsStart(qc, placedShape, start); //... optimise - generate this on demand
        if(contextObstacles && ShapeCollidesWithObstaclesButNotAtStart(qc, lcsStart, lcsExtruded, *contextObstacles, contextAgentToIgnore))
        {
            return true;
        }
    }
    return false;
}

bool
TestLineCollision_XY_NoPreprocess(
        cQueryContext& qc,
        const cConvexPoly& shape,
        const cLocalisedConvexShapeSet* baseObstacles,
        const cCompoundContextCollisionPreprocess* contextObstacles,
        const cInternalPosition& start,
        const tPoint& targetP,
        tFace& endF,
        const cAgent* contextAgentToIgnore
        )
{
    if(start.p == targetP)
    {
        endF = start.f;
        return false;
    }
    tLine l(start.p, targetP);
    endF = FaceAtEndOfLine<tMesh>(start.f, l);
    if(endF->isExternal())
    {
        return true;
    }
    cConvexPoly extruded;
    shape.extrude(start.p, targetP, extruded);
    if(ShapeCollidesWithMeshEdge(qc, start.f, extruded))
    {
        return true;
    }
    if(baseObstacles || contextObstacles)
    {
        cLocalisedConvexShape lcsExtruded(qc, extruded, start);
        if(baseObstacles && ShapeCollidesWithObstacles(qc, lcsExtruded, *baseObstacles))
        {
            return true;
        }
        cConvexPoly placedShape;
        shape.offset(start.p, placedShape);
        cLocalisedConvexShape lcsStart(qc, placedShape, start); //... optimise - generate this on demand
        if(contextObstacles && ShapeCollidesWithObstaclesButNotAtStart(qc, lcsStart, lcsExtruded, *contextObstacles, contextAgentToIgnore))
        {
            return true;
        }
    }
    return false;
}
