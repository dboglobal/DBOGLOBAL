#include "precomp_dboclient.h"
#include "NetPyShopGui.h"

// Core
#include "NtlDebug.h"

// Shared
#include "TableContainer.h"
#include "NPCTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "MerchantTable.h"
#include "GraphicDataTable.h"

// Sound
#include "GUISoundDefine.h"

// Presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// Dbo
#include "IconMoveManager.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"


#define dNETPYSHOP_TEM_NAME_START_X					68
#define dNETPYSHOP_TEM_NAME_START_Y					64
#define dNETPYSHOP_CON_START_X						27
#define dNETPYSHOP_CON_START_Y						68
#define dNETPYSHOP_PRICE_START_X					204
#define dNETPYSHOP_PRICE_START_Y					86
#define dNETPYSHOP_SLOT_GAP_HORI					54
#define dNETPYSHOP_METAL_SMALL_SIZE					15
#define dNETPYSHOP_SMALL_ICON_INFO_SLOT_INDEX_GAP	100

CNetPyShopGui::CNetPyShopGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(dNETPYSHOP_INVALID_INDEX)
,m_iSelectedSlot(dNETPYSHOP_INVALID_INDEX)
,m_iClickEffectedSlot(dNETPYSHOP_INVALID_INDEX)
,m_pExitButton(NULL)
,m_pPrePageButton(NULL)
,m_pNextPageButton(NULL)	
,m_pShopTitle(NULL)
,m_pPocketMoneytitle(NULL)
,m_pPocketMoney(NULL)
,m_pCurrentPage(NULL)
,m_eShopType(SHOP_TYPE_NETPY)
,m_iCurTab(0)
,m_iCurPage(0)
,m_bFocus(false)
,m_iInfoWindowIndex(dNETPYSHOP_INVALID_INDEX)
,m_pNetStoreButton(NULL)
{
}

CNetPyShopGui::~CNetPyShopGui()
{
}

