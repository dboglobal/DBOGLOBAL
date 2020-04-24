#include "precomp_dboclient.h"
#include "DojoUpgradeGui.h"


// shared
#include "NPCTable.h"
#include "ItemTable.h"
#include "DojoTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlCameraController.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGuild.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// client
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "MsgBoxManager.h"

#define dSTUFF_SLOT					(0)

#define dDOJO_SURFACE_LEFT_X		(13)
#define dDOJO_SURFACE_RIGHT_X		(182)
#define dDOJO_SURFACE_Y				(74)


CDojoUpradeGui::CDojoUpradeGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byLeftMouseDown(INVALID_BYTE)
,m_byRightMouseDown(INVALID_BYTE)
,m_bSlotFocus(FALSE)
,m_bSlotDest(FALSE)
,m_bExpectServerAnswer(TRUE)
,m_hNPCSerial(INVALID_SERIAL_ID)
,m_uiDojoTableIndex(INVALID_TBLIDX)
,m_eInfoWindowType(INVALID_INFO_WINDOW_TYPE)
{

}

CDojoUpradeGui::~CDojoUpradeGui()
{
	Destroy();
}

RwBool CDojoUpradeGui::Create()
{
	NTL_FUNCTION( "CDojoUpradeGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\DojoUpgrade.srf", "gui\\DojoUpgrade.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pBtnHelp		= (gui::CButton*)GetComponent( "btnHelp" );
	m_pBtnExit		= (gui::CButton*)GetComponent( "btnExit" );
	m_pBtnUpgrade	= (gui::CButton*)GetComponent( "btnUpgrade" );

	m_pDialogName			= (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pDojoNameStatic		= (gui::CStaticBox*)GetComponent( "stbDojoNameStatic" );
	m_pDojoName				= (gui::CStaticBox*)GetComponent( "stbDojoName" );
	m_pDojoCurLevel			= (gui::CStaticBox*)GetComponent( "stbDojoCurLevel" );
	m_pDojoNextLevel		= (gui::CStaticBox*)GetComponent( "stbDojoNextLevel" );
	m_pNeedGuildPointStatic	= (gui::CStaticBox*)GetComponent( "stbNeedGuildPointStatic" );
	m_pNeedGuildPoint		= (gui::CStaticBox*)GetComponent( "stbNeedGuildPoint" );
	m_pConsumptionGuildPointStatic	= (gui::CStaticBox*)GetComponent( "stbConsumptionGuildPointStatic" );
	m_pConsumptionGuildPoint= (gui::CStaticBox*)GetComponent( "stbConsumptionGuildPoint" );
	m_pNeedZennyStatic		= (gui::CStaticBox*)GetComponent( "stbNeedZennyStatic" );	
	m_pNeedZenny			= (gui::CStaticBox*)GetComponent( "stbNeedZennyPoint" );
	m_pConsumptionZennyStatic=(gui::CStaticBox*)GetComponent( "stbConsumptionZennyStatic" );
	m_pConsumptionZenny		= (gui::CStaticBox*)GetComponent( "stbNeedConsumptionZenny" );
	m_pNeedItemStatic		= (gui::CStaticBox*)GetComponent( "stbNeedItemStatic" );
	m_pNeedItem				= (gui::CStaticBox*)GetComponent( "stbNeedItem" );


	m_srfDojo[DOJO_VISUAL_LEVEL_1].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoLevel1" ) );
	m_srfDojo[DOJO_VISUAL_LEVEL_2].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoLevel2" ) );
	m_srfDojo[DOJO_VISUAL_LEVEL_3].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoLevel3" ) );
	m_srfDojo[DOJO_VISUAL_LEVEL_4].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoLevel4" ) );

	m_srfNextArrowBar	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNextArrow" ) );
	m_srfNextArrowBar	.SetPositionfromParent(147, 103);

	m_srfNeedGuildPoint	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuildPoint" ) );
	m_srfNeedGuildPoint	.SetPositionfromParent(289, 185);

	m_srfMyGuildPoint	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuildPoint" ) );
	m_srfMyGuildPoint	.SetPositionfromParent(289, 209);

	m_srfNeedZenny		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfZennyMark" ) );
	m_srfNeedZenny		.SetPositionfromParent(289, 241);

	m_srfCumptionZenny	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfZennyMark" ) );
	m_srfCumptionZenny	.SetPositionfromParent(289, 265);

	m_srfSlot			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlot" ) );
	m_srfSlot			.SetPositionfromParent(266, 299);

	m_srfSlotFocus		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_srfSlotFocus		.SetPositionfromParent(266, 299);


	m_DojoNameBackground.SetType(CWindowby3::WT_VERTICAL);
	m_DojoNameBackground.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoNameLeft" ) );
	m_DojoNameBackground.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoNameCenter" ) );
	m_DojoNameBackground.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfDojoNameRight" ) );
	m_DojoNameBackground.SetSize(298, 21);
	m_DojoNameBackground.SetPositionfromParent(11, 38);

	m_NeedGuildPointBackground.SetType(CWindowby3::WT_VERTICAL);
	m_NeedGuildPointBackground.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointLeft" ) );
	m_NeedGuildPointBackground.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointCenter" ) );
	m_NeedGuildPointBackground.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointRight" ) );
	m_NeedGuildPointBackground.SetSize(298, 50);
	m_NeedGuildPointBackground.SetPositionfromParent(11, 180);

	m_NeedZennyBackground.SetType(CWindowby3::WT_VERTICAL);
	m_NeedZennyBackground.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointLeft" ) );
	m_NeedZennyBackground.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointCenter" ) );
	m_NeedZennyBackground.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointRight" ) );
	m_NeedZennyBackground.SetSize(298, 50);
	m_NeedZennyBackground.SetPositionfromParent(11, 236);

	m_NeedItemBackground.SetType(CWindowby3::WT_VERTICAL);
	m_NeedItemBackground.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointLeft" ) );
	m_NeedItemBackground.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointCenter" ) );
	m_NeedItemBackground.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoUpgrade.srf", "srfNeedGuildPointRight" ) );
	m_NeedItemBackground.SetSize(298, 50);
	m_NeedItemBackground.SetPositionfromParent(11, 292);

	// 업그레이드 재료 슬롯
	m_StuffSlot.Create(m_pThis, DIALOG_DOGI, REGULAR_SLOT_ITEM_SOB);
	m_StuffSlot.SetPosition_fromParent(268, 301);


	// 스트링
	m_pDialogName					->SetText( GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE") );
	m_pDojoNameStatic				->SetText( GetDisplayStringManager()->GetString("DST_DOJO_NAME") );	
	m_pNeedGuildPointStatic			->SetText( GetDisplayStringManager()->GetString("DST_DOJO_NEED_GUILD_POINT") );
	m_pConsumptionGuildPointStatic	->SetText( GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE_CONSUMPTION_GUILD_POINT") );
	m_pNeedZennyStatic				->SetText( GetDisplayStringManager()->GetString("DST_DOJO_NEED_ZENNY") );
	m_pConsumptionZennyStatic		->SetText( GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE_CONSUMPTION_ZENNY") );
	m_pNeedItemStatic				->SetText( GetDisplayStringManager()->GetString("DST_DOJO_NEED_ITEM") );
		
	m_pBtnUpgrade					->SetText( GetDisplayStringManager()->GetString("DST_DOJO_RUN_UPGRADE") );


	// default value
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);	

	// 기본 위치 지정
	OnMove(0, 0);


	// sig
	m_slotHelpButton	= m_pBtnHelp->SigClicked().Connect(this, &CDojoUpradeGui::OnClicked_HelpButton);
	m_slotExitButton	= m_pBtnExit->SigClicked().Connect(this, &CDojoUpradeGui::OnClicked_ExitButton);	
	m_slotUpgradeButton	= m_pBtnUpgrade->SigClicked().Connect(this, &CDojoUpradeGui::OnClicked_UpgradeButton);
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CDojoUpradeGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CDojoUpradeGui::OnMouseUp );	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CDojoUpradeGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CDojoUpradeGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CDojoUpradeGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CDojoUpradeGui::OnPaint );
	m_slotCaptureMouseDown		= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CDojoUpradeGui::OnCaptureMouseDown );
	

	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventIconMoveClick);
	LinkMsg(g_EventNotifyGuild);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventDirectMoveIcon);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventEnableItemIcon);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDojoUpradeGui::Destroy()
{
	NTL_FUNCTION("CDojoUpradeGui::Destroy");

	m_StuffSlot.Destroy();

	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventDirectMoveIcon);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventEnableItemIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDojoUpradeGui::Clear()
{
	m_pDojoName					->Clear();
	m_pDojoCurLevel				->Clear();
	m_pDojoNextLevel			->Clear();
	m_pNeedGuildPoint			->Clear();
	m_pConsumptionGuildPoint	->Clear();
	m_pNeedZenny				->Clear();
	m_pConsumptionZenny			->Clear();
	m_pNeedItem					->Clear();

	CheckInfoWindow();
	UnsetHoipoiRock();

	m_eDojoVisualLevel		= DOJO_VISUAL_LEVEL_1;
	m_hNPCSerial			= INVALID_SERIAL_ID;
	m_byLeftMouseDown		= INVALID_BYTE;
	m_byRightMouseDown		= INVALID_BYTE;
	m_bExpectServerAnswer	= FALSE;
	m_bSlotFocus			= FALSE;
	m_bSlotDest				= FALSE;
	m_uiDojoTableIndex		= INVALID_TBLIDX;
}

