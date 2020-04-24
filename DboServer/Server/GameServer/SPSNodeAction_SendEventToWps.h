#ifndef __SERVER_SPSONODE_ACTION_SENDEVENTTOWPS__
#define __SERVER_SPSONODE_ACTION_SENDEVENTTOWPS__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEventToWps : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_SendEventToWps(const char* lpszName);
	virtual ~CSPSNodeAction_SendEventToWps();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};


#endif
