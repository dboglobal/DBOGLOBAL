#include "stdafx.h"
#include "CPlayer.h"
#include "Monster.h"
#include "NtlPacket.h"
#include "NtlPacketGU.h"
#include "GameServer.h"
#include "NtlBitFlagManager.h"
#include "calcs.h"
#include "item.h"
#include "battle.h"
#include "NtlResultCode.h"

#include "GameMain.h"
#include "TableContainerManager.h"
#include "ModelToolCharDataTable.h"
#include "FormulaTable.h"
#include "WpsAlgoObject.h"
#include "NtlNavi.h"

#include "RangeCheck.h"



CCharacter::CCharacter(eOBJTYPE eObjType)
	:CCharacterObject(eObjType)
{
	this->Initialize();
}

CCharacter::~CCharacter()
{
	if (!m_bIsDeleted)
		this->Destroy();
}


//--------------------------------------------------------------------------------------//
//		CREATE
//--------------------------------------------------------------------------------------//
bool CCharacter::Create(sCHARSTATE* sCharState)
{
	if (CCharacterObject::Create((eAIR_STATE)sCharState->sCharStateBase.eAirState))
	{
		m_pStateManager->CopyFrom(sCharState);

		m_pTargetListManager = CreateTargetListManager();
		if (m_pTargetListManager)
		{
			if (m_cBotBreakUp.Create(this))
			{
				if (m_cMeHaterManager.Create(this))
				{
					SetMoveDirection(NTL_MOVE_NONE);
					SetMoveFlag(NTL_MOVE_FLAG_INVALID);

					return true;
				}
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		DESTROY CHAR
//--------------------------------------------------------------------------------------//
void CCharacter::Destroy()
{
	g_pEventMgr->RemoveEvents(this);

	m_ScriptMap.clear(); //clear scripts

	SAFE_DELETE(m_pTargetListManager);

	m_cMeHaterManager.Destroy();
	m_cBotBreakUp.Destroy();
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CCharacter::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	CCharacterObject::TickProcess(dwTickDiff, fMultiple);

	Recover(dwTickDiff);

	m_pTargetListManager->TickProcess(dwTickDiff, fMultiple);
}

void CCharacter::LeaveGame()
{
	GetBuffManager()->LeaveGame();

	SetInitialized(false);
}


//--------------------------------------------------------------------------------------//
//		INITIALIZE CHAR (Called on ccharacter constructor)
//--------------------------------------------------------------------------------------//
void CCharacter::Initialize()
{
	m_vTeleportLoc.Reset();
	m_vTeleportDir.Reset();
	m_uiTeleportWorldID = INVALID_WORLDID;
	m_byTeleportType = TELEPORT_TYPE_INVALID;

	m_vBeforeTeleportLoc.Reset();
	m_uiBeforeTeleportWorldID = INVALID_WORLDID;

	m_partyId = INVALID_PARTYID;

	m_pTargetListManager = NULL;

	m_bySizeRate = 10;
	m_hMeAttackBot = INVALID_HOBJECT;
	InitKnockDown();
	m_hTargetInConfusion = INVALID_HOBJECT;
	m_bCheckTargetInConfusion = false;

	m_ScriptMap.clear();

	m_dwRecoverTick = 3000;

	m_pTimeQuestDungeon = NULL;

	m_specialSkillTblidx = INVALID_TBLIDX;

	m_hHtbSkillCaster = INVALID_HOBJECT;
}


float CCharacter::GetAttackRange(CCharacter * pTarget)
{
	float fRange = 0.0f;

	if (pTarget)
	{
		fRange = pTarget->GetObjectRadius();
	}

	return fRange;
}

//--------------------------------------------------------------------------------------//
//		GET CURRENT MOVEMENT SPEED
//--------------------------------------------------------------------------------------//
float CCharacter::GetCurSpeed()
{
	float fSpeed = 0.0f;

	switch (GetMoveFlag())
	{
		case NTL_MOVE_FLAG_RUN: fSpeed = GetRunSpeed(); break;
		case NTL_MOVE_FLAG_FLY: fSpeed = GetFlySpeed(); break;
		case NTL_MOVE_FLAG_FLY_DASH: fSpeed = GetFlyDashSpeed(); break;
		case NTL_MOVE_FLAG_FLY_ACCEL: fSpeed = GetFlyAccelSpeed(); break;
		case NTL_MOVE_FLAG_FLY_QUICK_DOWN: fSpeed = GetFlyAccelSpeed() * 1.35f; break;
		case NTL_MOVE_FLAG_JUMP: fSpeed = GetRunSpeed(); break;
		case NTL_MOVE_FLAG_DASH: 
		{
			if (GetCharStateID() == CHARSTATE_SKILL_AFFECTING)
				return NTL_ACTIVE_DASH_SPEED;

			fSpeed = Dbo_GetDashSpeed(GetWalkingSpeed(), GetRunSpeed());
		}
		break;
		case NTL_MOVE_FLAG_FALLING: fSpeed = DBO_FALLING_SPEED; break;
	
		default: fSpeed = GetWalkingSpeed(); break;
	}

	return fSpeed;
}


//--------------------------------------------------------------------------------------//
//	IS CHAR STATE MOVING (not used for PC)
//--------------------------------------------------------------------------------------//
bool CCharacter::IsBotStateMovingMode()
{
	return GetCharStateID() >= CHARSTATE_MOVING && GetCharStateID() <= CHARSTATE_FOLLOWING;
}

//--------------------------------------------------------------------------------------//
//	FOLLOW UPDATE FOR STATE
//--------------------------------------------------------------------------------------//
bool CCharacter::UpdateFollow(DWORD dwTickDiff, float fMultiple)
{
	if (GetCurWorld() == NULL)
	{
		return false;
	}

	CCharacter* pTarget = g_pObjectManager->GetChar(GetFollowTarget());
	if(pTarget && pTarget->IsInitialized())
	{
		SetDestLoc(pTarget->GetCurLoc());
		return CMoveableObject::UpdateFollow(dwTickDiff, fMultiple);
	}

	//if bot target not found then check if we actually follow(move to) an object.
	CTriggerObject* pObject = GetCurWorld()->FindStaticObject(GetFollowTarget());
	if (pObject)
	{
		return CMoveableObject::UpdateFollow(dwTickDiff, fMultiple);
	}


	CItemDrop* pDrop = g_pItemManager->FindDrop(GetFollowTarget());
	if (pDrop)
	{
		return CMoveableObject::UpdateFollow(dwTickDiff, fMultiple);
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		UPDATE LP
//--------------------------------------------------------------------------------------//
bool CCharacter::UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/)
{
	if (CCharacterObject::UpdateCurLP(lpDiff, bIncrease, bAutoRecover))
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP));
		sGU_UPDATE_CHAR_LP * res = (sGU_UPDATE_CHAR_LP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP;
		res->handle = GetID();
		res->dwLpEpEventId = GetLpEpEventID();
		res->curLp = GetCurLP();
		res->maxLp = GetMaxLP();
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP));
		Broadcast(&packet);

	//	printf("Cur LP %u Max Lp %u\n", res->curLp, res->maxLp);

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE EP
//--------------------------------------------------------------------------------------//
bool CCharacter::UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover)
{
	if (CCharacterObject::UpdateCurEP(wEpDiff, bIncrease, bAutoRecover))
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EP));
		sGU_UPDATE_CHAR_EP * res = (sGU_UPDATE_CHAR_EP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_EP;
		res->handle = this->GetID();
		res->dwLpEpEventId = GetLpEpEventID();
		res->wCurEP = GetCurEP();
		res->wMaxEP = GetMaxEP();
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_EP));
		Broadcast(&packet); 

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE LP & EP
//--------------------------------------------------------------------------------------//
bool CCharacter::UpdateCurLpEp(int nLpDiff, WORD wEpDiff, bool bIncrease, bool bAutoRecover)
{
	bool bUpdateEP = CCharacterObject::UpdateCurEP(wEpDiff, bIncrease, bAutoRecover);
	bool bUpdateLP = CCharacterObject::UpdateCurLP(nLpDiff, bIncrease, bAutoRecover);

	if (bUpdateEP || bUpdateLP)
	{
		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP_EP));
		sGU_UPDATE_CHAR_LP_EP * res = (sGU_UPDATE_CHAR_LP_EP *)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP_EP;
		res->handle = this->GetID();
		res->dwLpEpEventId = GetLpEpEventID();
		res->curLp = GetCurLP();
		res->maxLp = GetMaxLP();
		res->wCurEP = GetCurEP();
		res->wMaxEP = GetMaxEP();
		packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP_EP));
		Broadcast(&packet);

		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		CHANGE MODEL DATA
