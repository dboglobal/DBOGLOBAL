#ifndef __SCRIPT_ALGO_DBOG_CONDITION_SPAWNED_NPC_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_SPAWNED_NPC_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_SpawnedNpc : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_SpawnedNpc(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_SpawnedNpc();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

};

#endif