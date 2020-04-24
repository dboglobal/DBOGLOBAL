#include "precomp_dboclient.h"
#include "DogiGui.h"


// shared
#include "NPCTable.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLItem.h"
#include "NtlPLPalette.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobCharProxy.h"
#include "NtlSobCharProxyEquip.h"
#include "NtlCameraController.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSobItemAttr.h"

// client
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "MsgBoxManager.h"


#define dCOLOR_PALETTE_SIZE		(16)
#define dCOLOR_PALETTE_START_X	(137)
#define dCOLOR_PALETTE_START_Y	(334)
#define dCOLOR_GAP_ROW			(dCOLOR_PALETTE_SIZE + 3)
#define dCOLOR_GAP_COLUMN		(dCOLOR_PALETTE_SIZE + 3)

#define dDOGI_SLOT_INDEX		(dMAX_PALETTE + 1)


CDogiGui::CDogiGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bSlotFocus(FALSE)
,m_bSlotDest(FALSE)
,m_bExpectServerAnswer(FALSE)
,m_byLeftMouseDown(INVALID_BYTE)
,m_byRightMouseDown(INVALID_BYTE)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_bySelectedColorIndex(INVALID_BYTE)
,m_byOriginalColorIndex(INVALID_BYTE)
,m_uiDojoTableIndex(INVALID_TBLIDX)
,m_tblDogiItem(INVALID_TBLIDX)
,m_byDogiGrade(0)
{

}

CDogiGui::~CDogiGui()
{
	Destroy();
}