//--------------------------------------------------------------------------------------//
bool CCharacter::ChangeModelData(const char* pszModelDataResourceName)
{
	if (!pszModelDataResourceName)
		return false;

	sMODELTOOL_CHAR_TBLDAT* pNewAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(pszModelDataResourceName);
	if (!pNewAniTbldat)
	{
		ERR_LOG(LOG_GENERAL, "ERROR Can not find model %s. GetObjType() = %u", pszModelDataResourceName, GetObjType());
		return false;
	}

	m_pAniTbldat = pNewAniTbldat;

	return RefreshObjectRadius();
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CCharacter::StartDirectPlay(BYTE byDirectionType, TBLIDX directTblidx, bool bSynchronize, bool bCanSkip, BYTE byPlayMode, bool bSendDirectPlay /*= true*/)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_DIRECT_PLAY;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateDirectPlay.byDirectPlayType = byDirectionType;
	res->sCharState.sCharStateDetail.sCharStateDirectPlay.directTblidx = directTblidx; 
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	Broadcast(&packet);

	GetStateManager()->CopyFrom(&res->sCharState);

	if (bSendDirectPlay)
	{
		CNtlPacket packetPlay(sizeof(sGU_CHAR_DIRECT_PLAY));
		sGU_CHAR_DIRECT_PLAY* resPlay = (sGU_CHAR_DIRECT_PLAY *)packetPlay.GetPacketData();
		resPlay->wOpCode = GU_CHAR_DIRECT_PLAY;
		resPlay->hSubject = GetID();
		resPlay->byPlayMode = byPlayMode;
		resPlay->directTblidx = directTblidx;
		resPlay->bCanSkip = bCanSkip;
		resPlay->bSynchronize = bSynchronize;
		packetPlay.SetPacketLen(sizeof(sGU_CHAR_DIRECT_PLAY));
		SendPacket(&packetPlay);
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CCharacter::EndDirectPlay()
{
	if (GetStateManager()->GetCharStateID() == CHARSTATE_DIRECT_PLAY || GetStateManager()->IsCharCondition(CHARCOND_DIRECT_PLAY))
	{
		GetStateManager()->RemoveConditionState(CHARCOND_DIRECT_PLAY, NULL, false);
		SendCharStateStanding();
	}
}

void CCharacter::UpdateSizeRate(BYTE bySizeRate)
{
	SetSizeRate(bySizeRate);

	CNtlPacket packet(sizeof(sGU_CHANGED_CHAR_SIZE_NFY));
	sGU_CHANGED_CHAR_SIZE_NFY * res = (sGU_CHANGED_CHAR_SIZE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHANGED_CHAR_SIZE_NFY;
	res->hSubject = GetID();
	res->bySizeRate = bySizeRate;
	packet.SetPacketLen(sizeof(sGU_CHANGED_CHAR_SIZE_NFY));
	Broadcast(&packet);
}

//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	: 
//-----------------------------------------------------------------------------------
bool CCharacter::Recover(DWORD dwTickDiff)
{
	if (IsFainting() || IsTeleporting() || IsDespawning())
		return false;

	m_dwRecoverTick = UnsignedSafeDecrease<DWORD>(m_dwRecoverTick, dwTickDiff);

	if (m_dwRecoverTick == 0)
	{
		m_dwRecoverTick = 2000;
		return true;
	}

	return false;
}

void CCharacter::StartInvincibleEvent()
{
	if (GetCurWorld() == NULL)
		return;
	else if (GetCurWorld()->GetRuleType() == GAMERULE_RANKBATTLE)
		return;

	if (g_pEventMgr->HasEvent(this, EVENT_INVINCIBLE))
	{
		return;
	}

	if(GetStateManager()->AddConditionFlags(MAKE_BIT_FLAG(CHARCOND_ATTACK_DISALLOW) | MAKE_BIT_FLAG(CHARCOND_CANT_BE_TARGETTED), true))
		g_pEventMgr->AddEvent(this, &CCharacter::OnInvincibleEvent, EVENT_INVINCIBLE, NTL_INVINCIBLE_EVENT_TIME, 1, 0);
}

void CCharacter::OnInvincibleEvent()
{
	if (IsInitialized() == false)
		return;

	GetStateManager()->RemoveConditionFlags(MAKE_BIT_FLAG(CHARCOND_ATTACK_DISALLOW) | MAKE_BIT_FLAG(CHARCOND_CANT_BE_TARGETTED), true);
}

void CCharacter::EnterConfusedState(HOBJECT hCaster)
{
	HOBJECT hTarget = hCaster;

	if (GetCurWorldCell())
	{
		CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

		int nLoopCount = 0;
		int nLoopCount2 = 0;

		CWorldCell::QUADPAGE page = GetCurWorldCell()->GetCellQuadPage(GetCurLoc());
		for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
		{
			CWorldCell* pWorldCellSibling = GetCurWorldCell()->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
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

					if (IsInRange(pTarget, DBO_CONFUSE_SCAN_DISTANCE_PC))
					{
						if (IsAttackable(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}

					pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
				}

				CCharacter* pMob = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_MOB);
				while (pMob && pMob->IsInitialized())
				{
					++nLoopCount2;
					if (nLoopCount2 > 5000)
					{
						ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
					}

					if (IsInRange(pMob, DBO_CONFUSE_SCAN_DISTANCE_PC))
					{
						if (IsAttackable(pMob))
						{
							pRangeCheck.AddBot(pMob);
						}
					}

					pMob = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pMob->GetWorldCellObjectLinker());
				}
			}
		}

		CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
		if (pResult && pResult->GetID() != GetID())
			hTarget = pResult->GetID();
	}

	SetTargetInConfusion(hTarget);
	GetStateManager()->AddConditionState(CHARCOND_CONFUSED, NULL, true);

	// -- Check if we already attack the target.. If we do, then skip the last step
	if (IsPC())
	{
		CPlayer* pPlayer = (CPlayer*)this;
		if (pPlayer->GetAttackTarget() == hTarget)
			return;
	}
	else
	{
		if (GetTargetHandle() == hTarget)
			return;
	}
	// ---- End


	if (CannotMoveByBuff())
		return;


	CCharacter* pTarget = g_pObjectManager->GetChar(hTarget);
	if (pTarget)
	{
		sVECTOR3 vDestLoc;
		pTarget->GetCurLoc().CopyTo(vDestLoc);

		if (SendCharStateFollowing(hTarget, GetAttackRange(pTarget), DBO_MOVE_FOLLOW_AUTO_ATTACK, vDestLoc, true) == false)
			return;

		CNtlPacket packet(sizeof(sGU_SKILL_CONFUSE_TARGET_NFY));
		sGU_SKILL_CONFUSE_TARGET_NFY * res = (sGU_SKILL_CONFUSE_TARGET_NFY *)packet.GetPacketData();
		res->wOpCode = GU_SKILL_CONFUSE_TARGET_NFY;
		res->hSubject = GetID();
		res->hTarget = hTarget;
		packet.SetPacketLen(sizeof(sGU_SKILL_CONFUSE_TARGET_NFY));
		Broadcast(&packet);
	}
}

void CCharacter::EnterTerrorState(HOBJECT hCaster)
{
	GetStateManager()->AddConditionState(CHARCOND_TERROR, NULL, true);

	CCharacter* pCaster = g_pObjectManager->GetChar(hCaster);
	if (pCaster)
	{
		if (CannotMoveByBuff())
			return;

		CNtlVector vDestDir(GetCurLoc() - pCaster->GetCurLoc());
		vDestDir.y = 0.0f;
		if (vDestDir.SafeNormalize())
		{
			//printf("TargetDir: %f %f %f | DestDir: %f %f %f | CasterDir: %f %f %f \n", GetCurDir().x, GetCurDir().y, GetCurDir().z, vDestDir.x, vDestDir.y, vDestDir.z, pCaster->GetCurDir().x, pCaster->GetCurDir().y, pCaster->GetCurDir().z);
			SetCurDir(vDestDir);
		}

		if (SendCharStateMoving(NTL_MOVE_F, NTL_MOVE_FLAG_RUN, true) == false)
			return;

		CNtlPacket packet(sizeof(sGU_SKILL_TERROR_COURSE_CHANGE_NFY));
		sGU_SKILL_TERROR_COURSE_CHANGE_NFY * res = (sGU_SKILL_TERROR_COURSE_CHANGE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_SKILL_TERROR_COURSE_CHANGE_NFY;
		res->hSubject = GetID();
		res->hAttacker = hCaster;
		packet.SetPacketLen(sizeof(sGU_SKILL_TERROR_COURSE_CHANGE_NFY));
		Broadcast(&packet);
	}
}

void CCharacter::InsertKeepEffectTarget(HOBJECT hObj, BYTE byBuffIndex)
{
	m_mapKeepEffectTargets.insert(std::make_pair(hObj, byBuffIndex));
}

void CCharacter::EraseKeepEffectTarget(HOBJECT hObj)
{
	std::multimap<HOBJECT, BYTE>::iterator it = m_mapKeepEffectTargets.find(hObj);
	if(it != m_mapKeepEffectTargets.end())
		m_mapKeepEffectTargets.erase(it);	// erasing by iterator (1 element)
}

void CCharacter::ClearKeepEffectMap()
{
	m_mapKeepEffectTargets.clear();
}

void CCharacter::RemoveKeepEffectTargets()
{
	for (std::map<HOBJECT, BYTE>::iterator it = m_mapKeepEffectTargets.begin(); it != m_mapKeepEffectTargets.end(); )
	{
		HOBJECT hId = it->first;
		BYTE byBuffIndex = it->second;

		it = m_mapKeepEffectTargets.erase(it);

		CCharacter* pChar = g_pObjectManager->GetChar(hId);
		if (pChar && pChar->IsInitialized())
		{
			pChar->GetBuffManager()->EndBuff(byBuffIndex, CBuff::BUFF_REMOVAL_REASON_BY_ITSELF); // if we do this, then we crash client if 2 chars use skill like "Electrical Discharge" and then 1 leaves.
		}
	}

	m_mapKeepEffectTargets.clear();
}

float CCharacter::GetCrowdControlReduction(HOBJECT hTarget, TBLIDX skillIdx)
{
	std::map<TBLIDX, float>::iterator it = m_mapCrowdcontrolReduction.find(skillIdx);
	if (it == m_mapCrowdcontrolReduction.end())
	{
		m_mapCrowdcontrolReduction.insert({ skillIdx, 0.0f });
		return 0;
	}

	it->second += 20.0f;

	if (it->second >= 100.0f)
	{
		it->second = 80.0f;

		return 99.f;
	}

	return it->second;
}

void CCharacter::ClearCrowdControlReduction()
{
	m_mapCrowdcontrolReduction.clear();
}


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	: return true on death
//-----------------------------------------------------------------------------------
bool CCharacter::OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult, eFAINT_REASON eReason/* = FAINT_REASON_HIT*/)
{
	if (IsFainting())
		return false;

	if (pAttacker) //pAttacker can be NULL if reflectdmg is true.
	{
		DWORD dwAggro = (DWORD)((float)nDmg * CFormulaTable::m_afRate[8001][1] * max((pAttacker->GetLevel() - GetLevel()), 10) / 100.f);
		ChangeAggro(pAttacker->GetID(), DBO_AGGRO_CHANGE_TYPE_INCREASE, dwAggro + (DWORD)pAttacker->GetTbldat()->wBasic_Aggro_Point);
	}

	if (GetCharStateID() == CHARSTATE_SLEEPING && byAttackResult != BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE)
	{
		m_pBuffManager->EndBuff(ACTIVE_SLEEP);
	}
	else if (GetCharStateID() == CHARSTATE_KEEPING_EFFECT) //cancel keeping effect skill when getting attacked
	{
		SendCharStateStanding();
	}

	if (nDmg > 0)
	{
		//shield/barrier check
		if (GetStateManager()->IsCharCondition(CHARCOND_BARRIER))
		{
			if (CBuff* buffShield = GetBuffManager()->FindAnyBuff(ACTIVE_BARRIER))
			{
				int nShield = buffShield->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue;

				if (nDmg >= nShield)
				{

					if(buffShield->GetBuffInfo()->aBuffParameter[1].buffParameter.fParameter == 0.0f)
						buffShield->GetBuffInfo()->dwTimeRemaining = 0;

					nDmg -= nShield;
				}
				else
				{
					buffShield->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue -= (DWORD)nDmg;
					nDmg = 0;
				}
			}
		}
	}

	//check again if dmg is over 0. It could be set to 0 because of barrier
	if (nDmg <= 0)
		return false;

	if (byAttackResult == BATTLE_ATTACK_RESULT_REFLECTED_DAMAGE && GetCurLP() > 1)
	{
		int nCurLp = GetCurLP();

		if (nCurLp < nDmg)
		{
			nDmg = nCurLp - 1;
		}
		if (nCurLp > 1)
		{
			UpdateCurLP(nDmg, false, false);
		}
	}
	else
	{
		UpdateCurLP(nDmg, false, false); //do not do "if(UpdateCurLP)". Because if cur lp is 0 then the function return false and player will not get into faint (becomes immortal)

		if (GetCurLP() <= 0)
			return Faint(pAttacker, eReason);
	}

	return false;
}


bool CCharacter::OnSkillAction(CCharacter* pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget)
{
	if (IsFainting())
		return false;

	if (GetCharStateID() == CHARSTATE_KEEPING_EFFECT) //cancel keeping effect skill when getting attacked
	{
		SendCharStateStanding();
	}

	if (nDmg > 0)
	{
		if (GetCharStateID() == CHARSTATE_SLEEPING && bWakeUpTarget)
		{
			m_pBuffManager->EndBuff(ACTIVE_SLEEP);
		}

		//shield/barrier check
		if (GetStateManager()->IsCharCondition(CHARCOND_BARRIER))
		{
			if (CBuff* buffShield = GetBuffManager()->FindAnyBuff(ACTIVE_BARRIER))
			{
				int nShield = buffShield->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue;

				if (nDmg >= nShield)
				{
					buffShield->GetBuffInfo()->dwTimeRemaining = 0;
					nDmg -= nShield;
				}
				else
				{
					buffShield->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue -= nDmg;
					nDmg = 0;
				}
			}
		}
	}

	//check again if dmg is over 0. It could be set to 0 because of barrier
	if (nDmg <= 0)
		return false;


	UpdateCurLP(nDmg, false, false); //do not do "if(UpdateCurLP)". Because if cur lp is 0 then the function return false and player will not get into faint (becomes immortal)

	if (GetCurLP() <= 0)
		return Faint(pAttacker, FAINT_REASON_SKILL);

	return false;
}


bool CCharacter::ChangeFightMode(bool bFightMode)
{
	if (CCharacterObject::ChangeFightMode(bFightMode))
	{
		CNtlPacket packet(sizeof(sGU_CHAR_FIGHTMODE));
		sGU_CHAR_FIGHTMODE * res = (sGU_CHAR_FIGHTMODE *)packet.GetPacketData();
		res->wOpCode = GU_CHAR_FIGHTMODE;
		res->handle = GetID();
		res->bFightMode = bFightMode;
		packet.SetPacketLen(sizeof(sGU_CHAR_FIGHTMODE));
		Broadcast(&packet);

		return true;
	}
	return false;
}

bool CCharacter::ChangeAttackProgress(bool bAttackProgress)
{
	if (CCharacterObject::ChangeAttackProgress(bAttackProgress))
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CCharacter::SetTauntTarget(HOBJECT hTarget)
{
	if (hTarget != INVALID_HOBJECT)
	{
		ChangeTarget(hTarget);
		m_pTargetListManager->SetTauntTarget(hTarget);

		GetStateManager()->AddConditionState(CHARCOND_TAUNT, NULL, true);
	}
	else
	{
		m_pTargetListManager->SetTauntTarget(INVALID_HOBJECT);
		GetStateManager()->RemoveConditionState(CHARCOND_TAUNT, NULL, true);
	}
}

//--------------------------------------------------------------------------------------//
//		CREATE TARGET LIST MANAGER
//--------------------------------------------------------------------------------------//
CTargetListManager* CCharacter::CreateTargetListManager()
{
	CTargetListManager* pManager = new CTargetListManager;

	if (!pManager->Create(this))
	{
		ERR_LOG(LOG_SYSTEM, "CTargetListManager Create Fail");
		return NULL;
	}

	return pManager;
}

void CCharacter::ChangeAggro(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue)
{
	GetTargetListManager()->ChangeAggroDirectly(hProvoker, byAggroChangeType, dwValue);
}


BYTE CCharacter::GetAttackType()
{
	return GetTbldat()->byAttack_Type;
}

bool CCharacter::Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason)
{
	//remove all buffs except from items
	GetBuffManager()->RemoveAllBuff();

	ClearCrowdControlReduction();

	SendCharStateFaint(byReason); //this must be below removeallbuff or there will be issues with states

	//remove mark from all partys which marked me
	g_pPartyManager->CleanMark(&m_setMarkedByEnemyParty, GetID());

	return true;
}

float CCharacter::GetAttackFollowRange()
{
	float fRange = 0.0f;

	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget)
	{
		fRange = GetAttackRange(pTarget);
	}

	return fRange;
}


