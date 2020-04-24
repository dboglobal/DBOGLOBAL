#ifndef __TQS_ALGO_DBOG_CONDITION_NPCFAINT_H__
#define __TQS_ALGO_DBOG_CONDITION_NPCFAINT_H__

#include "TqsAlgoCondition_Base.h"

class CTqsAlgoCondition_NpcFaint : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_NpcFaint(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_NpcFaint();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX				m_npcTblidx;

	bool				m_bTmqFail;

};

#endif