//**********************************************************************
//
// Copyright (c) 2003-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "PushPathAroundObstacles.h"
#include "interface/EdgeExpansion.h"
#include "interface/iFirstCollisionTester.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh_Common/interface/iMeshTranslator.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "platform_common/PointMultiplication.h"

using std::vector;

namespace nPushPathAroundObstacles
{

class cState
{
//    const vector<vector<cElementInFaceEntry> >& _baseElementsLookup;
    const cEdgeExpansion& _edgeExpansion;
    const cLocalisedConvexShapeSet& _nonSmallConvexExpansions;
    const iMeshTranslator<tMesh>* _translatorToSmallConvex;
    const cLocalisedConvexShapeSet& _smallConvexExpansions;
    const cLocalisedConvexShapeSet* _dynamicExpansions;
    vector<cInternalPosition>& _pathResult;
    vector<tSigned32>* _connectionIndicesResult;

    void getPotentialHullPointsInFace(
                eSide directionToPush, tFace f,
                const tLine& line, const tLine& line2, const tLine& line3,
                vector<tPoint>& potentialHullPoints) const;

    void getPotentialHullPointsAndEndF(
            eSide directionToPush,
            tFace startF, const tLine& line, const tLine& line2,
            vector<tPoint>& potentialHullPoints, tFace& endF) const;
    
public:

    cState(const cEdgeExpansion& edgeExpansion,
            const cLocalisedConvexShapeSet& nonSmallConvexExpansions,
            const iMeshTranslator<tMesh>* translatorToSmallConvex,
            const cLocalisedConvexShapeSet& smallConvexExpansions,
            const cLocalisedConvexShapeSet* dynamicExpansions,
            vector<cInternalPosition>& pathResult,
            vector<tSigned32>* connectionIndicesResult
            ) :
     _edgeExpansion(edgeExpansion),
     _nonSmallConvexExpansions(nonSmallConvexExpansions),
     _translatorToSmallConvex(translatorToSmallConvex),
     _smallConvexExpansions(smallConvexExpansions),
     _dynamicExpansions(dynamicExpansions),
     _pathResult(pathResult),
     _connectionIndicesResult(connectionIndicesResult)
    {
        assertD(_pathResult.empty());
        assertD(_connectionIndicesResult == 0 || _connectionIndicesResult->empty());
    }

    void
    pushSectionAroundBaseObstacles(
            eSide directionToPush,
            const cInternalPosition& target,
            vector<tPoint>& potentialHullPoints
            );

    void pushSectionAroundSmallConvex(const cInternalPosition& target);
    
