
#pragma warning(disable : 4786)

#include "UIController.h"
#include "Globals.h"
#include "GameState.h"
#include "GameObject.h"
#include "MoveAgent.h"
#include "SimpleDOM.h"
#include "i_testbed.h"
#include <math.h>
#include <stdlib.h>

using std::string;

cUIController::cUIController(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _findClosestUnobstructedPointRange = element.attributeAsLong("findClosestUnobstructedPointRange");
    _currentPath = 0;
}

bool
cUIController::update(cGameObject& controlledObject)
{
    cPosition clickedPosition;

  // receive and process messages for all keys pressed since last frame
    const char* keyPressed;
    while(keyPressed = gTestBed->receiveKeyMessage())
    {
        if(keyPressed[0] != 'd') // is it a key down message?
        {
            continue;
        }
        switch(keyPressed[1])
        {
        case '_':
            {
                if(!strcmp("LMOUSE", keyPressed + 2))
                {
                    clickedPosition = gTestBed->positionAtMouse();
                    break;
                }
                break;
            }
        }
    }

    if(gGameState->uiControlIsFrozen())
    {
        _currentTarget.cell = -1;
        if(_currentPath)
        {
            delete _currentPath;
            _currentPath = 0;
        }
        return false;
    }

    if(clickedPosition.cell != -1)
    {
    // the user clicked on a valid point on the mesh since last frame
        delete _currentPath;
        _currentPath = 0;
        _currentTarget = gGameState->getMesh()->findClosestUnobstructedPosition(
                            controlledObject.getShape(),
                            gGameState->getObstructionsContext(),
                            clickedPosition,
                            _findClosestUnobstructedPointRange);
        if(_currentTarget.cell != -1)
        {
            _currentPath = controlledObject.findShortestPathTo(_currentTarget);
        }
    }

    float moveX, moveY;
    GetMovementVector(gTestBed, moveX, moveY);
    if(moveX != 0.0f || moveY != 0.0f)
    {
    // key movement requested
        _currentTarget.cell = -1;
        if(_currentPath)
        {
            delete _currentPath;
            _currentPath = 0;
        }

        // agent is faced away from camera
        controlledObject._heading = gGameState->getCameraHeading() + 3.14f;

        float vectorLength = static_cast<float>(sqrt(moveX * moveX + moveY * moveY));
        if(vectorLength > 1.f)
        {
            moveX /= vectorLength;
            moveY /= vectorLength;
        }

        float speedMultiplier = 1.08f; // slight speed boost to favour direct control
        controlledObject.moveUnderDirectControl(moveX, moveY, speedMultiplier);
    }
    else
    if(_currentPath)
    {
    // following a path
        if(_currentPath->size() >= 2)
        {
        // set heading for the current path section
            cPosition nextTarget = _currentPath->position(1);
            cPosition current = controlledObject.getPosition();
            long dx,dy;
            dx = nextTarget.x - current.x;
            dy = nextTarget.y - current.y;
            controlledObject._heading = static_cast<float>(atan2(static_cast<double>(dx), static_cast<double>(dy)));
        }

        bool blocked = controlledObject.advanceAlongPath(_currentPath);
        if(blocked)
        {
            delete _currentPath;
            _currentPath = 0;
            _currentTarget.cell = -1;
        }
        else
        if(_currentPath->size() == 1)
        {
            delete _currentPath;
            _currentPath = 0;
            _currentTarget.cell = -1;
        }
    }
    else
    {
        // face agent away from camera
        controlledObject._heading = gGameState->getCameraHeading() + 3.14f;
    }
    return false;
}

void
cUIController::draw(cGameObject& controlledObject)
{
    if(_currentTarget.cell != -1)
    {
        if(_currentPath)
        {
            gTestBed->setColour("green");
        }
        else
        {
            gTestBed->setColour("red");
        }
        gTestBed->drawPosition(_currentTarget, 30);
    }
}
