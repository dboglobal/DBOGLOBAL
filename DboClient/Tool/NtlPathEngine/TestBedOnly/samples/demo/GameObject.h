
#include "i_pathengine.h"
#include <string>
#include <vector>

class cSimpleDOM;
class iBehaviour;
class cGameState;

class cGameObject
{
  // initialised by constructor
    std::string _contentName;
    std::string _type;
    std::string _name;
    const cSimpleDOM* _initTemplate;
    size_t _index;
    iShape* _shape;
    iAgent* _agent;
    float _precisionX, _precisionY;
    bool _isPushable;
    float _speed;

  // initialised in completeInitalisation()
    bool _collisionFlagsValid;
    std::vector<char> _collisionFlags;
    iBehaviour* _currentBehaviour;
    cGameObject* _currentTarget;

    // returns true if the current behaviour completed
    bool stepCurrentBehaviour();

    void setContentName(const std::string& contentName);
    void constructorCommon();

public:

  // purely graphical attributes
  // these can be accessed and changed at will, for example by script commands
    std::string _colour;
    float _alpha;
    long _height;
    std::string _toggledColour;
    float _toggledAlpha;
    long _toggledHeight;
    std::string _headingIndicatorColour;
    long _headingIndicatorSize;
    bool _visible; // note that visibility and toggled status are used to control script flow
    bool _toggled;
    float _heading;

// at initial construction time, objects are placed and any properties not dependant on other objects or the game state are set

  // shapes index is supplied by template, placed at an anchor
    cGameObject(long index, iMesh* mesh, const cPosition& position, const cSimpleDOM& templates, const std::string& contentName, float orientation);
  // obstruction info was retrieved directly from the mesh as an agent
    cGameObject(long index, const cSimpleDOM& templates, const std::string& contentName, iAgent* agent);

// this method is then called once all objects have been placed and the game state has been initialised
// at this point the default behaviour is created, default target set, etc.
    void completeInitialisation();

    ~cGameObject();

// accessor methods
    const std::string& contentName() const {return _contentName;}
    const std::string& name() const {return _name;}
    const std::string& type() const {return _type;}
    iShape* getShape() const {return _shape;}
    iAgent* getAgent() const {return _agent;}
    cPosition getPosition() {return _agent->getPosition();}
    float getPrecisionX() const {return _precisionX;}
    float getPrecisionY() const {return _precisionY;}
    float getSpeed() const {return _speed;}
    cGameObject* getTarget() const {return _currentTarget;}
    const cSimpleDOM& refTemplate() const {return *_initTemplate;}

// call through to PathEngine
//  the obstructions context is applied
//  in the case of failed path, 0 is returned
    iPath* findShortestPathTo(const cPosition& target) const;
    iPath* findShortestPathTo_IgnoringOneObject(const cPosition& target, cGameObject& toIgnore) const;
    iPath* findPathAway(const cPosition& awayFrom, long distanceAway) const;
    iPath* findStraightLinePathTo(const cPosition& target) const;

// agent movement
    void moveUnderDirectControl(float localX, float localY, float speedMultiplier);
    bool moveInDirection(float dX, float dY);
    bool advanceAlongPath(iPath* path, float speedMultiplier = 1.f);

    void faceTowards(const cPosition& target);

    float distanceTo(const cGameObject& rhs) const;
    float distanceTo(const cPosition& p) const;

    void setDefaultTarget();
    void setTarget(cGameObject* value) {_currentTarget = value;}
    void setSpeed(float value) {_speed = value;}

    void setBehaviour(const cSimpleDOM& element);
    void setDefaultBehaviour();

// these collision flags are used to speed up collision checking against objects that are designated as switches
    void invalidateCollisionFlags();
    bool collisionFlagsAreValid() const;
    void initialiseCollisionFlags();
    void setCollisionFlag(cGameObject& collisionTarget);
    bool testCollisionFlag(cGameObject& collisionTarget);

    void update();
    void draw();
};
