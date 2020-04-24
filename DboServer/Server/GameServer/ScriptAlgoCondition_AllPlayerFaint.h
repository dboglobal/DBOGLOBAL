#ifndef __SCRIPT_ALGO_DBOG_CONDITION_ALL_PLAYER_FAINT_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_ALL_PLAYER_FAINT_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_AllPlayerFaint : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_AllPlayerFaint(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_AllPlayerFaint();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bWpsFail;

};

#endif