#include "precomp_dboclient.h"
#include "GuildWarehouseGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NPCTable.h"
#include "ItemTable.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlGuildWarehouse.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "ChatGui.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "PopupManager.h"


namespace
{
#define dGUILDWAREHOUSE_ADDON_INDEX		0

#define dSLOT_ROW			4
#define dSLOT_COLUMN		4

#define dSLOT_START_X		19
#define dSLOT_START_Y		31

#define dSLOT_VER_GAP		40
#define dSLOT_HORI_GAP		40

#define dHEIGHT_GAP			30		//Height difference between warehouse and general warehouse where zeni can be stored
};

CGuildWarehouseBar* CGuildWarehouseBar::m_pInstance = NULL;


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseGui
//////////////////////////////////////////////////////////////////////////

CGuildWarehouseGui::CGuildWarehouseGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(INVALID_INDEX)
,m_iSelectedSlot(INVALID_INDEX)
,m_iClickEffectedSlot(INVALID_INDEX)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
{

}


CGuildWarehouseGui::~CGuildWarehouseGui()
{

}

RwBool CGuildWarehouseGui::Create()
{
	NTL_FUNCTION( "CGuildWarehouseGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\32Bag.srf", "gui\\GuildWarehouse.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	Init();	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGuildWarehouseGui::Destroy()
{
	NTL_FUNCTION("CGuildWarehouseGui::Destroy");

	CheckInfoWindow();	

	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_ITEM_SLOT ; ++i)
	{
		m_Slot[i].Destroy();
	}

	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventPickedUpHide);
	UnLinkMsg(g_EventEnableItemIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGuildWarehouseGui::Init()
{
	CRectangle rect;

	// 다이얼로그 이름 스태틱
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 175, 16);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->Enable(false);

	if( m_byWarehouseIndex == 0 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_GUILDWAREHOUSE_TITLE_1"));
	else if( m_byWarehouseIndex == 1 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_GUILDWAREHOUSE_TITLE_2"));
	else if( m_byWarehouseIndex == 2 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_GUILDWAREHOUSE_TITLE_3"));

	// 창닫기 버튼
	m_pExitButton= (gui::CButton*)GetComponent("btnExit");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CGuildWarehouseGui::ClickedCloseButton);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect") );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	SetupItems();

	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventPickedUpHide);
	LinkMsg(g_EventEnableItemIcon);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CGuildWarehouseGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CGuildWarehouseGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CGuildWarehouseGui::OnMove);
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CGuildWarehouseGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CGuildWarehouseGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CGuildWarehouseGui::OnPaint );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CGuildWarehouseGui::OnCaptureMouseDown );
}

VOID CGuildWarehouseGui::RegisterWarehouseIndex(RwUInt8 byIndex)
{
	m_byWarehouseIndex = byIndex;
}

VOID CGuildWarehouseGui::SetItem(sGuildWarehouseSlot* pSlotData, RwUInt8 byPos)
{
	NTL_ASSERT(pSlotData, "CGuildWarehouseGui::SetItem, Invalid guild warehouse slot data pointer");

	if( pSlotData->hHandle == INVALID_SERIAL_ID )
		m_Slot[byPos].Clear();
	else
	{
		m_Slot[byPos].SetIcon(pSlotData->pITEM_TBLDAT->tblidx, pSlotData->byStackcount);
	}
}

