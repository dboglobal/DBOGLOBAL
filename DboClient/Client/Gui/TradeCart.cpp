#include "precomp_dboclient.h"
#include "TradeCart.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "NPCTable.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"
#include "NPCShop.h"

namespace
{
	#define dGUI_BUY_SELL_SLOT_GAP				97
	#define dGUI_SLOT_HORI_GAP					42
	#define dGUI_SLOT_VERT_GAP					57
	#define dGUI_BUTTON_HORI_GAP				19
}


CTradeCart::CTradeCart(const RwChar* pName)
:CNtlPLGui(pName)
,m_pBuyButton(NULL)
,m_pSellButton(NULL)
,m_pTotalBuyMoney(NULL)
,m_pTotalSellMoney(NULL)
,m_eTextureType(TRADECART_TEXTURE_NORMAL)
,m_uiSubjectSerial(INVALID_SERIAL_ID)
,m_eDialogType(DIALOG_SHOPING_CART)
,m_uiTotalSellPrice(0)
,m_byOthersEmptySlot(MAX_SLOT)
,m_bFocus(FALSE)
,m_bLeftLock(FALSE)
,m_bRightLock(FALSE)
,m_bZennySlotDestination(FALSE)
,m_uiUserTradePacketCount(0)
,m_byInfoWindowIndex(INVALID_BYTE)
,m_pMoneyIconTexture(NULL)
{
	for(RwInt32 i = 0 ; i < SLOTKIND_NUM ; ++i )
	{
		for(RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
		{
			m_pUpButton[i][j] = NULL;
			m_pDownButton[i][j] = NULL;
		}
	}
}

CTradeCart::~CTradeCart()
{
	NTL_FUNCTION("CTradeCart::Destroy");

	Destroy();

	NTL_RETURNVOID();
}

RwBool CTradeCart::Create()
{
	NTL_FUNCTION( "CTradeCart::Create" );

	char acSurfaceName[64];


	// 천하제일 무도회의 상점카트인지 아닌지에 따라서 텍스처가 변한다
	switch(m_eTextureType)
	{
	case TRADECART_TEXTURE_NORMAL:
		{
			if(!CNtlPLGui::Create("", "gui\\TradeCart.srf", "gui\\TradeCart.frm"))
				NTL_RETURN(FALSE);

			sprintf_s(acSurfaceName, 64, "TradeCart.srf");

			break;
		}
	case TRADECART_TEXTURE_TENKAICHI:
		{
			if(!CNtlPLGui::Create("gui\\TenkaichiShop.rsr", "gui\\TenkaichiTradeCart.srf", "gui\\TenkaichiTradeCart.frm"))
				NTL_RETURN(FALSE);

			sprintf_s(acSurfaceName, 64, "TenkaichiTradeCart.srf");

			break;
		}
	default:
		{
			DBO_FAIL("Wrong texture type");
			break;
		}
	}

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	// 다이얼로그 이름 스태틱
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 130, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->Enable(false);	

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CTradeCart::ClickedCloseButton);

	// 제니 버튼(유저 트레이드용 오른쪽 제니 버튼)
	m_pZennyButton = (gui::CButton*)GetComponent( "BtnZenny" );	
	m_slotBtnZenny = m_pZennyButton->SigClicked().Connect(this, &CTradeCart::ClickedZennyButton);

	// 구입 버튼
	m_pBuyButton = (gui::CButton*)GetComponent( "BuyButton" );	
	m_slotClickedBuy = m_pBuyButton->SigClicked().Connect(this, &CTradeCart::ClickedBuyButton);	

	// 판매 버튼
	m_pSellButton = (gui::CButton*)GetComponent( "SellButton" );	
	m_slotClickedSell = m_pSellButton->SigClicked().Connect(this, &CTradeCart::ClickedSellButton);

	// 아이템 갯수 더하기/빼기 버튼
	RwInt32 iButtonX = 16;
	for( RwInt32 i = 0 ; i < SLOTKIND_NUM ; ++i )
	{		
		RwInt32 iButtonY = 93;

		for( RwInt32 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( (j%2) == 0 )
				iButtonX = 16;
			else
				iButtonX = 16 + dGUI_SLOT_HORI_GAP;

			if( i == SELL_SLOT )
				iButtonX += dGUI_BUY_SELL_SLOT_GAP;


			// 아이템 갯수 더하기 버튼
			rect.SetRectWH(iButtonX, iButtonY, 18, 15);
			m_pUpButton[i][j] = NTL_NEW gui::CButton(rect, "",
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonUp" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDown" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonDis" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfUpButtonFocus" ),
								NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
								GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
			m_slotUpButton[i][j] = m_pUpButton[i][j]->SigClicked().Connect(this, &CTradeCart::ClickUpButton);


			// 아이템 갯수 빼기 버튼
			rect.SetRectWH(iButtonX + dGUI_BUTTON_HORI_GAP, iButtonY, 18, 15);
			m_pDownButton[i][j] = NTL_NEW gui::CButton(rect, "",
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonUp" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDown" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonDis" ),
								GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfDownButtonFocus" ),
								NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
								GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );

			m_slotDownButton[i][j] = m_pDownButton[i][j]->SigClicked().Connect(this, &CTradeCart::ClickDownButton);


			if( (j%2) != 0 )
			{
				iButtonY += dGUI_SLOT_VERT_GAP;
			}
		}

		iButtonX = 113;
	}	

	// Slots
	RwInt32 iSlotX;
	RwInt32 iSlotY = 57;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{	
		if( (i%2) == 0 )
			iSlotX = 18;
		else
			iSlotX = 18 + dGUI_SLOT_HORI_GAP;


		m_BuySlotInfo[i].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
		m_BuySlotInfo[i].slot.SetPosition_fromParent(iSlotX, iSlotY);

		m_SellSlotInfo[i].slot.Create(m_pThis, DIALOG_SHOPING_CART, REGULAR_SLOT_ITEM_SOB, SDS_COUNT | SFF_USE_ITEM_OPTION_TABLE);
		m_SellSlotInfo[i].slot.SetPosition_fromParent(iSlotX + dGUI_BUY_SELL_SLOT_GAP, iSlotY);


		if( (i%2) != 0 )
			iSlotY += dGUI_SLOT_VERT_GAP;
	}

	// 총 구입 금액
	rect.SetRectWH( 16, 406, 61, 16);
	m_pTotalBuyMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pTotalBuyMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalBuyMoney->SetText( "0");
	m_pTotalBuyMoney->Enable(false);

	// 총 판매 금액
	rect.SetRectWH( 114, 406, 61, 16);
	m_pTotalSellMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pTotalSellMoney->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTotalSellMoney->SetText("1,234,567,890");
	m_pTotalSellMoney->Enable(false);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 사기 서페이스
	m_BuyBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBuybar" ) );
	m_BuyBar.SetPositionfromParent(16, 28);

	// 팔기 서페이스
	m_SellBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfSellbar" ) );
	m_SellBar.SetPositionfromParent(133, 28);

	// 받기 서페이스
	m_GiveBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfGivebar" ) );
	m_GiveBar.SetPositionfromParent(16, 28);

	// 주기 서페이스
	m_TakeBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfTakebar" ) );
	m_TakeBar.SetPositionfromParent(133, 28);

	// 왼쪽 잠김 서페이스
	m_LeftLockSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_LeftLockSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceUp" ) );
	m_LeftLockSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceCenter" ) );
	m_LeftLockSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceDown" ) );
	m_LeftLockSurface.SetSize(m_LeftLockSurface.GetWidth(), 415);
	m_LeftLockSurface.SetPositionfromParent(10, 46);
	
	// 오른쪽 잠김 서페이스
	m_RightLockSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_RightLockSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceUp" ) );
	m_RightLockSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceCenter" ) );
	m_RightLockSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "LockSurfaceDown" ) );
	m_RightLockSurface.SetSize(m_RightLockSurface.GetWidth(), 415);
	m_RightLockSurface.SetPositionfromParent(108, 46);		

	// 제니 슬롯 Destination 이미지
	m_srfZennySlotDestination.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TradeCart.srf", "srfBtnZennyFoc" ) );
	m_srfZennySlotDestination.SetPositionfromParent(111, 404);

	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CTradeCart::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CTradeCart::OnMouseUp);
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTradeCart::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTradeCart::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTradeCart::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTradeCart::OnPaint );
	m_slotPostPaint		= m_pThis->SigPaint().Connect( this, &CTradeCart::OnPostPaint );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CTradeCart::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CTradeCart::OnCaptureMouseDown );

	LinkMsg(g_EventShop);
	LinkMsg(g_EventUserTrade);
	LinkMsg(g_EventCalcPopupResult);
	LinkMsg( g_EventCharObjDelete );
	LinkMsg( g_EventIconMoveClick );	

	Show(false);

	Clear();

	NTL_RETURN(TRUE);
}

