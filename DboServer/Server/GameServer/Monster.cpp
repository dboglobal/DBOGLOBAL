#include "stdafx.h"
#include "Monster.h"
#include "GameServer.h"
#include "NtlRandom.h"
#include "ObjectManager.h"
#include "RangeCheck.h"
#include "TableContainerManager.h"
#include "CPlayer.h"
#include "ItemManager.h"
#include "ItemDrop.h"
#include "GameMain.h"
#include "WpsAlgoObject.h"
#include "NtlPacketGU.h"
#include "DynamicFieldSystemEvent.h"
#include "DragonballHunt.h"
#include "TimeQuest.h"
#include "SystemEffectTable.h"
#include "SummonPet.h"
#include "HoneyBeeEvent.h"

#include <queue>


CMonster::CMonster()
	:CNpc(OBJTYPE_MOB)
{
	this->Initialize();
}

CMonster::~CMonster()
{
	if (!m_bIsDeleted)
		this->Destroy();
}

void CMonster::Initialize()
{
	m_SpawnGroupID = INVALID_SPAWNGROUPID;
	ResetDragonball();

	m_byKillerLevel = 200;
	m_hKillerCharID = INVALID_CHARACTERID;

	m_faintBuffIndex = INVALID_TBLIDX;
	m_byFaintBuffApplyType = INVALID_BYTE;
	m_fFaintBuffRange = INVALID_FLOAT;

	m_dropItem_ProbabilityTblidx = INVALID_TBLIDX;
}

void CMonster::Destroy()
{
}

void CMonster::CopyToObjectInfo(sOBJECT_INFO * pObjectInfo, CHARACTERID playerCharID)
{
	pObjectInfo->objType = GetObjType();

	m_pStateManager->CopyTo(&pObjectInfo->mobState);

	pObjectInfo->mobBrief.actionpatternTblIdx = GetActionPattern()->GetTableIdx();

	pObjectInfo->mobBrief.byActualLevel = GetLevel();
	pObjectInfo->mobBrief.byBallType = (playerCharID == m_hKillerCharID) ? m_byBallType : DRAGON_BALL_TYPE_NONE;

	pObjectInfo->mobBrief.bySizeRate = m_bySizeRate;
	pObjectInfo->mobBrief.curLp = GetCurLP();
	pObjectInfo->mobBrief.fLastFlyingAccelSpeed = GetFlyAccelSpeed();
	pObjectInfo->mobBrief.fLastFlyingDashSpeed = GetFlyDashSpeed();
	pObjectInfo->mobBrief.fLastFlyingSpeed = GetFlySpeed();
	pObjectInfo->mobBrief.fLastRunningSpeed = GetRunSpeed();
	pObjectInfo->mobBrief.fLastWalkingSpeed = GetWalkingSpeed();
	pObjectInfo->mobBrief.fSkillAnimationSpeedModifier = GetSkillAnimationSpeedModifier();
	pObjectInfo->mobBrief.maxLp = GetMaxLP();
	pObjectInfo->mobBrief.nicknameTblidx = INVALID_TBLIDX;
	pObjectInfo->mobBrief.tblidx = GetTblidx();
	pObjectInfo->mobBrief.wAttackSpeedRate = GetAttackSpeedRate();
	pObjectInfo->mobBrief.wCurEP = GetCurEP();
	pObjectInfo->mobBrief.wMaxEP = GetMaxEP();
}


