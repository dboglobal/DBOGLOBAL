#include "stdafx.h"
#include "CharacterObject.h"
#include "char.h"
#include "GameServer.h"
#include "NtlSvrMath.h"
#include "battle.h"
#include "NtlPacketGU.h"



CCharacterObject::CCharacterObject(eOBJTYPE eObjType)
	:CMoveableObject(eObjType)
{
	Init();
}

CCharacterObject::~CCharacterObject()
{
	if (!m_bIsDeleted)
		this->Destroy();
}



bool CCharacterObject::Create(eAIR_STATE eAirState)
{
	if (CMoveableObject::Create())
	{
		m_pStateManager = CreateStateManager(eAirState);
		m_pSkillManager = CreateSkillManager();
		m_pBuffManager = CreateBuffManager();
		m_pCharacterAtt = CreateCharAttManager();

		return true;
	}

	return false;
}


void CCharacterObject::Destroy()
{
	if (IsInitialized())
		ERR_LOG(LOG_GENERAL, "IMPORTANT-ERROR: TRYING TO DELETE INITIALIZED CHARACTEROBJECT");

	SAFE_DELETE(m_pStateManager);
	SAFE_DELETE(m_pSkillManager);
	SAFE_DELETE(m_pBuffManager);
	SAFE_DELETE(m_pCharacterAtt);
}


void CCharacterObject::Init()
{
	m_byLevel = 0;
	m_curAP = 0;
	m_curLP = 0;
	m_dwEXP = 0;
	m_dwZeni = 0;
	m_wCurEP = 0;

	m_dwLpEpEventID = INVALID_DWORD;

	m_pTbldat = NULL;
	m_tblidx = INVALID_TBLIDX;
	m_fScale = 0.0f;
	m_fObjectRadius = 0.0f;

	m_pStateManager = NULL;
	m_pSkillManager = NULL;
	m_pBuffManager = NULL;
	m_pCharacterAtt = NULL;
	m_pAniTbldat = NULL;
	m_bFightMode = false;
	m_bAttackProgress = false;
	m_dwNextAttackTime = 0;
	m_dwSkillBlockTime = 0;
	m_bAlreadyFainted = false;
	m_bNeedToIgnoreDistance = false;
	m_byImmortalMode = eIMMORTAL_MODE_OFF;

	m_byCurRPBall = 0;
	m_byMaxRPBall = 0;

	m_hTarget = INVALID_HOBJECT;

	m_byChainSequence = NTL_BATTLE_CHAIN_ATTACK_START;

	m_fWalkSpeedBeforeBlock = 0.0f;
	m_fRunSpeedBeforeBlock = 0.0f;
}



void CCharacterObject::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	//info: if we process state before skills, then it can happen that some skills will do not have any effect when the affecting and state time is the same (example: both 1000 ms) because state will leave first
	if (m_pSkillManager)
		m_pSkillManager->TickProcess(dwTickDiff);

	m_pStateManager->OnUpdate(dwTickDiff, fMultiple);

	if (m_pBuffManager)
		m_pBuffManager->TickProcess(dwTickDiff);

	//if (IsNPC() || IsMonster()/*|| GetTblidx() == 4512107 || GetID() == 2020040 || IsNPC()*/)
	//{
	//	ERR_LOG(LOG_GENERAL, "Update Obj %u, ID %u, Tblidx %u, WorldID %u, WorldTblidx %u, loc %f %f %f", GetObjType(), GetID(), GetTblidx(), GetWorldID(), GetWorldTblidx(), GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
	//}

	//if (IsPC() || (IsMonster() && GetTblidx() == 3411215))
		CMoveableObject::TickProcess(dwTickDiff, fMultiple);

	if (GetAttackProgress() && GetFightMode())
		AttackProgress(dwTickDiff, fMultiple);
}


