
#include "iBehaviour.h"

class cSimpleDOM;
class cGameObject;

class bScriptBehaviour : public iBehaviour
{
    iBehaviour* _currentBehaviour;

protected:

    const cSimpleDOM* _controlScript;
    long _currentLine;
    bool _commandIsInProgress;

    // returns true if the current behaviour completed
    bool stepCurrentBehaviour(cGameObject& controlledObject);

    // returns true if the command completed
    bool processControlCommand(cGameObject& controlledObject, const cSimpleDOM& command, bool continuedFromLastFrame);

public:

    bScriptBehaviour(const cSimpleDOM& element);
    ~bScriptBehaviour();
};

class cRunScriptOnceBehaviour : public bScriptBehaviour
{
public:

    cRunScriptOnceBehaviour(const cSimpleDOM& element, cGameObject& controlledObject) :
      bScriptBehaviour(element)
    {
    }
    bool update(cGameObject& controlledObject);
};

class cRepeatScriptBehaviour : public bScriptBehaviour
{
public:

    cRepeatScriptBehaviour(const cSimpleDOM& element, cGameObject& controlledObject) :
      bScriptBehaviour(element)
    {
    }
    bool update(cGameObject& controlledObject);
};

