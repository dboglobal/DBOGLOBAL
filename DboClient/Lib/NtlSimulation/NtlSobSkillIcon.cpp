#include "precomp_ntlsimulation.h"
#include "NtlSobSkillIcon.h"

// shared
#include "NtlSkill.h"
#include "SkillTable.h"
#include "NtlResultCode.h"
#include "NtlMovement.h"


// simulation
#include "TableContainer.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobProxy.h"
#include "ResultCodeString.h"
#include "NtlSobPlayerAttr.h"



DEFINITION_MEMORY_POOL(CNtlSobSkillIcon)

RwBool CNtlSobSkillIcon::m_bUseReq		= FALSE;
RwReal CNtlSobSkillIcon::m_fUseResTimeOut	= 0.0f;


CNtlSobSkillIcon::CNtlSobSkillIcon() 
{
	SetClassID(SLCLASS_SKILL_ICON);
	SetClassName(SLCLASS_NAME_SKILL_ICON);

	m_byIconState = ICON_STATE_USEABLE;
	m_fCoolTime = 0.0f;
	m_fMaxCoolTime = 0.0f;    
}

CNtlSobSkillIcon::~CNtlSobSkillIcon() 
{
	
}

void CNtlSobSkillIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobSkillCreate *pSobItemCreate = reinterpret_cast<CNtlEventSobSkillCreate*>(pMsg.pData);
		{
			CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>(m_pSobObj);
			CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pSobSkill->GetSobAttr());
			sSKILL_TBLDAT* pSkillTbl = pSobSkillAttr->GetSkillTbl();
			m_fMaxCoolTime = (RwReal)pSkillTbl->wCool_Time;

			if( pSobItemCreate->uiRemainTime > 0 )
			{
				SetSkillIconState( ICON_STATE_COOLING, INVALID_BYTE, FALSE, (RwReal)(m_fMaxCoolTime - ( pSobItemCreate->uiRemainTime * 0.001f) ) );
			}

			CreateImage(pSkillTbl->szIcon_Name);
		}
	}	
}

RwBool CNtlSobSkillIcon::IsDragAndDropPossible(void) 
{
	NTL_FUNCTION("CNtlSobSkillIcon::IsDragAndDropPossible");

	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>(m_pSobObj);
	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pSobSkill->GetSobAttr());
	sSKILL_TBLDAT* pSkillTbl = pSobSkillAttr->GetSkillTbl();

	if(pSkillTbl->bySkill_Class == NTL_SKILL_CLASS_ACTIVE)
	{
		NTL_RETURN(TRUE);
	}

	NTL_RETURN(FALSE);
}

RwBool CNtlSobSkillIcon::IsUsePossible(void)
{
	NTL_FUNCTION("CNtlSobSkillIcon::IsUsePossible");

	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>(m_pSobObj);
	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pSobSkill->GetSobAttr());
	sSKILL_TBLDAT* pSkillTbl = pSobSkillAttr->GetSkillTbl();

	if(pSkillTbl->bySkill_Class != NTL_SKILL_CLASS_ACTIVE)
	{
		m_wErrorMsg = GAME_SKILL_NOT_ACTIVE_TYPE;
		NTL_RETURN(FALSE);
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sCHARSTATE* pState = pAvatar->GetServerFullState();
	if (!pState)
	{
		m_wErrorMsg = GAME_SKILL_CANT_CAST_NOW;
		NTL_RETURN(FALSE);
	}

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());

	BYTE byCharState = pState->sCharStateBase.byStateID;
	BYTE stunType = pBeData->GetCtrlStuff()->uExtra.sStun.byStunType;
	BYTE aspectStateID = pAvatar->GetAspectState()->sAspectStateBase.byAspectStateId;
	QWORD qwCurCharCondition = pState->sCharStateBase.dwConditionFlag;
	bool bIsSwimming = false;
	bool bIsJumping = false;
	BYTE airState = pState->sCharStateBase.eAirState;
	
	bool bValidState = Dbo_IsValidStateToUseSkill(pSkillTbl->dwTransform_Use_Info_Bit_Flag, byCharState, stunType,
		aspectStateID, qwCurCharCondition, bIsSwimming, bIsJumping,
		airState, &m_wErrorMsg);
	
	if (bValidState == false)
	{
		NTL_RETURN(FALSE);
	}

    NTL_RETURN(TRUE);	
}

