#ifndef __SERVER_SPSONODE_ACTION_WAIT__
#define __SERVER_SPSONODE_ACTION_WAIT__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_Wait : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_Wait(const char* lpszName);
	virtual ~CSPSNodeAction_Wait();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	DWORD						m_dwWaitTime;

	eSPS_EVENT_TYPE				m_eEventType;

	DWORD						m_eventID;

	bool						m_bOperationAND;

};


#endif
