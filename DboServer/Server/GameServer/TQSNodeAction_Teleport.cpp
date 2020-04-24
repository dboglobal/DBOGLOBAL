#include "stdafx.h"
#include "TQSNodeAction_Teleport.h"
#include "ControlScriptNodeParam_Number.h"


CTQSNodeAction_Teleport::CTQSNodeAction_Teleport(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_TELEPORT, lpszName)
{
	m_vLoc.Reset();
	m_vDir.Reset();
}

CTQSNodeAction_Teleport::~CTQSNodeAction_Teleport()
{

}

CTQSNodeAction_Teleport::sPARAMETER_INFO* CTQSNodeAction_Teleport::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_Teleport::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "loc x") == 0)
	{
		m_vLoc.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "loc y") == 0)
	{
		m_vLoc.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "loc z") == 0)
	{
		m_vLoc.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	
	if (_stricmp(name, "dir x") == 0)
	{
		m_vDir.x = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "dir z") == 0)
	{
		m_vDir.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_Teleport: [%s] not exist \n", name);
	return false;
}
