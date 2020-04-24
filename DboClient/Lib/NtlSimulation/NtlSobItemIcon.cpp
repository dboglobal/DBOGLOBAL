#include "precomp_ntlsimulation.h"
#include "NtlSobItemIcon.h"

// shared
#include "ItemTable.h"
#include "NtlSkill.h"
#include "SkillTable.h"
#include "NtlResultCode.h"
#include "NtlMovement.h"
#include "UseItemTable.h"
#include "TableContainer.h"
#include "SystemEffectTable.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLDef.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlInventory.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"



DEFINITION_MEMORY_POOL(CNtlSobItemIcon)

CNtlSobItemIcon::CNtlSobItemIcon() 
{
	SetClassID(SLCLASS_ITEM_ICON);
	SetClassName(SLCLASS_NAME_ITEM_ICON);
}

CNtlSobItemIcon::~CNtlSobItemIcon() 
{
	
}

void CNtlSobItemIcon::Destroy(void)
{
	
}

void CNtlSobItemIcon::Update( RwReal fElapsed )
{
	if( Logic_IsCoolTimeUpdate( reinterpret_cast<CNtlSobActor*>( GetNtlSLGlobal()->GetSobAvatar() ) ) )
	{
		if( m_byIconState == ICON_STATE_COOLING )
		{
			m_fCoolTime += fElapsed*m_fCollingUpdateFactor;
			
			if( m_fCoolTime >= m_fMaxCoolTime )
			{
				m_fCoolTime = m_fMaxCoolTime;
				SetIconState( ICON_STATE_USEABLE );			

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pSobObj->GetSobAttr() );
				if( !pSobItemAttr->GetDurationType() )
					GetNtlSobManager()->RemoveUpdateQueue( m_pSobObj );
			}
		}
	}	
}

void CNtlSobItemIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobItemCreate *pSobItemCreate = reinterpret_cast<SNtlEventSobItemCreate*>(pMsg.pData);
		{
			CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
			CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
			
			if( pSobItemCreate->bNeedToIdentify )
			{
				CreateImage( UNIDENTIFIED_ICON_NAME );
			}
			else
			{
				sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();

				if(pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
					CreateImage(SEALED_ICON_NAME);
				else
					CreateImage(pItemTbl->szIcon_Name);

				if (pItemTbl->Use_Item_Tblidx == INVALID_TBLIDX)
					return;

				CUseItemTable *pUseItemTbl = API_GetTableContainer()->GetUseItemTable();
				sUSE_ITEM_TBLDAT *pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>(pUseItemTbl->FindData(pItemTbl->Use_Item_Tblidx));
				if (pUseItemTblData)
				{
					NTL_ASSERT(GetNtlSLGlobal()->GetSobAvatar(), "Not exist sob avatar instance");

					stCOOLTIMEDATA* pCooltimeData = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetFirstItemCooltimeInfo(pUseItemTblData->dwCool_Time_Bit_Flag);

					if (pCooltimeData)
					{
						if (pCooltimeData->fCoolTime != 0.0f)
						{
							SetIconState(CNtlSobIcon::ICON_STATE_COOLING, pCooltimeData->fCoolTime, pCooltimeData->fCurrentTime);
						}
					}
				}
			}			
		}
	}
	else if(pMsg.Id == g_EventSobItemUpdate)
	{
		SNtlEventSobItemUpdate* pSobItemUpdate = reinterpret_cast<SNtlEventSobItemUpdate*>(pMsg.pData);
		
		// 이때는 Attr보다 먼저 Update 됨.
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
		CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
				
		// 이전엔 ItemUnIdentified였다가 풀렸을때.
		if( pSobItemAttr->IsNeedToIdentify() && !pSobItemUpdate->bNeedToIdentify )
		{
			sITEM_TBLDAT* pItemData = reinterpret_cast<sITEM_TBLDAT*>( API_GetTableContainer()->GetItemTable()->FindData( pSobItemUpdate->uiItemTblId ) );
			CreateImage(pItemData->szIcon_Name);
		}
	}
	else if( pMsg.Id == g_EventSobWarehouseItemMove || pMsg.Id == g_EventSobWarehouseItemStackMove )
	{
		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( m_pSobObj );
		if( pSobItem->GetItemPosition() != CNtlSobItem::SIP_BAG_CHILD )
		{
			CNtlSLEventGenerator::SobDeleteQuickSlotIcon( m_pSobObj->GetSerialID() );
		}
	}
	else if (pMsg.Id == g_EventSobItemRestrictionUpdate)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

		SNtlEventSobItemRestrictionUpdate* pSobItemUpdate = reinterpret_cast<SNtlEventSobItemRestrictionUpdate*>(pMsg.pData);

		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
		CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

		// if item get sealed update texture
		if (pSobItemUpdate->byRestrictState == ITEM_RESTRICT_STATE_TYPE_SEAL)
		{
			CreateImage(SEALED_ICON_NAME);
		}
		//if item get unsealed, update texture
		else if (pSobItemUpdate->byRestrictState != ITEM_RESTRICT_STATE_TYPE_SEAL && pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
		{
			sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();

			CreateImage(pItemTbl->szIcon_Name);
		}
		else
		{
			DBO_WARNING_MESSAGE("unknown..");
		}
	}
}

