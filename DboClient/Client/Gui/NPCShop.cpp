#include "precomp_dboclient.h"
#include "NPCShop.h"

// shared
#include "TableContainer.h"
#include "NPCTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "MerchantTable.h"
#include "GraphicDataTable.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
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


#define dITEM_NAME_START_X		68
#define dITEM_NAME_START_Y		64
#define dICON_START_X			27
#define dICON_START_Y			68
#define dPRICE_START_X			204
#define dPRICE_START_Y			86
#define dSLOT_GAP_HORI			54
#define dMETAL_SMALL_SIZE		15
#define dSMALL_ICON_INFO_SLOT_INDEX_GAP		100


CNPCShop::CNPCShop(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(INVALID_INDEX)
,m_iSelectedSlot(INVALID_INDEX)
,m_iClickEffectedSlot(INVALID_INDEX)
,m_pExitButton(NULL)
,m_pRepairButton(NULL)
,m_pRepairAllButton(NULL)
,m_pPrePageButton(NULL)
,m_pNextPageButton(NULL)	
,m_pShopTitle(NULL)
,m_pPocketMoneytitle(NULL)
,m_pPocketMoney(NULL)
,m_pCurrentPage(NULL)
,m_eShopType(SHOP_TYPE_NORMAL)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_iCurTab(0)
,m_iCurPage(0)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
{
}

CNPCShop::~CNPCShop()
{
}

RwBool CNPCShop::Create()
{
	NTL_FUNCTION( "CNPCShop::Create" );


	char acSurfaceName[64];


	// The texture changes according to whether or not it is the shop of the first ball of the world.
	switch(m_eShopType)
	{
		case SHOP_TYPE_NORMAL:
		{
			if(!CNtlPLGui::Create("", "gui\\NPCShop.srf", "gui\\NPCShop.frm"))
				NTL_RETURN(FALSE);

			sprintf_s(acSurfaceName, 64, "NPCShop.srf");

			break;
		}
		case SHOP_TYPE_TENKAICHI:
		{
			if(!CNtlPLGui::Create("gui\\TenkaichiShop.rsr", "gui\\TenkaichiShop.srf", "gui\\TenkaichiShop.frm"))
				NTL_RETURN(FALSE);

			sprintf_s(acSurfaceName, 64, "TenkaichiShop.srf");

			break;
		}
		default:
		{
			DBO_FAIL("Wrong shop type");
			break;
		}
	}

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 상점 이름	
	//rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 175, 16);
	m_pShopTitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pShopTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pShopTitle->Clear();
	m_pShopTitle->Enable(false);

	rect = GetPosition();

	// background
	m_BackLineSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackLineSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineTop" ) );
	m_BackLineSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineCenter" ) );
	m_BackLineSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "srfBackLineBottom" ) );
	m_BackLineSurface.SetSize(303, 351);
	m_BackLineSurface.SetPositionfromParent(9, 50);

	// Tab button
	m_pTabButton = (gui::CTabButton*)GetComponent( "TabButton" );
	m_slotTab = m_pTabButton->SigSelectChanged().Connect( this, &CNPCShop::OnSelectChangeTabButton );	

	// Slot focus effect
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// Slot Select Effect
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	// Zeni background
	m_MoneyBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "MoneyBackPanel" ) );
	m_MoneyBackPanel.SetPositionfromParent(194, 439);

	// Page background
	m_PageBackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( acSurfaceName, "PageBackPanel" ) );
	m_PageBackPanel.SetPositionfromParent(203, 406);

	// Exit Button
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CNPCShop::ClickedCloseButton);		

	// ItemPanel
	std::string fullName = "";
	char acPanelName[] = "ItemPanel";
	char acNum[3] = "";
	RwInt32	 iItemNamePosY = dITEM_NAME_START_Y;
	RwInt32	 iIconY = dICON_START_Y;
	RwInt32	 iPricePosY = dPRICE_START_Y;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		_itoa_s(i, acNum, sizeof(acNum), 10);

		// Panel
		fullName = acPanelName;
		fullName += acNum;
		m_ItemPanel[i].pItemPanel = (gui::CPanel*)GetComponent( fullName.c_str() );
		m_ItemPanel[i].pItemPanel->Enable(false);

		// Icon
		m_ItemPanel[i].slot.Create(m_pThis, DIALOG_NPCSHOP, REGULAR_SLOT_ITEM_TABLE);
		m_ItemPanel[i].slot.SetPosition_fromParent(dICON_START_X, iIconY);

		// Item Name
		rect.SetRectWH( dITEM_NAME_START_X, iItemNamePosY, 200, 16);
		m_ItemPanel[i].pItemName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
		m_ItemPanel[i].pItemName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pItemName->Clear();
		m_ItemPanel[i].pItemName->Enable(false);



		// Need Item
		m_ItemPanel[i].srfNeedItem.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("TenkaichiShop.srf", "srfNeedItemBackpanel"));
		m_ItemPanel[i].srfNeedItem.SetPositionfromParent(dITEM_NAME_START_X + 55, iItemNamePosY + 20);
		m_ItemPanel[i].srfNeedItem.Show(false); // hide by default

		m_ItemPanel[i].srfNeedItemIcon.SetPositionfromParent(dITEM_NAME_START_X + 105, iPricePosY);
		m_ItemPanel[i].srfNeedItemIcon.SetSize(dMETAL_SMALL_SIZE, dMETAL_SMALL_SIZE);
		m_ItemPanel[i].srfNeedItemIcon.Show(false); // hide by default

		rect.SetRectWH(dITEM_NAME_START_X + 75, iPricePosY, 68, 16);
		m_ItemPanel[i].pstbNeedItemCount = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
		m_ItemPanel[i].pstbNeedItemCount->SetTextColor(INFOCOLOR_0);
		m_ItemPanel[i].pstbNeedItemCount->SetText(L"1 x");
		m_ItemPanel[i].pstbNeedItemCount->Enable(false);
		m_ItemPanel[i].pstbNeedItemCount->Show(false); // hide by default


		// Points required
		rect.SetRectWH( dPRICE_START_X, iPricePosY, 68, 16);
		m_ItemPanel[i].pPoint = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
		m_ItemPanel[i].pPoint->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		m_ItemPanel[i].pPoint->SetTextColor( RGB(202, 202, 202) );
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pPoint->Enable(false);

		iItemNamePosY += dSLOT_GAP_HORI;
		iIconY += dSLOT_GAP_HORI;
		iPricePosY += dSLOT_GAP_HORI;
	}		

	// Bulk purchase explanation
	rect.SetRect(16, 379, 298, 399);
	m_pLargeBuyExplan = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pLargeBuyExplan->CreateFontStd( "detail", 90, DEFAULT_FONT_ATTR);
	m_pLargeBuyExplan->SetText(GetDisplayStringManager()->GetString( "DST_NPCSHOP_LARGE_BUY_EXPLAIN" ));
	m_pLargeBuyExplan->Enable(false);

	// My salary title
	rect.SetRect(90, 441, 188, 458);
	m_pPocketMoneytitle = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pPocketMoneytitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	
	m_pPocketMoneytitle->Enable(false);

	//My revenues
	rect.SetRect(185, 441, 280, 458);
	m_pPocketMoney = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pPocketMoney->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPocketMoney->SetTextColor( RGB(202, 202, 202) );
	m_pPocketMoney->SetText(L"");
	m_pPocketMoney->Enable(false);

	// Repair button
	m_pRepairButton = (gui::CButton*)GetComponent( "RepairButton" );
	m_pRepairButton->SetToolTip(GetDisplayStringManager()->GetString("DST_NPCSHOP_ITEM_REPAIR"));
	m_slotRepairButton = m_pRepairButton->SigClicked().Connect(this, &CNPCShop::ClickedRepairButton);

	// Full repair button
	m_pRepairAllButton = (gui::CButton*)GetComponent( "RepairAllButton" );
	m_pRepairAllButton->SetClickSound(GSD_SYSTEM_ITEM_REPAIR);	
	m_slotRepairAllButton = m_pRepairAllButton->SigClicked().Connect(this, &CNPCShop::ClickedRepairAllButton);

	// Item identify button
	m_pIdentifyButton = (gui::CButton*)GetComponent( "IdentifyItemButton" );
	m_pIdentifyButton->SetToolTip(GetDisplayStringManager()->GetString("DST_NPCSHOP_ITEM_IDENTIFY"));
	m_slotIdentifyButton = m_pIdentifyButton->SigClicked().Connect(this, &CNPCShop::ClickedIdentifyButton);	

	// Show current page
	rect.SetRect(217, 409, 263, 425);
	m_pCurrentPage = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pCurrentPage->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pCurrentPage->SetTextColor( RGB(255, 192, 0) );

	// PrePageButton
	m_pPrePageButton = (gui::CButton*)GetComponent( "PrePageButton" );
	m_slotPrePageButton = m_pPrePageButton->SigClicked().Connect(this, &CNPCShop::ClickedPrePageButton);

	// ClickedNextPageButton Button
	m_pNextPageButton = (gui::CButton*)GetComponent( "NextPageButton" );
	m_slotNextPageButton = m_pNextPageButton->SigClicked().Connect(this, &CNPCShop::ClickedNextPageButton);


	// Sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CNPCShop::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CNPCShop::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CNPCShop::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CNPCShop::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CNPCShop::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNPCShop::OnPaint );	
	m_slotPostPaint		= m_pNextPageButton->SigPaint().Connect( this, &CNPCShop::OnPostPaint );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CNPCShop::OnCaptureMouseDown );

	LinkMsg(g_EventShop);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventPickedUpHide);

	ClearShop();
	OnMove(100, 100);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CNPCShop::Destroy()
{
	NTL_FUNCTION( "CNPCShop::Destroy" );

	CheckInfoWindow();	

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
		m_ItemPanel[i].slot.Destroy();

	UnLinkMsg(g_EventShop);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventPickedUpHide);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CNPCShop::ResetCartTexture(eShopType eType)
{
	if( m_eShopType == eType )
		return;

	CRectangle rect = m_pThis->GetScreenRect();
	Destroy();
	m_eShopType = eType;
	Create();
	m_pThis->SetPosition(rect);
}

