#ifndef __SERVER_CONTROLSCRIPT_NODESTATE__
#define __SERVER_CONTROLSCRIPT_NODESTATE__


#include "ControlScriptNode.h"


class CControlScriptNodeState : public CControlScriptNode
{

public:
	CControlScriptNodeState(int ctrlScrNodeID, const char* lpszName);
	virtual ~CControlScriptNodeState();


public:

	virtual CControlScriptNode::eNODE_TYPE		GetNodeType() { return CControlScriptNode::NODE_STATE; }

};


#endif