RwBool CNtlSobItemIcon::IsDragAndDropPossible(void) 
{
	return TRUE; 
}

RwBool CNtlSobItemIcon::IsUsePossible(void)
{
	NTL_FUNCTION("CNtlSobItemIcon::IsUsePossible");

	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
	CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();

	if( pItemTbl == NULL )
		NTL_RETURN( FALSE );

	if(pItemTbl->Use_Item_Tblidx == INVALID_TBLIDX)
	{
		NTL_RETURN(FALSE);
	}

	sUSE_ITEM_TBLDAT* pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>(API_GetTableContainer()->GetUseItemTable()->FindData(pItemTbl->Use_Item_Tblidx));
	if (!pUseItemTblData)
		NTL_RETURN(FALSE);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sCHARSTATE* pState = pAvatar->GetServerFullState();
	if (!pState)
	{
		m_wErrorMsg = GAME_ITEM_CANT_USE_NOW;
		NTL_RETURN(FALSE);
	}

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());

	BYTE byCharState = pState->sCharStateBase.byStateID;
	BYTE aspectStateID = pAvatar->GetAspectState()->sAspectStateBase.byAspectStateId;
	QWORD qwCurCharCondition = pState->sCharStateBase.dwConditionFlag;
	bool bIsSwimming = pBeData->GetMoveStuff()->byFormFlag == NTL_MOVE_FLAG_SWIM;
	bool bIsJumping = pBeData->GetJumpStuff()->eState != JUMPSTATE_NONE;
	BYTE airState = pState->sCharStateBase.eAirState;

	bool bValidState = IsValidStateToUseItem(pItemTbl->tblidx, pUseItemTblData->wNeed_State_Bit_Flag, byCharState,
		aspectStateID, qwCurCharCondition, bIsSwimming, bIsJumping,
		airState);

	if (bValidState == false)
	{
		m_wErrorMsg = GAME_ITEM_CANT_USE_NOW;
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlSobItemIcon::IsStackable(void)
{
	CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pSobObj->GetSobAttr() );
	sITEM_TBLDAT* pData = pAttr->GetItemTbl();

	if( pData == NULL )
		return FALSE;

	if( pData->byMax_Stack <= 1 )
		return FALSE;

	return TRUE;
}