VOID CNPCShop::OpenShop(SERIAL_HANDLE hNPC, sNPC_TBLDAT* pNPC_TBLDAT)
{
	CTextTable* pNPCTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();	
	CTextTable* pMerchantTextTable = API_GetTableContainer()->GetTextAllTable()->GetMerchantTbl();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	// NPC 번호
	m_hNPCSerial = hNPC;

	// 상점 이름
	m_pShopTitle->SetText( pNPCTextTable->GetText(pNPC_TBLDAT->Name).c_str() );

	// 전체 수리 툴팁
	SetRepairAllTooltip();	

	// 현재 상점에서 팔 수 있는 아이템 최대 4종류의 목록을 읽어온다.
	ShopItem shopItem;
	char acBuffer[256] = "";
	for(RwInt32 iTabIndex = 0 ; iTabIndex < NTL_MAX_MERCHANT_TAB_COUNT ; ++iTabIndex )
	{	
		if( pNPC_TBLDAT->amerchant_Tblidx[iTabIndex] <= 0 )
			continue;

		// 각 Tab 별 등록된 아이템을 읽어온다.
		sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable(pNPC_TBLDAT->amerchant_Tblidx[iTabIndex]);
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

			switch(m_eShopType)
			{
				case SHOP_TYPE_NORMAL:
				{
					m_aShopItem[iTabIndex][iMerchantIndex].uiPrice = pITEM_DATA->dwCost;
					break;
				}
				case SHOP_TYPE_TENKAICHI:
				{
					m_aShopItem[iTabIndex][iMerchantIndex].uiPoint			= pITEM_DATA->CommonPoint;
					m_aShopItem[iTabIndex][iMerchantIndex].pNeedITEM_DATA	= Logic_GetItemDataFromTable(pITEM_DATA->NeedItemTblidx);
					break;
				}
				default:
				{
					DBO_FAIL("Wrong shop type");
					break;
				}
			}
		}
	}		

	switch(m_eShopType)
	{
		case SHOP_TYPE_NORMAL:
		{
			SetZenny();
			m_pPocketMoneytitle->SetText(GetDisplayStringManager()->GetString( "DST_NPCSHOP_MYZENNY" ));
			break;
		}
		case SHOP_TYPE_TENKAICHI:
		{
			SetMudosaPoint();
			m_pPocketMoneytitle->SetText(GetDisplayStringManager()->GetString( "DST_NPCSHOP_MUDOA_POINT" ));
			break;
		}
		default:
		{
			DBO_FAIL("Wrong shop type");
			break;
		}
	}	

	// 최초 탭
	m_pTabButton->SelectTab(0);
	UpdateTabContent(0);

	GetDialogManager()->OpenDialog(DIALOG_NPCSHOP, m_hNPCSerial);
}		

