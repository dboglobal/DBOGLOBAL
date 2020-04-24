#ifndef __SERVER_SPS_NODEFACTORY__
#define __SERVER_SPS_NODEFACTORY__


#include "ControlScriptNodeFactory.h"

class CSPSNodeFactory : public CControlScriptNodeFactory
{

public:
	CSPSNodeFactory();
	virtual ~CSPSNodeFactory();


public:

	virtual CControlScriptNodeAction*		CreateControlNodeAction(const char* lpszNodeName);
	virtual CControlScriptNodeCondition*	CreateControlNodeCondition(const char* lpszNodeName);


};

#endif