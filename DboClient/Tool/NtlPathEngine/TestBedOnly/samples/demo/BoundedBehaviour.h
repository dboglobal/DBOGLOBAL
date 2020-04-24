
#include "iBehaviour.h"
#include <vector>

class cSimpleDOM;
class cGameObject;
class iAgent;

class cBoundedBehaviour : public iBehaviour
{
    iBehaviour* _base;
    std::vector<iAgent*> _bounds;

public:

    cBoundedBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    bool update(cGameObject& controlledObject);
};

