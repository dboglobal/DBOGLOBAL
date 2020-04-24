
#pragma warning(disable : 4786)

#include "GameState.h"
#include "Error.h"
#include "SimpleDOM.h"
#include "GameObject.h"
#include "Globals.h"
#include "iBehaviour.h"
#include "i_testbed.h"
#include "Resources.h"
#include "MoveAgent.h"
#include <string.h>
#include <math.h>

using std::string;

cGameState::cGameState(const cSimpleDOM& templates, const cSimpleDOM& levelScript)
{
    _frameScript = &levelScript.refFirstChildWithName("frame");

    assertD(gGameState == 0);
    gGameState = this;

    {
        char* buffer;
        unsigned long size;
        string fileName = "..\\resource\\meshes\\";
        fileName += "demo_";
        fileName += levelScript.getAttribute("mesh");
        fileName += ".tok";
        buffer = gTestBed->loadBinary(fileName.c_str(), size);
        _mesh = gPathEngine->loadMeshFromBuffer("tok", buffer, size, 0);
        gTestBed->freeBuffer(buffer);
    }

    gTestBed->setMesh(gGameState->_mesh);
    gTestBed->zoomExtents();

    _switchesContext = _mesh->newContext();
    _obstructionsContext = _mesh->newContext();
    _collisionCheckersContext = _mesh->newContext();

    long i;
    for(i = 0; i < _mesh->getNumberOfAnchors(); i++)
    {
        cPosition position;
        long orientationL;
        const char* id;
        _mesh->retrieveAnchorByIndex(i, position, orientationL, id);
        if(strncmp(id, "place.", 6) == 0)
        {
            float orientationF = static_cast<float>(orientationL);
            orientationF *= 6.283185f;
            orientationF /= 1048576.0f;
            _objects.push_back(new cGameObject(_objects.size(), _mesh, position, templates, id + 6, orientationF));
        }
    }
    for(i = 0; i < _mesh->getNumberOfNamedObstacles(); i++)
    {
        iAgent* agent;
        const char* id;
        _mesh->retrieveNamedObstacleByIndex(i, agent, id);
        _objects.push_back(new cGameObject(_objects.size(), templates, id, agent));
    }

  // add objects that should act as switches to the switches context
  // add fixed obstacles to a temporary context - these will be burnt in
  // the 'collision checkers' context is used when moving agents in the obstructions context

    {
        iCollisionContext* burnInContext = _mesh->newContext();
        for(i = 0; i < static_cast<long>(_objects.size()); i++)
        {
            const cSimpleDOM& initTemplate = _objects[i]->refTemplate();
            if(initTemplate.getAttribute("isSwitch") == "true")
            {
                _switchesContext->addAgent(_objects[i]->getAgent());
            }
            if(initTemplate.getAttribute("burnIn") == "true")
            {
                burnInContext->addAgent(_objects[i]->getAgent());
            }
            if(initTemplate.getAttribute("isCollisionChecker") == "true")
            {
                _collisionCheckersContext->addAgent(_objects[i]->getAgent());
            }
            if(initTemplate.getAttribute("isPushable") == "true")
            {
                _obstructionsContext->addAgent(_objects[i]->getAgent());
            }
        }
        _mesh->burnContextIntoMesh(burnInContext);
        delete burnInContext;
    }

  // doors are represented by two objects, for the open and closed states
  // the states are encoded into the name (together with a base name)
  // doors are initialised as closed

    for(i = 0; i < static_cast<long>(_objects.size()); i++)
    {
        string::size_type pos = _objects[i]->name().find(":");
        if(pos != string::npos)
        {
            string state(_objects[i]->name().c_str() + pos + 1);
            if(state == "closed")
            {
                _obstructionsContext->addAgent(_objects[i]->getAgent());
            }
            else
            if(state == "open")
            {
                _objects[i]->_visible = false;
            }
        }
    }

    for(i = 0; i < static_cast<long>(_objects.size()); ++i)
    {
        const cSimpleDOM& initTemplate = _objects[i]->refTemplate();
        if(initTemplate.getAttribute("requiresCollisionPreprocess") == "true")
        {
            if(!gGameState->getMesh()->shapeCanCollide(_objects[i]->getShape()))
            {
                gGameState->getMesh()->generateCollisionPreprocessFor(_objects[i]->getShape(), 0);
            }
        }
        if(initTemplate.getAttribute("requiresPathfindPreprocess") == "true")
        {
            if(!gGameState->getMesh()->shapeCanCollide(_objects[i]->getShape()))
            {
                gGameState->getMesh()->generateCollisionPreprocessFor(_objects[i]->getShape(), 0);
            }
            if(!gGameState->getMesh()->shapeCanPathfind(_objects[i]->getShape()))
            {
                gGameState->getMesh()->generatePathfindPreprocessFor(_objects[i]->getShape(), 0);
            }
        }
    }

    _agentsOverlappedBuffer.resize(_objects.size());

    _cameraDistance = 3300.0f;
    _cameraHeading = 3.14f;
    _cameraElevation = 1.0f;
    
    _lookAtActive = false;
    _endLevel = false;

// a final stage of object initialisation is performed once game state initialisation is complete
// this includes creation of default behaviour, setting default target, etc

    for(i = 0; i < static_cast<long>(_objects.size()); i++)
    {
        _objects[i]->completeInitialisation();
    }

}

