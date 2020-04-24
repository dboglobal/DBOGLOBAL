
#include "iBehaviour.h"
#include "i_pathengine.h"
#include <string>

class cSimpleDOM;
class cGameObject;

class cUIController : public iBehaviour
{
    long _findClosestUnobstructedPointRange;

    cPosition _currentTarget;
    iPath* _currentPath;

public:

    cUIController(const cSimpleDOM& element, cGameObject& controlledObject);

// interface for iController

    bool requiresPreprocess(bool& collisionOnly) const
    {
        collisionOnly = false;
        return true;
    }
    bool update(cGameObject& controlledObject);
    void draw(cGameObject& controlledObject);
};
