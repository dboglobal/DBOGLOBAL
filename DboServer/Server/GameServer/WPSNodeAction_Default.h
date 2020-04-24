#ifndef __SERVER_WPSNODE_ACTION_DEFAULT__
#define __SERVER_WPSNODE_ACTION_DEFAULT__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_Default : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_Default(const char* lpszName);
	virtual ~CWPSNodeAction_Default();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
