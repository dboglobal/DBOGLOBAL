#ifndef __SERVER_WPSNODE_ACTION_SYSTEMMESSAGE__
#define __SERVER_WPSNODE_ACTION_SYSTEMMESSAGE__


#include "ControlScriptNodeAction.h"
#include "NtlSharedDef.h"

enum eSERVER_TEXT_TYPE;

class CWPSNodeAction_SystemMessage : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SystemMessage(const char* lpszName);
	virtual ~CWPSNodeAction_SystemMessage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eSERVER_TEXT_TYPE			m_eDisplayType;

	TBLIDX						m_textTblidx;

	WORD						m_wMessageLengthInUnicode;

	WCHAR						m_awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];

};


#endif
