#ifndef __SERVER_AISONODE_CONDITION_RECVEVENTFROMTQS__
#define __SERVER_AISONODE_CONDITION_RECVEVENTFROMTQS__


#include "ControlScriptNodeCondition.h"


class CAISNodeCondition_RecvEventFromTQS : public CControlScriptNodeCondition
{

public:
	CAISNodeCondition_RecvEventFromTQS(const char* lpszName);
	virtual ~CAISNodeCondition_RecvEventFromTQS();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	DWORD						m_eventId;

};


#endif