bool CMonster::CreateDataAndSpawn(WORLDID worldId, sMOB_TBLDAT* mobTbldat, sSPAWN_TBLDAT* spawnTbldat, bool bSpawnOnServerStart/* = false*/, BYTE bySpawnFuncFlag/* = SPAWN_FUNC_FLAG_RESPAWN*/)
{
	SetWorldID(worldId);

	m_pTbldat = mobTbldat;

	m_SpawnGroupID = spawnTbldat->spawnGroupId;

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	sTempState.sCharStateBase.dwConditionFlag = 0;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	spawnTbldat->vSpawn_Dir.CopyTo(sTempState.sCharStateBase.vCurDir);
	spawnTbldat->vSpawn_Loc.CopyTo(sTempState.sCharStateBase.vCurLoc);
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	bot_profile.nicknameTblidx = INVALID_TBLIDX;
	SetPartyID(spawnTbldat->dwParty_Index);
	SetTblidx(mobTbldat->tblidx);

	m_dwAi_Bit_Flag = mobTbldat->dwAi_Bit_Flag;

	SetLevel(mobTbldat->byLevel);
	SetEffectiveLevel(mobTbldat->byLevel);
	m_bySizeRate = 10;

	bot_profile.sBotSubData.Init();
	bot_profile.bPartyLeader = spawnTbldat->bParty_Leader;
	bot_profile.byMoveRange = spawnTbldat->byMove_Range;
	bot_profile.byMoveType = spawnTbldat->bySpawn_Move_Type;
	bot_profile.bySpawnFuncFlag = bySpawnFuncFlag; //eSPAWN_FUNC_FLAG
	bot_profile.bySpawnRange = spawnTbldat->bySpawn_Loc_Range;
	bot_profile.byWanderRange = spawnTbldat->byWander_Range;
	bot_profile.pathTblidx = spawnTbldat->path_Table_Index;
	bot_profile.sScriptData.byActualLevel = mobTbldat->byLevel;
	bot_profile.sScriptData.byCreatorType = INVALID_BYTE;
	bot_profile.sScriptData.byEffectiveLevel = 0;
	bot_profile.sScriptData.byMobGrade = mobTbldat->byGrade;
	bot_profile.sScriptData.dwBattleScriptScene = 0;
	bot_profile.sScriptData.playScript = spawnTbldat->playScript;
	bot_profile.sScriptData.playScriptScene = spawnTbldat->playScriptScene;
	bot_profile.sScriptData.sBotAiIndex.tblidx = 0;
	bot_profile.sScriptData.sBotAiIndex.bySubIndex = 0;
	bot_profile.sScriptData.tblidxAiScript = spawnTbldat->aiScript;
	bot_profile.sScriptData.tblidxAiScriptScene = spawnTbldat->aiScriptScene;
	bot_profile.sScriptData.tblidxBattleScript = INVALID_TBLIDX;
	//bot_profile.sScriptData.wMobClass 

	bot_profile.vSpawnDir = spawnTbldat->vSpawn_Dir;
	bot_profile.vSpawnLoc = spawnTbldat->vSpawn_Loc;

	bot_profile.wSpawnTime = spawnTbldat->wSpawn_Cool_Time;
	bot_profile.dwParty_Index = spawnTbldat->dwParty_Index;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(mobTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]", mobTbldat->szModel);
		return false;
	}

	m_fScale = mobTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f Mob Tblidx %u", mobTbldat->fScale, mobTbldat->tblidx);
		return false;
	}

	if (!RefreshActionPatternData(spawnTbldat->actionPatternTblidx))
	{
		ERR_LOG(LOG_SYSTEM, "Couldnt refresh action pattern data. Mob tblidx %u action pattern tblidx %u", mobTbldat->tblidx, spawnTbldat->actionPatternTblidx);
		return false;
	}

	m_pSpeechGroup = g_pTableContainer->GetNpcSpeechTable()->FindSpeechGroup(mobTbldat->dwDialogGroup);

	if (CNpc::Create(&sTempState))
	{
		LoadSkillTable(bot_profile.sBotSubData.tblidxOnlyOneSkillUse);

		GetCharAtt()->CalculateAll();
		Spawn(bSpawnOnServerStart);
		return true;
	}

	return false;
}


