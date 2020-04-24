#include "precomp_ntlsimulation.h"
#include "NtlSobHTBSkillIcon.h"

// shared
#include "NtlSkill.h"
#include "HTBSetTable.h"
#include "NtlResultCode.h"
#include "NtlMovement.h"
#include "SkillTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSobAvatar.h"
#include "ResultCodeString.h"


DEFINITION_MEMORY_POOL(CNtlSobHTBSkillIcon)

CNtlSobHTBSkillIcon::CNtlSobHTBSkillIcon() 
{
	SetClassID(SLCLASS_HTBSKILL_ICON);
	SetClassName(SLCLASS_NAME_HTBSKILL_ICON);
}

CNtlSobHTBSkillIcon::~CNtlSobHTBSkillIcon() 
{

}

void CNtlSobHTBSkillIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobHTBSkillCreate* pSobCreate = reinterpret_cast<SNtlEventSobHTBSkillCreate*>( pMsg.pData );
		
		CNtlSobHTBSkill *pSobHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>(m_pSobObj);
		CNtlSobHTBSkillAttr *pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>(pSobHTBSkill->GetSobAttr());
		sHTB_SET_TBLDAT* pHTBSkillTbl = pSobHTBSkillAttr->GetHTBSkillTbl();
		m_fMaxCoolTime = (RwReal)pHTBSkillTbl->wCool_Time;

		if( pSobCreate->uiRemainTime > 0 )
		{
			SetIconState( ICON_STATE_COOLING, m_fMaxCoolTime - ( pSobCreate->uiRemainTime / 1000.0f ) );
		}

		CreateImage(pHTBSkillTbl->szIcon_Name);
	}
}

RwBool CNtlSobHTBSkillIcon::IsDragAndDropPossible(void) 
{
	NTL_FUNCTION("CNtlSobHTBSkillIcon::IsDragAndDropPossible");

	NTL_RETURN(TRUE);
}

