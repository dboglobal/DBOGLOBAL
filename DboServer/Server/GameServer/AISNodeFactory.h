#ifndef __SERVER_AIS_NODEFACTORY__
#define __SERVER_AIS_NODEFACTORY__


#include "ControlScriptNodeFactory.h"

class CAISNodeFactory : public CControlScriptNodeFactory
{

public:
	CAISNodeFactory();
	virtual ~CAISNodeFactory();


public: 
	
	virtual CControlScriptNodeAction*		CreateControlNodeAction(const char* lpszNodeName);
	virtual CControlScriptNodeCondition*	CreateControlNodeCondition(const char* lpszNodeName);
	
};


#endif