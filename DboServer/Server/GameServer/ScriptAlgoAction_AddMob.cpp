#include "stdafx.h"
#include "ScriptAlgoAction_AddMob.h"
#include "WPSNodeAction_AddMob.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "MobTable.h"
#include "Monster.h"


CWpsAlgoAction_AddMob::CWpsAlgoAction_AddMob(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_ADDMOB, "SCRIPTCONTROL_ACTION_ADDMOB")
{
}


CWpsAlgoAction_AddMob::~CWpsAlgoAction_AddMob()
{
}


bool CWpsAlgoAction_AddMob::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_AddMob* pAction = dynamic_cast<CWPSNodeAction_AddMob*>(pControlScriptNode);
	if (pAction)
	{
		m_spawnGroupId = pAction->m_spawnGroupId;
		m_mobTblidx = pAction->m_mobTblidx;
		m_bySpawnCount = pAction->m_bySpawnCount;
		m_wSpawnTime = pAction->m_wSpawnTime;
		m_bySpawn_Loc_Range = pAction->m_bySpawn_Loc_Range;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;

		m_bUseLocVar = pAction->m_bUseLocVar;
		m_strLocVar = pAction->m_strLocVar;
		m_strDirVar = pAction->m_strDirVar;

		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;
		m_battleScriptNumber = pAction->m_battleScriptNumber;
		m_dwBattleScriptScene = pAction->m_dwBattleScriptScene;
		m_dwAIScriptNumber = pAction->m_dwAIScriptNumber;
		m_dwAIScriptScene = pAction->m_dwAIScriptScene;
		m_qwCharConditionFlag = pAction->m_qwCharConditionFlag;
		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_partyId = pAction->m_partyId;
		m_bPartyLeader = pAction->m_bPartyLeader;
		m_dropItem_ProbabilityTblidx = pAction->m_dropItem_ProbabilityTblidx;
		m_bExecuterTargetFix = pAction->m_bExecuterTargetFix;
		m_byImmortalMode = pAction->m_byImmortalMode;
		m_eNestType = pAction->m_eNestType;
		m_byNestRange = pAction->m_byNestRange;

		m_faintBuffIndex = pAction->m_faintBuffIndex;
		m_byFaintBuffApplyType = pAction->m_byFaintBuffApplyType;
		m_fFaintBuffRange = pAction->m_fFaintBuffRange;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_AddMob", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_AddMob::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	sMOB_DATA data;
	InitMobData(data);

	data.spawnGroupId = m_spawnGroupId;

	if (GetOwner()->GetWorld())
	{
		data.worldID = GetOwner()->GetWorld()->GetID();
		data.worldtblidx = GetOwner()->GetWorld()->GetIdx();
	}
	else
	{
		data.worldID = 1;
		data.worldtblidx = 1;
	}

	data.tblidx = m_mobTblidx;


	if (m_bUseLocVar)
	{
		sSSD_VARIABLE* pLocVar = GetVariable(m_strLocVar);
		if (!pLocVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strLocVar %s \n", m_strLocVar.c_str());
			m_status = FAILED;
			return m_status;
		}
		sSSD_VARIABLE* pDirVar = GetVariable(m_strDirVar);
		if (!pDirVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strDirVar %s \n", m_strDirVar.c_str());
			m_status = FAILED;
			return m_status;
		}

		m_vLoc = pLocVar->sVector;
		m_vDir = pDirVar->sVector;
	}

	m_vLoc.CopyTo(data.vCurLoc);
	m_vLoc.CopyTo(data.vSpawnLoc);
	m_vDir.CopyTo(data.vCurDir);
	m_vDir.CopyTo(data.vSpawnDir);
	data.wSpawnTime = m_wSpawnTime;
	data.bySpawnRange = m_bySpawn_Loc_Range;
	data.bySpawnFuncFlag = m_bySpawnFuncFlag;
	data.sScriptData.playScript = (TBLIDX)m_dwPlayScriptNumber;
	data.sScriptData.playScriptScene = (TBLIDX)m_dwPlayScriptScene;
	data.sScriptData.tblidxBattleScript = (TBLIDX)m_battleScriptNumber;
	data.sScriptData.dwBattleScriptScene = (TBLIDX)m_dwBattleScriptScene;
	data.sScriptData.tblidxAiScript = (TBLIDX)m_dwAIScriptNumber;
	data.sScriptData.tblidxAiScriptScene = (TBLIDX)m_dwAIScriptScene;
	data.qwCharConditionFlag = m_qwCharConditionFlag;
	data.partyID = m_partyId;
	data.bPartyLeader = m_bPartyLeader;
	data.byImmortalMode = m_byImmortalMode;
	data.actionpatternTblIdx = 1;
	data.hTargetFixedExecuter = m_bExecuterTargetFix;
	data.sBotSubData.byNestType = m_eNestType;
	data.sBotSubData.byNestRange = m_byNestRange;
	data.byMoveType = SPAWN_MOVE_WANDER;

	for (BYTE i = 0; i < m_bySpawnCount; i++)
	{
		sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(m_mobTblidx);
		if (pTbldat)
		{
			CMonster* pMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
			if (pMob)
			{
				if (pMob->CreateDataAndSpawn(data, pTbldat))
				{
					pMob->SetFaintBuff(m_faintBuffIndex, m_byFaintBuffApplyType, m_fFaintBuffRange);
					pMob->SetDropItemProbabilityTblidx(m_dropItem_ProbabilityTblidx);
					pMob->AddScript(GetOwner());	//add script
					pMob->SetStandAlone(false);
					GetOwner()->AddSpawnGroupMob(m_spawnGroupId, pMob->GetID());
					m_status = COMPLETED;
				}
				else m_status = FAILED;
			}
			else m_status = FAILED;
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u", m_mobTblidx);
			m_status = FAILED;
		}
	}

	return m_status;
}
