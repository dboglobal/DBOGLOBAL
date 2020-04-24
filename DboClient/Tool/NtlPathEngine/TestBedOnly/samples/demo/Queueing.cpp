
#pragma warning(disable : 4786)

#include "Queueing.h"
#include "Globals.h"
#include "Resources.h"
#include "GameState.h"

size_t
cQueueManager::add(cQueueEntry* entry)
{
    size_t result = _order.size();
    _order.push_back(entry);
    return result;
}

void
cQueueManager::remove(long index)
{
    if(index + 1 != _order.size())
    {
        std::swap(_order[index], _order.back());
        _order[index]->indexChangedTo(index);
    }
    _order.pop_back();
}

void
cQueueEntry::updateQueueAgentPosition()
{
    if(_controlledObjectAgent->getPosition() != _queueCollisionAgent->getPosition())
    {
        _queueCollisionAgent->moveTo(_controlledObjectAgent->getPosition());
    }
}

cQueueEntry::cQueueEntry(cQueueManager& manager, long shapeIndex, iAgent* controlledObjectAgent) :
 _manager(manager)
{
    _distanceToTarget = 0.f;
    iShape* shape = gResources->getShape(shapeIndex);
    if(!gGameState->getMesh()->shapeCanCollide(shape))
    {
        gGameState->getMesh()->generateCollisionPreprocessFor(shape, 0);
    }
    _controlledObjectAgent = controlledObjectAgent;
    _queueCollisionAgent = gGameState->getMesh()->placeAgent(shape, _controlledObjectAgent->getPosition());
    _queueCollisionContext = gGameState->getMesh()->newContext();
    _queueCollisionContext->addAgent(_queueCollisionAgent);
    _queueIndex = _manager.add(this);
}

cQueueEntry::~cQueueEntry()
{
    _manager.remove(_queueIndex);
    delete _queueCollisionAgent;
    delete _queueCollisionContext;
}

void
cQueueEntry::updateDistanceToTarget(float value)
{
    _distanceToTarget = value;
    if(_queueIndex > 0 && _distanceToTarget < _manager._order[_queueIndex - 1]->_distanceToTarget)
    {
    // advance
        std::swap(_manager._order[_queueIndex], _manager._order[_queueIndex - 1]);
        _manager._order[_queueIndex]->_queueIndex++;
        _queueIndex--;
    }
}

bool
cQueueEntry::testQueueCollision()
{
    if(_queueIndex == 0)
    {
        return false;
    }
    cQueueEntry* testAgainst = _manager._order[_queueIndex - 1];
    testAgainst->updateQueueAgentPosition();
    return _controlledObjectAgent->testCollisionAt(testAgainst->_queueCollisionContext, _controlledObjectAgent->getPosition());
}