VOID CTradeCart::Destroy()
{
	NTL_FUNCTION( "CTradeCart::Destroy" );

	CheckInfoWindow();

	Logic_DeleteTexture(m_pMoneyIconTexture);
	m_pMoneyIconTexture = NULL;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		m_BuySlotInfo[i].slot.Destroy();
		m_SellSlotInfo[i].slot.Destroy();
	}

	UnLinkMsg(g_EventShop);
	UnLinkMsg(g_EventUserTrade);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventIconMoveClick);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTradeCart::Clear()
{
	m_MouseDownSlot.iSlotKind = -1;
	m_MouseDownSlot.iSlotIndex = -1;

	m_uiSubjectSerial = INVALID_SERIAL_ID;
	m_eDialogType = DIALOG_SHOPING_CART;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		ClearSlot(BUY_SLOT, i);
		ClearSlot(SELL_SLOT, i);
	}

	CalcTotalBuyPrice();
	CalcTotalSellPrice();
}

VOID CTradeCart::ClearSlot(RwInt32 iSlotKind, RwInt32 iSlot)
{
	if( iSlotKind == BUY_SLOT )
	{
		m_BuySlotInfo[iSlot].slot.Clear();
		memset((void*)&m_BuySlotInfo[iSlot].NPCShopBuyInfo, 0, sizeof(m_BuySlotInfo[iSlot].NPCShopBuyInfo) );
		memset((void*)&m_BuySlotInfo[iSlot].TakeItem, 0, sizeof(m_BuySlotInfo[iSlot].TakeItem) );
		m_BuySlotInfo[iSlot].TakeItem.itemNo = INVALID_SERIAL_ID;
		m_BuySlotInfo[iSlot].hGiveHandle = INVALID_SERIAL_ID;
	}
	else if( iSlotKind == SELL_SLOT )
	{
		// 가방으로 이벤트 보내기
		if( m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace >= CONTAINER_TYPE_BAG_FIRST && 
			m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace <= CONTAINER_TYPE_BAG_LAST )
		{
			GetIconMoveManager()->IconMoveEnd();

			RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG,
				iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, m_SellSlotInfo[iSlot].slot.GetSerial() );
		}		

		m_SellSlotInfo[iSlot].slot.Clear();
		memset((void*)&m_SellSlotInfo[iSlot].NPCShopSellInfo, 0, sizeof(m_SellSlotInfo[iSlot].NPCShopSellInfo) );
	}

	m_MouseDownSlot.iSlotKind = -1;
	m_MouseDownSlot.iSlotIndex = -1;
}

RwUInt32 CTradeCart::GetTargetSerial()
{
	return m_uiSubjectSerial;
}

VOID CTradeCart::ResetCartTexture(eTradeCartTextureType eType)
{
	if( m_eTextureType == eType )
		return;

	CNPCShop* pNPCShopGui = pNPCShopGui = reinterpret_cast<CNPCShop*>( GetDialogManager()->GetDialog(DIALOG_NPCSHOP) );
	NTL_ASSERT(pNPCShopGui, "CTradeCart::ResetCartTexture, Not exist NPCShop gui");
	pNPCShopGui->UnLink( m_pThis );
	
	CRectangle rect = m_pThis->GetScreenRect();
	Destroy();
	m_eTextureType = eType;
	Create();
	m_pThis->SetPosition(rect);

	pNPCShopGui->Link( m_pThis );
}

VOID CTradeCart::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;	

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;


	RwInt8 iSlot;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( m_eDialogType == DIALOG_SHOPING_CART )
	{
		iSlot = (RwInt8)PtinSlot(BUY_SLOT, pos.x - rtScreen.left, pos.y - rtScreen.top);
		if( iSlot != INVALID_INDEX )
		{
			if( sDelta > 0 )
				AddItem(BUY_SLOT, iSlot, 1, m_BuySlotInfo[iSlot].slot.GetItemTable()->tblidx);
			else
				SubItem(BUY_SLOT, iSlot, 1, m_BuySlotInfo[iSlot].slot.GetItemTable()->tblidx);

			return;
		}
	}


	iSlot = (RwInt8)PtinSlot(SELL_SLOT, pos.x - rtScreen.left, pos.y - rtScreen.top);
	if( iSlot != INVALID_INDEX )
	{
		if( sDelta > 0 )
			AddItem(SELL_SLOT, iSlot, 1, m_SellSlotInfo[iSlot].slot.GetItemTable()->tblidx);
		else
			SubItem(SELL_SLOT, iSlot, 1, m_SellSlotInfo[iSlot].slot.GetItemTable()->tblidx);
	}
}

VOID CTradeCart::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_NPCSHOP, key.m_fX, key.m_fY);
}

VOID CTradeCart::ClickedCloseButton(gui::CComponent* pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendTradeCancelReq(m_uiSubjectSerial);
}

VOID CTradeCart::ClickedZennyButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( GetIconMoveManager()->GetSrcPlace() != PLACE_SUB_BAG_ZENNY )
			return;

		// 제니 등록
		GetIconMoveManager()->IconMovePutDown(PLACE_SUB_TRADECART_ZENNY, m_uiSubjectSerial, INVALID_INDEX);
		m_uiTotalSellPrice = -1 * GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::USER_TRADE);
	}
	else
	{
		// 제니가 0 올려졌을 때에는 아무런 동작도 하지 않는다
		if( m_uiTotalSellPrice == 0 )
			return;

		CRectangle rtBtn = m_pZennyButton->GetScreenRect();
		CDboEventGenerator::CalcPopupShow(TRUE, INVALID_SERIAL_ID, PLACE_SUB_TRADECART_ZENNY,
										  rtBtn.right, rtBtn.bottom, m_uiTotalSellPrice);
	}
}

VOID CTradeCart::ClickedBuyButton(gui::CComponent* pComponent)
{
	switch(m_eDialogType)
	{
	case DIALOG_SHOPING_CART:
		{
			sSHOP_BUY_CART* pBuyInfo[MAX_SLOT];
			for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
			{
				pBuyInfo[i] = &m_BuySlotInfo[i].NPCShopBuyInfo;
			}

			CNPCShop* pNPCShopGui = reinterpret_cast<CNPCShop*>( GetDialogManager()->GetDialog(DIALOG_NPCSHOP) );

			if( pNPCShopGui->GetShopType() == CNPCShop::SHOP_TYPE_NORMAL )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopBuy(m_uiSubjectSerial, pBuyInfo);
			}
			else if( pNPCShopGui->GetShopType() == CNPCShop::SHOP_TYPE_TENKAICHI )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendEventItemShopBuyReq(m_uiSubjectSerial, pBuyInfo);
			}

			break;
		}
	default:
		{
			NTL_ASSERT(false, "CTradeCart::ClickedBuyButton, Wrong trade index");
		}
	}	
}

VOID CTradeCart::ClickedSellButton(gui::CComponent* pComponent)
{
	switch(m_eDialogType)
	{
	case DIALOG_SHOPING_CART:
		{
			sSHOP_SELL_CART* pSellInfo[MAX_SLOT];
			for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
			{			
				pSellInfo[i] = &m_SellSlotInfo[i].NPCShopSellInfo;				
			}
			GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopSell(m_uiSubjectSerial, pSellInfo);
			break;
		}
	case DIALOG_TRADECART:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendTradeEndReq(m_uiSubjectSerial, m_uiUserTradePacketCount, !m_bRightLock);
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CTradeCart::ClickedSellButto, Wrong trade index");
		}
	}
}

