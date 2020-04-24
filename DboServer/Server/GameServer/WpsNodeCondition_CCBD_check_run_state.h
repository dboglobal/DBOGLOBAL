#ifndef __SERVER_WPSNODE_CONDITION_CCBD_CHECK_RUN_STATE__
#define __SERVER_WPSNODE_CONDITION_CCBD_CHECK_RUN_STATE__


#include "ControlScriptNodeCondition.h"


class CWpsNodeCondition_CCBD_check_run_state : public CControlScriptNodeCondition
{

public:
	CWpsNodeCondition_CCBD_check_run_state(const char* lpszName);
	virtual ~CWpsNodeCondition_CCBD_check_run_state();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	

};


#endif
