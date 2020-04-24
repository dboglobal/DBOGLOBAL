#ifndef __SERVER_CONTROLSCRIPT_NODEACTION__
#define __SERVER_CONTROLSCRIPT_NODEACTION__


#include "ControlScriptNode.h"


class CControlScriptNodeAction : public CControlScriptNode
{

public:
	CControlScriptNodeAction(int ctrlScrNodeID, const char* lpszName);
	virtual ~CControlScriptNodeAction();


public:

	virtual CControlScriptNode::eNODE_TYPE		GetNodeType() { return CControlScriptNode::NODE_ACTION; }


};




#endif
