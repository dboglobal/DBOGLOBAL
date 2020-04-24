#include "precomp_dboclient.h"
#include "ItemUpgradeGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// shared
#include "NtlItem.h"
#include "NtlCharacter.h"
#include "ItemTable.h"
#include "ItemOptionTable.h"
#include "SystemEffectTable.h"
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "GraphicDataTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"
#include "NtlCameraController.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// Dbo
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"

#define ITEMUPGRADEGUI_SUCCESS_FLASHFILE		"ItemUpgrade_Success.swf"
#define ITEMUPGRADEGUI_FAIL_FLASHFILE			"ItemUpgrade_Fail.swf"


CItemUpgradeGui::CItemUpgradeGui(VOID)
{
	Init();
}

CItemUpgradeGui::CItemUpgradeGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CItemUpgradeGui::~CItemUpgradeGui(VOID)
{

}

VOID CItemUpgradeGui::Init(VOID)
{
	m_pUpgradeItem = NULL;
	m_pStoneItem = NULL;
	m_pGuardItem = NULL;
	m_nStoneStackNum = 0;
	m_nGuardStoneStackNum = 0;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;

	for( RwInt32 i = 0 ; i < NUM_SLOT; ++i )
	{
		m_anFocusEffect[i] = SLOT_FOCUS_NONE;
	}

	memset(m_pstbStackCount, 0, sizeof(gui::CStaticBox*) * (NUM_SLOT-1));

	m_eState = STATE_NONE;

	m_fCurrentTime = 0.0f;

	m_hNPCSerial = INVALID_SERIAL_ID;
}

RwBool CItemUpgradeGui::Create(VOID)
{
	NTL_FUNCTION( "CItemUpgradeGui::Create" );

	if( !CNtlPLGui::Create( "gui\\ItemUpgrade.rsr", "gui\\ItemUpgrade.srf", "gui\\ItemUpgrade.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pstbInfoTitle = (gui::CStaticBox*)GetComponent( "stbInfoTitle" );
	m_pbtnExit = (gui::CButton*)GetComponent("btnExit");
	m_pbtnSelect = (gui::CButton*)GetComponent("btnSelect");

	m_ppnlUpgradedItemSlot = (gui::CPanel*)GetComponent("pnlUpgradedItemSlot");
	m_ppnlUpgradedGuardStoneSlot = (gui::CPanel*)GetComponent("pnlUpgradedGuardStoneSlot");
	m_ppnlUpgradedStoneSlot = (gui::CPanel*)GetComponent("pnlUpgradedStoneSlot");

	m_phtmlInfoText = (gui::CHtmlBox*)GetComponent("htmlUpgradeInfo");

	m_pdlgResult = (gui::CDialog*)GetComponent("dlgResult");
	m_pflsResult = (gui::CFlash*)GetComponent("flsResult");
	m_pbtnCloseResult = (gui::CButton*)GetComponent("btnClose");

	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CItemUpgradeGui::OnMouseUp );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CItemUpgradeGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CItemUpgradeGui::OnMove );
	m_slotClickExitBtn  = m_pbtnExit->SigClicked().Connect( this, &CItemUpgradeGui::OnClickExitBtn );
	m_slotClickCompoundBtn = m_pbtnSelect->SigClicked().Connect( this, &CItemUpgradeGui::OnClickCompoundBtn );

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CItemUpgradeGui::OnCaptureMouseDown );
	m_slotCaptureMouseWheel = m_phtmlInfoText->SigMouseWheel().Connect(this, &CItemUpgradeGui::OnCaptureMouseWheel);
	
	m_slotUpgradeItemIconMouseEnter = m_ppnlUpgradedItemSlot->SigMouseEnter().Connect( this, &CItemUpgradeGui::OnUpgradeItemIconMouseEnter);
	m_slotUpgradeItemIconMouseLeave = m_ppnlUpgradedItemSlot->SigMouseLeave().Connect( this, &CItemUpgradeGui::OnUpgradeItemIconMouseLeave);
	m_slotUpgradeItemIconMouseDown = m_ppnlUpgradedItemSlot->SigMouseDown().Connect(this, &CItemUpgradeGui::OnUpgradeItemIconMouseDown);

	m_slotGuardItemIconMouseEnter = m_ppnlUpgradedGuardStoneSlot->SigMouseEnter().Connect(this, &CItemUpgradeGui::OnGuardItemIconMouseEnter);
	m_slotGuardItemIconMouseLeave = m_ppnlUpgradedGuardStoneSlot->SigMouseLeave().Connect(this, &CItemUpgradeGui::OnGuardItemIconMouseLeave);
	m_slotGuardItemIconMouseDown = m_ppnlUpgradedGuardStoneSlot->SigMouseDown().Connect(this, &CItemUpgradeGui::OnGuardItemIconMouseDown);

	m_slotStoneItemIconMouseEnter = m_ppnlUpgradedStoneSlot->SigMouseEnter().Connect(this, &CItemUpgradeGui::OnStoneItemIconMouseEnter);
	m_slotStoneItemIconMouseLeave = m_ppnlUpgradedStoneSlot->SigMouseLeave().Connect(this, &CItemUpgradeGui::OnStoneItemIconMouseLeave);
	m_slotStoneItemIconMouseDown = m_ppnlUpgradedStoneSlot->SigMouseDown().Connect(this, &CItemUpgradeGui::OnStoneItemIconMouseDown);

	m_slotUpgradeEffectResultEnd = m_pflsResult->SigMovieEnd().Connect(this, &CItemUpgradeGui::OnUpgradeResultEffectEnd);
	m_slotClickCloseResult = m_pbtnCloseResult->SigClicked().Connect(this, &CItemUpgradeGui::OnClickCloseResult);
	
	SetSlotRectHardCode();
	SetBasicUISetting();

	// Above the stack number.
	m_pdlgResult->Raise();

	Show( false );
	m_pdlgResult->Show(false);

	LinkMsg( g_EventItemUpgradeResult, 0 );
	LinkMsg( g_EventNPCDialogOpen, 0 );
	LinkMsg( g_EventDialog, 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventSobItemDelete, 0, 0x9000 );		// Because you are writing a pointer, you must come in before the object is cleared.
	LinkMsg( g_EventRegisterItemUpgrade, 0 );
	LinkMsg( g_EventGameServerChangeOut );

	NTL_RETURN( TRUE );
}

