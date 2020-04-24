#include "stdafx.h"
#include "SPSNodeAction_Rotate.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_Rotate::CSPSNodeAction_Rotate(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_ROTATE, lpszName)
{
}

CSPSNodeAction_Rotate::~CSPSNodeAction_Rotate()
{

}


CControlScriptNode::sPARAMETER_INFO* CSPSNodeAction_Rotate::GetParameterMap()
{
	return NULL;
}

bool CSPSNodeAction_Rotate::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "dir x") == 0)
	{
		m_vRotateDir.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dir y") == 0)
	{
		m_vRotateDir.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dir z") == 0)
	{
		m_vRotateDir.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Rotate: [%s] not exist \n", name);
	return false;
}
