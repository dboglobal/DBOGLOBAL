#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMTQS__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMTQS__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvEventFromTqs : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromTqs(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromTqs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	DWORD						m_eventId;

};


#endif