RwBool CDojoUpradeGui::SetHoipoiRock(SERIAL_HANDLE hItem, RwUInt32 uiCount)
{
	if( m_bExpectServerAnswer )
		return FALSE;

	UnsetHoipoiRock();

	if( FALSE == m_StuffSlot.SetIcon(hItem, uiCount) )
	{
		DBO_FAIL("Can not register HoipoiRock item");
		return FALSE;
	}

	CNtlSobItem* pSobItem = m_StuffSlot.GetSobItem();
	if( !pSobItem )
	{
		DBO_FAIL("Invalid item pointer");
		UnsetHoipoiRock();
		return FALSE;
	}

	RwInt32 iParentSlotIndex	= pSobItem->GetParentItemSlotIdx();
	RwInt32 iSlotIndex			= pSobItem->GetItemSlotIdx();

	CDboEventGenerator::EnableItemIcon(FALSE, PLACE_BAG, iSlotIndex, iParentSlotIndex);

	return TRUE;
}

VOID CDojoUpradeGui::UnsetHoipoiRock()
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(m_StuffSlot.GetSerial()) );
	if( !pSobItem )
		return;

	RwInt32 iParentSlotIndex	= pSobItem->GetParentItemSlotIdx();
	RwInt32 iSlotIndex			= pSobItem->GetItemSlotIdx();

	if( iParentSlotIndex != INVALID_BYTE && iSlotIndex != INVALID_BYTE )
		CDboEventGenerator::EnableItemIcon(TRUE, PLACE_BAG, iSlotIndex, iParentSlotIndex);


	m_StuffSlot.Clear();
}