VOID CNPCShop::CloseShop()
{
	if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) || 
		GetDialogManager()->IsMode( DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION ) )
		GetDialogManager()->OffMode();

	GetDialogManager()->CloseDialog(DIALOG_NPCSHOP);

	ClearShop();
}


VOID CNPCShop::ClearShop()
{
	m_hNPCSerial	= INVALID_SERIAL_ID;

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

	SelectEffect(FALSE);
}

VOID CNPCShop::ClearPanels()
{
	m_iMouseDownSlot = -1;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.Clear();
		m_ItemPanel[i].pItemName->Clear();		
		m_ItemPanel[i].pPoint->Clear();
		m_ItemPanel[i].pItemPanel->Show(false);
		m_ItemPanel[i].srfNeedItem.Show(false);
		m_ItemPanel[i].pstbNeedItemCount->Show(false);
		m_ItemPanel[i].bNeedItem = FALSE;

		m_ItemPanel[i].srfNeedItemIcon.Show(false);

		Logic_DeleteTexture(m_ItemPanel[i].srfNeedItemIcon.GetTexture());
		m_ItemPanel[i].srfNeedItemIcon.UnsetTexture();
	}
}

VOID CNPCShop::OnSelectChangeTabButton( INT nCurIndex, INT nPrevIndex )
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
		GetIconMoveManager()->IconMoveEnd();

	UpdateTabContent((RwUInt8)nCurIndex);
}

