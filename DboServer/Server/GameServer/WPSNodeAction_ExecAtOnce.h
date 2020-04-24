#ifndef __SERVER_WPSNODE_ACTION_EXECATONCE__
#define __SERVER_WPSNODE_ACTION_EXECATONCE__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_ExecAtOnce : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ExecAtOnce(const char* lpszName);
	virtual ~CWPSNodeAction_ExecAtOnce();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
