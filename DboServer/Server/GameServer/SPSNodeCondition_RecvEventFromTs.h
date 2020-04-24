#ifndef __SERVER_SPSONODE_CONDITION_RECVEVENTFROMTS__
#define __SERVER_SPSONODE_CONDITION_RECVEVENTFROMTS__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_RecvEventFromTs : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_RecvEventFromTs(const char* lpszName);
	virtual ~CSPSNodeCondition_RecvEventFromTs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	BYTE						m_byTriggerType;

	int							m_teId;

};


#endif