bool CMonster::CreateDataAndSpawn(sMOB_DATA& sData, sMOB_TBLDAT* mobTbldat, BYTE byActualLevel/* = INVALID_BYTE*/, BYTE byEffectLevel/* = INVALID_BYTE*/)
{
	SetWorldID(sData.worldID);

	m_pTbldat = mobTbldat;

	m_SpawnGroupID = sData.spawnGroupId;

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	sTempState.sCharStateBase.dwConditionFlag = sData.qwCharConditionFlag;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	sTempState.sCharStateBase.vCurDir = sData.vSpawnDir;
	sTempState.sCharStateBase.vCurLoc = sData.vSpawnLoc;
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	bot_profile.nicknameTblidx = INVALID_TBLIDX;
	SetPartyID(sData.partyID);
	SetTblidx(mobTbldat->tblidx);

	m_dwAi_Bit_Flag = mobTbldat->dwAi_Bit_Flag;

	m_byImmortalMode = sData.byImmortalMode;

	if (byActualLevel == INVALID_BYTE)
		SetLevel(mobTbldat->byLevel);
	else
		SetLevel(byActualLevel);

	if (byEffectLevel == INVALID_BYTE || byEffectLevel < mobTbldat->byLevel)
		SetEffectiveLevel(mobTbldat->byLevel);
	else
		SetEffectiveLevel(byEffectLevel);


	m_bySizeRate = 10;

	bot_profile.bPartyLeader = sData.bPartyLeader;
	bot_profile.byMoveRange = sData.byMoveRange;
	bot_profile.byMoveType = sData.byMoveType;
	bot_profile.bySpawnFuncFlag = sData.bySpawnFuncFlag;
	bot_profile.bySpawnRange = sData.bySpawnRange;
	bot_profile.byWanderRange = sData.byWanderRange;
	bot_profile.pathTblidx = sData.pathTblidx;

	memcpy(&bot_profile.sScriptData, &sData.sScriptData, sizeof(sBOT_SERVER_SCRIPT_DATA));
	memcpy(&bot_profile.sBotSubData, &sData.sBotSubData, sizeof(sBOT_SUB_DATA));

	bot_profile.vSpawnDir.operator=(sData.vSpawnDir);
	bot_profile.vSpawnLoc.operator=(sData.vSpawnLoc);

	bot_profile.wSpawnTime = sData.wSpawnTime;

	bot_profile.vPartyLeaderDistance = sData.vPartyLeaderDistance;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(mobTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]", mobTbldat->szModel);
		return false;
	}

	m_fScale = mobTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f MOB Tblidx %u", mobTbldat->fScale, mobTbldat->tblidx);
		return false;
	}

	if (!RefreshActionPatternData(sData.actionpatternTblIdx))
	{
		ERR_LOG(LOG_SYSTEM, "Couldnt refresh action pattern data. MOB tblidx %u action pattern tblidx %u", mobTbldat->tblidx, sData.actionpatternTblIdx);
		return false;
	}

	m_pSpeechGroup = g_pTableContainer->GetNpcSpeechTable()->FindSpeechGroup(mobTbldat->dwDialogGroup);

	if (Create(&sTempState))
	{
		LoadSkillTable(bot_profile.sBotSubData.tblidxOnlyOneSkillUse);

		GetCharAtt()->CalculateAll();
		Spawn(false);
		return true;
	}

	return false;
}

bool CMonster::DoTransformation(TBLIDX tblidx, ACTIONPATTERNTBLIDX actionPatternTblidx, TBLIDX tblidxOnlyOneSkillUse)
{
	sMOB_TBLDAT* pMobTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(tblidx);
	if (pMobTbldat == NULL)
	{
		ERR_LOG(LOG_BOTAI, "Can not find monster table. Tblidx %u", tblidx);
		return false;
	}

	sMODELTOOL_CHAR_TBLDAT* pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(pMobTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]. Tblidx %u", pMobTbldat->szModel, pMobTbldat->tblidx);
		return false;
	}

	m_fScale = pMobTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f NPC Tblidx %u", pMobTbldat->fScale, pMobTbldat->tblidx);
		return false;
	}

	m_pAniTbldat = pAniTbldat;
	m_pTbldat = pMobTbldat;

	SetTblidx(pMobTbldat->tblidx);
	m_dwAi_Bit_Flag = pMobTbldat->dwAi_Bit_Flag;
	SetLevel(pMobTbldat->byLevel);
	SetEffectiveLevel(pMobTbldat->byLevel);

	bot_profile.sBotSubData.tblidxOnlyOneSkillUse = tblidxOnlyOneSkillUse;
	LoadSkillTable(bot_profile.sBotSubData.tblidxOnlyOneSkillUse);

	CNtlPacket packet(sizeof(sGU_MONSTER_TRANSFORMED_NFY));
	sGU_MONSTER_TRANSFORMED_NFY * res = (sGU_MONSTER_TRANSFORMED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MONSTER_TRANSFORMED_NFY;
	res->hSubject = GetID();
	res->newTblidx = tblidx;
	packet.SetPacketLen(sizeof(sGU_MONSTER_TRANSFORMED_NFY));
	Broadcast(&packet);

	return true;
}


