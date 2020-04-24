#include "precomp_dboclient.h"
#include "ItemChangeBattleAttributeGui.h"

// shared
#include "NtlCharacter.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"
#include "NtlCameraController.h"
#include "NtlWorldConceptNPCCommu.h"

// Dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "GUISoundDefine.h"


#define ITEMUPGRADEGUI_SUCCESS_FLASHFILE		"ItemUpgrade_Success.swf"
#define ITEMUPGRADEGUI_FAIL_FLASHFILE			"ItemUpgrade_Fail.swf"
#define ITEMUPGRADEGUI_PROCESS_FLASHFILE		"ItemUpgrade_Effect.swf"



CItemChangeBattleAttributeGui::CItemChangeBattleAttributeGui(VOID)
{
	Init();
}

CItemChangeBattleAttributeGui::CItemChangeBattleAttributeGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CItemChangeBattleAttributeGui::~CItemChangeBattleAttributeGui(VOID)
{

}

VOID CItemChangeBattleAttributeGui::Init(VOID)
{
	m_pUpgradeItem = NULL;
	m_pOfferItem = NULL;
	m_uiOfferItemParentSlotIdx = INVALID_INDEX;
	m_uiOfferItemSlotIdx = INVALID_INDEX;

	m_nOfferItemBattleSlotIdx = INVALID_INDEX;

	m_nLSelectedSlotIdx = INVALID_INDEX;
	m_nRSelectedSlotIdx = INVALID_INDEX;
	m_nMouseOnIdx = INVALID_INDEX;
	m_nPushDownIndex = INVALID_INDEX;

	for( RwInt32 i = 0; i < NUM_SLOT; ++i )
	{
		m_anFocusEffect[i] = SLOT_FOCUS_NONE;				
	}

	m_eState = STATE_NONE;

	m_fCurrentTime = 0.0f;

	m_hNPCSerial = INVALID_SERIAL_ID;
}

RwBool CItemChangeBattleAttributeGui::Create(VOID)
{
	NTL_FUNCTION( "CItemChangeBattleAttributeGui::Create" );

	if( !CNtlPLGui::Create( "gui\\ItemChangeBattleAtt.rsr", "gui\\ItemChangeBattleAtt.srf", "gui\\ItemChangeBattleAtt.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pstbInfoTitle = (gui::CStaticBox*)GetComponent("stbInfoTitle");

	m_pflsUpgradeEffect = (gui::CFlash*)GetComponent("flsUpgradeEffect");
	m_phtmlInfoText = (gui::CHtmlBox*)GetComponent("htmlUpgradeInfo");

	m_pbtnExit = (gui::CButton*)GetComponent( "btnExit" );
	m_pbtnCompound = (gui::CButton*)GetComponent( "btnCompound" );

	m_pdlgResult = (gui::CDialog*)GetComponent( "dlgResult" );
	m_pflsResult = (gui::CFlash*)GetComponent( "flsResult" );
	m_pbtnCloseResult = (gui::CButton*)GetComponent( "btnClose" );

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CItemChangeBattleAttributeGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CItemChangeBattleAttributeGui::OnMouseUp );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CItemChangeBattleAttributeGui::OnMouseMove );
	m_slotMouseOut		= m_pThis->SigMouseLeave().Connect( this, &CItemChangeBattleAttributeGui::OnMouseOut );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CItemChangeBattleAttributeGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CItemChangeBattleAttributeGui::OnMove );
	m_slotClickExitBtn  = m_pbtnExit->SigClicked().Connect( this, &CItemChangeBattleAttributeGui::OnClickExitBtn );
	m_slotClickCompoundBtn = m_pbtnCompound->SigClicked().Connect( this, &CItemChangeBattleAttributeGui::OnClickCompoundBtn );
	m_slotClickCloseResult = m_pbtnCloseResult->SigClicked().Connect( this, &CItemChangeBattleAttributeGui::OnClickCloseResult );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CItemChangeBattleAttributeGui::OnCaptureMouseDown );
	m_slotCaptureMouseWheel= m_phtmlInfoText->SigMouseWheel().Connect( this, &CItemChangeBattleAttributeGui::OnCaptureMouseWheel );
	m_slotChangeAttributeEffectEnd = m_pflsUpgradeEffect->SigMovieEnd().Connect( this, &CItemChangeBattleAttributeGui::OnChangeAttributeEffectEnd);
	m_slotChangeAttributeEffectResultEnd = m_pflsResult->SigMovieEnd().Connect(this, &CItemChangeBattleAttributeGui::OnChangeAttributeResultEffectEnd);

	// Above the stack number.
	m_pdlgResult->Raise();

	SetSlotRectHardCode();
	SetBasicUISetting();
	Show( false );

	m_pdlgResult->Show( false );

	LinkMsg(g_EventItemChangeBattleAttributeResult, 0 );
	LinkMsg( g_EventNPCDialogOpen, 0 );
	LinkMsg( g_EventDialog, 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventSobItemDelete, 0, 0x9000 );		// Because you are writing a pointer, you must come in before the object is cleared.
	LinkMsg(g_EventRegisterItemChangeBattleAttribute, 0 );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg(g_EventMsgBoxResult);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	NTL_RETURN( TRUE );
}