VOID CGuildWarehouseGui::SetupItems()
{
	NTL_ASSERT(0 <= m_byWarehouseIndex && m_byWarehouseIndex < NTL_MAX_GUILD_BANK_COUNT, "CGuildWarehouseGui::SetupItems, Invalud guild warehouse of index : " << m_byWarehouseIndex);

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	CRectangle rtRect = GetPosition();

	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_ITEM_SLOT ; ++i)
	{
		sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(m_byWarehouseIndex, i);

		if( pSlotData->hHandle == INVALID_SERIAL_ID )
		{
			m_Slot[i].Create(m_pThis, (eDialogType)(DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex), REGULAR_SLOT_ITEM_TABLE, SDS_COUNT | SDS_LOCK);
		}
		else
		{
			if( pSlotData->bNeedToIdentify )
				m_Slot[i].Create(m_pThis, (eDialogType)(DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex), REGULAR_SLOT_ITEM_NOT_IDENTIFICATION, SDS_COUNT | SDS_LOCK);
			else
				m_Slot[i].Create(m_pThis, (eDialogType)(DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex), REGULAR_SLOT_ITEM_TABLE, SDS_COUNT | SDS_LOCK);

			m_Slot[i].SetIcon(pSlotData->pITEM_TBLDAT->tblidx, pSlotData->byStackcount);
		}

		if( m_byWarehouseIndex == dGUILDWAREHOUSE_ADDON_INDEX )
			m_Slot[i].SetPosition_fromParent(dSLOT_START_X + dSLOT_VER_GAP*(i%dSLOT_ROW), dSLOT_START_Y + dHEIGHT_GAP + dSLOT_HORI_GAP*(i/dSLOT_COLUMN));
		else
			m_Slot[i].SetPosition_fromParent(dSLOT_START_X + dSLOT_VER_GAP*(i%dSLOT_ROW), dSLOT_START_Y + dSLOT_HORI_GAP*(i/dSLOT_COLUMN));

		m_Slot[i].SetParentPosition(rtRect.left, rtRect.top);
	}
}

VOID CGuildWarehouseGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex);
}

RwInt8 CGuildWarehouseGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_ITEM_SLOT ; ++i)
	{
		if( m_Slot[i].PtInRect(iX, iY) )
		{
			return i;
		}
	}	

	return INVALID_BYTE;
}

VOID CGuildWarehouseGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iX = dSLOT_START_X + dSLOT_VER_GAP*(iSlotIdx%dSLOT_COLUMN);
		RwInt32 iY;
		CRectangle rect = m_pThis->GetScreenRect();

		if( m_byWarehouseIndex == dGUILDWAREHOUSE_ADDON_INDEX )
			iY = dSLOT_START_Y + dHEIGHT_GAP + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);
		else
			iY = dSLOT_START_Y + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);


		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
		m_bFocus = FALSE;
}

VOID CGuildWarehouseGui::SelectEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iX = dSLOT_START_X + dSLOT_VER_GAP*(iSlotIdx%dSLOT_COLUMN);
		RwInt32  iY;
		CRectangle rect = m_pThis->GetScreenRect();

		if( m_byWarehouseIndex == dGUILDWAREHOUSE_ADDON_INDEX )
			iY = dSLOT_START_Y + dHEIGHT_GAP + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);
		else
			iY = dSLOT_START_Y + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);

		m_iSelectedSlot = iSlotIdx;

		m_SelectEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
	}
	else
	{
		m_iSelectedSlot = INVALID_INDEX;
	}
}

VOID CGuildWarehouseGui::ShowDisableSlot(RwBool bShow, RwUInt8 bySlot)
{
	m_Slot[bySlot].ShowLock(bShow);
}

VOID CGuildWarehouseGui::CheckInfoWindow()
{
	if(	GetInfoWndManager()->GetRequestGui() == DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex )
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CGuildWarehouseGui::OnMouseDown(const CKey& key)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	RwInt8 iPtinSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( iPtinSlot != INVALID_INDEX )
	{
		m_iMouseDownSlot = iPtinSlot;
		m_pThis->CaptureMouse();

		// 클릭 이벤트 시작
		m_iClickEffectedSlot = iPtinSlot;	
		m_Slot[m_iClickEffectedSlot].ClickEffect(TRUE);
	}
	else
	{
		if( m_iClickEffectedSlot != INVALID_INDEX )
			m_Slot[m_iClickEffectedSlot].ClickEffect(TRUE);

		m_iMouseDownSlot = INVALID_INDEX;
		m_iClickEffectedSlot = INVALID_INDEX;
	}
}