//--------------------------------------------------------------------------------------//
//		SPAWN MOB
//--------------------------------------------------------------------------------------//
void CMonster::Spawn(bool bSpawnOnServerStart)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sMOB_TBLDAT* tbldat = GetTbldat();

	SetCurLP(GetCharAtt()->GetMaxLP());
	SetExp(tbldat->dwExp);
	SetZeni(tbldat->dwDrop_Zenny);
	SetCurEP(GetCharAtt()->GetMaxEP());
	SetRunSpeed(tbldat->fRun_Speed);

	m_vecFirstBattleLoc =(GetEnterLoc());
	m_vecFirstBattleDir =(GetEnterDir());

	if (!bSpawnOnServerStart)
	{
		sCHARSTATE sTempState;
		sTempState.sCharStateBase.bFightMode = false;
		sTempState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
		sTempState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = false;
		sTempState.sCharStateDetail.sCharStateSpawning.bIsFaint = false;
		sTempState.sCharStateDetail.sCharStateSpawning.byTeleportType = TELEPORT_TYPE_DEFAULT;
		sTempState.sCharStateBase.dwConditionFlag = 0;
		sTempState.sCharStateBase.dwStateTime = 0;
		sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
		bot_profile.vSpawnLoc.CopyTo(sTempState.sCharStateBase.vCurLoc);
		bot_profile.vSpawnDir.CopyTo(sTempState.sCharStateBase.vCurDir);
		sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

		m_pStateManager->CopyFrom(&sTempState);

		SetMoveDirection(NTL_MOVE_NONE);
		SetMoveFlag(NTL_MOVE_FLAG_INVALID);

		// if db hunt on and can drop db then ...
		if (g_pDragonballHuntEvent->IsOn() && GetTbldat()->bIsDragonBallDrop)
		{
			if (m_byKillerLevel <= GetLevel() + DBO_DRAGONBALL_EVENT_DROP_LEVEL_DIFF
				&& m_byKillerLevel >= GetLevel() - DBO_DRAGONBALL_EVENT_DROP_LEVEL_DIFF) //check lvl difference with killer.. So people cant keep killing low levels until a mob with db spawns
			{
				if (Dbo_CheckProbability(20))
				{
					m_byBallType = DRAGON_BALL_TYPE_EVENT;
					m_bHasDragonball = true;

					g_pDragonballHuntEvent->InsertMob(GetID());
				}
			}
		}
	}

	SetInitialized(true);
	m_bIsInRespawn = false;

	GetBotController()->ChangeControlState_Enter();
	app->GetGameMain()->GetWorldManager()->EnterObject(this, m_worldID, bSpawnOnServerStart);
	GetBotController()->SetControlBlock(false);
}

void CMonster::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	if (!IsInitialized())
		return;

	CCharacter::TickProcess(dwTickDiff, fMultiple);
}


