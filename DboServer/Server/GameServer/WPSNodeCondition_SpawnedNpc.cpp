#include "stdafx.h"
#include "WPSNodeCondition_SpawnedNpc.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeCondition_SpawnedNpc::CWPSNodeCondition_SpawnedNpc(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_SPAWNED_NPC, lpszName)
{
}

CWPSNodeCondition_SpawnedNpc::~CWPSNodeCondition_SpawnedNpc()
{

}

bool CWPSNodeCondition_SpawnedNpc::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "tblidx") == 0)
	{
		m_npcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}


	printf("CWPSNodeCondition_SpawnedNpc: [%s] not exist \n", name);
	return false;
}