VOID CNPCShop::UpdateTabContent(RwUInt8 byIndex)
{
	m_iCurTab = byIndex;

	ClearPanels();
	SetPage(dFIRST_PAGE);
	SetPanel(dFIRST_PAGE);
}

VOID CNPCShop::ClickedCloseButton(gui::CComponent* pComponent)
{	
	switch(m_eShopType)
	{
		case SHOP_TYPE_NORMAL:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopClose();
			break;
		}
		case SHOP_TYPE_TENKAICHI:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendEventItemShopEndReq();
			break;
		}
		default:
		{
			DBO_FAIL("Wrong shop type");
			break;
		}
	}
}

VOID CNPCShop::ClickedRepairButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->IsActive() )
		GetIconMoveManager()->IconMoveEnd();

	eDialogMode eMode = GetDialogManager()->GetMode();

	if( eMode == DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION ||
		eMode == DIALOGMODE_UNKNOWN )
	{
		GetDialogManager()->OffMode();
		GetDialogManager()->OnMode(DIALOGMODE_ITEM_REPAIR);	
	}
	else if( eMode == DIALOGMODE_ITEM_REPAIR )
	{
		return;
	}
	else
	{
		Logic_DialogModeMessage(eMode);
	}
}

VOID CNPCShop::ClickedRepairAllButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
		GetIconMoveManager()->IconMoveEnd();

	GetDboGlobal()->GetGamePacketGenerator()->SendItemAllRepair(m_hNPCSerial);
}

VOID CNPCShop::ClickedIdentifyButton(gui::CComponent* pComponent)
{
	if( GetIconMoveManager()->IsActive() )
		GetIconMoveManager()->IconMoveEnd();

	eDialogMode eMode = GetDialogManager()->GetMode();

	if( eMode == DIALOGMODE_ITEM_REPAIR ||
		eMode == DIALOGMODE_UNKNOWN )
	{
		GetDialogManager()->OffMode();
		GetDialogManager()->OnMode(DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION);	
	}
	else if( eMode == DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION )
	{
		return;
	}
	else
	{
		Logic_DialogModeMessage(eMode);
	}
}

