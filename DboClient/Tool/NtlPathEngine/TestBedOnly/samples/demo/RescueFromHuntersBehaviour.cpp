
#pragma warning(disable : 4786)

#include "RescueFromHuntersBehaviour.h"
#include "Error.h"
#include "Globals.h"
#include "GameState.h"
#include "GameObject.h"
#include "MoveAgent.h"
#include "SimpleDOM.h"
#include "PathFrequency.h"
#include "i_testbed.h"
#include <math.h>
#include <stdlib.h>

using std::string;

std::vector<cGameObject*> cRescueFromHuntersBehaviour::_targets;
std::vector<cQueueManager> cRescueFromHuntersBehaviour::_queueManagers;

long
cRescueFromHuntersBehaviour::findClosestTarget(cGameObject& controlledObject)
{
    long best = -1;
    float bestDistance;
    size_t i;
    for(i = 0; i < _targets.size(); ++i)
    {
        if(_targets[i]->_toggled)
        {
            continue;
        }
        float distance = controlledObject.distanceTo(*_targets[i]);
        if(best == -1 || distance < bestDistance)
        {
            best = i;
            bestDistance = distance;
        }
    }
    return best;
}

cRescueFromHuntersBehaviour::cRescueFromHuntersBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    if(_targets.empty())
    {
        gGameState->findAllObjectsWithType("hunter", _targets);
        _queueManagers.resize(_targets.size());
    }

    _queueShape = element.attributeAsLong("queueCollisionShape");

    _targetIndex = -1;
    _queueEntry = 0;

    _currentPath = 0;
    _timeSincePathQuery = 0;
}

cRescueFromHuntersBehaviour::~cRescueFromHuntersBehaviour()
{
    delete _queueEntry;
    delete _currentPath;
}

bool
cRescueFromHuntersBehaviour::update(cGameObject& controlledObject)
{
    {
        long closestTarget = findClosestTarget(controlledObject);
        if(closestTarget != _targetIndex)
        {
            delete _queueEntry;
            _queueEntry = 0;
            _targetIndex = closestTarget;
            if(_targetIndex != -1)
            {
                _queueEntry = new cQueueEntry(_queueManagers[_targetIndex], _queueShape, controlledObject.getAgent());
            }
        }
    }

    if(_targetIndex == -1)
    {
        return true;
    }

    cGameObject* target = _targets[_targetIndex];

// update position in pack order
    _queueEntry->updateDistanceToTarget(controlledObject.distanceTo(*target));

    _timeSincePathQuery++;

    float speedMultiplier = 1.f;

    if(controlledObject.distanceTo(*target) < 500)
    {
        iPath* straightLinePath = controlledObject.findStraightLinePathTo(target->getPosition());
        if(straightLinePath)
        {
            _timeSincePathQuery = 0;
            delete _currentPath;
            _currentPath = straightLinePath;
            speedMultiplier = 1.4f;
        }
    }

    if(_timeSincePathQuery >= 9)
    {
        _timeSincePathQuery = 0;
        delete _currentPath;
        _currentPath = controlledObject.findShortestPathTo(target->getPosition());
    }

    if(!_currentPath)
    {
        return false;
    }

    if(_queueEntry->testQueueCollision())
    {
        speedMultiplier = 0.7f;
    }

    bool blocked = controlledObject.advanceAlongPath(_currentPath);
    if(blocked)
    {
        delete _currentPath;
        _currentPath = 0;
    }
    return false;
}
