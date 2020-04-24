#include "stdafx.h"
#include "WPSNodeAction_AddMob.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"
#include "NtlBattle.h"


CWPSNodeAction_AddMob::CWPSNodeAction_AddMob(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_MOB, lpszName)
{
	Init();
}

CWPSNodeAction_AddMob::~CWPSNodeAction_AddMob()
{

}

CWPSNodeAction_AddMob::sPARAMETER_INFO* CWPSNodeAction_AddMob::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_AddMob::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "index") == 0)
	{
		m_mobTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "group") == 0)
	{
		m_spawnGroupId = (SPAWNGROUPID)pNode->GetValue();
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
	if (_stricmp(name, "spawn time") == 0)
	{
		m_wSpawnTime = (WORD)pNode->GetValue() * 1000;
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "spawn loc range") == 0)
	{
		m_bySpawn_Loc_Range = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "spawn count") == 0)
	{
		m_bySpawnCount = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "nest range") == 0)
	{
		m_byNestRange = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "ais script") == 0)
	{
		m_dwAIScriptNumber = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "ais scene") == 0)
	{
		m_dwAIScriptScene = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "faint buff index") == 0)
	{
		m_faintBuffIndex = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "faint buff range") == 0)
	{
		m_fFaintBuffRange = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_AddMob: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_AddMob::AddParam(CControlScriptNodeParam_String* pNode)
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
	if (_stricmp(name, "immortal") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_byImmortalMode = eIMMORTAL_MODE_NORMAL;
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "respawn") == 0)
	{
		if (_stricmp(value, "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_RESPAWN);
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "use loc var") == 0)
	{
		if (_stricmp(value, "true") == 0)
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
	if (_stricmp(name, "nest type") == 0)
	{
		if (_stricmp(value, "fix") == 0)
			m_eNestType = NPC_NEST_TYPE_FIX;
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "faint buff apply type") == 0)
	{
		if (_stricmp(value, "party") == 0)
		{
			m_byFaintBuffApplyType = eMOB_FAINT_BUFF_PARTY;
		}
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_AddMob: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_AddMob::CheckParameter()
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


void CWPSNodeAction_AddMob::Init()
{
	m_spawnGroupId = INVALID_SPAWNGROUPID;
	m_mobTblidx = INVALID_TBLIDX;
	m_bySpawnCount = 1;
	m_wSpawnTime = 0;
	m_bySpawn_Loc_Range = 0;
	m_bUseLocVar = false;
	m_dwPlayScriptNumber = INVALID_DWORD;
	m_dwPlayScriptScene = INVALID_DWORD;
	m_battleScriptNumber = -1;
	m_dwBattleScriptScene = INVALID_DWORD;
	m_dwAIScriptNumber = INVALID_DWORD;
	m_dwAIScriptScene = INVALID_DWORD;
	m_qwCharConditionFlag = 0;
	m_bySpawnFuncFlag = 0;
	m_partyId = INVALID_PARTYID;
	m_bPartyLeader = false;
	m_dropItem_ProbabilityTblidx = INVALID_TBLIDX;
	m_bExecuterTargetFix = false;
	m_byImmortalMode = eIMMORTAL_MODE_OFF;
	m_byNestRange = 80;
	m_eNestType = NPC_NEST_TYPE_DEFAULT;
	m_faintBuffIndex = INVALID_TBLIDX;
	m_byFaintBuffApplyType = eMOB_FAINT_BUFF_SLAYER;
	m_fFaintBuffRange = 0.0f;
}