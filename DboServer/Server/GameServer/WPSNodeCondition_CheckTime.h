#ifndef __SERVER_WPSNODE_CONDITION_CHECKTIME__
#define __SERVER_WPSNODE_CONDITION_CHECKTIME__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_CheckTime : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_CheckTime(const char* lpszName);
	virtual ~CWPSNodeCondition_CheckTime();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	DWORD						m_dwWaitTime;

};


#endif
