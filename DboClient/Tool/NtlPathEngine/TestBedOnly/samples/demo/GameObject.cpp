
#pragma warning(disable : 4786)

#include "GameObject.h"
#include "GameState.h"
#include "SimpleDOM.h"
#include "Error.h"
#include "Globals.h"
#include "i_testbed.h"
#include "iBehaviour.h"
#include "MoveAgent.h"
#include "Resources.h"
#include <math.h>

using std::string;

bool
cGameObject::stepCurrentBehaviour()
{
    assertD(_currentBehaviour);
    bool completed = _currentBehaviour->update(*this);
    if(completed)
    {
        delete _currentBehaviour;
        _currentBehaviour = 0;
    }
    return completed;
}

void
cGameObject::setContentName(const std::string& contentName)
{
    _contentName = contentName;
    string::size_type pos = contentName.find(".");
    if(pos == string::npos)
    {
        _type = contentName;
    }
    else
    {
        _type = std::string(contentName, 0, pos);
        _name = std::string(contentName.c_str() + pos + 1);
    }
}

void
cGameObject::constructorCommon()
{
    string attributeString;

    _colour = _initTemplate->getAttribute("colour");
    _alpha = 1.0f;
    if(_initTemplate->hasAttribute("alpha"))
    {
        attributeString = _initTemplate->getAttribute("alpha");
        _alpha = static_cast<float>(strtod(attributeString.c_str(), 0));
    }
    attributeString = _initTemplate->getAttribute("height");
    _height = strtol(attributeString.c_str(), 0, 10);

    _toggledColour = _colour;
    _toggledAlpha = _alpha;
    _toggledHeight = _height;

    long i = _initTemplate->firstChildWithName("toggled");
    if(i >= 0)
    {
        if(_initTemplate->_children[i].hasAttribute("colour"))
        {
            _toggledColour = _initTemplate->_children[i].getAttribute("colour");
        }
        if(_initTemplate->_children[i].hasAttribute("alpha"))
        {
            attributeString = _initTemplate->_children[i].getAttribute("alpha");
            _toggledAlpha = static_cast<float>(strtod(attributeString.c_str(), 0));
        }
        if(_initTemplate->_children[i].hasAttribute("height"))
        {
            attributeString = _initTemplate->_children[i].getAttribute("height");
            _toggledHeight = strtol(attributeString.c_str(), 0, 10);
        }
    }

    _headingIndicatorSize = _initTemplate->attributeAsLongWithDefault("headingIndicatorSize", 0);
    if(_headingIndicatorSize)
    {
        _headingIndicatorColour = _initTemplate->getAttribute("headingIndicatorColour");
    }

    _speed = _initTemplate->attributeAsFloatWithDefault("speed", 0.f);

    _isPushable = (_initTemplate->getAttribute("isPushable") == "true");

    _precisionX = 0.0f;
    _precisionY = 0.0f;

    _toggled = false;
    _visible = true;
    if(_initTemplate->getAttribute("startsInvisible") == "true")
    {
        _visible = false;
    }
    
    _currentBehaviour = 0;
    _currentTarget = 0;
}

cGameObject::cGameObject(long index,
                         iMesh* mesh, const cPosition& position,
                        const cSimpleDOM& templates, const std::string& contentName, float orientation)
{
    _index = index;
    setContentName(contentName);
    long i = templates.firstChildWithName(_type);
    assertR(i >= 0);
    _initTemplate = &templates._children[i];

    long shapeIndex = _initTemplate->attributeAsLong("shape");
    _shape = gResources->getShape(shapeIndex);

    _heading = orientation;

    constructorCommon();
    
    _agent = mesh->placeAgent(_shape, position);
    _agent->setUserData(reinterpret_cast<void*>(this));
}

cGameObject::cGameObject(long index, const cSimpleDOM& templates, const std::string& contentName, iAgent* agent)
{
    _index = index;
    setContentName(contentName);
    long i = templates.firstChildWithName(_type);
    assertR(i >= 0);
    _initTemplate = &templates._children[i];

    _shape = 0;
    _agent = agent;
    _agent->setUserData(reinterpret_cast<void*>(this));
    _heading = 0.f;

    constructorCommon();
}

void
cGameObject::completeInitialisation()
{
    _collisionFlags.resize(gGameState->numberOfObjects());
    _collisionFlagsValid = false;
    setDefaultBehaviour();
    setDefaultTarget();
}

