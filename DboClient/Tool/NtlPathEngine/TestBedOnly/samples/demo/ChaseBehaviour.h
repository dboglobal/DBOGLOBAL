
#include "iBehaviour.h"

class cSimpleDOM;
class cGameObject;
class iPath;

class cChaseBehaviour : public iBehaviour
{
    long _activationSound;
    iPath* _currentPath;
    long _timeSincePathQuery;
    long _engageDistance;
    long _findClosestUnobstructedPointRange;

public:

    cChaseBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cChaseBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
