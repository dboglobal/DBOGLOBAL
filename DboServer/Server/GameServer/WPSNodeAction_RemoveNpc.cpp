#include "stdafx.h"
#include "WPSNodeAction_RemoveNpc.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlCharacter.h"


CWPSNodeAction_RemoveNpc::CWPSNodeAction_RemoveNpc(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_REMOVE_NPC, lpszName)
{
	m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_DESPAWN;
}

CWPSNodeAction_RemoveNpc::~CWPSNodeAction_RemoveNpc()
{

}

CWPSNodeAction_RemoveNpc::sPARAMETER_INFO* CWPSNodeAction_RemoveNpc::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_RemoveNpc::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_npcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_RemoveNpc: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_RemoveNpc::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "clear") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_CLEAR;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "faint") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_FAINT;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "despawn") == 0)
		{
			m_eSpawnRemoveType = SPAWN_REMOVE_TYPE_DESPAWN;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_RemoveNpc: [%s] not exist \n", name);
	return false;
}