VOID CTradeCart::AddItemCount(RwInt32 iSlotKind, RwInt32 iSlot, RwInt32 iCount, RwUInt32 itemSerial)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable(itemSerial);
	if (!pITEM_DATA)
	{
		NTL_ASSERT(false, "CTradeCart::AddItemCount, Unknown tblidx " << itemSerial);
		return;
	}

	if( iSlotKind == BUY_SLOT )
	{
		RwInt32 iResult = m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack + iCount;
		
		if( iResult > pITEM_DATA->byMax_Stack)
			iResult = pITEM_DATA->byMax_Stack;

		if( iResult <= 0 )
		{
			ClearSlot(BUY_SLOT, iSlot);
			return;
		}

		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack = (BYTE)iResult;
		m_BuySlotInfo[iSlot].slot.SetCount(iResult);
	}
	else if( iSlotKind == SELL_SLOT )
	{
		RwInt32 iResult = m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack + iCount;

		if( iResult > pITEM_DATA->byMax_Stack)
			iResult = pITEM_DATA->byMax_Stack;

		if( iResult <= 0 )
		{
			ClearSlot(SELL_SLOT, iSlot);
			return;
		}

		m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack = (BYTE)iResult;
		m_SellSlotInfo[iSlot].slot.SetCount(iResult);
	}
}

VOID CTradeCart::UpdateCount(RwUInt8 bySlotKind, RwUInt8 bySlot, RwUInt8 byCount)
{
	if( bySlotKind == BUY_SLOT )
	{
		sITEM_TBLDAT* pITEM_DATA = m_BuySlotInfo[bySlot].slot.GetItemTable();

		if( byCount > pITEM_DATA->byMax_Stack)
			byCount = pITEM_DATA->byMax_Stack;

		if( byCount <= 0 )
		{
			ClearSlot(BUY_SLOT, bySlot);
			return;
		}

		m_BuySlotInfo[bySlot].NPCShopBuyInfo.byStack = byCount;
		m_BuySlotInfo[bySlot].slot.SetCount(byCount);
	}
	else if( bySlotKind == SELL_SLOT )
	{
		sITEM_TBLDAT* pITEM_DATA = m_SellSlotInfo[bySlot].slot.GetItemTable();

		if( byCount > pITEM_DATA->byMax_Stack)
			byCount = pITEM_DATA->byMax_Stack;

		if( byCount <= 0 )
		{
			ClearSlot(BUY_SLOT, bySlot);
			return;
		}

		m_SellSlotInfo[bySlot].NPCShopSellInfo.byStack = byCount;
		m_SellSlotInfo[bySlot].slot.SetCount(byCount);
	}
}

VOID CTradeCart::CalcTotalBuyPrice()
{
	RwInt32 iTotalPrice = 0;
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			sITEM_TBLDAT* pITEM_DATA = m_BuySlotInfo[i].slot.GetItemTable();

			if( TRADECART_TEXTURE_NORMAL == m_eTextureType )
				iTotalPrice += m_BuySlotInfo[i].NPCShopBuyInfo.byStack * pITEM_DATA->dwCost;
			else
				iTotalPrice += m_BuySlotInfo[i].NPCShopBuyInfo.byStack * pITEM_DATA->CommonPoint;
		}
	}

	m_pTotalBuyMoney->SetText(Logic_FormatZeni(iTotalPrice));


	if( m_eDialogType == DIALOG_SHOPING_CART )
	{
		if( iTotalPrice == 0 )
			m_pBuyButton->ClickEnable(false);
		else
			m_pBuyButton->ClickEnable(true);
	}	
}

VOID CTradeCart::CalcTotalSellPrice()
{
	DWORD dwSellPrice = 0;
	m_uiTotalSellPrice = 0;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_SellSlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			dwSellPrice = m_SellSlotInfo[i].slot.GetItemTable()->dwSell_Price * m_SellSlotInfo[i].NPCShopSellInfo.byStack;

			if(dwSellPrice <= 0 )
				dwSellPrice = 0;

			m_uiTotalSellPrice += dwSellPrice;
		}		
	}

	m_pTotalSellMoney->SetText(Logic_FormatZeni(m_uiTotalSellPrice));


	if( m_eDialogType == DIALOG_SHOPING_CART )
	{
		if( m_uiTotalSellPrice == 0 )
			m_pSellButton->ClickEnable(false);
		else
			m_pSellButton->ClickEnable(true);
	}	
}

VOID CTradeCart::ClickUpButton(gui::CComponent* pComponent)
{
	for( RwUInt8 i = 0 ; i< SLOTKIND_NUM ; ++i )
	{
		for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( m_pUpButton[i][j] == pComponent )
			{
				if (i == BUY_SLOT)
				{
					if(m_BuySlotInfo[j].slot.GetItemTable())
						AddItem(i, j, 1, m_BuySlotInfo[j].slot.GetItemTable()->tblidx);
				}
				else
				{
					if(m_SellSlotInfo[j].slot.GetItemTable())
						AddItem(i, j, 1, m_SellSlotInfo[j].slot.GetItemTable()->tblidx);
				}

				break;
			}
		}
	}
}

VOID CTradeCart::ClickDownButton(gui::CComponent* pComponent)
{
	for( RwUInt8 i = 0 ; i< SLOTKIND_NUM ; ++i )
	{
		for( RwUInt8 j = 0 ; j < MAX_SLOT ; ++j )
		{
			if( m_pDownButton[i][j] == pComponent )
			{
				if (i == BUY_SLOT)
				{
					if (m_BuySlotInfo[j].slot.GetItemTable())
						SubItem(i, j, 1, m_BuySlotInfo[j].slot.GetItemTable()->tblidx);
				}
				else
				{
					if (m_SellSlotInfo[j].slot.GetItemTable())
						SubItem(i, j, 1, m_SellSlotInfo[j].slot.GetItemTable()->tblidx);
				}

				break;
			}
		}
	}
}

VOID CTradeCart::OnMouseDown(const CKey& key)
{
	m_MouseDownSlot.iSlotKind = -1;
	m_MouseDownSlot.iSlotIndex = -1;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_MouseDownSlot.iSlotKind = BUY_SLOT;
			m_MouseDownSlot.iSlotIndex = i;

			m_pThis->CaptureMouse();
			return;
		}
		else if( m_SellSlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_MouseDownSlot.iSlotKind = SELL_SLOT;
			m_MouseDownSlot.iSlotIndex = i;

			m_pThis->CaptureMouse();
			return;
		}
	}
}

