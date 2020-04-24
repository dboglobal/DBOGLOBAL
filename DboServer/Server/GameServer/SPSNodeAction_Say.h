#ifndef __SERVER_SPSONODE_ACTION_SAY__
#define __SERVER_SPSONODE_ACTION_SAY__


#include "ControlScriptNodeAction.h"
#include "NtlChat.h"


class CSPSNodeAction_Say : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_Say(const char* lpszName);
	virtual ~CSPSNodeAction_Say();


	sPARAMETER_INFO*			GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eCHAR_DIALOG_TYPE		m_byDialogType;

	TBLIDX					m_textTblidx;

	CNtlString				m_strText;
};


#endif