//-------------------------------------------------------------------//
//	CHECK IF TARGET IS ATTACK ABLE
//-------------------------------------------------------------------//
bool CCharacter::IsTargetAttackble(CCharacter* pTarget, WORD wRange)
{
	if (!IsAttackable(pTarget))
	{
		return false;
	}

	if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_FAKE_DEATH))
	{
		return false;
	}
	else if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_ATTACK_DISALLOW))
	{
		return false;
	}

	float fAdjustedScanRange = (float)wRange;
	float fHeightDistance = (float)wRange;

	if (pTarget->GetStateManager()->IsCharCondition(CHARCOND_HIDING_KI))
	{
		fAdjustedScanRange /= 2.0f;
		fHeightDistance /= 2.0f;
	}

	if (pTarget->GetAirState() == AIR_STATE_ON)
	{
		// maybe update this ?

		if (!IsInRange3(pTarget, fHeightDistance))
			return false;

		if (pTarget->GetCurLoc().y - GetCurLoc().y > CCharacterObject::GetAttackRange())
			return false;
	}
	else if (!IsInRange(pTarget, fAdjustedScanRange))
	{
		return false;
	}
	
	return true;
}

bool CCharacter::IsTargetAttackble(HOBJECT hTarget, WORD wRange)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(hTarget);
	if (pTarget && pTarget->IsInitialized())
		return IsTargetAttackble(pTarget, wRange);

	return false;
}


