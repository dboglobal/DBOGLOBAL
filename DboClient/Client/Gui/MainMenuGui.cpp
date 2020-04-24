#include "precomp_dboclient.h"
#include "MainMenuGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLLuaGlue.h"
#include "DboTLDefine.h"
#include "NtlSLLogic.h"
#include "NtlSLCCBD.h"
#include "NtlSobAvatar.h"

// Dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"



CMainMenuGroup::CMainMenuGroup(VOID)
: m_pMainMenuGui( NULL ), m_pMainMenuButtonGui( NULL )
{

}

CMainMenuGroup::~CMainMenuGroup(VOID)
{

}

RwBool CMainMenuGroup::Create(VOID)
{
	NTL_FUNCTION("CMainMenuGroup::Create");

	m_pMainMenuGui = NTL_NEW CMainMenuGui( "MainMenu" );
	if( !m_pMainMenuGui->Create() )
	{
		m_pMainMenuGui->Destroy();
		NTL_DELETE( m_pMainMenuGui );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pMainMenuGui ); 
	GetDialogManager()->RegistDialog( DIALOG_MAINMENU, m_pMainMenuGui, &CMainMenuGui::SwitchDialog );

	m_pMainMenuButtonGui = NTL_NEW CMainMenuButtonGui( "MainMenuButton" );
	if( !m_pMainMenuButtonGui->Create() )
	{
		m_pMainMenuButtonGui->Destroy();
		NTL_DELETE( m_pMainMenuButtonGui );
		NTL_RETURN( FALSE );
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pMainMenuButtonGui ); 
	GetDialogManager()->RegistDialog( DIALOG_MAINMENUBUTTON, m_pMainMenuButtonGui, &CMainMenuButtonGui::SwitchDialog );

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CMainMenuGroup::OnCaptureMouseDown );

	NTL_RETURN( TRUE );

}

VOID CMainMenuGroup::Destroy(VOID)
{
	if( m_pMainMenuGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMainMenuGui );
		m_pMainMenuGui->Destroy();
		NTL_DELETE( m_pMainMenuGui );
	}

	if( m_pMainMenuButtonGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pMainMenuButtonGui );
		m_pMainMenuButtonGui->Destroy();
		NTL_DELETE( m_pMainMenuButtonGui );
	}
}

VOID CMainMenuGroup::OnCaptureMouseDown( const CKey& key )
{
	if( !m_pMainMenuGui->IsShow() )
		return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		return;

	if( m_pMainMenuGui->GetDialog()->PosInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) == gui::CComponent::INRECT ||
		m_pMainMenuButtonGui->GetDialog()->PosInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY) == gui::CComponent::INRECT ) 
		return;	

	GetDialogManager()->CloseDialog( DIALOG_MAINMENU );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

CMainMenuGui::CMainMenuGui()
{
}

CMainMenuGui::CMainMenuGui( const RwChar* pName )
:CNtlPLGui( pName )
{
	
}

CMainMenuGui::~CMainMenuGui()
{
}

