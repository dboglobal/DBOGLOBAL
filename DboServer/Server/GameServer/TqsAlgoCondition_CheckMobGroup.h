#ifndef __TQS_ALGO_DBOG_CONDITION_CHECKMOBGRPUP_H__
#define __TQS_ALGO_DBOG_CONDITION_CHECKMOBGRPUP_H__

#include "TqsAlgoCondition_Base.h"

class CTqsAlgoCondition_CheckMobGroup : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_CheckMobGroup(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_CheckMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	SPAWNGROUPID				m_spawnGroupId;

	int							m_nCheckCount;

};

#endif