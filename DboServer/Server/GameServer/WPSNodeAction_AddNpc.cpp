#include "stdafx.h"
#include "WPSNodeAction_AddNpc.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"
#include "NtlBattle.h"


CWPSNodeAction_AddNpc::CWPSNodeAction_AddNpc(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_NPC, lpszName)
{
	Init();
}

CWPSNodeAction_AddNpc::~CWPSNodeAction_AddNpc()
{
}

CWPSNodeAction_AddNpc::sPARAMETER_INFO* CWPSNodeAction_AddNpc::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_AddNpc::AddParam(CControlScriptNodeParam_Number* pNode)
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
		m_vDir.x = (float)pNode->GetValue();
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

	printf("CWPSNodeAction_AddNpc(number): [%s] not exist \n", name);

	return false;
}

bool CWPSNodeAction_AddNpc::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "use loc var") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bUseLocVar = true;

		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "loc var") == 0)
	{
		m_strLocVar = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "dir var") == 0)
	{
		m_strDirVar = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

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


	if (_stricmp(name, "stand alone") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bStandAlone = true;

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "char condition") == 0)
	{
		if (_stricmp(pNode->GetValue(), "click disable") == 0)
			BIT_FLAG_SET(m_qwCharConditionFlag, MAKE_BIT_FLAG(CHARCOND_CLICK_DISABLE));
		else
		{
			printf("CWPSNodeAction_AddNpc: invalid char condition [%s] \n", pNode->GetValue());
		}

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "party leader") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_bPartyLeader = true;

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "immortal") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			m_byImmortalMode = eIMMORTAL_MODE_NORMAL;

		return CControlScriptNode::AddParam(pNode);
	}
	

	printf("CWPSNodeAction_AddNpc(string): [%s] not exist \n", name);

	return false;
}


bool CWPSNodeAction_AddNpc::CheckParameter()
{
	if (m_bUseLocVar)
	{
		sSSD_VARIABLE* pLocVar = GetVariable(m_strLocVar);
		if (!pLocVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strLocVar %s \n", m_strLocVar.c_str());
			return false;
		}
		sSSD_VARIABLE* pDirVar = GetVariable(m_strDirVar);
		if (!pDirVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strDirVar %s \n", m_strDirVar.c_str());
			return false;
		}

		m_vLoc = pLocVar->sVector;
		m_vDir = pDirVar->sVector;
	}

	return true;
}


void CWPSNodeAction_AddNpc::Init()
{
	m_npcTblidx = INVALID_TBLIDX;

	m_bUseLocVar = false;

	m_bySpawnFuncFlag = 0;

	m_dwPlayScriptNumber = INVALID_DWORD;
	m_dwPlayScriptScene = INVALID_DWORD;

	m_dwAISNumber = INVALID_DWORD;
	m_dwAISScene = INVALID_DWORD;

	m_qwCharConditionFlag = 0;

	m_bStandAlone = false;
	
	m_partyId = INVALID_PARTYID;
	m_bPartyLeader = false;

	m_byImmortalMode = eIMMORTAL_MODE_OFF;

}