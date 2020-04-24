#include "precomp_dboclient.h"
#include "GMChatGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// cleint
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "PetitionManager.h"

#define dGM_TEXT_COLOR			RGB(255, 219, 71)

CGMChatGui::CGMChatGui(const RwChar* pName)
:CNtlPLGui( pName )
{

}

CGMChatGui::~CGMChatGui()
{

}

RwBool CGMChatGui::Create()
{
	NTL_FUNCTION( "CGMChatGui::Create" );

	if(!CNtlPLGui::Create("gui\\GMChat.rsr", "gui\\GMChat.srf", "gui\\GMChat.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_QUICKSLOT);

	// 다이얼로그 제목
	m_pDialogName	= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	WCHAR awcBuffer[256] = L"";
	const WCHAR* pwcTitle = GetDisplayStringManager()->GetString("DST_PETITION_GM_CHAT_TITLE");
	swprintf_s(awcBuffer, 256, L"%s : %s", pwcTitle, GetPetitionManager()->GetChattingGMName() );
	m_pDialogName->SetText( awcBuffer );

	// 대화내용
	m_pOutDisplay	= (gui::COutputBox*)GetComponent( "outputDisplay" );
	m_pOutDisplay->SetLineSpace(CHAT_DISP_LINE_GAP);
	m_pOutDisplay->SetMaxLine(CHAT_DISP_MAX_LINE);

	// IME 상태
	m_pIME			= (gui::CStaticBox*)GetComponent( "stbIME" );
	OnIMEChange( m_pInput->GetCurrentIMEState() );

	// 유저 인풋창
	m_pInput		= (gui::CInputBox*)GetComponent( "inbChat" );
	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_VERTICAL);
	m_BackPanel.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMChat.srf", "srfBackPanelLeft" ) );
	m_BackPanel.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMChat.srf", "srfBackPanelCenter" ) );
	m_BackPanel.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMChat.srf", "srfBackPanelRight" ) );
	m_BackPanel.SetSize(269, 305);
	m_BackPanel.SetPositionfromParent(12, 45);

	// 인풋 배경
	m_InputBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMChat.srf", "srfInputBack" ) );
	m_InputBack.SetPositionfromParent(12, 359);

	// sig
	m_slotGotFocus			= m_pInput->SigGotFocus().Connect( this, &CGMChatGui::OnGotFocus );
	m_slotLostFocus			= m_pInput->SigLostFocus().Connect( this, &CGMChatGui::OnLostFocus );
	m_slotReturn			= m_pInput->SigReturnPressed().Connect( this, &CGMChatGui::OnInputReturn );	
	m_slotIMEChange			= m_pInput->SigIMEChanged().Connect( this, &CGMChatGui::OnIMEChange );
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CGMChatGui::OnCaptureWheelMove );
	m_slotMove				= m_pThis->SigMove().Connect( this, &CGMChatGui::OnMove );
	m_slotPaint				= m_pThis->SigPaint().Connect( this, &CGMChatGui::OnPaint );

	// Surface들을 위치시키기 위해
	CRectangle rtScreen = m_pThis->GetScreenRect();
	OnMove(rtScreen.left, rtScreen.top);

	Show(false);

	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CGMChatGui::Destroy()
{
	NTL_FUNCTION("CGMChatGui::Destroy");

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );

	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGMChatGui::SetGMText(const WCHAR* pwcText)
{
	if( !pwcText )
		return;
	
	m_pOutDisplay->Format(dGM_TEXT_COLOR, L"%s : %s", GetPetitionManager()->GetChattingGMName(), pwcText);
}

VOID CGMChatGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

VOID CGMChatGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

VOID CGMChatGui::OnInputReturn()
{
	if( m_pInput->GetLength() == 0 )
		return;

	ACCOUNTID GMID = GetPetitionManager()->GetChattingGMID();

	GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatUserSayReq(GMID, m_pInput->GetText() );
	
	m_pOutDisplay->Format(L"%s : %s", Logic_GetAvatarName(), m_pInput->GetText() );

	m_pInput->Clear();
}

VOID CGMChatGui::OnIMEChange( RwInt32 uiIMEState )
{
	if( uiIMEState == 0 )
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( "DST_CHAT_IME_ENG" ) );
	}
	else
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( "DST_CHAT_IME_KOR" ) );
	}
}

VOID CGMChatGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	if( sDelta < 0 )
		m_pOutDisplay->NextLine();
	else
		m_pOutDisplay->PrevLine();
}

VOID CGMChatGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_BackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_InputBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CGMChatGui::OnPaint()
{
	m_BackPanel.Render();
	m_InputBack.Render();
}

RwInt32 CGMChatGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CGMChatGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGMChatGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		Logic_LocateDialog_in_CleintRect(this, TRUE);
	}

	NTL_RETURNVOID();
}