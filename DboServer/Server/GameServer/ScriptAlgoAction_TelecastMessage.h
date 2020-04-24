#ifndef __SCRIPT_ALGO_DBOG_ACTION_TELECAST_MESSAGR_H__
#define __SCRIPT_ALGO_DBOG_ACTION_TELECAST_MESSAGR_H__

#include "ScriptAlgoAction.h"


enum eTELECAST_MESSAGE_TYPE;

class CWpsAlgoAction_TelecastMessage : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_TelecastMessage(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_TelecastMessage();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bApply;

	TBLIDX						m_npcTblidx;

	eTELECAST_MESSAGE_TYPE		m_eTelecastType;

	TBLIDX						m_speechTblidx;

	DWORD						m_dwDisplayTime;

};

#endif