VOID CTradeCart::OnMouseUp(const CKey& key)
{	
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_MouseDownSlot.iSlotKind = INVALID_INDEX;
		m_MouseDownSlot.iSlotIndex = INVALID_INDEX;
		return;
	}
	
	for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		// 구입 슬롯
		if( m_BuySlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{				
			if( m_eDialogType == DIALOG_SHOPING_CART )
			{
				if( key.m_nID == UD_LEFT_BUTTON )	// 좌버튼
				{
					if( !GetIconMoveManager()->IsActive() )
						break;

					// NPC 상점에서 메세지를 보낸 경우
					if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
					{
						RegBuyItemByDrag(i);
						GetIconMoveManager()->IconMoveEnd();
					}					
				}
				else if( key.m_nID == UD_RIGHT_BUTTON )	// 우버튼
				{
					if( GetIconMoveManager()->IsActive() )
						break;

					if( m_MouseDownSlot.iSlotKind == BUY_SLOT &&
						m_MouseDownSlot.iSlotIndex == i )
					{
						ClearSlot(BUY_SLOT, m_MouseDownSlot.iSlotIndex);
						CalcTotalBuyPrice();
					}					
				}	
			}			

			// 해당슬롯의 아무런 처리도 하지 않았을 때
			break;
		}
		// 판매 슬롯
		else if( m_SellSlotInfo[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{	
			if( key.m_nID == UD_LEFT_BUTTON )	// 좌버튼
			{
				if( !GetIconMoveManager()->IsActive() )
					break;
				
				if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
				{
					if( m_eDialogType == DIALOG_SHOPING_CART )
					{
						// 캐릭터 가방에서 메세지를 보낸 경우
						RegSellItemByDrag(i);
						GetIconMoveManager()->IconMoveEnd();
					}
					else
					{
						if( m_SellSlotInfo[i].slot.IsOnlyUsableIcon() )
							break;

						if( m_SellSlotInfo[i].slot.GetSerial() == INVALID_SERIAL_ID )
						{
							// 아이템을 가방에서 옮겨왔다.
							GetDboGlobal()->GetGamePacketGenerator()->SendTradeAddReq(m_uiSubjectSerial,
								GetIconMoveManager()->GetSrcSerial(),
								(RwUInt8)GetIconMoveManager()->GetStackCount() );
						}					
					}
				}		
			}
			else if( key.m_nID == UD_RIGHT_BUTTON ) // 우버튼
			{
				if( GetIconMoveManager()->IsActive() )
					break;

				if( m_SellSlotInfo[i].slot.GetSerial() == INVALID_SERIAL_ID )
					break;

				if( m_MouseDownSlot.iSlotKind == SELL_SLOT &&
					m_MouseDownSlot.iSlotIndex == i )
				{
					if( m_eDialogType == DIALOG_SHOPING_CART )
					{
						ClearSlot(SELL_SLOT, m_MouseDownSlot.iSlotIndex);
						CalcTotalSellPrice();
					}					
					else
					{
						if( m_byOthersEmptySlot <= i )
							break;

						GetDboGlobal()->GetGamePacketGenerator()->SendTradeDelReq(m_uiSubjectSerial,
																	m_SellSlotInfo[m_MouseDownSlot.iSlotIndex].slot.GetSerial());
					}
				}
			}

			// 해당 슬롯의 아무런 처리도 하지 않았을 때
			break;
		}
	}

	m_MouseDownSlot.iSlotKind = INVALID_INDEX;
	m_MouseDownSlot.iSlotIndex = INVALID_INDEX;
}

VOID CTradeCart::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = GetPosition();

	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		m_BuySlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
		m_SellSlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
	}	

	m_LeftLockSurface.SetPositionbyParent(rect.left, rect.top);
	m_RightLockSurface.SetPositionbyParent(rect.left, rect.top);

	m_BuyBar				.SetPositionbyParent(rect.left, rect.top);
	m_SellBar				.SetPositionbyParent(rect.left, rect.top);
	m_GiveBar				.SetPositionbyParent(rect.left, rect.top);
	m_TakeBar				.SetPositionbyParent(rect.left, rect.top);
	m_srfZennySlotDestination.SetPositionbyParent(rect.left, rect.top);

	m_bFocus = FALSE;

	CheckInfoWindow();
}

VOID CTradeCart::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	FocusEffect(FALSE);	

	// 아이템 정보를 표시하고 포커스 이펙트를 보여준다
	for( RwInt8 i = 0 ; i < MAX_SLOT ; ++i )
	{		
		if( m_BuySlotInfo[i].slot.PtInRect(nX, nY) )
		{
			// 셀렉트 포커스
			FocusEffect(TRUE, BUY_SLOT, i);

			if( GetIconMoveManager()->IsActive() )
				return;

			if( m_BuySlotInfo[i].slot.GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION )
			{
				if( m_byInfoWindowIndex != i )
				{
					// 미확인 아이템
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
														rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
														rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(), NULL, m_eDialogType );
					m_byInfoWindowIndex = i;
				}
			}
			else if( m_BuySlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
			{
				if( m_eDialogType == DIALOG_SHOPING_CART )
				{
					if( m_byInfoWindowIndex != i )
					{
						CRectangle rtScreen = m_pThis->GetScreenRect();
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
															rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
															m_BuySlotInfo[i].slot.GetItemTable(), m_eDialogType );
						m_byInfoWindowIndex = i;
					}
				}				
				else
				{
					if( m_byInfoWindowIndex != i )
					{
						// 아이템의 업그레이드 효과를 표시하도록 수정
						// 2008-01-09 by Kell
						CRectangle rtScreen = m_pThis->GetScreenRect();
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM_DATA,
															rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
															&m_BuySlotInfo[i].TakeItem, m_eDialogType );
						m_byInfoWindowIndex = i;
					}
				}
			}
			else if( m_BuySlotInfo[i].slot.IsOnlyUsableIcon() )					
			{
				if( m_byInfoWindowIndex != i )
				{
					// 당신의 가방에 빈 공간이 모자랍니다
					CRectangle rtScreen = m_pThis->GetScreenRect();
					const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TRADE_YOUR_BAG_NOT_ENOUGH");
					GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
														rtScreen.left + m_BuySlotInfo[i].slot.GetX_fromParent(),
														rtScreen.top + m_BuySlotInfo[i].slot.GetY_fromParent(),
														(VOID*)pwcText, m_eDialogType );
					m_byInfoWindowIndex = i;
				}
			}

			return;
		}
		else if( m_SellSlotInfo[i].slot.PtInRect(nX, nY) )
		{
			// 셀렉트 포커스
			FocusEffect(TRUE, SELL_SLOT, i);

			if( GetIconMoveManager()->IsActive() )
				return;

			CRectangle rect = m_pThis->GetScreenRect();
			if( m_eDialogType == DIALOG_SHOPING_CART )
			{
				if( m_SellSlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
				{
					if( m_byInfoWindowIndex != i )
					{
						CRectangle rtScreen = m_pThis->GetScreenRect();
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
															rtScreen.left + m_SellSlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_SellSlotInfo[i].slot.GetY_fromParent(),
															m_SellSlotInfo[i].slot.GetSobItem(), m_eDialogType );
						m_byInfoWindowIndex = i;
					}
				}					
			}			
			else
			{
				if( m_SellSlotInfo[i].slot.IsOnlyUsableIcon() )
				{
					if( m_byInfoWindowIndex != i )
					{
						// 상대의 가방에 빈 공간이 모자랍니다
						CRectangle rtScreen = m_pThis->GetScreenRect();
						const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TRADE_THE_OTHERS_BAG_NOT_ENOUGH");
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
															rtScreen.left + m_SellSlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_SellSlotInfo[i].slot.GetY_fromParent(),
															(VOID*)pwcText, m_eDialogType );
						m_byInfoWindowIndex = i;
					}
				}
				else if( m_SellSlotInfo[i].slot.GetSerialType() == REGULAR_SLOT_ITEM_SOB_NOT_IDENTIFICATION )
				{
					if( m_byInfoWindowIndex != i )
					{
						// 미확인 아이템
						CRectangle rtScreen = m_pThis->GetScreenRect();
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
															rtScreen.left + m_SellSlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_SellSlotInfo[i].slot.GetY_fromParent(),
															NULL, m_eDialogType );
						m_byInfoWindowIndex = i;
					}
				}
				else if( m_SellSlotInfo[i].slot.GetSerial() != INVALID_SERIAL_ID )
				{
					if( m_byInfoWindowIndex != i )
					{
						CRectangle rtScreen = m_pThis->GetScreenRect();
						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
															rtScreen.left + m_SellSlotInfo[i].slot.GetX_fromParent(),
															rtScreen.top + m_SellSlotInfo[i].slot.GetY_fromParent(),
															m_SellSlotInfo[i].slot.GetSobItem(), m_eDialogType );
						m_byInfoWindowIndex = i;
					}						
				}
			}

			return;
		}		
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTradeCart::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTradeCart::FocusEffect( RwBool bPush, RwInt32 iSlotKind /* =0 */, RwInt32 iSlotIdx /* = -1 */)
{
	if( bPush)
	{
		RwInt32 iX = 18;
		RwInt32 iY = 57 + (iSlotIdx/2) * dGUI_SLOT_VERT_GAP;
		CRectangle rect = m_pThis->GetScreenRect();

		if( (iSlotIdx%2) != 0 )
			iX += dGUI_SLOT_HORI_GAP;

		if( iSlotKind == SELL_SLOT )
			iX += dGUI_BUY_SELL_SLOT_GAP;

		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}
}

VOID CTradeCart::OpenCart(eDialogType eType, RwUInt32 uiSerial)
{
	// 트레이드 카트를 연 주체의 시리얼 번호
	m_uiSubjectSerial = uiSerial;
	
	if( eType == DIALOG_SHOPING_CART )
		GetDialogManager()->OpenDialog(eType, INVALID_SERIAL_ID, FALSE);
	else
		GetDialogManager()->OpenDialog(eType);

	// 가방을 연다
	GetDialogManager()->SwitchBag( TRUE );
}

