
#include "iBehaviour.h"
#include "i_pathengine.h"
#include "Queueing.h"
#include <vector>

class cSimpleDOM;
class cGameObject;
class iPath;
class iCollisionContext;
class iAgent;

class cPackHunterBehaviour : public iBehaviour
{
    static cQueueManager _queueManager;
    static long _timeSincePathToGoal;
    static std::vector<cPosition> _interceptPositions;

    cGameObject& _controlledObject;
    cQueueEntry _queueEntry;
    iPath* _currentPath;
    long _timeSincePathQuery;

    bool _debugDisplay;

public:

    cPackHunterBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cPackHunterBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);

    void draw(cGameObject& controlledObject);
};
