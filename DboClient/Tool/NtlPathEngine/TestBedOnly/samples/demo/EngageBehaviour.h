
#include "iBehaviour.h"
#include <string>

class cSimpleDOM;
class cGameObject;
class iPath;

class cEngageBehaviour : public iBehaviour
{
    float _distance;
    iPath* _path;

public:

    cEngageBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    ~cEngageBehaviour();

// interface for iController

    bool update(cGameObject& controlledObject);
};