RwBool CDogiGui::Create()
{
	NTL_FUNCTION( "CDogiGui::Create" );

	if(!CNtlPLGui::Create("gui\\Dogi.rsr", "gui\\Dogi.srf", "gui\\Dogi.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pBtnHelp		= (gui::CButton*)GetComponent( "btnHelp" );
	m_pBtnExit		= (gui::CButton*)GetComponent( "btnExit" );
	m_pBtnLeftArrow	= (gui::CButton*)GetComponent( "btnLeftArrow" );
	m_pBtnRightArrow= (gui::CButton*)GetComponent( "btnRightArrow" );
	m_pBtnOK		= (gui::CButton*)GetComponent( "btnOK" );
	m_pBtnCancel	= (gui::CButton*)GetComponent( "btnCancel" );

	m_pDialogName			= (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pPreviewDogiStatic	= (gui::CStaticBox*)GetComponent( "stbPreviewDogi" );
	m_pSetupDogiStatic		= (gui::CStaticBox*)GetComponent( "stbSetupDogi" );	


	m_srfPreviewDogiBar			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfPreviewDogiBar			.SetPositionfromParent(10, 35);

	m_srfSetupDogiBar			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfSetupDogiBar			.SetPositionfromParent(10, 300);

	m_srfCharacterBackground	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dogi.srf", "srfCharacterBackground" ) );
	m_srfCharacterBackground	.SetPositionfromParent(50, 71);

	m_srfDogiBackground			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dogi.srf", "srfDogiBackground" ) );
	m_srfDogiBackground			.SetPositionfromParent(15, 318);

	m_srfSlotFocus				.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_srfSlotFocus				.SetPositionfromParent(48, 351);

	m_srfCharacter.SetRectWH(0, 0, 132, 176);


	RwInt32 iPosY = dCOLOR_PALETTE_START_Y - 1;
	for( RwUInt8 i = 0 ; i < dMAX_COLOR_ROW ; ++i )
	{
		m_srfColorDisable[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Dogi.srf", "srfColorDiable" ) );
		m_srfColorDisable[i].SetPositionfromParent(dCOLOR_PALETTE_START_X - 1, iPosY);

		iPosY += dCOLOR_GAP_ROW;
	}

	// 도복 슬롯
	m_DogiSlot.Create(m_pThis, DIALOG_DOGI, REGULAR_SLOT_ITEM_SOB);
	m_DogiSlot.SetPosition_fromParent(50, 353);


	// default value
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);

	m_tUseColorInfo.bUseAllColor		= FALSE;
	m_tUseColorInfo.byEnableColorRow	= 0;

	// 컴포넌트를 배치
	OnMove(0, 0);

	m_pDialogName		->SetText(GetDisplayStringManager()->GetString("DST_DOGI_SETUP"));
	m_pPreviewDogiStatic->SetText(GetDisplayStringManager()->GetString("DST_DOGI_PREVIEW"));
	m_pSetupDogiStatic	->SetText(GetDisplayStringManager()->GetString("DST_DOGI_COLOR"));

	m_pBtnOK		->SetText(GetDisplayStringManager()->GetString("DST_MSG_BTN_OK"));
	m_pBtnCancel	->SetText(GetDisplayStringManager()->GetString("DST_MSG_BTN_CANCEL"));

	// sig
	m_slotHelpButton				= m_pBtnHelp->SigClicked().Connect(this, &CDogiGui::OnClicked_HelpButton);
	m_slotExitButton				= m_pBtnExit->SigClicked().Connect(this, &CDogiGui::OnClicked_ExitButton);
	m_slotLeftArrowButtonPress		= m_pBtnLeftArrow->SigPressed().Connect( this, &CDogiGui::OnPressed_LeftArrowButton );
	m_slotLeftArrowButtonRelease	= m_pBtnLeftArrow->SigReleased().Connect( this, &CDogiGui::OnReleased_LeftArrowButton );
	m_slotRightArrowButtonPress		= m_pBtnRightArrow->SigPressed().Connect( this, &CDogiGui::OnPressed_RightArrowButton );
	m_slotRightArrowButtonRelease	= m_pBtnRightArrow->SigReleased().Connect( this, &CDogiGui::OnReleased_RightArrowButton );
	m_slotOKButton					= m_pBtnOK->SigClicked().Connect(this, &CDogiGui::OnClicked_OKButton);
	m_slotCancelButton				= m_pBtnCancel->SigClicked().Connect(this, &CDogiGui::OnClicked_ExitButton);
	m_slotMouseDown					= m_pThis->SigMouseDown().Connect( this, &CDogiGui::OnMouseDown );
	m_slotMouseUp					= m_pThis->SigMouseUp().Connect( this, &CDogiGui::OnMouseUp );	
	m_slotMove						= m_pThis->SigMove().Connect( this, &CDogiGui::OnMove );	
	m_slotMouseMove					= m_pThis->SigMouseMove().Connect( this, &CDogiGui::OnMouseMove );
	m_slotMouseLeave				= m_pThis->SigMouseLeave().Connect( this, &CDogiGui::OnMouseLeave );
	m_slotPaint						= m_pThis->SigPaint().Connect( this, &CDogiGui::OnPaint );
	m_slotCaptureMouseDown			= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CDogiGui::OnCaptureMouseDown );

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDirectMoveIcon);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventIconMoveClick);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventNotifyGuild);	
	LinkMsg(g_EventEnableItemIcon);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDogiGui::Destroy()
{
	NTL_FUNCTION("CDogiGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateBeforeFuncWithoutRefCheck( this );

	m_DogiSlot.Destroy();

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDirectMoveIcon);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventEnableItemIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDogiGui::Clear()
{
	GetNtlGuiManager()->RemoveUpdateBeforeFuncWithoutRefCheck( this );		
	CheckInfoWindow();	

	UnsetDogi();

	if( m_bySelectedColorIndex	!= m_byOriginalColorIndex &&
		(RwUInt8)INVALID_BYTE	!= m_byOriginalColorIndex )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( pAvatar )
		{
			Logic_SetDogiItemColor( reinterpret_cast<CNtlSobPlayer*>( pAvatar ), m_byOriginalColorIndex, TRUE );
		}
		else
		{
			DBO_FAIL("Not exist sob avatar instance");
		}
	}			

	m_bSlotFocus			= FALSE;
	m_bSlotDest				= FALSE;
	m_bExpectServerAnswer	= FALSE;
	m_byLeftMouseDown		= INVALID_BYTE;
	m_byRightMouseDown		= INVALID_BYTE;
	m_hNPCSerial			= INVALID_SERIAL_ID;
	m_bySelectedColorIndex	= (RwUInt8)INVALID_BYTE;
	m_byOriginalColorIndex	= (RwUInt8)INVALID_BYTE;
	m_uiDojoTableIndex		= INVALID_TBLIDX;

	m_tUseColorInfo.bUseAllColor		= FALSE;
	m_tUseColorInfo.byEnableColorRow	= 0;
}

VOID CDogiGui::UpdateBeforeCamera( RwReal fElapsed )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( !pCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return;
	}

	m_texCharacter.Load( pCharProxy->UIPcDogiWndRender() );
	m_srfCharacter.SetTexture( &m_texCharacter );
}

