#ifndef __SCRIPT_ALGO_DBOG_ACTION_CHANGE_STAGE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CHANGE_STAGE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ChangeStage : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ChangeStage(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ChangeStage();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byNextStage;

};

#endif