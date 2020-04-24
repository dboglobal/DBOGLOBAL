#ifndef __SERVER_TQSONODE_ACTION_PROGRESSMESSAGE__
#define __SERVER_TQSONODE_ACTION_PROGRESSMESSAGE__


#include "ControlScriptNodeAction.h"

enum ePROGRESS_MESSAGE_TYPE;
enum eTIMEQUEST_PROGRESS_MESSAGE_TYPE;

class CTQSNodeAction_ProgressMessage : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_ProgressMessage(const char* lpszName);
	virtual ~CTQSNodeAction_ProgressMessage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	ePROGRESS_MESSAGE_TYPE		m_eProgressType;

	eTIMEQUEST_PROGRESS_MESSAGE_TYPE	m_eMessageType;

	BYTE						m_byMessageValue;

};


#endif