RwBool CNtlSobSkillIcon::Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 uiRPBonusType/* = 0xFF*/, RwUInt32 hTarget/* = INVALID_SERIAL_ID*/, bool pStatus/* = false*/)
{
	CNtlSobAvatar* pAvatar = CNtlSLGlobal::GetInstance()->GetSobAvatar();
	NTL_ASSERT(pAvatar, "CNtlSobSkillIcon::Use => pAvatar is null !!!");

	// Code to prevent the use of skills in tutorials
	if ( m_pSobObj->GetOwnerID() == pAvatar->GetSerialID() )
	{
		if ( !Logic_CanAttack_in_Tutorial( ETL_ATTACK_TYPE_SKILL_ATTACK ) )
		{
			NTL_RETURN(FALSE);
		}
	}

	if(!IsUsePossible())
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), Logic_GetResultCodeString(m_wErrorMsg, "").c_str());
		NTL_RETURN(FALSE);
	}

	//-----------------------------------------------------------------
	// casting test.

	if(m_byIconState == ICON_STATE_COOLING)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_READY_TO_BE_CAST");
		NTL_RETURN(FALSE);
	}

	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>(m_pSobObj);
	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pSobSkill->GetSobAttr());
	sSKILL_TBLDAT* pSkillTbl = pSobSkillAttr->GetSkillTbl();

	NTL_ASSERT(pSkillTbl != NULL, "CNtlSobSkillIcon::Use => skill table pointer is null !!!");

	// -----------------------------------------------------------------
	// LP/EP/RP check.
	RwInt32 uiCurrLp = 0;
	RwUInt32 uiCurrEp = 0;
	BYTE byCurrRpStock = 0;
	float fRunSpeed = 0.0f;

	CNtlSob *pOwnerSobObj = GetNtlSobManager()->GetSobObject(m_pSobObj->GetOwnerID());
	if (pOwnerSobObj)
	{
		CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>(pOwnerSobObj);

		if (pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
		{
			CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr());
			SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

			uiCurrLp = pSobLifeAttr->GetLp();
			uiCurrEp = pSobLifeAttr->GetEp();
			byCurrRpStock = pAvatarInfo->sCharPf.byCurRPBall;
		}

		if (pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
		{
			CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pSobActor->GetSobAttr());
			fRunSpeed = pMoveableAttr->GetRunSpeed();
		}
	}

	if(uiCurrLp < (RwInt32)pSkillTbl->dwRequire_LP)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_ENOUGH_LP");
		NTL_RETURN(FALSE);
	}

	if(uiCurrEp < (RwUInt32)pSkillTbl->wRequire_EP)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_ENOUGH_EP");
		NTL_RETURN(FALSE);
	}

	// Rp 검사 ( wRequire_RP 는 이제 일반스킬도 존재한다. )
 	if(byCurrRpStock < pSkillTbl->byRequire_RP_Ball && uiRPBonusType != 0xFF )
 	{
 		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_ENOUGH_RP_BALL");
 		NTL_RETURN(FALSE);
 	}


	// -----------------------------------------------------------------
	// check if sub weapon required.
	if(pSkillTbl->byRequire_Epuip_Slot_Type == EQUIP_SLOT_TYPE_SUB_WEAPON)
	{
		SERIAL_HANDLE hSubWeaponSerialId = Logic_GetEquipedSubWeaponItemSerial(pOwnerSobObj);
		if(hSubWeaponSerialId == INVALID_SERIAL_ID)
		{
			CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NO_REQUIRED_ITEM");
			NTL_RETURN(FALSE);
		}
	}

	//-----------------------------------------------------------------
	SERIAL_HANDLE hNewTargetSerialId = hTargetSerialId;
	BYTE byRes = AppointTargetCheck(pSkillTbl, hTargetSerialId, hNewTargetSerialId);
	DBO_WARNING_MESSAGE("AppointTargetCheck: " << (int)byRes);
	switch (byRes)
	{
		case 0:
		{
			NTL_RETURN(FALSE);
		}
		break;

		case 1:
		{
			// dash able skill
			if (pSkillTbl->bDash_Able && fRunSpeed <= 0.f)
			{
				CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(hNewTargetSerialId);
				if (!pTargetActor)
				{
					CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
					NTL_RETURN(FALSE);
				}

				float fRange = Logic_GetRange(pOwnerSobObj->GetPosition(), pTargetActor->GetPosition());
				if (fRange > pSkillTbl->fUse_Range_Max)
				{
					CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_CANT_CAST_NOW");
					CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
					NTL_RETURN(FALSE);
				}
			}

			RwUInt32 uiResultCode;
			if (!GetNtlWorldConcept()->CanUseSkill(hNewTargetSerialId, uiResultCode))
			{
				CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), Logic_GetResultCodeString(uiResultCode, "").c_str()); // not sure
				NTL_RETURN(FALSE);
			}

			RwReal fSkillRange = Logic_GetSkillUseRange(pSkillTbl);

			CNtlSLEventGenerator::ActionMapSkillUse(pSobSkill->GetOwnerID(), TRUE, hNewTargetSerialId, m_pSobObj->GetSerialID(),
				fSkillRange, pSobSkill->GetSlotIdx(), (RwUInt8)uiRPBonusType, pSkillTbl->tblidx);
		}
		break;

		case 2:
		{
			if (pStatus)
			{
				CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_CANT_DO_THAT_TO_FREE_PVP_ZONE_TARGET");
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
				NTL_RETURN(FALSE);
			}

			CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "DST_SKILL_PLEASE_SELECT_TARGET", FALSE);
			StartSkillSelectMode();
			NTL_RETURN(FALSE);
		}
		break;

		case 3:
		{
			if (pStatus == false)
			{
				CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "DST_SKILL_PLEASE_SELECT_TARGET", FALSE);
				StartSkillSelectMode();
				NTL_RETURN(FALSE);
			}

			CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_INVALID_TARGET_APPOINTED");
			CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
			NTL_RETURN(FALSE);
		}
		break;

		case 4:
		{
			if (pStatus)
			{
				CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "DST_SKILL_OCCLUSION_CHECK", FALSE);
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
				NTL_RETURN(FALSE);
			}

			CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "DST_SKILL_PLEASE_SELECT_TARGET", FALSE);
			StartSkillSelectMode();
			NTL_RETURN(FALSE);
		}
		break;

		case 5:
		{
			if (pStatus)
			{
				NTL_RETURN(FALSE);
			}

			CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "DST_SKILL_PLEASE_SELECT_TARGET", FALSE);
			StartSkillSelectMode();
			NTL_RETURN(FALSE);
		}
		break;
	}

	NTL_RETURN(TRUE);
}