void CCharacterObject::Reset(CCharacterObject::eRESETTYPE eResetType)
{
	ChangeTarget(INVALID_HOBJECT);
	m_bFightMode = false;
	m_bAttackProgress = false;
	m_bNeedToIgnoreDistance = false;

	if (eResetType == FAINT)
		ResetAlreadyFainted();
}


bool CCharacterObject::ChangeFightMode(bool bFightMode)
{
	return SetFightMode(bFightMode);
}

bool CCharacterObject::ChangeAttackProgress(bool bAttackProgress)
{
	return SetAttackProgress(bAttackProgress);
}

bool CCharacterObject::AttackProgress(DWORD dwTickDiff, float fMultiple) //check if can attack
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->GetCurTickCount() < m_dwNextAttackTime) //check if can attack
	{
		return false;
	}
	else if (!IsStanding())
	{
		return false;
	}
	else if (GetAspectStateId() == ASPECTSTATE_SPINNING_ATTACK || GetAspectStateId() == ASPECTSTATE_VEHICLE || GetAspectStateId() == ASPECTSTATE_ROLLING_ATTACK)
	{
		return false;
	}
	else if (GetStateManager()->IsCharCondition(CHARCOND_BATTLE_INABILITY))
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------//
//		UPDATE LP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/)
{
	if (bIncrease)
	{
		return SetCurLP(Dbo_IncreaseDboLp(GetCurLP(), lpDiff, GetMaxLP()));
	}
	else
	{
		return SetCurLP(Dbo_DecreaseDboLp(GetCurLP(), lpDiff, GetMaxLP()));
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE EP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover)
{
	if (bIncrease)
	{
		return SetCurEP(Dbo_IncreaseDboEp(GetCurEP(), wEpDiff, GetMaxEP()));
	}
	else
	{
		return SetCurEP(Dbo_DecreaseDboEp(GetCurEP(), wEpDiff, GetMaxEP()));
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE RP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::UpdateCurRP(WORD wRpDiff, bool bIncrease, bool bHitCharge)
{
	if (bIncrease)
	{
		BYTE byCurRpBall = GetCurRPBall();

		if (byCurRpBall == GetMaxRPBall())
			return false;
		else
		{
			//printf("wRpDiff %u GetCurRP() %u GetCharAtt()->GetMaxRP() %u\n", wRpDiff, GetCurRP(), GetCharAtt()->GetMaxRP());
			if (wRpDiff + GetCurRP() < GetCharAtt()->GetMaxRP())
			{
				return SetCurRP(UnsignedSafeIncrease<WORD>(GetCurRP(), wRpDiff));
			}
			else
			{
				int nAddRpBall = wRpDiff / GetCharAtt()->GetMaxRP();
				if (nAddRpBall <= 0)
					nAddRpBall = 1;

				UpdateRpBall((BYTE)nAddRpBall, true, false); //increase rp ball

				if (GetCurRPBall() == GetMaxRPBall())
					SetCurRP(0);
				else
					SetCurRP((GetCurRP() + wRpDiff) - (nAddRpBall * GetCharAtt()->GetMaxRP()));

				return true;
			}
		}
	}
	else
	{
		bool bRes = SetCurRP(UnsignedSafeDecrease<WORD>(GetCurRP(), wRpDiff));
		if (bRes)
		{
			if (GetCurRP() == 0 && GetCurRPBall() > 0)
			{
				UpdateRpBall(1, false, false); //decrease rp ball by one
				SetCurRP(GetCharAtt()->GetMaxRP());
			}
		}
		return bRes;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE AP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::UpdateCurAP(int apDiff, bool bIncrease)
{
	if (bIncrease)
		return SetCurAP(Dbo_IncreaseDboAp(GetCurAP(), apDiff, GetCharAtt()->GetMaxAP()));
	else
		return SetCurAP(Dbo_DecreaseDboAp(GetCurAP(), apDiff, GetCharAtt()->GetMaxAP()));

	return false;
}


//--------------------------------------------------------------------------------------//
//		UPDATE RP BALL
//--------------------------------------------------------------------------------------//
bool CCharacterObject::UpdateRpBall(BYTE byDiff, bool bIncrease, bool bDropByTime)
{
	if (byDiff > 0)
	{
		if (bIncrease)
		{
			if (m_byCurRPBall < m_byMaxRPBall)
			{
				return SetRPBall(UnsignedSafeIncrease<BYTE>(m_byCurRPBall, byDiff));
			}
		}
		else
		{
			return SetRPBall(UnsignedSafeDecrease<BYTE>(m_byCurRPBall, byDiff));
		}
	}

	return false;
}


void CCharacterObject::OnUpdateState()
{
	CNtlPacket pStatePacket(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)pStatePacket.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = GetID();
	GetStateManager()->CopyTo(&res->sCharState);
	pStatePacket.SetPacketLen(sizeof(sGU_UPDATE_CHAR_STATE));
	Broadcast(&pStatePacket);
}

void CCharacterObject::OnUpdateAspectState()
{
	
}

void CCharacterObject::OnUpdateCondition()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_CONDITION));
	sGU_UPDATE_CHAR_CONDITION * res = (sGU_UPDATE_CHAR_CONDITION *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_CONDITION;
	res->handle = GetID();
	res->qwConditionFlag = GetStateManager()->GetConditionState();
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_CONDITION));
	Broadcast(&packet);
}



//--------------------------------------------------------------------------------------//
//		SET CUR LP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetCurLP(int curLp)
{
	if (m_curLP != curLp)
	{
		if (curLp <= 0)
		{
			if (GetImmortalMode() == eIMMORTAL_MODE_NORMAL)
				return true;
		}

		m_curLP = curLp;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET CUR EP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetCurEP(WORD wEP)
{
	if (m_wCurEP != wEP)
	{
		m_wCurEP = wEP;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET CUR RP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetCurRP(WORD wRP)
{
	if (m_wCurRP != wRP)
	{
		m_wCurRP = wRP;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET CUR RP
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetCurAP(int charAP)
{
	if (m_curAP != charAP)
	{
		m_curAP = charAP;
		return true;
	}

	return false;
}


bool CCharacterObject::ConsiderLPLow(float fLowLPFactor)
{
	return (float)GetCurLP() <= (float)GetMaxLP() * fLowLPFactor / 100.0f;
}


bool CCharacterObject::ConsiderEPLow(float fLowLPFactor)
{
	return (float)GetCurEP() <= (float)GetMaxEP() * fLowLPFactor / 100.0f;
}


//--------------------------------------------------------------------------------------//
//		Update movement speed
//--------------------------------------------------------------------------------------//
void CCharacterObject::UpdateMoveSpeed(float fWalkSpeed, float fRunSpeed)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_SPEED));
	sGU_UPDATE_CHAR_SPEED * res = (sGU_UPDATE_CHAR_SPEED *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_SPEED;
	res->handle = GetID();
	res->fLastWalkingSpeed = fWalkSpeed;
	res->fLastRunningSpeed = fRunSpeed;
	res->fLastFlyingSpeed = GetFlySpeed();
	res->fLastFlyingDashSpeed = GetFlyDashSpeed();
	res->fLastFlyingAccelSpeed = GetFlyAccelSpeed();
	Broadcast(&packet);

	SetRunSpeed(res->fLastRunningSpeed);
}
//--------------------------------------------------------------------------------------//
//		UPDATE CHARACTER ATTACK SPEED
//--------------------------------------------------------------------------------------//
void CCharacterObject::UpdateAttackSpeed(WORD wAttackSpeed)
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_ATTACK_SPEEDRATE));
	sGU_UPDATE_CHAR_ATTACK_SPEEDRATE * res = (sGU_UPDATE_CHAR_ATTACK_SPEEDRATE *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_ATTACK_SPEEDRATE;
	res->handle = this->GetID();
	res->wAttackSpeedRate = wAttackSpeed;
	packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_ATTACK_SPEEDRATE));
	Broadcast(&packet);

	SetAttackSpeedRate(res->wAttackSpeedRate);
}

void CCharacterObject::BlockMovementSpeed()
{
	m_fWalkSpeedBeforeBlock = GetWalkingSpeed();
	m_fRunSpeedBeforeBlock = GetRunSpeed();

	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_SPEED));
	sGU_UPDATE_CHAR_SPEED * res = (sGU_UPDATE_CHAR_SPEED *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_SPEED;
	res->handle = GetID();
	res->fLastWalkingSpeed = 0.0f;
	res->fLastRunningSpeed = 0.0f;
	res->fLastFlyingSpeed = GetFlySpeed();
	res->fLastFlyingDashSpeed = GetFlyDashSpeed();
	res->fLastFlyingAccelSpeed = GetFlyAccelSpeed();
	SendPacket(&packet);

	SetRunSpeed(0);
}

