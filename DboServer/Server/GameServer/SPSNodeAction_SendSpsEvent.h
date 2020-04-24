#ifndef __SERVER_SPSONODE_ACTION_SENDSPSEVENT__
#define __SERVER_SPSONODE_ACTION_SENDSPSEVENT__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendSpsEvent : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SendSpsEvent(const char* lpszName);
	virtual ~CSPSNodeAction_SendSpsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eSPS_EVENT_TYPE				m_eEventType;

	BYTE						m_byTriggerType;

	DWORD						m_eventID;

};


#endif