VOID CDojoUpradeGui::SetDojoName(WCHAR* pwcGuildName)
{
	if( pwcGuildName && wcslen(pwcGuildName) > 0 )
	{
		std::wstring wstrDojoName = pwcGuildName;
		wstrDojoName += L" ";
		wstrDojoName += GetDisplayStringManager()->GetString("DST_DOJO");
		m_pDojoName->SetText( wstrDojoName.c_str() );
	}
	else
	{
		sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(m_uiDojoTableIndex) );
		if(!pDOJO_TBLDAT)
		{
			DBO_FAIL("Not exist dojo table of index : " << m_uiDojoTableIndex);
			return;
		}

		CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetTextTbl(CTextAllTable::MAP_NAME);
		std::wstring wstrRegionName = pTextTable->GetText(pDOJO_TBLDAT->mapName);

		wstrRegionName += L" ";
		wstrRegionName += GetDisplayStringManager()->GetString("DST_DOJO");
		m_pDojoName->SetText( wstrRegionName.c_str() );
	}
}

RwUInt32 CDojoUpradeGui::GetNeedGuildPoint(sDOJO_INFO* pDOJO_INFO)
{	
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return 0;
	}

	RwUInt8 byGuildFunction = ConvertDojoLeveltoGuildFunction(pDOJO_INFO->byLevel);

	if( DBO_GUILD_FUNCTION_INVALID == byGuildFunction )
	{
		DBO_FAIL("Invalid guild function");
		return 0;
	}

	return g_aGuildFunctionInfo[byGuildFunction].dwRequiredPoint;
}

RwUInt32 CDojoUpradeGui::GetNeedZennyPoint(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return 0;
	}

	RwUInt8 byGuildFunction = ConvertDojoLeveltoGuildFunction(pDOJO_INFO->byLevel);

	if( DBO_GUILD_FUNCTION_INVALID == byGuildFunction )
	{
		DBO_FAIL("Invalid guild function");
		return 0;
	}

	return g_aGuildFunctionInfo[byGuildFunction].dwRequiredZenny;
}

VOID CDojoUpradeGui::SetNeedGuildPoint(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}

	m_pNeedGuildPoint->SetText( GetNeedGuildPoint(pDOJO_INFO) );
}

VOID CDojoUpradeGui::SetNeedZennyPoint(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}

	m_pNeedZenny->SetText(Logic_FormatZeni(GetNeedZennyPoint(pDOJO_INFO)) );
}