VOID CGuildWarehouseGui::OnMouseUp(const CKey& key)
{
	m_pThis->ReleaseMouse();

	// 클릭 이벤트 종료	
	if( m_iClickEffectedSlot != INVALID_INDEX )
	{		
		m_Slot[m_iClickEffectedSlot].ClickEffect(FALSE);
		m_iClickEffectedSlot = INVALID_INDEX;
	}

	if( !IsShow() )
	{
		m_iClickEffectedSlot = INVALID_INDEX;
		return;
	}

	if( m_iMouseDownSlot == INVALID_INDEX )
		return;

	if( m_Slot[m_iMouseDownSlot].PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( m_Slot[m_iMouseDownSlot].IsShowLock() == FALSE )
		{
			if( key.m_nID == UD_LEFT_BUTTON )
			{
				CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

				if( GetIconMoveManager()->IsActive() )
				{
					// 창고로 물건을 옮긴다. 선택한 아이템의 부모 핸들이 아니라 창고의 인덱스를 보낸다
					GetIconMoveManager()->IconMovePutDown(PLACE_GUILD_WAREHOUSE, m_byWarehouseIndex, m_iMouseDownSlot);
				}
				else
				{					
					if(m_Slot[m_iMouseDownSlot].GetSerial() != INVALID_SERIAL_ID)
					{
						sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(m_byWarehouseIndex, (RwUInt8)m_iMouseDownSlot);
						NTL_ASSERT(pSlotData, "CGuildWarehouseGui::OnMouseUp, Not exist guild warehouse item of index : " << m_byWarehouseIndex << " of slot : " << m_iMouseDownSlot);

						if( key.m_dwVKey & UD_MK_CONTROL )
						{
							// 창고에서 아이템을 나누기 위해 계산기를 연다
							CRectangle rtScreen = m_pThis->GetScreenRect();
							CDboEventGenerator::CalcPopupShow( TRUE, pSlotData->hHandle, PLACE_GUILD_WAREHOUSE, rtScreen.left, rtScreen.top, m_Slot[m_iMouseDownSlot].GetCount() );
						}
						else
						{
							// 창고에서 물건을 집는다.
							GetIconMoveManager()->IconMovePickUp(pSlotData->hHandle, PLACE_GUILD_WAREHOUSE,
								m_iMouseDownSlot, pSlotData->byStackcount, m_Slot[m_iMouseDownSlot].GetTexture(), pSlotData->pITEM_TBLDAT->tblidx);

							SelectEffect(TRUE, m_iMouseDownSlot);
						}						
					}				
				}
			}
			else if( key.m_nID == UD_RIGHT_BUTTON )
			{
				if( GetIconMoveManager()->IsActive() == FALSE && m_Slot[m_iMouseDownSlot].GetSerial() != INVALID_SERIAL_ID )
				{
					RwUInt32 uiBagIndex;
					SERIAL_HANDLE hBagHandle;
					RwUInt32 uiSlot_of_Bag;

					Logic_GetFirstEmptySlot_of_Bag(uiBagIndex, hBagHandle, uiSlot_of_Bag);

					if( uiBagIndex != INVALID_INDEX)
					{
						CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
						sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(m_byWarehouseIndex, (RwUInt8)m_iMouseDownSlot);
						NTL_ASSERT(pSlotData, "CGuildWarehouseGui::OnMouseUp, Not exist guild warehouse item of index : " << m_byWarehouseIndex << " of slot : " << m_iMouseDownSlot);

						// 바로 가방으로 옮긴다
						Logic_ItemMoveProc(pSlotData->hHandle, PLACE_GUILD_WAREHOUSE, (RwUInt8)m_iMouseDownSlot,
							PLACE_BAG, hBagHandle, (RwUInt8)uiSlot_of_Bag, m_Slot[m_iMouseDownSlot].GetCount());

						SelectEffect(FALSE);
					}
				}
			}
		}
	}	

	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CGuildWarehouseGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_ITEM_SLOT ; ++i )
	{
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
	}

	m_SelectEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_FocusEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CGuildWarehouseGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt8 iPtinSlot = PtinSlot(nX, nY);

	if( iPtinSlot != INVALID_INDEX )
	{
		FocusEffect(TRUE, iPtinSlot);

		// 슬롯 클릭 이펙트
		if( m_iClickEffectedSlot != INVALID_INDEX )
		{
			if( m_iClickEffectedSlot == iPtinSlot )
				m_Slot[m_iClickEffectedSlot].ClickEffect(TRUE);
			else
				m_Slot[m_iClickEffectedSlot].ClickEffect(FALSE);
		}

		if( m_Slot[iPtinSlot].GetSerial() != INVALID_SERIAL_ID )
		{
			if(m_Slot[iPtinSlot].GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION)
			{
				if( m_byInfoWindowIndex != iPtinSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
					sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(m_byWarehouseIndex, iPtinSlot);
					NTL_ASSERT(pSlotData, "CGuildWarehouseGui::OnMouseMove, Not exist guild warehouse item of index " << m_byWarehouseIndex << " of slot : " << iPtinSlot);

					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
						rtScreen.left + m_Slot[iPtinSlot].GetX_fromParent(),
						rtScreen.top + m_Slot[iPtinSlot].GetY_fromParent(),
						pSlotData->pITEM_TBLDAT, DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex );
					m_byInfoWindowIndex = iPtinSlot;
				}
			}
			else
			{
				if( m_byInfoWindowIndex != iPtinSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
					sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(m_byWarehouseIndex, iPtinSlot);
					NTL_ASSERT(pSlotData, "CGuildWarehouseGui::OnMouseMove, Not exist guild warehouse item of index " << m_byWarehouseIndex << " of slot : " << iPtinSlot);
					sITEM_PROFILE item_Profile;

					item_Profile.handle				= pSlotData->hHandle;
					item_Profile.tblidx				= pSlotData->pITEM_TBLDAT->tblidx;
					item_Profile.byPlace			= pSlotData->byServerPlace;
					item_Profile.byPos				= pSlotData->byPos;
					item_Profile.byStackcount		= pSlotData->byStackcount;
					item_Profile.byRank				= pSlotData->byRank;
					item_Profile.byCurDur			= pSlotData->byCurDur;
					item_Profile.bNeedToIdentify	= B2b(pSlotData->bNeedToIdentify);
					item_Profile.byGrade			= pSlotData->byGrade;
					item_Profile.byBattleAttribute	= pSlotData->byBattleAttribute;
					item_Profile.byRestrictState = pSlotData->byRestrictState;

					memcpy(&item_Profile.sOptionSet, &pSlotData->sOptionSet, sizeof(sITEM_OPTION_SET));

					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM,
						rtScreen.left + m_Slot[iPtinSlot].GetX_fromParent(),
						rtScreen.top + m_Slot[iPtinSlot].GetY_fromParent(),
						&item_Profile, DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex );
					m_byInfoWindowIndex = iPtinSlot;
				}
			}			
		}
		else
		{
			m_byInfoWindowIndex = INVALID_BYTE;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}
	else
	{
		FocusEffect(FALSE);

		if( m_iClickEffectedSlot != INVALID_INDEX )
			m_Slot[m_iClickEffectedSlot].ClickEffect(FALSE);

		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CGuildWarehouseGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CGuildWarehouseGui::OnPaint()
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_ITEM_SLOT ; ++i )
	{
		m_Slot[i].Paint();
	}

	if( m_iSelectedSlot != INVALID_INDEX )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CGuildWarehouseGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_GUILD_WAREHOUSE_1 + m_byWarehouseIndex, key.m_fX, key.m_fY);
}

