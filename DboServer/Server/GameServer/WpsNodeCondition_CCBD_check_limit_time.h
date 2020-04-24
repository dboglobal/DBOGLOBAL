#ifndef __SERVER_WPSNODE_CONDITION_CCBD_CHECK_TIME_LIMIT__
#define __SERVER_WPSNODE_CONDITION_CCBD_CHECK_TIME_LIMIT__


#include "ControlScriptNodeCondition.h"


class CWpsNodeCondition_CCBD_check_limit_time : public CControlScriptNodeCondition
{

public:
	CWpsNodeCondition_CCBD_check_limit_time(const char* lpszName);
	virtual ~CWpsNodeCondition_CCBD_check_limit_time();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:



};


#endif
