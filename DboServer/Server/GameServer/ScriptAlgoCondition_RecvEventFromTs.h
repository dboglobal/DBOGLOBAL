#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMTS_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMTS_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvEventFromTs : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvEventFromTs(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvEventFromTs();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int					OnObjectMsg(CObjectMsg* pObjMsg);

private:

	BYTE						m_byTriggerType;

	unsigned int				m_teId;

	DWORD						m_dwWaitTime;

	bool						m_bSynchronize;

};

#endif