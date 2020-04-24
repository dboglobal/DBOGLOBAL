
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

cWanderBehaviour::cWanderBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _currentPath = 0;
    _range = element.attributeAsLongWithDefault("range", -1);
    assertD(_range >= -1);
    _objectToUseAsCentre = element.getAttribute("objectToUseAsCentre");
    _forwardBias = element.attributeAsFloatWithDefault("forwardBias", 0.f);
}

cWanderBehaviour::~cWanderBehaviour()
{
    delete _currentPath;
}

bool
cWanderBehaviour::update(cGameObject& controlledObject)
{
    if(!_currentPath)
    {
        cPosition targetPosition;
        long i;
        for(i = 0; i < 10; ++i)
        {
            if(_range == -1)
            {
                targetPosition = gGameState->getMesh()->generateRandomPosition();
            }
            else
            {
                cPosition centre;
                if(_objectToUseAsCentre.empty())
                {
                    centre = controlledObject.getPosition();
                }
                else
                {
                    cGameObject* object = gGameState->findObject(_objectToUseAsCentre);
                    centre = object->getPosition();
                }
                if(_forwardBias != 0.f)
                {
                    cPosition biasedCentre = centre;
                    double sinOf = sin(double(controlledObject._heading));
                    double cosOf = cos(double(controlledObject._heading));
                    biasedCentre.x += static_cast<long>(sinOf * _forwardBias);
                    biasedCentre.y += static_cast<long>(cosOf * _forwardBias);
                    biasedCentre.cell = gGameState->getMesh()->getCellForEndOfLine(centre, biasedCentre.x, biasedCentre.y);
                    if(biasedCentre.cell != -1)
                    {
                        centre = biasedCentre;
                    }
                }
                targetPosition = gGameState->getMesh()->generateRandomPositionLocally(centre, _range);
            }
            if(targetPosition.cell == -1)
            {
                continue;
            }
            if(controlledObject.getAgent()->testCollisionAt(gGameState->getObstructionsContext(), targetPosition))
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
        _currentPath = controlledObject.findShortestPathTo(targetPosition);
        if(!_currentPath)
        {
            return false;
        }
    }

    bool blocked = controlledObject.advanceAlongPath(_currentPath);
    if(blocked || _currentPath->size() == 1)
    {
        delete _currentPath;
        _currentPath = 0;
    }
    return false;
}