RwBool CDogiGui::SetDogi(SERIAL_HANDLE hDogi)
{
	if( m_bExpectServerAnswer )
		return FALSE;

	UnsetDogi();

	if( false == m_DogiSlot.SetIcon(hDogi) )
	{
		DBO_FAIL("Can not register dogi item");
		return FALSE;
	}


	CNtlSobItem* pSobItem = m_DogiSlot.GetSobItem();
	if( !pSobItem )
	{
		DBO_FAIL("Invalid item pointer");
		UnsetDogi();
		return FALSE;
	}

	RwInt32 iParentSlotIndex	= pSobItem->GetParentItemSlotIdx();
	RwInt32 iSlotIndex			= pSobItem->GetItemSlotIdx();

	CDboEventGenerator::EnableItemIcon(FALSE, PLACE_BAG, iSlotIndex, iParentSlotIndex);


	if( !m_tUseColorInfo.bUseAllColor )
	{
		sITEM_TBLDAT* pITEM_TBLDAT = m_DogiSlot.GetItemTable();
		if( !pITEM_TBLDAT )
		{
			DBO_FAIL("Not exist item table");
			UnsetDogi();
			return FALSE;
		}

		switch(pITEM_TBLDAT->contentsTblidx)
		{
		case 8:
			{
				m_tUseColorInfo.byEnableColorRow = 1;
				break;
			}
		case 16:
			{
				m_tUseColorInfo.byEnableColorRow = 2;
				break;
			}
		case 24:
			{
				m_tUseColorInfo.byEnableColorRow = 3;
				break;
			}
		case 32:
			{
				m_tUseColorInfo.byEnableColorRow = dMAX_COLOR_ROW;
				break;
			}
		default:
			{
				DBO_FAIL("Invalid dogi color type" << pITEM_TBLDAT->byItem_Type);
				UnsetDogi();
				return FALSE;
			}
		}
	}	

	RegisterDogiItem( m_DogiSlot.GetItemTable()->tblidx, m_DogiSlot.GetSobItemAttr()->GetGrade() );

	return TRUE;
}

VOID CDogiGui::UnsetDogi()
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(m_DogiSlot.GetSerial()) );
	if( !pSobItem )
		return;

	RwInt32 iParentSlotIndex	= pSobItem->GetParentItemSlotIdx();
	RwInt32 iSlotIndex			= pSobItem->GetItemSlotIdx();

	if( (RwUInt8)INVALID_BYTE != iParentSlotIndex && (RwUInt8)INVALID_BYTE != iSlotIndex )
		CDboEventGenerator::EnableItemIcon(TRUE, PLACE_BAG, iSlotIndex, iParentSlotIndex);

	m_DogiSlot.Clear();
	UnregisterDogiIem();
}

VOID CDogiGui::SelectColor(RwUInt8 byIndex)
{
	if( dMAX_PALETTE <= byIndex )
		return;

	if( dMAX_COLOR_COLUMN*m_tUseColorInfo.byEnableColorRow <= byIndex )
		return;

	if( m_DogiSlot.GetSerial() == INVALID_SERIAL_ID )
	{
		GetAlarmManager()->AlarmMessage("DST_DOGI_MUST_REGI_DOGI_SLOT");
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar )
		Logic_SetDogiItemColor( reinterpret_cast<CNtlSobPlayer*>( pAvatar ), byIndex, TRUE );

	m_bySelectedColorIndex = byIndex;
}

VOID CDogiGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui()== DIALOG_DOGI )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

RwUInt8 CDogiGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	RwInt32 iPosX = dCOLOR_PALETTE_START_X;
	RwInt32 iPosY = dCOLOR_PALETTE_START_Y;

	for( RwUInt8 i = 0 ; i < dMAX_COLOR_ROW ; ++i )
	{
		for( RwUInt8 k = 0 ; k < dMAX_COLOR_COLUMN ; ++k )
		{
			if( iPosX <= iX && iX < (iPosX + dCOLOR_PALETTE_SIZE) &&
				iPosY <= iY && iY < (iPosY + dCOLOR_PALETTE_SIZE) )
			{
				return i*dMAX_COLOR_COLUMN + k;
			}

			iPosX += dCOLOR_GAP_COLUMN;
		}

		iPosX = dCOLOR_PALETTE_START_X;
		iPosY += dCOLOR_GAP_ROW;
	}

	if( m_DogiSlot.PtInRect(iX, iY) )
		return dDOGI_SLOT_INDEX;

	return INVALID_BYTE;
}

