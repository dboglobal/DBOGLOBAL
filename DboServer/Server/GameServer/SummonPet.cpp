#include "stdafx.h"
#include "SummonPet.h"
#include "CPlayer.h"
#include "TableContainerManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "NtlPacketGU.h"


CSummonPet::CSummonPet()
	:CNpc(OBJTYPE_SUMMON_PET)
{
	Initialize();
}

CSummonPet::~CSummonPet()
{
	if (!m_bIsDeleted)
		Destroy();
}

void CSummonPet::CopyToObjectInfo(sOBJECT_INFO * pObjectInfo, CHARACTERID playerCharID)
{
	pObjectInfo->objType = GetObjType();

	m_pStateManager->CopyTo(&pObjectInfo->summonPetState);

	pObjectInfo->summonPetBrief.bySourceType = DBO_OBJECT_SOURCE_SKILL;
	pObjectInfo->summonPetBrief.sourceTblidx = m_sourceTblidx;
	pObjectInfo->summonPetBrief.npcTblidx = GetTblidx();
	pObjectInfo->summonPetBrief.bySizeRate = m_bySizeRate;
	pObjectInfo->summonPetBrief.hOwner = GetOwnerID();
	pObjectInfo->summonPetBrief.fLastRunningSpeed = GetRunSpeed();
	pObjectInfo->summonPetBrief.fLastAttackRange = CCharacterObject::GetAttackRange();
	pObjectInfo->summonPetBrief.fSkillAnimationSpeedModifier = GetSkillAnimationSpeedModifier();
	pObjectInfo->summonPetBrief.curLp = GetCurLP();
	pObjectInfo->summonPetBrief.wCurEP = GetCurEP();
	pObjectInfo->summonPetBrief.maxLp = GetMaxLP();
	pObjectInfo->summonPetBrief.wMaxEP = GetMaxEP();
	pObjectInfo->summonPetBrief.wAttackSpeedRate = GetAttackSpeedRate();

	if (CPlayer* pPlayer = GetOwner())
		pObjectInfo->summonPetBrief.bIsInFreePvpZone = pPlayer->IsPvpZone();
	else
		pObjectInfo->summonPetBrief.bIsInFreePvpZone = false;
}


void CSummonPet::Initialize()
{
	m_hOwner = INVALID_HOBJECT;
	m_hAttackTarget = INVALID_HOBJECT;
	m_sourceTblidx = INVALID_TBLIDX;
	SetToggleAttack(true);
}

void CSummonPet::Destroy()
{
	m_hOwner = INVALID_HOBJECT;
	m_sourceTblidx = INVALID_TBLIDX;
	m_hAttackTarget = INVALID_HOBJECT;
	SetToggleAttack(false);
}

bool CSummonPet::CreateDataAndSpawn(HOBJECT hOwner, TBLIDX sourceTblidx, sNPC_TBLDAT * npcTbldat, WORLDID worldId, CNtlVector & rCurLoc, CNtlVector & rCurDir)
{
	SetWorldID(worldId);

	m_hOwner = hOwner;
	m_pTbldat = npcTbldat;

	m_sourceTblidx = sourceTblidx;

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	sTempState.sCharStateBase.dwConditionFlag = 0;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	rCurLoc.CopyTo(sTempState.sCharStateBase.vCurLoc);
	rCurDir.CopyTo(sTempState.sCharStateBase.vCurDir);
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	bot_profile.nicknameTblidx = INVALID_TBLIDX;
	SetPartyID(INVALID_PARTYID);
	SetTblidx(npcTbldat->tblidx);

	m_dwAi_Bit_Flag = npcTbldat->dwAi_Bit_Flag;

	m_byImmortalMode = eIMMORTAL_MODE_OFF;

	SetLevel(npcTbldat->byLevel);
	SetEffectiveLevel(npcTbldat->byLevel);
	m_bySizeRate = 10;

	bot_profile.vSpawnLoc = rCurLoc;
	bot_profile.vSpawnDir = rCurDir;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(npcTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "SummonPet can't find Animation Data[%s]", npcTbldat->szModel);
		return false;
	}

	m_fScale = npcTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "SummonPet couldnt refresh object radius. Scale %f NPC Tblidx %u", npcTbldat->fScale, npcTbldat->tblidx);
		return false;
	}

	if (!RefreshActionPatternData(BOTAP_MOVE))
	{
		ERR_LOG(LOG_SYSTEM, "Couldnt refresh action pattern data. NPC tblidx %u action pattern tblidx %u", npcTbldat->tblidx, BOTAP_MOVE);
		return false;
	}

	if (Create(&sTempState))
	{
		LoadSkillTable(INVALID_TBLIDX);

		GetCharAtt()->CalculateAll();

		if (CPlayer* pOwner = GetOwner())
		{
			pOwner->SetCurrentPetId(GetID());

			if (pOwner->GetAttackProgress())
			{
				if (CCharacter* pChar = g_pObjectManager->GetChar(pOwner->GetTargetHandle()))
				{
					m_hAttackTarget = pOwner->GetTargetHandle();
				}
			}
		}

		return true;
	}

	return false;
}