RwBool CNetPyShopGui::Create()
{
	NTL_FUNCTION( "CNetPyShopGui::Create" );

	m_adwNETPYSHOP_MERCHANT_TBLIDX[0] = 1001;
	m_adwNETPYSHOP_MERCHANT_TBLIDX[1] = 1002;
	m_adwNETPYSHOP_MERCHANT_TBLIDX[2] = 1003;
	m_adwNETPYSHOP_MERCHANT_TBLIDX[3] = 1004;

	char acSurfaceName[64];

	//// Todo : 추후 Shop Gui의 Surface가 변경될 상황이 생긴다면 다음과 같은 코드를 이용한다.
	//if( m_eShopType == SHOP_TYPE_NETPY )
	//{

	if(!CNtlPLGui::Create("", "gui\\NetPyShopGui.srf", "gui\\NetPyShopGui.frm"))
		NTL_RETURN(FALSE);

	sprintf_s(acSurfaceName, 64, "gui\\NetPyShopGui.srf");
	
	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 상점 이름	
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pShopTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pShopTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pShopTitle->Clear();
	m_pShopTitle->Enable(false);

	rect = GetPosition();

	// 백라인
	m_BackLineSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackLineSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineTop" ) );
	m_BackLineSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineCenter" ) );
	m_BackLineSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineBottom" ) );
	m_BackLineSurface.SetSize(303, 351);
	m_BackLineSurface.SetPositionfromParent(9, 50);

	// 탭 버튼
	m_pTabButton = (gui::CTabButton*)GetComponent( "TabButton" );
	m_slotTab = m_pTabButton->SigSelectChanged().Connect( this, &CNetPyShopGui::OnSelectChangeTabButton );	

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	// 소지금 배경 ( 현재 없음 요청해야함 )
	m_MoneyBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "MoneyBackPanel" ) );
	m_MoneyBackPanel.SetPositionfromParent(194, 439);

	// 페이지 배경
	m_PageBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "PageBackPanel" ) );
	m_PageBackPanel.SetPositionfromParent(203, 406);

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CNetPyShopGui::ClickedCloseButton);		

	// ItemPanel
	std::string fullName = "";
	char acPanelName[] = "ItemPanel";
	char acNum[3] = "";
	RwInt32	 iItemNamePosY = dNETPYSHOP_TEM_NAME_START_Y;
	RwInt32	 iIconY = dNETPYSHOP_CON_START_Y;
	RwInt32	 iPricePosY = dNETPYSHOP_PRICE_START_Y;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		_itoa_s(i, acNum, sizeof(acNum), 10);

		// Panel
		fullName = acPanelName;
		fullName += acNum;
		m_ItemPanel[i].pItemPanel = (gui::CPanel*)GetComponent( fullName.c_str() );
		m_ItemPanel[i].pItemPanel->Enable(false);

		// Icon
		m_ItemPanel[i].slot.Create(m_pThis, DIALOG_NETPYSHOP, REGULAR_SLOT_ITEM_TABLE);
		m_ItemPanel[i].slot.SetPosition_fromParent(dNETPYSHOP_CON_START_X, iIconY);		

		// Item Name
		rect.SetRectWH( dNETPYSHOP_TEM_NAME_START_X, iItemNamePosY, 200, 16);
		m_ItemPanel[i].pItemName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		m_ItemPanel[i].pItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pItemName->Clear();
		m_ItemPanel[i].pItemName->Enable(false);

		// Need Item
		m_ItemPanel[i].srfNeedItem.SetPositionfromParent(dNETPYSHOP_TEM_NAME_START_X, iItemNamePosY + 22);
		m_ItemPanel[i].srfNeedItem.SetSize(dNETPYSHOP_METAL_SMALL_SIZE, dNETPYSHOP_METAL_SMALL_SIZE);

		// 필요한 포인트
		rect.SetRectWH( dNETPYSHOP_PRICE_START_X, iPricePosY, 68, 16);
		m_ItemPanel[i].pPoint = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		m_ItemPanel[i].pPoint->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pPoint->SetTextColor( RGB(202, 202, 202) );
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pPoint->Enable(false);

		iItemNamePosY += dNETPYSHOP_SLOT_GAP_HORI;
		iIconY += dNETPYSHOP_SLOT_GAP_HORI;
		iPricePosY += dNETPYSHOP_SLOT_GAP_HORI;
	}		

	// 대량 구매 설명
	rect.SetRectWH(16, 379, 282, 20);
	m_pLargeBuyExplan = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pLargeBuyExplan->CreateFontStd( "detail", 90, DEFAULT_FONT_ATTR);
	m_pLargeBuyExplan->SetText(GetDisplayStringManager()->GetString( "DST_NPCSHOP_LARGE_BUY_EXPLAIN" ));
	m_pLargeBuyExplan->Enable(false);

	// 나의 소지금 제목
	rect.SetRectWH(135, 442, 63, 17);
	m_pPocketMoneytitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pPocketMoneytitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPocketMoneytitle->SetText(GetDisplayStringManager()->GetString( "DST_NETPY" ));
	m_pPocketMoneytitle->Enable(false);

	// 나의 소지금	
	rect.SetRectWH(185, 441, 90, 17);
	m_pPocketMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pPocketMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPocketMoney->SetTextColor( RGB(202, 202, 202) );
	m_pPocketMoney->SetText("");
	m_pPocketMoney->Enable(false);

	// 현재 페이지 표시
	rect.SetRectWH(217, 409, 46, 16);
	m_pCurrentPage = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pCurrentPage->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pCurrentPage->SetTextColor( RGB(255, 192, 0) );

	// PrePageButton
	m_pPrePageButton = (gui::CButton*)GetComponent( "PrePageButton" );
	m_slotPrePageButton = m_pPrePageButton->SigClicked().Connect(this, &CNetPyShopGui::ClickedPrePageButton);

	// ClickedNextPageButton Button
	m_pNextPageButton = (gui::CButton*)GetComponent( "NextPageButton" );
	m_slotNextPageButton = m_pNextPageButton->SigClicked().Connect(this, &CNetPyShopGui::ClickedNextPageButton);

	m_pNetStoreButton = (gui::CButton*)GetComponent( "NetStoreButton" );
	m_pNetStoreButton->SetText( GetDisplayStringManager()->GetString("DST_NETSTORE" ) );
	m_slotClickedNetStore = m_pNetStoreButton->SigClicked().Connect( this, &CNetPyShopGui::ClickedNetStoreButton );
	

	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CNetPyShopGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CNetPyShopGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CNetPyShopGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CNetPyShopGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CNetPyShopGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNetPyShopGui::OnPaint );
	m_slotPostPaint		= m_pNextPageButton->SigPaint().Connect( this, &CNetPyShopGui::OnPostPaint );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CNetPyShopGui::OnCaptureMouseDown );

	ClearShop();
	OnMove(100, 100);

	Show(false);

	LinkMsg( g_EventSobInfoUpdate );
	LinkMsg( g_EventPickedUpHide );
	LinkMsg( g_EventNetPyShopEvent );
	LinkMsg( g_EventDialog );

	NTL_RETURN(TRUE);
}

