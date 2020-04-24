#include "stdafx.h"
#include "SPSNodeAction_DirectTurning.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeAction_DirectTurning::CSPSNodeAction_DirectTurning(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_DIRECT_TURNING, lpszName)
{
}

CSPSNodeAction_DirectTurning::~CSPSNodeAction_DirectTurning()
{

}

bool CSPSNodeAction_DirectTurning::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "tblidx") == 0)
	{
		m_directionTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_DirectTurning: [%s] not exist \n", name);

	return false;
}
