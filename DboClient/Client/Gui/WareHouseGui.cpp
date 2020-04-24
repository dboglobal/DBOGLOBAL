#include "precomp_dboclient.h"
#include "WareHouseGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NPCTable.h"
#include "MerchantTable.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlWareHouse.h"
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
#include "MsgBoxGui.h"
#include "MsgBoxManager.h"


namespace
{
	#define dSLOT_ROW			4
	#define dSLOT_COLUMN		4

	#define dSLOT_START_X		19
	#define dSLOT_START_Y		31	

	#define dSLOT_VER_GAP		40
	#define dSLOT_HORI_GAP		40

	#define dHEIGHT_GAP			30		// 공유 창고와 일반 창고의 높이 차이
};


//////////////////////////////////////////////////////////////////////////
//	CWarehouseGui
//////////////////////////////////////////////////////////////////////////

CWarehouseGui::CWarehouseGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(INVALID_INDEX)
,m_iSelectedSlot(INVALID_INDEX)
,m_iClickEffectedSlot(INVALID_INDEX)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
{

}


CWarehouseGui::~CWarehouseGui()
{

}

RwBool CWarehouseGui::Create()
{
	NTL_FUNCTION( "CWarehouseGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\16Bag.srf", "gui\\Warehouse.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	Init();	
		
	Show(false);

	NTL_RETURN(TRUE);
}

VOID CWarehouseGui::Destroy()
{
	NTL_FUNCTION("CWarehouseGui::Destroy");

	CheckInfoWindow();	

	for(int i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i)
	{
		m_Slot[i].Destroy();
	}

	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventPickedUpHide);
	UnLinkMsg(g_EventEnableItemIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CWarehouseGui::Init()
{
	CRectangle rect;

	// 다이얼로그 이름 스태틱
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->Enable(false);

	if( m_byWarehouseIndex == 0 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_1"));
	else if( m_byWarehouseIndex == 1 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_2"));
	else if( m_byWarehouseIndex == 2 )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_3"));
	else if( m_byWarehouseIndex == NTL_COMMON_WAREHOUSE )
		m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_COMMON"));
	

	// 창닫기 버튼
	m_pExitButton= (gui::CButton*)GetComponent("btnExit");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CWarehouseGui::ClickedCloseButton);

	// 아이템 슬롯
	for(RwInt32 i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i )
	{
		m_Slot[i].Create(m_pThis, (eDialogType)(DIALOG_WAREHOUSE_1 + m_byWarehouseIndex), REGULAR_SLOT_ITEM_SOB, SDS_COUNT | SDS_LOCK);

		if( m_byWarehouseIndex == NTL_COMMON_WAREHOUSE )
			m_Slot[i].SetPosition_fromParent(dSLOT_START_X + dSLOT_VER_GAP*(i%dSLOT_ROW), dSLOT_START_Y + dHEIGHT_GAP + dSLOT_HORI_GAP*(i/dSLOT_COLUMN));
		else
			m_Slot[i].SetPosition_fromParent(dSLOT_START_X + dSLOT_VER_GAP*(i%dSLOT_ROW), dSLOT_START_Y + dSLOT_HORI_GAP*(i/dSLOT_COLUMN));
	}

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect") );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventCalcPopupResult);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventPickedUpHide);
	LinkMsg(g_EventEnableItemIcon);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CWarehouseGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CWarehouseGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CWarehouseGui::OnMove);
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CWarehouseGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CWarehouseGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CWarehouseGui::OnPaint );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CWarehouseGui::OnCaptureMouseDown );
}

VOID CWarehouseGui::RegisterWarehouseIndex(RwUInt8 byIndex)
{
	m_byWarehouseIndex = byIndex;
}