VOID CNetPyShopGui::Destroy()
{
	NTL_FUNCTION( "CNetPyShopGui::Destroy" );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventNetPyShopEvent );
	UnLinkMsg( g_EventDialog );

	CheckInfoWindow();	

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
		m_ItemPanel[i].slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CNetPyShopGui::ResetCartTexture(eShopType eType)
{
	if( m_eShopType == eType )
		return;

	CRectangle rect = m_pThis->GetScreenRect();
	Destroy();
	m_eShopType = eType;
	Create();
	m_pThis->SetPosition(rect);
}

VOID CNetPyShopGui::OpenShop()
{
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	// 상점 이름
	m_pShopTitle->SetText( GetDisplayStringManager()->GetString(" DST_NETPYSHOP" ) );

	// NetPy Shop 에서 정의된 Index 4가지의 탭을 불러온다.
	ShopItem shopItem;
	char acBuffer[256] = "";
	for(RwInt32 iTabIndex = 0 ; iTabIndex < dNETPYSHOP_TAB_NUMS; ++iTabIndex )
	{	
		if( m_adwNETPYSHOP_MERCHANT_TBLIDX[iTabIndex] <= 0 )
			continue;

		// 각 Tab 별 등록된 아이템을 읽어온다.
		sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable(m_adwNETPYSHOP_MERCHANT_TBLIDX[iTabIndex]);
		if(!pMERCHANT_TBLDAT)
			continue;

		// Tab 이름			
		const wchar_t* pwcMerchantName = pMerchantTextTable->GetText(pMERCHANT_TBLDAT->Tab_Name).c_str();
		WideCharToMultiByte(GetACP(), 0, pwcMerchantName, -1, acBuffer, 256, NULL, NULL);
		std::string str = acBuffer;
		m_pTabButton->AddTab(str);

		// 각 Tab별 등록된 아이템을 ShopItem에 등록한다			
		sITEM_TBLDAT* pITEM_DATA;			
		for( RwInt32 iMerchantIndex = 0 ; iMerchantIndex < NTL_MAX_MERCHANT_COUNT ; ++iMerchantIndex )
		{				
			pITEM_DATA = Logic_GetItemDataFromTable(pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex]);
			if(!pITEM_DATA)
				continue;

			// 실제 ShopItem 데이터				
			if( pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex] == 0 )
			{
				m_aShopItem[iTabIndex][iMerchantIndex].hItem = INVALID_SERIAL_ID;
			}
			else
			{
				m_aShopItem[iTabIndex][iMerchantIndex].hItem = pMERCHANT_TBLDAT->aitem_Tblidx[iMerchantIndex];
			}

			pItemTextTable->GetText(pITEM_DATA->Name, &m_aShopItem[iTabIndex][iMerchantIndex].wstrItemName);			
			m_aShopItem[iTabIndex][iMerchantIndex].pITEM_DATA = pITEM_DATA;
			m_aShopItem[iTabIndex][iMerchantIndex].uiPrice = pITEM_DATA->CommonPoint;
		}
	}		

	// 소지금
	SetZenny();

	// 최초 탭
	m_pTabButton->SelectTab(0);
	UpdateTabContent(0);

	// Todo :: OpenDialog
	GetDialogManager()->OpenDialog( DIALOG_NETPYSHOP );
	
}

VOID CNetPyShopGui::CloseShop()
{
	if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) || 
		GetDialogManager()->IsMode( DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION ) )
		GetDialogManager()->OffMode();

	// Todo :: CloseDialog
	GetDialogManager()->CloseDialog( DIALOG_NETPYSHOP );
	
	ClearShop();
}


