#include "stdafx.h"
#include "WPSNodeAction_AddMobGroup.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"
#include "NtlBattle.h"
#include "SvrScrFormulaToken.h"


CWPSNodeAction_AddMobGroup::CWPSNodeAction_AddMobGroup(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_MOB_GROUP, lpszName)
{
	Init();
}

CWPSNodeAction_AddMobGroup::~CWPSNodeAction_AddMobGroup()
{

}

CWPSNodeAction_AddMobGroup::sPARAMETER_INFO* CWPSNodeAction_AddMobGroup::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_AddMobGroup::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_spawnGroupId = (SPAWNGROUPID)pNode->GetValue();
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

	if (_stricmp(name, "faint buff index") == 0)
	{
		m_faintBuffIndex = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "faint buff range") == 0)
	{
		m_fFaintBuffRange = (float)(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "bind mob list") == 0)
	{
		m_nBindMobList = (int)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "drop item") == 0)
	{
		m_dropItem_ProbabilityTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "nest range") == 0)
	{
		m_byNestRange = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_AddMobGroup: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_AddMobGroup::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "no spawn wait") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "spawn func") == 0)
	{
		if (_stricmp(pNode->GetValue(), "no spawn wait") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_NO_SPAWN_WAIT);
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "respawn") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
			BIT_FLAG_SET(m_bySpawnFuncFlag, SPAWN_FUNC_FLAG_RESPAWN);
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "faint buff apply type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "party") == 0)
			m_byFaintBuffApplyType = eMOB_FAINT_BUFF_PARTY;
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "nest type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "fix") == 0)
			m_eNestType = NPC_NEST_TYPE_FIX;
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "bind mob") == 0)
	{
		m_strBindMob = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_AddMobGroup: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_AddMobGroup::CheckParameter()
{
	if (m_nBindMobList > 0)
		ReadBindList();

	return true;
}


void CWPSNodeAction_AddMobGroup::Init()
{
	m_spawnGroupId = INVALID_SPAWNGROUPID;

	m_dwPlayScriptNumber = INVALID_DWORD;
	m_dwPlayScriptScene = INVALID_DWORD;
	m_dwAIScriptNumber = INVALID_DWORD;
	m_dwAIScriptScene = INVALID_DWORD;

	m_qwCharConditionFlag = 0;
	m_bySpawnFuncFlag = 0;

	m_nBindMobList = 0;

	m_dropItem_ProbabilityTblidx = INVALID_TBLIDX;

	m_bPartyLeader = false;
	m_partyId = INVALID_PARTYID;

	m_bExecuterTargetFix = false;

	m_byImmortalMode = eIMMORTAL_MODE_OFF;

	m_eNestType = NPC_NEST_TYPE_DEFAULT;
	m_byNestRange = 80;

	m_faintBuffIndex = INVALID_TBLIDX;
	m_byFaintBuffApplyType = eMOB_FAINT_BUFF_SLAYER;
	m_fFaintBuffRange = 0.0f;
}

void CWPSNodeAction_AddMobGroup::ReadBindList()
{
	eBIND_STR_STATE eState = BIND_STR_STATE_LEFT;

	CSvrScrFormulaToken cToken;
	if (!cToken.Create())
		return;

	if (!cToken.Separate(m_strBindMob))
	{
		ERR_LOG(LOG_SCRIPT, "Fail AddSeparator");
		return;
	}

	sSSD_MOB_PAIR mobPair;
	mobPair.pairIndex = 0;
	mobPair.ePairType = SSD_MOB_PAIR_LIST_INDEX;

	for(CStringSeparator::TOKEN_IT it = cToken.Begin(); it != cToken.End(); it++)
	{
		switch (eState)
		{
			case BIND_STR_STATE_LEFT:
			{
				mobPair.tblidx = std::stoi(*it);

				eState = BIND_STR_STATE_EQUAL;
			}
			break;

			case BIND_STR_STATE_EQUAL:
			{
				eState = BIND_STR_STATE_RIGHT;
			}
			break;

			case BIND_STR_STATE_RIGHT:
			{
				std::string strIndex = *it;
				strIndex.erase(strIndex.find("mob"), 3);

				mobPair.pairIndex = std::stoi(strIndex) - 1;
				m_mapBindMob.insert({ mobPair.tblidx , mobPair });
				eState = BIND_STR_STATE_COMMA;
			}
			break;

			case BIND_STR_STATE_COMMA:
			{
				eState = BIND_STR_STATE_LEFT;
			}
			break;

			default: break;
		}
	}

	/*printf("m_mapBindMob.size() %u \n", m_mapBindMob.size());
	for (std::map<TBLIDX, sSSD_MOB_PAIR>::iterator it = m_mapBindMob.begin(); it != m_mapBindMob.end(); it++)
	{
		sSSD_MOB_PAIR& mobPair = it->second;
		printf("it->first %u, mobPair.tblidx %u, mobPair.ePairType %u, mobPair.pairIndex %u\n", it->first, mobPair.tblidx, mobPair.ePairType, mobPair.pairIndex);
	}*/
}
