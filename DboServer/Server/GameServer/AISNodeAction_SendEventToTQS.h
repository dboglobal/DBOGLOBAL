#ifndef __SERVER_AISONODE_ACTION_SENDEVENTTOTQS__
#define __SERVER_AISONODE_ACTION_SENDEVENTTOTQS__


#include "ControlScriptNodeAction.h"


class CAISNodeAction_SendEventToTQS : public CControlScriptNodeAction
{

public:
	CAISNodeAction_SendEventToTQS(const char* lpszName);
	virtual ~CAISNodeAction_SendEventToTQS();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

private:

	void						Init() {}


public:

	DWORD						m_eventId;

};


#endif
