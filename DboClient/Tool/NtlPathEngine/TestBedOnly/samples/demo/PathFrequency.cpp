
#include "PathFrequency.h"
#include "GameObject.h"
#include "i_pathengine.h"

long
GetPathFrequency(const cGameObject& controlledObject, const cGameObject& targetObject)
{
    long distance = static_cast<long>(controlledObject.distanceTo(targetObject));
    if(distance <= 100)
    {
        // pathfind every frame at close range
        return 1;
    }
    long result = distance - 100;
    result /= 8;
    if(result > 50)
    {
        // maximum time between paths
        result = 50;
    }
    return result;
}

long
GetPathFrequency(iPath* currentPath)
{
    unsigned long length = currentPath->getLength();
    if(length <= 200)
    {
        // pathfind every frame at close range
        return 1;
    }
    length -= 100;
    if(length > 4000)
    {
        length = 4000;
    }
    return static_cast<long>(length) / 15;
}