RwInt32 CGuildWarehouseGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);

		SelectEffect(FALSE);
		CheckInfoWindow();
	}	

	return 1;
}

VOID CGuildWarehouseGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGuildWarehouseGui::HandleEvents");

	if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pEvent = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pEvent->nSrcPlace == PLACE_GUILD_WAREHOUSE )
		{
			CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

			// 3개의 창고중에 해당 아이템을 가지고 있는 것을 찾는다
			sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(pEvent->uiSerial);
			NTL_ASSERT(pSlotData, "CGuildWarehouseGui::HandleEvents, Not exist item of handle : " << pEvent->uiSerial <<" in guild warehouse");

			if( m_byWarehouseIndex == (pSlotData->byServerPlace - CONTAINER_TYPE_GUILD_FIRST) )
				GetIconMoveManager()->IconMovePickUp(pSlotData->hHandle, PLACE_GUILD_WAREHOUSE, pSlotData->byPos,
													 pEvent->uiValue, m_Slot[pSlotData->byPos].GetTexture());
		}
	}
	else if(msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pEvent = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

		if( pEvent->nSrcPlace != PLACE_GUILD_WAREHOUSE )
			return;

		// 3개의 창고중에 해당 아이템을 가지고 있는 것을 찾는다
		CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
		sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(pEvent->uiSerial);
		NTL_ASSERT(pSlotData, "CGuildWarehouseGui::HandleEvents, Not exist item of handle : " << pEvent->uiSerial <<" in guild warehouse");

		if( pSlotData->hHandle != pEvent->uiSerial )
			NTL_RETURNVOID();

		if( pEvent->nWorkId != PMW_PICKUP )
			NTL_RETURNVOID();
	
		if( m_byWarehouseIndex == (pSlotData->byServerPlace - CONTAINER_TYPE_GUILD_FIRST) )
			GetIconMoveManager()->IconMovePickUp(pEvent->uiSerial, PLACE_GUILD_WAREHOUSE, pEvent->nSrcSlotIdx,
												 m_Slot[pEvent->nSrcSlotIdx].GetCount(), m_Slot[pEvent->nSrcSlotIdx].GetTexture() );
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_GUILD_WAREHOUSE )
			NTL_RETURNVOID();

		SelectEffect(FALSE);
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pEvent = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pEvent->ePlace != PLACE_GUILD_WAREHOUSE )
			NTL_RETURNVOID();

		if( m_byWarehouseIndex != pEvent->nPlaceIdx )
			NTL_RETURNVOID();

		ShowDisableSlot(!pEvent->bEnable, (RwUInt8)pEvent->uiSlotIdx);
	}

	NTL_RETURNVOID();
}



