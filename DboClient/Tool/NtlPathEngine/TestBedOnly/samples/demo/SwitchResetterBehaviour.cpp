
#pragma warning(disable : 4786)

#include "SwitchResetterBehaviour.h"
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

cGameObject*
cSwitchResetterBehaviour::chooseRandomTarget() const
{
    size_t numberToggled = 0;
    size_t i;
    for(i = 0; i < _targets.size(); ++i)
    {
        if(_targets[i]->_toggled)
        {
            numberToggled++;
        }
    }
    if(numberToggled == 0)
    {
        return 0;
    }
    size_t selected = rand() % numberToggled;
    for(i = 0; i < _targets.size(); ++i)
    {
        if(_targets[i]->_toggled)
        {
            if(selected == 0)
            {
                return _targets[i];
            }
            selected--;
        }
    }
    invalid();
    return 0;
}

cSwitchResetterBehaviour::cSwitchResetterBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _radius1 = element.attributeAsLong("radius1");
    long radius2 = element.attributeAsLong("radius2");
    _radius3 = element.attributeAsLong("radius3");
    assertD(_radius1 > 0);
    assertD(radius2 > _radius1);
    assertD(_radius3 > radius2);

    _frightenedSound = element.attributeAsLongWithDefault("frightenedSound", -1);

    size_t i;
    for(i = 0; i < element._children.size(); ++i)
    {
        const cSimpleDOM& child = element._children[i];
        if(child._name == "target")
        {
            std::string targetName = child.getAttribute("object");
            _targets.push_back(gGameState->findObject(targetName));
        }
    }

    iShape* runAwayAreaShape;
    {
        long vertices[8];
        vertices[0] = -_radius1;
        vertices[1] = -_radius1;
        vertices[2] = -_radius1;
        vertices[3] = _radius1;
        vertices[4] = _radius1;
        vertices[5] = _radius1;
        vertices[6] = _radius1;
        vertices[7] = -_radius1;
        runAwayAreaShape = gPathEngine->newShape(4, vertices);
    }
    _runAwayArea = gGameState->getMesh()->placeAgent(runAwayAreaShape, controlledObject.getPosition());
   
    _runAwayContext = gGameState->getMesh()->newContext();
    _runAwayContext->addAgent(_runAwayArea);

    iShape* avoidAreaShape;
    {
        long vertices[8];
        vertices[0] = -radius2;
        vertices[1] = -radius2;
        vertices[2] = -radius2;
        vertices[3] = radius2;
        vertices[4] = radius2;
        vertices[5] = radius2;
        vertices[6] = radius2;
        vertices[7] = -radius2;
        avoidAreaShape = gPathEngine->newShape(4, vertices);
    }
    _avoidArea = gGameState->getMesh()->placeAgent(avoidAreaShape, controlledObject.getPosition());

    _currentPath = 0;
    _timeSincePathQuery = 0;
    _state = IDLE;
}

cSwitchResetterBehaviour::~cSwitchResetterBehaviour()
{
    delete _runAwayArea;
    delete _runAwayContext;
    delete _avoidArea;
    delete _currentPath;
}

bool
cSwitchResetterBehaviour::update(cGameObject& controlledObject)
{
    cGameObject* target = gGameState->findObject("player");
    assertR(target);

    float distance = controlledObject.distanceTo(*target);
    if(distance < _radius1 && _state != RUNNING_AWAY)
    {
        _runAwayArea->moveTo(target->getPosition());
        if(controlledObject.getAgent()->testCollisionAt(_runAwayContext, controlledObject.getPosition()))
        {
            if(_currentPath)
            {
                delete _currentPath;
                _currentPath = 0;
            }
            _state = RUNNING_AWAY;
            if(_frightenedSound != -1)
            {
                gTestBed->playSound(_frightenedSound);
            }
        }
    }
    else
    if(distance > _radius3 && _state != MOVING_TO_SWITCHES)
    {
        if(_currentPath)
        {
            delete _currentPath;
            _currentPath = 0;
        }
        _state = MOVING_TO_SWITCHES;
    }

    if(_state == IDLE)
    {
        return false;
    }

    _timeSincePathQuery++;
    if(_timeSincePathQuery >= 10 && (_state == RUNNING_AWAY || _currentPath == 0))
    {
        _timeSincePathQuery = 0;
        if(_currentPath)
        {
            delete _currentPath;
            _currentPath = 0;
        }
        if(_state == RUNNING_AWAY)
        {
            _currentPath = controlledObject.findPathAway(target->getPosition(), _radius3 + 1);
        }
        else
        {
            assertD(_state == MOVING_TO_SWITCHES);
            _currentTarget = chooseRandomTarget();
            if(_currentTarget)
            {
                _avoidArea->moveTo(target->getPosition());
                gGameState->getObstructionsContext()->addAgent(_avoidArea);
                if(!controlledObject.getAgent()->testCollisionAt(gGameState->getObstructionsContext(), _currentTarget->getPosition()))
                {
                    _currentPath = controlledObject.findShortestPathTo(_currentTarget->getPosition());
                }
                gGameState->getObstructionsContext()->removeAgent(_avoidArea);
            }
        }
    }

    if(_currentPath)
    {
        bool blocked = controlledObject.advanceAlongPath(_currentPath);
        if(blocked || _currentPath->size() == 1)
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