VOID CItemUpgradeGui::Destroy(VOID)
{
	UnLinkMsg( g_EventItemUpgradeResult );
	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobItemDelete );
	UnLinkMsg( g_EventRegisterItemUpgrade );
	UnLinkMsg( g_EventGameServerChangeOut );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CItemUpgradeGui::DestroyResultDialog(VOID)
{
	m_pdlgResult->Show(false);
	m_pflsResult->PlayMovie(FALSE);
}

VOID CItemUpgradeGui::Update( RwReal fElapsed )
{
	if( m_eState == STATE_RESULT )
	{
		m_pflsResult->Update(fElapsed);
	}
}

RwInt32 CItemUpgradeGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
		pWorldConceptController->ChangeState(WORLD_NPC_ITEM_UPGRADE);

		RaiseTop();			

		Show( true );
		DeleteHoipoiStone();
		DeleteUpgradeItem();
		SetState( STATE_NONE );

		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		if( m_eState == STATE_PACKETWAIT )
			return -1;

		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		GetDialogManager()->SwitchBag( FALSE );

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
			GetIconMoveManager()->IconMoveEnd();

		if (m_pdlgResult->IsVisible())
			DestroyResultDialog();

		Show( false );
		DeleteHoipoiStone();
		DeleteUpgradeItem();

		Logic_CancelNpcFacing();

		GetNtlGuiManager()->RemoveUpdateFunc( this );
	}

	return 1;
}

VOID CItemUpgradeGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventItemUpgradeResult )
	{
		SDboItemUpgradeResult* pData = reinterpret_cast<SDboItemUpgradeResult*>( msg.pData );

		SetResult( pData->wResultcode, pData->byGrade, pData->wResultMessageCode, pData->byStoneCount, pData->byGuardStoneCount);
	}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_ITEMUPGRADE )
			return;

		GetDialogManager()->OpenDialog( DIALOG_ITEMUPGRADE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		GetDialogManager()->SwitchBag( TRUE );
		m_hNPCSerial = pData->hSerialId;
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );
		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_ITEMUPGRADE )
		{
			GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
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

		if( m_pUpgradeItem )
		{
			if( pData->hItemSerialId == m_pUpgradeItem->GetSerialID() )
				DeleteUpgradeItem();						
		}

		if( m_pStoneItem )
		{
			if( pData->hItemSerialId == m_pStoneItem->GetSerialID() )
				DeleteHoipoiStone();
		}

		if (m_pGuardItem)
		{
			if (pData->hItemSerialId == m_pGuardItem->GetSerialID())
				DeleteGuardStone();
		}
	}
	else if( msg.Id == g_EventRegisterItemUpgrade )
	{
		SDboRegisterItemUpgrade* pData = reinterpret_cast<SDboRegisterItemUpgrade*>( msg.pData );

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->hSrcSerial ) );
		if( pItem )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

			if( IsUpgradableItem( pItemAttr ) )
			{
				SetItemSlot( pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_ITEM, FALSE );
				RaiseTop();			
			}
			else if( IsHoipoiStone( pItemAttr ) )
			{
				SetItemSlot( pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_STONE, FALSE );
				RaiseTop();			
			}
			else if (IsGuardStone(pItemAttr))
			{
				SetItemSlot(pData->hSrcSerial, pData->eSrcPlace, pData->uiSrcSlotIdx, SLOT_PROTECTION, FALSE);
				RaiseTop();
			}
			else
				GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEITEM_OR_HOIPOISTONE" );
		}		
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
	}
}

