#include "precomp_dboclient.h"
#include "ChatOptionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"

// dbo
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"
#include "DialogPriority.h"
#include "AlarmManager.h"

#define dENABLE_COLOR		RGB(255, 255, 104)
#define dDISABLE_COLOR		RGB(168, 168, 168)

CChatOptionGui::CChatOptionGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CChatOptionGui::~CChatOptionGui()
{

}

RwBool CChatOptionGui::Create(CChatGui* pChatGui)
{
	NTL_FUNCTION( "CChatOptionGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ChatOption.srf", "gui\\ChatOption.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_CHATTING_OPTION);

	m_pChatGui = pChatGui;

	// 닫기 버튼
	m_pCloseButton = (gui::CButton*)GetComponent("btnClose");
	m_slotCloseButton = m_pCloseButton->SigClicked().Connect(this, &CChatOptionGui::OnClicked_CloseButton);	

	// 타이틀
	m_pTitle = (gui::CStaticBox*)GetComponent("stbTitle");
	m_pTitle->SetTextColor(dENABLE_COLOR);
	m_pTitle->SetText( GetDisplayStringManager()->GetString("DST_CHAT_OPTION_GUI_TITLE") );


	// 일반
	m_pShoutcut[CHAT_TYPE_GENERAL]			= (gui::CStaticBox*)GetComponent("stbShortcutGeneral");
	m_pShoutcutExplain[CHAT_TYPE_GENERAL]	= (gui::CStaticBox*)GetComponent("stbShortcutGeneralExplain");
	m_pChatTypeButton[CHAT_TYPE_GENERAL]	= (gui::CButton*)GetComponent("btnGeneral");
	m_pToggleQuickButton[CHAT_TYPE_GENERAL]	= (gui::CButton*)GetComponent("btnToggleGeneral");

	m_pShoutcut[CHAT_TYPE_GENERAL]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_GENERAL") );
	m_pShoutcutExplain[CHAT_TYPE_GENERAL]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_GENERAL") );

	
	// 거래
	m_pShoutcut[CHAT_TYPE_TRADE]			= (gui::CStaticBox*)GetComponent("stbShortcutTrade");
	m_pShoutcutExplain[CHAT_TYPE_TRADE]		= (gui::CStaticBox*)GetComponent("stbShortcutTradeExplain");
	m_pChatTypeButton[CHAT_TYPE_TRADE]		= (gui::CButton*)GetComponent("btnTrade");
	m_pToggleQuickButton[CHAT_TYPE_TRADE]	= (gui::CButton*)GetComponent("btnToggleTrade");

	m_pShoutcut[CHAT_TYPE_TRADE]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_TRADE") );
	m_pShoutcutExplain[CHAT_TYPE_TRADE]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_TRADE") );

	// find party
	m_pShoutcut[CHAT_TYPE_FIND_PARTY] = (gui::CStaticBox*)GetComponent("stbShortcutFindParty");
	m_pShoutcutExplain[CHAT_TYPE_FIND_PARTY] = (gui::CStaticBox*)GetComponent("stbShortcutFindPartyExplain");
	m_pChatTypeButton[CHAT_TYPE_FIND_PARTY] = (gui::CButton*)GetComponent("btnFindParty");
	m_pToggleQuickButton[CHAT_TYPE_FIND_PARTY] = (gui::CButton*)GetComponent("btnToggleFindParty");

	m_pShoutcut[CHAT_TYPE_FIND_PARTY]->SetText(GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_FIND_PARTY"));
	m_pShoutcutExplain[CHAT_TYPE_FIND_PARTY]->SetText(GetDisplayStringManager()->GetString("DST_CHAT_MODE_FIND_PARTY"));


	// 유파
	m_pShoutcut[CHAT_TYPE_GUILD]			= (gui::CStaticBox*)GetComponent("stbShortcutGuild");
	m_pShoutcutExplain[CHAT_TYPE_GUILD]		= (gui::CStaticBox*)GetComponent("stbShortcutGuildExplain");
	m_pChatTypeButton[CHAT_TYPE_GUILD]		= (gui::CButton*)GetComponent("btnGuild");	
	m_pToggleQuickButton[CHAT_TYPE_GUILD]	= (gui::CButton*)GetComponent("btnToggleGuild");

	m_pShoutcut[CHAT_TYPE_GUILD]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_GUILD") );
	m_pShoutcutExplain[CHAT_TYPE_GUILD]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_GUILD") );


	// 파티
	m_pShoutcut[CHAT_TYPE_PARTY]			= (gui::CStaticBox*)GetComponent("stbShortcutParty");
	m_pShoutcutExplain[CHAT_TYPE_PARTY]		= (gui::CStaticBox*)GetComponent("stbShortcutPartyExplain");
	m_pChatTypeButton[CHAT_TYPE_PARTY]		= (gui::CButton*)GetComponent("btnParty");	
	m_pToggleQuickButton[CHAT_TYPE_PARTY]	= (gui::CButton*)GetComponent("btnToggleParty");	

	m_pShoutcut[CHAT_TYPE_PARTY]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_PARTY") );
	m_pShoutcutExplain[CHAT_TYPE_PARTY]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_PARTY") );


	// 귓속말
	m_pShoutcut[CHAT_TYPE_WHISPER]			= (gui::CStaticBox*)GetComponent("stbShortcutWhisper");
	m_pShoutcutExplain[CHAT_TYPE_WHISPER]	= (gui::CStaticBox*)GetComponent("stbShortcutWhisperExplain");
	m_pChatTypeButton[CHAT_TYPE_WHISPER]	= (gui::CButton*)GetComponent("btnWhisper");	
	m_pToggleQuickButton[CHAT_TYPE_WHISPER] = (gui::CButton*)GetComponent("btnToggleWhisper");

	m_pShoutcut[CHAT_TYPE_WHISPER]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_WHISPER") );
	m_pShoutcutExplain[CHAT_TYPE_WHISPER]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_WHISPER") );


	// Shout
	m_pShoutcut[CHAT_TYPE_SHOUT]			= (gui::CStaticBox*)GetComponent("stbShortcutShout");
	m_pShoutcutExplain[CHAT_TYPE_SHOUT]		= (gui::CStaticBox*)GetComponent("stbShortcutShoutExplain");
	m_pChatTypeButton[CHAT_TYPE_SHOUT]		= (gui::CButton*)GetComponent("btnShout");
	m_pToggleQuickButton[CHAT_TYPE_SHOUT]	= (gui::CButton*)GetComponent("btnToggleShout");

	m_pShoutcut[CHAT_TYPE_SHOUT]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_SHOUT") );
	m_pShoutcutExplain[CHAT_TYPE_SHOUT]->SetText( GetDisplayStringManager()->GetString("DST_CHAT_MODE_SHOUT") );
	

	for( RwUInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		m_pToggleQuickButton[i]->SetToggleMode(true);
		m_slotChatTypeButton[i]		= m_pChatTypeButton[i]->SigClicked().Connect(this, &CChatOptionGui::OnClicked_ChatTypeButton);	
		m_slotToggleQuickButton[i]	= m_pToggleQuickButton[i]->SigToggled().Connect(this, &CChatOptionGui::OnClicked_ToggleQuickButton);
	}

	// Command (Help)
	m_pstbCommandShortcutExplain = (gui::CStaticBox*)GetComponent("stbCommandShortcutExplain");
	m_pCommand								= (gui::CStaticBox*)GetComponent("stbCommandExplain");
	m_pCommandButton						= (gui::CButton*)GetComponent("btnCommand");

	m_pstbCommandShortcutExplain->SetText(L"/?");

	m_pCommand->SetText( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_COMMAND") );
	m_pCommandButton->SetToolTip( GetDisplayStringManager()->GetString("DST_CHAT_SHORT_CUT_COMMAND") );
	m_slotCommand = m_pCommandButton->SigClicked().Connect(this, &CChatOptionGui::OnClicked_CommandButton);


	EnableCatting(CHAT_TYPE_GENERAL,	true);
	EnableCatting(CHAT_TYPE_TRADE,		true);
	EnableCatting(CHAT_TYPE_FIND_PARTY, true);
	EnableCatting(CHAT_TYPE_GUILD,		false);
	EnableCatting(CHAT_TYPE_PARTY,		false);
	EnableCatting(CHAT_TYPE_WHISPER,	false);
	EnableCatting(CHAT_TYPE_SHOUT,		true);


	Show(false);

	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventNotifyGuild);
	LinkMsg(g_EventGameServerChangeOut);

	NTL_RETURN(TRUE);
}

VOID CChatOptionGui::Destroy()
{
	NTL_FUNCTION("CChatOptionGui::Destroy");

	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventGameServerChangeOut);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CChatOptionGui::EnableCatting(eChatType eType, BOOL bEnable)
{
	if( FALSE == Logic_IsUsableIndex(eType, NUM_NET_CHAT_TYPE) )
	{
		DBO_FAIL("Invalid chat type");
		return;
	}	

	if( bEnable )
	{
		m_pShoutcut[eType]->SetTextColor(dENABLE_COLOR, TRUE);
		m_pShoutcutExplain[eType]->SetTextColor(dENABLE_COLOR, TRUE);
		m_pToggleQuickButton[eType]->Show(true);
	}
	else
	{
		if( false == m_pChatGui->GetLastSentWhisperName().empty() )
			return;

		m_pShoutcut[eType]->SetTextColor(dDISABLE_COLOR, TRUE);
		m_pShoutcutExplain[eType]->SetTextColor(dDISABLE_COLOR, TRUE);

		m_pToggleQuickButton[eType]->Show(false);
		m_pToggleQuickButton[eType]->SetDown(false);
		m_pChatGui->ShowChatButton(eType, false);
	}

	m_pChatTypeButton[eType]->ClickEnable(bEnable);
}

VOID CChatOptionGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	SwitchDialog(false);
}

VOID CChatOptionGui::OnClicked_ChatTypeButton(gui::CComponent* pComponent)
{
	for( RwUInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		if( m_pChatTypeButton[i] == pComponent )
		{
			m_pChatGui->SetChatType((eChatType)i);
			SwitchDialog(false);
			break;
		}
	}
}

VOID CChatOptionGui::OnClicked_ToggleQuickButton(gui::CComponent* pComponent, bool bToggled)
{
	for( RwUInt8 i = 0 ; i < NUM_NET_CHAT_TYPE ; ++i )
	{
		if( m_pToggleQuickButton[i] == pComponent )
		{
			m_pChatGui->ShowChatButton((eChatType)i, bToggled);
			break;
		}
	}
}

VOID CChatOptionGui::OnClicked_CommandButton(gui::CComponent* pComponent)
{
	GetAlarmManager()->AlarmMessage("DST_CHAT_HELP_TEXT");
}

VOID CChatOptionGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		RaiseLinked();
		Show(true);
	}
	else
	{
		Show(false);		
	}
}

VOID CChatOptionGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChatOptionGui::HandleEvents");

	if( msg.Id == g_EventPartyUpdateValue )
	{
		SNtlEventPartyUpdate* pEvent = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		if( pEvent->iMessage == PMT_PARTY_CREATE )
		{
			EnableCatting(CHAT_TYPE_PARTY, true);
		}
		else if( pEvent->iMessage == PMT_PARTY_LEAVE )
		{
			EnableCatting(CHAT_TYPE_PARTY, false);
		}
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );

		if( pEvent->iMessage == SLGE_GUILD_INFO )
		{
			EnableCatting(CHAT_TYPE_GUILD, true);
		}
		else if( pEvent->iMessage == SLGE_DISBAND )
		{
			EnableCatting(CHAT_TYPE_GUILD, false);
		}
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		EnableCatting(CHAT_TYPE_PARTY, false);
		EnableCatting(CHAT_TYPE_GUILD, false);
	}

	NTL_RETURNVOID();
}