    void
    doWholePath(
            const vector<cInternalPosition>& path,
            const vector<tSigned32>* connectionIndices
            );
};

void
cState::getPotentialHullPointsInFace(
                eSide directionToPush,
                tFace f,
                const tLine& line, const tLine& line2, const tLine& line3,
                vector<tPoint>& potentialHullPoints) const
{
    tSigned32 i;
//    for(i = 0; i < SizeL(_baseElementsLookup[f.index()]); ++i)
//    {
//        const cElementInFaceEntry& e = _baseElementsLookup[f.index()][i];
//        if(e.faceIndex != 0)
//        {
//            continue;
//        }
//        if(!e.element->cornerIsConvex())
//        {
//            continue;
//        }
//        tPoint candidate = e.element->cornerPoint();
//        if(line.side(candidate) == OtherSide(directionToPush)
//         || line2.side(candidate) == OtherSide(directionToPush)
//         || line3.side(candidate) != OtherSide(directionToPush)
//         )
//        {
//            continue;
//        }
//        potentialHullPoints.push_back(candidate);
//    }
    cEdgeExpansion::tCornerInFaceIterator it;
    while(_edgeExpansion.iterateCornersInFace(f.index(), it))
    {
        if(line.side(it._p) == OtherSide(directionToPush)
            || line2.side(it._p) == OtherSide(directionToPush)
            || line3.side(it._p) != OtherSide(directionToPush)
            )
        {
            continue;
        }
        potentialHullPoints.push_back(it._p);
    }
    for(i = 0; i < _nonSmallConvexExpansions.shapesInFace(f.index()); ++i)
    {
        const cLocalisedConvexShape& shape = _nonSmallConvexExpansions.shapeInFace(f.index(), i);
        tSigned32 j;
        for(j = 0; j < shape.sides(); ++j)
        {
            tPoint candidate = shape.poly().vertex(j);
            if(line.side(candidate) == OtherSide(directionToPush)
             || line2.side(candidate) == OtherSide(directionToPush)
             || line3.side(candidate) != OtherSide(directionToPush)
             )
            {
                continue;
            }
            potentialHullPoints.push_back(candidate);
        }
    }
    if(!_dynamicExpansions)
    {
        return;
    }
    for(i = 0; i < _dynamicExpansions->shapesInFace(f.index()); ++i)
    {
        const cLocalisedConvexShape& shape = _dynamicExpansions->shapeInFace(f.index(), i);
        tSigned32 j;
        for(j = 0; j < shape.sides(); ++j)
        {
            tPoint candidate = shape.poly().vertex(j);
            if(line.side(candidate) == OtherSide(directionToPush)
             || line2.side(candidate) == OtherSide(directionToPush)
             || line3.side(candidate) != OtherSide(directionToPush)
             )
            {
                continue;
            }
            potentialHullPoints.push_back(candidate);
        }
    }
}


// directionToPush is the direction the path is being pushed around a small convex
// the hull points here will actually have the effect of pushing in the opposite direction
void
cState::getPotentialHullPointsAndEndF(
                eSide directionToPush,
                tFace startF, const tLine& line, const tLine& line2,
                vector<tPoint>& potentialHullPoints, tFace& endF) const
{
    PROFILER_SCOPE("cState::getPotentialHullPointsAndEndF");
    tLine line3(line.start(), line2.end());

    endF = startF;
    tEdge crossed;

    {
        tIntersection intersection(line, line2);
        cIntersection_PointLike target(intersection);
        do
        {
            getPotentialHullPointsInFace(directionToPush, endF, line, line2, line3, potentialHullPoints);
        }
        while(TraverseToTarget_SI<tMesh>(endF, line, target, crossed));
    }
    {
        cPoint_PointLike target(line2.end());
        while(TraverseToTarget_SI<tMesh>(endF, line2, target, crossed))
        {
            getPotentialHullPointsInFace(directionToPush, endF, line, line2, line3, potentialHullPoints);
        }
    }
}

void
cState::pushSectionAroundBaseObstacles(eSide directionToPush,
                                const cInternalPosition& target,
                                vector<tPoint>& potentialHullPoints)
{
    tPoint fromP = _pathResult.back().p;
    tPoint targetP = target.p;

    eSide otherSide = OtherSide(directionToPush);

    while(1)
    {
        tSigned32 bestIndex = -1;
        tPoint bestOffset(targetP - fromP);

        tSigned32 i;
        for(i = 0; i < SizeL(potentialHullPoints); ++i)
        {
            tPoint offset = potentialHullPoints[i] - fromP;
            eSide side = bestOffset.side(offset);
            if(side == otherSide || (side == SIDE_CENTRE && offset.lengthSquared() > bestOffset.lengthSquared()))
            {
                bestIndex = i;
                bestOffset = offset;
            }
        }

        if(bestIndex == -1)
        {
            break;
        }

        tLine line(fromP, potentialHullPoints[bestIndex]);
        tFace f = FaceAtEndOfLine<tMesh>(_pathResult.back().f, line);
        
        cInternalPosition insertedP(f, line.end());
        pushSectionAroundSmallConvex(insertedP);
        _pathResult.push_back(insertedP);
        if(_connectionIndicesResult)
        {
            _connectionIndicesResult->push_back(-1);
        }

        fromP = line.end();
    }

    pushSectionAroundSmallConvex(target);
}


static void
ToNextAroundShape(const cConvexPoly& shape, eSide directionToPush, tSigned32& i)
{
    if(directionToPush == SIDE_LEFT)
    {
        ++i;
        if(i == shape.sides())
        {
            i = 0;
        }
    }
    else
    {
        if(i == 0)
        {
            i = shape.sides();
        }
        --i;
    }
}

void
cState::pushSectionAroundSmallConvex(const cInternalPosition& target)
{
    tPoint fromP = _pathResult.back().p;
    tPoint targetP = target.p;
    tLine line(fromP, targetP);

    tSigned32 collidingAgent;
    {
        bool collides = false;
        cAxisFraction distanceAlong;
        tLine collidingLine;
        tFace startFOnSmallConvex;
        if(_translatorToSmallConvex)
        {
            startFOnSmallConvex = TranslateToPartitioned(*_translatorToSmallConvex, _pathResult.back().f, fromP);
        }
        else
        {
            startFOnSmallConvex = _pathResult.back().f;
        }
        FirstCollision_AgainstShapeSet(
            _smallConvexExpansions,
            startFOnSmallConvex, line, collides, distanceAlong, collidingLine, collidingAgent
            );
        if(!collides)
        {
            return;
        }
    }

    const cLocalisedConvexShape& shape = _smallConvexExpansions[collidingAgent];
    const cConvexPoly& poly = shape.poly();

    tSigned32 joinIndex;
    tPoint p;
    tPoint oppositeP;
    eSide directionToPush;

    {
        tSigned32 leftIndex = poly.getIndexForSilhouettePoint(SIDE_LEFT, fromP);
        tSigned32 rightIndex = poly.getIndexForSilhouettePoint(SIDE_RIGHT, fromP);

        tPoint pLeft = poly.vertex(leftIndex);
        tSigned32 distLeft = Length(pLeft - fromP) + Length(targetP - pLeft);
        tPoint pRight = poly.vertex(rightIndex);
        tSigned32 distRight = Length(pRight - fromP) + Length(targetP - pRight);
        if(distRight < distLeft)
        {
            joinIndex = rightIndex;
            directionToPush = SIDE_RIGHT;
            p = pRight;
            oppositeP = pLeft;
        }
        else
        {
            joinIndex = leftIndex;
            directionToPush = SIDE_LEFT;
            p = pLeft;
            oppositeP = pRight;
        }
    }

    vector<tPoint> potentialHullPoints;
    tFace f;
    tLine line2(oppositeP, p);
    getPotentialHullPointsAndEndF(directionToPush, _pathResult.back().f, line, line2, potentialHullPoints, f);

    cInternalPosition joinP(f, p);
    pushSectionAroundBaseObstacles(directionToPush, joinP, potentialHullPoints);
    _pathResult.push_back(joinP);
    if(_connectionIndicesResult)
    {
        _connectionIndicesResult->push_back(-1);
    }

  // follow corners around the obstacle until the target is visible

    tSigned32 leaveIndex = poly.getIndexForSilhouettePoint(OtherSide(directionToPush), targetP);
    while(joinIndex != leaveIndex)
    {
        ToNextAroundShape(poly, directionToPush, joinIndex);
        tLine lineToNext(p, poly.vertex(joinIndex));
        tFace f = FaceAtEndOfLine<tMesh>(_pathResult.back().f, lineToNext);
        p = poly.vertex(joinIndex);
        _pathResult.push_back(cInternalPosition(f, p));
        if(_connectionIndicesResult)
        {
            _connectionIndicesResult->push_back(-1);
        }
    }

    potentialHullPoints.clear();
    line2 = line;
    tSigned32 oppositeIndex = poly.getIndexForSilhouettePoint(directionToPush, targetP);
    oppositeP = poly.vertex(oppositeIndex);
    line = tLine(p, oppositeP);

    {
        tFace endF_Ignored;
        getPotentialHullPointsAndEndF(directionToPush, _pathResult.back().f, line, line2, potentialHullPoints, endF_Ignored);
    }

    pushSectionAroundBaseObstacles(directionToPush, target, potentialHullPoints);
}

void
cState::doWholePath(
        const vector<cInternalPosition>& path,
        const vector<tSigned32>* connectionIndices
        )
{
    tSigned32 i;
    _pathResult.push_back(path[0]);
    for(i = 0; i + 1 < SizeL(path); ++i)
    {
        if(connectionIndices && (*connectionIndices)[i] != -1)
        {
            _pathResult.push_back(path[i + 1]);
            assertD(_connectionIndicesResult);
            _connectionIndicesResult->push_back((*connectionIndices)[i]);
        }
        else
        {
            pushSectionAroundSmallConvex(path[i + 1]);
            _pathResult.push_back(path[i + 1]);
            if(_connectionIndicesResult)
            {
                _connectionIndicesResult->push_back(-1);
            }
        }
    }
}

void
pushPathAroundSmallConvex(
        const cEdgeExpansion& edgeExpansion,
        const cLocalisedConvexShapeSet& nonSmallConvexExpansions,
        const iMeshTranslator<tMesh>* translatorToSmallConvex,
        const cLocalisedConvexShapeSet& smallConvexExpansions,
        const cLocalisedConvexShapeSet* dynamicExpansions,
        const std::vector<cInternalPosition>& path,
        const std::vector<tSigned32>* connectionIndices,
        std::vector<cInternalPosition>& pathResult,
        std::vector<tSigned32>* connectionIndicesResult
        )
{
    PROFILER_SCOPE("nPushPathAroundObstacles::pushPathAroundSmallConvex");
    if(path.size() < 2)
    {
        assertD(pathResult.empty());
        if(path.size() == 1)
        {
            pathResult.push_back(path[0]);
        }
        return;
    }
    cState state(
            edgeExpansion,
            nonSmallConvexExpansions,
            translatorToSmallConvex,
            smallConvexExpansions,
            dynamicExpansions,
            pathResult, connectionIndicesResult
            );
    state.doWholePath(path, connectionIndices);
}

}
