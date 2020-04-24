
#include "iBehaviour.h"

class cSimpleDOM;
class cGameObject;

class cLimitedDurationBehaviour : public iBehaviour
{
    iBehaviour* _base;
    long _remainingDuration;

public:

    cLimitedDurationBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    bool update(cGameObject& controlledObject);
};