VOID CItemUpgradeGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtUpgradeItemScreen = m_ppnlUpgradedItemSlot->GetScreenRect();

	m_surSlot[SLOT_ITEM].SetRectWH(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurFocus[SLOT_ITEM].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_asurFocus[SLOT_ITEM].SetRectWH(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurDisableSlot[SLOT_ITEM].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
	m_asurDisableSlot[SLOT_ITEM].SetRectWH(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);

	CRectangle rtStoneItemScreen = m_ppnlUpgradedStoneSlot->GetScreenRect();

	m_surSlot[SLOT_STONE].SetRectWH(rtStoneItemScreen.left, rtStoneItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurFocus[SLOT_STONE].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_asurFocus[SLOT_STONE].SetRectWH(rtStoneItemScreen.left, rtStoneItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurDisableSlot[SLOT_STONE].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
	m_asurDisableSlot[SLOT_STONE].SetRectWH(rtStoneItemScreen.left, rtStoneItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);

	CRectangle rtGuardItemScreen = m_ppnlUpgradedGuardStoneSlot->GetScreenRect();

	m_surSlot[SLOT_PROTECTION].SetRectWH(rtGuardItemScreen.left, rtGuardItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurFocus[SLOT_PROTECTION].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_asurFocus[SLOT_PROTECTION].SetRectWH(rtGuardItemScreen.left, rtGuardItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
	m_asurDisableSlot[SLOT_PROTECTION].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
	m_asurDisableSlot[SLOT_PROTECTION].SetRectWH(rtGuardItemScreen.left, rtGuardItemScreen.top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
}

VOID CItemUpgradeGui::SetBasicUISetting(VOID)
{
	m_pstbInfoTitle->SetText( GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_DLGTITLE" ) );

	
	std::wstring strPrice = GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_MAINTITLE");
	strPrice += L"[br]";
	strPrice += GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_INFO");
	strPrice += L"[br][br]";
	strPrice += GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_SUBTITLE_01");
	strPrice += L"[br]";
	strPrice += GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_ITEMDATAINFO");
	strPrice += L"[br][br]";
	strPrice += GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_SUBTITLE_02");
	strPrice += L"[br]";
	strPrice += GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_ITEMGUARDINFO");

	m_phtmlInfoText->SetHtmlFromMemory(strPrice.c_str(), wcslen(strPrice.c_str()));
}

RwBool CItemUpgradeGui::CreateStackNumber(RwInt32 nSlotIdx, RwInt32 nValue)
{
	NTL_FUNCTION("CItemUpgradeGui::CreateStackNumber");

	// check if already exist.. update text
	if (m_pstbStackCount[nSlotIdx])
	{
		m_pstbStackCount[nSlotIdx]->SetText(nValue);
		NTL_RETURN(TRUE);
	}


	CRectangle rtScreen;

	if(nSlotIdx == SLOT_STONE)
		rtScreen = m_ppnlUpgradedStoneSlot->GetScreenRect();
	else
		rtScreen = m_ppnlUpgradedGuardStoneSlot->GetScreenRect();

	CRectangle rect;
	rect.SetRectWH(1, 0, DBOGUI_STACKNUM_WIDTH, DBOGUI_STACKNUM_HEIGHT);

	if (nSlotIdx == SLOT_STONE)
		m_pstbStackCount[nSlotIdx] = NTL_NEW gui::CStaticBox(rect, m_ppnlUpgradedStoneSlot, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);
	else
		m_pstbStackCount[nSlotIdx] = NTL_NEW gui::CStaticBox(rect, m_ppnlUpgradedGuardStoneSlot, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN);

	if (!m_pstbStackCount[nSlotIdx])
		NTL_RETURN(FALSE);

	m_pstbStackCount[nSlotIdx]->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
	m_pstbStackCount[nSlotIdx]->SetEffectMode(DBOGUI_STACKNUM_FONTEFFECTMODE);
	m_pstbStackCount[nSlotIdx]->SetText(nValue);
	m_pstbStackCount[nSlotIdx]->Enable(false);

	NTL_RETURN(TRUE);
}

VOID CItemUpgradeGui::DeleteStackNumber(RwInt32 nSlotIdx)
{
	if (m_pstbStackCount[nSlotIdx])
		NTL_DELETE(m_pstbStackCount[nSlotIdx]);
}

// Error Notify is handled here.
VOID CItemUpgradeGui::SetItemSlot( RwUInt32 hSerialID, RwUInt32 eSrcplace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiDestSlotIdx, RwBool bNeedToIconMoveEnd )
{
	if( m_eState == STATE_UPGRADE_PROCESS ||
		m_eState == STATE_PACKETWAIT ||
		m_eState == STATE_RESULT )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE" );
		return;
	}

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerialID ) );
	NTL_ASSERT( pItem, "CItemUpgradeGui::SetITemSlot : pItem is must be present!" );

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

	if(eSrcplace == PLACE_ITEMUPGRADE )
	{
		if( uiSrcSlotIdx == uiDestSlotIdx )
		{
			if( bNeedToIconMoveEnd )
				GetIconMoveManager()->IconMoveEnd();

			return;
		}
	}
	else if(eSrcplace == PLACE_BAG )
	{
		if( m_eState == STATE_NONE || m_eState == STATE_UPGRADE_READYFAIL_NOMORESTONES)
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				if (m_pUpgradeItem)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX");
					return;
				}

				if (!IsValidUpgradeItem(pItemAttr, true))
					return;

				SetUpgradeItem( pItem );

				if (m_pUpgradeItem && m_pStoneItem)
				{
					SetState(STATE_UPGRADE_READY);
				}
			}
			else if (uiDestSlotIdx == SLOT_STONE)
			{
				if (m_pStoneItem)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE");
					return;
				}

				if (!IsValidHoipoiStone(pItemAttr, true))
				{
					return;
				}

				SetHoipoiStone(pItemAttr->GetStackNum(), pItem);

				if (m_pUpgradeItem && m_pStoneItem)
				{
					SetState(STATE_UPGRADE_READY);
				}
			}
			else if (uiDestSlotIdx == SLOT_PROTECTION)
			{
				if (m_pGuardItem)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_GUARDSTONE");
					return;
				}

				if (!IsValidProtectionStone(pItemAttr, true))
				{
					return;
				}

				SetGuardStone(pItemAttr->GetStackNum(), pItem);
			}
			else 
			{
				// something wrong
			}
		}
		else if( m_eState == STATE_UPGRADE_READYFAIL_NOMOREUPGRADE)
		{
			// dont allow insert anything because item is max grade
			return;
		}
		else // Upgrade Ready
		{
			if( uiDestSlotIdx == SLOT_ITEM )
			{
				GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX" );
				return;
			}
			else if (uiDestSlotIdx == SLOT_PROTECTION)
			{
				if (m_pGuardItem)
				{
					GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_GUARDSTONE");
					return;
				}

				if (!IsValidProtectionStone(pItemAttr, true))
				{
					return;
				}

				SetGuardStone(pItemAttr->GetStackNum(), pItem);
			}
			else
			{
				GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE" );
				return;
			}
		}	

		CDboEventGenerator::DialogEvent( DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx() );
	}
	else
	{
		return;
	}	

	// 
	UpdateSlotIcon();

	// 아이콘 무브 완료
	if( bNeedToIconMoveEnd )
		GetIconMoveManager()->IconMoveEnd();
}

