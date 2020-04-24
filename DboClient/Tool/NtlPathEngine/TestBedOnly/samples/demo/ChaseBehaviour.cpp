
#pragma warning(disable : 4786)

#include "ChaseBehaviour.h"
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

cChaseBehaviour::cChaseBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _activationSound = element.attributeAsLongWithDefault("activationSound", -1);
    _currentPath = 0;
    _timeSincePathQuery = 0;
    _engageDistance = element.attributeAsLongWithDefault("engageDistance", 0);
    _findClosestUnobstructedPointRange = element.attributeAsLongWithDefault("findClosestUnobstructedPointRange", 0);
}

cChaseBehaviour::~cChaseBehaviour()
{
    delete _currentPath;
}

bool
cChaseBehaviour::update(cGameObject& controlledObject)
{
    cGameObject* target = controlledObject.getTarget();
    if(!target)
    {
        return true; // failed
    }
    
    long frequency;
    if(_currentPath)
    {
        frequency = GetPathFrequency(_currentPath);
    }
    else
    {
        frequency = GetPathFrequency(controlledObject, *target);
    }

    bool wasActive = (_currentPath != 0);

    _timeSincePathQuery++;
    if(_timeSincePathQuery >= frequency)
    {
        _timeSincePathQuery = 0;
        delete _currentPath;
        _currentPath = 0;

        cPosition targetP = target->getPosition();
        if(_findClosestUnobstructedPointRange)
        {
            targetP = gGameState->getMesh()->findClosestUnobstructedPosition(
                                controlledObject.getShape(),
                                gGameState->getObstructionsContext(),
                                targetP,
                                _findClosestUnobstructedPointRange);
        }

        if(targetP.cell != -1)
        {
            _currentPath = controlledObject.findShortestPathTo_IgnoringOneObject(targetP, *target);
        }
    }

    if(!wasActive && _currentPath && _activationSound != -1)
    {
        gTestBed->playSound(_activationSound);
    }

    if(_currentPath)
    {
        float speedMultiplier = 1.f;
        if(_engageDistance)
        {
            if(_currentPath->getLength() < _engageDistance * 0.7f)
            {
                speedMultiplier = 0.f;
            }
            else
            if(_currentPath->getLength() < _engageDistance * 1.3f)
            {
                speedMultiplier = (_currentPath->getLength() - _engageDistance * 0.7f) / (_engageDistance * 0.6f);
            }
        }
        else
        if(frequency == 1)
        {
        // add a speed boost when close in so the AI can really go for the throat
            speedMultiplier = 1.15f;
        }

        bool blocked = controlledObject.advanceAlongPath(_currentPath, speedMultiplier);
        if(blocked)
        {
            delete _currentPath;
            _currentPath = 0;
        }
    }
    else
    {
        controlledObject.faceTowards(target->getPosition());
    }
    return false;
}