VOID CDojoUpradeGui::SetConsumptionGuildPoint(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	RwUInt32 uiNeedGuildPoint			= GetNeedGuildPoint(pDOJO_INFO);
	RwUInt32 uiConsumptionGuildPoint	= pGuild->GetReputation();

	if( uiConsumptionGuildPoint >= uiNeedGuildPoint )
	{
		m_pConsumptionGuildPoint->SetTextColor(INFOCOLOR_1);
		m_pConsumptionGuildPoint->SetText(uiConsumptionGuildPoint);
	}
	else
	{
		m_pConsumptionGuildPoint->SetTextColor(INFOCOLOR_5);
		m_pConsumptionGuildPoint->SetText(uiConsumptionGuildPoint);
	}
}

VOID CDojoUpradeGui::SetConsumptionZennyPoint(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}

	RwUInt32 uiNeedZenny	= GetNeedZennyPoint(pDOJO_INFO);
	RwUInt32 uiAvatarZenny	= Logic_GetZenny();

	if( uiAvatarZenny >= uiNeedZenny )
	{
		m_pConsumptionZenny->SetTextColor(INFOCOLOR_1);
		m_pConsumptionZenny->SetText(Logic_FormatZeni(uiAvatarZenny));
	}
	else
	{
		m_pConsumptionZenny->SetTextColor(INFOCOLOR_5);
		m_pConsumptionZenny->SetText(Logic_FormatZeni(uiAvatarZenny));
	}
}

VOID CDojoUpradeGui::SetNeedItem(sDOJO_INFO* pDOJO_INFO)
{
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}

	if( DBO_MAX_LEVEL_DOJO_IN_WORLD <= pDOJO_INFO->byLevel )
	{
		m_pNeedItem->Clear();
		return;
	}

	RwUInt8 byGuildFunction = ConvertDojoLeveltoGuildFunction(pDOJO_INFO->byLevel);
	if( DBO_GUILD_FUNCTION_INVALID == byGuildFunction )
	{
		m_pNeedItem->Clear();
		return;
	}

	//if( g_aGuildFunctionInfo[byGuildFunction].byRequiredRock == 0 ) // disabled by daneos.. 
	{
		m_pNeedItem->Clear();
		return;
	}

	m_pNeedItem->Format(L"%s X %d", GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE_NEED_ITEM"),
									g_aGuildFunctionInfo[byGuildFunction].byRequiredRock);
}

VOID CDojoUpradeGui::SetDojoLevel(sDOJO_INFO* pDOJO_INFO)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info pointer");
		return;
	}


	switch( pDOJO_INFO->byLevel )
	{
		case 1:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_1;

			m_srfDojo[DOJO_VISUAL_LEVEL_1].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionfromParent(dDOJO_SURFACE_RIGHT_X,dDOJO_SURFACE_Y);

			m_srfDojo[DOJO_VISUAL_LEVEL_1].SetPositionbyParent(rtScreen.left, rtScreen.top);
			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		// by daneos
		/*case 2:
		case 3:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_2;

			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionfromParent(dDOJO_SURFACE_RIGHT_X,dDOJO_SURFACE_Y);

			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionbyParent(rtScreen.left, rtScreen.top);
			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}*/


		case 2:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_2;

			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionfromParent(dDOJO_SURFACE_RIGHT_X, dDOJO_SURFACE_Y);

			m_srfDojo[DOJO_VISUAL_LEVEL_2].SetPositionbyParent(rtScreen.left, rtScreen.top);
			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}

		case 3:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_3;

			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionfromParent(dDOJO_SURFACE_RIGHT_X, dDOJO_SURFACE_Y);

			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionbyParent(rtScreen.left, rtScreen.top);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}

		case 4:
		case 5:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_3;

			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionfromParent(dDOJO_SURFACE_RIGHT_X,dDOJO_SURFACE_Y);

			m_srfDojo[DOJO_VISUAL_LEVEL_3].SetPositionbyParent(rtScreen.left, rtScreen.top);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		case 6:
		{
			m_eDojoVisualLevel = DOJO_VISUAL_LEVEL_4;

			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionfromParent(dDOJO_SURFACE_LEFT_X, dDOJO_SURFACE_Y);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].SetPositionbyParent(rtScreen.left, rtScreen.top);
			break;
		}
		default:
		{
			DBO_FAIL("Invalid dojo visual level");
			return;
		}
	}

	m_pDojoCurLevel->Format("Lv %d", pDOJO_INFO->byLevel);

	if( pDOJO_INFO->byLevel < DBO_MAX_LEVEL_DOJO_IN_WORLD )
		m_pDojoNextLevel->Format(L"Lv %d", pDOJO_INFO->byLevel + 1);
	else
		m_pDojoNextLevel->Clear();
}