//--------------------------------------------------------------------------------------//
//	ON TARGET CHANGED
//--------------------------------------------------------------------------------------//
void CCharacter::OnTargetChanged(HOBJECT hOldTarget)
{
	if (hOldTarget != INVALID_HOBJECT)
	{
		CCharacter* pOldTarget = g_pObjectManager->GetChar(hOldTarget);
		if (pOldTarget && pOldTarget->IsInitialized())
		{
			pOldTarget->GetTargetListManager()->OnTargetReleased(GetID());
		}
	}

	if (GetTargetHandle() != INVALID_HOBJECT)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
		if (pTarget && pTarget->IsInitialized())
		{
			pTarget->GetTargetListManager()->OnTargeted(GetID(), GetObjType());
		}
	}

	//send my target to everyone who target me
	CNtlPacket packet(sizeof(sGU_CHAR_TARGET_CHANGED));
	sGU_CHAR_TARGET_CHANGED * res = (sGU_CHAR_TARGET_CHANGED *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_TARGET_CHANGED;
	res->hTarget = GetTargetHandle();
	res->hObject = GetID();
	packet.SetPacketLen(sizeof(sGU_CHAR_TARGET_CHANGED));
	GetTargetListManager()->BroadcastTargetPacket(&packet);
}


//-------------------------------------------------------------------//
//	CHECK IF CAN GO TO TARGET
//-------------------------------------------------------------------//
bool CCharacter::IsReachable(CCharacter *pTarget, std::list<CNtlVector>& rlistCollisionPos)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (rlistCollisionPos.size() > 0)
		rlistCollisionPos.clear();

	if (pTarget && pTarget->IsInitialized())
	{
		if (app->m_config.m_bEnableNavigator)
		{
			if (GetCurWorld())
			{
				if (GetNaviEngine()->IsPathDataLoaded(GetCurWorld()->GetTbldat()->dwWorldResourceID))
				{
					float fSubjectAgentRadius = Dbo_ConvertToAgentRadius(GetObjectRadius());

					sNAVI_POS src(GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
					sNAVI_POS dest(pTarget->GetCurLoc().x, pTarget->GetCurLoc().y, pTarget->GetCurLoc().z);

					eCOL_TEST_RESULT eResult = GetNaviEngine()->FastCanMoveNearestDest(GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, dest);
					switch (eResult)
					{
						case eCOL_TEST_RESULT_INVALID_DEST_POS:
						{
							sNAVI_POS sFirstCollisionPos;
							if (GetNaviEngine()->FastFirstCollisionTest(GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, dest, sFirstCollisionPos) == eCOL_TEST_RESULT_COL)
							{
								rlistCollisionPos.push_back(CNtlVector(sFirstCollisionPos.x, sFirstCollisionPos.y, sFirstCollisionPos.z));
							}
						}
						break;

						case eCOL_TEST_RESULT_COL:
						{
							sNAVI_POS sFirstCollisionPos;
							if (GetNaviEngine()->FastFirstCollisionTest(GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, dest, sFirstCollisionPos) == eCOL_TEST_RESULT_COL)
							{
								rlistCollisionPos.push_back(CNtlVector(sFirstCollisionPos.x, sFirstCollisionPos.y, sFirstCollisionPos.z));
							}
							std::vector<sNAVI_POS> vTempVector;

							return GetNaviEngine()->FastFindPath(GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, dest, vTempVector);
						}
						break;

						case eCOL_TEST_RESULT_NO_COL:
						{
							return true;
						}
						break;

						default: return false; break;
					}
				}
				else
				{
					return true;
				}
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool CCharacter::IsReachable(HOBJECT hTarget, std::list<CNtlVector>& rlistCollisionPos)
{
	if (rlistCollisionPos.size() > 0)
		rlistCollisionPos.clear();

	CCharacter* pTarget = g_pObjectManager->GetChar(hTarget);
	if (pTarget && pTarget->IsInitialized())
		return IsReachable(pTarget, rlistCollisionPos);

	return false;
}


bool CCharacter::ApplyPush(CNtlVector& rvShift)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CWorld* pWorld = GetCurWorld();
	if (pWorld)
	{
		CNtlVector rLoc;
		CNtlVector rDir;

		if (app->GetGameMain()->GetWorldManager()->GetDestLocAfterCollision(pWorld, this, rvShift, rLoc, rDir))
		{
			SetCurLoc(rLoc, pWorld);
			SetCurDir(rDir);

			return true;
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "GetDestLocAfterCollision() failed., pWorld->GetID() = %u, GetCurLoc() = (%f, %f, %f)", pWorld->GetID(), GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "The bot doesn't belong to any world.(NULL == pWorld)., GetID() = %u", GetID());
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		SEND EFFECT AFFECTED
//--------------------------------------------------------------------------------------//
void	CCharacter::SendEffectAffected(TBLIDX EffectTblidx, BYTE bySrcType, TBLIDX SrcTblidx, float fArgument1, float fArgument2, HOBJECT hCaster)
{
	CNtlPacket packet(sizeof(sGU_EFFECT_AFFECTED));
	sGU_EFFECT_AFFECTED * res = (sGU_EFFECT_AFFECTED *)packet.GetPacketData();
	res->wOpCode = GU_EFFECT_AFFECTED;
	res->handle = GetID();
	res->bySourceType = bySrcType;
	res->effectTblidx = EffectTblidx;
	res->fActualArgument1 = fArgument1;
	res->fActualArgument2 = fArgument2;
	res->hCaster = hCaster;
	res->sourceTblidx = SrcTblidx;
	res->wResultCode = GAME_SUCCESS;
	packet.SetPacketLen(sizeof(sGU_EFFECT_AFFECTED));
	this->Broadcast(&packet);
}


//--------------------------------------------------------------------------------------//
//		SEND AUTO RECOVER EFFECT
//--------------------------------------------------------------------------------------//
void	CCharacter::SendAutoRecoverNfy(TBLIDX Tblidx, BYTE buffIndex, BYTE bySrcType, TBLIDX commonConfigTblidx, DWORD dwRemainTime, DWORD dwRemainValue, int recoverValue)
{
	CNtlPacket packet(sizeof(sGU_AUTO_RECOVER_NFY));
	sGU_AUTO_RECOVER_NFY * res = (sGU_AUTO_RECOVER_NFY *)packet.GetPacketData();
	res->wOpCode = GU_AUTO_RECOVER_NFY;
	res->hHandle = GetID();
	res->buffIndex = buffIndex;
	res->bySourceType = bySrcType;
	res->commonConfigTblidx = commonConfigTblidx;
	res->dwRemainTime = dwRemainTime;
	res->dwRemainValue = dwRemainValue;
	res->recoverValue = recoverValue;
	res->tblidx = Tblidx;
	packet.SetPacketLen(sizeof(sGU_AUTO_RECOVER_NFY));
	this->Broadcast(&packet);
}


void CCharacter::LpEpRecoverOnHit(HOBJECT hCaster, float fDmg)
{
	CNtlPacket packet(sizeof(sGU_CHAR_LP_EP_RECOVERED));
	sGU_CHAR_LP_EP_RECOVERED * res = (sGU_CHAR_LP_EP_RECOVERED *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_LP_EP_RECOVERED;
	res->hCaster = hCaster;
	res->hSubject = GetID();

	res->lpEpRecovered.Reset();

	//check lp
	res->lpEpRecovered.targetLpRecoveredWhenHit = (int)(GetCharAtt()->GetLpRecoveryWhenHit() + (fDmg * GetCharAtt()->GetLpRecoveryWhenHitInPercent() / 100.0f));
	if (res->lpEpRecovered.targetLpRecoveredWhenHit > 0)
		res->lpEpRecovered.bIsLpRecoveredWhenHit = true;
	//check ep
	res->lpEpRecovered.dwTargetEpRecoveredWhenHit = (DWORD)(GetCharAtt()->GetEpRecoveryWhenHit() + (fDmg * GetCharAtt()->GetEpRecoveryWhenHitInPercent() / 100.0f));
	if (res->lpEpRecovered.dwTargetEpRecoveredWhenHit > 0)
		res->lpEpRecovered.bIsEpRecoveredWhenHit = true;
	//check if lp or ep is being recovered
	if (res->lpEpRecovered.bIsLpRecoveredWhenHit || res->lpEpRecovered.bIsEpRecoveredWhenHit)
	{
		//broadcast packet
		Broadcast(&packet);

		//update LP
		if (res->lpEpRecovered.bIsLpRecoveredWhenHit)
			UpdateCurLP(res->lpEpRecovered.targetLpRecoveredWhenHit, true, false);

		//update EP
		if (res->lpEpRecovered.bIsEpRecoveredWhenHit)
			UpdateCurEP((WORD)res->lpEpRecovered.dwTargetEpRecoveredWhenHit, true, false);
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
bool CCharacter::CannotMoveByBuff()
{
	if (GetCharStateID() == CHARSTATE_STUNNED)
		return true;
	else if (GetCharStateID() == CHARSTATE_SLEEPING)
		return true;
	else if (GetCharStateID() == CHARSTATE_PARALYZED)
		return true;
	else if (GetCharStateID() == CHARSTATE_KNOCKDOWN)
		return true;

	return false;
}


//--------------------------------------------------------------------------------------//
//		return true on death
//--------------------------------------------------------------------------------------//
bool CCharacter::OnBuffDamage(HOBJECT hCaster, float fValue)
{
	if (fValue > 0.0f)
	{
		CCharacter* pCaster = NULL;

		if (hCaster != GetID())
		{
			pCaster = g_pObjectManager->GetChar(hCaster);
			if (pCaster)
			{
				ChangeAggro(hCaster, DBO_AGGRO_CHANGE_TYPE_INCREASE, (DWORD)fValue / 2);
			}
		}
	
		UpdateCurLP((int)fValue, false, false); //do not do "if(UpdateCurLP)". Because if cur lp is 0 then the function return false and player will not get into faint (becomes immortal)
		
		if (GetCurLP() <= 0)
			return Faint(pCaster, FAINT_REASON_BUFF);
	}

	return false;
}


void CCharacter::StartTeleport(CNtlVector& destLoc, CNtlVector& destDir, WORLDID worldid, BYTE byTeleType, TBLIDX directPlayIdx, bool bSyncDirectPlay, bool bTeleportAnotherServer)
{
	SetTeleportLoc(destLoc);
	SetTeleportDir(destDir);
	SetTeleportWorldID(worldid);
	SetTeleportType(byTeleType);

	if (GetWorldID() != worldid)
	{
		m_vBeforeTeleportLoc = GetCurLoc();
		m_uiBeforeTeleportWorldID = GetWorldID();
	}

	SendCharStateDespawning(byTeleType);
}


//--------------------------------------------------------------------------------------//
//		SET CHARACTER STATE SLEEPING
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateSleeping()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_SLEEPING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = this->GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen( sizeof(sGU_UPDATE_CHAR_STATE) );

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET CHARACTER STATE PARALYZED
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateParalyzed()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_PARALYZED;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = this->GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen( sizeof(sGU_UPDATE_CHAR_STATE) );

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET CHARACTER STATE STUNNED
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateStunned(eDBO_STUN_TYPE eStunType/* = DBO_STUN_TYPE_GENERAL*/, HOBJECT hCaster/* = INVALID_HOBJECT*/, bool bCheckCaster/* = false*/)
{
	if (GetStateManager()->CanCharStateTransition(CHARSTATE_STUNNED) == false)
	{
		return;
	}

	m_sCharStun.eStunType = eStunType;
	m_sCharStun.hCaster = hCaster;
	m_sCharStun.bCheckCaster = bCheckCaster;

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_STUNNED;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateStunned.byStunType = eStunType;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))
		Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		SANDBAG
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateSandbag()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_SANDBAG;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	GetStateManager()->CopyFrom(&res->sCharState);
	Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		Knockdown
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStateKnockdown(sVECTOR3& rShift)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_KNOCKDOWN;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = this->GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateKnockdown.vShift.x = rShift.x;
	res->sCharState.sCharStateDetail.sCharStateKnockdown.vShift.y = rShift.y;
	res->sCharState.sCharStateDetail.sCharStateKnockdown.vShift.z = rShift.z;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		Broadcast(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		SET STANDING STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateStanding(bool bCheckState/* = true*/)
{
	if (bCheckState && GetCharStateID() == CHARSTATE_STANDING)
	{
		/*if (IsPC())
			printf("pc handle %u already in standing state \n", GetID());*/
		return;
	}

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		SET PC CASTING STATE
//--------------------------------------------------------------------------------------//
bool	CCharacter::SendCharStateCasting(DWORD castingtime, DWORD timeremaining, TBLIDX skillid, HOBJECT target)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_CASTING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = this->GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateCasting.dwCastingTime = castingtime;
	res->sCharState.sCharStateDetail.sCharStateCasting.dwCastingTimeRemaining = timeremaining;
	res->sCharState.sCharStateDetail.sCharStateCasting.skillId = skillid;
	res->sCharState.sCharStateDetail.sCharStateCasting.hTarget = target;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		Broadcast(&packet);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET PC ITEM CASTING STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateItemCasting(DWORD castingtime, DWORD timeremaining, TBLIDX itemid, HOBJECT target)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_CASTING_ITEM;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateCastingItem.dwCastingTime = castingtime;
	res->sCharState.sCharStateDetail.sCharStateCastingItem.dwCastingTimeRemaining = timeremaining;
	res->sCharState.sCharStateDetail.sCharStateCastingItem.itemTblidx = itemid;
	res->sCharState.sCharStateDetail.sCharStateCastingItem.hTarget = target;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET SKILL AFFECTING (state while skill usage)
//--------------------------------------------------------------------------------------//
bool CCharacter::SendCharStateSkillAffecting(TBLIDX skillid, HOBJECT hTarget)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_SKILL_AFFECTING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateSkillAffecting.skillId = skillid;
	res->sCharState.sCharStateDetail.sCharStateSkillAffecting.hTarget = hTarget;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		Broadcast(&packet);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET PC IDLE STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateIdle()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_CAMPING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET PC KEEPING EFFECT STATE
//--------------------------------------------------------------------------------------//
bool	CCharacter::SendCharStateKeepingEffect(TBLIDX skillTblidx)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_KEEPING_EFFECT;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateKeepingEffect.skillId = skillTblidx;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		Broadcast(&packet);
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET PC SPAWNING STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateSpawning(BYTE byType, bool bSpawnDirection /*= true*/, bool bIsFaint /*= false*/)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateSpawning.byTeleportType = byType;
	res->sCharState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = bSpawnDirection;
	res->sCharState.sCharStateDetail.sCharStateSpawning.bIsFaint = bIsFaint;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if(GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET FALLING STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateFalling(BYTE byMoveDirection)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_FALLING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.vCurDir.y = 0.0f; // y always 0.0f when falling !
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateFalling.byMoveFlag = NTL_MOVE_FLAG_FALLING;
	res->sCharState.sCharStateDetail.sCharStateFalling.byMoveDirection = byMoveDirection;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET DESPAWNING STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateDespawning(BYTE byTeleportType)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_DESPAWNING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateDespawning.byTeleportType = byTeleportType;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET OPERATING STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStateOperating(TBLIDX directTblidx, DWORD dwOperateTime, HOBJECT hTarget)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_OPERATING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateOperating.directTblidx = directTblidx;
	res->sCharState.sCharStateDetail.sCharStateOperating.dwOperateTime = dwOperateTime;
	res->sCharState.sCharStateDetail.sCharStateOperating.hTargetObject = hTarget;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		SET FOLLOWING STATE
//--------------------------------------------------------------------------------------//
bool CCharacter::SendCharStateFollowing(HOBJECT hTarget, float fDistance, BYTE byMoveReason, sVECTOR3& vDestLoc, bool bRunMode, bool bBroadCast/* = true*/)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_FOLLOWING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;

	if (GetAirState() == AIR_STATE_ON)
	{
		res->sCharState.sCharStateDetail.sCharStateFollwing.byMoveFlag = GetMoveFlag();
	}
	else
	{
		if (bRunMode)
		{
			res->sCharState.sCharStateDetail.sCharStateFollwing.byMoveFlag = NTL_MOVE_FLAG_RUN; // to do: check if swimming
		}
		else
		{
			res->sCharState.sCharStateDetail.sCharStateFollwing.byMoveFlag = NTL_MOVE_FLAG_WALK;
		}
	}

	res->sCharState.sCharStateDetail.sCharStateFollwing.byMovementReason = byMoveReason;
	res->sCharState.sCharStateDetail.sCharStateFollwing.fDistance = fDistance;
	res->sCharState.sCharStateDetail.sCharStateFollwing.hTarget = hTarget;
	res->sCharState.sCharStateDetail.sCharStateFollwing.vDestLoc.x = vDestLoc.x;
	res->sCharState.sCharStateDetail.sCharStateFollwing.vDestLoc.y = vDestLoc.y;
	res->sCharState.sCharStateDetail.sCharStateFollwing.vDestLoc.z = vDestLoc.z;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		if (bBroadCast)
		{
			Broadcast(&packet);
		}

		return true;
	}

	return false;
}
//--------------------------------------------------------------------------------------//
//		SET TELEPORTING STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStateTeleporting()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE* res = (sGU_UPDATE_CHAR_STATE*)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_TELEPORTING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateTeleporting.byTeleportType = GetTeleportType();
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}


//--------------------------------------------------------------------------------------//
//		SET FOCUSING STATE | CALLED WHEN USING RP BALL
//--------------------------------------------------------------------------------------//
bool CCharacter::SendCharStateFocusing()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->handle = GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_FOCUSING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = true;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		Broadcast(&packet);
		return true;
	}

	return false;
}


void CCharacter::SendCharStateLeaving(eCHARLEAVING_TYPE eCharLeaveType, bool bSendPacket)
{
	if (GetCharStateID() == CHARSTATE_LEAVING)
		return;

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_LEAVING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateLeaving.byLeavingType = eCharLeaveType;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState) && bSendPacket)	//if change state success then broadcast
		Broadcast(&packet);
}

void CCharacter::SendCharStateDash(CNtlVector & destLoc)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_DASH_PASSIVE;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateDashPassive.byMoveDirection = NTL_MOVE_F;
	destLoc.CopyTo(res->sCharState.sCharStateDetail.sCharStateDashPassive.vDestLoc);
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	GetStateManager()->CopyFrom(&res->sCharState);

	Broadcast(&packet, this);//send packet no matter what. Dont need to send to myself otherwise there will be issues when getting kd'ed while dashing
}

//--------------------------------------------------------------------------------------//
//		SET RIDING STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStateRiding(HOBJECT hBusId)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_RIDEON;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateRideOn.hTarget = hBusId;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		SET FAINT STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateFaint(BYTE byReason)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = this->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_FAINTING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateFainting.byReason = byReason;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET PRIVATE SHOP STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStatePrivateShop(bool bIsOwnerEmpty, BYTE byShopState, WCHAR* wcPrivateShopName)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_PRIVATESHOP;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStatePrivateShop.sSummaryPrivateShopData.bIsOwnerEmpty = bIsOwnerEmpty;
	res->sCharState.sCharStateDetail.sCharStatePrivateShop.sSummaryPrivateShopData.byShopState = byShopState;
	wcscpy_s(res->sCharState.sCharStateDetail.sCharStatePrivateShop.sSummaryPrivateShopData.wcPrivateShopName, NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1, wcPrivateShopName);
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		SET AIR DASH ACCEL STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendCharStateAirDashAccel(BYTE byMoveDir, BYTE byMoveFlag)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE* res = (sGU_UPDATE_CHAR_STATE*)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_AIR_DASH_ACCEL;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = false;
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateAirAccel.byMoveFlag = byMoveFlag;
	res->sCharState.sCharStateDetail.sCharStateAirAccel.byMoveDirection = byMoveDir;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}
//--------------------------------------------------------------------------------------//
//		SET MOVING STATE
//--------------------------------------------------------------------------------------//
bool	CCharacter::SendCharStateMoving(BYTE byMoveDir, BYTE byFlag, bool bBroadCast)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_MOVING;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = GetAirState();
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateMoving.byMoveFlag = byFlag;
	res->sCharState.sCharStateDetail.sCharStateMoving.byMoveDirection = byMoveDir;

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
	{
		if (bBroadCast)
		{
			packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
			Broadcast(&packet);
		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET DEST MOVE STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendCharStateDestMove(bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc, BYTE actionPatternIndex, BYTE byDestLocCount, sVECTOR3* pavDestLoc, bool bRunMode)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_DESTMOVE;
	GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.bFightMode = GetFightMode();
	res->sCharState.sCharStateBase.dwConditionFlag = GetConditionState();
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateDetail.sCharStateDestMove.actionPatternIndex = actionPatternIndex;

	for (int i = 0; i < byDestLocCount; i++)
	{
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].x = pavDestLoc[i].x;
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].y = pavDestLoc[i].y;
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].z = pavDestLoc[i].z;
	}

	if (bRunMode)
		res->sCharState.sCharStateDetail.sCharStateDestMove.byMoveFlag = NTL_MOVE_FLAG_RUN; // to do: check if swimming
	else
		res->sCharState.sCharStateDetail.sCharStateDestMove.byMoveFlag = NTL_MOVE_FLAG_WALK;

	res->sCharState.sCharStateDetail.sCharStateDestMove.bHaveSecondDestLoc = bHaveSecondDestLoc;
	res->sCharState.sCharStateDetail.sCharStateDestMove.byDestLocCount = byDestLocCount;

	res->sCharState.sCharStateBase.eAirState = GetAirState();
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));

	if (GetStateManager()->CopyFrom(&res->sCharState))	//if change state success then broadcast
		Broadcast(&packet);
}

