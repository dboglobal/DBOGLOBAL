
#include <vector>
#include <string>
#include <map>
class iAgent;
class iMesh;
class iShape;
class iCollisionContext;
class cGameObject;
class cSimpleDOM;

class cGameState
{
    const cSimpleDOM* _frameScript;
    iMesh* _mesh;
    std::vector<cGameObject*> _objects;
    iCollisionContext* _obstructionsContext;
    iCollisionContext* _switchesContext;
    iCollisionContext* _collisionCheckersContext;
    std::vector<iAgent*> _agentsOverlappedBuffer;
    std::map<std::string, long> _flags;
    float _cameraDistance;
    float _cameraHeading;
    float _cameraElevation;
    bool _lookAtActive;
    bool _endLevel;
    std::string _result;

    cGameObject* resolveObjectArgument(const cSimpleDOM& command) const;
    void updateCollisionFor(cGameObject& object);

public:

    cGameState(const cSimpleDOM& templates, const cSimpleDOM& levelScript);
    ~cGameState();

  // accessors
    iMesh* getMesh() const {return _mesh;}
    iCollisionContext* getObstructionsContext() const {return _obstructionsContext;}
    iCollisionContext* getCollisionCheckersContext() const {return _collisionCheckersContext;}
    float getCameraHeading() const {return _cameraHeading;}

    bool uiControlIsFrozen() const {return _lookAtActive;}

    size_t numberOfObjects() const {return _objects.size();}
    cGameObject* findObject(const std::string& contentName) const;
    void findAllObjectsWithType(const std::string& type, std::vector<cGameObject*>& result) const;

    void update();
    void draw();

    bool levelCompleted(std::string& result);

    void execute(const cSimpleDOM& command);
    bool evaluate(const cSimpleDOM& expression);
};
