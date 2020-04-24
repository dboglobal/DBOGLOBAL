#include "precomp_dboclient.h"
#include "CommunityGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "CommunityBase.h"
#include "CommunityGuild.h"
#include "CommunityRaid.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"


CCommunityGui::CCommunityGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pDialogName(NULL)
,m_iPageType(INVALID_PAGE)
{

}

CCommunityGui::~CCommunityGui()
{
	Destroy();
}

RwBool CCommunityGui::Create()
{
	NTL_FUNCTION( "CCommunityGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Community.srf", "gui\\Community.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;
	char acBuffer[64] = "";

	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_COMMUNITY_NAME"));
	m_pDialogName->Enable(false);

	rect = GetPosition();

	// 백라인
	m_BackLineSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackLineSurface.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineTop" ) );
	m_BackLineSurface.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineCenter" ) );
	m_BackLineSurface.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineBottom" ) );
	m_BackLineSurface.SetSize(303, 414);
	m_BackLineSurface.SetPositionfromParent(9, 50);


	// 길드 페이지
	m_pTabPage[PAGETYPE_GUILD] = NTL_NEW CCommunityGuild;
	m_pTabPage[PAGETYPE_GUILD]->Create(this);
/*
	// 공대 페이지
	m_pTabPage[PAGETYPE_RAID] = NTL_NEW CCommunityRaid;
	m_pTabPage[PAGETYPE_RAID]->Create(this);
*/	

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CCommunityGui::CloseButtonClicked);		

	// 탭 버튼
	m_pTabButton = (gui::CTabButton*)GetComponent( "TabButton" );	

	m_pBtnHelp = (gui::CButton*)GetComponent( "btnHelp" );
	m_slotClickedBtnHelp = m_pBtnHelp->SigClicked().Connect( this, &CCommunityGui::OnClickedBtnHelp );

	std::string str;
	for( int i = 0 ; i < NUM_PAGE ; ++i )
	{
		WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_TAB_GUILD" + i), -1, acBuffer, 64, NULL, NULL);
		str = acBuffer;
		m_pTabButton->AddTab( str );
	}

	m_pTabButton->SelectTab(0);
	
	// 아웃라인
	rect = GetPosition();	

	// sig
	m_slotTabChanged	= m_pTabButton->SigSelectChanged().Connect( this, &CCommunityGui::OnTabChanged );
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CCommunityGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CCommunityGui::OnMouseUp );	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CCommunityGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CCommunityGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CCommunityGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCommunityGui::OnPaint );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CCommunityGui::OnCaptureMouseDown );

	// OnMove를 통해 위치가 결정되는 컴포넌트를 위해서
	OnMove(100, 100);

	// Link
	LinkMsg(g_EventNotifyGuild);
	LinkMsg(g_EventPostDialog);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventGameServerChangeOut);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CCommunityGui::Destroy()
{
	NTL_FUNCTION("CCommunityGui::Destroy");

	for( RwUInt8 i = 0 ; i < NUM_PAGE ; ++i )
	{
		if( m_pTabPage[i] )
		{
			m_pTabPage[i]->Destroy();
			NTL_DELETE(m_pTabPage[i]);
		}		
	}

	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventPostDialog);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventGameServerChangeOut);
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CCommunityGui::CloseButtonClicked(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_COMMUNITY_WND_CLOSE_BTN ) )
		return;

	GetDialogManager()->CloseDialog(DIALOG_COMMUNITY);
}

VOID CCommunityGui::OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex )
{
	SetupPage(nIndex);
}

VOID CCommunityGui::SetPageType(RwInt32 iPageType)
{
	m_pTabButton->SelectTab(iPageType);
	SetupPage(iPageType);
}

RwInt32 CCommunityGui::GetPageType()
{
	return m_iPageType;
}

VOID CCommunityGui::SetupPage(RwInt32 iPage)
{
	if( m_iPageType != INVALID_PAGE )
		m_pTabPage[m_iPageType]->ActiveTab(false);

	m_iPageType = iPage;

	m_pTabPage[m_iPageType]->ActiveTab(true);
}

CHARACTERID CCommunityGui::GetGuildSelectedID()
{
	CCommunityGuild* pCommunityGuild = reinterpret_cast<CCommunityGuild*>( m_pTabPage[PAGETYPE_GUILD] );
	return pCommunityGuild->GetSeletedID();
}

VOID CCommunityGui::OnMouseDown(const CKey& key)
{
	m_pTabPage[m_iPageType]->OnMouseDown(key);
}

VOID CCommunityGui::OnMouseUp(const CKey& key)
{		
	
}

VOID CCommunityGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = GetPosition();	
	
	for( RwUInt8 i = 0 ; i < NUM_PAGE ; ++i )
		m_pTabPage[i]->OnMove(iOldX, iOldY);

	m_BackLineSurface.SetPositionbyParent(rect.left, rect.top);

	MoveLinkedPLGui(rect.left - iOldX, rect.top - iOldY);
}

VOID CCommunityGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{	
	m_pTabPage[m_iPageType]->OnMouseMove(nFlags, nX, nY);
}

VOID CCommunityGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_pTabPage[m_iPageType]->OnMouseLeave(pComponent);
}

VOID CCommunityGui::OnPaint()
{
	m_BackLineSurface.Render();
	if( m_iPageType < INVALID_PAGE )
		m_pTabPage[m_iPageType]->OnPaint();	
}

VOID CCommunityGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_COMMUNITY, key.m_fX, key.m_fY);
}

RwInt32 CCommunityGui::SwitchDialog(bool bOpen)
{
	if( m_iPageType == INVALID_PAGE )
		SetupPage(PAGETYPE_GUILD);

	m_pTabPage[m_iPageType]->SwitchDialog(bOpen);	

	if( bOpen )
	{
		Show(true);
	}	
	else
	{
		Show(false);
	}

	return 1;
}

VOID CCommunityGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCommunityGui::HandleEvents");

	if(msg.Id == g_EventNotifyGuild || msg.Id == g_EventIconPopupResult ||
	   msg.Id == g_EventPostDialog)
	{
		m_pTabPage[PAGETYPE_GUILD]->HandleEvents(msg);
	}
	else if(msg.Id == g_EventGameServerChangeOut)
	{
		for( RwInt8 i = 0 ; i < NUM_PAGE ; ++i )
			m_pTabPage[i]->HandleEvents(msg);
	}
	
	NTL_RETURNVOID();
}

VOID CCommunityGui::OnClickedBtnHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_COMMUNITY );
}