RwBool CMainMenuGui::Create(VOID)
{
	NTL_FUNCTION("CMainMenuGui::Create");

	if(!CNtlPLGui::Create("", "gui\\MainBtn.srf", "gui\\MainMenu.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pbtnStatus = (gui::CButton*)GetComponent("btnStatus");
	m_pBtnInven = (gui::CButton*)GetComponent( "btnInven" );
	m_pbtnSkill = (gui::CButton*)GetComponent("btnSkill");
	m_pbtnQuest = (gui::CButton*)GetComponent( "btnQuest" );
	m_pbtnFriendList = (gui::CButton*)GetComponent("btnFriendList");
	m_pbtnCommunity = (gui::CButton*)GetComponent( "btnCommunity" );
	m_pbtnPartySearch = (gui::CButton*)GetComponent("btnPartySearch");
	m_pbtnRankBoard = (gui::CButton*)GetComponent( "btnRankBoard" );
	m_pbtnMascot = (gui::CButton*)GetComponent("btnMascot");
	m_pbtnHelp = (gui::CButton*)GetComponent("btnHelp");
	m_pbtnSystem = (gui::CButton*)GetComponent("btnSystem");
	m_pbtnChannelChange = (gui::CButton*)GetComponent( "btnChannelChange" );
	m_pbtnLogin = (gui::CButton*)GetComponent( "btnLogin" );
	m_pbtnExit = (gui::CButton*)GetComponent( "btnExit" );
	
	m_slotClickedHelp = m_pbtnHelp->SigClicked().Connect( this, &CMainMenuGui::OnClickedHelp );
	m_slotClickedSystem = m_pbtnSystem->SigClicked().Connect( this, &CMainMenuGui::OnClickedSystem );	
	m_slotClickedInven = m_pBtnInven->SigClicked().Connect( this, &CMainMenuGui::OnClickedInven );
	m_slotClickedQuest	= m_pbtnQuest->SigClicked().Connect( this, &CMainMenuGui::OnClickedQuest );
	m_slotClickedSkill	= m_pbtnSkill->SigClicked().Connect( this, &CMainMenuGui::OnClickedSkill );
	m_slotClickedStatus = m_pbtnStatus->SigClicked().Connect( this, &CMainMenuGui::OnClickedStatus );
	m_slotClickedCommunity = m_pbtnCommunity->SigClicked().Connect( this, &CMainMenuGui::OnClickedCommunity );
	m_slotClickedPartySearch = m_pbtnPartySearch->SigClicked().Connect(this, &CMainMenuGui::OnClickedPartySearch);
	m_slotClickedRankBoard = m_pbtnRankBoard->SigClicked().Connect( this, &CMainMenuGui::OnClickedRankBoard );
	m_slotClickedMascot = m_pbtnMascot->SigClicked().Connect(this, &CMainMenuGui::OnClickedMascot);
	m_slotClickedFriendList = m_pbtnFriendList->SigClicked().Connect( this, &CMainMenuGui::OnClickedFriendList );
	m_slotClickedChannelChange	= m_pbtnChannelChange->SigClicked().Connect( this, &CMainMenuGui::OnClickedChannelChange );
	m_slotClickedLogin	= m_pbtnLogin->SigClicked().Connect( this, &CMainMenuGui::OnClickedLogin );
	m_slotClickedExit	= m_pbtnExit->SigClicked().Connect( this, &CMainMenuGui::OnClickedExit );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CMainMenuGui::OnCaptureMouseDown );

	m_pbtnStatus->SetText(( GetDisplayStringManager()->GetString( "DST_MAINMENU_STATUS" ) ) );
	m_pBtnInven->SetText(( GetDisplayStringManager()->GetString( "DST_MAINMENU_INVEN" ) ) );
	m_pbtnSkill->SetText((GetDisplayStringManager()->GetString("DST_MAINMENU_SKILL")));
	m_pbtnQuest->SetText(( GetDisplayStringManager()->GetString( "DST_MAINMENU_QUEST" ) ) );
	m_pbtnFriendList->SetText((GetDisplayStringManager()->GetString("DST_MAINMENU_FRIENDLIST")));
	m_pbtnCommunity->SetText( ( GetDisplayStringManager()->GetString( "DST_MAINMENU_COMMUNITY" ) ) );
	m_pbtnPartySearch->SetText((GetDisplayStringManager()->GetString("DST_MAINMENU_PARTYMATCHINGLIST")));
	m_pbtnRankBoard->SetText( ( GetDisplayStringManager()->GetString( "DST_MAINMENU_RANKBOARD") ) );
	m_pbtnMascot->SetText((GetDisplayStringManager()->GetString("DST_MASCOT")));
	m_pbtnHelp->SetText((GetDisplayStringManager()->GetString("DST_MAINMENU_HELP")));
	m_pbtnSystem->SetText((GetDisplayStringManager()->GetString("DST_MAINMENU_OPTION")));
	m_pbtnChannelChange->SetText( ( GetDisplayStringManager()->GetString( "DST_MAINMENU_CHANNEL_CHANGE" ) ) );
	m_pbtnLogin->SetText( ( GetDisplayStringManager()->GetString( "DST_MAINMENU_END_SESSION" ) ) );
	m_pbtnExit->SetText( ( GetDisplayStringManager()->GetString( "DST_MAINMENU_END_GAME" ) ) );

	m_pThis->Show( false );
	m_pThis->SetPriority( dDIALOGPRIORITY_MAINMENU );

	NTL_RETURN(TRUE);
}

VOID CMainMenuGui::Destroy(VOID)
{
	NTL_FUNCTION("CMainMenuGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CMainMenuGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_MAINMENU_OPEN);

	Show( bOpen );
	
	return 1;
}

VOID CMainMenuGui::OnClickedHelp( gui::CComponent* pComponent ) 
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_HELPWND) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_HELPWND );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_HELPWND );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedSystem( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_OPTIONWND) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_OPTIONWND );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_OPTIONWND );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedQuest( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_QUESTLIST) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_QUESTLIST );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_QUEST_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_QUESTLIST );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedSkill( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_SKILL) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_SKILL );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_SKILL_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_SKILL );
	}
	
	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );

	//if( GetDialogManager()->IsOpenDialog(DIALOG_SKILL_TRAINER) )
	//{
	//	const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_SKILL_USING_TRINER_DIALOG);
	//	CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, L"", (int)wcslen(pwcText), pwcText);
	//	return;
	//}  
}

VOID CMainMenuGui::OnClickedStatus( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_STATUS) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_STATUS );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_STATUS_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_STATUS );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedCommunity( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_COMMUNITY) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_COMMUNITY );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_COMMUNITY_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_COMMUNITY );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedPartySearch(gui::CComponent * pComponent)
{
	return VOID();
}