VOID CNetPyShopGui::ClearShop()
{
	m_pShopTitle->Clear();

	ClearPanels();

	m_pPocketMoney->Clear();
	m_pCurrentPage->Clear();

	for( RwInt32 i = 0 ; i < NTL_MAX_MERCHANT_TAB_COUNT ; ++i )
	{
		for( RwInt32 j = 0 ; j < NTL_MAX_MERCHANT_COUNT ; ++j )
		{
			m_aShopItem[i][j].hItem		= INVALID_SERIAL_ID;			
			m_aShopItem[i][j].wstrItemName.clear();
			m_aShopItem[i][j].pITEM_DATA		= NULL;
			m_aShopItem[i][j].pNeedITEM_DATA	= NULL;
		}
	}

	m_pTabButton->ClearTab();

	m_iCurTab = 0;
	SetPage(-1);

	SelectEffect(false);
}

VOID CNetPyShopGui::ClearPanels()
{
	m_iMouseDownSlot = -1;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.Clear();
		m_ItemPanel[i].pItemName->Clear();		
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pItemPanel->Show(false);
		m_ItemPanel[i].bNeedItem = false;

		Logic_DeleteTexture(m_ItemPanel[i].srfNeedItem.GetTexture());
		m_ItemPanel[i].srfNeedItem.UnsetTexture();
	}
}

VOID CNetPyShopGui::OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex )
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
		GetIconMoveManager()->IconMoveEnd();

	UpdateTabContent((RwUInt8)nCurIndex);
}

VOID CNetPyShopGui::UpdateTabContent(RwUInt8 byIndex)
{
	m_iCurTab = byIndex;

	ClearPanels();
	SetPage(dFIRST_PAGE);
	SetPanel(dFIRST_PAGE);
}

VOID CNetPyShopGui::ClickedCloseButton(gui::CComponent* pComponent)
{	
	// Todo :: CloseDialog
	GetDboGlobal()->GetGamePacketGenerator()->SendShopNetPyItemEndReq();
}

bool CNetPyShopGui::SetPage(RwInt32 iPage)
{
	m_iCurPage = iPage;

	if(m_iCurPage < 0)
	{
		m_pCurrentPage->Clear();
		return false;
	}
	else
	{
		RwUInt8 byPageCount = GetPageCount_of_CurTab();
		m_pCurrentPage->Format(L"%d / %d", m_iCurPage+1, byPageCount);
		return true;		
	}
}

VOID CNetPyShopGui::SetPanel(RwInt32 iPage)
{
	RwInt32 iIndex;
	CRectangle rtScreen = m_pThis->GetScreenRect();


	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		iIndex = (iPage * dMAX_ITEM_PANEL ) + i;

		m_ItemPanel[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);

		if( m_ItemPanel[i].slot.SetIcon(m_aShopItem[m_iCurTab][iIndex].hItem) )
		{			
			m_ItemPanel[i].slot.SetPrice(m_aShopItem[m_iCurTab][iIndex].uiPrice);
			m_ItemPanel[i].pItemName->SetText(m_aShopItem[m_iCurTab][iIndex].wstrItemName.c_str());			
			m_ItemPanel[i].pItemPanel->Show(true);
			m_ItemPanel[i].pPoint->SetText(m_aShopItem[m_iCurTab][iIndex].uiPrice);
		}		
	}

	SelectEffect(false);
	SetPageButton();
}

VOID CNetPyShopGui::SetPageButton()
{
	if( IsFirstPage() )
		m_pPrePageButton->ClickEnable(false);
	else
		m_pPrePageButton->ClickEnable(true);

	if( IsLastPage() )
		m_pNextPageButton->ClickEnable(false);
	else
		m_pNextPageButton->ClickEnable(true);
}

VOID CNetPyShopGui::ClickedPrePageButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
		GetIconMoveManager()->IconMoveEnd();

	if( IsFirstPage() )
		return;

	--m_iCurPage;
	ClearPanels();
	SetPage(m_iCurPage);
	SetPanel(m_iCurPage);
} 

VOID CNetPyShopGui::ClickedNextPageButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
		GetIconMoveManager()->IconMoveEnd();

	if( IsLastPage() )
		return;

	++m_iCurPage;
	ClearPanels();
	SetPage(m_iCurPage);
	SetPanel(m_iCurPage);
}

