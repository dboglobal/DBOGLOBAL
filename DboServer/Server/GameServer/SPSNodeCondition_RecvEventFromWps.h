#ifndef __SERVER_SPSONODE_CONDITION_RECVEVENTFROMWPS__
#define __SERVER_SPSONODE_CONDITION_RECVEVENTFROMWPS__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_RecvEventFromWps : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_RecvEventFromWps(const char* lpszName);
	virtual ~CSPSNodeCondition_RecvEventFromWps();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_wpsTblidx;

	DWORD						m_eventId;

};


#endif