void CCharacter::StandUpIfPossible(BYTE byEndState, WORD wEffectCodeEnd/* = INVALID_WORD*/)
{
	/*
		Checks if object can stand up. Used when buff effect ends.

		-- Rules for char being able to stand up
		1. The char state must be the same as the effect state which is being removed
		2. if user has the same state as the end state then check if other stuns still exist and apply them
	*/

	if (GetCharStateID() == CHARSTATE_FAINTING || GetCharStateID() == CHARSTATE_KNOCKDOWN || GetCharStateID() == CHARSTATE_SANDBAG || GetCharStateID() == CHARSTATE_GUARD || GetCharStateID() == CHARSTATE_PRIVATESHOP)
		return;

	if (GetCharStateID() == byEndState) //check if the last registered stun ends
	{
		if (GetBuffManager()->CheckAndApplyOtherStun(byEndState, wEffectCodeEnd) == false) //check if any previous stuns exist. If yes, then apply them and return true. Otherwise return false.
		{
			SendCharStateStanding();
		}
	}
}

void CCharacter::StartCandy()
{
	SendCharStateStunned(DBO_STUN_TYPE_CANDY);
}

void CCharacter::EndCandy()
{
	if (GetCharStateID() == CHARSTATE_STUNNED)
	{
		SendCharStateStanding();
	}
}



