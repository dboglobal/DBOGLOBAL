#include "stdafx.h"
#include "ScriptAlgoAction_AddMobGroup.h"
#include "WPSNodeAction_AddMobGroup.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "MobTable.h"
#include "Monster.h"
#include "WPSNodeAction_MobList.h"
#include "WpsAlgoStage.h"
#include "NtlRandom.h"


CWpsAlgoAction_AddMobGroup::CWpsAlgoAction_AddMobGroup(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_ADDMOB_GROUP, "SCRIPTCONTROL_ACTION_ADDMOB_GROUP")
{
}


CWpsAlgoAction_AddMobGroup::~CWpsAlgoAction_AddMobGroup()
{
}


bool CWpsAlgoAction_AddMobGroup::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_AddMobGroup* pAction = dynamic_cast<CWPSNodeAction_AddMobGroup*>(pControlScriptNode);
	if (pAction)
	{
		m_spawnGroupId = pAction->m_spawnGroupId;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;
		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;
		m_dwAIScriptNumber = pAction->m_dwAIScriptNumber;
		m_dwAIScriptScene = pAction->m_dwAIScriptScene;
		m_qwCharConditionFlag = pAction->m_qwCharConditionFlag;
		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_nBindMobList = pAction->m_nBindMobList;
		m_mapBindMob = pAction->m_mapBindMob;
		m_dropItem_ProbabilityTblidx = pAction->m_dropItem_ProbabilityTblidx;
		m_bPartyLeader = pAction->m_bPartyLeader;
		m_partyId = pAction->m_partyId;
		m_bExecuterTargetFix = pAction->m_bExecuterTargetFix;
		m_byImmortalMode = pAction->m_byImmortalMode;
		m_eNestType = pAction->m_eNestType;
		m_byNestRange = pAction->m_byNestRange;
		m_faintBuffIndex = pAction->m_faintBuffIndex;
		m_byFaintBuffApplyType = pAction->m_byFaintBuffApplyType;
		m_fFaintBuffRange = pAction->m_fFaintBuffRange;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Calculate", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_AddMobGroup::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	DWORD spawnCount = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupCount(m_spawnGroupId);
	sSPAWN_TBLDAT* spawnTbldat = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupFirst(m_spawnGroupId);

	//NTL_PRINT(PRINT_APP, "spawnCount: %u, CCBD-STAGE %u, m_spawnGroupId %u, m_mapBindMob.size %I64u, m_nBindMobList", 
	//	spawnCount, GetOwner()->GetCCBDStage(), m_spawnGroupId, m_mapBindMob.size(), m_nBindMobList);

	if (m_nBindMobList == 0)
	{
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
					data.sScriptData.playScript = (m_dwPlayScriptNumber != INVALID_DWORD) ? m_dwPlayScriptNumber : spawnTbldat->playScript;
					data.sScriptData.playScriptScene = (m_dwPlayScriptScene != INVALID_DWORD) ? m_dwPlayScriptScene : spawnTbldat->playScriptScene;
					data.sScriptData.tblidxAiScript = (m_dwAIScriptNumber != INVALID_DWORD) ? m_dwAIScriptNumber : spawnTbldat->aiScript;
					data.sScriptData.tblidxAiScriptScene = (m_dwAIScriptScene != INVALID_DWORD) ? m_dwAIScriptScene : spawnTbldat->aiScriptScene;
					data.qwCharConditionFlag = m_qwCharConditionFlag;
					data.partyID = (m_partyId != INVALID_DWORD) ? m_partyId : spawnTbldat->dwParty_Index;
					data.bPartyLeader = (m_bPartyLeader != false) ? m_bPartyLeader : spawnTbldat->bParty_Leader;
					data.byImmortalMode = m_byImmortalMode;
					data.actionpatternTblIdx = spawnTbldat->actionPatternTblidx;
					data.bySpawnRange = spawnTbldat->bySpawn_Loc_Range;
					data.wSpawnTime = spawnTbldat->wSpawn_Cool_Time;
					data.byMoveType = spawnTbldat->bySpawn_Move_Type;
					data.byWanderRange = spawnTbldat->byWander_Range;
					data.byMoveRange = spawnTbldat->byMove_Range;
					data.pathTblidx = spawnTbldat->path_Table_Index;
					data.hTargetFixedExecuter = m_bExecuterTargetFix;
					data.sBotSubData.byNestRange = m_byNestRange;
					data.sBotSubData.byNestType = m_eNestType;

					sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(data.tblidx);
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
						m_status = FAILED;
						break;
					}
				}
			}

			spawnTbldat = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupNext(m_spawnGroupId);
		}
	}
	else
	{
		CWpsAlgoStage* pStage = dynamic_cast<CWpsAlgoStage*>(GetOwner()->GetController()->GetCurrentStage());
		if (pStage == NULL)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find current stage. CCBD-STAGE %u", GetOwner()->GetCCBDStage());
			m_status = FAILED;
			return m_status;
		}

		for (DWORD i = 0; i < spawnCount; i++)
		{
			if (spawnTbldat)
			{
				std::map<TBLIDX, sSSD_MOB_PAIR>::iterator it = m_mapBindMob.find(spawnTbldat->mob_Tblidx);
				if (it != m_mapBindMob.end())
				{
					sSSD_MOB_PAIR& mobPair = it->second;

					//find mob list
					CWPSNodeAction_MobList* pMobList = dynamic_cast<CWPSNodeAction_MobList*>(pStage->GetMobList(m_nBindMobList-1));
					if (pMobList == NULL)
					{
						ERR_LOG(LOG_SCRIPT, "Could not find CWPSNodeAction_MobList. CCBD-STAGE %u, m_nBindMobList %u", GetOwner()->GetCCBDStage(), m_nBindMobList);
						return m_status;
					}

					if (pMobList->m_aMobList[mobPair.pairIndex].size() > 0)
					{
						TBLIDX mobIdx = pMobList->m_aMobList[mobPair.pairIndex][RandomRange(0, (int)(pMobList->m_aMobList[mobPair.pairIndex].size() - 1))];

						sMOB_DATA data;
						InitMobData(data);

						data.spawnGroupId = m_spawnGroupId;
						data.worldID = GetOwner()->GetWorld()->GetID();
						data.worldtblidx = GetOwner()->GetWorld()->GetIdx();
						data.tblidx = mobIdx;
						spawnTbldat->vSpawn_Loc.CopyTo(data.vCurLoc);
						spawnTbldat->vSpawn_Loc.CopyTo(data.vSpawnLoc);
						spawnTbldat->vSpawn_Dir.CopyTo(data.vCurDir);
						spawnTbldat->vSpawn_Dir.CopyTo(data.vSpawnDir);
						data.bySpawnFuncFlag = m_bySpawnFuncFlag;
						data.sScriptData.playScript = m_dwPlayScriptNumber;
						data.sScriptData.playScriptScene = m_dwPlayScriptScene;
						data.sScriptData.tblidxAiScript = m_dwAIScriptNumber;
						data.sScriptData.tblidxAiScriptScene = m_dwAIScriptScene;
						data.qwCharConditionFlag = m_qwCharConditionFlag;
						data.partyID = m_partyId;
						data.bPartyLeader = m_bPartyLeader;
						data.byImmortalMode = m_byImmortalMode;
						data.actionpatternTblIdx = spawnTbldat->actionPatternTblidx;
						data.bySpawnRange = spawnTbldat->bySpawn_Loc_Range;
						data.wSpawnTime = 1;
						data.byMoveType = spawnTbldat->bySpawn_Move_Type;
						data.byWanderRange = spawnTbldat->byWander_Range;
						data.byMoveRange = spawnTbldat->byMove_Range;
						data.pathTblidx = spawnTbldat->path_Table_Index;
						data.hTargetFixedExecuter = m_bExecuterTargetFix;
						data.sBotSubData.byNestRange = m_byNestRange;
						data.sBotSubData.byNestType = m_eNestType;

						sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(data.tblidx);
						if (pTbldat)
						{
							CMonster* pMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
							if (pMob)
							{
								if (pMob->CreateDataAndSpawn(data, pTbldat, pMobList->m_byActualLevel, pMobList->m_byEffectiveLevel))
								{
									pMob->SetFaintBuff(m_faintBuffIndex, m_byFaintBuffApplyType, m_fFaintBuffRange);
									pMob->SetDropItemProbabilityTblidx(m_dropItem_ProbabilityTblidx);
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
							ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u Grouptblidx %u, m_nBindMobList %u, mobPair.pairIndex %u", data.tblidx, m_spawnGroupId, m_nBindMobList, mobPair.pairIndex);
						}

					}
					else printf("pMobList->m_aMobList[mobPair.pairIndex].size() == 0. ccbd stage %u, bindmoblist %u, pairIndex %u \n", GetOwner()->GetCCBDStage(), m_nBindMobList, mobPair.pairIndex);
				}
				else printf("could not find spawnTbldat->mob_Tblidx %u in bind mob list, m_spawnGroupId = %u, spawnTbldat->tblidx = %u \n", spawnTbldat->mob_Tblidx, m_spawnGroupId, spawnTbldat->tblidx);
			}
			else printf("spawnTbldat is null. m_spawnGroupId: %u \n", m_spawnGroupId);

			spawnTbldat = g_pTableContainer->GetMobSpawnTable(GetOwner()->GetWorld()->GetIdx())->GetSpawnGroupNext(m_spawnGroupId);
		}
	}


	return m_status;
}
