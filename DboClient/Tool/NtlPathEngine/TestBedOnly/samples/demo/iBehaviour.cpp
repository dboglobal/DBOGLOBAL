
#pragma warning(disable : 4786)

#include "iBehaviour.h"
#include "UIController.h"
#include "ChaseBehaviour.h"
#include "RunAwayBehaviour.h"
#include "EngageBehaviour.h"
#include "WanderBehaviour.h"
#include "AgitatedBehaviour.h"
#include "MoveDirectlyTowardsBehaviour.h"
#include "SwitchResetterBehaviour.h"
#include "PackHunterBehaviour.h"
#include "VisitGreenSceneryBehaviour.h"
#include "RescueFromHuntersBehaviour.h"
#include "BoundedBehaviour.h"
#include "LimitedDurationBehaviour.h"
#include "ScriptBehaviours.h"
#include "SimpleDOM.h"
#include "Error.h"

iBehaviour*
iBehaviour::create_IgnoreDuration(const cSimpleDOM& element, cGameObject& controlledObject)
{
    std::string type = element.getAttribute("type");
    if(type == "uiControl")
    {
        return new cUIController(element, controlledObject);
    }
    if(type == "chase")
    {
        return new cChaseBehaviour(element, controlledObject);
    }
    if(type == "runAway")
    {
        return new cRunAwayBehaviour(element, controlledObject);
    }
    if(type == "engage")
    {
        return new cEngageBehaviour(element, controlledObject);
    }
    if(type == "wander")
    {
        return new cWanderBehaviour(element, controlledObject);
    }
    if(type == "agitated")
    {
        return new cAgitatedBehaviour(element, controlledObject);
    }
    if(type == "moveDirectlyTowards")
    {
        return new cMoveDirectlyTowardsBehaviour(element, controlledObject);
    }
    if(type == "switchResetter")
    {
        return new cSwitchResetterBehaviour(element, controlledObject);
    }
    if(type == "packHunter")
    {
        return new cPackHunterBehaviour(element, controlledObject);
    }
    if(type == "visitGreenScenery")
    {
        return new cVisitGreenSceneryBehaviour(element, controlledObject);
    }
    if(type == "rescueFromHunters")
    {
        return new cRescueFromHuntersBehaviour(element, controlledObject);
    }
    if(type == "bounded")
    {
        return new cBoundedBehaviour(element, controlledObject);
    }
    if(type == "runScriptOnce")
    {
        return new cRunScriptOnceBehaviour(element, controlledObject);
    }
    if(type == "repeatScript")
    {
        return new cRepeatScriptBehaviour(element, controlledObject);
    }
    if(type == "wait")
    {
        return new cNullBehaviour;
    }
    if(type == "null")
    {
        return new cNullBehaviour;
    }
    const char* attributes[3];
    attributes[0] = "type";
    attributes[1] = type.c_str();
    attributes[2] = 0;
    Error("NonFatal", "iBehaviour::create() - unknown behaviour type, creating null behaviour.", attributes);
    return new cNullBehaviour;
}

iBehaviour*
iBehaviour::create(const cSimpleDOM& element, cGameObject& controlledObject)
{
    if(element.hasAttribute("duration"))
    {
        return new cLimitedDurationBehaviour(element, controlledObject);
    }
    return create_IgnoreDuration(element, controlledObject);
}