RwUInt8 CDojoUpradeGui::ConvertDojoLeveltoGuildFunction(RwUInt8 byDojoLevel)
{
	switch(byDojoLevel)
	{
	//	case 0:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_1;
		case 1:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_2;
		case 2:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_3;
		case 3:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_4;
		case 4:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_5;
		case 5:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_6;
		case 6:				return DBO_GUILD_FUNCTION_DOJO_UPGRADE_7;
	}

	return DBO_GUILD_FUNCTION_INVALID;
}

VOID CDojoUpradeGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui()== DIALOG_DOJO_UPGRADE )
	{
		m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

RwInt8 CDojoUpradeGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	if( m_StuffSlot.PtInRect(iX, iY) )
		return dSTUFF_SLOT;

	return INVALID_BYTE;
}

VOID CDojoUpradeGui::OnClicked_HelpButton( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent(DIALOG_DOJO_UPGRADE);
}

VOID CDojoUpradeGui::OnClicked_ExitButton( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog(DIALOG_DOJO_UPGRADE);
}

VOID CDojoUpradeGui::OnClicked_UpgradeButton( gui::CComponent* pComponent )
{
	sMsgBoxData msgExData;
	sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(m_hNPCSerial);
	if( !pDOJO_INFO )
	{
		DBO_FAIL("Not exist dojo info of NPC handle : " << m_hNPCSerial);
		return;
	}

	if( DBO_MAX_LEVEL_DOJO_IN_WORLD <= pDOJO_INFO->byLevel )
	{
		GetAlarmManager()->AlarmMessage("DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_MAX_LEVEL");
		return;
	}

	RwUInt8 byGuildFunction = ConvertDojoLeveltoGuildFunction(pDOJO_INFO->byLevel);
	if( DBO_GUILD_FUNCTION_INVALID == byGuildFunction )
	{
		DBO_FAIL("Invalid guild function");
		return;
	}	

	// disabled by daneos
	/*if( g_aGuildFunctionInfo[byGuildFunction].byRequiredRock > 0 )
	{
		CNtlSobItem* pSobItem = m_StuffSlot.GetSobItem();
		if( !pSobItem )
		{
			GetAlarmManager()->AlarmMessage("DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_NEED_ITEM");
			return;
		}

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		if( pSobItemAttr->GetStackNum() < g_aGuildFunctionInfo[byGuildFunction].byRequiredRock )
		{
			GetAlarmManager()->AlarmMessage("DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_NEED_MORE_ITEM");
			return;
		}

		RwInt32 iParentSlotIndex	= pSobItem->GetParentItemSlotIdx();
		RwInt32 iSlotIndex			= pSobItem->GetItemSlotIdx();

		msgExData.ucData[0] = byGuildFunction;
		msgExData.ucData[1]	= (RwUInt8)Logic_ConvertBagIdxToContainerType((RwUInt8)iParentSlotIndex);
		msgExData.ucData[2]	= (RwUInt8)iSlotIndex;

		GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI", FALSE, &msgExData,
			Logic_FormatZeni(g_aGuildFunctionInfo[byGuildFunction].dwRequiredZenny),
												g_aGuildFunctionInfo[byGuildFunction].byRequiredRock);

		if( GetMsgBoxManager()->GetMsgBox("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI") )
			m_bExpectServerAnswer = TRUE;
	}
	else*/
	{
		msgExData.ucData[0] = byGuildFunction;

		GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY", FALSE, &msgExData,
			Logic_FormatZeni(g_aGuildFunctionInfo[byGuildFunction].dwRequiredZenny));

		if( GetMsgBoxManager()->GetMsgBox("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY") )
			m_bExpectServerAnswer = TRUE;
	}
}

VOID CDojoUpradeGui::OnMouseDown(const CKey& key)
{
	RwInt8 iSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( iSlot == INVALID_BYTE )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		m_byLeftMouseDown = iSlot;
		m_pThis->CaptureMouse();			
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		m_byRightMouseDown = iSlot;
		m_pThis->CaptureMouse();		
	}
}

