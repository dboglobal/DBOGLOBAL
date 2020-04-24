//**********************************************************************
//
// Copyright (c) 2007
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "SimpleHeap.h"
#include "PointsOfVisibilityState.h"
#include "platform_common/AddWithOverflow.h"
#include "common/Containers/PriorityQueue.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

class cPathNode
{
    friend class cAStarHeap;

    cPathNode* _prev;
    cPointsOfVisibilityState _endState;
    tUnsigned32 _costSoFar;
    tUnsigned32 _estimatedTotalCost;
    bool _validated;

    void
    initOrReinitAsStartNode(
            cPointsOfVisibilityState startState,
            tUnsigned32 minimumCostToGoal
            )
    {
        _prev = 0;
        _costSoFar = 0;
        _endState = startState;
        _estimatedTotalCost = minimumCostToGoal;
        _validated = false;
    }

    void
    initOrReinitAsExtensionNode(
            cPathNode* prev,
            cPointsOfVisibilityState targetState,
            tUnsigned32 connectionCost,
            tUnsigned32 minimumCostToGoal
            )
    {
        assertD(prev);
        _prev = prev;
        if(AddWithOverflow(_prev->_costSoFar, connectionCost, _costSoFar))
        {
            _costSoFar = 0xffffffff;
        }
		_endState = targetState;
        if(AddWithOverflow(_costSoFar, minimumCostToGoal, _estimatedTotalCost))
        {
            _estimatedTotalCost = 0xffffffff;
        }
        _validated = false;
    }

    bool
    validated() const
    {
        return _validated;
    }

    void
    validate(tUnsigned32 additionalCost)
    {
        assertD(!_validated);
        _validated = true;
        if(AddWithOverflow(_costSoFar, additionalCost, _costSoFar))
        {
            _costSoFar = 0xffffffff;
        }
        if(AddWithOverflow(_estimatedTotalCost, additionalCost, _estimatedTotalCost))
        {
            _estimatedTotalCost = 0xffffffff;
        }
    }

public:

    bool
    operator<(const cPathNode& rhs) const
    {
        return _estimatedTotalCost < rhs._estimatedTotalCost;
    }
};

class cAStarHeap
{
    cSimpleHeap<cPathNode, 1500> _nodeHeap;
//    std::vector<bool> _pointIsClosed;
    cBitVector _pointIsClosed;
    std::vector<cPathNode*> _nodeToPoint;
    cPriorityQueue<cPathNode*> _openQueue;
    std::vector<cPathNode*> _closed;

  // used during search
    cPathNode* _fromNode;
    cPointsOfVisibilityState _newState;
    cPointsOfVisibilityState _previousState;
    tUnsigned32 _minimumCostToGoalFromNewPoint;

    void
    enqueue(cPathNode* node)
    {
        _openQueue.insert(node);
    }

public:

    cAStarHeap():
      _pointIsClosed(1000),
      _nodeToPoint(1000),
      _openQueue(500),
      _closed(500)
    {
        _closed.resize(0);
    }
    ~cAStarHeap()
    {
        clear();
    }

    void
    clear()
    {
        tSigned32 i;
        for(i = 0; i != SizeL(_closed); ++i)
        {
            _nodeHeap.release(_closed[i]);
        }
        _closed.resize(0);
    }

    cPointsOfVisibilityState
    getParentState(cPointsOfVisibilityState state) const
    {
        assertD(connectionsHaveBeenGeneratedFromPoint(state.pointIndex()));
        if(_nodeToPoint[state.pointIndex()]->_prev)
        {
            return _nodeToPoint[state.pointIndex()]->_prev->_endState;
        }
        cPointsOfVisibilityState result;
        result.setInvalid();
        return result;
    }

    tSigned32
    getNumberOfPathPointsTo(cPointsOfVisibilityState state) const
    {
        assertD(state.valid());
        tSigned32 result = 0;
        do
        {
            ++result;
            state = getParentState(state);
        }
        while(state.valid());
        return result;
    }

    tUnsigned32
    getPathLengthTo(cPointsOfVisibilityState state) const
    {
        assertD(state.valid());
        assertD(connectionsHaveBeenGeneratedFromPoint(state.pointIndex()));
        return _nodeToPoint[state.pointIndex()]->_costSoFar;
    }

  // interface for graph connection generation callbacks

    bool
    pointIsClosed(tSigned32 i) const
    {
        return _pointIsClosed[i];
    }