//--------------------------------------------------------------------------------------//
//		SET SPINNING ATTACK ASPECT STATE
//--------------------------------------------------------------------------------------//
void	CCharacter::SendAspectStateSpinningAttack()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
 	sGU_UPDATE_CHAR_ASPECT_STATE * res = (sGU_UPDATE_CHAR_ASPECT_STATE*)packet.GetPacketData();
	res->handle = this->GetID();
 	res->wOpCode = GU_UPDATE_CHAR_ASPECT_STATE;
	res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_SPINNING_ATTACK;
 	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	this->Broadcast(&packet);

	GetStateManager()->CopyAspectFrom(&res->aspectState);
}

//--------------------------------------------------------------------------------------//
//		SET INVALID ASPECT STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendAspectStateInvalid()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
 	sGU_UPDATE_CHAR_ASPECT_STATE * res = (sGU_UPDATE_CHAR_ASPECT_STATE*)packet.GetPacketData();
	res->handle = GetID();
 	res->wOpCode = GU_UPDATE_CHAR_ASPECT_STATE;
	res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;
 	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	Broadcast(&packet);

	GetStateManager()->CopyAspectFrom(&res->aspectState);
}

//--------------------------------------------------------------------------------------//
//		SET ROLLING ATTACK ASPECT STATE
//--------------------------------------------------------------------------------------//
void CCharacter::SendAspectStateRollingAttack()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	sGU_UPDATE_CHAR_ASPECT_STATE * res = (sGU_UPDATE_CHAR_ASPECT_STATE*)packet.GetPacketData();
	res->handle = this->GetID();
	res->wOpCode = GU_UPDATE_CHAR_ASPECT_STATE;
	res->aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_ROLLING_ATTACK;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ASPECT_STATE));
	Broadcast(&packet);

	GetStateManager()->CopyAspectFrom(&res->aspectState);
}