RwBool CNPCShop::SetPage(RwInt32 iPage)
{
	m_iCurPage = iPage;

	if(m_iCurPage < 0)
	{
		m_pCurrentPage->Clear();
		return FALSE;
	}
	else
	{
		RwUInt8 byPageCount = GetPageCount_of_CurTab();
		m_pCurrentPage->Format(L"%d / %d", m_iCurPage+1, byPageCount);
		return TRUE;		
	}
}

VOID CNPCShop::SetPanel(RwInt32 iPage)
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

			switch(m_eShopType)
			{
				case SHOP_TYPE_NORMAL:
				{
					m_ItemPanel[i].pPoint->SetText(Logic_FormatZeni(m_aShopItem[m_iCurTab][iIndex].uiPrice));
					break;
				}
				case SHOP_TYPE_TENKAICHI:
				{
					if( m_aShopItem[m_iCurTab][iIndex].pITEM_DATA->NeedItemTblidx != INVALID_SERIAL_ID )
					{
						m_ItemPanel[i].srfNeedItem.Show(true);
						m_ItemPanel[i].pstbNeedItemCount->Show(true);

						m_ItemPanel[i].srfNeedItemIcon.SetTexture( Logic_CreateTexture(m_aShopItem[m_iCurTab][iIndex].pNeedITEM_DATA->szIcon_Name) );
						m_ItemPanel[i].srfNeedItemIcon.Show(true);

						m_ItemPanel[i].bNeedItem = TRUE;
					}

					if( m_aShopItem[m_iCurTab][iIndex].uiPoint != INVALID_MUDOSAPOINT )
						m_ItemPanel[i].pPoint->SetText(Logic_FormatZeni(m_aShopItem[m_iCurTab][iIndex].uiPoint));
					break;
				}
				default:
				{
					DBO_FAIL("Wrong shop type");
					break;
				}
			}
		}		
	}

	SelectEffect(FALSE);
	SetPageButton();
}

VOID CNPCShop::SetPageButton()
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

VOID CNPCShop::ClickedPrePageButton(gui::CComponent* pComponent)
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

VOID CNPCShop::ClickedNextPageButton(gui::CComponent* pComponent)
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

VOID CNPCShop::SelectItem(RwInt32 iPanelIndex)
{	
	RwInt32 iItemIndex = (m_iCurPage*dMAX_ITEM_PANEL) + iPanelIndex;
	CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_NPCSHOP,
		m_aShopItem[m_iCurTab][iItemIndex].hItem,
		m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
		(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
		m_iCurTab, iItemIndex, 1);
}

VOID CNPCShop::SetZenny()
{
	m_pPocketMoney->SetText(Logic_FormatZeni(Logic_GetZenny()));
}

VOID CNPCShop::SetMudosaPoint()
{
	m_pPocketMoney->SetText(Logic_FormatZeni(Logic_GetMudosaPoint()));
}

VOID CNPCShop::SetRepairAllTooltip()
{
	WCHAR		awcBuffer[512]	= L"";
	RwUInt32	uiRepairCost	= Logic_AllRepairCost();

	if( 0 < uiRepairCost )
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_NPCSHOP_ITEM_ALL_REPAIR");
		swprintf_s(awcBuffer, 512, L"%s : %sZ", pwcText, Logic_FormatZeni(uiRepairCost));
		m_pRepairAllButton->SetToolTip(awcBuffer);
	}
	else
	{
		m_pRepairAllButton->SetToolTip(GetDisplayStringManager()->GetString("DST_NPCSHOP_NO_MORE_REPAIR_REG_ITEM"));
	}
}

RwUInt8 CNPCShop::GetPageCount_of_CurTab()
{
	RwUInt8 byLastIndex = 0;

	// Item slots may be empty in the middle
	for( RwUInt8 i = 0 ; i < NTL_MAX_MERCHANT_COUNT ; ++i )
	{
		if( m_aShopItem[m_iCurTab][i].hItem != INVALID_SERIAL_ID )
			byLastIndex = i;
	}

	byLastIndex /= dMAX_ITEM_PANEL;

	return byLastIndex + 1;
}

SERIAL_HANDLE CNPCShop::GetNPCSerial()
{
	return m_hNPCSerial;
}

CNPCShop::eShopType CNPCShop::GetShopType()
{
	return m_eShopType;
}

RwBool CNPCShop::IsFirstPage()
{
	if( dFIRST_PAGE == m_iCurPage )
		return TRUE;

	return FALSE;
}

