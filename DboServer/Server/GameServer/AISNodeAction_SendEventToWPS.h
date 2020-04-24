#ifndef __SERVER_AISONODE_ACTION_SENDEVENTTOWPS__
#define __SERVER_AISONODE_ACTION_SENDEVENTTOWPS__


#include "ControlScriptNodeAction.h"


class CAISNodeAction_SendEventToWPS : public CControlScriptNodeAction
{

public:
	CAISNodeAction_SendEventToWPS(const char* lpszName);
	virtual ~CAISNodeAction_SendEventToWPS();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

private:

	void						Init() {}


public:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};


#endif
