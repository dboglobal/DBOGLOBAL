//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE__GRAPH_INCLUDED
#define PATHENGINE_CORE__GRAPH_INCLUDED

class cQueryContext;

template <class tDynamicComponent, class tTargets>
class cGraph
{
    typedef cGraph<tDynamicComponent, tTargets> tThis;

    cQueryContext& _qc;
    const cGraph_StaticComponent& _staticComponent;
    tDynamicComponent _dynamicComponent;
    cInternalPosition _start;
    tTargets _targets;

    tPoint
    point(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.point(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        if(pointIndex < _dynamicComponent.points())
        {
            return _dynamicComponent.point(pointIndex);
        }
        assertD(pointIndex == _dynamicComponent.points());
        return _start.p;
    }
    tPoint
    point(tSigned32 pointIndex, const tPoint& previousP) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.point(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        if(pointIndex < _dynamicComponent.points())
        {
            return _dynamicComponent.point(pointIndex);
        }
        pointIndex -= _dynamicComponent.points();
        if(pointIndex == 0)
        {
            return _start.p;
        }
        --pointIndex;
        assertD(pointIndex < _targets.getNumberOfTargets());
        return _targets.getTargetPoint(pointIndex, previousP);
    }

    bool
    pointIsStatic(tSigned32 pointIndex) const
    {
        return pointIndex < _staticComponent.points();
    }

    tPoint
    axisBefore(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.axisBefore(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        assertD(pointIndex < _dynamicComponent.points());
        return _dynamicComponent.axisBefore(pointIndex);
    }

    tPoint
    axisAfter(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.axisAfter(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        assertD(pointIndex < _dynamicComponent.points());
        return _dynamicComponent.axisAfter(pointIndex);
    }

	template <class tCallBack> void
	potentialConnectionsTo(
			tSigned32 toPoint,
            tUnsigned32 minimumCostToGoal,
			tCallBack& callBack
			) const
	{
        cInternalPosition toIP = position(toPoint);
        assertD(pointIsCorner(toPoint));
        tPoint axisBefore = tThis::axisBefore(toPoint);
        tPoint axisAfter = tThis::axisAfter(toPoint);

        // connection from start
        {
            tSigned32 pointIndex = _staticComponent.points() + _dynamicComponent.points();
			assertD(callBack.connectionsHaveBeenGeneratedFromPoint(pointIndex));

            tPoint fromP = _start.p;
            eSilhouetteZone targetApproachZone = ZoneFor(axisBefore, axisAfter, fromP - toIP.p);
            if(targetApproachZone != SZ_NOTSILHOUETTE)
            {
                cPointsOfVisibilityState fromState(pointIndex, EPZ_DEPARTING);
                cPointsOfVisibilityState targetState(toPoint, targetApproachZone);
			    callBack.additionalPotentialConnection(*this, fromState, targetState, minimumCostToGoal);
            }
        }

        _staticComponent.potentialConnectionsToDynamicCorner(
                *this,
                toPoint, toIP, axisBefore, axisAfter,
                minimumCostToGoal,
                callBack
                );

		tSigned32 i;
        for(i = 0; i < _dynamicComponent.points(); i++)
        {
			if(!_dynamicComponent.pointIsAttached(i))
			{
				continue;
			}
            tSigned32 pointIndex = _staticComponent.points() + i;
			if(!callBack.connectionsHaveBeenGeneratedFromPoint(pointIndex))
			{
				continue;
			}

            tPoint fromP = _dynamicComponent.point(i);
            assertD(pointIsCorner(pointIndex));            
            eSilhouetteZone zoneAtFromPoint = ZoneFor(
                    _dynamicComponent.axisBefore(i),
                    _dynamicComponent.axisAfter(i),
                    toIP.p - fromP
                    );
            if(zoneAtFromPoint == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            eSilhouetteZone approachZone = OppositeZone(zoneAtFromPoint);
            cPointsOfVisibilityState fromState(pointIndex, approachZone);

            eSilhouetteZone targetApproachZone = ZoneFor(axisBefore, axisAfter, fromP - toIP.p);
            if(targetApproachZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            cPointsOfVisibilityState targetState(toPoint, targetApproachZone);
			callBack.additionalPotentialConnection(*this, fromState, targetState, minimumCostToGoal);
        }
	}

	template <class tGoalConditions, class tCallBack> void
    attachDynamicObstacle(
            const tGoalConditions& goalConditions,
            tSigned32 obstacleIndex,
            tCallBack& callBack)
    {
		tSigned32 firstPoint = _staticComponent.points() + _dynamicComponent.obstacleFirstCorner(obstacleIndex);
		tSigned32 pointAfter = _staticComponent.points() + _dynamicComponent.obstacleCornerAfter(obstacleIndex);
		tSigned32 i;
		for(i = firstPoint; i != pointAfter; ++i)
		{
		    tUnsigned32 minimumCostToGoalFromNewPoint = goalConditions.minimumCostToGoal(i);
		    potentialConnectionsTo(i, minimumCostToGoalFromNewPoint, callBack);
		}
        _dynamicComponent.attachObstacle(obstacleIndex); // attaching afterwards prevents dynamic component connecting point to itself
    }

	template <class tGoalConditions, class tCallBack> bool
    testDynamicCollisionAndAttachObstructing(
            const tGoalConditions& goalConditions,
            const cInternalPosition& fromIP,
            const tPoint& toP,
			tCallBack& callBack
            )
    {
        tLine line(fromIP.p, toP);
        tSigned32 blockingUnattachedIndex;
        bool result = _dynamicComponent.checkCollision(fromIP.f, line, blockingUnattachedIndex);
        if(result && blockingUnattachedIndex != -1)
        {
            attachDynamicObstacle(goalConditions, blockingUnattachedIndex, callBack);
        }
        return result;
    }

public:

    cGraph(
            cQueryContext& qc,
            const cGraph_StaticComponent& staticComponent,
            const tDynamicComponent& dynamicComponentToCopy,
            const cInternalPosition& start,
            const typename tTargets::cInitParams& targetsParams
            ) :
     _qc(qc),
     _staticComponent(staticComponent),
     _dynamicComponent(dynamicComponentToCopy),
     _start(start),
     _targets(targetsParams)
    {
    }

    cPointsOfVisibilityState
    startState() const
    {
        tSigned32 startPoint = _staticComponent.points() + _dynamicComponent.points();
        return cPointsOfVisibilityState(startPoint, EPZ_DEPARTING);
    }

    cInternalPosition
    position(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.position(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        if(pointIndex < _dynamicComponent.points())
        {
            return _dynamicComponent.position(pointIndex);
        }
        assertD(pointIndex == _dynamicComponent.points());
        return _start;
    }
    cInternalPosition
    position(tSigned32 pointIndex, const cInternalPosition& parentPosition) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.position(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        if(pointIndex < _dynamicComponent.points())
        {
            return _dynamicComponent.position(pointIndex);
        }
        pointIndex -= _dynamicComponent.points();
        if(pointIndex == 0)
        {
            return _start;
        }
        --pointIndex;
        assertD(pointIndex < _targets.getNumberOfTargets());
        return _targets.getTargetPosition(pointIndex, parentPosition);
    }

    tSigned32
    getConnectionIndex(cPointsOfVisibilityState fromState, cPointsOfVisibilityState toState) const
    {
        if(fromState.approachEndPointZone() != EPZ_ARRIVING)
        {
            return -1;
        }
        if(toState.approachEndPointZone() != EPZ_DEPARTING)
        {
            return -1;
        }
        if(pointIsCorner(fromState.pointIndex()) || pointIsCorner(toState.pointIndex()))
        {
            return -1;
        }
        return _staticComponent.getConnectionIndex(fromState.pointIndex(), toState.pointIndex());
    }

    bool
    pointIsCorner(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points())
        {
            return _staticComponent.pointIsCorner(pointIndex);
        }
        pointIndex -= _staticComponent.points();
        if(pointIndex < _dynamicComponent.points())
        {
            return _dynamicComponent.pointIsCorner(pointIndex);
        }
        return false;
    }

// graph interface

	tSigned32
    points() const
    {
        return _staticComponent.points() + _dynamicComponent.points() + 1 + _targets.getNumberOfTargets();
    }

	template <class tGoalConditions, class tCallBack> void
	potentialConnectionsFrom(
			const tGoalConditions& goalConditions,
			cPointsOfVisibilityState fromState,
			tCallBack& callBack
			) const
	{
        tSigned32 fromPoint = fromState.pointIndex();
        cInternalPosition fromIP = position(fromPoint);

	// connections to permanent corners

        if(pointIsStatic(fromPoint))
		{
            _staticComponent.potentialConnectionsFrom(
                    *this, goalConditions,
                    fromState,
                    callBack
                    );
        }
		else if(pointIsCorner(fromPoint))
        {
            _staticComponent.potentialConnectionsFromDynamicCorner(
                    *this, goalConditions,
                    fromIP, fromState.approachSilhouetteZone(),
                    axisBefore(fromPoint), axisAfter(fromPoint),
                    callBack
                    );
		}
        else
        {
            _staticComponent.potentialConnectionsFromDynamicPoint(
                    *this, goalConditions,
                    fromIP,
                    callBack
                    );
        }
        
    // connections to dynamic corners

        if(fromState.approachEndPointZone() == EPZ_ARRIVING && !pointIsCorner(fromPoint))
        {
          // start of off-mesh connection
            return;
        }

        eSilhouetteZone departZone;
        tPoint axisBefore;
        tPoint axisAfter;
        if(pointIsCorner(fromPoint))
        {
            departZone = OppositeZone(fromState.approachSilhouetteZone());
            axisBefore = tThis::axisBefore(fromPoint);
            axisAfter = tThis::axisAfter(fromPoint);
        }
        tSigned32 i;
        for(i = 0; i < _dynamicComponent.points(); i++)
        {
            if(!_dynamicComponent.pointIsAttached(i))
            {
                continue;
            }
            tSigned32 pointIndex = _staticComponent.points() + i;
			if(i == fromPoint || callBack.pointIsClosed(i))
            {
                continue;
            }
            tPoint targetP = _dynamicComponent.point(i);
            if(pointIsCorner(fromPoint) && ZoneFor(axisBefore, axisAfter, targetP - fromIP.p) != departZone)
            {
                continue;
            }
            cPointsOfVisibilityState targetState;
            assertD(pointIsCorner(pointIndex));
            //if(pointIsCorner(pointIndex))
            {
                eSilhouetteZone targetApproachZone = ZoneFor(
                        _dynamicComponent.axisBefore(i),
                        _dynamicComponent.axisAfter(i),
                        fromIP.p - targetP
                        );
                if(targetApproachZone == SZ_NOTSILHOUETTE)
                {
                    continue;
                }
                targetState = cPointsOfVisibilityState(pointIndex, targetApproachZone);
            }
            //else
            //{
            //    targetState = cPointsOfVisibilityState(pointIndex, EPZ_ARRIVING);
            //}
   		    callBack.directlyGeneratedPotentialConnection(*this, goalConditions, targetState);
        }
        if(!_targets.stateCanPotentiallyConnectToTarget(fromState))
        {
            return;
        }
        for(i = 0; i < _targets.getNumberOfTargets(); i++)
        {
            tPoint targetP = _targets.getTargetPoint(i, fromIP.p);
            if(pointIsCorner(fromPoint) && ZoneFor(axisBefore, axisAfter, targetP - fromIP.p) != departZone)
            {
                continue;
            }
            tSigned32 pointIndex = _staticComponent.points() + _dynamicComponent.points() + 1 + i;
            cPointsOfVisibilityState targetState(pointIndex, EPZ_ARRIVING);
   		    callBack.directlyGeneratedPotentialConnection(*this, goalConditions, targetState);
        }
    }

    tUnsigned32
    connectionCost(tSigned32 fromPoint, tSigned32 toPoint) const
    {
        tPoint fromP = point(fromPoint);
        tPoint toP = point(toPoint, fromP);
        return Length(toP - fromP);
    }

	template <class tGoalConditions, class tCallBack> bool
    validateConnection(
            const tGoalConditions& goalConditions,
            cPointsOfVisibilityState fromState, cPointsOfVisibilityState toState,
            tUnsigned32& additionalCost,
			tCallBack& callBack
            )
    {
        tSigned32 fromPoint = fromState.pointIndex();
        tSigned32 toPoint = toState.pointIndex();
        cInternalPosition fromIP = position(fromPoint);
        tPoint toP = point(toPoint, fromIP.p);
        if(fromIP.p == toP)
        {
            if(!pointIsCorner(fromPoint) || !pointIsCorner(toPoint))
            {
                // land and then take off again from same point
                additionalCost = 0;
                return true;
            }
            return false;
        }

        if(pointIsStatic(fromPoint) && pointIsStatic(toPoint))
        {
        // visibility graph connection
            if(fromState.approachEndPointZone() == EPZ_ARRIVING && !pointIsCorner(fromPoint) && !pointIsCorner(toPoint))
            {
              // off mesh connection
                cInternalPosition toIP = position(toPoint);
                additionalCost = 0;
                return !_dynamicComponent.testPointCollision(_qc, toIP);
            }
        }
        else
        {
            cInternalPosition toIP = position(toPoint, fromIP);
            if(_staticComponent.testLineCollision(_qc, fromIP, toIP))
            {
                return false;
            }
        }

        if(testDynamicCollisionAndAttachObstructing(
                goalConditions,
                fromIP, toP,
                callBack
                ))
        {
            return false;
        }

    // basic connection is validated (and not off mesh connection or zero length)
    // (although it is still possible to be blocked by -1 traverse costs)
    // go ahead and work out additional costs

      // dynamic costs are always added
        
        bool blocked;
        additionalCost = _dynamicComponent.getAdditionalCost(_qc, fromIP, toP, blocked);
        if(blocked)
        {
            return false;
        }

      // fixed costs are added for connections not in the visibility graph
      // (costs in the visibility graph already include fixed costs)
        if(!pointIsStatic(fromPoint) || !pointIsStatic(toPoint))
        {
            _staticComponent.addAdditionalCost(_qc, fromIP, toP, additionalCost, blocked);
            if(blocked)
            {
                return false;
            }
        }

        return true;
    }

    bool
    pathCornerCanBeCollapsed(cPointsOfVisibilityState state1, cPointsOfVisibilityState state2, cPointsOfVisibilityState state3) const
    {
        if(!pointIsCorner(state2.pointIndex()))
        {
            return false;
        }
        tPoint p1 = point(state1.pointIndex());
        tPoint p2 = point(state2.pointIndex());
        tPoint p3 = point(state3.pointIndex(), p2);
        p3 -= p2;
        p1 -= p2;
        if(state2.approachSilhouetteZone() == SZ_LEFTSILHOUETTE)
        {
            return p1.side(p3) != SIDE_LEFT;
        }
        return p1.side(p3) != SIDE_RIGHT;
    }

// goal conditions interface

    bool isGoal(cPointsOfVisibilityState state) const
    {
        return state.pointIndex() >= _staticComponent.points() + _dynamicComponent.points() + 1
            //... check this second bit gets optimised out for single target
            || _targets.nonTargetStateIsGoal(point(state.pointIndex()));
    }
    tUnsigned32 minimumCostToGoal(tSigned32 pointIndex) const
    {
        if(pointIndex < _staticComponent.points() + _dynamicComponent.points())
        {
            return _targets.minimumCostToGoal(point(pointIndex));
        }
        return 0;
    }
};

#endif
