#ifndef __SERVER_SPSONODE_ACTION_SENDEVENTTOSPS__
#define __SERVER_SPSONODE_ACTION_SENDEVENTTOSPS__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEventToSps : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SendEventToSps(const char* lpszName);
	virtual ~CSPSNodeAction_SendEventToSps();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


private:

	eSPS_EVENT_TYPE				m_eEventType;

	BYTE						m_byTriggerType;

	DWORD						m_eventID;

};


#endif