VOID CItemChangeBattleAttributeGui::Destroy(VOID)
{
	UnLinkMsg(g_EventItemChangeBattleAttributeResult);
	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobItemDelete );
	UnLinkMsg(g_EventRegisterItemChangeBattleAttribute);
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg(g_EventMsgBoxResult);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CItemChangeBattleAttributeGui::DestroyResultDialog(VOID)
{
	m_pdlgResult->Show( false );
	m_pflsResult->PlayMovie( FALSE );
}

VOID CItemChangeBattleAttributeGui::Update( RwReal fElapsed )
{
	if( m_eState == STATE_RESULT )
	{
		m_pflsResult->Update( fElapsed );
	}
	else if( m_eState == STATE_UPGRADE_PROCESS )
	{
		m_pflsUpgradeEffect->Update( fElapsed );
	}

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_aSlotEffect[i].Update( fElapsed );
	}

	m_TempItemSlot.Update( fElapsed );
	m_BackFocus.Update( fElapsed );
}

RwInt32 CItemChangeBattleAttributeGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
		pWorldConceptController->ChangeState(WORLD_NPC_ITEM_UPGRADE);

		RaiseTop();			

		Show( true );
		DeleteOfferItem();
		DeleteUpgradeItem();
		SetState( STATE_NONE );

		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
			m_aSlotEffect[i].SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );
		m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		GetNtlGuiManager()->AddUpdateFunc( this );		
		m_pflsUpgradeEffect->Show( false );
	}
	else
	{
		if( m_eState == STATE_PACKETWAIT )
			return -1;

		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		GetDialogManager()->SwitchBag( FALSE );

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
			GetIconMoveManager()->IconMoveEnd();

		if( m_pdlgResult->IsVisible() )
			DestroyResultDialog();

		Show( false );
		DeleteOfferItem();
		DeleteUpgradeItem();

		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
			m_aSlotEffect[i].SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );
		m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_NONE );

		Logic_CancelNpcFacing();

		GetNtlGuiManager()->RemoveUpdateFunc( this );
		m_pflsUpgradeEffect->Unload();
	}

	return 1;
}

VOID CItemChangeBattleAttributeGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventItemChangeBattleAttributeResult)
	{
		SDboItemChangeBattleAttributeResult* pData = reinterpret_cast<SDboItemChangeBattleAttributeResult*>( msg.pData );

		SetResult( pData->wResultcode, pData->byBattleAttribute );
	}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE )
			return;

		GetDialogManager()->OpenDialog( DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		GetDialogManager()->SwitchBag( TRUE );
		m_hNPCSerial = pData->hSerialId;
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );
		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE )
		{
			GetDialogManager()->CloseDialog( DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE );
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pData = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pData->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pData->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateSlotIcon();			
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUpState = (RwBool)msg.pData;
		ShowIconDestination( bPickUpState );
	}
	else if( msg.Id == g_EventSobItemDelete )
	{
		SNtlEventSobItemDelete* pData = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		if(m_pOfferItem)
		{
			if( pData->hItemSerialId == m_pOfferItem->GetSerialID() )
				DeleteOfferItem();						
		}
	}
	else if( msg.Id == g_EventRegisterItemChangeBattleAttribute)
	{
		SDboRegisterItemUpgrade* pData = reinterpret_cast<SDboRegisterItemUpgrade*>( msg.pData );

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->hSrcSerial ) );
		if( pItem )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

			if (m_pUpgradeItem == NULL)
			{
				if (IsUpgradableItem(pItemAttr))
				{
					if(SetItemSlot(pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_ITEM, FALSE))
						RaiseTop();
				}
			}
			else
			{
				if(SetItemSlot(pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_HONEST, FALSE))
					RaiseTop();
			}
		}		
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		GetDialogManager()->CloseDialog( DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE );
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		if (pEvent->strID == "DST_CHANGE_ITEMBATTLEATTR_ONE_MSG" || pEvent->strID == "DST_CHANGE_ITEMBATTLEATTR_TWO_MSG")
		{
			if (pEvent->eResult == MBR_OK)
			{
				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pUpgradeItem->GetSobAttr());
				DWORD dwPrice = Dbo_GetChargeItemBattleAttributeChange(pSobItemAttr->GetRank(), pSobItemAttr->GetItemTbl()->byNeed_Min_Level);

				if (Logic_GetZenny() >= dwPrice)
				{
					SetState(STATE_UPGRADE_PROCESS);
				}
				else
				{
					GetAlarmManager()->AlarmMessage("DST_LACK_OF_ZENNY");
					SetState(STATE_UPGRADE_READY);
				}
			}
			else
			{
				SetState(STATE_UPGRADE_READY);
			}
		}
	}
}

VOID CItemChangeBattleAttributeGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_rtSlot[SLOT_ITEM].SetRectWH( 144, 267, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_HONEST].SetRectWH( 144, 201, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_STRANGE].SetRectWH( 179, 327, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_WILD].SetRectWH( 211, 263, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_ELEGANT].SetRectWH( 109, 327, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	m_rtSlot[SLOT_FUNNY].SetRectWH( 76, 263, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );

	m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.SetRectWH( 110, 236, 101, 92 );
	m_aSlotEffect[SLOT_HONEST].m_rtEffectSlot.SetRectWH( 126, 171, 64, 72 );
	m_aSlotEffect[SLOT_STRANGE].m_rtEffectSlot.SetRectWH( 168, 317, 93, 76 );
	m_aSlotEffect[SLOT_WILD].m_rtEffectSlot.SetRectWH( 200, 244, 81, 62 );
	m_aSlotEffect[SLOT_ELEGANT].m_rtEffectSlot.SetRectWH( 61, 317, 90, 81 );
	m_aSlotEffect[SLOT_FUNNY].m_rtEffectSlot.SetRectWH( 36, 239, 84, 66 );
	m_TempItemSlot.m_rtEffectSlot = m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot;

	m_aSlotEffect[SLOT_HONEST].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfEffectHonest" ) );
	m_aSlotEffect[SLOT_STRANGE].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfEffectStrange" ) );
	m_aSlotEffect[SLOT_WILD].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfEffectWild" ) );
	m_aSlotEffect[SLOT_ELEGANT].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfEffectElegant" ) );
	m_aSlotEffect[SLOT_FUNNY].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfEffectFunny" ) );

	m_BackFocus.m_rtEffectSlot.SetRectWH( 15, 152, 290, 276 );
	m_BackFocus.m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfBackFocus" ) );

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_surSlot[i].SetRectWH( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		m_asurFocus[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
		m_asurFocus[i].SetRectWH( 0, 0, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );		
		m_asurDisableSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );
		m_asurDisableSlot[i].SetRectWH( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );				
		m_aSlotEffect[i].m_surSlotEffect.Show( FALSE );
	}
}	

VOID CItemChangeBattleAttributeGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_ITEMPROPERTY_TRANSFER" ) );
	m_pstbInfoTitle->SetText( GetDisplayStringManager()->GetString( "DST_ITEMPROPERTY_TRANSFER_INFOTITLE" ) );
	m_pbtnCompound->SetText( GetDisplayStringManager()->GetString( "DST_ITEMPROPERTY_TRANSFER" ) );	
}

RwInt32 CItemChangeBattleAttributeGui::GetChildSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		if( m_rtSlot[i].PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

// Error Notify는 여기서 처리.
bool CItemChangeBattleAttributeGui::SetItemSlot( RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd )
{
	if( m_eState == STATE_UPGRADE_PROCESS ||
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE" );
		return false;
	}

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerialID ) );
	NTL_ASSERT( pItem, "CItemChangeBattleAttributeGui::SetITemSlot : pItem is must be present!" );

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	RwInt32 nSrcPlace = eSrcplace;

	if( nSrcPlace == PLACE_ITEMUPGRADE )
	{
		RwInt32 nSrcSlotIdx = uiSrcSlotIdx;

		if( uiSrcSlotIdx == uiDestSlotIdx )
		{
			if( bNeedToIconMoveEnd )
				GetIconMoveManager()->IconMoveEnd();

			return true;
		}

		if( uiDestSlotIdx == SLOT_ITEM )
		{
			GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM" );
			return false;
		}
		else
		{
			SetOfferItem( uiDestSlotIdx, pItem );

			if (m_eState == STATE_UPGRADE_READY)
				SetState(STATE_UPGRADE_READY);
		}
	}
	else if( nSrcPlace == PLACE_BAG )
	{
		if( m_eState == STATE_NONE )
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( !IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM" );
					return false;
				}

				SetUpgradeItem( pItem );
				SetState(STATE_UPGRADE_READY);
			}
			else 
			{
				if (m_pUpgradeItem == NULL)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEMPROPERTY_NOTIFY_TRANSITEM_NONE");
					return false;
				}

				if( !IsValidOfferItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEMPROPERTY_NOTIFY_TRANSITEM_NEED_LEVEL_ERR" );
					return false;
				}


				DeleteOfferItem(true);

				SetOfferItem( uiDestSlotIdx, pItem );
			}			
		}
		else if( m_eState == STATE_UPGRADE_READY)
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if( IsUpgradableItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX" );
					return false;
				}
				else
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM" );
					return false;
				}
			}
			else
			{
				if( !IsValidOfferItem( pItemAttr ) )
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEMPROPERTY_NOTIFY_TRANSITEM_NEED_LEVEL_ERR" );
					return false;
				}

				DeleteOfferItem(true);

				SetOfferItem( uiDestSlotIdx, pItem );		
			}				
		}
		else // Upgrade Ready
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX" );
			}
			else
			{
				GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE" );
			}

			return false;
		}	

		CDboEventGenerator::DialogEvent( DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx() );
	}
	else
	{
		return false;
	}	

	// 
	UpdateSlotIcon();

	// 아이콘 무브 완료
	if( bNeedToIconMoveEnd )
		GetIconMoveManager()->IconMoveEnd();

	return true;
}

VOID CItemChangeBattleAttributeGui::UpdateSlotIcon(VOID)
{
	// Info Wnd 변경
	if( m_nMouseOnIdx >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE )
	{
		ShowInfoWnd( TRUE, m_nMouseOnIdx );					
	}

	// UpgradeReady상태시 스카우터 파츠 관련해서 바뀌었으면 다시 세팅
	if( m_eState == STATE_UPGRADE_READY )
	{
		SetState( STATE_UPGRADE_READY );
	}
}

