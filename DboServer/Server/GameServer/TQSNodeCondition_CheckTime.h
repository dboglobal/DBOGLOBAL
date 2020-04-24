#ifndef __SERVER_TQSONODE_CONDITION_CHECKTIME_
#define __SERVER_TQSONODE_CONDITION_CHECKTIME_


#include "ControlScriptNodeCondition.h"


class CTQSNodeCondition_CheckTime : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_CheckTime(const char* lpszName);
	virtual ~CTQSNodeCondition_CheckTime();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	DWORD						m_dwWaitTime;

};


#endif
