#ifndef __SERVER_WPSNODE_ACTION_CCBD_STAGE_CLEAR__
#define __SERVER_WPSNODE_ACTION_CCBD_STAGE_CLEAR__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_stage_clear : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_stage_clear(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_stage_clear();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