//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseAddonGui
//////////////////////////////////////////////////////////////////////////

CGuildWarehouseAddonGui::CGuildWarehouseAddonGui(const RwChar* pName)
:CGuildWarehouseGui(pName)
,m_pMoneyIconTexture(NULL)
,m_bZennySlotDestination(FALSE)
{

}


CGuildWarehouseAddonGui::~CGuildWarehouseAddonGui()
{

}

RwBool CGuildWarehouseAddonGui::Create()
{
	NTL_FUNCTION( "CGuildWarehouseAddonGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\GuildWarehouseAddon.srf", "gui\\GuildWarehouseAddon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	Init();

	// 제니 버튼
	m_pZennyButton = (gui::CButton*)GetComponent("btnZenny");
	m_slotZennyButton = m_pZennyButton->SigClicked().Connect(this, &CGuildWarehouseAddonGui::ClickedZennyButton);

	// 제니 스태틱
	rect.SetRectWH(56, 36, 82, 14);
	m_pZenny = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pZenny->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pZenny->Enable(false);	

	CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
	m_pZenny->SetText( pGuildWarehouse->GetZenny() );

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// 제니 슬롯 Destination 이미지
	m_srfZennySlotDestination.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "BasicBag.srf", "srfMoneyBtnWayFocus" ) );
	m_srfZennySlotDestination.SetPositionfromParent(31, 32);

	// sig
	m_slotPostPaint		= m_pZennyButton->SigPaint().Connect( this, &CGuildWarehouseAddonGui::OnPostPaint );

	LinkMsg(g_EventIconMoveClick);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGuildWarehouseAddonGui::Destroy()
{
	NTL_FUNCTION("CGuildWarehouseAddonGui::Destroy");

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	UnLinkMsg(g_EventIconMoveClick);

	CGuildWarehouseGui::Destroy();

	NTL_RETURNVOID();
}
VOID CGuildWarehouseAddonGui::SetZenny(RwUInt32 uiZenny)
{
	m_pZenny->SetText(uiZenny);
}