VOID CTradeCart::CloseCart(eDialogType eType)
{	
	Clear();

	if( eType == DIALOG_SHOPING_CART )
		GetDialogManager()->CloseDialog(eType, FALSE);
	else
	{
		GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::USER_TRADE, 0 );

		GetDialogManager()->CloseDialog(eType);
	}

	if( eType == DIALOG_TRADECART )
	{
		GetDialogManager()->UnRegistDialog(DIALOG_TRADECART);
		GetDialogManager()->RegistDialog(DIALOG_SHOPING_CART, this, &CTradeCart::SwitchDialog );
	}	

	m_uiTotalSellPrice = 0;
	m_byOthersEmptySlot = MAX_SLOT;
	m_uiUserTradePacketCount = 0;

	m_bLeftLock		= FALSE;
	m_bRightLock	= FALSE;	
}

VOID CTradeCart::AddItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CTradeCart::AddItem, Unknown index " << iSlotY);
		return;
	}

	if( iSlotKind == BUY_SLOT )
	{
		if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_BuySlotInfo[iSlotY].slot.GetItemTable()->byMax_Stack >= m_BuySlotInfo[iSlotY].slot.GetCount() + iCount )
			{
				AddItemCount(BUY_SLOT, iSlotY, iCount, itemTblidx);
				CalcTotalBuyPrice();
			}			
		}		
	}	
	else if( iSlotKind == SELL_SLOT )
	{
		if( m_SellSlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{			
			if( m_eDialogType == DIALOG_SHOPING_CART )
			{
				RwInt32 iNewCount = iCount + m_SellSlotInfo[iSlotY].slot.GetCount();
				CNtlSobItemAttr* pSobItemAttr = m_SellSlotInfo[iSlotY].slot.GetSobItemAttr();

				// 새로운 갯수가 자신이 가지고 있는 실제 아이템 갯수보다 많다면 리턴
				if( iNewCount > pSobItemAttr->GetStackNum() )
					return;

				AddItemCount(SELL_SLOT, iSlotY, iCount, itemTblidx);
				CalcTotalSellPrice();
			}
			else
			{
				RwInt32 iNewCount = 1 + m_SellSlotInfo[iSlotY].slot.GetCount();
				CNtlSobItemAttr* pSobItemAttr = m_SellSlotInfo[iSlotY].slot.GetSobItemAttr();

				if( !pSobItemAttr )
					return;

				// 새로운 갯수가 자신이 가지고 있는 실제 아이템 갯수보다 많다면 리턴
				if( iNewCount > pSobItemAttr->GetStackNum() )
					return;

				GetDboGlobal()->GetGamePacketGenerator()->SendTradeUpdateItem(m_uiSubjectSerial,
					m_SellSlotInfo[iSlotY].slot.GetSerial(),
					(RwUInt8)iNewCount );
			}			
		}		
	}
}

VOID CTradeCart::SubItem(RwInt32 iSlotKind, RwInt32 iSlotY, RwInt32 iCount, RwUInt32 itemTblidx)
{
	if( iSlotY < 0 || iSlotY >= MAX_SLOT )
	{
		NTL_ASSERT(false, "CTradeCart::SubItem, Unknown index " << iSlotY);
		return;
	}
	
	if( iSlotKind == BUY_SLOT )
	{
		if( m_BuySlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{			
			//RwInt32 iPrice = m_BuySlotInfo[iSlotY].slot.GetPrice();

			AddItemCount(BUY_SLOT, iSlotY, -iCount, itemTblidx);

			CalcTotalBuyPrice();
		}
	}	
	else if( iSlotKind == SELL_SLOT )
	{
		if( m_SellSlotInfo[iSlotY].slot.GetSerial() != INVALID_SERIAL_ID )
		{	
			if( m_eDialogType == DIALOG_SHOPING_CART )
			{
				AddItemCount(SELL_SLOT, iSlotY, -iCount, itemTblidx);
				CalcTotalSellPrice();
			}
			else
			{
				RwInt32 iSlotCount = m_SellSlotInfo[iSlotY].slot.GetCount() - 1;
				DBO_WARNING_MESSAGE("iSlotCount: " << iSlotCount);
				if(iSlotCount > 0)
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendTradeUpdateItem(m_uiSubjectSerial,
						m_SellSlotInfo[iSlotY].slot.GetSerial(),
						(RwUInt8)iSlotCount);
				}
				else if( iSlotCount == 0)
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendTradeDelReq(m_uiSubjectSerial,
						m_SellSlotInfo[iSlotY].slot.GetSerial());
				}
			}			
		}		
	}
}

VOID CTradeCart::RegBuyItemByDrag(RwInt32 iSlot)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( GetIconMoveManager()->GetSrcSerial() );
	if(!pITEM_DATA)
		return;

	RwUInt8 byMerchantTab	= (BYTE)GetIconMoveManager()->GetEXData1();
	RwUInt8 byItemPos		= (BYTE)GetIconMoveManager()->GetSrcSlotIdx();

	// 기간제 아이템
	if( eDURATIONTYPE_FLATSUM		== pITEM_DATA->byDurationType ||
		eDURATIONTYPE_METERRATE		== pITEM_DATA->byDurationType )
	{
		sSHOP_BUY_CART tSHOP_BUY_CART;

		tSHOP_BUY_CART.byMerchantTab	= byMerchantTab;
		tSHOP_BUY_CART.byItemPos		= byItemPos;
		tSHOP_BUY_CART.byStack			= GetIconMoveManager()->GetStackCount();

		CDboEventGenerator::StartProcessBuyDurItemNPC(pITEM_DATA->tblidx, m_uiSubjectSerial, tSHOP_BUY_CART);
		return;
	}


	if( m_BuySlotInfo[iSlot].slot.GetCount() <= 0 )
	{
		// 빈 슬롯이면 등록
		RwInt32 iCount = GetIconMoveManager()->GetStackCount();	
		RwInt32 iPrice = GetIconMoveManager()->GetEXData2();

		CRectangle rect = m_pThis->GetScreenRect();					
		m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
		m_BuySlotInfo[iSlot].slot.SetIcon(GetIconMoveManager()->GetSrcSerial(), iCount);
		m_BuySlotInfo[iSlot].slot.SetPrice(iPrice);

		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byMerchantTab	= byMerchantTab;
		m_BuySlotInfo[iSlot].NPCShopBuyInfo.byItemPos		= byItemPos;

		AddItemCount(BUY_SLOT, iSlot, iCount, GetIconMoveManager()->GetSrcSerial());
		CalcTotalBuyPrice();

		return;						
	}
	else if( m_BuySlotInfo[iSlot].slot.GetSerial() == GetIconMoveManager()->GetSrcSerial() )
	{
		if( pITEM_DATA->byMax_Stack > m_BuySlotInfo[iSlot].NPCShopBuyInfo.byStack)
		{
			// If the same item as the previously registered item does not exceed the maximum number of slots that can be placed in the slot
			AddItem(BUY_SLOT, iSlot, 1, GetIconMoveManager()->GetSrcSerial());
		}			
	}
}

VOID CTradeCart::RegSellItemByDrag(RwInt32 iSlot)
{
	// 클라이언트와 서버의 Bag 인덱스가 다르기에 컨버트한다.			
	RwUInt32 uiSerial = GetIconMoveManager()->GetSrcSerial();	
	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));
	NTL_ASSERT(pSobItem, "CTradeCart::RegSellItemByDrag, Can not found sob item of the handle : " << uiSerial );
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "CTradeCart::RegSellItemByDrag, Can not found sob item attr of the handle : " << uiSerial );	

	if( pSobItemAttr->IsNeedToIdentify() )
	{
		// 팔 수 없는 아이템입니다
		GetAlarmManager()->AlarmMessage("DST_TRADECART_CANNOT_SELL_ITEM");

		return;
	}

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	NTL_ASSERT(pITEM_TBLDAT, "CTradeCart::RegSellItemByDrag, Not exist item table of item handle " << uiSerial);

	if( Logic_IsCanSellItem( pSobItemAttr ) == FALSE )
	{
		// This item can not be sold
		GetAlarmManager()->AlarmMessage("DST_TRADECART_CANNOT_SELL_ITEM");

		return;
	}

	if (pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		GetAlarmManager()->AlarmMessage("DST_SEALING_NOT_EXTRACT");
		return;
	}
			
	// 이미 판매 슬롯에 등록된 아이템은 다른 슬롯에 등록할 수 없다.
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( i != iSlot )
		{
			if( m_SellSlotInfo[i].slot.GetSerial() == uiSerial )
				return;
		}
	}

	if( m_SellSlotInfo[iSlot].slot.GetCount() <= 0 )
	{
		// 빈 슬롯이면 등록
		CRectangle rect = m_pThis->GetScreenRect();				
		RwInt32 iCount = GetIconMoveManager()->GetStackCount();
		RwUInt32 uiPrice = pITEM_TBLDAT->dwSell_Price;

		m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
		m_SellSlotInfo[iSlot].slot.SetIcon(uiSerial, iCount);
		m_SellSlotInfo[iSlot].slot.SetPrice(uiPrice);

		// 가방 번호
		m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace = (BYTE)Logic_WhichBagHasItem(uiSerial);

		// 가방 안의 슬롯 인덱스
		m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos = (BYTE)GetIconMoveManager()->GetSrcSlotIdx();		

		// 갯수
		AddItemCount(SELL_SLOT, iSlot, iCount, pITEM_TBLDAT->tblidx);

		// 가격
		CalcTotalSellPrice();

		// 가방으로 이벤트 보내기
		RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
		CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG, 
			iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, uiSerial);
	}
	else if( m_SellSlotInfo[iSlot].slot.GetSerial() == uiSerial )
	{
		AddItem(SELL_SLOT, iSlot, 1, pITEM_TBLDAT->tblidx);
	}
}

