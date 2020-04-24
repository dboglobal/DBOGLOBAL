#ifndef __SCRIPT_ALGO_DBOG_CONDITION_NPCFAINT_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_NPCFAINT_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_NpcFaint : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_NpcFaint(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_NpcFaint();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	bool						m_bWpsFail;

};

#endif