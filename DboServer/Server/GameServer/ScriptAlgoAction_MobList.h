#ifndef __SCRIPT_ALGO_DBOG_ACTION_MOB_LIST_H__
#define __SCRIPT_ALGO_DBOG_ACTION_MOB_LIST_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_MobList : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_MobList(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_MobList();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	int							m_nIndex;

	CControlScriptNode*			m_pNode;

};

#endif