#ifndef __SERVER_SPSONODE_ACTION_SENDEVENTTOTQS__
#define __SERVER_SPSONODE_ACTION_SENDEVENTTOTQS__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEventToTqs : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SendEventToTqs(const char* lpszName);
	virtual ~CSPSNodeAction_SendEventToTqs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	DWORD						m_eventId;

};


#endif
