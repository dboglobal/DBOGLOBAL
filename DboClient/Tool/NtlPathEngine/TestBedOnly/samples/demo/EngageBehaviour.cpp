
#pragma warning(disable : 4786)

#include "EngageBehaviour.h"
#include "SimpleDOM.h"
#include "GameState.h"
#include "GameObject.h"
#include "Error.h"
#include "i_pathengine.h"
#include "Globals.h"
#include <math.h>

cEngageBehaviour::cEngageBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _distance = element.attributeAsFloatWithDefault("distance", 0.f);
    assertD(_distance >= 0);
    _path = 0;
}
cEngageBehaviour::~cEngageBehaviour()
{
    delete _path;
}

bool
cEngageBehaviour::update(cGameObject& controlledObject)
{
    cGameObject* target = controlledObject.getTarget();
    if(!target)
    {
        return true; // failed
    }

    if(!_path)
    {
        _path = controlledObject.findShortestPathTo_IgnoringOneObject(target->getPosition(), *target);
        if(!_path)
        {
            return true; // failed
        }
    }

    bool blocked = controlledObject.advanceAlongPath(_path);
    if(blocked)
    {
        delete _path;
        _path = 0;
        return true; // failed
    }

    if(_path->getLength() <= _distance)
    {
        return true; // succeeded
    }
    if(_path->size() == 1)
    {
        return true; // succeeded
    }
    return false;
}