VOID CItemChangeBattleAttributeGui::SetUpgradeItem( CNtlSobItem* pUpgradeItem )
{
	if( !pUpgradeItem )
	{
		NTL_ASSERTFAIL( "CItemChangeBattleAttributeGui::SetUpgrade : Slot Fail or Invalid Icon");
		return;
	}

	m_pUpgradeItem = pUpgradeItem;
	m_surSlot[SLOT_ITEM].SetTexture( (gui::CTexture*)pUpgradeItem->GetIcon()->GetImage() );	

	SetAttributeSlotEffect( SLOT_ITEM, TRUE );
}

VOID CItemChangeBattleAttributeGui::SetOfferItem( RwInt32 nSlotIdx, CNtlSobItem* pOfferItem )
{
	if( !pOfferItem || ! IsValidIdx( nSlotIdx ) )
	{
		NTL_ASSERTFAIL( "CItemChangeBattleAttributeGui::SetOfferItem : Slot Fail or Invalid Icon");
		return;
	}

	// 아이템 업그레이드 내부에서 움직일때 이전 이펙트를 지운다. DeleteHoipoiStone은 업그레이드 창에서 사라질때만 호출된다. 
	if(m_pOfferItem)
	{
		m_surSlot[nSlotIdx].UnsetTexture();
		SetAttributeSlotEffect(m_nOfferItemBattleSlotIdx, FALSE );
	}

	m_pOfferItem = pOfferItem;

	m_uiOfferItemParentSlotIdx = m_pOfferItem->GetParentItemSlotIdx();
	m_uiOfferItemSlotIdx = m_pOfferItem->GetItemSlotIdx();

	m_nOfferItemBattleSlotIdx = nSlotIdx;
	m_surSlot[nSlotIdx].SetTexture( (gui::CTexture*)pOfferItem->GetIcon()->GetImage() );

	SetAttributeSlotEffect( nSlotIdx, TRUE );	

	Logic_PlayGUISound( GSD_SYSTEM_ITEM_LIGHT_ON );
}

VOID CItemChangeBattleAttributeGui::DeleteUpgradeItem(VOID)
{
	if (m_pUpgradeItem)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG,
			m_pUpgradeItem->GetParentItemSlotIdx(), m_pUpgradeItem->GetItemSlotIdx());

		m_surSlot[SLOT_ITEM].SetTexture((gui::CTexture*)NULL);
		m_pUpgradeItem = NULL;

		SetAttributeSlotEffect(SLOT_ITEM, FALSE);
	}

	if (m_eState != STATE_RESULT)
	{
		SetState(STATE_NONE);
	}
}

VOID CItemChangeBattleAttributeGui::DeleteOfferItem(bool bReplace)
{
	if(m_uiOfferItemParentSlotIdx != INVALID_INDEX)
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, 
			m_uiOfferItemParentSlotIdx, m_uiOfferItemSlotIdx);

		m_pOfferItem = NULL;
		m_uiOfferItemParentSlotIdx = INVALID_INDEX;
		m_uiOfferItemSlotIdx = INVALID_INDEX;
	
		SetAttributeSlotEffect( m_nOfferItemBattleSlotIdx, FALSE );
		m_surSlot[m_nOfferItemBattleSlotIdx].SetTexture( (gui::CTexture*)NULL );
		m_nOfferItemBattleSlotIdx = -1;

		// Info
		if (m_pUpgradeItem && bReplace == false)
		{
			const WCHAR* pInfo = GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_MSG_MAXLEVEL_ITEM_IN");
			m_phtmlInfoText->SetHtmlFromMemory(pInfo, wcslen(pInfo));
		}
	}
}

// peessiupgrade : If it disappears, it should be received and processed.
VOID CItemChangeBattleAttributeGui::SetResult(WORD wResultcode, RwUInt8 byBattleAttribute)
{
	NTL_ASSERT( m_pUpgradeItem, "CItemChangeBattleAttributeGui::HandleEvents : UpgradeItem Must be Present!" );

	// execption.. --;; 에러메시지는 패킷핸들러에서 처리된다.
	if(wResultcode != GAME_SUCCESS )
	{
		SetState(STATE_UPGRADE_READY);
		return;
	}



	if (byBattleAttribute == BATTLE_ATTRIBUTE_NONE ||	// check if fail without offer item
		(m_uiOfferItemParentSlotIdx != INVALID_INDEX && byBattleAttribute != GetAttributeFromSlot()))	// check if fail when we offer item
	{
		m_pflsResult->Load(ITEMUPGRADEGUI_FAIL_FLASHFILE);
		m_pflsResult->PlayMovie(TRUE);
	}
	else
	{
		m_pflsResult->Load( ITEMUPGRADEGUI_SUCCESS_FLASHFILE );
		m_pflsResult->PlayMovie( TRUE ); 
	}

	// remove the offer item
	DeleteOfferItem();

	SetState( STATE_RESULT );
}

