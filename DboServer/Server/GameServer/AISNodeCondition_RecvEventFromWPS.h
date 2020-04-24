#ifndef __SERVER_AISONODE_CONDITION_RECVEVENTFROMWPS__
#define __SERVER_AISONODE_CONDITION_RECVEVENTFROMWPS__


#include "ControlScriptNodeCondition.h"


class CAISNodeCondition_RecvEventFromWPS : public CControlScriptNodeCondition
{

public:
	CAISNodeCondition_RecvEventFromWPS(const char* lpszName);
	virtual ~CAISNodeCondition_RecvEventFromWPS();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_wpsTblidx;
	
	DWORD						m_eventId;
};


#endif
