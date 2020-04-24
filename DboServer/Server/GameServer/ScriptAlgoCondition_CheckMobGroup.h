#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHECKMOBGRPUP_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHECKMOBGRPUP_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_CheckMobGroup : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_CheckMobGroup(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_CheckMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	SPAWNGROUPID				m_spawnGroupId;

	int							m_nCheckCount;

};

#endif