VOID CWarehouseGui::UpdateItems()
{
	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();	
	SERIAL_HANDLE hWarehouseSlotHandle = pWarehouse->GetSlotSerailID(m_byWarehouseIndex);

	if( hWarehouseSlotHandle == INVALID_SERIAL_ID )
		return;

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hWarehouseSlotHandle));
	CNtlSobItem* pChildItem;
	CNtlSobItemAttr* pChildItemAttr;
	CRectangle rectParent = GetPosition();
	for(RwUInt8 i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i )
	{
		pChildItem = pItem->GetChildItem(i);

		if(pChildItem)
		{
			pChildItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pChildItem->GetSobAttr());
			m_Slot[i].SetParentPosition(rectParent.left, rectParent.top);

			if(m_Slot[i].GetSerial() != pChildItem->GetSerialID())
			{
				m_Slot[i].Clear();

				if( pChildItemAttr->IsNeedToIdentify() )
					// 미확인 아이템
					m_Slot[i].SetSerialType(REGULAR_SLOT_ITEM_NOT_IDENTIFICATION);
				else
					m_Slot[i].SetSerialType(REGULAR_SLOT_ITEM_SOB);

				m_Slot[i].SetIcon(pChildItem->GetSerialID(), pChildItemAttr->GetStackNum());
			}				
			else
				m_Slot[i].SetCount(pChildItemAttr->GetStackNum());
		}
		else
			m_Slot[i].Clear();
	}
}

VOID CWarehouseGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_1 + m_byWarehouseIndex);
}

RwInt8 CWarehouseGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i)
	{
		if( m_Slot[i].PtInRect(iX, iY) )
		{
			return i;
		}
	}	

	return INVALID_BYTE;
}

VOID CWarehouseGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iX = dSLOT_START_X + dSLOT_VER_GAP*(iSlotIdx%dSLOT_COLUMN);
		RwInt32 iY;
		CRectangle rect = m_pThis->GetScreenRect();
		
		if( m_byWarehouseIndex == NTL_COMMON_WAREHOUSE )
			iY = dSLOT_START_Y + dHEIGHT_GAP + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);
		else
			iY = dSLOT_START_Y + dSLOT_HORI_GAP*(iSlotIdx/dSLOT_ROW);
		

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
		m_bFocus = FALSE;
}

VOID CWarehouseGui::SelectEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iX = dSLOT_START_X + dSLOT_VER_GAP*(iSlotIdx%dSLOT_COLUMN);
		RwInt32  iY;
		CRectangle rect = m_pThis->GetScreenRect();

		if( m_byWarehouseIndex == NTL_COMMON_WAREHOUSE )
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

VOID CWarehouseGui::ShowDisableSlot(RwBool bShow, RwUInt8 bySlot)
{
	m_Slot[bySlot].ShowLock(bShow);
}

VOID CWarehouseGui::CheckInfoWindow()
{
	if(	GetInfoWndManager()->GetRequestGui() == DIALOG_WAREHOUSE_1 + m_byWarehouseIndex )
	{
		if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM ||
			GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_ITEM )
		{
			m_byInfoWindowIndex = INVALID_BYTE;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}			
}

VOID CWarehouseGui::OnMouseDown(const CKey& key)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	RwInt8 iPtinSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( iPtinSlot != INVALID_INDEX )
	{
		m_iMouseDownSlot = iPtinSlot;

		// 클릭 이벤트 시작
		m_iClickEffectedSlot = iPtinSlot;	
		m_Slot[m_iClickEffectedSlot].ClickEffect(TRUE);
	}	
}