RwBool CNtlSobHTBSkillIcon::IsUsePossible(void)
{
	NTL_FUNCTION("CNtlSobHTBSkillIcon::IsUsePossible");

	CNtlSobHTBSkill* pHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>( GetSobObj() );
	CNtlSobHTBSkillAttr* pHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pHTBSkill->GetSobAttr() );
	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT* pSkillData = NULL;

	for( RwInt32 k = 0 ; k < NTL_HTB_MAX_SKILL_COUNT_IN_SET ; ++k )
	{
		if( pHTBSkillAttr->GetHTBSkillTbl()->aHTBAction[k].skillTblidx != INVALID_TBLIDX )
		{
			pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pHTBSkillAttr->GetHTBSkillTbl()->aHTBAction[k].skillTblidx ) );
			break;
		}
	}

	// 비히클, 스피닝어택중일때는 HTB스킬은 사용하지 못한다
	if(Logic_IsTransform(GetNtlSLGlobal()->GetSobAvatar()))
	{
		DWORD flag = pSkillData->dwTransform_Use_Info_Bit_Flag;        
		BYTE byAspectStateID = GetNtlSLGlobal()->GetSobAvatar()->GetAspectState()->sAspectStateBase.byAspectStateId;
		switch(byAspectStateID)
		{
			case ASPECTSTATE_SUPER_SAIYAN:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SUPER_SAIYAN))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_SUPER_SAIYAN;
					NTL_RETURN(FALSE);
				}
			break;
			case ASPECTSTATE_PURE_MAJIN:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_PURE_MAJIN))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_PURE_MAJIN;
					NTL_RETURN(FALSE);
				}
			break;
			case ASPECTSTATE_GREAT_NAMEK:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_GREAT_NAMEK))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_GREAT_NAMEK;
					NTL_RETURN(FALSE);
				}            
			break;
			case ASPECTSTATE_KAIOKEN:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_KAIOKEN))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_KAIOKEN;
					NTL_RETURN(FALSE);
				}
			break;
			case ASPECTSTATE_SPINNING_ATTACK:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_SPINNING_ATTACK))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_SPINNING_ATTACK;
					NTL_RETURN(FALSE);
				}
			break;
			case ASPECTSTATE_VEHICLE:
				if(!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_VEHICLE))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_VEHICLE;
					NTL_RETURN(FALSE);
				}
			break;
			case ASPECTSTATE_ROLLING_ATTACK:
				if (!(flag & SKILL_TRANSFORM_USE_INFO_FLAG_ABLE_WHEN_ROLLING_ATTACK))
				{
					m_wErrorMsg = GAME_SKILL_CANT_USE_SKILL_WHEN_ROLLING_ATTACK;
					NTL_RETURN(FALSE);
				}
			break;
		}
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlSobHTBSkillIcon::Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 uiRPBonusType/* = 0xFF*/, RwUInt32 hTarget/* = INVALID_SERIAL_ID*/, bool pStatus/* = false*/)
{
	NTL_FUNCTION("CNtlSobHTBSkillIcon::Use");

	if(!IsUsePossible())
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), Logic_GetResultCodeString(m_wErrorMsg, "").c_str());
		NTL_RETURN(FALSE);
	}

	//-----------------------------------------------------------------
	// casting 검사.

	if(m_byIconState == ICON_STATE_COOLING)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_READY_TO_BE_CAST");
		NTL_RETURN(FALSE);
	}

	//-----------------------------------------------------------------
	// target 유효 검사.

	// 자기 자신을 click하고 사용했을 경우.
	if(m_pSobObj->GetOwnerID() == hTargetSerialId)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_INVALID_TARGET_APPOINTED");
		NTL_RETURN(FALSE);
	}

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hTargetSerialId);
	if(pSobObj == NULL )
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NO_TARGET_APPOINTED");
		NTL_RETURN(FALSE);
	}

	if(!pSobObj->CanHurted())
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_CHAR_IS_WRONG_STATE");
		NTL_RETURN(FALSE);
	}
	


	CNtlSobHTBSkill *pSobHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>(m_pSobObj);
	CNtlSobHTBSkillAttr *pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>(pSobHTBSkill->GetSobAttr());
	sHTB_SET_TBLDAT* pHTBSkillTbl = pSobHTBSkillAttr->GetHTBSkillTbl();

	NTL_ASSERT(pHTBSkillTbl != NULL, "CNtlSobHTBSkillIcon::Use => HTB skill table pointer is null !!!");


	// -----------------------------------------------------------------
	// EP/RP check.
	CNtlSob *pOwnerSobObj = GetNtlSobManager()->GetSobObject(m_pSobObj->GetOwnerID());
	RwUInt32 uiCurrEp		= Logic_GetEp(pOwnerSobObj);
	RwUInt32 uiCurrRpStock	= Logic_GetRpStock(pOwnerSobObj);

	if(uiCurrEp < (RwUInt32)pHTBSkillTbl->wNeed_EP)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_ENOUGH_EP");
		NTL_RETURN(FALSE);
	}
	
 	if(uiCurrRpStock < (RwUInt32)DBO_HTB_PREREQUISITE_RP_BALL)
 	{
 		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_SKILL_NOT_ENOUGH_RP_BALL");
 		NTL_RETURN(FALSE);
 	}

	//-----------------------------------------------------------------
	// follow range check.

	RwReal fSkillRange = 0;
	for(RwInt32 i = 0; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET; ++i)
	{
		if(pHTBSkillTbl->aHTBAction[i].skillTblidx != INVALID_TBLIDX)
		{
			CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pHTBSkillTbl->aHTBAction[i].skillTblidx) );
			fSkillRange = Logic_GetSkillUseRange(pSkillTblData);
			break;
		}
	}

	//-----------------------------------------------------------------
	// event 보내기.

	CNtlSLEventGenerator::ActionMapHTBUse(pSobHTBSkill->GetOwnerID(), hTargetSerialId, pSobHTBSkill->GetSerialID(),
											fSkillRange, pSobHTBSkill->GetSlotIdx());

	NTL_RETURN(TRUE);
}

void CNtlSobHTBSkillIcon::SetIconState(RwUInt8 byIconState, RwReal fCoolTIme /* = 0.0f */)
{
	CNtlSobHTBSkillAttr* pHTBAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( m_pSobObj->GetSobAttr() );
	sHTB_SET_TBLDAT* pHTBData = pHTBAttr->GetHTBSkillTbl();

	m_fMaxCoolTime = (RwReal)pHTBData->wCool_Time;

	CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( m_pSobObj->GetOwnerID() ) );
	m_fMaxCoolTime *= Logic_GetCoolingTimeFactor( pActor );

	CNtlSobIcon::SetIconState( byIconState, fCoolTIme );
}