VOID CItemChangeBattleAttributeGui::SetState( STATE eState )
{
	m_pbtnExit->ClickEnable( TRUE );
	m_pbtnCompound->ClickEnable( FALSE );

	DisableSlot( FALSE );

	const WCHAR* pInfo;

	switch( eState )
	{
		case STATE_NONE:
		{
			// Info
			pInfo = GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_MSG_SLOT_IS_EMPTY" );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );

			if( GetIconMoveManager()->IsActive() )
				ShowIconDestination( TRUE );			

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		break;
		case STATE_UPGRADE_READY:
		{
			// Info
			if( m_pOfferItem == NULL )
			{
				pInfo = GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_MSG_MAXLEVEL_ITEM_IN" );
				m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );			
			}
			else
			{
				pInfo = GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_MSG_BLACK_HOIPOISTONE_IN" );
				m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );			
			}

			// enable change attribute Button
			m_pbtnCompound->ClickEnable(TRUE);

			if( m_BackFocus.m_eSlotEffect != stSLOTEFFECT::SLOT_EFFECT_NONE )
				m_BackFocus.SetState( stSLOTEFFECT::SLOT_EFFECT_ON);
		}		
		break;
		case TATE_UPGRADE_CONFIRM:
		{
			DisableSlot(TRUE);

			CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pUpgradeItem->GetSobAttr());
			
			DWORD dwPrice = Dbo_GetChargeItemBattleAttributeChange(pSobItemAttr->GetRank(), pSobItemAttr->GetItemTbl()->byNeed_Min_Level);

			if (m_pOfferItem)
			{
				int nPercent = 50;
				GetAlarmManager()->FormattedAlarmMessage("DST_CHANGE_ITEMBATTLEATTR_TWO_MSG", FALSE, NULL, Logic_FormatZeni(dwPrice), nPercent);
			}
			else
			{
				int nPercent = 20;
				GetAlarmManager()->FormattedAlarmMessage("DST_CHANGE_ITEMBATTLEATTR_ONE_MSG", FALSE, NULL, Logic_FormatZeni(dwPrice), nPercent);
			}
		}
		break;
		case STATE_UPGRADE_PROCESS:
		{
			DisableSlot(TRUE);

			m_pflsUpgradeEffect->Load(ITEMUPGRADEGUI_PROCESS_FLASHFILE);

			m_pflsUpgradeEffect->RestartMovie();
			m_pflsUpgradeEffect->Show(TRUE);
		}
		break;
		case STATE_PACKETWAIT:
		{
			DisableSlot( TRUE );

			// Exit button disabled !!!
			m_pbtnExit->ClickEnable( FALSE );
		}
		break;
		case STATE_RESULT:
		{
			DisableSlot( TRUE );
			m_pdlgResult->Show( true );			
		}
		break;
	}

	m_eState = eState;
}

// peessiupgrade : Info window check
VOID CItemChangeBattleAttributeGui::ShowInfoWnd( RwBool bShow, RwInt32 nMouseOnIdx /* = -1  */ )
{
	if(nMouseOnIdx >= NUM_SLOT)
		return;

	if( bShow )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();

		if( nMouseOnIdx == SLOT_ITEM)
		{
			if( m_pUpgradeItem )
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
					m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top,
					(VOID*)GetSlotObject( nMouseOnIdx ), DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE );				
			}
			else
			{
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
					m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top,
					(VOID*)GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_INFO_ITEM_SLOT" ), DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE );
			}			
		}
		else
		{
			if (m_eState != STATE_UPGRADE_PROCESS && m_eState != STATE_PACKETWAIT && m_eState != STATE_RESULT)
			{
				if (m_pOfferItem)
				{
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
						m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top,
						(VOID*)GetSlotObject(nMouseOnIdx), DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE);
				}
				else
				{
					stINFOWND_BATTLEATTR sBattleAttr;
					sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_UPGRADE;

					switch (nMouseOnIdx)
					{
						case SLOT_HONEST: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_HONEST;	break;
						case SLOT_STRANGE: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_STRANGE; break;
						case SLOT_WILD: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_WILD; break;
						case SLOT_ELEGANT: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_ELEGANCE; break;
						case SLOT_FUNNY: sBattleAttr.bySourceWeaponAttr = BATTLE_ATTRIBUTE_FUNNY; break;
						default: return;
					}

					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE, m_rtSlot[nMouseOnIdx].left + rtScreen.left, m_rtSlot[nMouseOnIdx].top + rtScreen.top, (VOID*)&sBattleAttr, DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE);

				}
			}
		}
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );		
	}
}

void CItemChangeBattleAttributeGui::SendItemChangeBattleAttributePacket()
{
	RwUInt8 byItemPlace = m_pUpgradeItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byItemPos = m_pUpgradeItem->GetItemSlotIdx();

	RwUInt8 byAdditialItemPlace = INVALID_BYTE;
	RwUInt8 byAdditialItemPos = INVALID_BYTE;
	RwUInt8 byAdditionalAttribute = INVALID_BYTE;

	if (m_pOfferItem)
	{
		byAdditialItemPlace = m_pOfferItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
		byAdditialItemPos = m_pOfferItem->GetItemSlotIdx();

		byAdditionalAttribute = GetAttributeFromSlot();
	}


	GetDboGlobal()->GetGamePacketGenerator()->SendItemChangeBattleAttributeReq(m_hNPCSerial, byItemPlace, byItemPos, byAdditialItemPlace, byAdditialItemPos, byAdditionalAttribute);
	SetState(STATE_PACKETWAIT);
}