VOID CWarehouseGui::OnMouseUp(const CKey& key)
{
	// 클릭 이벤트 종료	
	if( m_iClickEffectedSlot != INVALID_INDEX )
	{		
		m_Slot[m_iClickEffectedSlot].ClickEffect(FALSE);
		m_iClickEffectedSlot = INVALID_INDEX;
	}

	if( m_iMouseDownSlot == INVALID_INDEX )
	{
		m_iMouseDownSlot = INVALID_INDEX;
		return;
	}

	if( m_Slot[m_iMouseDownSlot].PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( m_Slot[m_iMouseDownSlot].IsShowLock() == FALSE )
		{
			if( key.m_nID == UD_LEFT_BUTTON )
			{
				if( GetIconMoveManager()->IsActive() )
				{
					// 창고로 물건을 옮긴다			
					CNtlWarehouse* pWarehouses = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
					GetIconMoveManager()->IconMovePutDown(PLACE_WAREHOUSE, pWarehouses->GetSlotSerailID(m_byWarehouseIndex), m_iMouseDownSlot);
				}
				else
				{					
					if(m_Slot[m_iMouseDownSlot].GetSerial() != INVALID_SERIAL_ID)
					{
						if( key.m_dwVKey & UD_MK_CONTROL )
						{
							// 창고에서 아이템을 나누기 위해 계산기를 연다
							CRectangle rtScreen = m_pThis->GetScreenRect();
							CDboEventGenerator::CalcPopupShow( TRUE, m_Slot[m_iMouseDownSlot].GetSerial(), PLACE_WAREHOUSE, rtScreen.left, rtScreen.top, m_Slot[m_iMouseDownSlot].GetCount() );
						}
						else
						{
							// 창고에서 물건을 집는다
							GetIconMoveManager()->IconMovePickUp(m_Slot[m_iMouseDownSlot].GetSerial(), PLACE_WAREHOUSE,
								m_iMouseDownSlot, m_Slot[m_iMouseDownSlot].GetCount(), m_Slot[m_iMouseDownSlot].GetTexture(), 0, 0);	

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
						// 바로 가방으로 옮긴다
						Logic_ItemMoveProc(m_Slot[m_iMouseDownSlot].GetSerial(), PLACE_WAREHOUSE, (RwUInt8)m_iMouseDownSlot,
							PLACE_BAG, hBagHandle, (RwUInt8)uiSlot_of_Bag, m_Slot[m_iMouseDownSlot].GetCount());

						SelectEffect(FALSE);
					}
					/*
					if( m_Slot[m_iMouseDownSlot].GetSerial() != INVALID_SERIAL_ID )
					{
					int iX, iY;
					CRectangle& rect = m_pThis->GetScreenRect();
					m_Slot[m_iMouseDownSlot].GetPosition(iX, iY);

					CDboEventGenerator::IconPopupShow(true, m_Slot[m_iMouseDownSlot].GetSerial(), PLACE_WAREHOUSE,
					m_iMouseDownSlot, iX + rect.left, iY + rect.top);						

					SelectEffect(false);
					}
					*/
				}
			}
		}
	}	
	
	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CWarehouseGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwUInt8 i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i )
	{
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
	}

	m_SelectEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_FocusEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CWarehouseGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
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
			if(m_Slot[iPtinSlot].GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION )
			{
				if( m_byInfoWindowIndex != iPtinSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
						rtScreen.left + m_Slot[iPtinSlot].GetX_fromParent(),
						rtScreen.top + m_Slot[iPtinSlot].GetY_fromParent(),
						m_Slot[iPtinSlot].GetItemTable(), DIALOG_WAREHOUSE_1 + m_byWarehouseIndex );
					m_byInfoWindowIndex = iPtinSlot;						
				}
			}
			else
			{
				if( m_byInfoWindowIndex != iPtinSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
						rtScreen.left + m_Slot[iPtinSlot].GetX_fromParent(),
						rtScreen.top + m_Slot[iPtinSlot].GetY_fromParent(),
						m_Slot[iPtinSlot].GetSobItem(), DIALOG_WAREHOUSE_1 + m_byWarehouseIndex );
					m_byInfoWindowIndex = iPtinSlot;
				}
			}

			return;
		}

		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
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

VOID CWarehouseGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CWarehouseGui::OnPaint()
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_BANK_ITEM_SLOT ; ++i )
	{
		m_Slot[i].Paint();
	}

	if( m_iSelectedSlot != INVALID_INDEX )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CWarehouseGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_WAREHOUSE_1 + m_byWarehouseIndex, key.m_fX, key.m_fY);
}

RwInt32 CWarehouseGui::SwitchDialog(bool bOpen)
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

VOID CWarehouseGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CWarehouseGui::HandleEvents");
	
	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) == FALSE )
			NTL_RETURNVOID();


		// 창고 정보를 업데이트 한다.
		UpdateItems();
	}
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_WAREHOUSE )
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pData->uiSerial ) );

			// 3개의 창고중에 해당 아이템을 가지고 있는 것을 찾는다
			if( m_Slot[pItem->GetItemSlotIdx()].GetSerial() != pItem->GetSerialID() )
				return;
		
			GetIconMoveManager()->IconMovePickUp( pItem->GetSerialID(), PLACE_WAREHOUSE, pItem->GetItemSlotIdx(),
				pData->uiValue, m_Slot[pItem->GetItemSlotIdx()].GetTexture());
		}
	}
	else if(msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

		if( pPacket->nSrcPlace != PLACE_WAREHOUSE )
			return;

		// 3개의 창고중에 해당 아이템을 가지고 있는 것을 찾는다
		if( m_Slot[pPacket->nSrcSlotIdx].GetSerial() != pPacket->uiSerial )
			return;

		if( pPacket->nWorkId == PMW_PICKUP )
		{
			GetIconMoveManager()->IconMovePickUp( pPacket->uiSerial, PLACE_WAREHOUSE, pPacket->nSrcSlotIdx,
				m_Slot[pPacket->nSrcSlotIdx].GetCount(), m_Slot[pPacket->nSrcSlotIdx].GetTexture() );
		}
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_WAREHOUSE )
			return;

		SelectEffect(FALSE);
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace != PLACE_WAREHOUSE )
			return;

		if( m_byWarehouseIndex != pData->nPlaceIdx )
			return;

		ShowDisableSlot(!pData->bEnable, (RwUInt8)pData->uiSlotIdx);
	}

	NTL_RETURNVOID();
}



