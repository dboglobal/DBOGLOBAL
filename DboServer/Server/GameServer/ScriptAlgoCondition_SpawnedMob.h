#ifndef __SCRIPT_ALGO_DBOG_CONDITION_SPAWNED_MOB_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_SPAWNED_MOB_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_SpawnedMob : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_SpawnedMob(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_SpawnedMob();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID				m_mobSpawnGroupId;

	BYTE						m_byMobCount;

};

#endif