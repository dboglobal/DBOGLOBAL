#ifndef __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_AIS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_AIS_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_SendEventToAis : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendEventToAis(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendEventToAis();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eTargetType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventId;

};

#endif