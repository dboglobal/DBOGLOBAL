//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GRAPH__STATIC_COMPONENT_INCLUDED
#define GRAPH__STATIC_COMPONENT_INCLUDED

#include "SilhouetteZone.h"
#include "SilhouetteLookup.h"
#include "VisibilityGraph.h"
#include "StaticLineCollisionTester.h"
#include "interface/CombinedCostCalculator.h"
#include "interface/CombinedCollisionTesters.h"
#include "interface/CornerProvider.h"
#include "interface/iCollisionTester.h"
#include "interface/OffMeshConnections.h"
#include "interface/ShapesCostCalculator.h"
#include "libs/Geometry/interface/tLine.h"
#include "platform_common/AddWithOverflow.h"
#include "common/Containers/BitVector.h"

class cQueryContext;

class cGraph_StaticComponent
{
    typedef cStaticLineCollisionTester tLineCollision;
    typedef iPointCollisionTester tPointCollision;
    typedef cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator> tCostCalculator;

    const cOffMeshConnections& _connections;
    const cCornerProvider& _corners;
    const tSigned32 _numberOfHardCorners;
    const tLineCollision& _lineCollision;
    const tCostCalculator* _costCalculator;
    cSilhouetteLookup _silhouetteLookup;
    cVisibilityGraph _visibilityGraph;
    cBitVector _softCornerIsObstructed; //... optimise - obstructed soft corners should not be added to the corner provider in the first place
    cBitVector _endPointIsObstructed;

    void constructorCommon(cQueryContext& qc, const tPointCollision& pointCollision)
    {
        tSigned32 i;
        for(i = _numberOfHardCorners; i!= _corners.size(); ++i)
        {
            tFace f = _corners.cornerFace(i);
            tPoint p = _corners.cornerPoint(i);
            bool obstructed = pointCollision.testPointCollision(qc, f, p);
            _softCornerIsObstructed.setBit(i - _numberOfHardCorners, obstructed);
        }
        for(i = 0; i!= _connections.numberOfEndPoints(); ++i)
        {
            const cInternalPosition& ip = _connections.refEndPoint(i);
            bool obstructed = pointCollision.testPointCollision(qc, ip.f, ip.p);
            _endPointIsObstructed.setBit(i, obstructed);
        }
    }

public:

    // called when constructing preprocess
    // silhouette lookup must then be built outside this class, and then passed in to buildVisibilityGraph()
    cGraph_StaticComponent(
            cQueryContext& qc,
            const cOffMeshConnections& connections,
            const cCornerProvider& corners,
            tSigned32 numberOfHardCorners,
            const tLineCollision& lineCollision,
            const tPointCollision& pointCollision,
            const tCostCalculator* costCalculator
            ) :
     _connections(connections),
     _corners(corners),
     _numberOfHardCorners(numberOfHardCorners),
     _lineCollision(lineCollision),
     _costCalculator(costCalculator),
     _softCornerIsObstructed(corners.size() - numberOfHardCorners),
     _endPointIsObstructed(connections.numberOfEndPoints())
    {
        constructorCommon(qc, pointCollision);
    }

    // called when loading preprocess from persistence
    cGraph_StaticComponent(
            cQueryContext& qc,
            const cOffMeshConnections& connections,
            const cCornerProvider& corners,
            tSigned32 numberOfHardCorners,
            const tLineCollision& lineCollision,
            const tPointCollision& pointCollision,
            const tCostCalculator* costCalculator,
            iXMLInputStream& is
            ) :
     _connections(connections),
     _corners(corners),
     _numberOfHardCorners(numberOfHardCorners),
     _lineCollision(lineCollision),
     _costCalculator(costCalculator),
     _silhouetteLookup(is),
     _visibilityGraph(is),
     _softCornerIsObstructed(corners.size() - numberOfHardCorners),
     _endPointIsObstructed(connections.numberOfEndPoints())
    {
        constructorCommon(qc, pointCollision);
    }

    // the lookup is swapped in
    void
    buildVisibilityGraph(cQueryContext& qc, cSilhouetteLookup& silhouetteLookup)
    {
        silhouetteLookup.swap(_silhouetteLookup);
        _visibilityGraph.build(qc, _silhouetteLookup, *this, _connections);
    }

