#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECV_SPS_EVENT_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECV_SPS_EVENT_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvSpsEvent : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvSpsEvent(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvSpsEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int					OnObjectMsg(CObjectMsg* pObjMsg);

private:

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

	DWORD						m_dwTime;

};

#endif