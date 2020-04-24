
#include "iBehaviour.h"
#include <string>

class cSimpleDOM;
class cGameObject;
class iPath;

class cMoveDirectlyTowardsBehaviour : public iBehaviour
{
    std::string _targetName;
    float _engageDistance;

public:

    cMoveDirectlyTowardsBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);

// interface for iController

    bool update(cGameObject& controlledObject);
};
