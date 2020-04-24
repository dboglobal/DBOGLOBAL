//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "PointsOfVisibilityState.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "common/Containers/BitVector.h"

class cGraph_SingleTarget
{
    typedef cGraph_SingleTarget tThis;

    cInternalPosition _goal;
    cBitVector _stateCanPotentiallyConnectToGoal;

public:

    class cInitParams
    {
        friend class cGraph_SingleTarget;
        const cGraph_StaticComponent& _staticComponent;
        cInternalPosition _goal;
    public:
        cInitParams(
                const cGraph_StaticComponent& staticComponent,
                cInternalPosition goal
                ) :
         _staticComponent(staticComponent),
         _goal(goal)
        {
        }
    };

    cGraph_SingleTarget(const cInitParams& params) :
     _stateCanPotentiallyConnectToGoal(params._staticComponent.points() * 2, false),
     _goal(params._goal)
    {
    	params._staticComponent.potentialConnectionsFromDynamicPoint(
                *this, // not actually a graph, but only passed back to callback
			    *this, // not actually goal conditions, but only passed back to callback
                _goal,
			    *this
			    );
    }

// graph callback interface

    bool
    pointIsClosed(tSigned32 i) const
    {
        return false;
    }
    void
    directlyGeneratedPotentialConnection(
			const tThis& graph,
			const tThis& goalConditions,
			cPointsOfVisibilityState targetState
			)
    {
        cPointsOfVisibilityState reversedState = targetState.reversed();
        _stateCanPotentiallyConnectToGoal.setBit(reversedState.value(), true);
    }

// interface for graph targets

    tSigned32
    getNumberOfTargets() const
    {
        return 1;
    }
    tPoint
    getTargetPoint(tSigned32 index, const tPoint& parent) const
    {
        return _goal.p;
    }
    cInternalPosition
    getTargetPosition(tSigned32 index, const cInternalPosition& parent) const
    {
        return _goal;
    }
    tUnsigned32
    minimumCostToGoal(const tPoint &p) const
    {
        return Length(_goal.p - p);
    }
    bool
    nonTargetStateIsDisabled(cPointsOfVisibilityState state, bool stateIsCorner, const tPoint& p) const
    {
        return false;
    }
    bool
    nonTargetStateIsGoal(const tPoint& p) const
    {
        return false;
    }
    bool
    stateCanPotentiallyConnectToTarget(cPointsOfVisibilityState state) const
    {
        if(state.value() < static_cast<tUnsigned32>(_stateCanPotentiallyConnectToGoal.size()))
        {
            return _stateCanPotentiallyConnectToGoal[state.value()];
        }
        return true;
    }
    static bool
    hasDynamicTargets()
    {
        return false;
    }
};