BYTE CNtlSobSkillIcon::AppointTargetCheck(sSKILL_TBLDAT * pSkillTbldat, SERIAL_HANDLE hTargetSerialId, SERIAL_HANDLE& rhTargetSerialId)
{
	NTL_ASSERT(pSkillTbldat, "CNtlSobSkillIcon::AppointTargetCheck => pSkillTbldat is null !!!");

	CNtlSobAvatar* pAvatar = CNtlSLGlobal::GetInstance()->GetSobAvatar();

	NTL_ASSERT(pAvatar, "CNtlSobSkillIcon::AppointTargetCheck => pAvatar is null !!!");
	DBO_WARNING_MESSAGE("pSkillTbldat->byAppoint_Target: " << (int)pSkillTbldat->byAppoint_Target);
	rhTargetSerialId = hTargetSerialId;
	if (pSkillTbldat->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF)
	{
		if (hTargetSerialId != pAvatar->GetSerialID())
			rhTargetSerialId = pAvatar->GetSerialID();
	}
	else
	{
		if (pSkillTbldat->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_UNKNOWN)
			return 0;

		CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(hTargetSerialId);
		if (pTargetActor)
		{
			if (pTargetActor->GetClassID() == SLCLASS_PLAYER)
			{
				CNtlSobPlayerAttr* pTargetPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pTargetActor->GetSobAttr());

				if (pTargetPlayerAttr->GetIsInFreePvpZone())
				{
					CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pAvatar->GetSobAttr());

					if (pPlayerAttr->GetIsInFreePvpZone() == false)
						return 2;
				}
			}
		}
		
		if (Dbo_IsHarmfulEffectType(pSkillTbldat->bySkill_Active_Type))
		{
			if (hTargetSerialId == INVALID_SERIAL_ID)
				return 5;

			if (!ApplyTargetCheck(pSkillTbldat, hTargetSerialId))
				return 3;

			if (Logic_isEnemyTargetOcclusion(hTargetSerialId))
				return 4;

			if (pTargetActor == NULL)
				return 3;

		//	if (!sub_4556C0(v31)) // to do
		//		return 3;

			if (pTargetActor->GetClassID() == SLCLASS_TRIGGER_OBJECT || pTargetActor->GetClassID() == SLCLASS_EVENT_OBJECT || pTargetActor->GetClassID() == SLCLASS_DYNAMIC_OBJECT)// not sure
				return 3;
		}
		else if (!Logic_IsSkillSystemEffectCode(pSkillTbldat->tblidx, ACTIVE_WARP_STUN) && !Logic_IsSkillSystemEffectCode(pSkillTbldat->tblidx, ACTIVE_RESCUE) && !ApplyTargetCheck(pSkillTbldat, hTargetSerialId))
		{
			return 3;
		}
	}

	return 1;
}

