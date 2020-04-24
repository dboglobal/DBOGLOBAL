
#include "iBehaviour.h"
#include "i_pathengine.h"
#include <string>

class cSimpleDOM;
class cGameObject;

class cWanderBehaviour : public iBehaviour
{
    iPath* _currentPath;
    long _range;
    std::string _objectToUseAsCentre;
    float _forwardBias;

public:

    cWanderBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cWanderBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
