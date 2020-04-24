#ifndef __SERVER_WPSNODE_ACTION_SENDEVENTTOTQS__
#define __SERVER_WPSNODE_ACTION_SENDEVENTTOTQS__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_SendEventToTqs : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendEventToTqs(const char* lpszName);
	virtual ~CWPSNodeAction_SendEventToTqs();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	DWORD						m_eventId;

};


#endif