//////////////////////////////////////////////////////////////////////////
//	CWarehouseCommonGui
//////////////////////////////////////////////////////////////////////////

CWarehouseCommonGui::CWarehouseCommonGui(const RwChar* pName)
:CWarehouseGui(pName)
,m_uiZenny(0)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_pMoneyIconTexture(NULL)
,m_bZennySlotDestination(FALSE)
{

}


CWarehouseCommonGui::~CWarehouseCommonGui()
{

}

RwBool CWarehouseCommonGui::Create()
{
	NTL_FUNCTION( "CWarehouseCommonGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\BasicBag.srf", "gui\\WarehouseCommon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	Init();

	// 제니 버튼
	m_pZennyButton = (gui::CButton*)GetComponent("btnZenny");
	m_slotZennyButton = m_pZennyButton->SigClicked().Connect(this, &CWarehouseCommonGui::ClickedZennyButton);

	// 제니 스태틱
	rect.SetRectWH(56, 36, 82, 14);
	m_pZenny = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pZenny->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pZenny->SetText("0");
	m_pZenny->Enable(false);	

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// 제니 슬롯 Destination 이미지
	m_srfZennySlotDestination.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "BasicBag.srf", "srfMoneyBtnWayFocus" ) );
	m_srfZennySlotDestination.SetPositionfromParent(31, 32);

	// sig
	m_slotPostPaint		= m_pZennyButton->SigPaint().Connect( this, &CWarehouseCommonGui::OnPostPaint );

	LinkMsg(g_EventIconMoveClick);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CWarehouseCommonGui::Destroy()
{
	NTL_FUNCTION("CWarehouseCommonGui::Destroy");

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	UnLinkMsg(g_EventIconMoveClick);

	CWarehouseGui::Destroy();

	NTL_RETURNVOID();
}

VOID CWarehouseCommonGui::SetNPCHandle(SERIAL_HANDLE hHandle)
{
	m_hNPCSerial = hHandle;
}

SERIAL_HANDLE CWarehouseCommonGui::GetNPCHandle()
{
	return m_hNPCSerial;
}

VOID CWarehouseCommonGui::ClickedZennyButton(gui::CComponent* pComponent)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	if( GetIconMoveManager()->IsActive() )
	{
		// 제니 보관
		GetIconMoveManager()->IconMovePutDown(PLACE_SUB_WAREHOUSE_ZENNY, m_hNPCSerial, INVALID_INDEX);
	}
	else
	{
		if( m_uiZenny > 0 )
		{
			// 창고에 제니가 있다면...인출
			CRectangle rect = m_pZennyButton->GetScreenRect();
			CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_WAREHOUSE_ZENNY, 
				rect.right, rect.bottom, m_uiZenny);
		}
	}
	
	SelectEffect(FALSE);
}

VOID CWarehouseCommonGui::OnPostPaint()
{
	if( m_bZennySlotDestination )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_srfZennySlotDestination.SetPositionbyParent(rtScreen.left, rtScreen.top);
		m_srfZennySlotDestination.Render();
	}
}

