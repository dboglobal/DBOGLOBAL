#ifndef __SERVER_TQS_NODEFACTORY__
#define __SERVER_TQS_NODEFACTORY__


#include "ControlScriptNodeFactory.h"


class CTQSNodeFactory : public CControlScriptNodeFactory
{

public:
	CTQSNodeFactory();
	virtual ~CTQSNodeFactory();


public:

	CControlScriptNodeAction*		CreateControlNodeAction(const char* lpszNodeName);
	CControlScriptNodeCondition*	CreateControlNodeCondition(const char* lpszNodeName);


};

#endif