VOID CGuildWarehouseAddonGui::ClickedZennyButton(gui::CComponent* pComponent)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	if( GetIconMoveManager()->IsActive() )
	{
		CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

		// 제니 보관
		GetIconMoveManager()->IconMovePutDown(PLACE_SUB_GUILD_WAREHOUSE_ZENNY, pGuildWarehouse->GetNPCHandle(), INVALID_INDEX);
	}
	else
	{
		CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();

		if( pGuildWarehouse->GetZenny() > 0 )
		{
			// 창고에 제니가 있다면...인출
			CRectangle rect = m_pZennyButton->GetScreenRect();
			CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_GUILD_WAREHOUSE_ZENNY, 
				rect.right, rect.bottom, pGuildWarehouse->GetZenny());
		}
	}

	SelectEffect(FALSE);
}

VOID CGuildWarehouseAddonGui::OnPostPaint()
{
	if( m_bZennySlotDestination )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_srfZennySlotDestination.SetPositionbyParent(rtScreen.left, rtScreen.top);
		m_srfZennySlotDestination.Render();
	}
}

VOID CGuildWarehouseAddonGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGuildWarehouseAddonGui::HandleEvents");

	CGuildWarehouseGui::HandleEvents(msg);

	if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pEvent = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );
		if( pEvent->nSrcPlace == PLACE_SUB_GUILD_WAREHOUSE_ZENNY )
		{
			// 창고(은행)에서 돈을 뺀다
			GetIconMoveManager()->IconMovePickUp(INVALID_SERIAL_ID, PLACE_SUB_GUILD_WAREHOUSE_ZENNY, INVALID_INDEX, pEvent->uiValue, m_pMoneyIconTexture);
		}		
	}
	else if(msg.Id == g_EventIconMoveClick)
	{
		RwBool bPickUp = (RwBool)msg.pData;

		if( bPickUp )
		{
			if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_BAG_ZENNY )
			{
				m_bZennySlotDestination = TRUE;
			}
		}
		else
		{
			m_bZennySlotDestination = FALSE;
		}
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//	CGuildWarehouseBar
//////////////////////////////////////////////////////////////////////////

CGuildWarehouseBar::CGuildWarehouseBar(const RwChar* pName)
:CNtlPLGui(pName)
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
		m_pWareHouseGui[i] = NULL;
}


CGuildWarehouseBar::~CGuildWarehouseBar()
{

}

