#ifndef __SERVER_WPSNODE_ACTION_CCBD_LEAVE__
#define __SERVER_WPSNODE_ACTION_CCBD_LEAVE__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_Leave : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_Leave(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_Leave();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
