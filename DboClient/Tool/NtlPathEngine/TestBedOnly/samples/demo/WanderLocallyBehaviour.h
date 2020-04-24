
#include "iBehaviour.h"
#include "i_pathengine.h"
#include <string>

class cSimpleDOM;
class cGameObject;

class cWanderBehaviour : public iBehaviour
{
    iPath* _currentPath;

public:

    cWanderBehaviour(const cSimpleDOM& element);
    ~cWanderBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