VOID CDogiGui::RegisterDogiItem( TBLIDX tblIdx, RwUInt8 byGrade )
{
	UnregisterDogiIem();

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

	if ( pCharProxy )
	{
		SEquipItem* pEquipItem = pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->GetEquipItem( EQUIP_SLOT_TYPE_QUEST );

		if ( pEquipItem )
		{
			m_tblDogiItem = pEquipItem->uiItemTblId;
			m_byDogiGrade = pEquipItem->byGrade;

			RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();
			GetNtlResourceManager()->SetLoadScheduling( FALSE );

			pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->DeleteEquipItem(EQUIP_SLOT_TYPE_QUEST);
			pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->CreateEquipItem( tblIdx, EQUIP_SLOT_TYPE_QUEST, byGrade );

			GetNtlResourceManager()->SetLoadScheduling( bScheduling );
		}
	}
}

VOID CDogiGui::UnregisterDogiIem( void )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

	if ( pCharProxy )
	{
		SEquipItem* pEquipItem = pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->GetEquipItem(EQUIP_SLOT_TYPE_QUEST);

		if ( pEquipItem && m_tblDogiItem != pEquipItem->uiItemTblId )
		{
			RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();
			GetNtlResourceManager()->SetLoadScheduling( FALSE );

			pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->DeleteEquipItem(EQUIP_SLOT_TYPE_QUEST);
			pCharProxy->GetUIDogiCharacter()->pUIEquipProxy->CreateEquipItem( m_tblDogiItem, EQUIP_SLOT_TYPE_QUEST, m_byDogiGrade );

			GetNtlResourceManager()->SetLoadScheduling( bScheduling );
		}

		m_tblDogiItem = INVALID_TBLIDX;
		m_byDogiGrade = 0;
	}
}

VOID CDogiGui::OnClicked_HelpButton( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent(DIALOG_DOGI);
}

VOID CDogiGui::OnClicked_ExitButton( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog(DIALOG_DOGI);
}

VOID CDogiGui::OnPressed_LeftArrowButton( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( !pCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return;
	}
	
	pCharProxy->PcDogiRotateRight();	
}

VOID CDogiGui::OnPressed_RightArrowButton( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( !pCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return;
	}
	
	pCharProxy->PcDogiRotateLeft();
}

VOID CDogiGui::OnReleased_LeftArrowButton( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( !pCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return;
	}

	pCharProxy->PcDogiRotateStop();
}

VOID CDogiGui::OnReleased_RightArrowButton( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( !pCharProxy )
	{
		DBO_FAIL("Not exist avatar proxy");
		return;
	}

	pCharProxy->PcDogiRotateStop();
}

VOID CDogiGui::OnClicked_OKButton( gui::CComponent* pComponent )
{
	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(m_hNPCSerial);
	if( !pNPC_TBLDAT )
	{
		DBO_FAIL("Not exist npc table of handle : " << m_hNPCSerial);
		return;
	}


	sMsgBoxData msgExData;
	msgExData.hHandle		= m_DogiSlot.GetSerial();
	msgExData.byIndex		= m_bySelectedColorIndex;

	if( NPC_JOB_GUILD_MANAGER == pNPC_TBLDAT->byJob )
	{
		GetAlarmManager()->AlarmMessage("DST_DOGI_ASK_APPLY_CHANGE", FALSE, &msgExData);

		if( GetMsgBoxManager()->GetMsgBox("DST_DOGI_ASK_APPLY_CHANGE") )
			m_bExpectServerAnswer = TRUE;
	}
	else if( NPC_JOB_DOJO_MANAGER == pNPC_TBLDAT->byJob )
	{
		GetAlarmManager()->AlarmMessage("DST_DOJO_DOGI_ASK_APPLY_CHANGE", FALSE, &msgExData);

		if( GetMsgBoxManager()->GetMsgBox("DST_DOJO_DOGI_ASK_APPLY_CHANGE") )
			m_bExpectServerAnswer = TRUE;
	}
	else
	{
		DBO_FAIL("Wrong npc job type" << pNPC_TBLDAT->byJob);
	}	
}

VOID CDogiGui::OnMouseDown(const CKey& key)
{
	RwUInt8 bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( bySlot == INVALID_BYTE )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		m_byLeftMouseDown = bySlot;
		m_pThis->CaptureMouse();			
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		m_byRightMouseDown = bySlot;
		m_pThis->CaptureMouse();		
	}
}

