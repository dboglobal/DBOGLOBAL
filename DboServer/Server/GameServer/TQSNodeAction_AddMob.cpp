#include "stdafx.h"
#include "TQSNodeAction_AddMob.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlCharacter.h"


CTQSNodeAction_AddMob::CTQSNodeAction_AddMob(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_ADD_MOB, lpszName)
{
	Init();
}

CTQSNodeAction_AddMob::~CTQSNodeAction_AddMob()
{
}

CTQSNodeAction_AddMob::sPARAMETER_INFO* CTQSNodeAction_AddMob::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_AddMob::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_mobTblidx = (TBLIDX)pNode->GetValue();
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
		m_vDir.x = (float)pNode->GetValue();
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
	if (_stricmp(name, "party id") == 0)
	{
		m_partyId = (PARTYID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_AddMob: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_AddMob::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "no spawn wait") == 0)
	{
		if (_stricmp(value, "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "party leader") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bPartyLeader = true;
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "respawn") == 0)
	{
		if (_stricmp(value, "true") == 0)
		{
			m_bRespawn = true;
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_RESPAWN);
		}
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_AddMob: [%s] not exist \n", name);
	return false;
}

void CTQSNodeAction_AddMob::Init()
{
	m_mobTblidx = INVALID_TBLIDX;
	m_vLoc.Reset();
	m_vDir.Reset();
	m_bRespawn = false;
	m_dwPlayScriptNumber = INVALID_DWORD;
	m_dwPlayScriptScene = INVALID_DWORD;
	m_partyId = INVALID_PARTYID;
	m_bPartyLeader = false;
	m_bySpawnFuncFlag = 0;
}
