#ifndef __SCRIPT_ALGO_DBOG_ACTION_SEND_AIS_EVENT_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SEND_AIS_EVENT_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_SendAISEvent : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendAISEvent(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendAISEvent();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

};

#endif