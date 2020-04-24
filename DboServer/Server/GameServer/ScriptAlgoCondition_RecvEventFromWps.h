#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMWPS_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMWPS_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvEventFromWps : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvEventFromWps(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvEventFromWps();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int					OnObjectMsg(CObjectMsg* pObjMsg);

private:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};

#endif