VOID CWarehouseCommonGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CWarehouseCommonGui::HandleEvents");

	CWarehouseGui::HandleEvents(msg);

	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_WAREHOUSE, msg.pData) == FALSE )
			NTL_RETURNVOID();


		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		switch(pEvent->uiParam1)
		{
		case NESWUT_ADD_ZENNY:	// 창고에 제니가 늘었다
			{
				m_uiZenny = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse()->GetZenny();
				m_pZenny->SetText( m_uiZenny );

				break;
			}
		case NESWUT_SUB_ZENNY:	// 창고에 제니가 줄었다
			{
				m_uiZenny = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse()->GetZenny();
				m_pZenny->SetText( m_uiZenny );
				break;
			}
		}
	}	
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );
		if( pData->nSrcPlace == PLACE_SUB_WAREHOUSE_ZENNY )
		{
			GetIconMoveManager()->IconMovePickUp(INVALID_SERIAL_ID, PLACE_SUB_WAREHOUSE_ZENNY, INVALID_INDEX, pData->uiValue, m_pMoneyIconTexture);
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
//	CWarehouseBarGui
//////////////////////////////////////////////////////////////////////////

CWarehouseBarGui::CWarehouseBarGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_hNPCSerial(INVALID_SERIAL_ID)
{

}


CWarehouseBarGui::~CWarehouseBarGui()
{

}

RwBool CWarehouseBarGui::Create()
{
	NTL_FUNCTION( "CWarehouseBarGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\WarehouseBar.srf", "gui\\WarehouseBar.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 1번 창고 버튼
	m_pWarehouseBtn[0] = (gui::CButton*)GetComponent("btn1");
	m_pWarehouseBtn[0]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_1"));
	m_pWarehouseBtn[0]->ClickEnable(false);
	m_slotWarehouseBtn[0] = m_pWarehouseBtn[0]->SigClicked().Connect(this, &CWarehouseBarGui::Clicked_1_Button);

	// 2번 창고 버튼
	m_pWarehouseBtn[1] = (gui::CButton*)GetComponent("btn2");
	m_pWarehouseBtn[1]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_2"));
	m_pWarehouseBtn[1]->ClickEnable(false);
	m_slotWarehouseBtn[1] = m_pWarehouseBtn[1]->SigClicked().Connect(this, &CWarehouseBarGui::Clicked_2_Button);

	// 3번 창고 버튼
	m_pWarehouseBtn[2] = (gui::CButton*)GetComponent("btn3");
	m_pWarehouseBtn[2]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_3"));
	m_pWarehouseBtn[2]->ClickEnable(false);
	m_slotWarehouseBtn[2] = m_pWarehouseBtn[2]->SigClicked().Connect(this, &CWarehouseBarGui::Clicked_3_Button);

	// 공유 창고 버튼
	m_pWarehouseBtn[3] = (gui::CButton*)GetComponent("btnCommon");
	m_pWarehouseBtn[3]->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_NAME_COMMON"));
	m_pWarehouseBtn[3]->ClickEnable(false);
	m_slotWarehouseBtn[3] = m_pWarehouseBtn[3]->SigClicked().Connect(this, &CWarehouseBarGui::Clicked_Common_Button);

	// 모든 창고 버튼
	m_p_All_Button = (gui::CButton*)GetComponent("btnAll");
	m_p_All_Button->SetToolTip(GetDisplayStringManager()->GetString("DST_WAREHOUSE_ALL"));
	m_slot_All_Button = m_p_All_Button->SigClicked().Connect(this, &CWarehouseBarGui::ClickedAllButton);

	// 창닫기 버튼
	m_pExitButton= (gui::CButton*)GetComponent("btnExit");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CWarehouseBarGui::ClickedCloseButton);

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CWarehouseBarGui::OnMove );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CWarehouseBarGui::OnCaptureMouseDown );

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CWarehouseBarGui::Destroy()
{
	NTL_FUNCTION("CWarehouseBarGui::Destroy");

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CWarehouseBarGui::RegisterWarehouse(RwUInt8 byIndex, CWarehouseGui* pWarehouseGui)
{
	m_pWareHouseGui[byIndex] = pWarehouseGui;
}

SERIAL_HANDLE CWarehouseBarGui::GetNPCSerial()
{
	return m_hNPCSerial;
}

VOID CWarehouseBarGui::Clicked_1_Button(gui::CComponent* pComponent)
{
	// 기본 창고
	GetDialogManager()->SwitchDialog(DIALOG_WAREHOUSE_1);
}

VOID CWarehouseBarGui::Clicked_2_Button(gui::CComponent* pComponent)
{
	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	if( pWarehouse->GetSlotSerailID(1) != INVALID_SERIAL_ID )
		GetDialogManager()->SwitchDialog(DIALOG_WAREHOUSE_2);
}

VOID CWarehouseBarGui::Clicked_3_Button(gui::CComponent* pComponent)
{
	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	if( pWarehouse->GetSlotSerailID(2) != INVALID_SERIAL_ID )
		GetDialogManager()->SwitchDialog(DIALOG_WAREHOUSE_3);
}

VOID CWarehouseBarGui::Clicked_Common_Button(gui::CComponent* pComponent)
{
	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	if( pWarehouse->IsHaveCommonWarehouse() )
		GetDialogManager()->SwitchDialog(DIALOG_WAREHOUSE_COMMON);
}

VOID CWarehouseBarGui::ClickedAllButton(gui::CComponent* pComponent)
{
	// 창고가 하나라도 열려있다면 전부 닫는다
	RwBool bOpen = FALSE;

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
	for(int i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
	{
		if( pWarehouse->GetSlotSerailID(i) != INVALID_SERIAL_ID )
		{
			if( GetDialogManager()->IsOpenDialog(DIALOG_WAREHOUSE_1 + i) )
			{
				bOpen = TRUE;
				break;
			}
		}
	}

	for(int i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
	{
		if( pWarehouse->GetSlotSerailID(i) != INVALID_SERIAL_ID )
		{
			if( bOpen )
			{
				if( i == 0 )
					GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_1 + i);
				else
					GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_1 + i, FALSE);
			}
			else
			{
				if( i == 0 )
					GetDialogManager()->OpenDialog(DIALOG_WAREHOUSE_1 + i);
				else
					GetDialogManager()->OpenDialog(DIALOG_WAREHOUSE_1 + i, INVALID_SERIAL_ID, FALSE);
			}
		}
	}
}

VOID CWarehouseBarGui::ClickedCloseButton(gui::CComponent* pComponent)
{
	// 서버에 창고 이용이 끝났음을 알린다
	GetDboGlobal()->GetGamePacketGenerator()->SendBankEnd();
}

VOID CWarehouseBarGui::CloseWarehouse()
{
	GetDialogManager()->SwitchBag(FALSE);

	m_hNPCSerial = INVALID_SERIAL_ID;
	((CWarehouseCommonGui*)m_pWareHouseGui[NTL_COMMON_WAREHOUSE])->SetNPCHandle(INVALID_SERIAL_ID);
}

VOID CWarehouseBarGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

VOID CWarehouseBarGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_WAREHOUSEBAR, key.m_fX, key.m_fY);
}

