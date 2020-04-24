#ifndef __SERVER_TQSONODE_CONDITION_RECVTSEVENT_
#define __SERVER_TQSONODE_CONDITION_RECVTSEVENT_


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_RecvTsEvent : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_RecvTsEvent(const char* lpszName);
	virtual ~CTQSNodeCondition_RecvTsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	BYTE						m_byTriggerType;

	DWORD						m_teid;

	DWORD						m_dwWaitTime;

	bool						m_bTmqFail;

	bool						m_bSynchronize;

};


#endif
