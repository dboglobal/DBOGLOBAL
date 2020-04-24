
#pragma warning(disable : 4786)

#include "BoundedBehaviour.h"
#include "Error.h"
#include "GameState.h"
#include "GameObject.h"
#include "SimpleDOM.h"
#include "Globals.h"

cBoundedBehaviour::cBoundedBehaviour(const cSimpleDOM& element, cGameObject& controlledObject)
{
    _base = iBehaviour::create(element.refFirstChildWithName("base"), controlledObject);
    size_t i;
    for(i = 0; i < element._children.size(); ++i)
    {
        const cSimpleDOM& child = element._children[i];
        if(child._name == "bound")
        {
            std::string targetName = child.getAttribute("object");
            _bounds.push_back(gGameState->findObject(targetName)->getAgent());
        }
    }
}

bool
cBoundedBehaviour::update(cGameObject& controlledObject)
{
// the bound agents are assumed to not already be included in the obstructions context
// i.e. these should be markers or something like that
    long i;
    for(i = 0; i < static_cast<long>(_bounds.size()); ++i)
    {
        gGameState->getObstructionsContext()->addAgent(_bounds[i]);
    }
    bool result = _base->update(controlledObject);
    for(i = 0; i < static_cast<long>(_bounds.size()); ++i)
    {
        gGameState->getObstructionsContext()->removeAgent(_bounds[i]);
    }
    return result;
}