RwInt32 CWarehouseBarGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);		
	}
	else
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_WAREHOUSE ||
			GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_WAREHOUSE_ZENNY )
			GetIconMoveManager()->IconMoveEnd();

		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
		if( pWorldConcept && pWorldConcept->GetConceptType() == WORLD_PLAY_NPC_COMMU )
		{
			if( pWorldConcept->GetState() == WORLD_NPC_WAREHOUSE )
				ClickedCloseButton(NULL);
		}
		else
		{
			Show(false);
			Logic_CancelNpcFacing();

			// 창고 아이템을 버리려고 띄운 메세지 박스를 닫는다
			CMsgBoxManager::LIST_MSGBOX listMSGBOX;
			GetMsgBoxManager()->GetMsgBox("DST_ITEM_CONFIRM_DROP", &listMSGBOX);

			for each( CMsgBoxGui* pMsgBox in listMSGBOX )
			{
				SERIAL_HANDLE hItem = pMsgBox->GetMsgBoxData()->sItemInfo.hSerial;
				RwUInt8 byWarehouseIndex, bySlot_of_warehouse;

				if( Logic_FindWarehouseItem(hItem, byWarehouseIndex, bySlot_of_warehouse) )
					GetMsgBoxManager()->DeleteMsgBox(pMsgBox);
			}

			for(RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
			{
				if( !m_pWareHouseGui[i] )
					continue;
		
				for( RwUInt8 k = 0 ; k < NTL_MAX_BANK_ITEM_SLOT ; ++k )
					m_pWareHouseGui[i]->ShowDisableSlot(FALSE, k);
			}
		}
	}

	return 1;
}

