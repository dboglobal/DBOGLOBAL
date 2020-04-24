#include "stdafx.h"
#include "WPSNodeCondition_SpawnedMob.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeCondition_SpawnedMob::CWPSNodeCondition_SpawnedMob(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_SPAWNED_MOB, lpszName)
{
}

CWPSNodeCondition_SpawnedMob::~CWPSNodeCondition_SpawnedMob()
{

}

bool CWPSNodeCondition_SpawnedMob::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_mobSpawnGroupId = (SPAWNGROUPID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "count") == 0)
	{
		m_byMobCount = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_SpawnedMob: [%s] not exist \n", name);
	return false;
}
