#include "stdafx.h"
#include "TqsAlgoAction_AddMobGroup.h"
#include "TQSNodeAction_AddMobGroup.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "MobTable.h"
#include "Monster.h"


CTqsAlgoAction_AddMobGroup::CTqsAlgoAction_AddMobGroup(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_ADDMOB_GROUP, "TQS_ALGOCONTROLID_ACTION_ADDMOB_GROUP")
{
}


CTqsAlgoAction_AddMobGroup::~CTqsAlgoAction_AddMobGroup()
{
}


bool CTqsAlgoAction_AddMobGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_AddMobGroup* pAction = dynamic_cast<CTQSNodeAction_AddMobGroup*>(pControlScriptNode);
	if (pAction)
	{
		m_spawnGroupId = pAction->m_spawnGroupId;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;
		m_bRespawn = pAction->m_bRespawn;
		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;
		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_byImmortalMode = pAction->m_byImmortalMode;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Calculate", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_AddMobGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD spawnCount = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupCount(m_spawnGroupId);

	sSPAWN_TBLDAT* spawnTbldat = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupFirst(m_spawnGroupId);

	for (DWORD i = 0; i < spawnCount; i++)
	{
		if (spawnTbldat)
		{
			for (int ii = 0; ii < spawnTbldat->bySpawn_Quantity; ii++)
			{
				sMOB_DATA data;
				InitMobData(data);

				data.spawnGroupId = m_spawnGroupId;
				data.worldID = GetOwner()->GetWorld()->GetID();
				data.worldtblidx = GetOwner()->GetWorld()->GetIdx();
				data.tblidx = spawnTbldat->mob_Tblidx;
				spawnTbldat->vSpawn_Loc.CopyTo(data.vCurLoc);
				spawnTbldat->vSpawn_Loc.CopyTo(data.vSpawnLoc);
				spawnTbldat->vSpawn_Dir.CopyTo(data.vCurDir);
				spawnTbldat->vSpawn_Dir.CopyTo(data.vSpawnDir);
				data.bySpawnFuncFlag = m_bySpawnFuncFlag;
				data.sScriptData.playScript = m_dwPlayScriptNumber;
				data.sScriptData.playScriptScene = m_dwPlayScriptScene;
				data.actionpatternTblIdx = spawnTbldat->actionPatternTblidx;
				data.bySpawnRange = spawnTbldat->bySpawn_Loc_Range;
				data.wSpawnTime = spawnTbldat->wSpawn_Cool_Time;
				data.byMoveType = spawnTbldat->bySpawn_Move_Type;
				data.byWanderRange = spawnTbldat->byWander_Range;
				data.byMoveRange = spawnTbldat->byMove_Range;
				data.pathTblidx = spawnTbldat->path_Table_Index;
				data.byImmortalMode = m_byImmortalMode;

				sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(data.tblidx);
				if (pTbldat)
				{
					CMonster* pMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
					if (pMob)
					{
						if (pMob->CreateDataAndSpawn(data, pTbldat))
						{
							pMob->AddScript(GetOwner());	//add script
							pMob->SetStandAlone(false);
							GetOwner()->AddSpawnGroupMob(m_spawnGroupId, pMob->GetID());
							m_status = COMPLETED;
						}
						else
						{
							m_status = FAILED;
							break;
						}
					}
					else
					{
						m_status = FAILED;
						break;
					}
				}
				else
				{
					ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u Grouptblidx %u", data.tblidx, m_spawnGroupId);
				}
			}
		}

		spawnTbldat = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupNext(m_spawnGroupId);
	}

	return m_status;
}