RwBool CGuildWarehouseBar::Create()
{
	NTL_FUNCTION( "CGuildWarehouseBar::Create" );

	if(!CNtlPLGui::Create("", "gui\\GuildWarehouseBar.srf", "gui\\GuildWarehouseBar.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 1번 창고 버튼
	m_pWarehouseBtn[0] = (gui::CButton*)GetComponent("btnAddon");
	m_pWarehouseBtn[0]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_1"));
	m_slotWarehouseBtn[0] = m_pWarehouseBtn[0]->SigClicked().Connect(this, &CGuildWarehouseBar::Clicked_1_Button);

	// 2번 창고 버튼
	m_pWarehouseBtn[1] = (gui::CButton*)GetComponent("btn1");
	m_pWarehouseBtn[1]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_2"));
	m_slotWarehouseBtn[1] = m_pWarehouseBtn[1]->SigClicked().Connect(this, &CGuildWarehouseBar::Clicked_2_Button);

	// 3번 창고 버튼
	m_pWarehouseBtn[2] = (gui::CButton*)GetComponent("btn2");
	m_pWarehouseBtn[2]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_3"));
	m_slotWarehouseBtn[2] = m_pWarehouseBtn[2]->SigClicked().Connect(this, &CGuildWarehouseBar::Clicked_3_Button);

	// 모든 창고 버튼
	m_p_All_Button = (gui::CButton*)GetComponent("btnAll");
	m_p_All_Button->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_ALL"));
	m_slot_All_Button = m_p_All_Button->SigClicked().Connect(this, &CGuildWarehouseBar::ClickedAllButton);

	// 창닫기 버튼
	m_pExitButton= (gui::CButton*)GetComponent("btnExit");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CGuildWarehouseBar::ClickedCloseButton);

	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
	{
		if( i == dGUILDWAREHOUSE_ADDON_INDEX )
			m_pWareHouseGui[i] = NTL_NEW CGuildWarehouseAddonGui("GuildWarehouseAddGui");
		else
			m_pWareHouseGui[i] = NTL_NEW CGuildWarehouseGui("GuildWarehouseGui");

		m_pWareHouseGui[i]->RegisterWarehouseIndex(i);

		if( !m_pWareHouseGui[i]->Create() )
			NTL_RETURN(FALSE);

		CNtlPLGuiManager::GetInstance()->AddGui(m_pWareHouseGui[i]);
		GetDialogManager()->RegistDialog(DIALOG_GUILD_WAREHOUSE_1 + i, m_pWareHouseGui[i], &CGuildWarehouseGui::SwitchDialog);

		Link( m_pWareHouseGui[i]->GetDialog() );
	}	

	// 위치 지정
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iXPos = rtScreen.left;
	RwInt32 iYPos = rtScreen.bottom + NTL_LINKED_DIALOG_GAP;

	m_pWareHouseGui[0]->SetPosition(iXPos, iYPos);

	iYPos = m_pWareHouseGui[0]->GetPosition().bottom + NTL_LINKED_DIALOG_GAP + 3;
	m_pWareHouseGui[1]->SetPosition(iXPos, iYPos);

	iXPos = m_pWareHouseGui[0]->GetPosition().right + NTL_LINKED_DIALOG_GAP;
	iYPos = rtScreen.top;
	m_pWareHouseGui[2]->SetPosition(iXPos, iYPos);


	// sig
	m_slotMove			   = m_pThis->SigMove().Connect( this, &CGuildWarehouseBar::OnMove );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CGuildWarehouseBar::OnCaptureMouseDown );

	LinkMsg(g_EventGuildWarehouseNotify);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGuildWarehouseBar::Destroy()
{
	NTL_FUNCTION("CGuildWarehouseBar::Destroy");

	UnLinkAll();

	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
	{
		if(m_pWareHouseGui[i])
		{
			GetDialogManager()->UnRegistDialog(DIALOG_GUILD_WAREHOUSE_1 + i);
			CNtlPLGuiManager::GetInstance()->RemoveGui(m_pWareHouseGui[i]);

			m_pWareHouseGui[i]->Destroy();
			NTL_DELETE(m_pWareHouseGui[i]);
		}		
	}

	UnLinkMsg(g_EventGuildWarehouseNotify);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGuildWarehouseBar::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CGuildWarehouseBar("GuildWarehouseBarGui");
	if( !m_pInstance->Create() )
	{
		m_pInstance->Destroy();
		return;
	}

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);
	GetDialogManager()->RegistDialog( DIALOG_GUILD_WAREHOUSEBAR, m_pInstance, &CGuildWarehouseBar::SwitchDialog );	
}

VOID CGuildWarehouseBar::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetDialogManager()->UnRegistDialog(DIALOG_GUILD_WAREHOUSEBAR);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);

	Logic_CancelNpcFacing();

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE ||
		GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_GUILD_WAREHOUSE_ZENNY )
		GetIconMoveManager()->IconMoveEnd();

	if( GetPopupManager()->IsVisiblePopupGui( PLACE_GUILD_WAREHOUSE ) )
		GetPopupManager()->AllPopupClose();
}

VOID CGuildWarehouseBar::Clicked_1_Button(gui::CComponent* pComponent)
{
	GetDialogManager()->SwitchDialog(DIALOG_GUILD_WAREHOUSE_1);
}

VOID CGuildWarehouseBar::Clicked_2_Button(gui::CComponent* pComponent)
{
	GetDialogManager()->SwitchDialog(DIALOG_GUILD_WAREHOUSE_2);
}

VOID CGuildWarehouseBar::Clicked_3_Button(gui::CComponent* pComponent)
{
	GetDialogManager()->SwitchDialog(DIALOG_GUILD_WAREHOUSE_3);
}