void CSummonPet::Despawn(bool bSendPacket/* = false*/)
{
	CGameServer* app = (CGameServer*)g_pApp;

	app->GetGameMain()->GetWorldManager()->LeaveObject(this);

	if (CPlayer* pOwner = GetOwner())
	{
		if (pOwner->IsInitialized())
		{
			pOwner->SetCurrentPetId(INVALID_HOBJECT);

			if (bSendPacket)
			{
				CNtlPacket packetPet(sizeof(sGU_PET_DESPAWNED_NFY));
				sGU_PET_DESPAWNED_NFY * resPet = (sGU_PET_DESPAWNED_NFY *)packetPet.GetPacketData();
				resPet->wOpCode = GU_PET_DESPAWNED_NFY;
				resPet->byAvatarType = DBO_AVATAR_TYPE_SUMMON_PET_1;
				packetPet.SetPacketLen(sizeof(sGU_PET_DESPAWNED_NFY));
				pOwner->SendPacket(&packetPet);
			}
		}
	}

	SetInitialized(false);

	g_pObjectManager->DestroyCharacter(this);
}

void CSummonPet::CopySummonPetProfile(sSUMMON_PET_PROFILE * pProfile)
{
	memcpy(&pProfile->attribute, &GetCharAtt()->GetAvatarAttribute(), sizeof(sAVATAR_ATTRIBUTE));
	pProfile->byAvatarType = DBO_AVATAR_TYPE_SUMMON_PET_1;
	pProfile->bySourceType = DBO_OBJECT_SOURCE_SKILL;
	pProfile->curLp = GetCurLP();
	pProfile->hOwner = GetOwnerID();
	pProfile->npcTblidx = GetTblidx();
	pProfile->sourceTblidx = m_sourceTblidx;
	pProfile->wCurEP = GetCurEP();
}

void CSummonPet::Spawn(CPlayer* pOwner)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sNPC_TBLDAT* tbldat = GetTbldat();

	SetCurLP(tbldat->dwBasic_LP);
	SetExp(0);
	SetZeni(0);
	SetCurEP(tbldat->wBasic_EP);

	m_vecFirstBattleLoc.operator=(GetEnterLoc());
	m_vecFirstBattleDir.operator=(GetEnterDir());

	CNtlPacket packetPet(sizeof(sGU_PET_CHAR_INFO));
	sGU_PET_CHAR_INFO * resPet = (sGU_PET_CHAR_INFO *)packetPet.GetPacketData();
	resPet->wOpCode = GU_PET_CHAR_INFO;
	resPet->handle = GetID();
	resPet->byObjectType = OBJTYPE_SUMMON_PET;
	CopySummonPetProfile(&resPet->sSummonPetProfile);	
	resPet->sCharState.sCharStateBase.bFightMode = false;
	resPet->sCharState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	resPet->sCharState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = true;
	resPet->sCharState.sCharStateDetail.sCharStateSpawning.byTeleportType = TELEPORT_TYPE_DEFAULT;
	resPet->sCharState.sCharStateBase.dwConditionFlag = 0;
	resPet->sCharState.sCharStateBase.dwStateTime = 0;
	resPet->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	bot_profile.vSpawnLoc.CopyTo(resPet->sCharState.sCharStateBase.vCurLoc);
	bot_profile.vSpawnDir.CopyTo(resPet->sCharState.sCharStateBase.vCurDir);
	resPet->sCharState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	m_pStateManager->CopyFrom(&resPet->sCharState);

	resPet->wCharStateSize = sizeof(sCHARSTATE_BASE) + sizeof(CHARSTATE_SPAWNING);
	packetPet.SetPacketLen(sizeof(sGU_PET_CHAR_INFO));
	pOwner->SendPacket(&packetPet);


	CNtlPacket packetPet2(sizeof(sGU_PET_SKILL_INFO));
	sGU_PET_SKILL_INFO * resPet2 = (sGU_PET_SKILL_INFO *)packetPet2.GetPacketData();
	resPet2->wOpCode = GU_PET_SKILL_INFO;
	resPet2->bySkillCount = GetSkillManager()->FillPetSkillInfo(resPet2->aSkillInfo);
	packetPet2.SetPacketLen(sizeof(sGU_PET_SKILL_INFO));
	pOwner->SendPacket(&packetPet2);


	CNtlPacket packetPet3(sizeof(sGU_PET_INFO_END));
	sGU_PET_INFO_END * resPet3 = (sGU_PET_INFO_END *)packetPet3.GetPacketData();
	resPet3->wOpCode = GU_PET_INFO_END;
	packetPet3.SetPacketLen(sizeof(sGU_PET_INFO_END));
	pOwner->SendPacket(&packetPet3);


	SetMoveDirection(NTL_MOVE_NONE);
	SetMoveFlag(NTL_MOVE_FLAG_INVALID);

	SetInitialized(true);
	m_bIsInRespawn = false;


	GetBotController()->ChangeControlState_Enter();
	app->GetGameMain()->GetWorldManager()->EnterObject(this, m_worldID, false);
}

