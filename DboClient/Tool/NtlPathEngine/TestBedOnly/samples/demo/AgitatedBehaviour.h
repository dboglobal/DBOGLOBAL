
#include "iBehaviour.h"

class cSimpleDOM;
class cGameObject;

class cAgitatedBehaviour : public iBehaviour
{
    float _radius;
    float _angularRange;

    enum eState
    {
        MOVING_FORWARDS1,
        MOVING_BACK,
        MOVING_FORWARDS2,
    };

    eState _state;
    float _centralHeading;
    float _currentHeading;
    float _distanceToMove;

    float _dx, _dy;

    void initDeltas();
    void randomiseHeading(cGameObject& controlledObject);

public:

    cAgitatedBehaviour(const cSimpleDOM& element, cGameObject& controlledObject);

// interface for iController

    bool update(cGameObject& controlledObject);
};
