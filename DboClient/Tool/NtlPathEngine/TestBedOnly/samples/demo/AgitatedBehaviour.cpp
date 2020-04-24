
#pragma warning(disable : 4786)

#include "AgitatedBehaviour.h"
#include "SimpleDOM.h"
#include "GameState.h"
#include "GameObject.h"
#include "Error.h"
#include "i_pathengine.h"
#include "Globals.h"
#include <math.h>

void
cAgitatedBehaviour::randomiseHeading(cGameObject& controlledObject)
{
// generate random value in the range -1.f to 1.f
    assertD(RAND_MAX >= 511);
    float randomValue = static_cast<float>(rand() & 511);
    randomValue /= (511.f / 2.f);
    randomValue -= 1.f;

// generate a heading in range around the central heading
    float heading = _centralHeading + _angularRange * randomValue;

// precalculate sin and cos for current heading and speed
    float speed = controlledObject.getSpeed();
    _dx = static_cast<float>(speed * sin(double(heading)));
    _dy = static_cast<float>(speed * cos(double(heading)));

    controlledObject._heading = heading;
}

cAgitatedBehaviour::cAgitatedBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _radius = element.attributeAsFloat("radius");
    _angularRange = element.attributeAsFloatWithDefault("angularRange", 0.8f);
    _centralHeading = controlledObject._heading;
    randomiseHeading(controlledObject);
    _distanceToMove = _radius;
    _state = MOVING_FORWARDS1;
}

bool
cAgitatedBehaviour::update(cGameObject& controlledObject)
{
    controlledObject.moveInDirection(_dx, _dy);
    _distanceToMove -= controlledObject.getSpeed();
    if(_distanceToMove <= 0.f)
    {
        switch(_state)
        {
        default:
            invalid();
        case MOVING_FORWARDS1:
            _dx = -_dx;
            _dy = -_dy;
            _distanceToMove = _radius * 2.f;
            _state = MOVING_BACK;
            break;
        case MOVING_BACK:
            _dx = -_dx;
            _dy = -_dy;
            _distanceToMove = _radius;
            _state = MOVING_FORWARDS2;
            break;
        case MOVING_FORWARDS2:
            randomiseHeading(controlledObject);
            _distanceToMove = _radius;
            _state = MOVING_FORWARDS1;
            break;
        }
    }
    return false;
}
