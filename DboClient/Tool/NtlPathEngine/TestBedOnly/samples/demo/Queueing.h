
#ifndef QUEUEING_INCLUDED
#define QUEUEING_INCLUDED

#include <vector>

class iAgent;
class iCollisionContext;

class cQueueEntry;
class cQueueManager
{
    std::vector<cQueueEntry*> _order;

public:
    friend class cQueueEntry;

    size_t add(cQueueEntry*);
    void remove(long);

};

class cQueueEntry
{
    cQueueManager& _manager;
    size_t _queueIndex;
    float _distanceToTarget;
    iAgent* _controlledObjectAgent;
    iCollisionContext* _queueCollisionContext;
    iAgent* _queueCollisionAgent;

    void updateQueueAgentPosition();

public:

    cQueueEntry(cQueueManager& manager, long shapeIndex, iAgent* controlledObjectAgent);
    ~cQueueEntry();

    void indexChangedTo(size_t value)
    {
        _queueIndex = value;
    }

    bool isLeader() const
    {
        return _queueIndex == 0;
    }
    float getDistanceToTarget() const
    {
        return _distanceToTarget;
    }
    size_t queueIndex() const
    {
        return _queueIndex;
    }

    void updateDistanceToTarget(float value);
    bool testQueueCollision();
};

#endif
