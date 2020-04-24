#ifndef __SERVER_WPSNODE_CONDITION_WAITCONDITION__
#define __SERVER_WPSNODE_CONDITION_WAITCONDITION__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_WaitCondition : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_WaitCondition(const char* lpszName);
	virtual ~CWPSNodeCondition_WaitCondition();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bOperationAND;

};


#endif