    void
    save(iXMLOutputStream& os) const
    {
        _silhouetteLookup.save(os);
        _visibilityGraph.save(os);
    }

// graph interface

	tSigned32
	points() const
	{
        return _connections.numberOfEndPoints() + _corners.size();
	}

	template <class tGraph, class tGoalConditions, class tCallBack> void
	potentialConnectionsFrom(
            const tGraph& graph,
			const tGoalConditions& goalConditions,
			cPointsOfVisibilityState fromState,
			tCallBack& callBack
			) const
	{
        tSigned32 i;
        for(i = 0; i != _visibilityGraph.connectionsFrom(fromState); ++i)
        {
            cPointsOfVisibilityState targetState = _visibilityGraph.connectionFrom(fromState, i);
			if(callBack.pointIsClosed(targetState.pointIndex()))
            {
                continue;
            }
			callBack.directlyGeneratedPotentialConnection(
                    graph,
                    goalConditions,
                    targetState,
                    _visibilityGraph.connectionCost(fromState, i)
                    );
        }
    }

	template <class tGraph, class tGoalConditions, class tCallBack> void
	potentialConnectionsFromDynamicCorner(
            const tGraph& graph,
			const tGoalConditions& goalConditions,
            const cInternalPosition& cornerIP,
            eSilhouetteZone approachZone, 
            const tPoint& axisBefore, const tPoint& axisAfter,
			tCallBack& callBack
			) const
	{
        tSigned32 faceIndex = cornerIP.cell();
		tSigned32 i;
        for(i = 0; i < _silhouetteLookup.pointsFromFace(faceIndex); i++)
        {
            tSigned32 pointIndex = _silhouetteLookup.pointFromFace(faceIndex, i);
            assertD(pointIsCorner(pointIndex));
			if(callBack.pointIsClosed(pointIndex))
            {
                continue;
            }
            tPoint targetP = point(pointIndex);
            eSilhouetteZone departZone = ZoneFor(axisBefore, axisAfter, targetP - cornerIP.p);
            if(departZone != OppositeZone(approachZone))
            {
                continue;
            }
            eSilhouetteZone targetApproachZone = zoneFor(pointIndex, cornerIP.p);
            if(targetApproachZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            cPointsOfVisibilityState targetState(pointIndex, targetApproachZone);
			callBack.directlyGeneratedPotentialConnection(graph, goalConditions, targetState);
        }
        for(i = 0; i < _silhouetteLookup.statesFromFace(faceIndex); i++)
        {
            cPointsOfVisibilityState targetState = _silhouetteLookup.stateFromFace(faceIndex, i);
			if(callBack.pointIsClosed(targetState.pointIndex()))
            {
                continue;
            }
            tPoint targetP = point(targetState.pointIndex());
            eSilhouetteZone departZone = ZoneFor(axisBefore, axisAfter, targetP - cornerIP.p);
            if(departZone != OppositeZone(approachZone))
            {
                continue;
            }
            if(pointIsCorner(targetState.pointIndex()))
            {
                eSilhouetteZone regionApproachZone = targetState.approachSilhouetteZone();
                eSilhouetteZone targetApproachZone = zoneFor(targetState.pointIndex(), cornerIP.p);
                if(targetApproachZone != regionApproachZone)
                {
                    continue;
                }
            }
			callBack.directlyGeneratedPotentialConnection(graph, goalConditions, targetState);
        }        
    }

	template <class tGraph, class tGoalConditions, class tCallBack> void
	potentialConnectionsFromDynamicPoint(
            const tGraph& graph,
			const tGoalConditions& goalConditions,
            const cInternalPosition& fromIP,
			tCallBack& callBack
			) const
	{
        tSigned32 faceIndex = fromIP.cell();
		tSigned32 i;
        for(i = 0; i < _silhouetteLookup.pointsFromFace(faceIndex); i++)
        {
            tSigned32 pointIndex = _silhouetteLookup.pointFromFace(faceIndex, i);
            assertD(pointIsCorner(pointIndex));
			if(callBack.pointIsClosed(pointIndex))
            {
                continue;
            }
            eSilhouetteZone targetApproachZone = zoneFor(pointIndex, fromIP.p);
            if(targetApproachZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            cPointsOfVisibilityState targetState(pointIndex, targetApproachZone);
			callBack.directlyGeneratedPotentialConnection(graph, goalConditions, targetState);
        }
        for(i = 0; i < _silhouetteLookup.statesFromFace(faceIndex); i++)
        {
            cPointsOfVisibilityState targetState = _silhouetteLookup.stateFromFace(faceIndex, i);
			if(callBack.pointIsClosed(targetState.pointIndex()))
            {
                continue;
            }
            if(pointIsCorner(targetState.pointIndex()))
            {
                eSilhouetteZone regionApproachZone = targetState.approachSilhouetteZone();
                eSilhouetteZone targetApproachZone = zoneFor(targetState.pointIndex(), fromIP.p);
                if(targetApproachZone != regionApproachZone)
                {
                    continue;
                }
            }
			callBack.directlyGeneratedPotentialConnection(graph, goalConditions, targetState);
        }
    }

	template <class tGraph, class tCallBack> void
	potentialConnectionsToDynamicCorner(
            const tGraph& graph,
            tSigned32 toPointIndex,
            const cInternalPosition& cornerIP,
            const tPoint& axisBefore, const tPoint& axisAfter,
            tUnsigned32 minimumCostToGoal,
			tCallBack& callBack
			) const
    {
        tSigned32 faceIndex = cornerIP.cell();
		tSigned32 i;
        for(i = 0; i < _silhouetteLookup.pointsFromFace(faceIndex); i++)
        {
            tSigned32 pointIndex = _silhouetteLookup.pointFromFace(faceIndex, i);
            assertD(pointIsCorner(pointIndex));
			if(!callBack.connectionsHaveBeenGeneratedFromPoint(pointIndex))
			{
				continue;
			}
            eSilhouetteZone departZone = zoneFor(pointIndex, cornerIP.p);
            if(departZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            eSilhouetteZone approachZone = OppositeZone(departZone);

            tPoint fromP = point(pointIndex);
            eSilhouetteZone targetApproachZone = ZoneFor(axisBefore, axisAfter, fromP - cornerIP.p);
            if(targetApproachZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }

            cPointsOfVisibilityState fromState(pointIndex, approachZone);
            cPointsOfVisibilityState targetState(toPointIndex, targetApproachZone);

			callBack.additionalPotentialConnection(graph, fromState, targetState, minimumCostToGoal);
        }
        for(i = 0; i < _silhouetteLookup.statesFromFace(faceIndex); i++)
        {
            cPointsOfVisibilityState lookupState = _silhouetteLookup.stateFromFace(faceIndex, i);
            tSigned32 pointIndex = lookupState.pointIndex();
			if(!callBack.connectionsHaveBeenGeneratedFromPoint(pointIndex))
			{
				continue;
			}

            if(pointIsCorner(pointIndex))
            {
                eSilhouetteZone regionApproachZone = lookupState.approachSilhouetteZone();
                eSilhouetteZone testZone = zoneFor(pointIndex, cornerIP.p);
                if(testZone != regionApproachZone)
                {
                    continue;
                }
            }

            cPointsOfVisibilityState fromState = lookupState.reversed();
            tPoint fromP = point(pointIndex);

            eSilhouetteZone targetApproachZone = ZoneFor(axisBefore, axisAfter, fromP - cornerIP.p);
            if(targetApproachZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }

            cPointsOfVisibilityState targetState(toPointIndex, targetApproachZone);

			callBack.additionalPotentialConnection(graph, fromState, targetState, minimumCostToGoal);
        }        
    }

// point and corner info lookup

    tPoint
    point(tSigned32 pointIndex) const
    {
        if(pointIndex < _connections.numberOfEndPoints())
        {
            return _connections.refEndPoint(pointIndex).p;
        }
        pointIndex -= _connections.numberOfEndPoints();
        return _corners.cornerPoint(pointIndex);
    }
    cInternalPosition
    position(tSigned32 pointIndex) const
    {
        if(pointIndex < _connections.numberOfEndPoints())
        {
            return _connections.refEndPoint(pointIndex);
        }
        pointIndex -= _connections.numberOfEndPoints();
        return cInternalPosition(_corners.cornerFace(pointIndex), _corners.cornerPoint(pointIndex));
    }
    bool
    pointIsCorner(tSigned32 pointIndex) const
    {
        assertD(pointIndex >= 0 && pointIndex < points());
        return pointIndex >= _connections.numberOfEndPoints();
    }
    tSigned32
    cornerForPointIndex(tSigned32 pointIndex) const
    {
        assertD(pointIsCorner(pointIndex));
        return pointIndex - _connections.numberOfEndPoints();
    }

    static tSigned32
    pointIndexForEndPoint(tSigned32 endPoint)
    {
        return endPoint;
    }
    static tSigned32
    endPointForPointIndex(tSigned32 pointIndex)
    {
//        assertD(!pointIsCorner(pointIndex));
        return pointIndex;
    }

    tSigned32
    getConnectionIndex(tSigned32 fromPoint, tSigned32 toPoint) const
    {
        assertD(!pointIsCorner(fromPoint) && !pointIsCorner(toPoint));
        tSigned32 result = _connections.lookupConnection(fromPoint, toPoint);
        assertD(result != -1);
        return result;
    }

    tPoint
    axisBefore(tSigned32 pointIndex) const
    {
        assertD(pointIsCorner(pointIndex));
        return _corners.axisBefore(cornerForPointIndex(pointIndex));
    }
    tPoint
    axisAfter(tSigned32 pointIndex) const
    {
        assertD(pointIsCorner(pointIndex));
        return _corners.axisAfter(cornerForPointIndex(pointIndex));
    }

    bool
    graphPointIsObstructed(tSigned32 pointIndex) const
    {
        if(pointIsCorner(pointIndex))
        {
            tSigned32 corner = cornerForPointIndex(pointIndex);
            if(corner < _numberOfHardCorners)
            {
                return false;
            }
            return _softCornerIsObstructed[corner - _numberOfHardCorners];
        }
        return _endPointIsObstructed[endPointForPointIndex(pointIndex)];
    }

    eSilhouetteZone
    zoneFor(tSigned32 pointIndex, const tPoint& targetP) const
    {
        return ZoneFor(
                axisBefore(pointIndex),
                axisAfter(pointIndex),
                targetP - point(pointIndex)
                );
    }

// line collision

    bool
    testLineCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
    {
        return _lineCollision.testCollision(qc, from, to);
    }
    bool
    testCollisionToXY(cQueryContext& qc, const cInternalPosition& from, const tPoint& to) const
    {
        tLine l(from.p, to);
        return _lineCollision.testCollisionToXY(qc, from.f, l);
    }

// cost computation

    void
    addAdditionalCost(
            cQueryContext& qc,
            const cInternalPosition& from, const tPoint& to,
            tUnsigned32& result,
            bool& blocked
            ) const
    {
        if(_costCalculator)
        {
            tUnsigned32 additionalCost;
            additionalCost = static_cast<tUnsigned32>(_costCalculator->calculate(qc, from, to, blocked));
            //if(!blocked) // avoid the conditional, the value of result doesn't matter in the blocked case
            //{
                if(AddWithOverflow(result, additionalCost, result))
                {
                    result = 0xffffffff;
                }
            //}
        }
    }

    void
    getTotalCosts_TwoWay(
            cQueryContext& qc,
            const cInternalPosition& from, const cInternalPosition& to,
            tUnsigned32& costOut, tUnsigned32& costBack,
            bool& blockedOut, bool& blockedBack
            ) const
    {
        tUnsigned32 distanceComponent = Length(to.p - from.p);

        costOut = distanceComponent;
        costBack = distanceComponent;
        blockedOut = false;
        blockedBack = false;

        if(_costCalculator)
        {
            tUnsigned32 additionalCost;
            additionalCost = static_cast<tUnsigned32>(_costCalculator->calculate(qc, from, to.p, blockedOut));
            if(AddWithOverflow(costOut, additionalCost, costOut))
            {
                costOut = 0xffffffff;
            }
            additionalCost = static_cast<tUnsigned32>(_costCalculator->calculate(qc, to, from.p, blockedBack));
            if(AddWithOverflow(costBack, additionalCost, costBack))
            {
                costBack = 0xffffffff;
            }
        }
    }

};

#endif

