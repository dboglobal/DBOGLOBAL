
#pragma warning(disable : 4786)

#include "ScriptBehaviours.h"
#include "Error.h"
#include "GameState.h"
#include "GameObject.h"
#include "SimpleDOM.h"
#include "Globals.h"

bool
bScriptBehaviour::stepCurrentBehaviour(cGameObject& controlledObject)
{
    assertD(_currentBehaviour);
    bool completed = _currentBehaviour->update(controlledObject);
    if(completed)
    {
        delete _currentBehaviour;
        _currentBehaviour = 0;
    }
    return completed;
}

bool
bScriptBehaviour::processControlCommand(cGameObject& controlledObject, const cSimpleDOM& command, bool continuedFromLastFrame)
{
    if(command._name == "behaviour")
    {
        if(!continuedFromLastFrame)
        {
            if(_currentBehaviour)
            {
                delete _currentBehaviour;
            }
            _currentBehaviour = iBehaviour::create(command, controlledObject);
        }
        return stepCurrentBehaviour(controlledObject);
    }
    if(command._name == "waitUntil")
    {
        assertD(command._children.size() == 1);
        return gGameState->evaluate(command._children.front());
    }
    if(command._name == "while")
    {
        assertD(command._children.size() == 2);
        if(gGameState->evaluate(command._children[0]))
        {
            return processControlCommand(controlledObject, command._children[1], continuedFromLastFrame);
        }
        else
        {
            return true;
        }
    }
    gGameState->execute(command);
    return true;
}
bScriptBehaviour::bScriptBehaviour(const cSimpleDOM& element)
{
    _controlScript = &element;
    _currentLine = 0;
    _commandIsInProgress = false;
    _currentBehaviour = 0;
}
bScriptBehaviour::~bScriptBehaviour()
{
    delete _currentBehaviour;
}

bool
cRunScriptOnceBehaviour::update(cGameObject& controlledObject)
{
    if(!_controlScript || _currentLine == _controlScript->_children.size())
    {
        return true;
    }
    bool completed = processControlCommand(controlledObject, _controlScript->_children[_currentLine], _commandIsInProgress);
    while(completed)
    {
        _currentLine++;
        if(_currentLine == _controlScript->_children.size())
        {
            return true;
        }
        completed = processControlCommand(controlledObject, _controlScript->_children[_currentLine], false);
    }
    _commandIsInProgress = true;
    return false;
}

bool
cRepeatScriptBehaviour::update(cGameObject& controlledObject)
{
    if(!_controlScript || _currentLine == _controlScript->_children.size())
    {
        return false;
    }
    bool completed = processControlCommand(controlledObject, _controlScript->_children[_currentLine], _commandIsInProgress);
    while(completed)
    {
        _currentLine++;
        if(_currentLine == _controlScript->_children.size())
        {
            _currentLine = 0;
        }
        completed = processControlCommand(controlledObject, _controlScript->_children[_currentLine], false);
    }
    _commandIsInProgress = true;
    return false;
}

