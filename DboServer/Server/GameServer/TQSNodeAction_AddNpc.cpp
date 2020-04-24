#include "stdafx.h"
#include "TQSNodeAction_AddNpc.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlCharacter.h"


CTQSNodeAction_AddNpc::CTQSNodeAction_AddNpc(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_ADD_NPC, lpszName)
{
	Init();
}

CTQSNodeAction_AddNpc::~CTQSNodeAction_AddNpc()
{
}

CTQSNodeAction_AddNpc::sPARAMETER_INFO* CTQSNodeAction_AddNpc::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_AddNpc::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_npcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
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
		m_vDir.x = (float)(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "dir y") == 0)
	{
		m_vDir.y = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "dir z") == 0)
	{
		m_vDir.z = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "play script") == 0)
	{
		m_dwPlayScriptNumber = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "play scene") == 0)
	{
		m_dwPlayScriptScene = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_AddNpc: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_AddNpc::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "respawn") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_RESPAWN);

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "no spawn wait") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "char condition") == 0)
	{
		if (_stricmp(pNode->GetValue(), "click disable") == 0)
			BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_CLICK_DISABLE));
		else if (_stricmp(pNode->GetValue(), "direct play") == 0)
			BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_DIRECT_PLAY));
		else if (_stricmp(pNode->GetValue(), "invincible") == 0)
			BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_INVINCIBLE));
		else
		{
			printf("CTQSNodeAction_AddNpc: invalid char condition [%s] \n", pNode->GetValue());
		}

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_AddNpc: [%s] not exist \n", name);
	return false;
}

void CTQSNodeAction_AddNpc::Init()
{
	m_npcTblidx = INVALID_TBLIDX;
	m_vLoc.Reset();
	m_vDir.Reset();
	m_bySpawnFuncFlag = 0;
	m_dwPlayScriptNumber = INVALID_DWORD;
	m_dwPlayScriptScene = INVALID_DWORD;
	m_qwCharConditionFlag = 0;
}
