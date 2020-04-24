#ifndef __TQS_ALGO_DBOG_CONDITION_AREAENTER_H__
#define __TQS_ALGO_DBOG_CONDITION_AREAENTER_H__

#include "TqsAlgoCondition_Base.h"

class CTqsAlgoCondition_AreaEnter : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_AreaEnter(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_AreaEnter();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector					m_vStart;

	CNtlVector					m_vEnd;

	CNtlVector					m_vOrigin;

	float						m_fRadius;

	bool						m_bAllMember;

	DWORD						m_dwTimeLimit;

	bool						m_bTmqFail;

};

#endif