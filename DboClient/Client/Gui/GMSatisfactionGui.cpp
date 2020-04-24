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
	m_pThis->SetPriority(dDIALOGPRIORITY_CHATTING);

	// 다이얼로그 제목
	m_pDialogName	= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	WCHAR awcBuffer[256] = L"";
	swprintf_s(awcBuffer, 256, L"GM 대화창 : ", GetPetitionManager()->GetChattingGMName() );
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

	// sig
	m_slotGotFocus			= m_pInput->SigGotFocus().Connect( this, &CGMChatGui::OnGotFocus );
	m_slotLostFocus			= m_pInput->SigLostFocus().Connect( this, &CGMChatGui::OnLostFocus );
	m_slotReturn			= m_pInput->SigReturnPressed().Connect( this, &CGMChatGui::OnInputReturn );	
	m_slotIMEChange			= m_pInput->SigIMEChanged().Connect( this, &CGMChatGui::OnIMEChange );
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CGMChatGui::OnCaptureWheelMove );

	Show(true);

	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CGMChatGui::Destroy()
{
	NTL_FUNCTION("CGMChatGui::Destroy");

	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGMChatGui::SetGMText(const WCHAR* pwcText)
{
	if( !pwcText )
		return;

	m_pOutDisplay->AddText( pwcText, dGM_TEXT_COLOR );
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

	RwUInt32 uiGMID = GetPetitionManager()->GetChattingGMID();

	GetDboGlobal()->GetChatPacketGenerator()->SendPetitionChatUserSayReq(uiGMID, m_pInput->GetText() );
	m_pOutDisplay->AddText( m_pInput->GetText() );

	m_pInput->Clear();
}

VOID CGMChatGui::OnIMEChange( RwInt32 uiIMEState )
{
	if( uiIMEState == 0 )
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_ENG ) );
	}
	else
	{
		m_pIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_KOR ) );
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
		Logic_LocateDialog_in_CleintRect(this, true);
	}

	NTL_RETURNVOID();
}