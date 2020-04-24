#include "stdafx.h"
#include "WPSNodeAction_RemoveMobGroup.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlCharacter.h"


CWPSNodeAction_RemoveMobGroup::CWPSNodeAction_RemoveMobGroup(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_REMOVE_MOB_GROUP, lpszName)
{
	m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_DESPAWN;
}

CWPSNodeAction_RemoveMobGroup::~CWPSNodeAction_RemoveMobGroup()
{

}

CWPSNodeAction_RemoveMobGroup::sPARAMETER_INFO* CWPSNodeAction_RemoveMobGroup::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_RemoveMobGroup::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_spawnGroupId = (SPAWNGROUPID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_RemoveMobGroup: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_RemoveMobGroup::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "faint") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_FAINT;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "clear") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_CLEAR;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "despawn") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_DESPAWN;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_RemoveMobGroup: [%s] not exist \n", name);
	return false;
}