VOID CDojoUpradeGui::OnMouseUp(const CKey& key)
{		
	m_pThis->ReleaseMouse();

	RwInt8 iSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( iSlot == INVALID_BYTE )
	{
		m_byLeftMouseDown	= INVALID_BYTE;
		m_byRightMouseDown	= INVALID_BYTE;
		return;
	}


	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_byLeftMouseDown == iSlot )
		{
			if( dSTUFF_SLOT == m_byLeftMouseDown )
			{
				if( GetIconMoveManager()->IsActive() )
				{
					if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
					{
						SERIAL_HANDLE hItem = GetIconMoveManager()->GetSrcSerial();
						CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hItem));
						if(pSobItem)
						{
							CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

							// 미확인 아이템이 아닌 것만 등록
							if( FALSE == pSobItemAttr->IsNeedToIdentify() )
							{
								sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
								if( pITEM_TBLDAT )
								{
									if( ITEM_TYPE_HOIPOIROCK == pITEM_TBLDAT->byItem_Type )
									{
										// 가방에서 도장 업그레이드 재료 아이템을 등록했다
										SetHoipoiRock(hItem, pSobItemAttr->GetStackNum());
										GetIconMoveManager()->IconMoveEnd();
									}
									else
									{
										GetAlarmManager()->AlarmMessage("DST_DOJO_ONLY_REGISTER_ITEM");
									}
								}
								else
								{
									DBO_FAIL("Not exist ITEM TABLE of handle : " << hItem);
								}
							}
						}
						else
						{
							DBO_FAIL("Not exist sob item of handle : " << hItem);
						}
					}
				}
			}
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( FALSE == GetIconMoveManager()->IsActive() )
		{
			if( m_byRightMouseDown == iSlot )
			{
				// 등록된 도장 업그레이드 아이템을 지운다				
				CheckInfoWindow();
				UnsetHoipoiRock();
			}
		}
	}

	m_byLeftMouseDown	= INVALID_BYTE;
	m_byRightMouseDown	= INVALID_BYTE;
}

VOID CDojoUpradeGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwUInt8 i = 0 ; i < NUM_DOJO_VISUAL_LEVEL ; ++i )
		m_srfDojo[i].SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_DojoNameBackground		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_NeedGuildPointBackground	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_NeedZennyBackground		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_NeedItemBackground		.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfNextArrowBar			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfNeedGuildPoint			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfMyGuildPoint			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfNeedZenny				.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCumptionZenny			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlot					.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlotFocus				.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_StuffSlot					.SetParentPosition(rtScreen.left, rtScreen.top);
}