cGameObject::~cGameObject()
{
    delete _currentBehaviour;
    delete _agent;
}


iPath*
cGameObject::findShortestPathTo(const cPosition& target) const
{
    iPath* result = _agent->findShortestPathTo(gGameState->getObstructionsContext(), target);
    return result;
}

iPath*
cGameObject::findShortestPathTo_IgnoringOneObject(const cPosition& target, cGameObject& toIgnore) const
{
    iPath* result;
    if(gGameState->getObstructionsContext()->includes(toIgnore._agent))
    {
        gGameState->getObstructionsContext()->removeAgent(toIgnore._agent);
        result = _agent->findShortestPathTo(gGameState->getObstructionsContext(), target);
        gGameState->getObstructionsContext()->addAgent(toIgnore._agent);
    }
    else
    {
        result = _agent->findShortestPathTo(gGameState->getObstructionsContext(), target);
    }
    return result;
}

iPath*
cGameObject::findPathAway(const cPosition& awayFrom, long distanceAway) const
{
    iPath* result = _agent->findPathAway(gGameState->getObstructionsContext(), awayFrom, distanceAway);
    return result;
}

iPath*
cGameObject::findStraightLinePathTo(const cPosition& target) const
{
    if(_agent->testCollisionTo(gGameState->getObstructionsContext(), target))
    {
        return 0;
    }
    iPath* result = _agent->findShortestPathTo(gGameState->getObstructionsContext(), target);
    assertD(result);
    return result;
}

void
cGameObject::moveUnderDirectControl(float localX, float localY, float speedMultiplier)
{
    localX *= _speed * speedMultiplier;
    localY *= _speed * speedMultiplier;
    double sinOf = sin(double(_heading));
    double cosOf = cos(double(_heading));
    double dx = _precisionX + sinOf * localY + cosOf * localX;
    double dy = _precisionY + cosOf * localY - sinOf * localX;
    long dxL = static_cast<long>(dx);
    long dyL = static_cast<long>(dy);

    if(dxL == 0 && dyL == 0)
    {
    // no movement after approximation
        _precisionX = static_cast<float>(dx);
        _precisionY = static_cast<float>(dy);
        return;
    }

    cPosition current = _agent->getPosition();
    cPosition target;
    target.x = current.x + dxL;
    target.y = current.y + dyL;

    iCollisionInfo* collisionInfo = _agent->firstCollisionTo(gGameState->getObstructionsContext(), target.x, target.y, target.cell);
    if(!collisionInfo)
    {
        _precisionX = static_cast<float>(dx - dxL);
        _precisionY = static_cast<float>(dy - dyL);
        _agent->moveTo(target);
        return;
    }

    iAgent* collidingAgent = collisionInfo->getCollidingAgent();
    if(collidingAgent)
    {
        cGameObject* collidingGameObject = static_cast<cGameObject*>(collidingAgent->getUserData());
        if(collidingGameObject->_isPushable)
        {
            MoveAgent(gPathEngine, gGameState->getObstructionsContext(), gGameState->getCollisionCheckersContext(),
                      collidingGameObject->_agent, _heading, localX, localY,
                      _precisionX, _precisionY
                      );
        }
    }
    delete collisionInfo;

    MoveAgent(gPathEngine, gGameState->getObstructionsContext(),
                      _agent, _heading, localX, localY,
                      _precisionX, _precisionY
                      );
}

bool
cGameObject::moveInDirection(float dX, float dY)
{
    float offsetX = _precisionX + dX;
    float offsetY = _precisionY + dY;
    cPosition currentP = getPosition();
    cPosition targetP;
    targetP.x = static_cast<long>(currentP.x + offsetX);
    targetP.y = static_cast<long>(currentP.y + offsetY);
    bool collided = _agent->testCollisionTo_XY(gGameState->getObstructionsContext(), targetP.x, targetP.y, targetP.cell);
    if(!collided)
    {
        _agent->moveTo(targetP);
        long offsetXL = targetP.x - currentP.x;
        long offsetYL = targetP.y - currentP.y;
        _precisionX = offsetX - offsetXL;
        _precisionY = offsetY - offsetYL;
    }
    return collided;
}

