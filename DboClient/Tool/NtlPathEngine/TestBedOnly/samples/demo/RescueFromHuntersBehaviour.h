
#include "iBehaviour.h"
#include "Queueing.h"
#include <vector>

class cSimpleDOM;
class cGameObject;
class iPath;

class cRescueFromHuntersBehaviour : public iBehaviour
{
    static std::vector<cGameObject*> _targets;
    static std::vector<cQueueManager> _queueManagers;

    long _queueShape;
    long _targetIndex;
    cQueueEntry* _queueEntry;
    iPath* _currentPath;
    long _timeSincePathQuery;

    long findClosestTarget(cGameObject& controlledObject);

public:

    cRescueFromHuntersBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cRescueFromHuntersBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