void CCharacter::Send_BotNonBattleActionPatternSet_UnitIdx(BYTE actionpatternIdx)
{
	CNtlPacket packet(sizeof(sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY));
	sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY * res = (sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY;
	res->hBotHandle = GetID();
	res->unitIdx = actionpatternIdx;
	packet.SetPacketLen(sizeof(sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY));
	Broadcast(&packet);
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
bool CCharacter::AddScript(CScriptAlgoObject* pScript)
{
	if (pScript == NULL)
		return false;

	if (pScript->IsStopped())
	{
		ERR_LOG(LOG_GENERAL, "Can not add script to player because script is stopped.");
		return false;
	}

	if (m_ScriptMap.find(pScript->GetScriptID()) != m_ScriptMap.end())
		return false;

	m_ScriptMap.insert(std::make_pair(pScript->GetScriptID(), pScript));

	return true;
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CCharacter::RemScript(DWORD dwScriptID)
{
	m_ScriptMap.erase(dwScriptID);
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
CScriptAlgoObject* CCharacter::GetScript(DWORD dwScriptID)
{
	std::map<DWORD, CScriptAlgoObject*>::iterator it = m_ScriptMap.find(dwScriptID);
	if (it != m_ScriptMap.end())
		return it->second;

	return NULL;
}

int CCharacter::GetScriptCount()
{
	return (int)m_ScriptMap.size();
}

//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CCharacter::RemAllScript()
{
	if (m_ScriptMap.size() > 0)
	{
		for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_ScriptMap.begin(); it != m_ScriptMap.end(); it++)
		{
			CScriptAlgoObject* pScript = it->second;
			if (pScript)
			{
				if (IsPC())
					pScript->NotifyPlayerLeave(GetID(), false); //remove player from script. false is needed so we dont remove script from m_ScriptMap inside the function
				else
				{
					if (IsNPC())
						pScript->RemoveNpc(GetTblidx()); //remove npc from script
					else if (IsMonster())
						pScript->RemoveMob((CMonster*)this); //remove mob from script
				}
			}
		}

		m_ScriptMap.clear();
	}
}

void CCharacter::ClearScripts()
{
	m_ScriptMap.clear();
}

CScriptAlgoObject * CCharacter::GetFirstScript(eSSD_SCRIPT_TYPE scripType)
{
	for (std::map<DWORD, CScriptAlgoObject*>::iterator it = m_ScriptMap.begin(); it != m_ScriptMap.end(); it++)
	{
		CScriptAlgoObject* pScript = it->second;
		if (pScript)
		{
			if (pScript->GetScriptType() == scripType)
				return pScript;
		}
	}

	return NULL;
}