VOID CDogiGui::OnMouseUp(const CKey& key)
{
	m_pThis->ReleaseMouse();

	RwUInt8 bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( bySlot == INVALID_BYTE )
	{
		m_byLeftMouseDown	= INVALID_BYTE;
		m_byRightMouseDown	= INVALID_BYTE;
		return;
	}
	

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_byLeftMouseDown == bySlot )
		{
			if( dDOGI_SLOT_INDEX == m_byLeftMouseDown )
			{
				if( GetIconMoveManager()->IsActive() )
				{
					if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
					{
						SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
						sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hItem);

						if( pITEM_TBLDAT )
						{
							if( ITEM_TYPE_DOGI == pITEM_TBLDAT->byItem_Type )
							{
								// 가방에서 도복 아이템을 등록했다
								if( SetDogi( GetIconMoveManager()->GetSrcSerial() ) )
								{
									GetIconMoveManager()->IconMoveEnd();
								}								
							}
							else
							{
								GetAlarmManager()->AlarmMessage("DST_DOGI_ONLY_REGISTER_DOGI_ITEM");
							}
						}
						else
						{
							DBO_FAIL("Not exist ITEM TABLE of handle : " << hItem);
						}	
					}
				}
			}
			else if( m_byLeftMouseDown < dMAX_PALETTE )
			{
				if( FALSE == GetIconMoveManager()->IsActive() )
				{
					// 팔레트를 선택했다
					SelectColor(m_byLeftMouseDown);
				}
			}
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( FALSE == GetIconMoveManager()->IsActive() )
		{
			if( m_byRightMouseDown == bySlot )
			{
				// 등록된 도복을 지운다
				CheckInfoWindow();
				UnsetDogi();				

				if( !m_tUseColorInfo.bUseAllColor )
					m_tUseColorInfo.byEnableColorRow = 0;
			}
		}
	}

	m_byLeftMouseDown	= INVALID_BYTE;
	m_byRightMouseDown	= INVALID_BYTE;
}

VOID CDogiGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	
	m_srfPreviewDogiBar			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSetupDogiBar			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCharacterBackground	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfDogiBackground			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlotFocus				.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfCharacter.SetPosition(rtScreen.left + 94, rtScreen.top + 99);

	for( RwUInt8 i = 0 ; i < dMAX_COLOR_ROW ; ++i )
		m_srfColorDisable[i].SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_DogiSlot.SetParentPosition(rtScreen.left, rtScreen.top);
}

VOID CDogiGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_DogiSlot.PtInRect(nX, nY) )
	{
		m_bSlotFocus = TRUE;

		if( m_DogiSlot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_ITEM &&
				GetInfoWndManager()->GetRequestGui() != DIALOG_DOGI )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
													rtScreen.left + m_DogiSlot.GetX_fromParent(),
													rtScreen.top + m_DogiSlot.GetY_fromParent(),
													m_DogiSlot.GetSobItem(), DIALOG_DOGI );				
			}
		}

		return;
	}

	m_bSlotFocus = FALSE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CDogiGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_bSlotFocus = FALSE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CDogiGui::OnPaint()
{
	m_srfPreviewDogiBar			.Render();
	m_srfSetupDogiBar			.Render();
	m_srfCharacterBackground	.Render();
	m_srfDogiBackground			.Render();
	m_srfCharacter				.Render();


	RwUInt8 byEnableColorRow = 0;

	if( m_tUseColorInfo.bUseAllColor )
		byEnableColorRow = dMAX_COLOR_ROW;
	else
		byEnableColorRow = m_tUseColorInfo.byEnableColorRow;

	for( RwUInt8 i = byEnableColorRow ; i < dMAX_COLOR_ROW ; ++i )
		m_srfColorDisable[i].Render();

	m_DogiSlot.Paint();


	if( m_bSlotFocus || m_bSlotDest )
		m_srfSlotFocus.Render();
}

VOID CDogiGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_DOGI, key.m_fX, key.m_fY);
}

