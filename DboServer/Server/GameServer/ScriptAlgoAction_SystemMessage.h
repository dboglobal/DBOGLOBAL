#ifndef __SCRIPT_ALGO_DBOG_ACTION_SYSTEM_MESSAGE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_SYSTEM_MESSAGE_H__

#include "ScriptAlgoAction.h"
#include "NtlSharedDef.h"

enum eSERVER_TEXT_TYPE;

class CWpsAlgoAction_SystemMessage : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_SystemMessage(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_SystemMessage();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSERVER_TEXT_TYPE			m_eDisplayType;

	TBLIDX						m_textTblidx;

	WORD						m_wMessageLengthInUnicode;

	WCHAR						m_awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];

};

#endif