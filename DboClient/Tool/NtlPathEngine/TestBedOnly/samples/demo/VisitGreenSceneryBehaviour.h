
#include "iBehaviour.h"
#include "i_pathengine.h"
#include <vector>

class cSimpleDOM;
class cGameObject;

class cVisitGreenSceneryBehaviour : public iBehaviour
{
    std::vector<cPosition> _greenScenery;
    float _engageDistance;
    iPath* _path;

public:

    cVisitGreenSceneryBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);
    bool update(cGameObject& controlledObject);
};