VOID CItemUpgradeGui::UpdateSlotIcon(VOID)
{
	if( m_pStoneItem )
	{
		CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pStoneItem->GetSobAttr() );
		CreateStackNumber(SLOT_STONE, pAttr->GetStackNum() );
	}

	if (m_pGuardItem)
	{
		CNtlSobItemAttr* pAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pGuardItem->GetSobAttr());
		CreateStackNumber(SLOT_PROTECTION, pAttr->GetStackNum());
	}
}

VOID CItemUpgradeGui::SetUpgradeItem( CNtlSobItem* pUpgradeItem )
{
	if( !pUpgradeItem )
	{
		NTL_ASSERTFAIL( "CItemUpgradeGui::SetUpgrade : Slot Fail or Invalid Icon");
		return;
	}

	m_pUpgradeItem = pUpgradeItem;
	m_surSlot[SLOT_ITEM].SetTexture( (gui::CTexture*)pUpgradeItem->GetIcon()->GetImage() );
}

VOID CItemUpgradeGui::SetHoipoiStone(RwInt32 nStackCount, CNtlSobItem* pHoipoiStone)
{
	if( !pHoipoiStone  )
	{
		NTL_ASSERTFAIL( "CItemUpgradeGui::SetHoipoiStone : Slot Fail or Invalid Icon");
		return;
	}

	m_pStoneItem = pHoipoiStone;
	m_nStoneStackNum = nStackCount;
	m_surSlot[SLOT_STONE].SetTexture( (gui::CTexture*)pHoipoiStone->GetIcon()->GetImage() );
	CreateStackNumber(SLOT_STONE, nStackCount );
}

VOID CItemUpgradeGui::SetGuardStone(RwInt32 nStackCount, CNtlSobItem * pGuardStone)
{
	if (!pGuardStone)
	{
		NTL_ASSERTFAIL("CItemUpgradeGui::SetGuardStone : Slot Fail or Invalid Icon");
		return;
	}

	m_pGuardItem = pGuardStone;
	m_nGuardStoneStackNum = nStackCount;
	m_surSlot[SLOT_PROTECTION].SetTexture((gui::CTexture*)pGuardStone->GetIcon()->GetImage());
	CreateStackNumber(SLOT_PROTECTION, nStackCount);
}

VOID CItemUpgradeGui::DeleteUpgradeItem()
{
	if( m_pUpgradeItem )
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, 
			m_pUpgradeItem->GetParentItemSlotIdx(), m_pUpgradeItem->GetItemSlotIdx() );

		m_pUpgradeItem = NULL;
		m_surSlot[SLOT_ITEM].SetTexture( (gui::CTexture*)NULL );

		// delete guard stone
		DeleteGuardStone();

		if (m_eState != STATE_UPGRADE_PROCESS)
			SetState(STATE_NONE);
	}
}

VOID CItemUpgradeGui::DeleteHoipoiStone(bool bReplace)
{
	if( m_pStoneItem )
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, 
			m_pStoneItem->GetParentItemSlotIdx(), m_pStoneItem->GetItemSlotIdx() );

		m_pStoneItem = NULL;
		DeleteStackNumber(SLOT_STONE);
		m_surSlot[SLOT_STONE].SetTexture( (gui::CTexture*)NULL );

		if(m_eState != STATE_UPGRADE_PROCESS)
			SetState(STATE_NONE);
	}

	m_pbtnSelect->SetText( GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_BTN_COMPOUND" ) );
}

VOID CItemUpgradeGui::DeleteGuardStone(bool bReplace)
{
	if (m_pGuardItem)
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG,
			m_pGuardItem->GetParentItemSlotIdx(), m_pGuardItem->GetItemSlotIdx());

		m_pGuardItem = NULL;
		DeleteStackNumber(SLOT_PROTECTION);
		m_surSlot[SLOT_PROTECTION].SetTexture((gui::CTexture*)NULL);
	}
}

// peessiupgrade : If it disappears, it must be received and processed.. 
VOID CItemUpgradeGui::SetResult(WORD wResultcode, BYTE byGrade, WORD wResultMessageCode, BYTE byStoneCount, BYTE byGuardStoneCount)
{
	NTL_ASSERT(m_pUpgradeItem, "CItemUpgradeGui::HandleEvents : UpgradeItem Must be Present!");

	// execption.. --;; 에러메시지는 패킷핸들러에서 처리된다.
	if (wResultcode != GAME_SUCCESS)
	{
		SetState(STATE_UPGRADE_READY);
		return;
	}

	switch (wResultMessageCode)
	{
		case ITEM_UPGRADE_RESULT_SUCCESS:
		{
			m_pflsResult->Load(ITEMUPGRADEGUI_SUCCESS_FLASHFILE);
			m_pflsResult->PlayMovie(TRUE);

			// alarm message item upgrade success
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_SUCCESS");
		}
		break;
		case ITEM_UPGRADE_RESULT_FAIL:
		{
			m_pflsResult->Load(ITEMUPGRADEGUI_FAIL_FLASHFILE);
			m_pflsResult->PlayMovie(TRUE);

			// alarm message item upgrade success
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_FAIL");
		}
		break;
		case ITEM_UPGRADE_RESULT_RESET:
		{
			m_pflsResult->Load(ITEMUPGRADEGUI_FAIL_FLASHFILE);
			m_pflsResult->PlayMovie(TRUE);

			// alarm message item upgrade success
			if(m_pGuardItem)
				GetAlarmManager()->AlarmMessage("GAME_ITEM_UPGRADE_FAIL_AND_NODEL_BY_CORE");
			else
				GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_FAIL");//GetAlarmManager()->AlarmMessage("GAME_ITEM_UPGRADE_FAIL_AND_DEL");
		}
		break;
		case ITEM_UPGRADE_RESULT_DESTROY:
		{
			m_pflsResult->Load(ITEMUPGRADEGUI_FAIL_FLASHFILE);
			m_pflsResult->PlayMovie(TRUE);

			// alarm message item upgrade success
			if (m_pGuardItem)
				GetAlarmManager()->AlarmMessage("GAME_ITEM_UPGRADE_FAIL_AND_NODEL_BY_CORE");
			else
				GetAlarmManager()->AlarmMessage("GAME_ITEM_UPGRADE_FAIL_AND_DEL");
		}
		break;

		default: break;
	}

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pUpgradeItem->GetSobAttr());
	std::wstring wstrItemName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText(pItemAttr->GetItemTbl()->Name);

	GetAlarmManager()->FormattedAlarmMessage("DST_ITEMUPGRADE_TEXTDATA", FALSE, NULL, wstrItemName.c_str(), byGrade);

	// remove the stone item
	if (byStoneCount == 0)
		DeleteHoipoiStone();
	else
		CreateStackNumber(SLOT_STONE, byStoneCount);

	// remove guard stone
	if(byGuardStoneCount == 0)
		DeleteGuardStone();
	else
		CreateStackNumber(SLOT_PROTECTION, byGuardStoneCount);


	SetState(STATE_RESULT);
}