void CSummonPet::LeaveGame()
{
	if (CPlayer* pOwner = GetOwner())
	{
		if(pOwner->IsInitialized())
			pOwner->SetCurrentPetId(INVALID_HOBJECT);
	}

	CNpc::LeaveGame();
}

HOBJECT CSummonPet::ConsiderAttackTarget(DWORD dwTickTime)
{
	if (m_pStateManager->IsCharCondition(CHARCOND_TAUNT) && GetTargetListManager()->GetTauntTarget() != INVALID_HOBJECT)
		return GetTargetHandle();

	if (m_pStateManager->IsCharCondition(CHARCOND_CONFUSED))
		return GetTargetInConfusion();

	if (GetCurWorld() == NULL)
	{
		m_hAttackTarget = INVALID_HOBJECT;
		return m_hAttackTarget;
	}

	if (m_hAttackTarget != INVALID_HOBJECT)
	{
		CCharacter* pChar = g_pObjectManager->GetChar(m_hAttackTarget);
		if (pChar == NULL || pChar->IsInitialized() == false)
		{
			m_hAttackTarget = INVALID_HOBJECT;
			return m_hAttackTarget;
		}

		if (GetCurWorld()->CanDoBattle(this, pChar) == false)
		{
			m_hAttackTarget = INVALID_HOBJECT;
			return m_hAttackTarget;
		}

		CPlayer* pMaster = GetOwner();
		if (pMaster && pMaster->IsInitialized() && pChar->IsPC())
		{
			CPlayer* pTargetPc = dynamic_cast<CPlayer*>(pChar);
			if (pTargetPc)
			{
				if (pMaster->IsAttackable(pTargetPc) == false || pTargetPc->IsValidTarget(0) == false)
				{
					m_hAttackTarget = INVALID_HOBJECT;
					return m_hAttackTarget;
				}

				//if (
				//	(pMaster->IsPvpZone() == false || pTargetPc->IsPvpZone() == false) //dont attack if master or target are not in pvp zone
				//	|| (pMaster->GetFreeBattleID() != pTargetPc->GetFreeBattleID()) //dont attack if master and target dont have same freebattle id
				//	) 
				//{
				//	m_hAttackTarget = INVALID_HOBJECT;
				//	return m_hAttackTarget;
				//}

				//dont attack party members
				if (pMaster->GetPartyID() != INVALID_PARTYID && pMaster->GetPartyID() == pTargetPc->GetPartyID())
				{
					m_hAttackTarget = INVALID_HOBJECT;
					return m_hAttackTarget;
				}
			}
		}

		if (IsTargetAttackble(pChar, INVALID_WORD))
		{
			std::list<CNtlVector> rlistCollisionPos;
			if (IsReachable(pChar, rlistCollisionPos))
			{
				return m_hAttackTarget;
			}
		}

		if (pMaster && pMaster->IsInitialized() && pMaster->GetAttackProgress())
		{
			m_hAttackTarget = pMaster->GetTargetHandle();
			return m_hAttackTarget;
		}

		m_hAttackTarget = INVALID_HOBJECT;
	}

	return INVALID_HOBJECT;
}

bool CSummonPet::AttackProgress(DWORD dwTickDiff, float fMultiple)
{
	/*if (GetToggleAttack() == false)
	{
		ChangeTarget(INVALID_HOBJECT);
		m_hAttackTarget = INVALID_HOBJECT;
		return false;
	}*/

	return CNpc::AttackProgress(dwTickDiff, fMultiple);
}




CPlayer * CSummonPet::GetOwner()
{
	return g_pObjectManager->GetPC(m_hOwner);
}

BYTE CSummonPet::GetLevel()
{
	return GetTbldat()->byLevel;
}