VOID CTradeCart::RegBuyItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo)
{
	sITEM_TBLDAT* pITEM_DATA = Logic_GetItemDataFromTable( TradeInfo.uiSerial );
	if( !pITEM_DATA )
	{
		DBO_FAIL("Not exist item table of index : " << TradeInfo.uiSerial);
		return;
	}


	// 기간제 아이템
	if( eDURATIONTYPE_FLATSUM		== pITEM_DATA->byDurationType ||
		eDURATIONTYPE_METERRATE		== pITEM_DATA->byDurationType )
	{
		sSHOP_BUY_CART tSHOP_BUY_CART;

		tSHOP_BUY_CART.byMerchantTab	= TradeInfo.iPlace;
		tSHOP_BUY_CART.byItemPos		= TradeInfo.iPosition;
		tSHOP_BUY_CART.byStack			= TradeInfo.iOverlapCount;

		CDboEventGenerator::StartProcessBuyDurItemNPC(pITEM_DATA->tblidx, m_uiSubjectSerial, tSHOP_BUY_CART);
		return;
	}


	CRectangle rect = m_pThis->GetScreenRect();	

	m_BuySlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
	m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
	m_BuySlotInfo[iSlot].slot.SetIcon(TradeInfo.uiSerial, TradeInfo.iOverlapCount);
	m_BuySlotInfo[iSlot].slot.SetPrice(TradeInfo.ulPrice);

	m_BuySlotInfo[iSlot].NPCShopBuyInfo.byMerchantTab = (BYTE)TradeInfo.iPlace;
	m_BuySlotInfo[iSlot].NPCShopBuyInfo.byItemPos = (BYTE)TradeInfo.iPosition;

	AddItemCount(BUY_SLOT, iSlot, TradeInfo.iOverlapCount, TradeInfo.uiSerial);
	CalcTotalBuyPrice();
}

VOID CTradeCart::RegSellItemByEvent(RwInt32 iSlot, SDboEventShop& TradeInfo)
{		
	CRectangle rect = m_pThis->GetScreenRect();	
	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(TradeInfo.uiSerial));
	NTL_ASSERT(pSobItem, "CTradeCart::RegSellItemByEvent, Can not found sob item of the handle : " << TradeInfo.uiSerial );
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	NTL_ASSERT(pSobItemAttr, "CTradeCart::RegSellItemByEvent, Can not found sob item attr of the handle : " << TradeInfo.uiSerial );	

	if( pSobItemAttr->IsNeedToIdentify() )
	{
		// 팔 수 없는 아이템
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
		return;
	}

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	NTL_ASSERT(pITEM_TBLDAT, "CTradeCart::RegSellItemByEvent, Not exist item table of item handle " << TradeInfo.uiSerial);

	if( Logic_IsCanSellItem( pSobItemAttr ) == FALSE )
	{
		// 팔 수 없는 아이템
		GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_SELL");
		return;
	}

	if (pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		GetAlarmManager()->AlarmMessage("DST_SEALING_NOT_EXTRACT");
		return;
	}

	m_SellSlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_SOB);
	m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
	m_SellSlotInfo[iSlot].slot.SetIcon(TradeInfo.uiSerial, TradeInfo.iOverlapCount);
	m_SellSlotInfo[iSlot].slot.SetPrice(pITEM_TBLDAT->dwSell_Price);

	// 가방 번호
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace = (BYTE)Logic_WhichBagHasItem(TradeInfo.uiSerial);

	// 가방 안의 슬롯 인덱스
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos = (BYTE)TradeInfo.iPosition;		

	// 갯수
	// 이벤트로 판매 아이템을 등록한 경우는 AddItemCount(..)함수를 써서 갯수를 더하지 않고
	// TradeInfo.iOverlapCount 갯수 그대로 세팅한다.
	if( TradeInfo.iOverlapCount > m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack)
		TradeInfo.iOverlapCount = m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack;

	m_SellSlotInfo[iSlot].NPCShopSellInfo.byStack = (BYTE)TradeInfo.iOverlapCount;
	m_SellSlotInfo[iSlot].slot.SetCount(TradeInfo.iOverlapCount);

	// 가격
	CalcTotalSellPrice();

	// 가방으로 이벤트 보내기
	RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
	CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG, 
									iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, m_SellSlotInfo[iSlot].slot.GetSerial() );
}

VOID CTradeCart::RegTakeItemByPacket(RwInt32 iSlot, SERIAL_HANDLE hGiveHandle, sITEM_DATA* pITEM_DATA, RwUInt8 byCount)
{
	CRectangle rect = m_pThis->GetScreenRect();		

	if( false == pITEM_DATA->bNeedToIdentify )
	{
		m_BuySlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
		m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
		m_BuySlotInfo[iSlot].slot.SetIcon(pITEM_DATA->itemNo, byCount);		
	}
	else
	{
		//미확인 아이템
		m_BuySlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_NOT_IDENTIFICATION);
		m_BuySlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);						
		m_BuySlotInfo[iSlot].slot.SetIcon(hGiveHandle);
	}

	m_BuySlotInfo[iSlot].TakeItem = *pITEM_DATA;
	m_BuySlotInfo[iSlot].hGiveHandle = hGiveHandle;

	AddItemCount(BUY_SLOT, iSlot, byCount, pITEM_DATA->itemNo);
}

VOID CTradeCart::RegGiveItemByPacket(RwInt32 iSlot, RwUInt32 uiSerial, RwUInt8 byCount)
{
	CRectangle rect = m_pThis->GetScreenRect();	
	CNtlSobItemAttr* pSobItemAttr = Logic_GetItemSobAttrFromSobSerial( uiSerial );
	sITEM_TBLDAT* pITEM_DATA = NULL;
	
	if( pSobItemAttr )
		pITEM_DATA = pSobItemAttr->GetItemTbl();
	
	if( pITEM_DATA )
	{
		if( Logic_IsCanUserTradeItem( pSobItemAttr ) == FALSE )
		{
			// 유저간 거래할 수 없는 아이템
			GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_TRADE");
			return;
		}

		m_SellSlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_SOB);
		m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
		m_SellSlotInfo[iSlot].slot.SetIcon(uiSerial, byCount);		

		m_SellSlotInfo[iSlot].slot.SetCount(byCount);

		if (byCount > m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack)
			byCount = m_SellSlotInfo[iSlot].slot.GetItemTable()->byMax_Stack;
	}
	else
	{
		if( m_SellSlotInfo[iSlot].slot.GetSerial() != INVALID_SERIAL_ID )
			return;

		// 미확인 아이템
		m_SellSlotInfo[iSlot].slot.SetSerialType(REGULAR_SLOT_ITEM_SOB_NOT_IDENTIFICATION);
		m_SellSlotInfo[iSlot].slot.SetParentPosition(rect.left, rect.top);
		m_SellSlotInfo[iSlot].slot.SetIcon(uiSerial);
	}

	// 가방 번호
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace = (BYTE)Logic_WhichBagHasItem(uiSerial);

	// 가방 안의 슬롯 인덱스
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( uiSerial ) );
	m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos = (BYTE)pItem->GetItemSlotIdx();
	
	GetIconMoveManager()->IconMoveEnd();

	// 가방으로 이벤트 보내기
	RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(m_SellSlotInfo[iSlot].NPCShopSellInfo.byPlace);
	CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_TRADECART, PLACE_BAG, 
		iBagIndex, m_SellSlotInfo[iSlot].NPCShopSellInfo.byPos, m_SellSlotInfo[iSlot].slot.GetSerial() );
}