VOID CGuildWarehouseBar::ClickedAllButton(gui::CComponent* pComponent)
{
	// 창고가 하나라도 열려있다면 전부 닫는다

	RwBool bAlreadyOpen = FALSE;
	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
	{
		if( m_pWareHouseGui[i]->IsShow() )
		{
			bAlreadyOpen = TRUE;
			break;
		}
	}

	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
	{
		if(bAlreadyOpen)
		{
			if( i == 0 )
				GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSE_1 + i);
			else
				GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSE_1 + i, FALSE);
		}
		else
		{
			if( i == 0 )
				GetDialogManager()->OpenDialog(DIALOG_GUILD_WAREHOUSE_1 + i);
			else
				GetDialogManager()->OpenDialog(DIALOG_GUILD_WAREHOUSE_1 + i, INVALID_SERIAL_ID, FALSE);
		}
	}
}

VOID CGuildWarehouseBar::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

VOID CGuildWarehouseBar::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseEnd();
}

VOID CGuildWarehouseBar::OnCaptureMouseDown(const CKey& key)
{	
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_GUILD_WAREHOUSEBAR, key.m_fX, key.m_fY);
}

RwInt32 CGuildWarehouseBar::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i)
		{
			GetDialogManager()->OpenDialog(DIALOG_GUILD_WAREHOUSE_1 + i, INVALID_SERIAL_ID, FALSE);
		}

		Show(true);		
	}
	else
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE ||
			GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_GUILD_WAREHOUSE_ZENNY )
			GetIconMoveManager()->IconMoveEnd();

		for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i)
		{
			GetDialogManager()->CloseDialog(DIALOG_GUILD_WAREHOUSE_1 + i, FALSE);
		}

		CNtlSLEventGenerator::GuildWarehousePostEvent(GUILD_WAREHOUSE_EVENT_END);

		// 서버에 길드 창고 이용이 끝났음을 알린다
		GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseEnd();

		Show(false);
	}

	return 1;
}

VOID CGuildWarehouseBar::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGuildWarehouseBar::HandleEvents");

	if( msg.Id == g_EventGuildWarehouseNotify )
	{
		SNtlGuildWarehouseNotify* pEvent = reinterpret_cast<SNtlGuildWarehouseNotify*>( msg.pData );

		if( pEvent->byMessage == GUILD_WAREHOUSE_EVENT_SET_ITEM )
		{
			RwUInt8 byIndex = (RwUInt8)pEvent->uiParam;
			RwUInt8 byPos = (RwUInt8)pEvent->uiParam2;
			CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
			sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem(byIndex, byPos);
			NTL_ASSERT(pSlotData, "CGuildWarehouseBar::HandleEvents, Not exist guild warehouhse slot data of index : " << byIndex << " of slot " << byPos);

			m_pWareHouseGui[byIndex]->SetItem(pSlotData, byPos);
		}
		else if( pEvent->byMessage == GUILD_WAREHOUSE_EVENT_SET_ZENNY )
		{
			((CGuildWarehouseAddonGui*)m_pWareHouseGui[dGUILDWAREHOUSE_ADDON_INDEX])->SetZenny(pEvent->uiParam);
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		// 갑자기 캐릭터가 사라졋다
		SERIAL_HANDLE* pDeleteSerial = reinterpret_cast<SERIAL_HANDLE*>( msg.pData );
		SERIAL_HANDLE hNPC = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse()->GetNPCHandle();

		if( hNPC == *pDeleteSerial )
		{
			CNtlSLEventGenerator::GuildWarehousePostEvent(GUILD_WAREHOUSE_EVENT_END);

			// 서버에 길드 창고 이용이 끝났음을 알린다
			GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseEnd();
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_GUILD_WAREHOUSEBAR )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE )
			{
				SERIAL_HANDLE hNPC = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse()->GetNPCHandle();
				NTL_ASSERT(hNPC != INVALID_SERIAL_ID, "CGuildWarehouseBar::HandleEvents, Invalid handle of guild warehouse NPC : " << hNPC);

				CNtlSLEventGenerator::GuildWarehousePostEvent(GUILD_WAREHOUSE_EVENT_END);

				// 서버에 길드 창고 이용이 끝났음을 알린다
				GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseEnd();
			}
			else if( pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				if( GetDialogManager()->IsOpenDialog(DIALOG_MULTIDIALOG) == FALSE )
				{
					Logic_CancelNpcFacing();
				}			
			}
		}		
	}

	NTL_RETURNVOID();
}