void CCharacterObject::UndoMovementBlock()
{
	CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_SPEED));
	sGU_UPDATE_CHAR_SPEED * res = (sGU_UPDATE_CHAR_SPEED *)packet.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_SPEED;
	res->handle = GetID();
	res->fLastWalkingSpeed = m_fWalkSpeedBeforeBlock;
	res->fLastRunningSpeed = m_fRunSpeedBeforeBlock;
	res->fLastFlyingSpeed = GetFlySpeed();
	res->fLastFlyingDashSpeed = GetFlyDashSpeed();
	res->fLastFlyingAccelSpeed = GetFlyAccelSpeed();
	SendPacket(&packet);

	SetRunSpeed(m_fRunSpeedBeforeBlock);
}


//--------------------------------------------------------------------------------------//
//		UPDATE SCALE
//--------------------------------------------------------------------------------------//
bool CCharacterObject::ChangeScale(float fScale)
{
	float fBackUpScale = m_fScale;
	m_fScale = fScale;

	if (RefreshObjectRadius())
		return true;

	m_fScale = fBackUpScale;

	return false;
}


//--------------------------------------------------------------------------------------//
//		set rp ball
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetRPBall(BYTE byNewRPBall)
{
	if (m_byCurRPBall == byNewRPBall)
		return false;

	if (byNewRPBall >= m_byMaxRPBall)
	{
		m_byCurRPBall = m_byMaxRPBall;
		return true;
	}
	else
	{
		if (byNewRPBall < 0)
		{
			ERR_LOG(LOG_SYSTEM, "Invalid RP Ball Value");
			return false;
		}
	}

	m_byCurRPBall = byNewRPBall;
	return true;
}