RwBool CNPCShop::IsLastPage()
{
	RwInt32 iIndex;
	RwInt32 iNextPage = m_iCurPage + 1;

	if( m_iCurPage >= dMAX_PAGE -1 )
		return TRUE;

	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		iIndex = (iNextPage * dMAX_ITEM_PANEL ) + i;
		if( m_aShopItem[m_iCurTab][iIndex].hItem != INVALID_SERIAL_ID )
			return FALSE;
	}

	return TRUE;
}

RwInt8 CNPCShop::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt8 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if( m_ItemPanel[i].slot.PtInRect(iX, iY) )
			return i ;
	}

	return INVALID_BYTE;
}

RwInt8 CNPCShop::PtinSlot_of_SmallIcon_in_panel(RwInt32 iX, RwInt32 iY)
{
	if( m_eShopType != SHOP_TYPE_TENKAICHI )
		return INVALID_BYTE;

	for( RwInt8 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		if(m_ItemPanel[i].srfNeedItemIcon.PtInRect(iX, iY) )
			return i;
	}

	return INVALID_BYTE;
}

VOID CNPCShop::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		RwInt32 iY = dICON_START_Y + iSlotIdx*dSLOT_GAP_HORI;
		CRectangle rect = m_pThis->GetScreenRect();

		m_FocusEffect.SetRectWH(rect.left + dICON_START_X, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
	{
		m_bFocus = FALSE;
	}
}

VOID CNPCShop::SelectEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if( bPush)
	{
		RwInt32 iY = dICON_START_Y + dSLOT_GAP_HORI*iSlotIdx;
		CRectangle rect = m_pThis->GetScreenRect();

		m_SelectEffect.SetRectWH(rect.left + dICON_START_X, rect.top + iY, 32, 32);
		m_iSelectedSlot = iSlotIdx;
	}
	else
		m_iSelectedSlot = INVALID_INDEX;
}

VOID CNPCShop::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_NPCSHOP )
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CNPCShop::OnMouseDown( const CKey& key )
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

VOID CNPCShop::OnMouseUp( const CKey& key )
{
	m_pThis->ReleaseMouse();

	// 클릭 이벤트 종료	
	if( m_iClickEffectedSlot != INVALID_INDEX )
	{		
		m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		m_iClickEffectedSlot = INVALID_INDEX;
	}

	if( !IsShow() )
	{
		m_iMouseDownSlot = INVALID_INDEX;
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

			SelectEffect(TRUE, m_iMouseDownSlot);
		}
		else if( key.m_nID == UD_RIGHT_BUTTON )
		{
			RwInt32 iItemIndex = (m_iCurPage*dMAX_ITEM_PANEL) + m_iMouseDownSlot;

			if( key.m_dwVKey == UD_MK_CONTROL )
			{
				// 최대 스택 갯수를 산다(1개 혹은 20개)
				CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_NPCSHOP_LARGE_BUY,
												m_aShopItem[m_iCurTab][iItemIndex].hItem,
												m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
												(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
												m_iCurTab, iItemIndex, m_aShopItem[m_iCurTab][iItemIndex].pITEM_DATA->byMax_Stack);
			}
			else
			{				
				CDboEventGenerator::ShopEvent(TRM_REG_ITEM_BY_NPCSHOP,
												m_aShopItem[m_iCurTab][iItemIndex].hItem,
												m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
												(wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
												m_iCurTab, iItemIndex, 1);
			}
		}
	}

	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CNPCShop::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_MoneyBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);	
	m_PageBackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);

	// NPCShop 다이얼로그가 움직일 때	
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);
		
		m_ItemPanel[i].srfNeedItemIcon.SetPositionbyParent(rtScreen.left, rtScreen.top);

		m_ItemPanel[i].srfNeedItem.SetPositionbyParent(rtScreen.left, rtScreen.top);
	}

	m_BackLineSurface.SetPositionbyParent(rtScreen.left, rtScreen.top);

	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);

	CheckInfoWindow();
}

