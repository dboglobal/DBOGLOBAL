#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMSPS_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMSPS_H__

#include "ScriptAlgoCondition.h"

enum eOBJTYPE;

class CWpsAlgoCondition_RecvEventFromSps : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvEventFromSps(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvEventFromSps();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int					OnObjectMsg(CObjectMsg* pObjMsg);

private:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_objectTblidx;

	DWORD						m_eventId;

};

#endif