HOBJECT CMonster::ConsiderScanTarget(WORD wRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	WORD wScanRange = wRange;
	if (wScanRange == INVALID_WORD)
		wScanRange = GetTbldat()->wScan_Range;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	int nLoopCount = 0;
	int nLoopCount2 = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
			while(pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if(IsTargetAttackble(pTarget, wScanRange))
				{
					if (IsTargetAttackState(pTarget))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}

			CNpc* pNpcTarget = (CNpc*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_NPC);
			while (pNpcTarget && pNpcTarget->IsInitialized())
			{
				++nLoopCount2;
				if (nLoopCount2 > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (pNpcTarget->HasFunction(NPC_FUNC_FLAG_SCAN_BY_MOB))
				{
					if (IsTargetAttackble(pNpcTarget, wScanRange))
					{
						if (IsTargetAttackState(pNpcTarget))
						{
							if (CanSeeOnLineOfSight(pNpcTarget))
							{
								pRangeCheck.AddBot(pNpcTarget);
							}
						}
					}
				}

				pNpcTarget = (CNpc*)pWorldCellSibling->GetObjectList()->GetNext(pNpcTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CMonster::Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason)
{
	if (CCharacterObject::Faint(pkKiller, byReason))
	{
		if (pkKiller && GetCurWorld() && (pkKiller->IsPC() || pkKiller->IsSummonPet()))
		{
			CPlayer* pKiller = (pkKiller->IsSummonPet() == false) ? (CPlayer*)pkKiller : ((CSummonPet*)pkKiller)->GetOwner();
			
			if (pKiller->GetLevel() > DBO_DRAGONBALL_EVENT_DROP_LEVEL_DIFF)
			{
				m_byKillerLevel = pKiller->GetLevel();
			}

			m_hKillerCharID = pKiller->GetCharID();

			CreateKillReward(GetCurWorld()->GetRuleType() != GAMERULE_CCBATTLEDUNGEON);
			g_pDynamicFieldSystemEvent->Update(this, pKiller);
			g_pHoneyBeeEvent->Update(this, pKiller);

			FaintBuffReward(pKiller);

			if (pKiller->GetTMQ())
			{
				pKiller->GetTMQ()->UpdateTmqPoints();
			}
		}

		if (m_bHasDragonball)
		{
			ResetDragonball();
			g_pDragonballHuntEvent->EraseMob(GetID());
		}

		/*Info:
			When we remove script from monster which has respawn then the monster will exist for ever after death 
		*/
		if (BIT_FLAG_TEST(GetSpawnFuncFlag(), SPAWN_FUNC_FLAG_RESPAWN) == false) //Check if can respawn
		{
			RemAllScript();
		}

		CCharacter::Faint(pkKiller, byReason);

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
DWORD CalcExp(CMonster* victim, CPlayer* player, DWORD dw_Exp)
{
	//// level difference calculation
	int l_LevelGap = abs(player->GetLevel() - victim->GetLevel());

	if (l_LevelGap >= 20)
	{
		dw_Exp = 1;
	}
	else if (l_LevelGap >= 15)
	{
		dw_Exp = DWORD((float)dw_Exp*0.1f);
	}
	else if (l_LevelGap >= 10)
	{
		dw_Exp = DWORD((float)dw_Exp*0.25f);
	}
	else if (l_LevelGap >= 5)
	{
		dw_Exp = DWORD((float)dw_Exp*0.5f);
	}
	//////////////////////////////////////////////////////////////////////////

	if (dw_Exp == 0) dw_Exp = 1;

	return dw_Exp;
}

namespace NPartyExpDistribute
{
	struct FPartyTotaler
	{
		int			member_count;
		CNtlVector	vPos;
		WORLDID		worldId;

		FPartyTotaler(CMonster* center)
			: member_count(0)
		{
			worldId = center->GetWorldID();
			vPos = center->GetCurLoc();
		};

		void operator () (CPlayer* ch)
		{
			if (worldId == ch->GetWorldID() && ch->IsInRange(vPos, 100.f)) //do not add ch->IsReceiveExpDisabled() == false check here to avoid player power lvling others while disable exp
			{
				++member_count;
			}
		}
	};

	struct FPartyDistributor
	{
		CMonster*	c; //defeated target
		CNtlVector	vPos;
		WORLDID		worldId;
		DWORD		_iExp;
		int			m_iMemberCount;

		FPartyDistributor(CMonster* center, int member_count, DWORD iExp)
			: c(center), _iExp(iExp), m_iMemberCount(member_count)
		{
			vPos = center->GetCurLoc();
			worldId = center->GetWorldID();
			if (m_iMemberCount == 0)
				m_iMemberCount = 1;
		};

		void operator () (CPlayer* ch)
		{
			if (worldId == ch->GetWorldID() && ch->IsInRange(vPos, 100.f) && ch->IsReceiveExpDisabled() == false)
			{
				DWORD iExp2 = CalcExp(c, ch, (DWORD)_iExp / m_iMemberCount);

				ch->UpdateExp(iExp2, true);
			}
		}
	};
}

typedef struct SDamageInfo
{
	DWORD dwDam;
	CPlayer* pAttacker;
	CParty* pParty;

	void Clear()
	{
		pAttacker = NULL;
		pParty = NULL;
	}

	inline void Distribute(CMonster* ch, DWORD dwExp) //ch = monster
	{
		CGameServer* app = (CGameServer*)g_pApp;

		if (pAttacker)
		{
			if (pAttacker->IsReceiveExpDisabled())
				return;

			dwExp += dwExp * app->GetSoloExpRate() / 100;
			pAttacker->UpdateExp(CalcExp(ch, pAttacker, dwExp), true);
		}
		else if (pParty)
		{
			NPartyExpDistribute::FPartyTotaler f(ch);
			pParty->ForEachOnlineMember(f);

			if(f.member_count > 1)	//only add party bonus if at least 2 member inside range
				dwExp += dwExp * app->GetPartyExpRate() / 100;
			else
				dwExp += dwExp * app->GetSoloExpRate() / 100;

			NPartyExpDistribute::FPartyDistributor fDist(ch, f.member_count, dwExp);
			pParty->ForEachOnlineMember(fDist);

		}
	}
} TDamageInfo;

CPlayer* CMonster::DistributeExp()
{
	DWORD dwExpToDistribute = GetExp();

	if (GetCurWorld()->GetRuleType() == GAMERULE_CCBATTLEDUNGEON)
		dwExpToDistribute = 0;

	DWORD	dwTotalDam = 0;
	CPlayer* pkChrMostAttacked = NULL;
	DWORD	dwMostDam = 0;

	typedef std::vector<TDamageInfo> TDamageInfoTable;
	TDamageInfoTable damage_info_table;
	std::map<CParty*, TDamageInfo> map_party_damage;

	damage_info_table.reserve(GetTargetListManager()->GetAggroCount());
	CTargetListManager::AGGROPOINT_MAP::iterator it = GetTargetListManager()->AggroBegin();

	int nLoopCount = 0;

	// Filter out people which are more far than 50m
	while (it != GetTargetListManager()->AggroEnd())
	{
		++nLoopCount;
		if (nLoopCount > 5000)
		{
			ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
		}

		const HOBJECT hPlayer = it->first;
		DWORD dwDam = it->second.dwTotalDamage;

		++it;

		CCharacter* pAttacker = g_pObjectManager->GetChar(hPlayer); //get char
		if (!pAttacker || IsInRange(pAttacker, 50.0f) == false)
			continue;

		if (pAttacker->IsPC() == false && pAttacker->IsSummonPet() == false)
			continue;

		CPlayer* pAttackerPC = (pAttacker->IsPC()) ? (CPlayer*)pAttacker : ((CSummonPet*)pAttacker)->GetOwner();
		if (pAttackerPC == NULL)
			continue;

		dwTotalDam += dwDam;
		if (!pkChrMostAttacked || dwDam > dwMostDam)
		{
			pkChrMostAttacked = pAttackerPC;
			dwMostDam = dwDam;
		}

		if (pAttackerPC && pAttackerPC->GetParty())
		{
			std::map<CParty*, TDamageInfo>::iterator it2 = map_party_damage.find(pAttackerPC->GetParty());
			if (it2 == map_party_damage.end())
			{
				TDamageInfo di;
				di.dwDam = dwDam;
				di.pAttacker = NULL;
				di.pParty = pAttackerPC->GetParty();
				map_party_damage.insert(std::make_pair(di.pParty, di));
			}
			else
			{
				it2->second.dwDam += dwDam;
			}
		}
		else
		{
			TDamageInfo di;

			di.dwDam = dwDam;
			di.pAttacker = pAttackerPC;
			di.pParty = NULL;

			damage_info_table.push_back(di);
		}
	}

	for (std::map<CParty*, TDamageInfo>::iterator it3 = map_party_damage.begin(); it3 != map_party_damage.end(); ++it3)
	{
		damage_info_table.push_back(it3->second);
	}

	//printf("reward: total damage %d \n", dwTotalDam);
	if (dwTotalDam == 0)
		return NULL;

	if (damage_info_table.empty())
		return NULL;

	if (dwExpToDistribute > 0)
	{
		TDamageInfoTable::iterator it4;
		for (it4 = damage_info_table.begin(); it4 != damage_info_table.end(); ++it4)
		{
			TDamageInfo & di = *it4;

			float fPercent = (float)di.dwDam / (float)dwTotalDam;

			if (fPercent > 1.0f)
				fPercent = 1.0f;

			di.Distribute(this, (DWORD)((float)dwExpToDistribute * fPercent));
		}
	}

	return pkChrMostAttacked;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CMonster::CreateKillReward(bool bItemDrop)
{
	CPlayer* pkAttacker = DistributeExp(); //give exp to everyone. Return person who dealt most dmg
	if (!pkAttacker || !pkAttacker->IsInitialized())
		return;

	if (bItemDrop && pkAttacker->IsPC())
	{
		bool bDynamicWorld = GetCurWorld()->GetTbldat()->bDynamic;
		//update quest 
		if (pkAttacker->GetParty())//update quest for all party members
		{
			for (int i = 0; i < pkAttacker->GetParty()->GetPartyMemberCount(); i++)
			{
				CPlayer* partymember = g_pObjectManager->GetPC(pkAttacker->GetParty()->GetMemberInfo(i).hHandle);
				if (partymember && (bDynamicWorld || partymember->IsInRange(pkAttacker, NTL_MAX_RADIUS_OF_VISIBLE_AREA)))
				{
					partymember->GetQuests()->UpdateQuestEvt(this->GetTblidx());
				}
			}
		}
		else pkAttacker->GetQuests()->UpdateQuestEvt(this->GetTblidx()); //if player not inside party then update only for myself
	}

	if (GetCurWorld() && GetCurWorld()->GetTbldat()->bDynamic) //check if inside dynamic world
	{
		if (BIT_FLAG_TEST(SPAWN_FUNC_FLAG_RESPAWN, GetSpawnFuncFlag())) //check if has respawn
			return;
	}

	if (bItemDrop || m_dropItem_ProbabilityTblidx != INVALID_TBLIDX)
	{
		sVECTOR3 pos;
		GetCurLoc().CopyTo(pos);
		//TO-DO: check if its possible to move at this location

		CItemDrop* item = NULL;
		std::vector<CItemDrop*> s_vec_item;

		if (m_dropItem_ProbabilityTblidx != INVALID_TBLIDX)
			g_pItemManager->CreateItemDrop(m_dropItem_ProbabilityTblidx, s_vec_item);

		if (bItemDrop)
			g_pItemManager->CreateItemDrop(this, pkAttacker, s_vec_item);

		if (s_vec_item.size() > 0)
		{
			if (s_vec_item.size() == 0);
			else if (s_vec_item.size() == 1)
			{
				item = s_vec_item[0];

				pos.x = GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
				pos.y = GetCurLoc().y;
				pos.z = GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

				item->SetOwnership(pkAttacker->GetID(), pkAttacker->GetPartyID());
				item->StartDestroyEvent();
				item->AddToGround(GetWorldID(), pos);
			}
			else
			{
				int iItemIdx = (int)(s_vec_item.size() - 1);

				std::priority_queue<std::pair<DWORD, CPlayer*> > pq;

				DWORD total_dam = 0;

				for (CTargetListManager::AGGROPOINT_MAP::iterator it = GetTargetListManager()->AggroBegin(); it != GetTargetListManager()->AggroEnd(); it++)
				{
					DWORD dwDamage = it->second.dwTotalDamage;
					if (dwDamage > 0)
					{
						CPlayer* ch = g_pObjectManager->GetPC(it->first);

						if (ch)
						{
							pq.push(std::make_pair(dwDamage, ch));
							total_dam += dwDamage;
						}
					}
				}

				std::vector<CPlayer*> v;

				while (!pq.empty() && pq.top().first * 10 >= total_dam)
				{
					v.push_back(pq.top().second);
					pq.pop();
				}

				if (v.empty()) //only when one-hit kill
				{

					while (iItemIdx >= 0)
					{
						item = s_vec_item[iItemIdx--];

						if (!item)
						{
							ERR_LOG(LOG_GENERAL, "item null in vector idx %d \n", iItemIdx + 1);
							continue;
						}

						// 10% less damage who gave each other no ownership
						pos.x = GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
						pos.y = GetCurLoc().y;
						pos.z = GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

						item->SetOwnership(pkAttacker->GetID(), pkAttacker->GetPartyID());
						item->StartDestroyEvent();
						item->AddToGround(GetWorldID(), pos);
					//	printf("add item to ground. World %u WorldTblidx %u Pos %f %f %f \n", item->GetWorldID(), item->GetWorldTblidx(), item->GetCurLoc().x, item->GetCurLoc().y, item->GetCurLoc().z);
					}
				}
				else //many different people attacked the monster..
				{
					std::vector<CPlayer*>::iterator it = v.begin();

					while (iItemIdx >= 0)
					{
						item = s_vec_item[iItemIdx--];

						if (!item)
						{
							ERR_LOG(LOG_GENERAL, "item null in vector idx %d \n", iItemIdx + 1);
							continue;
						}

						CPlayer* ch = *it++;

						if (it == v.end())
							it = v.begin();

						pos.x = GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
						pos.y = GetCurLoc().y;
						pos.z = GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

						item->SetOwnership(pkAttacker->GetID(), pkAttacker->GetPartyID());
						item->StartDestroyEvent();
						item->AddToGround(GetWorldID(), pos);
					//	printf("add item to ground. World %u WorldTblidx %u Pos %f %f %f \n", item->GetWorldID(), item->GetWorldTblidx(), item->GetCurLoc().x, item->GetCurLoc().y, item->GetCurLoc().z);
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
HOBJECT CMonster::ConsiderScanTargetRingRange(WORD wRingRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	WORD wRange = GetTbldat()->wSight_Range - wRingRange;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	int nLoopCount = 0;
	int nLoopCount2 = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (IsTargetAttackble(pTarget, GetTbldat()->wSight_Range))
				{
					if (!IsTargetAttackble(pTarget, wRange))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}

			CNpc* pNpcTarget = (CNpc*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_NPC);
			while (pNpcTarget && pNpcTarget->IsInitialized())
			{
				++nLoopCount2;
				if (nLoopCount2 > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (pNpcTarget->HasFunction(NPC_FUNC_FLAG_SCAN_BY_MOB))
				{
					if (IsTargetAttackble(pNpcTarget, GetTbldat()->wSight_Range))
					{
						if (!IsTargetAttackble(pNpcTarget, wRange))
						{
							if (CanSeeOnLineOfSight(pNpcTarget))
							{
								pRangeCheck.AddBot(pNpcTarget);
							}
						}
					}
				}

				pNpcTarget = (CNpc*)pWorldCellSibling->GetObjectList()->GetNext(pNpcTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}

void CMonster::ResetDragonball()
{
	m_byBallType = DRAGON_BALL_TYPE_NONE;
	m_bHasDragonball = false;;
}

void CMonster::SetFaintBuff(TBLIDX faintBuffIndex, BYTE byFaintBuffApplyType, float fFaintBuffRange)
{
	m_faintBuffIndex = faintBuffIndex;
	m_byFaintBuffApplyType = byFaintBuffApplyType;
	m_fFaintBuffRange = fFaintBuffRange;
}

void CMonster::SetDropItemProbabilityTblidx(TBLIDX dropItem_ProbabilityTblidx)
{
	m_dropItem_ProbabilityTblidx = dropItem_ProbabilityTblidx;
}

void CMonster::FaintBuffReward(CPlayer* pPlayer)
{
	if (m_faintBuffIndex == INVALID_TBLIDX)
		return;

	if (pPlayer->IsInitialized())
	{
		sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_faintBuffIndex);
		if (pSkillTbldat == NULL)
		{
			ERR_LOG(LOG_GENERAL, "Faint buff tblidx %u not found.", m_faintBuffIndex);
			return;
		}

		sDBO_BUFF_PARAMETER aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
		eSYSTEM_EFFECT_CODE aeEffectCode[NTL_MAX_EFFECT_IN_SKILL];

		for (int i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
		{
			aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DEFAULT;
			aBuffParameter[i].buffParameter.fParameter = (float)(pSkillTbldat->aSkill_Effect_Value[i]);
			aBuffParameter[i].buffParameter.dwRemainValue = (DWORD)pSkillTbldat->aSkill_Effect_Value[i];

			aeEffectCode[i] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pSkillTbldat->skill_Effect[i]);

			if (aeEffectCode[i] == ACTIVE_HEAL_OVER_TIME || aeEffectCode[i] == ACTIVE_EP_OVER_TIME)
			{
				aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
				aBuffParameter[i].buffParameter.dwRemainTime = pSkillTbldat->dwKeepTimeInMilliSecs;
			}
			else if (aeEffectCode[i] == ACTIVE_BLEED || aeEffectCode[i] == ACTIVE_POISON || aeEffectCode[i] == ACTIVE_STOMACHACHE || aeEffectCode[i] == ACTIVE_BURN)
			{
				aBuffParameter[i].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_DOT;
				aBuffParameter[i].buffParameter.dwRemainTime = pSkillTbldat->dwKeepTimeInMilliSecs;
			}
		}

		//GIVE BUFF

		eBUFF_TYPE buffType = BUFF_TYPE_BLESS;
		if (Dbo_IsForCurseBuff(pSkillTbldat->bySkill_Active_Type))
			buffType = BUFF_TYPE_CURSE;

		DWORD dwKeepTime = pSkillTbldat->dwKeepTimeInMilliSecs;

		if (m_byFaintBuffApplyType == eMOB_FAINT_BUFF_SLAYER || pPlayer->GetPartyID() == INVALID_PARTYID) //if personal or dont have party
		{
			pPlayer->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, buffType, pSkillTbldat);
		}
		else
		{
			for (int i = 0; i < pPlayer->GetParty()->GetPartyMemberCount(); i++)
			{
				CPlayer* pMember = g_pObjectManager->GetPC(pPlayer->GetParty()->GetMemberInfo(i).hHandle);
				if (pMember && pMember->IsInitialized())
				{
					if (pPlayer->IsInRange(pMember, m_fFaintBuffRange))
					{
						pMember->GetBuffManager()->RegisterBuff(dwKeepTime, aeEffectCode, aBuffParameter, INVALID_HOBJECT, buffType, pSkillTbldat);
					}
				}
			}
		}
	}
}

bool CMonster::IsSightAngle(CCharacter * pTarget)
{
	if(pTarget == NULL)
		return false;

	if (IsPerceiveEnergy(pTarget))
	{
		return true;
	}

	if (pTarget->GetMeHaterManager()->GetContainerMeHater()->IsMeHater())
		return true;

	if (GetTbldat())
	{
		CNtlVector v1(GetCurDir());
		CNtlVector V2(pTarget->GetCurLoc() - GetCurLoc());
		V2.y = 0.0f;
		
		if (V2.SafeNormalize())
		{
			float fSightAngleRadian = GetDegreeToRadian(90 - (GetTbldat()->wSightAngle >> 1));

			return v1.Dot(V2) > fSightAngleRadian;
		}
	}

	return false;
}