VOID CWarehouseBarGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CWarehouseBarGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pData->eDialog != DIALOG_WAREHOUSEBAR )
			NTL_RETURNVOID();

		if( GetDialogManager()->IsOpenDialog(DIALOG_WAREHOUSEBAR) )
			NTL_RETURNVOID();

		if( m_hNPCSerial == pData->hSerialId )
			NTL_RETURNVOID();

		m_hNPCSerial = pData->hSerialId;

		// 창고 열기를 서버에 요청한다
		RwBool	bPacketLock		= FALSE;
		bool	bSendSuccess	= GetDboGlobal()->GetGamePacketGenerator()->SendBankStart(m_hNPCSerial, &bPacketLock);

		if( bPacketLock && !bSendSuccess )
		{
			m_hNPCSerial = INVALID_SERIAL_ID;

			Logic_CancelNpcFacing();
		}
	}	
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_WAREHOUSE, msg.pData) == FALSE )
			NTL_RETURNVOID();


		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		switch(pEvent->uiParam1)
		{
		case NESWUT_WAREHOUSE_START:
			{
				// 창고 컨트롤 바 열기
				GetDialogManager()->OpenDialog(DIALOG_WAREHOUSEBAR, pEvent->hSerialId, FALSE);

				// 창고 열기					
				CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
				for(RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
				{
					if( pWarehouse->GetSlotSerailID(i) != INVALID_SERIAL_ID )
						GetDialogManager()->OpenDialog(DIALOG_WAREHOUSE_1 + i, INVALID_SERIAL_ID, FALSE);
				}
				NTL_ASSERT(pWarehouse->GetSlotSerailID(0) != INVALID_SERIAL_ID, "CWarehouseBarGui::HandleEvents, Not exist bagic warehouse");

				// 가방 열기
				GetDialogManager()->SwitchBag( TRUE );

				// NPC 번호
				m_hNPCSerial = pEvent->hSerialId;
				((CWarehouseCommonGui*)m_pWareHouseGui[NTL_COMMON_WAREHOUSE])->SetNPCHandle(m_hNPCSerial);

				break;
			}
		case NESWUT_BUY_SLOT:
			{
				RwUInt8 byCount = 0;
				CRectangle rect = GetPosition();
				CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();
				for(RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i)
				{
					if( pWarehouse->GetSlotSerailID(i) != INVALID_SERIAL_ID )
					{
						if( byCount == 0 )
						{
							m_pWareHouseGui[i]->SetPosition(rect.left, rect.bottom + NTL_LINKED_DIALOG_GAP);
							rect = m_pWareHouseGui[i]->GetPosition();								
						}
						else if( byCount == 1 )
						{
							m_pWareHouseGui[i]->SetPosition(rect.left, rect.bottom + NTL_LINKED_DIALOG_GAP + 3);
						}
						else if( byCount == 2 )
						{
							m_pWareHouseGui[i]->SetPosition(rect.right + NTL_LINKED_DIALOG_GAP, GetPosition().top);
							rect = m_pWareHouseGui[i]->GetPosition();
						}
						else if( byCount == 3 )
						{
							m_pWareHouseGui[i]->SetPosition(rect.left, rect.bottom + NTL_LINKED_DIALOG_GAP + 3);
						}

						m_pWarehouseBtn[i]->ClickEnable(TRUE);
						++byCount;
					}							
				}

				break;
			}
		case NESWUT_CLOSE:
			{
				GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

				GetDialogManager()->CloseDialog(DIALOG_WAREHOUSEBAR, FALSE);
				GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_1, FALSE);
				GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_2, FALSE);
				GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_3, FALSE);
				GetDialogManager()->CloseDialog(DIALOG_WAREHOUSE_COMMON, FALSE);
				
				CloseWarehouse();
				break;
			}
		}
	}	
	else if( msg.Id == g_EventCharObjDelete )
	{
		// 갑자기 캐릭터가 사라졋다
		SERIAL_HANDLE* pDeleteSerial = reinterpret_cast<SERIAL_HANDLE*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// 서버에 창고 이용이 끝났음을 알린다
			GetDboGlobal()->GetGamePacketGenerator()->SendBankEnd();
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_WAREHOUSEBAR )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE )
			{
				// 서버에 창고 이용이 끝났음을 알린다
				GetDboGlobal()->GetGamePacketGenerator()->SendBankEnd();
			}
			else if( pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				Logic_CancelNpcFacing();

				CloseWarehouse();
			}
		}		
	}
	
	NTL_RETURNVOID();
}