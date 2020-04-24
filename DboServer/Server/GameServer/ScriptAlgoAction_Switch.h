#ifndef __SCRIPT_ALGO_DBOG_ACTION_SWITCH_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SWITCH_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_Switch : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Switch(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Switch();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	std::string					m_strCompareVariable;

};

#endif