void CNtlSobSkillIcon::SetSkillIconState( RwUInt8 byIconState, RwUInt8 byRpBonusType, RwBool bSpecialCoolTimeCheck /* = FALSE */, RwReal fCoolTime /* = 0.0f  */ )
{
	CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( m_pSobObj->GetSobAttr() );
	sSKILL_TBLDAT* pSkillData = pSkillAttr->GetSkillTbl();

	// 스킬 쿨타임을 서버가 직접 제어하는 경우. 서버에서 새로 패킷이 날라오게 되어 있다. 
	if( bSpecialCoolTimeCheck && pSkillData->dwFunction_Bit_Flag & SKILL_FUNCTION_FLAG_SPECIAL_COOL_TIME_RULE )
		return;

	m_fMaxCoolTime = (RwReal)pSkillData->wCool_Time;

	if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
	{
		CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( m_pSobObj->GetOwnerID() ) );
		m_fMaxCoolTime *= Logic_GetCoolingTimeFactor( pActor );
	}

	if( byRpBonusType == DBO_RP_BONUS_TYPE_COOL_TIME_MINUS )
	{
		for( RwInt32 i = 0 ; i < DBO_MAX_RP_BONUS_COUNT_PER_SKILL ; ++i )
		{
			if( pSkillData->abyRpEffect[i] == DBO_RP_BONUS_TYPE_COOL_TIME_MINUS )
			{
				m_fMaxCoolTime = m_fMaxCoolTime - pSkillData->afRpEffectValue[i];
				if( m_fMaxCoolTime <= 0.0f )
					m_fMaxCoolTime = 0.0f;
				break;
			}
		}
	}	

	CNtlSobIcon::SetIconState( byIconState, fCoolTime );
}

RwBool CNtlSobSkillIcon::ApplyTargetCheck(sSKILL_TBLDAT* pData, RwUInt32 hTargetSerial )
{
	if( hTargetSerial == INVALID_SERIAL_ID )
	{
		return FALSE;			
	}
	else
	{
		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hTargetSerial );
		
		if( !pSobObj )
		{
			return FALSE;
		}

		if( pData->byApply_Range == DBO_SKILL_APPLY_RANGE_SINGLE )
		{
			if( pSobObj->GetClassID() == SLCLASS_AVATAR )
			{
				if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_SELF ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_PARTY ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY || 
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
					return TRUE;
				else
					return FALSE;
			}
			else if( pSobObj->GetClassID() == SLCLASS_NPC )							 
			{
				if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_NPC ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
					return TRUE;
				else
					return FALSE;
			}
			else if( pSobObj->GetClassID() == SLCLASS_MONSTER )
			{
				if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ENEMY ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
					pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_MOB)
					return TRUE;
				else
					return FALSE;
			}
			else if( pSobObj->GetClassID() == SLCLASS_PLAYER || pSobObj->GetClassID() == SLCLASS_PET )
			{
				if( GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor( NULL, hTargetSerial ) )
				{
					if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ENEMY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY )
						return TRUE;
					else
						return FALSE;
				}
				else
				{
					if( pSobObj->GetClassID() == SLCLASS_PET )
					{
						if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
							pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY )
							return TRUE;
						else
							return FALSE;		
					}
					else
					{
						if( Logic_IsMyPartyMember( hTargetSerial ) )
						{
							if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
								pData->byApply_Target == DBO_SKILL_APPLY_TARGET_PARTY ||
								pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY )
								return TRUE;
							else 
								return FALSE;
						}
						else
						{
							if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
								pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY )
								return TRUE;
							else 
								return FALSE;
						}								
					}
				}
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNtlSobSkillIcon::StartSkillSelectMode()
{
	if (CNtlSLGlobal::m_bSelectSkillMode)
	{
		CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
	}

	CNtlSLEventGenerator::ActionSkillSelectMode(true, m_pSobObj->GetSerialID());
}