VOID CItemUpgradeGui::SetState( STATE eState )
{
	m_pbtnExit->ClickEnable( TRUE );
	m_pbtnSelect->ClickEnable( FALSE );

	DisableSlot(FALSE);

	const WCHAR* pInfo;

	switch( eState )
	{
		case STATE_NONE:
		{
			// Info
			SetBasicUISetting();

			if( GetIconMoveManager()->IsActive() )
				ShowIconDestination( TRUE );			

		}
		break;
		case STATE_UPGRADE_READYFAIL_NOMOREUPGRADE:
		{
			// Info			
			pInfo = GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_MSG_UNUPGRADABLE_ITEM" );
			m_phtmlInfoText->SetHtmlFromMemory( pInfo, wcslen( pInfo ) );
		}
		break;
		case STATE_UPGRADE_READYFAIL_NOMORESTONES:
		{
			// Info			
			pInfo = GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_NOTIFY_NEED_TO_REGISTER_HOIPOISTONE");
			m_phtmlInfoText->SetHtmlFromMemory(pInfo, wcslen(pInfo));
		}
		break;
		case STATE_UPGRADE_READY:
		{
			// Info
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

			if (m_pStoneItem == NULL)
			{
				SetState(STATE_UPGRADE_READYFAIL_NOMORESTONES);
				return;
			}

			if( pItemAttr->GetGrade() == NTL_ITEM_MAX_GRADE)
			{
				SetState( STATE_UPGRADE_READYFAIL_NOMOREUPGRADE );
				return;
			}

			if (m_eState == STATE_UPGRADE_READYFAIL_NOMORESTONES || m_eState == STATE_UPGRADE_READYFAIL_NOMOREUPGRADE)// if text was changed.. set it back
			{
				SetBasicUISetting();
			}

			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_BTNCLICK");

			// UpgradeButton 활성화
			m_pbtnSelect->ClickEnable( TRUE );

		}
		break;
		case STATE_UPGRADE_PROCESS:
		{
			DisableSlot( TRUE );

			// Exit button disabled !!!
			m_pbtnExit->ClickEnable(FALSE);

			SendUpgradePacket();
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

VOID CItemUpgradeGui::SendUpgradePacket(VOID)
{
	RwUInt8 byItemPlace = m_pUpgradeItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byItemPos = m_pUpgradeItem->GetItemSlotIdx();

	RwUInt8 byStonePlace = m_pStoneItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
	RwUInt8 byStonePos = m_pStoneItem->GetItemSlotIdx();

	RwUInt8 byCorePlace = INVALID_BYTE;
	RwUInt8 byCorePos = INVALID_BYTE;

	if (m_pGuardItem)
	{
		byCorePlace = m_pGuardItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
		byCorePos = m_pGuardItem->GetItemSlotIdx();
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendItemUpgradeReq( m_hNPCSerial, byItemPlace, byItemPos, byStonePlace, byStonePos, byCorePlace, byCorePos);
	SetState( STATE_PACKETWAIT );
}

VOID CItemUpgradeGui::ShowIconDestination( RwBool isPick )
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
				NTL_ASSERT( pSobItem, "CItemUpgradeGui::ShowIconDestination : pSobItem is must be present!" );			

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
				NTL_ASSERT( pSobItemAttr, "CItemUpgradeGui::ShowIconDestination : pSobItemAttr is must be present!" );			

				if(IsValidUpgradeItem( pSobItemAttr ) )
				{
					if(m_pUpgradeItem == NULL)
						m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] |= SLOT_FOCUS_CAN_MOVE;
				}
				else if( IsValidHoipoiStone( pSobItemAttr ) )
				{
					if (m_pStoneItem == NULL)
						m_anFocusEffect[SLOT_STONE] = m_anFocusEffect[SLOT_STONE] |= SLOT_FOCUS_CAN_MOVE;
				}
				else if (IsValidProtectionStone(pSobItemAttr))
				{
					if (m_pGuardItem == NULL)
						m_anFocusEffect[SLOT_PROTECTION] = m_anFocusEffect[SLOT_PROTECTION] |= SLOT_FOCUS_CAN_MOVE;
				}
			}
		}
		else if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		{
			if( m_eState == STATE_UPGRADE_PROCESS || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
				return;
			else
			{

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

VOID CItemUpgradeGui::DisableSlot(RwBool bDisable)
{
	for (RwInt32 i = 0; i < NUM_SLOT; ++i)
	{
		m_asurDisableSlot[i].Show(bDisable);
	}
}

RwBool CItemUpgradeGui::IsUpgradableItem( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr->IsNeedToIdentify() )
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !pData )
		return FALSE;

	if( EQUIP_TYPE_MAIN_WEAPON == pData->byEquip_Type ||
		EQUIP_TYPE_SUB_WEAPON == pData->byEquip_Type ||
		EQUIP_TYPE_ARMOR == pData->byEquip_Type )
	{
		return TRUE;
	}

	return FALSE;
}

RwBool CItemUpgradeGui::IsHoipoiStone( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr->IsNeedToIdentify() )
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if( !pData )
		return FALSE;

	if( pData->byItem_Type == ITEM_TYPE_UPGRADE_STONE_WEAPON ||
		pData->byItem_Type == ITEM_TYPE_UPGRADE_STONE_ARMOR ||
		pData->byItem_Type == ITEM_TYPE_GREATER_UPGRADE_STONE_WEAPON || 
		pData->byItem_Type == ITEM_TYPE_GREATER_UPGRADE_STONE_ARMOR)
		return TRUE;

	return FALSE;
}

RwBool CItemUpgradeGui::IsGuardStone(CNtlSobItemAttr * pItemAttr)
{
	if (pItemAttr->IsNeedToIdentify())
		return FALSE;

	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!pData)
		return FALSE;

	if (pData->byItem_Type == ITEM_TYPE_STONE_CORE ||
		pData->byItem_Type == ITEM_TYPE_STONE_CORE_OLD ||
		pData->byItem_Type == ITEM_TYPE_STONE_CORE_PLUS)
		return TRUE;

	return FALSE;
}

