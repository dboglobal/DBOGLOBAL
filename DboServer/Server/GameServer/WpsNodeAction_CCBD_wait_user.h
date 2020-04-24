#ifndef __SERVER_WPSNODE_ACTION_CCBD_WAIT_USER__
#define __SERVER_WPSNODE_ACTION_CCBD_WAIT_USER__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_wait_user : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_wait_user(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_wait_user();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