RwBool CNtlSobItemIcon::Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 param1/* = 0xFF*/, RwUInt32 param2/* = INVALID_SERIAL_ID*/, bool pStatus/* = false*/)
{
	NTL_FUNCTION("CNtlSobItemIcon::Use");

	if(!IsUsePossible())
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_ITEM_NOT_FOR_USE");
		NTL_RETURN(FALSE);
	}

	if( !Logic_CanRideOnVehicle( m_pSobObj->GetOwnerID(), reinterpret_cast<CNtlSobItem*>(m_pSobObj) ) )
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_VEHICLE_END_BY_FORBIDDEN_ZONE");
		NTL_RETURN(FALSE);
	}

	//-----------------------------------------------------------------
	// casting 검사.

	if(m_byIconState == ICON_STATE_COOLING)
	{
		CNtlSLEventGenerator::SysMsg(m_pSobObj->GetOwnerID(), "GAME_ITEM_NOT_READY_TO_BE_USED");
		NTL_RETURN(FALSE);
	}

	//-----------------------------------------------------------------
	// target 유효 검사.
	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
	CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();
	NTL_ASSERT(pItemTbl != NULL, "CNtlSobItemIcon::Use => item table pointer is null !!!");

	CUseItemTable *pUseItemTbl = API_GetTableContainer()->GetUseItemTable();
	NTL_ASSERT(pUseItemTbl != NULL, "CNtlSobItemIcon::Use => use item table pointer is null !!!");

	sUSE_ITEM_TBLDAT *pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTbl->FindData(pItemTbl->Use_Item_Tblidx) );
	if (!pUseItemTblData)
		NTL_RETURN(FALSE);
	
	//-----------------------------------------------------------------
	// appoint target 유효 검사.
	//-----------------------------------------------------------------
	// self
	RwUInt8 byAppointTarget = pUseItemTblData->byAppoint_Target;
	if(byAppointTarget == DBO_SKILL_APPOINT_TARGET_SELF)
	{
		if(m_pSobObj->GetOwnerID() != hTargetSerialId)
		{
			hTargetSerialId = m_pSobObj->GetOwnerID();
		}
	}
	// target
	else if(byAppointTarget == DBO_SKILL_APPOINT_TARGET_TARGET)
	{
		if(hTargetSerialId == INVALID_SERIAL_ID)
		{
			CNtlSLEventGenerator::SysMsg( m_pSobObj->GetOwnerID(), "DST_ITEM_NO_TARGET_APPOINTED", FALSE );
			NTL_RETURN(FALSE);
		}
	}
	// point
	else
	{
		NTL_RETURN(FALSE);
	}
	
	//-----------------------------------------------------------------
	// Apply target scan. (I do not test at all yet)
	//-----------------------------------------------------------------

	RwUInt8 byApplyTarget = pUseItemTblData->byApply_Target;
	// self
	if(byApplyTarget == DBO_SKILL_APPLY_TARGET_SELF)
	{
		// check 안함.
	}
	// enemy
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ENEMY)
	{
		// check 안함.
	}
	// alliance
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ALLIANCE)
	{
		// check 안함.
	}
	// party
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_PARTY)
	{
	}
	else
	{
		DBO_WARNING_MESSAGE("Unknown apply target type: " << byApplyTarget);
	}

	//-----------------------------------------------------------------
	// Lp, Ep, Rp Max 검사. 
	//-----------------------------------------------------------------
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
	CSystemEffectTable *pSystemEffectTbl = API_GetTableContainer()->GetSystemEffectTable();

	NTL_ASSERT(pSystemEffectTbl != NULL, "CNtlSobItemIcon::Use => system effect table pointer is null !!!");

	struct sEFFECT_RESULT
	{
		sEFFECT_RESULT(VOID)
		{
			bResult = FALSE;
			uiSystemEffect = INVALID_DWORD;
		}

		RwBool		bResult;
		RwUInt32	uiSystemEffect;
	};

	sEFFECT_RESULT sResult[NTL_MAX_EFFECT_IN_ITEM];
	RwBool bLPEPRPUseCheck = FALSE;

	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
	{
		sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTbl->FindData( pUseItemTblData->aSystem_Effect[i] ) );

		if( !pSystemEffectData )
		{
			sResult[i].bResult = FALSE;
			sResult[i].uiSystemEffect = INVALID_DWORD;
			continue;
		}
		
		if( pSystemEffectData->effectCode == ACTIVE_DIRECT_HEAL )
		{
			sResult[i].uiSystemEffect = ACTIVE_DIRECT_HEAL;

			if( pAvatarAttr->GetLp() >= pAvatarAttr->GetMaxLp() )
				sResult[i].bResult = FALSE;
			else
			{
				sResult[i].bResult = TRUE;
				bLPEPRPUseCheck = TRUE;
			}
		}
		else if( pSystemEffectData->effectCode == ACTIVE_EP_UP )
		{
			sResult[i].uiSystemEffect = ACTIVE_EP_UP;

			if( pAvatarAttr->GetEp() >= pAvatarAttr->GetMaxEp() )
				sResult[i].bResult = FALSE;
			else
			{
				sResult[i].bResult = TRUE;
				bLPEPRPUseCheck = TRUE;
			}
		}
		else if( pSystemEffectData->effectCode == ACTIVE_RP_UP )
		{
			sResult[i].uiSystemEffect = ACTIVE_RP_UP;
			SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

			if (pAvatarInfo->sCharPf.byCurRPBall >= pAvatarInfo->byMaxRpStock)
			{
				sResult[i].bResult = FALSE;
			}
			else
			{
				sResult[i].bResult = TRUE;
				bLPEPRPUseCheck = TRUE;
			}
		}
		else // 그외의 Effect가 있는 경우 무조건 사용 가능.
		{
			bLPEPRPUseCheck = TRUE;
			sResult[i].bResult = TRUE;
			sResult[i].uiSystemEffect = pSystemEffectData->effectCode;
		}
	}

	if( !bLPEPRPUseCheck )
	{
		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
		{
			if( !sResult[i].bResult && sResult[i].uiSystemEffect != INVALID_DWORD )
			{
				switch( sResult[i].uiSystemEffect )
				{
				case ACTIVE_DIRECT_HEAL: CNtlSLEventGenerator::SysMsg( m_pSobObj->GetOwnerID(), "DST_ITEM_CAN_NOT_USE_MAX_LP", FALSE ); break;
				case ACTIVE_EP_UP: CNtlSLEventGenerator::SysMsg( m_pSobObj->GetOwnerID(), "DST_ITEM_CAN_NOT_USE_MAX_EP", FALSE ); break;
				case ACTIVE_RP_UP: CNtlSLEventGenerator::SysMsg( m_pSobObj->GetOwnerID(), "DST_ITEM_CAN_NOT_USE_MAX_RP_BALL", FALSE ); break;
				}
			}
		}

		NTL_RETURN( FALSE );
	}

	CNtlSobItem *pParentSobItem = pSobItem->GetParentItem();
	NTL_ASSERT(pParentSobItem, "CNtlSobItemIcon::Use => item parent is null");

	CNtlSLEventGenerator::ActionMapItemUse(pSobItem->GetOwnerID(), hTargetSerialId, (RwUInt8)(pParentSobItem->GetItemSlotIdx() + CONTAINER_TYPE_BAG1),
		(RwUInt8)pSobItem->GetItemSlotIdx(), (RwUInt8)(pUseItemTblData->fCasting_Time), INVALID_HOBJECT, INVALID_BYTE, INVALID_BYTE);

	NTL_RETURN(TRUE);
}

void CNtlSobItemIcon::SetIconState( RwUInt8 byIconState, RwReal fCoolTIme )
{
	if( byIconState == ICON_STATE_USEABLE )
	{
		if( m_byIconState == ICON_STATE_COOLING )
		{
			CNtlSLEventGenerator::SobCooling( GetSobObj()->GetSerialID(), FALSE );				
		}
	}
	else if( byIconState == ICON_STATE_COOLING )
	{
		m_fCoolTime = fCoolTIme;

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pSobObj->GetSobAttr() );
		if( !pSobItemAttr->GetDurationType() )
			GetNtlSobManager()->AddUpdate( m_pSobObj );

		CNtlSLEventGenerator::SobCooling( GetSobObj()->GetSerialID(), TRUE );			
	}

	m_byIconState = byIconState;
}

void CNtlSobItemIcon::SetIconState( RwUInt8 byIconState, RwReal fMaxCoolTime, RwReal fCoolTime )
{
	CNtlSobIcon::SetIconState( byIconState, fMaxCoolTime, fCoolTime );
}