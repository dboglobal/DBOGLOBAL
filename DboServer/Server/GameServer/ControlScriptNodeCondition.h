#ifndef __SERVER_CONTROLSCRIPT_NODECONDITION__
#define __SERVER_CONTROLSCRIPT_NODECONDITION__


#include "ControlScriptNode.h"


class CControlScriptNodeCondition : public CControlScriptNode
{

public:
	CControlScriptNodeCondition(int ctrlScrNodeID, const char * lpszName);
	virtual ~CControlScriptNodeCondition();


public:

	virtual CControlScriptNode::eNODE_TYPE		GetNodeType() { return CControlScriptNode::NODE_CONDITION; }


};


#endif
