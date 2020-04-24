#ifndef __SCRIPT_ALGO_DBOG_ACTION_SENDEVENTTOTS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SENDEVENTTOTS_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_SendEventToTs : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendEventToTs(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendEventToTs();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE						m_byTriggerType;

	eSSD_TSEVENT_APPLY_TYPE		m_eApplyType;

	BYTE						m_byTargetType;

	TBLIDX						m_targetTblidx;

	CNtlVector					m_vLoc;

	float						m_fRadius;

	unsigned int				m_teId;


};

#endif