VOID CNetPyShopGui::ClickedNetStoreButton( gui::CComponent* pComponent )
{
	/*if( !GetDialogManager()->IsOpenDialog( DIALOG_CAPSULE_NETPY ) )
		GetDialogManager()->OpenDialog( DIALOG_CAPSULE_NETPY );*/

	GetDialogManager()->SwitchDialog( DIALOG_CAPSULE_NETPY );
}

VOID CNetPyShopGui::SelectItem(RwInt32 iPanelIndex)
{	
	RwInt32 iItemIndex = (m_iCurPage*dMAX_ITEM_PANEL) + iPanelIndex;
	CDboEventGenerator::NetPyShopEvent(eNETPYSHOP_EVENT_REG_ITEM,
		m_aShopItem[m_iCurTab][iItemIndex].hItem,
		m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
		(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
		m_iCurTab, iItemIndex, 1);
}

VOID CNetPyShopGui::SetZenny()
{
	m_pPocketMoney->SetText( (RwUInt32)Logic_GetNetPy());
}

RwUInt8 CNetPyShopGui::GetPageCount_of_CurTab()
{
	RwUInt8 byLastIndex = 0;

	// 아이템 슬롯은 중간에 비어있을 수도 있다
	for( RwUInt8 i = 0 ; i < NTL_MAX_MERCHANT_COUNT ; ++i )
	{
		if( m_aShopItem[m_iCurTab][i].hItem != INVALID_SERIAL_ID )
			byLastIndex = i;
	}

	byLastIndex /= dMAX_ITEM_PANEL;

	return byLastIndex + 1;
}

CNetPyShopGui::eShopType CNetPyShopGui::GetShopType()
{
	return m_eShopType;
}

RwBool CNetPyShopGui::IsFirstPage()
{
	if( dFIRST_PAGE == m_iCurPage )
		return true;

	return false;
}

RwBool CNetPyShopGui::IsLastPage()
{
	RwInt32 iIndex;
	RwInt32 iNextPage = m_iCurPage + 1;

	if( m_iCurPage >= dMAX_PAGE -1 )
		return true;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		iIndex = (iNextPage * dMAX_ITEM_PANEL ) + i;
		if( m_aShopItem[m_iCurTab][iIndex].hItem != INVALID_SERIAL_ID )
			return false;
	}

	return true;
}

RwInt32 CNetPyShopGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if( m_ItemPanel[i].slot.PtInRect(iX, iY) )
			return i ;
	}

	return dNETPYSHOP_INVALID_INDEX;
}

RwInt32 CNetPyShopGui::PtinSlot_of_SmallIcon_in_panel(RwInt32 iX, RwInt32 iY)
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if( m_ItemPanel[i].srfNeedItem.PtInRect(iX, iY) )
			return i;
	}

	return dNETPYSHOP_INVALID_INDEX;
}

VOID CNetPyShopGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iY = dNETPYSHOP_CON_START_Y + iSlotIdx*dNETPYSHOP_SLOT_GAP_HORI;
		CRectangle rect = m_pThis->GetScreenRect();

		m_FocusEffect.SetRectWH(rect.left + dNETPYSHOP_CON_START_X, rect.top + iY, 32, 32);
		m_bFocus = true;
	}
	else
	{
		m_bFocus = false;
	}
}

VOID CNetPyShopGui::SelectEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if( bPush)
	{
		RwInt32 iY = dNETPYSHOP_CON_START_Y + dNETPYSHOP_SLOT_GAP_HORI*iSlotIdx;
		CRectangle rect = m_pThis->GetScreenRect();

		m_SelectEffect.SetRectWH(rect.left + dNETPYSHOP_CON_START_X, rect.top + iY, 32, 32);
		m_iSelectedSlot = iSlotIdx;
	}
	else
		m_iSelectedSlot = dNETPYSHOP_INVALID_INDEX;
}