VOID CItemChangeBattleAttributeGui::ShowIconDestination( RwBool isPick )
{
	if( isPick )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
		{
			if( m_eState == STATE_UPGRADE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
				return;
			else
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
				NTL_ASSERT( pSobItem, "CItemChangeBattleAttributeGui::ShowIconDestination : pSobItem is must be present!" );			

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
				NTL_ASSERT( pSobItemAttr, "CItemChangeBattleAttributeGui::ShowIconDestination : pSobItemAttr is must be present!" );			

				if( m_pUpgradeItem == NULL && IsUpgradableItem(pSobItemAttr))
				{
					m_anFocusEffect[SLOT_ITEM] |= SLOT_FOCUS_CAN_MOVE;
				}
				else if( IsValidOfferItem( pSobItemAttr ) )
				{
					for( RwInt32 i = 0 ; i < NUM_SLOTEFFECT ; ++i )
					{
						m_anFocusEffect[i] = m_anFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
					}	
				}				
			}
		}
		else if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		{
			if( m_eState == STATE_UPGRADE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
				return;
			else
			{
				// Item 은 장착된후에는 Pickup이 되지 않는다.
				for( RwInt32 i = 0 ; i < NUM_SLOTEFFECT ; ++i )
				{
					m_anFocusEffect[i] = m_anFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
				}
			}			
		}
	}
	else
	{
		for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
		{
			m_anFocusEffect[i] = m_anFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
		}
	}
}

VOID CItemChangeBattleAttributeGui::DisableSlot( RwBool bDisable )
{
	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_asurDisableSlot[i].Show( bDisable );
	}
}

VOID CItemChangeBattleAttributeGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */ )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( bPush )
	{
		m_surSlot[nSlotIdx].SetRect( rtScreen.left + m_rtSlot[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].top + ICONPUSH_SIZEDIFF,
			rtScreen.left + m_rtSlot[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtSlot[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_surSlot[m_nPushDownIndex].SetRect( rtScreen.left + m_rtSlot[m_nPushDownIndex].left, rtScreen.top + m_rtSlot[m_nPushDownIndex].top,
			rtScreen.left + m_rtSlot[m_nPushDownIndex].right, rtScreen.top + m_rtSlot[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CItemChangeBattleAttributeGui::SetAttributeSlotEffect( RwInt32 nSlotIdx, RwBool bInSlot )
{
	if(nSlotIdx >= NUM_SLOT)
		return;

	if( bInSlot )
	{
		if( nSlotIdx == SLOT_ITEM )
		{
			if( !m_pOfferItem )
			{
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectNone" ) );
			}
			else
			{
				switch( m_nOfferItemBattleSlotIdx )
				{
					case SLOT_HONEST: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectHonest" ) ); break;
					case SLOT_STRANGE: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectStrange" ) ); break;
					case SLOT_WILD: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectWild" ) ); break;
					case SLOT_ELEGANT: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectElegant" ) ); break;
					case SLOT_FUNNY: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectFunny" ) ); break;
				}
			}

			CRectangle rtScreen = m_pThis->GetScreenRect();
			m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
			m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );
		}
		else // hoipoi stone slot
		{
			if( m_pUpgradeItem )
			{
				m_TempItemSlot.m_surSlotEffect.SetSurface( *m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.GetSurface() );

				switch(m_nOfferItemBattleSlotIdx)
				{
					case SLOT_HONEST: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectHonest" ) ); break;
					case SLOT_STRANGE: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectStrange" ) ); break;
					case SLOT_WILD: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectWild" ) ); break;
					case SLOT_ELEGANT: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectElegant" ) ); break;
					case SLOT_FUNNY: m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectFunny" ) ); break;
				}

				CRectangle rtScreen = m_pThis->GetScreenRect();
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
				m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );

				m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
				m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
			}

			m_aSlotEffect[nSlotIdx].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );
		}
	}	
	else
	{
		if( nSlotIdx == SLOT_ITEM )
		{
			m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
		else
		{
			// Only when the Hoi Poi Stone has completely disappeared, the original slot is turned to a no-attribute effect.
			if( m_pUpgradeItem && !m_pOfferItem )
			{
				m_TempItemSlot.m_surSlotEffect.SetSurface( *m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.GetSurface() );
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ItemChangeBattleAtt.srf", "srfItemEffectNone" ) );

				CRectangle rtScreen = m_pThis->GetScreenRect();
				m_aSlotEffect[SLOT_ITEM].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[SLOT_ITEM].m_rtEffectSlot.top );
				m_aSlotEffect[SLOT_ITEM].SetState( stSLOTEFFECT::SLOT_EFFECT_ON );

				m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
				m_TempItemSlot.SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
			}

			m_aSlotEffect[nSlotIdx].SetState( stSLOTEFFECT::SLOT_EFFECT_OFF );
		}
	}
}

RwBool CItemChangeBattleAttributeGui::IsUpgradableItem( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr->IsNeedToIdentify() )
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !pData )
		return FALSE;

	if( EQUIP_TYPE_MAIN_WEAPON == pData->byEquip_Type //||
		/*EQUIP_TYPE_SUB_WEAPON == pData->byEquip_Type ||*/
		/*EQUIP_TYPE_ARMOR == pData->byEquip_Type*/ )
	{
		return TRUE;
	}

	return FALSE;
}

RwBool CItemChangeBattleAttributeGui::IsValidOfferItem( CNtlSobItemAttr* pItemAttr )
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !m_pUpgradeItem )
		return TRUE;

	CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );
	sITEM_TBLDAT* pItemData = pUpgradeItemAttr->GetItemTbl();
	NTL_ASSERT( pItemData, "CItemChangeBattleAttributeGui::IsValidOfferItem : Item Must have ItemTable" );

	if( pData->byNeed_Min_Level >= pItemData->byNeed_Min_Level && pItemAttr->GetRank() >= pUpgradeItemAttr->GetRank()
		&& pData->byEquip_Type == pItemData->byEquip_Type && pItemAttr->GetGrade() == 0)
		return TRUE;

	return FALSE;
}