VOID CDojoUpradeGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 재료 슬롯 인포
	if( m_StuffSlot.PtInRect(nX, nY) )
	{
		if( m_StuffSlot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( INFO_WINDOW_TYPE_SLOT != m_eInfoWindowType )
			{		
				m_eInfoWindowType = INFO_WINDOW_TYPE_SLOT;
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
													rtScreen.left + m_StuffSlot.GetX_fromParent(),
													rtScreen.top + m_StuffSlot.GetY_fromParent(),
													m_StuffSlot.GetSobItem(), DIALOG_DOJO_UPGRADE );				
			}			
		}

		m_bSlotFocus = TRUE;
		return;
	}
	else
	{
		m_bSlotFocus = FALSE;
	}

	// 도장 레벨 설명 인포
	RwBool		bExistDojoLevelInfo = TRUE;
	CRectangle	rtLeftDojoVisual, rtRightDojoVisual;

	switch( m_eDojoVisualLevel )
	{
	case DOJO_VISUAL_LEVEL_1:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_1].GetRect(rtLeftDojoVisual);
			m_srfDojo[DOJO_VISUAL_LEVEL_2].GetRect(rtRightDojoVisual);
			break;
		}
	case DOJO_VISUAL_LEVEL_2:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_2].GetRect(rtLeftDojoVisual);
			m_srfDojo[DOJO_VISUAL_LEVEL_3].GetRect(rtRightDojoVisual);
			break;
		}
	case DOJO_VISUAL_LEVEL_3:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_3].GetRect(rtLeftDojoVisual);
			m_srfDojo[DOJO_VISUAL_LEVEL_4].GetRect(rtRightDojoVisual);
			break;
		}
	case DOJO_VISUAL_LEVEL_4:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_4].GetRect(rtLeftDojoVisual);

			rtRightDojoVisual.left		= 0;
			rtRightDojoVisual.top		= 0;
			rtRightDojoVisual.right		= 0;
			rtRightDojoVisual.bottom	= 0;
			break;
		}
	default:
		{
			DBO_FAIL("Invalid dojo visual level");
			bExistDojoLevelInfo = FALSE;
			break;
		}
	}

	if( bExistDojoLevelInfo )
	{
		const WCHAR*	pwcText	= NULL;
		CRectangle		rtRect;

		if( rtLeftDojoVisual.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
		{
			if( INFO_WINDOW_TYPE_LEFT_DOJO_VISUAL != m_eInfoWindowType )
			{
				SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
				sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(hTarget);

				if( pDOJO_INFO )
				{
					if( DBO_MAX_LEVEL_DOJO_IN_WORLD > pDOJO_INFO->byLevel )
					{
						pwcText	= GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE_CURENT_LEVEL_1" + pDOJO_INFO->byLevel);
						rtRect	= rtLeftDojoVisual;
						m_eInfoWindowType = INFO_WINDOW_TYPE_LEFT_DOJO_VISUAL;

						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
															rtRect.left, rtRect.top,
															(VOID*)pwcText, DIALOG_DOJO_UPGRADE );
					}
				}
			}

			return;
		}
		else if( rtRightDojoVisual.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
		{
			if( INFO_WINDOW_TYPE_RIGHT_DOJO_VISUAL != m_eInfoWindowType )
			{
				SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
				sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(hTarget);

				if( pDOJO_INFO )
				{
					if( DBO_MAX_LEVEL_DOJO_IN_WORLD > pDOJO_INFO->byLevel )
					{
						pwcText	= GetDisplayStringManager()->GetString("DST_DOJO_UPGRADE_NEXT_LEVEL_2" + pDOJO_INFO->byLevel);
						rtRect	= rtRightDojoVisual;
						m_eInfoWindowType = INFO_WINDOW_TYPE_RIGHT_DOJO_VISUAL;

						GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
															rtRect.left, rtRect.top,
															(VOID*)pwcText, DIALOG_DOJO_UPGRADE );
					}
				}
			}				

			return;
		}
	}

	m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CDojoUpradeGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_bSlotFocus = FALSE;
	m_eInfoWindowType = INVALID_INFO_WINDOW_TYPE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CDojoUpradeGui::OnPaint()
{
	switch( m_eDojoVisualLevel )
	{
	case DOJO_VISUAL_LEVEL_1:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_1].Render();
			m_srfDojo[DOJO_VISUAL_LEVEL_2].Render();
			break;
		}
	case DOJO_VISUAL_LEVEL_2:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_2].Render();
			m_srfDojo[DOJO_VISUAL_LEVEL_3].Render();
			break;
		}
	case DOJO_VISUAL_LEVEL_3:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_3].Render();
			m_srfDojo[DOJO_VISUAL_LEVEL_4].Render();
			break;
		}
	case DOJO_VISUAL_LEVEL_4:
		{
			m_srfDojo[DOJO_VISUAL_LEVEL_4].Render();
			break;
		}
	default:
		{
			DBO_FAIL("Invalid dojo visual level");
			break;
		}
	}
		
	m_DojoNameBackground		.Render();
	m_NeedGuildPointBackground	.Render();
	m_NeedZennyBackground		.Render();
	m_NeedItemBackground		.Render();

	m_srfNextArrowBar			.Render();
	m_srfNeedGuildPoint			.Render();
	m_srfMyGuildPoint			.Render();
	m_srfNeedZenny				.Render();
	m_srfCumptionZenny			.Render();
	m_srfSlot					.Render();

	m_StuffSlot					.Paint();

	if( m_bSlotFocus || m_bSlotDest )
		m_srfSlotFocus.Render();
}

VOID CDojoUpradeGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_DOJO_UPGRADE, key.m_fX, key.m_fY);
}

RwInt32 CDojoUpradeGui::SwitchDialog(bool bOpen)
{
	if( bOpen)
	{
		SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
		sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(hTarget);

		if( !pDOJO_INFO )
		{
			GetAlarmManager()->AlarmMessage("DST_DOJO_HAVE_NOT_DOJO");

			Logic_CancelNpcFacing();
			return -1;
		}

		m_hNPCSerial = hTarget;		

		SetDojoLevel(pDOJO_INFO);

		SetNeedGuildPoint(pDOJO_INFO);
		SetConsumptionGuildPoint(pDOJO_INFO);

		SetNeedZennyPoint(pDOJO_INFO);
		SetConsumptionZennyPoint(pDOJO_INFO);

		SetNeedItem(pDOJO_INFO);

		m_uiDojoTableIndex = pDOJO_INFO->uiDojoTableIndex;

		GetDboGlobal()->GetGamePacketGenerator()->SendDojo_NPCInfoReq(hTarget);
	}
	else
	{	
		Clear();

		Logic_CancelNpcFacing();

		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI");
		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_ASK_UPGRAGE_WITH_ZENNY");
	}

	Show(bOpen);
	return 1;
}