VOID CNetPyShopGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_NETPYSHOP )
	{
		m_iInfoWindowIndex = dNETPYSHOP_INVALID_INDEX;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CNetPyShopGui::OnMouseDown( const CKey& key )
{
	// 현재 다이얼로그에 포커스가 들어오면 링크된 다이얼로그들도 화면의 최상단으로 옮긴다.
	gui::CGUIManager *pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();  
	if( pGuiMgr->GetFocus() == m_pThis )
		RaiseLinked();

	if( GetIconMoveManager()->IsActive() )
		return;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		// 아이콘 영역에서 마우스를 눌렀다
		if( m_ItemPanel[i].slot.GetSerial() != INVALID_SERIAL_ID &&
			m_ItemPanel[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			m_iMouseDownSlot = i;
			m_pThis->CaptureMouse();

			// 클릭 이벤트 시작
			m_iClickEffectedSlot = i;	
			m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(true);

			return;
		}
	}
}

VOID CNetPyShopGui::OnMouseUp( const CKey& key )
{
	m_pThis->ReleaseMouse();

	// 클릭 이벤트 종료	
	if( m_iClickEffectedSlot != dNETPYSHOP_INVALID_INDEX )
	{		
		m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		m_iClickEffectedSlot = dNETPYSHOP_INVALID_INDEX;
	}

	if( !IsShow() )
	{
		m_iMouseDownSlot = dNETPYSHOP_INVALID_INDEX;
		return;
	}

	if( m_iMouseDownSlot < 0 || m_iMouseDownSlot >= dMAX_ITEM_PANEL )
		return;

	// 아이콘 영역에서 마우스를 눌렀다
	if( m_ItemPanel[m_iMouseDownSlot].slot.GetSerial() != INVALID_SERIAL_ID &&
		m_ItemPanel[m_iMouseDownSlot].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			RwInt32 iListIndex = (m_iCurPage*dMAX_ITEM_PANEL) + m_iMouseDownSlot;
			GetIconMoveManager()->IconMovePickUp(m_ItemPanel[m_iMouseDownSlot].slot.GetSerial(), PLACE_NPCSHOP,
				iListIndex, 1, m_ItemPanel[m_iMouseDownSlot].slot.GetTexture(), m_iCurTab, m_ItemPanel[m_iMouseDownSlot].slot.GetPrice() );	

			SelectEffect(true, m_iMouseDownSlot);
		}
		else if( key.m_nID == UD_RIGHT_BUTTON )
		{
			RwInt32 iItemIndex = (m_iCurPage*dMAX_ITEM_PANEL) + m_iMouseDownSlot;

			if( key.m_dwVKey == UD_MK_CONTROL )
			{
				// 최대 스택 갯수를 산다(1개 혹은 20개)
				CDboEventGenerator::NetPyShopEvent(eNETPYSHOP_EVENT_REG_ITEM_MAX,
					m_aShopItem[m_iCurTab][iItemIndex].hItem,
					m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
					(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
					m_iCurTab, iItemIndex, m_aShopItem[m_iCurTab][iItemIndex].pITEM_DATA->byMax_Stack);
			}
			else
			{				
				CDboEventGenerator::NetPyShopEvent(eNETPYSHOP_EVENT_REG_ITEM,
					m_aShopItem[m_iCurTab][iItemIndex].hItem,
					m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
					(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
					m_iCurTab, iItemIndex, 1);
			}
		}
	}

	m_iMouseDownSlot = dNETPYSHOP_INVALID_INDEX;
}

VOID CNetPyShopGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_MoneyBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);	
	m_PageBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);

	// NPCShop 다이얼로그가 움직일 때	
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);
		m_ItemPanel[i].srfNeedItem.SetPositionbyParent(rtScreen.left, rtScreen.top);
	}

	m_BackLineSurface.SetPositionbyParent(rtScreen.left, rtScreen.top);

	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);

	CheckInfoWindow();
}

