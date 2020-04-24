#ifndef __SERVER_WPSNODE_CONDITION_RECVTSEVENT__
#define __SERVER_WPSNODE_CONDITION_RECVTSEVENT__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvTsEvent : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvTsEvent(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvTsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	BYTE						m_byTriggerType;

	unsigned int				m_teid;

	DWORD						m_dwWaitTime;

	bool						m_bSynchronize;

};


#endif
