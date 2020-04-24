#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMWPS__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMWPS__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvEventFromWps : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromWps(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromWps();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};


#endif
