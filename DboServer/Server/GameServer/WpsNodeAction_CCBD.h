#ifndef __SERVER_WPSNODE_ACTION_CCBD__
#define __SERVER_WPSNODE_ACTION_CCBD__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