RwBool CItemUpgradeGui::IsValidUpgradeItem(CNtlSobItemAttr * pItemAttr, bool bSendError)
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!IsUpgradableItem(pItemAttr))
	{
		if(bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM");

		return FALSE;
	}

	if (pItemAttr->GetGrade() >= NTL_ITEM_MAX_GRADE) // already max level
	{
		if (bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_ITEMSLOT_ERROR");

		return FALSE;
	}


	if (m_pStoneItem) // if already stone inserted.. lets do check if item works with this stone
	{
		CNtlSobItemAttr* pStoneItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pStoneItem->GetSobAttr());
		sITEM_TBLDAT* pItemData = pStoneItemAttr->GetItemTbl();
		NTL_ASSERT(pItemData, "CItemUpgradeGui::IsValidUpgradeItem : Item Must have ItemTable");

		if (pData->byNeed_Min_Level > pItemData->byNeed_Max_Level)
		{
			if (bSendError)
				GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_STONELEVEL");

			return FALSE;
		}
	}

	return TRUE;
}

RwBool CItemUpgradeGui::IsValidHoipoiStone(CNtlSobItemAttr* pItemAttr, bool bSendError)
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!IsHoipoiStone(pItemAttr))
	{
		if(bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE");
		return FALSE;
	}

	if (m_pUpgradeItem)
	{
		CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pUpgradeItem->GetSobAttr());
		sITEM_TBLDAT* pItemData = pUpgradeItemAttr->GetItemTbl();
		NTL_ASSERT(pItemData, "CItemUpgradeGui::IsValidHoipoiStone : Item Must have ItemTable");

		if (pData->byNeed_Max_Level < pItemData->byNeed_Min_Level)
		{
			if (bSendError)
				GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_STONELEVEL");

			return FALSE;
		}
	}

	return TRUE;
}

RwBool CItemUpgradeGui::IsValidProtectionStone(CNtlSobItemAttr * pItemAttr, bool bSendError)
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();

	if (!IsGuardStone(pItemAttr))
	{
		if(bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_CORESTONE");

		return FALSE;
	}

	
	if (!m_pUpgradeItem)
	{
		if (bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_ITEM_TOOTLP"); // place weapon or armor first..

		return FALSE;
	}

	CNtlSobItemAttr* pUpgradeItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pUpgradeItem->GetSobAttr());
	sITEM_TBLDAT* pItemData = pUpgradeItemAttr->GetItemTbl();
	NTL_ASSERT(pItemData, "CItemUpgradeGui::IsValidProtectionStone : Item Must have ItemTable");

	// check if white stone is even required..
	if (pUpgradeItemAttr->GetGrade() < 1)
	{
		if(bSendError)
			GetAlarmManager()->AlarmMessage("DST_ITEMUPGRADE_GUARDSTONE_NOT_NEED");

		return FALSE;
	}

	if (pData->byNeed_Max_Level < pItemData->byNeed_Min_Level)
	{
		if (bSendError)
			GetAlarmManager()->AlarmMessage("GAME_ITEM_UPGRADE_MUST_USE_PROPER_LEVEL_CORE_STONE");

		return FALSE;
	}

	return TRUE;
}

RwBool CItemUpgradeGui::IsValidIconPickup( RwInt32 nClickIdx )
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

RwBool CItemUpgradeGui::IsValidRightBtnProc( RwInt32 nClickIdx )
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

RwBool CItemUpgradeGui::IsEmptySlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx == SLOT_ITEM )
	{
		if( m_pUpgradeItem )
			return FALSE;
		else
			return TRUE;
	}
	else if (nSlotIdx == SLOT_STONE)
	{
		if(m_pStoneItem)
			return FALSE;
		else 
			return TRUE;
	}
	else if (nSlotIdx == SLOT_PROTECTION)
	{
		if (m_pGuardItem)
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}