VOID CNetPyShopGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt32 iPtinSlot = PtinSlot(nX, nY);

	if( iPtinSlot != dNETPYSHOP_INVALID_INDEX )
	{
		// 슬롯 클릭 이펙트
		if( m_iClickEffectedSlot != dNETPYSHOP_INVALID_INDEX )
		{
			if( m_iClickEffectedSlot == iPtinSlot )
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(true);
			else
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		}

		// 현재 아이템 패널의 인포
		if( m_ItemPanel[iPtinSlot].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			FocusEffect(true, iPtinSlot);

			if( m_iInfoWindowIndex != iPtinSlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
					rtScreen.left + m_ItemPanel[iPtinSlot].slot.GetX_fromParent(),
					rtScreen.top + m_ItemPanel[iPtinSlot].slot.GetY_fromParent(),
					m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iPtinSlot].pITEM_DATA, DIALOG_NETPYSHOP );
				m_iInfoWindowIndex = iPtinSlot;
			}	
		}
		else
		{
			m_iInfoWindowIndex = dNETPYSHOP_INVALID_INDEX;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}
	else
	{
		RwInt32 iSmallIconSlot = PtinSlot_of_SmallIcon_in_panel(nX, nY);
		if( iSmallIconSlot != dNETPYSHOP_INVALID_INDEX )
		{
			// 아이템을 사기 위해서 가지고 있어야 하는 아이템 정보
			if( m_ItemPanel[iSmallIconSlot].slot.GetSerial() != INVALID_SERIAL_ID )
			{
				FocusEffect(true, iSmallIconSlot);

				if( m_iInfoWindowIndex != iSmallIconSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();

					if( m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pNeedITEM_DATA )
					{
						GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_ITEM_NAME,
							rtScreen.left + m_ItemPanel[iSmallIconSlot].slot.GetX_fromParent(),
							rtScreen.top + m_ItemPanel[iSmallIconSlot].slot.GetY_fromParent(),
							m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pNeedITEM_DATA, DIALOG_NETPYSHOP );
					}
					else
					{
						WCHAR awcBuffer[128] = L"";
						swprintf_s(awcBuffer, 128, L"Not exist item table of index : %u", m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pITEM_DATA->NeedItemTblidx);
						GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
							rtScreen.left + m_ItemPanel[iSmallIconSlot].slot.GetX_fromParent(),
							rtScreen.top + m_ItemPanel[iSmallIconSlot].slot.GetY_fromParent(),
							(VOID*)awcBuffer, DIALOG_NETPYSHOP );
					}

					m_iInfoWindowIndex = iSmallIconSlot + dNETPYSHOP_SMALL_ICON_INFO_SLOT_INDEX_GAP;
				}	
			}
			else
			{
				m_iInfoWindowIndex = INVALID_BYTE;
				GetInfoWndManager()->ShowInfoWindow( FALSE );
			}
		}
		else
		{
			FocusEffect(false);

			if( m_iClickEffectedSlot != dNETPYSHOP_INVALID_INDEX )
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);

			m_iInfoWindowIndex = INVALID_BYTE;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}	
}

VOID CNetPyShopGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(false);
	m_iInfoWindowIndex = dNETPYSHOP_INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CNetPyShopGui::OnPaint()
{
	m_BackLineSurface.Render();
	m_MoneyBackPanel.Render();
	m_PageBackPanel.Render();	
}

VOID CNetPyShopGui::OnPostPaint()
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.Paint();

		if( m_ItemPanel[i].bNeedItem )
			m_ItemPanel[i].srfNeedItem.Render();
	}	

	if( m_iSelectedSlot != dNETPYSHOP_INVALID_INDEX )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CNetPyShopGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_NETPYSHOP, key.m_fX, key.m_fY);
}

RwInt32 CNetPyShopGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
			GetIconMoveManager()->IconMoveEnd();

		CheckInfoWindow();
	}

	return 1;
}

VOID CNetPyShopGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CNetPyShopGui::HandleEvents" );

	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_NETPY, msg.pData) == false )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		// 상점이 열려있는 상태라면 나의 소지금 업데이트
		SetZenny();
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_NPCSHOP )
			return;

		SelectEffect(false);
	}
	else if( msg.Id == g_EventNetPyShopEvent )
	{
		SDboEventNetPyShopEvent* pData = reinterpret_cast<SDboEventNetPyShopEvent*>( msg.pData );

		switch( pData->byEventType )
		{
		case eNETPYSHOP_EVENT_START:
			{
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog( DIALOG_NETPYSHOP_TRADE );
				pPLGui->SetPosition( rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top );

				OpenShop();
			}
			break;
		case eNETPYSHOP_EVENT_REG_ITEM:
			break;
		case eNETPYSHOP_EVENT_REG_ITEM_MAX:
			break;
		case eNETPYSHOP_EVENT_BUY_SUCCESS:
			break;
		case eNETPYSHOP_EVENT_END:
			{
				CloseShop();
			}
			break;
		default:
			break;
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		switch( pEvent->iDestDialog )
		{
		case DIALOG_NETPYSHOP:
			{
				switch( pEvent->iType )
				{
				case DIALOGEVENT_NPC_BYEBYE:
					{
						ClickedCloseButton(NULL);

						break;
					}
				}
			}
		}
	}
	
	NTL_RETURNVOID();
}