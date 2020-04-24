#ifndef __SERVER_TQSONODE_CONDITION_RECVSPSEVENT__
#define __SERVER_TQSONODE_CONDITION_RECVSPSEVENT__


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_RecvSpsEvent : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_RecvSpsEvent(const char* lpszName);
	virtual ~CTQSNodeCondition_RecvSpsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

	DWORD						m_dwTime;

};


#endif
