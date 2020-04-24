#include "precomp_dboclient.h"
#include "GuildNoticeGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"


CGuildNoticeGui::CGuildNoticeGui(const RwChar* pName)
:CNtlPLGui(pName)
{
}

CGuildNoticeGui::~CGuildNoticeGui()
{
}

RwBool CGuildNoticeGui::Create(eNOTICE_TYPE eNoticeType)
{
	NTL_FUNCTION( "CGuildNoticeGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\GuildNotice.srf", "gui\\GuildNotice.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());


	m_eNoticeType = eNoticeType;


	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 뒷 배경
	m_srfBackboard.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildNotice.srf", "srfBackboardLeft" ) );
	m_srfBackboard.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildNotice.srf", "srfBackboardCenter" ) );
	m_srfBackboard.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildNotice.srf", "srfBackboardRight" ) );
	m_srfBackboard.SetSize(277, 208);
	m_srfBackboard.SetPositionfromParent(0, 0);	

	m_pTitle		= (gui::CStaticBox*)GetComponent("stbTitle");

	if( NOTICE_TYPE_GUILD == m_eNoticeType )
	{
		m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_GUILD_NOTICE_TITLE"));
	}
	else if( NOTICE_TYPE_DOJO == m_eNoticeType )
	{
		m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_NOTICE_INPUT"));
	}

	m_pExitButton	= (gui::CButton*)GetComponent("btnExit");
	m_pOKButton		= (gui::CButton*)GetComponent("btnOK");
	m_pCancelButton	= (gui::CButton*)GetComponent("btnCancel");

	m_pOKButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));

	m_slotExitButton	= m_pExitButton->SigClicked().Connect(this, &CGuildNoticeGui::OnClicked_CancelButton);
	m_slotOKButton		= m_pOKButton->SigClicked().Connect(this, &CGuildNoticeGui::OnClicked_OKButton);
	m_slotCancelButton	= m_pCancelButton->SigClicked().Connect(this, &CGuildNoticeGui::OnClicked_CancelButton);

	// 공지 입력란
	m_pNoticeInput		= (gui::CInputBox*)GetComponent("NoticeInput");	
	m_pNoticeInput->SetMultilineMode(TRUE);
	m_pNoticeInput->SetMaxLength(NTL_MAX_LENGTH_OF_GUILD_NOTICE);
	m_pNoticeInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_slotReturnInput	= m_pNoticeInput->SigReturnPressed().Connect(this, &CGuildNoticeGui::OnReturn_NoticeInput);


	m_slotMove			= m_pThis->SigMove().Connect( this, &CGuildNoticeGui::OnMove );	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CGuildNoticeGui::OnPaint );	

	LinkMsg(g_EventPostDialog);

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CGuildNoticeGui::Destroy()
{
	NTL_FUNCTION("CGuildNoticeGui::Destroy");

	UnLinkMsg(g_EventPostDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CGuildNoticeGui::Active(bool bOpen)
{
	RaiseTop();
	Show(bOpen);
}

VOID CGuildNoticeGui::SetInputFocus()
{
	m_pNoticeInput->SetFocus();
}

VOID CGuildNoticeGui::OnReturn_NoticeInput()
{
	OnClicked_OKButton(NULL);
}

VOID CGuildNoticeGui::OnClicked_OKButton(gui::CComponent* pComponent)
{
	const WCHAR* pwcText = m_pNoticeInput->GetText();
	if( !pwcText )
		return;

	RwInt32 iLength = wcslen(pwcText);

	if( iLength == 0 || iLength > NTL_MAX_LENGTH_OF_GUILD_NOTICE)
		return;


	if( NOTICE_TYPE_GUILD == m_eNoticeType )
	{
		if( GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Notice_Req((RwUInt16)iLength, pwcText) )
			Enable(false);
	}
	else if( NOTICE_TYPE_DOJO == m_eNoticeType )
	{
		if( GetDboGlobal()->GetChatPacketGenerator()->SendDojo_NoticeChangeReq((RwUInt16)iLength, (WCHAR*)pwcText) )
			Enable(false);
	}
}

VOID CGuildNoticeGui::OnClicked_CancelButton(gui::CComponent* pComponent)
{
	Enable(false);
	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_GUILD_NOTICE);
}

VOID CGuildNoticeGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfBackboard.SetPositionbyParent(rtScreen.left, rtScreen.top);	
}

VOID CGuildNoticeGui::OnPaint()
{
	m_srfBackboard.Render();
}

VOID CGuildNoticeGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGuildNoticeGui::HandleEvents");

	if( msg.Id == g_EventPostDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iType == DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE )
			Enable(true);
	}

	NTL_RETURNVOID();
}