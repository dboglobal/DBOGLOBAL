#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_REWARD_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_REWARD_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD_reward : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD_reward(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD_reward();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	TBLIDX						m_itemTblidx;
	bool						m_bLastStage;

};

#endif