#ifndef __SERVER_WPSNODE_CONDITION_CHECKCOUNTDOWN__
#define __SERVER_WPSNODE_CONDITION_CHECKCOUNTDOWN__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_CheckCountDown : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_CheckCountDown(const char* lpszName);
	virtual ~CWPSNodeCondition_CheckCountDown();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	DWORD						m_dwWaitTime;

};


#endif
