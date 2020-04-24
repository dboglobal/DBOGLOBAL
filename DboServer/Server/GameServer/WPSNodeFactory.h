#ifndef __SERVER_WPS_NODEFACTORY__
#define __SERVER_WPS_NODEFACTORY__


#include "ControlScriptNodeFactory.h"


class CWPSNodeFactory : public CControlScriptNodeFactory
{

public:
	CWPSNodeFactory();
	virtual ~CWPSNodeFactory();


public:

	virtual CControlScriptNodeAction*		CreateControlNodeAction(const char* lpszNodeName);
	virtual CControlScriptNodeCondition*	CreateControlNodeCondition(const char* lpszNodeName);

};


#endif