RwInt32 CTradeCart::FindEmptySlot(RwInt32 iSlotType)
{
	for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
	{
		if( iSlotType == BUY_SLOT )
		{
			if( m_BuySlotInfo[i].slot.GetCount() <= 0 )
				return i;
		}
		else if( iSlotType == SELL_SLOT )
		{
			if( m_SellSlotInfo[i].slot.GetCount() <= 0 )
				return i;

		}
	}

	return -1;
}

RwUInt8 CTradeCart::FindSlot(RwUInt8 bySlotType, RwUInt32 uiSerial)
{
	if( bySlotType == BUY_SLOT )
	{
		for( RwUInt8 i = 0 ; i < MAX_SLOT ; ++i )
		{   
			if( m_eDialogType == DIALOG_SHOPING_CART )
			{
				if( m_BuySlotInfo[i].slot.GetSerial() == uiSerial )
					return i;
			}
			else
			{
				if( m_BuySlotInfo[i].hGiveHandle == uiSerial )
					return i;
			}
		}
	}
	else
	{
		for( RwUInt8 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_SellSlotInfo[i].slot.GetSerial() == uiSerial )
			{
				return i;
			}
		}
	}

	return INVALID_SLOTID;
}

RwInt32	 CTradeCart::PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY)
{
	if( iSlotKind == BUY_SLOT )
	{
		for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_BuySlotInfo[i].slot.PtInRect(iX, iY)  )
				return i;
		}
	}
	else if( iSlotKind == SELL_SLOT )
	{
		for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
		{
			if( m_SellSlotInfo[i].slot.PtInRect(iX, iY)  )
				return i;
		}
	}

	return INVALID_INDEX;
}

VOID CTradeCart::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SHOPING_CART ||
		GetInfoWndManager()->GetRequestGui() == DIALOG_TRADECART )
	{
        m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );		
	}
}

RwInt32 CTradeCart::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TRADE) )
		{
			ClickedCloseButton(NULL);
		}
		else
		{
			Show(false);
			CheckInfoWindow();
		}
	}

	return 1;
}

VOID CTradeCart::OnPaint()
{
	for( RwInt32 i = 0 ; i< MAX_SLOT ; ++i )
	{	
		m_BuySlotInfo[i].slot.Paint();
		m_SellSlotInfo[i].slot.Paint();
	}	

	if( m_bFocus )
		m_FocusEffect.Render();

	if( m_bLeftLock )
		m_LeftLockSurface.Render();

	if( m_bRightLock )
		m_RightLockSurface.Render();

	if( m_eDialogType == DIALOG_SHOPING_CART )
	{
		m_BuyBar.Render();
		m_SellBar.Render();
	}
	else if( m_eDialogType == DIALOG_TRADECART )
	{
		m_GiveBar.Render();
		m_TakeBar.Render();
	}
}

VOID CTradeCart::OnPostPaint()
{
	if( m_eDialogType == DIALOG_TRADECART &&
		m_bZennySlotDestination )
	{
		m_srfZennySlotDestination.Render();
	}
}