RwInt32 CDogiGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{		
		GetNtlGuiManager()->AddUpdateBeforeFunc( this );

		sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(m_hNPCSerial);
		if( !pNPC_TBLDAT )
		{
			DBO_WARNING_MESSAGE("Not exist npc table of handle : " << m_hNPCSerial);
			Logic_CancelNpcFacing();
			return -1;
		}

		switch( pNPC_TBLDAT->byJob )
		{
		case NPC_JOB_DOJO_MANAGER:
			{
				m_tUseColorInfo.bUseAllColor		= TRUE;
				m_tUseColorInfo.byEnableColorRow	= dMAX_COLOR_ROW;
				break;
			}
		case NPC_JOB_GUILD_MANAGER:
			{
				m_tUseColorInfo.bUseAllColor		= FALSE;
				break;
			}
		default:
			{
				DBO_WARNING_MESSAGE("Wrong npc job type" << pNPC_TBLDAT->byJob);
				Logic_CancelNpcFacing();
				return -1;
			}
		}

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_WARNING_MESSAGE("Not exist sob avatar instance");
			Logic_CancelNpcFacing();
			return -1;
		}
			
		m_byOriginalColorIndex = Logic_GetDogiItemColor( reinterpret_cast<CNtlSobPlayer*>( pAvatar ), TRUE );

		m_uiDojoTableIndex = pNPC_TBLDAT->contentsTblidx;
	}
	else
	{
		Clear();

		Logic_CancelNpcFacing();

		GetMsgBoxManager()->DeleteMsgBox("DST_DOGI_ASK_APPLY_CHANGE");
		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_DOGI_ASK_APPLY_CHANGE");
	}

	Show(bOpen);
	return 1;
}

VOID CDogiGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDogiGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pEvent = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pEvent->eDialog != DIALOG_DOGI )
			return;

		// 같은 NPC
		if( m_hNPCSerial == pEvent->hSerialId )
			return;

		// 다른 NPC와 대화를 하는 경우이다
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			GetDialogManager()->CloseDialog(DIALOG_DOGI);
			return;
		}

		m_hNPCSerial = pEvent->hSerialId;

		if( FALSE == GetDialogManager()->OpenDialog(DIALOG_DOGI, m_hNPCSerial) )
			m_hNPCSerial = INVALID_SERIAL_ID;
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC 서버가 다운되거나 하여 갑자기 NPC가 사라지는 경우
			GetDialogManager()->CloseDialog(DIALOG_DOGI);
		}
	}
	else if( msg.Id == g_EventDirectMoveIcon )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOGI) )
			return;

		SDboEventDirectMoveIcon* pEvent = reinterpret_cast<SDboEventDirectMoveIcon*>( msg.pData );

		if( PLACE_DOGI != pEvent->eDestPlace )
			NTL_RETURNVOID();		

		CheckInfoWindow();
		SetDogi(pEvent->uiSrcSerial);
	}
	else if( msg.Id == g_EventDialog )
	{		
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_DOGI )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE ||
				pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				GetDialogManager()->CloseDialog(DIALOG_DOGI);
			}
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUp = (RwBool)msg.pData;
		if( bPickUp && GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
		{
			SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
			sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hItem);

			if( pITEM_TBLDAT )
			{
				if( ITEM_TYPE_DOGI == pITEM_TBLDAT->byItem_Type )
				{
					m_bSlotDest = TRUE;
					NTL_RETURNVOID();
				}
			}
			else
			{
				DBO_FAIL("Not exist item table of handle : " << hItem);
			}

			m_bSlotDest = FALSE;
		}
		else
		{
			m_bSlotDest = FALSE;
		}
	}
	else if( msg.Id == g_EventDojoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOGI) )
			return;

		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( pEvent->uiParam != m_uiDojoTableIndex )
			NTL_RETURNVOID();

		if( DOJO_EVENT_LOST_DOJO != pEvent->byDojoEvent )
			NTL_RETURNVOID();

		Clear();
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOGI) )
			return;

		if( m_hNPCSerial == INVALID_SERIAL_ID )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		if( FALSE == Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) )
			NTL_RETURNVOID();


		if( INVALID_SERIAL_ID != m_DogiSlot.GetSerial() )
		{
			if( NULL == GetNtlSobManager()->GetSobObject(m_DogiSlot.GetSerial()) )
			{
				// 등록했던 도복이 사라졌다
				CheckInfoWindow();
				UnsetDogi();
			}
		}
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );
		if( SLGE_DOGI == pEvent->iMessage )
			m_bExpectServerAnswer = FALSE;
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( false == GetDialogManager()->IsOpenDialog(DIALOG_DOGI) )
			NTL_RETURNVOID();

		SDboEventEnableItemIcon* pEvent = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );
		if( PLACE_DOJO_UPRAGE != pEvent->ePlace )
			NTL_RETURNVOID();

		m_bExpectServerAnswer = !pEvent->bEnable;
	}

	NTL_RETURNVOID();
}