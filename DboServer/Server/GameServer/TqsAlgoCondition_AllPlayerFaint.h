#ifndef __TQS_ALGO_DBOG_CONDITION_ALL_PLAYER_FAINT_H__
#define __TQS_ALGO_DBOG_CONDITION_ALL_PLAYER_FAINT_H__

#include "TqsAlgoCondition_Base.h"

class CTqsAlgoCondition_AllPlayerFaint : public CTqsAlgoCondition_Base
{

public:

	CTqsAlgoCondition_AllPlayerFaint(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoCondition_AllPlayerFaint();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bTmqFail;

};

#endif