VOID CMainMenuGui::OnClickedRankBoard( gui::CComponent* pComponent ) 
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_RANKBOARD) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_RANKBOARD );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_RANKBOARD_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_RANKBOARD );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedMascot(gui::CComponent * pComponent)
{
	return VOID();
}

VOID CMainMenuGui::OnClickedFriendList( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog(DIALOG_FRIEND_LIST) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_CLOSE ) )
			return;

		GetDialogManager()->CloseDialog( DIALOG_FRIEND_LIST );
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_FRIEND_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_FRIEND_LIST );
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedChannelChange( gui::CComponent* pComponent )
{
	if( GetNtlWorldConcept()->IsActivePlayGrade(WORLD_CONCEPT_FIRST_GRADE) )
	{
		Logic_WarnningWorldConceptActionMessage(WORLD_CONCEPT_FIRST_GRADE);
		return;
	}

	GetDialogManager()->SwitchDialog(DIALOG_CHANNGEL_CHANGE);
	GetDialogManager()->CloseDialog( DIALOG_MAINMENU );
}

VOID CMainMenuGui::OnClickedLogin( gui::CComponent* pComponent )
{
	if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_CCBD))
	{
		Logic_WarnningWorldConceptActionMessage(WORLD_CONCEPT_FIRST_GRADE);
		return;
	}

	GetAlarmManager()->AlarmMessage( "DST_CHAR_SERVER_EXIT" );

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnClickedExit( gui::CComponent* pComponent )
{
	if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_CCBD))
	{
		CNtlCCBD* pCCBD = GetNtlSLGlobal()->GetSobAvatar()->GetCCBD();

		GetAlarmManager()->FormattedAlarmMessage("DST_CCBD_LEAVE_CCBD", FALSE, NULL, pCCBD->GetStage());
		return;
	}

	GetAlarmManager()->AlarmMessage( "DST_GAME_EXIT" );

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}

VOID CMainMenuGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_MAINMENU, key.m_fX, key.m_fY );
}

VOID CMainMenuGui::OnClickedInven( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsBagOpen() )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_CLOSE ) )
			return;

		GetDialogManager()->SwitchBag(false);
	}
	else
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CAPSULE_KIT_OPEN ) )
			return;

		RwBool bAllClosedCapsuleClosed = GetDialogManager()->IsBagOpen();

		// 가방이 한개라도 열려 있다면 전부 닫는다.
		GetDialogManager()->SwitchBag(!bAllClosedCapsuleClosed);
	}

	if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
		GetDialogManager()->CloseDialog( DIALOG_MAINMENU, FALSE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////

CMainMenuButtonGui::CMainMenuButtonGui(VOID)
: m_pbtnMain( NULL )
{

}

CMainMenuButtonGui::CMainMenuButtonGui( const RwChar *pName )
: CNtlPLGui( pName ), m_pbtnMain( NULL )
{
	
}

CMainMenuButtonGui::~CMainMenuButtonGui(VOID)
{

}

RwBool CMainMenuButtonGui::Create(VOID)
{
	NTL_FUNCTION("CMainMenuGui::Create");

	if(!CNtlPLGui::Create("", "gui\\MainBtn.srf", "gui\\MainMenuButton.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pbtnMain = (gui::CButton*)GetComponent( "btnMain" );
	m_pbtnMain->SetToolTip( GetDisplayStringManager()->GetString( "DST_MAINMENU_TOOLTIP" ) );
	m_slotClickedMain = m_pbtnMain->SigClicked().Connect( this, &CMainMenuButtonGui::OnClickedMain );
	m_slotPaint = m_pbtnMain->SigPaint().Connect( this, &CMainMenuButtonGui::OnPaint );

	m_pThis->Show( true );
	m_pThis->SetPriority( dDIALOGPRIORITY_MAINMENU );

	NTL_RETURN(TRUE);
}

VOID CMainMenuButtonGui::Destroy(VOID)
{
	NTL_FUNCTION("CMainMenuButtonGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CMainMenuButtonGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	return 1;
}

VOID CMainMenuButtonGui::OnPaint(VOID)
{
}

VOID CMainMenuButtonGui::OnClickedMain( gui::CComponent* pComponent )
{
	if( !GetDialogManager()->IsOpenDialog( DIALOG_MAINMENU ) )
	{
		GetDialogManager()->GetDialog( DIALOG_MAINMENU )->RaiseTop();
	}
	
	if( GetDialogManager()->IsOpenDialog( DIALOG_MAINMENU ) )
	{
		if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE ) )
			GetDialogManager()->CloseDialog( DIALOG_MAINMENU );
	}
	else
	{
		if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPEN ) )
			GetDialogManager()->OpenDialog( DIALOG_MAINMENU );
	}	
}