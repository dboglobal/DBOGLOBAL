#ifndef __SERVER_WPSNODE_ACTION_TELECASTMESSAGE__
#define __SERVER_WPSNODE_ACTION_TELECASTMESSAGE__


#include "ControlScriptNodeAction.h"

enum eTELECAST_MESSAGE_TYPE;

class CWPSNodeAction_TelecastMessage : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_TelecastMessage(const char* lpszName);
	virtual ~CWPSNodeAction_TelecastMessage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	bool						m_bApply;

	TBLIDX						m_npcTblidx;

	eTELECAST_MESSAGE_TYPE		m_eTelecastType;

	TBLIDX						m_speechTblidx;

	DWORD						m_dwDisplayTime;

};


#endif
