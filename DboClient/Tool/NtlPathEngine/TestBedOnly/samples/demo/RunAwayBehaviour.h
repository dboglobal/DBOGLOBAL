
#include "iBehaviour.h"

class cSimpleDOM;
class cGameObject;
class iPath;

class cRunAwayBehaviour : public iBehaviour
{
    long _distanceAway;
    iPath* _currentPath;
    long _timeSincePathQuery;

public:

    cRunAwayBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cRunAwayBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