VOID CNPCShop::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt8 iPtinSlot = PtinSlot(nX, nY);

	if( iPtinSlot != INVALID_INDEX )
	{
		// 슬롯 클릭 이펙트
		if( m_iClickEffectedSlot != INVALID_INDEX )
		{
			if( m_iClickEffectedSlot == iPtinSlot )
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(true);
			else
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(false);
		}

		// Information on current item panel
		if( m_ItemPanel[iPtinSlot].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			FocusEffect(TRUE, iPtinSlot);

			if( m_byInfoWindowIndex != iPtinSlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
													rtScreen.left + m_ItemPanel[iPtinSlot].slot.GetX_fromParent(),
													rtScreen.top + m_ItemPanel[iPtinSlot].slot.GetY_fromParent(),
													m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iPtinSlot].pITEM_DATA, DIALOG_NPCSHOP );
				m_byInfoWindowIndex = iPtinSlot;
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
		RwInt8 iSmallIconSlot = PtinSlot_of_SmallIcon_in_panel(nX, nY);
		if( iSmallIconSlot != INVALID_INDEX )
		{
			// 아이템을 사기 위해서 가지고 있어야 하는 아이템 정보
			if( m_ItemPanel[iSmallIconSlot].slot.GetSerial() != INVALID_SERIAL_ID )
			{
				FocusEffect(TRUE, iSmallIconSlot);

				if( m_byInfoWindowIndex != iSmallIconSlot )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();

					if( m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pNeedITEM_DATA )
					{
						GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_ITEM_NAME,
											rtScreen.left + m_ItemPanel[iSmallIconSlot].slot.GetX_fromParent(),
											rtScreen.top + m_ItemPanel[iSmallIconSlot].slot.GetY_fromParent(),
											m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pNeedITEM_DATA, DIALOG_NPCSHOP );
					}
					else
					{
						WCHAR awcBuffer[128] = L"";
						swprintf_s(awcBuffer, 128, L"Not exist item table of index : %u", m_aShopItem[m_iCurTab][m_iCurPage*dMAX_ITEM_PANEL + iSmallIconSlot].pITEM_DATA->NeedItemTblidx);
						GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
											rtScreen.left + m_ItemPanel[iSmallIconSlot].slot.GetX_fromParent(),
											rtScreen.top + m_ItemPanel[iSmallIconSlot].slot.GetY_fromParent(),
											(VOID*)awcBuffer, DIALOG_NPCSHOP );
					}
					
					m_byInfoWindowIndex = iSmallIconSlot + dSMALL_ICON_INFO_SLOT_INDEX_GAP;
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
				m_ItemPanel[m_iClickEffectedSlot].slot.ClickEffect(FALSE);

			m_byInfoWindowIndex = INVALID_BYTE;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}
	}	
}

VOID CNPCShop::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CNPCShop::OnPaint()
{
	m_BackLineSurface.Render();
	m_MoneyBackPanel.Render();
	m_PageBackPanel.Render();	
}

VOID CNPCShop::OnPostPaint()
{
	for( RwInt32 i = 0 ; i < dMAX_ITEM_PANEL ; ++i )
	{
		m_ItemPanel[i].slot.Paint();

		if (m_ItemPanel[i].bNeedItem)
		{
			m_ItemPanel[i].srfNeedItem.Render();
			m_ItemPanel[i].srfNeedItemIcon.Render();
		}
	}	

	if( m_iSelectedSlot != INVALID_INDEX )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

VOID CNPCShop::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_NPCSHOP, key.m_fX, key.m_fY);
}

RwInt32 CNPCShop::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
        // NPC 애니메이션
        Logic_SetActorAnimation(m_hNPCSerial, NML_IDLE_LOOP, TRUE);

		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
		if( pWorldConcept && pWorldConcept->GetConceptType() == WORLD_PLAY_NPC_COMMU )
		{
			if( pWorldConcept->GetState() == WORLD_NPC_SHOP )
				ClickedCloseButton(NULL);
		}
		else
		{
			Show(false);

			if( GetIconMoveManager()->GetSrcPlace() == PLACE_NPCSHOP )
				GetIconMoveManager()->IconMoveEnd();

			CheckInfoWindow();			

			Logic_CancelNpcFacing();
		}
	}

	return 1;
}

