
#pragma warning(disable : 4786)

#include "WanderBehaviour.h"
#include "Error.h"
#include "Globals.h"
#include "GameState.h"
#include "GameObject.h"
#include "MoveAgent.h"
#include "SimpleDOM.h"
#include "i_testbed.h"
#include <math.h>
#include <stdlib.h>

using std::string;

cWanderBehaviour::cWanderBehaviour(const cSimpleDOM& element)
{
    _currentPath = 0;
}

cWanderBehaviour::~cWanderBehaviour()
{
    delete _currentPath;
}

bool
cWanderBehaviour::update(cGameObject& controlledObject)
{
    if(!_currentPath || _currentPath->size() < 2)
    {
        cPosition targetPosition;
        long i;
        for(i = 0; i < 10; ++i)
        {
            targetPosition = gameState._mesh->generateRandomPosition();
            if(targetPosition.cell == -1)
            {
                continue;
            }
            if(controlledObject._agent->testCollisionAt(gameState._obstructionsContext, targetPosition))
            {
                continue;
            }
            break;
        }
        if(i == 10)
        {
            return false;
        }
        delete _currentPath;
        _currentPath = controlledObject._agent->findShortestPathTo(gameState._obstructionsContext, targetPosition);
    }

    if(_currentPath->size() >= 2)
    {
        cPosition nextTarget = _currentPath->position(1);
        cPosition current = _currentPath->position(0);
        long dx, dy;
        dx = nextTarget.x - current.x;
        dy = nextTarget.y - current.y;
        controlledObject._heading = static_cast<float>(atan2(dx, dy));
    }

    iCollisionInfo* ci = controlledObject._agent->advanceAlongPath(_currentPath, controlledObject._speed, gameState._obstructionsContext);
    if(ci)
    {
        delete ci;
        delete _currentPath;
        _currentPath = 0;
    }
    return false;
}