    template <class tGraph, class tGoalConditions> void
    directlyGeneratedPotentialConnection(
			const tGraph& graph,
			const tGoalConditions& goalConditions,
			cPointsOfVisibilityState targetState
			)
    {
        if(_previousState.valid() && graph.pathCornerCanBeCollapsed(_previousState, _fromNode->_endState, targetState))
        {
            return;
        }
        tUnsigned32 connectionCost = graph.connectionCost(_fromNode->_endState.pointIndex(), targetState.pointIndex());
        tUnsigned32 minimumCostToGoal = goalConditions.minimumCostToGoal(targetState.pointIndex());
        cPathNode* node = _nodeHeap.get();
        node->initOrReinitAsExtensionNode(_fromNode, targetState, connectionCost, minimumCostToGoal);
        enqueue(node);
    }
    template <class tGraph, class tGoalConditions> void
    directlyGeneratedPotentialConnection(
			const tGraph& graph,
			const tGoalConditions& goalConditions,
			cPointsOfVisibilityState targetState,
            tUnsigned32 connectionCost
			)
    {
        if(_previousState.valid() && graph.pathCornerCanBeCollapsed(_previousState, _fromNode->_endState, targetState))
        {
            return;
        }
        tUnsigned32 minimumCostToGoal = goalConditions.minimumCostToGoal(targetState.pointIndex());
        cPathNode* node = _nodeHeap.get();
        node->initOrReinitAsExtensionNode(_fromNode, targetState, connectionCost, minimumCostToGoal);
        enqueue(node);
    }

    bool
    connectionsHaveBeenGeneratedFromPoint(tSigned32 i) const
    {
        return _nodeToPoint[i] != 0;
    }

    template <class tGraph> void
    additionalPotentialConnection(
			const tGraph& graph,
			cPointsOfVisibilityState from,
			cPointsOfVisibilityState to,
            tUnsigned32 minimumCostToGoal
			)
    {
        cPointsOfVisibilityState previousState = getParentState(from);
        if(previousState.valid() && graph.pathCornerCanBeCollapsed(previousState, from, to))
        {
            return;
        }
        cPathNode* prevNode = _nodeToPoint[from.pointIndex()];
        assertD(prevNode);
		tSigned32 connectionCost = graph.connectionCost(from.pointIndex(), to.pointIndex());
        cPathNode* node = _nodeHeap.get();
        node->initOrReinitAsExtensionNode(prevNode, to, connectionCost, minimumCostToGoal);
        enqueue(node);
    }

  // body of search algorithm

    template <class tGraph, class tGoalConditions> cPointsOfVisibilityState
    search(
            tGraph& graph,
            const tGoalConditions& goalConditions,
            cPointsOfVisibilityState startState
            )
    {
        clear();

        //_pointIsClosed.resize(0);
        //_pointIsClosed.resize(graph.points(), false);
        _pointIsClosed.resize(graph.points());
        _pointIsClosed.fill(false);

        _nodeToPoint.resize(0);
        _nodeToPoint.resize(graph.points(), 0);

        _fromNode = _nodeHeap.get();
        _fromNode->initOrReinitAsStartNode(startState, goalConditions.minimumCostToGoal(startState.pointIndex()));
        _closed.push_back(_fromNode);

//        _pointIsClosed[startState.pointIndex()] = true;
        _pointIsClosed.setBit(startState.pointIndex());
        _nodeToPoint[startState.pointIndex()] = _fromNode;

        _previousState.setInvalid();

        cPointsOfVisibilityState result;

        while(!goalConditions.isGoal(_fromNode->_endState))
        {
            graph.potentialConnectionsFrom(goalConditions, _fromNode->_endState, *this);

//            if(_open.empty())
            if(_openQueue.empty())
            {
              // failure
                result.setInvalid();
                return result;
            }

            //.. could prune dead branches here!

            cPathNode* candidate;
            do
            {
                candidate = _openQueue.popFront();
                _closed.push_back(candidate);
                // may be re-opened based on additional cost, below
				_fromNode = candidate->_prev;

                if(_pointIsClosed[candidate->_endState.pointIndex()])
                {
                  // try next candidate or fail
                    candidate = 0;
                    continue;
                }

                if(candidate->validated())
                {
                  // accept candidate as next _fromNode
                    break;
                }

                tUnsigned32 additionalCost;
                bool valid = graph.validateConnection(
                        goalConditions,
                        _fromNode->_endState, candidate->_endState,
                        additionalCost,
                        *this
                        );
                if(!valid)
                {
	              // try next candidate or fail
		            candidate = 0;
					continue;
                }

                candidate->validate(additionalCost);

			  // if there is any additional cost to add for this connection
			  // then add this here
              // and then re-insert in open list
                if(additionalCost)
                {
                    _closed.pop_back();
                    enqueue(candidate);
                    continue;
                }

              // accept candidate as next _fromNode
                break;			
			}
            while(!_openQueue.empty());

            if(candidate == 0)
            {
              // failure
                result.setInvalid();
                return result;
            }

            _fromNode = candidate;
            //_pointIsClosed[_fromNode->_endState.pointIndex()] = true;
            _pointIsClosed.setBit(_fromNode->_endState.pointIndex());
            _nodeToPoint[_fromNode->_endState.pointIndex()] = _fromNode;
            _previousState = _fromNode->_prev->_endState;
        }

        tSigned32 i;
        for(i = 0; i != _openQueue.size(); ++i)
        {
            _nodeHeap.release(_openQueue[i]);
        }
        _openQueue.clear();

        // success
        return _fromNode->_endState;
    }
};
