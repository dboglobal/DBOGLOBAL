#ifndef __SERVER_CONTROLSCRIPT_NODEPARAM__
#define __SERVER_CONTROLSCRIPT_NODEPARAM__


#include "ControlScriptNode.h"


class CControlScriptNodeParam : public CControlScriptNode
{

public:
	CControlScriptNodeParam(const char * lpszName);
	virtual ~CControlScriptNodeParam();


public:

	virtual CControlScriptNode::eNODE_TYPE	GetNodeType() { return CControlScriptNode::NODE_PARAM; }

};


#endif