cGameState::~cGameState()
{
    size_t i;
    for(i = 0; i < _objects.size(); i++)
    {
        delete _objects[i];
    }
    delete _mesh;
}

cGameObject*
cGameState::findObject(const string& contentName) const
{
    size_t i;
    for(i = 0; i < _objects.size(); i++)
    {
        if(_objects[i]->contentName() == contentName)
        {
            return _objects[i];
        }
    }
    const char* attributes[3];
    attributes[0] = "object_name";
    attributes[1] = contentName.c_str();
    attributes[2] = 0;
    Error("Fatal", "An object requested by the script was not found.", attributes);
    return 0;
}
void
cGameState::findAllObjectsWithType(const std::string& type, std::vector<cGameObject*>& result) const
{
    long i;
    for(i = 0; i < static_cast<long>(_objects.size()); ++i)
    {
        if(_objects[i]->type() == type)
        {
            result.push_back(_objects[i]);
        }
    }
}

cGameObject*
cGameState::resolveObjectArgument(const cSimpleDOM& command) const
{
    string objectName = command.getAttribute("object");
    if(objectName.empty())
    {
        assertD(gCurrentObject);
        return gCurrentObject;
    }
    return findObject(objectName);
}

void
cGameState::updateCollisionFor(cGameObject& object)
{
    if(object.collisionFlagsAreValid())
    {
        return;
    }
    object.initialiseCollisionFlags();
    long numberOverlapped;
    object.getAgent()->getAllAgentsOverlapped(_switchesContext, &_agentsOverlappedBuffer.front(), numberOverlapped);
    long i;
    for(i = 0; i < numberOverlapped; i++)
    {
        cGameObject* overlapped = reinterpret_cast<cGameObject*>(_agentsOverlappedBuffer[i]->getUserData());
        object.setCollisionFlag(*overlapped);
    }
}

void
cGameState::update()
{
    size_t i;
    for(i = 0; i < _objects.size(); i++)
    {
        _objects[i]->update();
        _objects[i]->invalidateCollisionFlags();
    }
    _lookAtActive = false;
    for(i = 0; i < _frameScript->_children.size(); i++)
    {
        gGameState->execute(_frameScript->_children[i]);
    }
    if(gTestBed->getRightMouseState())
    {
    // mouse movement controls the camera
        TurnAgentUnderMouseControl(gTestBed, _cameraHeading, _cameraElevation);
    }
}

void
cGameState::draw()
{
    gTestBed->setColourRGB(0.0f,0.0f,0.85f);
    gTestBed->drawMesh();
    gTestBed->drawMeshEdges();
    size_t i;
    for(i = 0; i < gGameState->_objects.size(); i++)
    {
        gGameState->_objects[i]->draw();
    }

    static bool freeCameraOn = false;
    if(gTestBed->getKeyState("C"))
    {
        freeCameraOn = gTestBed->getKeyState("_LSHIFT");
    }

    if(_lookAtActive)
    {
        gTestBed->displaySplashImage(0, 0, cResources::SPLASH_CAMERA);
    }            
    else if(!freeCameraOn)
    {
        cGameObject* player = gGameState->findObject("player");
        if(player)
        {
            gTestBed->lookAtWithPrecision(player->getPosition(), player->getPrecisionX(), player->getPrecisionY(), _cameraHeading, _cameraElevation, _cameraDistance);
        }
    }
}

bool
cGameState::levelCompleted(std::string& result)
{
    if(_endLevel)
    {
        result = _result;
        return true;
    }
    return false;
}