RwBool CItemUpgradeGui::IsUpgradeItemSlot( RwInt32 nSlotIdx )
{
	return ( SLOT_ITEM == nSlotIdx ) ? TRUE : FALSE;
}

RwBool CItemUpgradeGui::IsHoipoiStoneSlot( RwInt32 nSlotIdx )
{
	return (SLOT_STONE == nSlotIdx ) ? TRUE : FALSE;
}

RwBool CItemUpgradeGui::IsGuardStoneSlot(RwInt32 nSlotIdx)
{
	return (SLOT_PROTECTION == nSlotIdx) ? TRUE : FALSE;
}

CNtlSobItem* CItemUpgradeGui::GetSlotObject( RwInt32 nSlotIdx )
{
	if( IsUpgradeItemSlot( nSlotIdx ) )
		return m_pUpgradeItem;
	else if( IsHoipoiStoneSlot( nSlotIdx ) )
		return m_pStoneItem;
	else if (IsGuardStoneSlot(nSlotIdx))
		return m_pGuardItem;

	return NULL;
}

VOID CItemUpgradeGui::OnMouseUp( const CKey& key )
{
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if (GetIconMoveManager()->IsActive())
		{
			if (GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
			{
				SetItemSlot(GetIconMoveManager()->GetSrcSerial(),
					GetIconMoveManager()->GetSrcPlace(),
					GetIconMoveManager()->GetSrcSlotIdx(),
					m_nLSelectedSlotIdx, TRUE);
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if (!GetIconMoveManager()->IsActive())
		{
			switch (m_nRSelectedSlotIdx)
			{
				case SLOT_STONE:
				{
					DeleteHoipoiStone();
				}
				break;
				case SLOT_PROTECTION:
				{
					DeleteGuardStone();
				}
				break;
				case SLOT_ITEM:
				{
					DeleteUpgradeItem();
				}
				break;

				default: break;
			}
		}

		m_nRSelectedSlotIdx = -1;
	}
}


VOID CItemUpgradeGui::OnPaint(VOID)
{
	if( m_pUpgradeItem )
		m_surSlot[SLOT_ITEM].Render();

	if (m_pStoneItem)
		m_surSlot[SLOT_STONE].Render();

	if (m_pGuardItem)
		m_surSlot[SLOT_PROTECTION].Render();

	for( RwInt32 i = 0 ; i < NUM_SLOT ; ++i )
	{
		if( m_anFocusEffect[i] )
			m_asurFocus[i].Render();

		m_asurDisableSlot[i].Render();
	}	
}

VOID CItemUpgradeGui::OnMove( RwInt32 nXPos, RwInt32 nYPos )
{
	CRectangle rtUpgradeItemScreen = m_ppnlUpgradedItemSlot->GetScreenRect();

	m_surSlot[SLOT_ITEM].SetPosition(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top);
	m_asurFocus[SLOT_ITEM].SetPosition(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top);
	m_asurDisableSlot[SLOT_ITEM].SetPosition(rtUpgradeItemScreen.left, rtUpgradeItemScreen.top);

	CRectangle rtStoneItemScreen = m_ppnlUpgradedStoneSlot->GetScreenRect();

	m_surSlot[SLOT_STONE].SetPosition(rtStoneItemScreen.left, rtStoneItemScreen.top);
	m_asurFocus[SLOT_STONE].SetPosition(rtStoneItemScreen.left, rtStoneItemScreen.top);
	m_asurDisableSlot[SLOT_STONE].SetPosition(rtStoneItemScreen.left, rtStoneItemScreen.top);

	CRectangle rtGuardItemScreen = m_ppnlUpgradedGuardStoneSlot->GetScreenRect();

	m_surSlot[SLOT_PROTECTION].SetPosition(rtGuardItemScreen.left, rtGuardItemScreen.top);
	m_asurFocus[SLOT_PROTECTION].SetPosition(rtGuardItemScreen.left, rtGuardItemScreen.top);
	m_asurDisableSlot[SLOT_PROTECTION].SetPosition(rtGuardItemScreen.left, rtGuardItemScreen.top);

}

VOID CItemUpgradeGui::OnClickExitBtn( gui::CComponent* pComponent )
{
	if( m_eState == STATE_PACKETWAIT )
		return;

	GetDialogManager()->CloseDialog( DIALOG_ITEMUPGRADE );
}

VOID CItemUpgradeGui::OnClickCompoundBtn( gui::CComponent* pComponent )
{
	if( m_eState != STATE_UPGRADE_READY )
		return;

	SetState( STATE_UPGRADE_PROCESS );

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_ITEMUPGRADE )
		GetIconMoveManager()->IconMoveEnd();	
}

VOID CItemUpgradeGui::OnClickCloseResult( gui::CComponent* pComponent )
{
	if (m_pStoneItem == NULL)
	{
		SetState(STATE_UPGRADE_READYFAIL_NOMORESTONES);
		return;
	}

	if( m_pUpgradeItem )
	{
		CNtlSobItemAttr* pHoipoiAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pStoneItem->GetSobAttr() );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pUpgradeItem->GetSobAttr() );

		if( !IsValidUpgradeItem( pItemAttr ) )
		{
			SetState( STATE_UPGRADE_READYFAIL_NOMOREUPGRADE );
		}
		else if( IsValidHoipoiStone( pHoipoiAttr ) )
		{
			SetState( STATE_UPGRADE_READY );
		}					
	}
	else
	{
		SetState(STATE_NONE);
	}

	DestroyResultDialog();
}

VOID CItemUpgradeGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_ITEMUPGRADE, key.m_fX, key.m_fY );
}

