#ifndef __SERVER_WPSNODE_CONDITION_RECVEVENTFROMTS__
#define __SERVER_WPSNODE_CONDITION_RECVEVENTFROMTS__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvEventFromTs : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvEventFromTs(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvEventFromTs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	BYTE						m_byTriggerType;

	unsigned int				m_teId;

	DWORD						m_dwWaitTime;

	bool						m_bSynchronize;

};


#endif
