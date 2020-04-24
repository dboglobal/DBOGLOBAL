#ifndef __SERVER_WPSNODE_ACTION_SENDEVENTTOWPS__
#define __SERVER_WPSNODE_ACTION_SENDEVENTTOWPS__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_SendEventToWps : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_SendEventToWps(const char* lpszName);
	virtual ~CWPSNodeAction_SendEventToWps();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};


#endif
