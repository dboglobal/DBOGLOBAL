#ifndef __SERVER_CONTROLSCRIPT_NODEFACTORY__
#define __SERVER_CONTROLSCRIPT_NODEFACTORY__


#include "ControlScriptNodeAction.h"
#include "ControlScriptNodeCondition.h"


class CControlScriptNodeFactory
{

public:
	CControlScriptNodeFactory();
	virtual ~CControlScriptNodeFactory();


public:

	virtual CControlScriptNodeAction*		CreateControlNodeAction(const char* lpszNodeName) = 0;
	virtual CControlScriptNodeCondition*	CreateControlNodeCondition(const char* lpszNodeName) = 0;


};


#endif