bool
cGameObject::advanceAlongPath(iPath* path, float speedMultiplier)
{
    if(path->size() >= 2)
    {
        cPosition nextTarget = path->position(1);
        cPosition current = path->position(0);
        long dx, dy;
        dx = nextTarget.x - current.x;
        dy = nextTarget.y - current.y;
        _heading = static_cast<float>(atan2(static_cast<double>(dx), static_cast<double>(dy)));
    }
    iCollisionInfo* ci = _agent->advanceAlongPathWithPrecision(path, _speed * speedMultiplier, gGameState->getObstructionsContext(), _precisionX, _precisionY);
    if(ci)
    {
        delete ci;
        return true;
    }
    return false;
}

void
cGameObject::faceTowards(const cPosition& target)
{
    cPosition p = getPosition();
    long dx, dy;
    dx = target.x - p.x;
    dy = target.y - p.y;
    _heading = static_cast<float>(atan2(static_cast<double>(dx), static_cast<double>(dy)));
}

float
cGameObject::distanceTo(const cGameObject& rhs) const
{
    cPosition p1 = _agent->getPosition();
    cPosition p2 = rhs._agent->getPosition();
    float dx = static_cast<float>(p2.x - p1.x);
    float dy = static_cast<float>(p2.y - p1.y);
    return static_cast<float>(sqrt(dx * dx + dy * dy));
}
float
cGameObject::distanceTo(const cPosition& p) const
{
    cPosition p1 = _agent->getPosition();
    float dx = static_cast<float>(p.x - p1.x);
    float dy = static_cast<float>(p.y - p1.y);
    return static_cast<float>(sqrt(dx * dx + dy * dy));
}

void
cGameObject::setDefaultTarget()
{
    std::string targetName = _initTemplate->getAttribute("defaultTarget");
    if(!targetName.empty())
    {
        _currentTarget = gGameState->findObject(targetName);
    }
}

void
cGameObject::setBehaviour(const cSimpleDOM& element)
{
    if(_currentBehaviour)
    {
        delete _currentBehaviour;
    }
    _currentBehaviour = iBehaviour::create(element, *this);
}
void
cGameObject::setDefaultBehaviour()
{
    if(_currentBehaviour)
    {
        delete _currentBehaviour;
    }
    long i = _initTemplate->firstChildWithName("defaultBehaviour");
    if(i >= 0)
    {
        _currentBehaviour = iBehaviour::create(_initTemplate->_children[i], *this);
    }
    else
    {
        _currentBehaviour = 0;
    }
}

void
cGameObject::invalidateCollisionFlags()
{
    _collisionFlagsValid = false;
}
bool
cGameObject::collisionFlagsAreValid() const
{
    return _collisionFlagsValid;
}
void
cGameObject::initialiseCollisionFlags()
{
    std::fill(_collisionFlags.begin(), _collisionFlags.end(), false);
    _collisionFlagsValid = true;
}
void
cGameObject::setCollisionFlag(cGameObject& collisionTarget)
{
    _collisionFlags[collisionTarget._index] = true;
}
bool
cGameObject::testCollisionFlag(cGameObject& collisionTarget)
{
    return _collisionFlags[collisionTarget._index] != 0;
}

void
cGameObject::update()
{
    if(_currentBehaviour)
    {
        gCurrentObject = this;
        stepCurrentBehaviour();
        gCurrentObject = 0;
    }
}

void
cGameObject::draw()
{
    if(!_visible)
    {
        return;
    }
    long usedHeight;
    if(_toggled)
    {
        gTestBed->setColour(_toggledColour.c_str());
        if(_toggledAlpha < 1.0f)
        {
            gTestBed->setAdditiveBlending(_toggledAlpha);
        }
        usedHeight = _toggledHeight;
    }
    else
    {
        gTestBed->setColour(_colour.c_str());
        if(_alpha < 1.0f)
        {
            gTestBed->setAdditiveBlending(_alpha);
        }
        usedHeight = _height;
    }
    gTestBed->drawAgent(_agent, usedHeight);
    if(_headingIndicatorSize)
    {
        gTestBed->setColour(_headingIndicatorColour.c_str());
        gTestBed->drawAgentHeadingWithPrecision(_agent, _headingIndicatorSize, usedHeight, _heading, _precisionX, _precisionY);
    }
    if(_currentBehaviour)
    {
        _currentBehaviour->draw(*this);
    }
    gTestBed->setOpaque();
}