RwBool CItemChangeBattleAttributeGui::IsValidIconPickup( RwInt32 nClickIdx )
{
	if( m_eState == STATE_UPGRADE_PROCESS || 
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE" );
		return FALSE;
	}

	if( IsEmptySlot( nClickIdx ) )
	{
		return FALSE;
	}
	else if( IsUpgradeItemSlot( nClickIdx ) )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_PICKUP_UPGRADE_ITEM" );
		return FALSE;
	}

	return TRUE;
}

RwBool CItemChangeBattleAttributeGui::IsValidRightBtnProc( RwInt32 nClickIdx )
{
	if( m_eState == STATE_UPGRADE_PROCESS || 
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE" );
		return FALSE;
	}

	if( IsEmptySlot( nClickIdx ) )
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CItemChangeBattleAttributeGui::IsValidIdx( RwInt32 nIdx )
{
	if( nIdx >= 0 && nIdx < NUM_SLOT )
		return TRUE;

	return FALSE;
}

RwBool CItemChangeBattleAttributeGui::IsEmptySlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx == SLOT_ITEM )
	{
		if( m_pUpgradeItem )
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		if( m_nOfferItemBattleSlotIdx == nSlotIdx )
			return FALSE;
		else 
			return TRUE;
	}	
}

RwBool CItemChangeBattleAttributeGui::IsUpgradeItemSlot( RwInt32 nSlotIdx )
{
	return ( SLOT_ITEM == nSlotIdx ) ? TRUE : FALSE ;
}

RwBool CItemChangeBattleAttributeGui::IsOfferItemSlot( RwInt32 nSlotIdx )
{
	return (m_nOfferItemBattleSlotIdx == nSlotIdx ) ? TRUE : FALSE ;
}

BYTE CItemChangeBattleAttributeGui::GetAttributeFromSlot()
{
	if (m_pOfferItem)
	{
		switch (m_nOfferItemBattleSlotIdx)
		{
			case SLOT_HONEST: return BATTLE_ATTRIBUTE_HONEST;
			case SLOT_STRANGE: return BATTLE_ATTRIBUTE_STRANGE;
			case SLOT_WILD: return BATTLE_ATTRIBUTE_WILD;
			case SLOT_ELEGANT: return BATTLE_ATTRIBUTE_ELEGANCE;
			case SLOT_FUNNY: return BATTLE_ATTRIBUTE_FUNNY;

			default: return INVALID_BYTE;
		}
	}

	return INVALID_BYTE;
}

CNtlSobItem* CItemChangeBattleAttributeGui::GetSlotObject( RwInt32 nSlotIdx )
{
	if( IsUpgradeItemSlot( nSlotIdx ) )
		return m_pUpgradeItem;
	else if(IsOfferItemSlot( nSlotIdx ) )
		return m_pOfferItem;

	return NULL;
}

VOID CItemChangeBattleAttributeGui::OnMouseDown( const CKey& key )
{
	int nClickIdx = GetChildSlotIdx( (int)key.m_fX, (int)key.m_fY );

	if( nClickIdx < 0 )
		return;

	if( !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( m_asurDisableSlot[nClickIdx].IsShow() )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
		{
			m_nLSelectedSlotIdx = nClickIdx;
		}
		else
		{
			if( IsValidIconPickup( nClickIdx ) )
				m_nLSelectedSlotIdx = nClickIdx;			
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( !GetIconMoveManager()->IsActive() )
		{
			if( IsValidRightBtnProc( nClickIdx ) )
			{
				if( nClickIdx == SLOT_ITEM )
				{
					if( m_pUpgradeItem )
						m_nRSelectedSlotIdx = nClickIdx;
				}
				else
				{
					if( nClickIdx == m_nOfferItemBattleSlotIdx)
						m_nRSelectedSlotIdx = nClickIdx;
				}				
			}					
		}
	}

	m_pThis->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
		ClickEffect( FALSE );
	}
}