VOID CItemUpgradeGui::OnCaptureMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos & pos)
{
	gui::CScrollBar* pScrollBar = m_phtmlInfoText->GetScrollBar();

	RwInt32 nValue = pScrollBar->GetValue();
	RwInt32 nMaxValue = pScrollBar->GetMaxValue();
	RwInt32 nDelta = nValue - (sDelta * 18) / GUI_MOUSE_WHEEL_DELTA;

	if (nDelta < 0)
		nDelta = 0;
	else if (nDelta > nMaxValue)
		nDelta = nMaxValue;

	pScrollBar->SetValue(nDelta);
}


VOID CItemUpgradeGui::OnUpgradeItemIconMouseEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = m_ppnlUpgradedItemSlot->GetScreenRect();

	if( m_pUpgradeItem )
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)GetSlotObject( SLOT_ITEM ), DIALOG_ITEMUPGRADE );		
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, 
			(VOID*)GetDisplayStringManager()->GetString( "DST_ITEMUPGRADE_EX_ITEM_SLOT" ), DIALOG_ITEMUPGRADE );
	}

	m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] | SLOT_FOCUS;
}

VOID CItemUpgradeGui::OnUpgradeItemIconMouseLeave( gui::CComponent* pComponent )
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_anFocusEffect[SLOT_ITEM] = m_anFocusEffect[SLOT_ITEM] & ~SLOT_FOCUS;
	}
}

VOID CItemUpgradeGui::OnUpgradeItemIconMouseDown(const CKey & key)
{
	if (m_asurDisableSlot[SLOT_ITEM].IsShow())
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			m_nLSelectedSlotIdx = SLOT_ITEM;
		}
		else
		{
			if (IsValidIconPickup(SLOT_ITEM))
				m_nLSelectedSlotIdx = SLOT_ITEM;
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (IsValidRightBtnProc(SLOT_ITEM))
			{
				if (m_pUpgradeItem)
					m_nRSelectedSlotIdx = SLOT_ITEM;
			}
		}
	}

	m_pThis->CaptureMouse();

	if (m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0)
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
	}
}


VOID CItemUpgradeGui::OnGuardItemIconMouseEnter(gui::CComponent * pComponent)
{
	CRectangle rtScreen = m_ppnlUpgradedGuardStoneSlot->GetScreenRect();

	if (m_pGuardItem)
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)GetSlotObject(SLOT_PROTECTION), DIALOG_ITEMUPGRADE);
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top,
			(VOID*)GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_EX_GUARD_STONE_SLOT"), DIALOG_ITEMUPGRADE);
	}

	m_anFocusEffect[SLOT_PROTECTION] = m_anFocusEffect[SLOT_PROTECTION] | SLOT_FOCUS;
}

VOID CItemUpgradeGui::OnGuardItemIconMouseLeave(gui::CComponent * pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_anFocusEffect[SLOT_PROTECTION] = m_anFocusEffect[SLOT_PROTECTION] & ~SLOT_FOCUS;
	}
}

VOID CItemUpgradeGui::OnGuardItemIconMouseDown(const CKey & key)
{
	if (m_asurDisableSlot[SLOT_PROTECTION].IsShow())
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			m_nLSelectedSlotIdx = SLOT_PROTECTION;
		}
		else
		{
			if (IsValidIconPickup(SLOT_PROTECTION))
				m_nLSelectedSlotIdx = SLOT_PROTECTION;
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (IsValidRightBtnProc(SLOT_PROTECTION))
			{
				if (m_pGuardItem)
					m_nRSelectedSlotIdx = SLOT_PROTECTION;
			}
		}
	}

	m_pThis->CaptureMouse();

	if (m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0)
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
	}
}

VOID CItemUpgradeGui::OnStoneItemIconMouseEnter(gui::CComponent * pComponent)
{
	CRectangle rtScreen = m_ppnlUpgradedStoneSlot->GetScreenRect();

	if (m_pStoneItem)
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rtScreen.left, rtScreen.top,
			(VOID*)GetSlotObject(SLOT_STONE), DIALOG_ITEMUPGRADE);
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top,
			(VOID*)GetDisplayStringManager()->GetString("DST_ITEMUPGRADE_EX_STONE_SLOT"), DIALOG_ITEMUPGRADE);
	}

	m_anFocusEffect[SLOT_STONE] = m_anFocusEffect[SLOT_STONE] | SLOT_FOCUS;
}

VOID CItemUpgradeGui::OnStoneItemIconMouseLeave(gui::CComponent * pComponent)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_ITEMUPGRADE)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);

		m_anFocusEffect[SLOT_STONE] = m_anFocusEffect[SLOT_STONE] & ~SLOT_FOCUS;
	}
}

VOID CItemUpgradeGui::OnStoneItemIconMouseDown(const CKey & key)
{
	if (m_asurDisableSlot[SLOT_STONE].IsShow())
		return;

	if (key.m_nID == UD_LEFT_BUTTON)
	{
		if (GetIconMoveManager()->IsActive())
		{
			m_nLSelectedSlotIdx = SLOT_STONE;
		}
		else
		{
			if (IsValidIconPickup(SLOT_STONE))
				m_nLSelectedSlotIdx = SLOT_STONE;
		}
	}
	else if (key.m_nID == UD_RIGHT_BUTTON)
	{
		if (!GetIconMoveManager()->IsActive())
		{
			if (IsValidRightBtnProc(SLOT_STONE))
			{
				if (m_pStoneItem)
					m_nRSelectedSlotIdx = SLOT_STONE;
			}
		}
	}

	m_pThis->CaptureMouse();

	if (m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0)
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		m_pThis->ReleaseMouse();
	}
}


VOID CItemUpgradeGui::OnUpgradeResultEffectEnd( gui::CComponent* pComponent )
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