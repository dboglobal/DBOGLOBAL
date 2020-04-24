#ifndef __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_SPS_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SEND_EVENT_TO_SPS_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_SendEventToSps : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SendEventToSps(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SendEventToSps();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eTargetType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventId;

};

#endif