VOID CItemChangeBattleAttributeGui::OnMouseUp( const CKey& key )
{
	RwInt32 nClickIdx = GetChildSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );

	m_pThis->ReleaseMouse();

	if( nClickIdx < 0 || !IsShow() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( nClickIdx == m_nLSelectedSlotIdx )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				if( GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID )
				{
					SetItemSlot( GetIconMoveManager()->GetSrcSerial(), 
						GetIconMoveManager()->GetSrcPlace(),
						GetIconMoveManager()->GetSrcSlotIdx(),
						nClickIdx, TRUE );
				}
			}
			else
			{
				if( IsOfferItemSlot( nClickIdx ) )
				{
					GetIconMoveManager()->IconMovePickUp( m_pOfferItem->GetSerialID(), PLACE_ITEMUPGRADE,
						nClickIdx, 1, m_pOfferItem->GetIcon()->GetImage() );
				}								
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( nClickIdx == m_nRSelectedSlotIdx )
		{
			if( !GetIconMoveManager()->IsActive() )
			{
				if (nClickIdx == SLOT_ITEM)
				{
					DeleteUpgradeItem();
					DeleteOfferItem();
				}
				else
					DeleteOfferItem();

				// Info Wnd 변경
				if( m_nMouseOnIdx >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE )
				{
					ShowInfoWnd( TRUE, m_nMouseOnIdx );					
				}
			}
		}		

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CItemChangeBattleAttributeGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	RwInt32 nMouseOnIdx = GetChildSlotIdx( nXPos, nYPos );

	if( nMouseOnIdx >= 0 )
	{
		m_anFocusEffect[nMouseOnIdx] = m_anFocusEffect[nMouseOnIdx] | SLOT_FOCUS;		

		if( m_nMouseOnIdx != nMouseOnIdx )
		{
			m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
			m_nMouseOnIdx = nMouseOnIdx;

			ShowInfoWnd( TRUE, m_nMouseOnIdx );							

			if( nMouseOnIdx == m_nPushDownIndex )
				ClickEffect( TRUE, nMouseOnIdx );
			else if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
		}
	}
	else
	{
		if( m_nMouseOnIdx >= 0 )
		{
			m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
			m_nMouseOnIdx = -1;
			if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
			ShowInfoWnd( FALSE );
		}
	}
}

VOID CItemChangeBattleAttributeGui::OnMouseOut( gui::CComponent* pComponent )
{
	if( m_nMouseOnIdx >= 0 )
	{
		m_anFocusEffect[m_nMouseOnIdx] = m_anFocusEffect[m_nMouseOnIdx] & ~SLOT_FOCUS;
		m_nMouseOnIdx = -1;
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		ShowInfoWnd( FALSE );
	}
}

VOID CItemChangeBattleAttributeGui::OnPaint(VOID)
{
	m_BackFocus.m_surSlotEffect.Render();

	if( m_pUpgradeItem )
		m_surSlot[SLOT_ITEM].Render();

	if( m_nOfferItemBattleSlotIdx >= 0 )
		m_surSlot[m_nOfferItemBattleSlotIdx].Render();

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		if( m_anFocusEffect[i] )
			m_asurFocus[i].Render();

		m_asurDisableSlot[i].Render();
		m_aSlotEffect[i].m_surSlotEffect.Render();				
	}	

	m_TempItemSlot.m_surSlotEffect.Render();
}

VOID CItemChangeBattleAttributeGui::OnMove( RwInt32 nXPos, RwInt32 nYPos )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rect;

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		m_surSlot[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_asurFocus[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_asurDisableSlot[i].SetPosition( rtScreen.left + m_rtSlot[i].left, rtScreen.top + m_rtSlot[i].top );
		m_aSlotEffect[i].m_surSlotEffect.SetPosition( rtScreen.left + m_aSlotEffect[i].m_rtEffectSlot.left, rtScreen.top + m_aSlotEffect[i].m_rtEffectSlot.top );
	}	

	m_TempItemSlot.m_surSlotEffect.SetPosition( rtScreen.left + m_TempItemSlot.m_rtEffectSlot.left, rtScreen.top + m_TempItemSlot.m_rtEffectSlot.top );
	m_BackFocus.m_surSlotEffect.SetPosition( rtScreen.left + m_BackFocus.m_rtEffectSlot.left, rtScreen.top + m_BackFocus.m_rtEffectSlot.top );
}

VOID CItemChangeBattleAttributeGui::OnClickExitBtn( gui::CComponent* pComponent )
{
	if( m_eState == STATE_PACKETWAIT )
		return;

	GetDialogManager()->CloseDialog( DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE );
}

VOID CItemChangeBattleAttributeGui::OnClickCompoundBtn( gui::CComponent* pComponent )
{
	if( m_eState != STATE_UPGRADE_READY )
		return;

	SetState(TATE_UPGRADE_CONFIRM);

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		GetIconMoveManager()->IconMoveEnd();	
}

VOID CItemChangeBattleAttributeGui::OnClickCloseResult( gui::CComponent* pComponent )
{
	if( m_pUpgradeItem )
	{
		SetState(STATE_UPGRADE_READY);
	}
	else
	{
		SetState( STATE_NONE );
	}

	DestroyResultDialog();
}

VOID CItemChangeBattleAttributeGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE, key.m_fX, key.m_fY );
}

VOID CItemChangeBattleAttributeGui::OnCaptureMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	gui::CScrollBar* pScrollBar = m_phtmlInfoText->GetScrollBar();

	RwInt32 nValue = pScrollBar->GetValue();
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - ( sDelta * 18 ) / GUI_MOUSE_WHEEL_DELTA;

	if( nDelta < 0 )
		nDelta = 0;
	else if( nDelta > nMaxValue )
		nDelta = nMaxValue;

	pScrollBar->SetValue( nDelta );
}

VOID CItemChangeBattleAttributeGui::OnChangeAttributeEffectEnd( gui::CComponent* pComponent )
{
	m_pflsUpgradeEffect->Show( false );
	SendItemChangeBattleAttributePacket();
}

void CItemChangeBattleAttributeGui::OnChangeAttributeResultEffectEnd(gui::CComponent* pComponent)
{
	if (m_pUpgradeItem)
	{
		SetState(STATE_UPGRADE_READY);
	}
	else
	{
		SetState(STATE_NONE);
	}

	DestroyResultDialog();
}