VOID CNPCShop::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CNPCShop::HandleEvents" );

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

					GetAlarmManager()->AlarmMessage("DST_TARGET_NONE");

					CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
					if( pWorldConcept && pWorldConcept->GetConceptType() == WORLD_PLAY_NPC_COMMU )
					{
						ClickedCloseButton(NULL);
					}

					Logic_CancelNpcFacing();
					NTL_RETURNVOID();
				}

				// Textures vary depending on the type of store
				if( pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT2 || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT3)
				{
					if( m_eShopType == SHOP_TYPE_NORMAL )
						ResetCartTexture(SHOP_TYPE_TENKAICHI);
				}
				else
				{
					if( m_eShopType == SHOP_TYPE_TENKAICHI )
						ResetCartTexture(SHOP_TYPE_NORMAL);
				}

				OpenShop(pPacket->uiSerial, pNPC_TBLDAT);

				// Move the trade cart to the side of the store
				CRectangle rect = GetPosition();
				CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_SHOPING_CART);
				pPLGui->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);
			}
			break;
			case TRM_CLOSE:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
				NTL_RETURNVOID();

				GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				CloseShop();
			}
			break;
			case TRM_BUY_SUCCESS:
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				// NPC animation (by agebreak)
				Logic_SetActorAnimation(m_hNPCSerial, SOC_AGREE, FALSE);
			}
			break;
			case TRM_SELL_SUCCESS:    
			{
				if (m_hNPCSerial == INVALID_SERIAL_ID)
					NTL_RETURNVOID();

				Logic_SetActorAnimation(m_hNPCSerial, SOC_AGREE, FALSE);
			}
			break;

			default: break;
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if (m_hNPCSerial == INVALID_SERIAL_ID)
			NTL_RETURNVOID();

		switch( pEvent->iDestDialog )
		{
			case DIALOG_NPCSHOP:
			{
				switch( pEvent->iType )
				{
					case DIALOGEVENT_NPC_BYEBYE:
					{
						ClickedCloseButton(NULL);
					}
					break;
					case DIALOGEVENT_OPEN_FAIL_NPC_DIALOG:
					{
						Logic_CancelNpcFacing();

						ClearShop();
					}		
					break;
					case DIALOGEVENT_REPAIR:
					case DIALOGEVENT_REPAIR_ALL:
					{
						SetRepairAllTooltip();
						break;
					}
				}
			}
			break;

			default: break;
		}
	}
	else if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );
		
		if( pData->eDialog != DIALOG_NPCSHOP )
			return;

		// Same NPC
		if( m_hNPCSerial == pData->hSerialId )
			return;

		// If you are having a conversation with another NPC
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			ClickedCloseButton(NULL);
			return;
		}

		sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(pData->hSerialId);
		if( !pNPC_TBLDAT )
		{
			DBO_FAIL("Not exist npc table of sob object of handle of : " << m_hNPCSerial);
			return;
		}

		RwBool bPacketLock = FALSE;

		if( pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT2 || pNPC_TBLDAT->byJob == NPC_JOB_BUDOHSI_MERCHANT3)
		{
			bool bSendSuccess = GetDboGlobal()->GetGamePacketGenerator()->SendEventItemShopStartReq(pData->hSerialId, &bPacketLock);

			if( bPacketLock && !bSendSuccess )
			{
				Logic_CancelNpcFacing();
			}
		}
		else
		{
			bool bSendSuccess = GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopStartReq(pData->hSerialId, NPC_SHOP_TYPE_DEFAULT, &bPacketLock);

			if( bPacketLock && !bSendSuccess )
			{
				Logic_CancelNpcFacing();
			}
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( INVALID_SERIAL_ID == m_hNPCSerial )
			NTL_RETURNVOID();		

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();


		if(m_eShopType == SHOP_TYPE_NORMAL && pEvent->uiUpdateType & EVENT_AIUT_ZENNY)
		{
			SetZenny();
		}
		else if(m_eShopType == SHOP_TYPE_TENKAICHI && pEvent->uiUpdateType & EVENT_AIUT_POINT_MUDOSA )
		{
			SetMudosaPoint();
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC server is down or suddenly NPC disappears
			ClickedCloseButton(NULL);
		}
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_NPCSHOP )
			return;

		SelectEffect(FALSE);
	}    

	NTL_RETURNVOID();
}