//--------------------------------------------------------------------------------------//
//	CHECK IF TARGET VALID
//--------------------------------------------------------------------------------------//
bool CCharacterObject::IsValidTarget(DWORD dwNoCheckStateFlag)
{
	if (m_pStateManager->IsValidTarget(dwNoCheckStateFlag))
	{
		return !m_pStateManager->IsCharCondition(CHARCOND_INVISIBLE) && !m_pStateManager->IsCharCondition(CHARCOND_TRANSPARENT) && !m_pStateManager->IsCharCondition(CHARCOND_CANT_BE_TARGETTED);
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//	CHECK IF TARGET ATTACK ABLE
//--------------------------------------------------------------------------------------//
bool CCharacterObject::IsAttackable(CCharacterObject* pTarget)
{
	if (pTarget && pTarget->IsInitialized())
	{
		if (this != pTarget)
		{
			if (pTarget->IsValidTarget(0)) //not sure
			{
				if (pTarget->GetCharStateID() != CHARSTATE_SLIDING && pTarget->GetCharStateID() != CHARSTATE_HTB && pTarget->GetCharStateID() != CHARSTATE_SANDBAG && pTarget->GetCharStateID() != CHARSTATE_DIRECT_PLAY && pTarget->GetCharStateID() != CHARSTATE_TURNING)
				{
					if (!pTarget->GetStateManager()->IsCharCondition(CHARCOND_ATTACK_DISALLOW))
					{
						if (pTarget->GetAirState() == AIR_STATE_ON)
						{
							if (pTarget->GetCurLoc().y - GetCurLoc().y > GetAttackRange())
								return false;
						}

						return true;
					}
				}
			}
		}
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
float CCharacterObject::GetAttackRange()
{
	return GetCharAtt()->GetAttackRange();
}


//--------------------------------------------------------------------------------------//
//	SET TARGET ID
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetTargetHandle(HOBJECT hHandle)
{
	if (m_hTarget == hHandle)
		return false;

	m_hTarget = hHandle;
	return true;
}


//--------------------------------------------------------------------------------------//
//	SELECT TARGET
//--------------------------------------------------------------------------------------//
bool CCharacterObject::ChangeTarget(HOBJECT hTarget)
{
	HOBJECT hOldTarget = m_hTarget;

	if (SetTargetHandle(hTarget))
	{
		OnTargetChanged(hOldTarget);
		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//	Check if object can faint / Set faint state
//--------------------------------------------------------------------------------------//
bool CCharacterObject::IsJustFainting(bool bNeedToCheckLp)
{
	if (IsAlreadyFainted())
	{
		return false;
	}
	else if (m_pStateManager->IsCharState(CHARSTATE_FAINTING))
		return false;
	else if (bNeedToCheckLp && GetCurLP() > 0)
		return false;
	else
	{
		m_bAlreadyFainted = true;
	}

	return true;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CCharacterObject::Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason)
{
	//if (IsJustFainting(false) == false)
	//	return false;
	if (IsFainting())
		return false;

	return true;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetFightMode(bool bFlag)
{
	if (m_bFightMode != bFlag)
	{
		m_byChainSequence = NTL_BATTLE_CHAIN_ATTACK_START; //reset chain attack

		m_bFightMode = bFlag;
		return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CCharacterObject::SetAttackProgress(bool bFlag)
{
	if (m_bAttackProgress != bFlag)
	{
		m_bAttackProgress = bFlag;
		return true;
	}

	return false;
}


DWORD CCharacterObject::AcquireLpEpEventID()
{
	m_dwLpEpEventID = g_pObjectManager->AcquireLpEpEventID();

	return m_dwLpEpEventID;
}

//--------------------------------------------------------------------------------------//
//		REFRESH OBJECT RADIUS
//--------------------------------------------------------------------------------------//
bool CCharacterObject::RefreshObjectRadius()
{
	if (m_pTbldat)
	{
		if (m_pAniTbldat)
		{

			m_fObjectRadius = CNtlSvrMath::CalculateObjectRadius(m_pAniTbldat->m_vMax, m_pAniTbldat->m_vMin, m_pAniTbldat->m_fScale, GetScale());
			
		//	printf("m_pAniTbldat->m_vMax = %f, m_pAniTbldat->m_vMin = %f, m_pAniTbldat->m_fScale = %f, GetScale() = %f \n", m_pAniTbldat->m_vMax.x, m_pAniTbldat->m_vMin.x, m_pAniTbldat->m_fScale, GetScale());

			/*
			if(IsPC())
				printf("m_fObjectRadius %f m_pAniTbldat->m_fScale %f GetScale %f max x%f z%f min x%f z %f\n", 
				m_fObjectRadius, m_pAniTbldat->m_fScale, GetScale(), m_pAniTbldat->m_vMax.x, m_pAniTbldat->m_vMax.z, m_pAniTbldat->m_vMin.x, m_pAniTbldat->m_vMin.z);*/

			return true;
		}
		else
		{
			ERR_LOG(LOG_USER, "Animation table data is not prepared.(NULL == m_pAniTbldat), GetObjType() = %u, m_pTbldat->tblidx = %u", GetObjType(), m_pTbldat->tblidx);
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "Character table data is not prepared.(NULL == m_pTbldat), GetObjType() = %u. Tblidx %u", GetObjType(), GetTblidx());
	}

	return false;
}





