VOID CTradeCart::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CTradeCart::HandleEvents" );

	if( msg.Id == g_EventShop )
	{
		SDboEventShop* pPacket = (SDboEventShop*)msg.pData;
		switch(pPacket->iType)
		{
			case TRM_NPCSHOP_OPEN:
			{
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(pPacket->uiSerial);
				if( !pNPC_TBLDAT )
				{
					DBO_FAIL("Not exist npc table of sob handle : " << pPacket->uiSerial);
					NTL_RETURNVOID();
				}

				// 상점의 종류에 따라 텍스처가 달라진다
				if( pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT2 || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT3)
					ResetCartTexture(TRADECART_TEXTURE_TENKAICHI);
				else
					ResetCartTexture(TRADECART_TEXTURE_NORMAL);


				//  NPC 상점을 위한 카트
				m_eDialogType = DIALOG_SHOPING_CART;

				m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_NPCSHOP_SHOPINGCART"));

				m_pBuyButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_BUY"));
				m_pSellButton->SetText(GetDisplayStringManager()->GetString("DST_TRADECART_SELL"));

				m_pBuyButton->ClickEnable(false);
				m_pSellButton->ClickEnable(false);

				m_pZennyButton->Show(false);

				m_pExitButton->Show(false);
				m_pBuyButton->Show(true);				
				
				for(RwUInt8 i = 0 ; i < MAX_SLOT ; ++i)
				{
					m_pUpButton[BUY_SLOT][i]->Show(true);
					m_pDownButton[BUY_SLOT][i]->Show(true);
				}


				SetMovable(false);

				OpenCart(DIALOG_SHOPING_CART, pPacket->uiSerial);				
			}
			break;
			case TRM_REG_ITEM_BY_NPCSHOP:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				RwInt8 iSlot = INVALID_BYTE;

				// Add if you have already registered items.
				for( RwInt8 i = 0 ; i < MAX_SLOT; ++i )
				{
					if( m_BuySlotInfo[i].slot.GetSerial() == pPacket->uiSerial )
					{
						if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].NPCShopBuyInfo.byStack)
						{
							// If the maximum number of stacks has not been exceeded
							AddItem(BUY_SLOT, i, 1, m_BuySlotInfo[i].slot.GetItemTable()->tblidx);
							iSlot = i;

							break;
						}
					}
				}

				// If it is not an existing registered item, it finds an empty slot and registers it.
				if(iSlot == INVALID_INDEX)
				{
					iSlot = (RwInt8)FindEmptySlot(BUY_SLOT);

					if( iSlot >= 0 )
						RegBuyItemByEvent(iSlot, *pPacket);
					else
					{
						// 카트에 슬롯이 부족합니다
						GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
					}
				}				
			}
			break;
			case TRM_REG_ITEM_BY_NPCSHOP_LARGE_BUY:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				RwInt8 iSlot = (RwInt8)FindEmptySlot(BUY_SLOT);
				if( iSlot >= 0 )
				{
					// 빈 슬롯을 찾아서 20개를 등록한다
					RegBuyItemByEvent(iSlot, *pPacket);
					NTL_RETURNVOID();
				}
				else
				{					
					for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					{
						if( m_BuySlotInfo[i].slot.GetSerial() == pPacket->uiSerial )
						{
							// 최대 스택갯수를 초과하지 않았다면
							if(m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack > m_BuySlotInfo[i].slot.GetCount())
							{
								// 마저 더해서 20개를 채운다
								AddItem(BUY_SLOT, i, m_BuySlotInfo[i].slot.GetItemTable()->byMax_Stack - m_BuySlotInfo[i].slot.GetCount(), m_BuySlotInfo[i].slot.GetItemTable()->tblidx);
								NTL_RETURNVOID();
							}							
						}
					}
				}

				GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
			}
			break;
			case TRM_REG_ITEM_BY_BAG:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				RwInt32 iSlot = INVALID_INDEX;

				// 기존에 등록된 아이템이면 다시 등록한다.
				for( RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
				{
					if( m_SellSlotInfo[i].slot.GetSerial() == pPacket->uiSerial )
					{
						// 가방에서 이벤트로 팔 아이템을 넘겨주었다.
						// 이 뜻은 그 슬롯에 있는 모든 아이템을 판다는 뜻이다.
						RegSellItemByEvent(i, *pPacket);
						iSlot = i;

						break;
					}
				}

				// 빈 슬롯이면 등록				
				if( iSlot == INVALID_INDEX )
				{					
					iSlot = FindEmptySlot(SELL_SLOT);

					if( iSlot >= 0 )
						RegSellItemByEvent(iSlot, *pPacket);
					else
					{
						// 카트에 슬롯이 부족합니다
						GetAlarmManager()->AlarmMessage("DST_TRADECART_NO_MORE_SLOT");
					}
				}			
			}
			break;
			case TRM_BUY_SUCCESS:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					ClearSlot(BUY_SLOT, i);

				CalcTotalBuyPrice();

				CheckInfoWindow();
			}
			break;
			case TRM_SELL_SUCCESS:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				for(RwInt32 i = 0 ; i < MAX_SLOT ; ++i )
					ClearSlot(SELL_SLOT, i);

				CalcTotalSellPrice();

				CheckInfoWindow();
			}
			break;
			case TRM_CLOSE:
			{
				if (m_uiSubjectSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				CloseCart(DIALOG_SHOPING_CART);
			}
			break;
		}
		
	}
	else if( msg.Id == g_EventUserTrade )
	{
		SDboEventUserTrade* pPacket = (SDboEventUserTrade*)msg.pData;
		switch(pPacket->byEventType)
		{
		case USERTRADE_START:
			{	
				m_eDialogType = DIALOG_TRADECART;

				ResetCartTexture(TRADECART_TEXTURE_NORMAL);

				SetMovable(true);

				CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pPacket->uiTarget);
				NTL_ASSERT(pSob, "CTradeCart::HandleEvents, Not exist trade target of handle : " << pPacket->uiTarget);
				
				m_pDialogName->SetText(Logic_GetName(pSob));
				m_pSellButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));

				m_pBuyButton->ClickEnable(true);
				m_pSellButton->ClickEnable(true);

				m_pZennyButton->Show(true);

				m_pExitButton->Show(true);
				m_pBuyButton->Show(false);

				for(RwUInt8 i = 0 ; i < MAX_SLOT ; ++i)
				{
					m_pUpButton[BUY_SLOT][i]->Show(false);
					m_pDownButton[BUY_SLOT][i]->Show(false);
				}				

				// 내가 상대에게 아이템을 줄 수 있는 활성화된 슬롯 갯수
				// 상대의 가방에 빈 슬롯 갯수이다
				if(pPacket->byCount < MAX_SLOT)
				{
					m_byOthersEmptySlot = pPacket->byCount;
					CRectangle rect = GetPosition();

					for(RwUInt8 i = m_byOthersEmptySlot ; i < MAX_SLOT ; ++i)
					{
						m_SellSlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
						m_SellSlotInfo[i].slot.SetIcon(UNABLE_SLOT_NAME);
					}
				}

				// 나의 가방의 빈 슬롯 갯수
				// 상대가 나에게 줄 수 있는 아이템의 갯수이다
				RwInt32 iEmptySlot = Logic_GetCountEmptySlotofBag();

				if(iEmptySlot < MAX_SLOT)
				{
					CRectangle rect = GetPosition();

					for(RwInt32 i = iEmptySlot ; i < MAX_SLOT ; ++i)
					{
						m_BuySlotInfo[i].slot.SetParentPosition(rect.left, rect.top);
						m_BuySlotInfo[i].slot.SetIcon(UNABLE_SLOT_NAME);
					}
				}

				
				GetDialogManager()->UnRegistDialog(DIALOG_SHOPING_CART);
				GetDialogManager()->RegistDialog(DIALOG_TRADECART, this, &CTradeCart::SwitchDialog );

				OpenCart(DIALOG_TRADECART, pPacket->uiTarget);				

				break;
			}
		case USERTRADE_ADD_ITEM:
			{
				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 bySlot = (RwUInt8)FindEmptySlot(SELL_SLOT);
				if( bySlot != INVALID_SLOTID )
					RegGiveItemByPacket(bySlot, pPacket->uiHandle, pPacket->byCount);

				break;
			}
		case USERTRADE_ADD_ITEM_NOTIFY:
			{
				// 서버와의 동기화를 위해 업데이트 된 상대방의 패킷의 갯수를 센다
				++m_uiUserTradePacketCount;

				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 bySlot = (RwUInt8)FindEmptySlot(BUY_SLOT);
				if( bySlot != INVALID_SLOTID )
					RegTakeItemByPacket(bySlot, pPacket->uiHandle, reinterpret_cast<sITEM_DATA*>(pPacket->pData), pPacket->byCount);

				break;
			}
		case USERTRADE_DEL_ITEM:
			{
				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 byIndex = FindSlot(SELL_SLOT, pPacket->uiHandle);
				ClearSlot(SELL_SLOT, byIndex);

				CheckInfoWindow();

				break;
			}
		case USERTRADE_DEL_ITEM_NOTIFY:
			{
				// 서버와의 동기화를 위해 업데이트 된 패킷의 갯수를 센다
				++m_uiUserTradePacketCount;

				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 byIndex = FindSlot(BUY_SLOT, pPacket->uiHandle);
				ClearSlot(BUY_SLOT, byIndex);

				CheckInfoWindow();

				break;
			}
		case USERTRADE_UPDATE_ITEM:
			{
				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 byIndex = (RwUInt8)FindSlot(SELL_SLOT, pPacket->uiHandle);									
				DBO_WARNING_MESSAGE("UPDATE COUNT: " << (int)pPacket->byCount);
				UpdateCount(SELL_SLOT, byIndex, pPacket->byCount);

				break;
			}
		case USERTRADE_UPDATE_ITEM_NOTIFY:
			{
				// 서버와의 동기화를 위해 업데이트 된 상대방의 패킷의 갯수를 센다
				++m_uiUserTradePacketCount;

				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				RwUInt8 byIndex = (RwUInt8)FindSlot(BUY_SLOT, pPacket->uiHandle);				

				UpdateCount(BUY_SLOT, byIndex, pPacket->byCount);

				break;
			}
		case USERTRADE_UPDATE_ZENNY:
			{
				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				m_pTotalSellMoney->SetText(Logic_FormatZeni(pPacket->uiHandle));
				m_uiTotalSellPrice = pPacket->uiHandle;
				GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::USER_TRADE, -1 * pPacket->uiHandle);

				break;
			}
		case USERTRADE_UPDATE_ZENNY_NOTIFY:
			{
				// 서버와의 동기화를 위해 업데이트 된 상대방의 패킷의 갯수를 센다
				++m_uiUserTradePacketCount;

				// 서버락이 풀렸다
				m_bLeftLock		= FALSE;
				m_bRightLock	= FALSE;

				m_pTotalBuyMoney->SetText(Logic_FormatZeni(pPacket->uiHandle));

				break;
			}
		case USERTRADE_REDAY:
			{
				m_bRightLock = *((bool*)pPacket->pData);

				if(m_bRightLock )
				{
					CRectangle rect = GetPosition();
					m_RightLockSurface.SetPositionbyParent(rect.left, rect.top);
				}

				// 거래가 성립되어 카트를 닫는다
				if( m_bLeftLock && m_bRightLock )
				{
					GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_TRADE);
					CloseCart(DIALOG_TRADECART);
				}

				break;
			}
		case USERTRADE_REDAY_NOTIFY:
			{
				m_bLeftLock = *((bool*)pPacket->pData);

				if(m_bLeftLock)
				{
					CRectangle rect = GetPosition();
					m_LeftLockSurface.SetPositionbyParent(rect.left, rect.top);
				}

				// 거래가 성립되어 카트를 닫는다
				if( m_bLeftLock && m_bRightLock )
				{
					GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_TRADE);
					CloseCart(DIALOG_TRADECART);
				}

				break;
			}
		case USERTRADE_END:
			{
				GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_TRADE);
				CloseCart(DIALOG_TRADECART);

				break;
			}
		}
	}
	else if( msg.Id == g_EventCalcPopupResult )
	{
		SDboEventCalcPopupResult* pData = reinterpret_cast<SDboEventCalcPopupResult*>( msg.pData );

		if( pData->nSrcPlace == PLACE_SUB_TRADECART_ZENNY )
		{
			GetIconMoveManager()->IconMovePickUp(INVALID_SERIAL_ID, PLACE_SUB_TRADECART_ZENNY, INVALID_INDEX, pData->uiValue, m_pMoneyIconTexture);
			m_uiTotalSellPrice = -1 * GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny(SAdjustZennyInfo::USER_TRADE);
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_uiSubjectSerial == *pDeleteSerial )
		{
			// 갑자기 상대 캐릭터가 사라지는 경우
			if( GetDialogManager()->IsOpenDialog(DIALOG_SHOPING_CART) )
				CloseCart(DIALOG_SHOPING_CART);
			else
			{
				GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_TRADE);
				CloseCart(DIALOG_TRADECART);
			}
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
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