VOID CDojoUpradeGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDojoUpradeGui::HandleEvents");

	if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC 서버가 다운되거나 하여 갑자기 NPC가 사라지는 경우
			GetDialogManager()->CloseDialog(DIALOG_DOJO_UPGRADE);
		}
	}
	else if( msg.Id == g_EventDialog )
	{		
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_DOJO_UPGRADE )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE ||
				pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				GetDialogManager()->CloseDialog(DIALOG_DOJO_UPGRADE);
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
				if( ITEM_TYPE_HOIPOIROCK == pITEM_TBLDAT->byItem_Type )
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
	else if( msg.Id == g_EventNotifyGuild )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_UPGRADE) )
			NTL_RETURNVOID();


		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(m_hNPCSerial);
		if( !pDOJO_INFO )
		{
			DBO_FAIL("Invalid dojo info pointer");
			NTL_RETURNVOID();
		}

		CNtlGuild* pGuild = pAvatar->GetGuild();
		if( pGuild->GetGuildID() != pDOJO_INFO->guildID )
			NTL_RETURNVOID();


		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );
		switch(pEvent->iMessage)
		{
		case SLGE_DISBAND:
			{
				GetDialogManager()->CloseDialog(DIALOG_DOJO_UPGRADE);
				break;
			}
		case SLGE_CHANGE_GUILD_NAME:
			{
				SetDojoName( (WCHAR*)pGuild->GetGuildName() );
				break;
			}
		case SLGE_CHANGE_REPUTATION:
			{
				sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(m_hNPCSerial);
				SetNeedGuildPoint(pDOJO_INFO);
				break;
			}
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( m_hNPCSerial == INVALID_SERIAL_ID )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		if( Logic_IsUpdateType(EVENT_AIUT_ZENNY, msg.pData) )
		{
			sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(m_hNPCSerial);
			SetConsumptionZennyPoint(pDOJO_INFO);	
		}
		else if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) )
		{
			if( INVALID_SERIAL_ID != m_StuffSlot.GetSerial() )
			{
				if( NULL == GetNtlSobManager()->GetSobObject(m_StuffSlot.GetSerial()) )
				{
					// 등록했던 호이포이락이 사라졌다
					CheckInfoWindow();
					UnsetHoipoiRock();
				}
			}
		}
	}
	else if( msg.Id == g_EventDirectMoveIcon )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_UPGRADE) )
			NTL_RETURNVOID();

		SDboEventDirectMoveIcon* pEvent = reinterpret_cast<SDboEventDirectMoveIcon*>( msg.pData );

		if( PLACE_DOJO_UPRAGE != pEvent->eDestPlace )
			NTL_RETURNVOID();		

		CheckInfoWindow();
		SetHoipoiRock(pEvent->uiSrcSerial, pEvent->uiSrcStackCount);
	}
	else if( msg.Id == g_EventDojoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_UPGRADE) )
			NTL_RETURNVOID();


		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( pEvent->uiParam != m_uiDojoTableIndex )
			NTL_RETURNVOID();

		if( DOJO_EVENT_NPC_GUI_INFO == pEvent->byDojoEvent )
		{
			sDBO_DOJO_NPC_INFO* pDBO_DOJO_NPC_INFO = (sDBO_DOJO_NPC_INFO*)pEvent->pExData;
			SetDojoName(pDBO_DOJO_NPC_INFO->wszName);
		}
		else if( DOJO_EVENT_CHANGE_LEVEL == pEvent->byDojoEvent )
		{
			sDOJO_INFO* pDOJO_INFO = Logic_GetDojoInfo(m_hNPCSerial);

			SetDojoLevel(pDOJO_INFO);

			SetNeedGuildPoint(pDOJO_INFO);
			SetConsumptionGuildPoint(pDOJO_INFO);

			SetNeedZennyPoint(pDOJO_INFO);
			SetConsumptionZennyPoint(pDOJO_INFO);

			SetNeedItem(pDOJO_INFO);

			UnsetHoipoiRock();
		}
		else if( DOJO_EVENT_LOST_DOJO == pEvent->byDojoEvent )
		{
			Clear();
		}
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_UPGRADE) )
			NTL_RETURNVOID();

		SDboEventEnableItemIcon* pEvent = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );
		if( PLACE_DOJO_UPRAGE != pEvent->ePlace )
			NTL_RETURNVOID();

		m_bExpectServerAnswer = !pEvent->bEnable;
	}

	NTL_RETURNVOID();
}