void
cGameState::execute(const cSimpleDOM& command)
{
    if(command._name == "setVisible")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_visible = true;
    }
    else
    if(command._name == "setInvisible")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_visible = false;
    }
    else
    if(command._name == "setColour")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_colour = command.getAttribute("value");
    }
    else
    if(command._name == "setToggledColour")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_toggledColour = command.getAttribute("value");
    }
    else
    if(command._name == "setToggled")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_toggled = true;
    }
    else
    if(command._name == "clearToggled")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->_toggled = false;
    }
    else
    if(command._name == "toggleDoorState")
    {
        string openName("door.");
        openName += command.getAttribute("door");
        openName += ":";
        string closedName(openName);
        openName += "open";
        closedName += "closed";
        cGameObject* open = findObject(openName);
        cGameObject* closed = findObject(closedName);
        if(open->_visible)
        {
            open->_visible = false;
            _obstructionsContext->removeAgent(open->getAgent());
            closed->_visible = true;
            _obstructionsContext->addAgent(closed->getAgent());
        }
        else
        {
            open->_visible = true;
            _obstructionsContext->addAgent(open->getAgent());
            closed->_visible = false;
            _obstructionsContext->removeAgent(closed->getAgent());
        }
    }
    else
    if(command._name == "setDoorOpen")
    {
        string openName("door.");
        openName += command.getAttribute("door");
        openName += ":";
        string closedName(openName);
        openName += "open";
        closedName += "closed";
        cGameObject* open = findObject(openName);
        cGameObject* closed = findObject(closedName);
        if(closed->_visible)
        {
            open->_visible = true;
            _obstructionsContext->addAgent(open->getAgent());
            closed->_visible = false;
            _obstructionsContext->removeAgent(closed->getAgent());
        }
    }
    else
    if(command._name == "setDoorClosed")
    {
        string openName("door.");
        openName += command.getAttribute("door");
        openName += ":";
        string closedName(openName);
        openName += "open";
        closedName += "closed";
        cGameObject* open = findObject(openName);
        cGameObject* closed = findObject(closedName);
        if(open->_visible)
        {
            open->_visible = false;
            _obstructionsContext->removeAgent(open->getAgent());
            closed->_visible = true;
            _obstructionsContext->addAgent(closed->getAgent());
        }
    }
    else
    if(command._name == "setFlag")
    {
        _flags[command.getAttribute("flag")] = command.attributeAsLongWithDefault("value", 1);
    }
    else
    if(command._name == "clearFlag")
    {
        _flags[command.getAttribute("flag")] = 0;
    }
    else
    if(command._name == "decrement")
    {
        long& ref = _flags[command.getAttribute("flag")];
        if(ref)
        {
            ref--;
        }
    }
    else
    if(command._name == "setSpeed")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->setSpeed(command.attributeAsFloat("value"));
    }
    else
    if(command._name == "endLevel")
    {
        _endLevel = true;
        _result = command.getAttribute("result");
    }
    else
    if(command._name == "setBehaviour")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->setBehaviour(command);
    }
    else
    if(command._name == "setDefaultBehaviour")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->setDefaultBehaviour();
    }
    else
    if(command._name == "setTarget")
    {
        cGameObject* object = resolveObjectArgument(command);
        object->setTarget(findObject(command.getAttribute("target")));
    }
    else

// conditional, looping, or programmatical constructs

    if(command._name == "if")
    {
        assertR(command._children.size() > 1);
        if(evaluate(command._children[0]))
        {
            size_t i;
            for(i = 1; i < command._children.size(); i++)
            {
                if(command._children[i]._name != "else")
                {
                    execute(command._children[i]);
                }
            }
        }
        else
        {
            size_t i;
            for(i = 1; i < command._children.size(); i++)
            {
                if(command._children[i]._name != "else")
                {
                    continue;
                }
                size_t j;
                for(j = 0; j < command._children[i]._children.size(); j++)
                {
                    execute(command._children[i]._children[j]);
                }
            }
        }
    }
    else
    if(command._name == "forAllOfType")
    {
        cGameObject* toRestore = gCurrentObject;
        std::string type = command.getAttribute("type");
        long i;
        for(i = 0; i < static_cast<long>(_objects.size()); ++i)
        {
            cGameObject* object = gGameState->_objects[i];
            if(object->type() == type)
            {
                gCurrentObject = object;
                size_t j;
                for(j = 0; j < command._children.size(); j++)
                {
                    execute(command._children[j]);
                }
            }
        }
        gCurrentObject = toRestore;
    }
    else
    if(command._name == "commented")
    {
    }
    else

// commands that feed through directly to the testbed or rendering parameters

    if(command._name == "playSound")
    {
        gTestBed->playSound(command.attributeAsLong("index"));
    }
    else
    if(command._name == "lookAt")
    {
        cGameObject* object = resolveObjectArgument(command);
        cPosition target = object->getPosition();
        float angle;
        if(command.hasAttribute("faceTowards"))
        {
            cGameObject* faceTowardsObject = findObject(command.getAttribute("faceTowards"));
            cPosition faceTowardsPosition = faceTowardsObject->getPosition();
            long dx = target.x - faceTowardsPosition.x;
            long dy = target.y - faceTowardsPosition.y;
            angle = static_cast<float>(atan2(static_cast<double>(dx), static_cast<double>(dy)));
        }
        else
        {
            angle = command.attributeAsFloatWithDefault("angle", 0.f);
        }
        gTestBed->lookAt(target, angle,
                         command.attributeAsFloatWithDefault("elevation", 1.f),
                         command.attributeAsFloatWithDefault("distance", 3300.f)
                         );
        _lookAtActive = true;
    }
    else
    if(command._name == "setCameraDistance")
    {
        _cameraDistance = command.attributeAsFloat("value");
    }
    else

// command not recognised

    {
        const char* attributes[3];
        attributes[0] = "tag";
        attributes[1] = command._name.c_str();
        attributes[2] = 0;
        Error("NonFatal", "Unknown command tag. Skipping.", attributes);
    }
}

bool
cGameState::evaluate(const cSimpleDOM& expression)
{
    if(expression._name == "and")
    {
        assertR(expression._children.size() >= 2);
        size_t i;
        for(i = 0; i < expression._children.size(); i++)
        {
            if(!evaluate(expression._children[i]))
            {
                return false;
            }
        }
        return true;
    }
    if(expression._name == "or")
    {
        assertR(expression._children.size() >= 2);
        size_t i;
        for(i = 0; i < expression._children.size(); i++)
        {
            if(evaluate(expression._children[i]))
            {
                return true;
            }
        }
        return false;
    }
    if(expression._name == "not")
    {
        assertR(expression._children.size() == 1);
        return !evaluate(expression._children[0]);
    }
    if(expression._name == "overlaps")
    {
        cGameObject* tester = resolveObjectArgument(expression);
        cGameObject* target = findObject(expression.getAttribute("target"));
        assertR(tester);
        assertR(target);
        updateCollisionFor(*tester);
        return tester->testCollisionFlag(*target);
    }
    if(expression._name == "flagSet")
    {
        return _flags[expression.getAttribute("flag")] != 0;
    }
    if(expression._name == "flagNotSet")
    {
        return _flags[expression.getAttribute("flag")] == 0;
    }
    if(expression._name == "flagEquals")
    {
        return _flags[expression.getAttribute("flag")] == expression.attributeAsLong("value");
    }
    if(expression._name == "flagInRange")
    {
        long value = _flags[expression.getAttribute("flag")];
        return value >= expression.attributeAsLong("rangeStart") && value <= expression.attributeAsLong("rangeEnd");
    }
    if(expression._name == "isToggled")
    {
        cGameObject* object = resolveObjectArgument(expression);
        return object->_toggled;
    }
    if(expression._name == "isNotToggled")
    {
        cGameObject* object = resolveObjectArgument(expression);
        return !object->_toggled;
    }
    if(expression._name == "isVisible")
    {
        cGameObject* object = resolveObjectArgument(expression);
        return object->_visible;
    }
    if(expression._name == "isInvisible")
    {
        cGameObject* object = resolveObjectArgument(expression);
        return !object->_visible;
    }
    if(expression._name == "doorIsOpen")
    {
        string openName("door.");
        openName += expression.getAttribute("door");
        openName += ":open";
        cGameObject* open = findObject(openName);
        return open->_visible;
    }
    if(expression._name == "doorIsClosed")
    {
        string openName("door.");
        openName += expression.getAttribute("door");
        openName += ":open";
        cGameObject* open = findObject(openName);
        return !open->_visible;
    }
    if(expression._name == "isObstruction")
    {
        cGameObject* object = resolveObjectArgument(expression);
        return _obstructionsContext->includes(object->getAgent());
    }
    if(expression._name == "inRange")
    {
        cGameObject* object = resolveObjectArgument(expression);
        cGameObject* object2 = findObject(expression.getAttribute("object2"));
        long range = expression.attributeAsLong("range");
        assertD(range >= 0);
        long distance = static_cast<long>(object->distanceTo(*object2));
        if(distance < 0)
        {
            distance = -distance;
        }
        return distance <= range;
    }

// expression not recognised

    const char* attributes[3];
    attributes[0] = "tag";
    attributes[1] = expression._name.c_str();
    attributes[2] = 0;
    Error("NonFatal", "Unknown expression tag. Returning false.", attributes);
    return false;
}
