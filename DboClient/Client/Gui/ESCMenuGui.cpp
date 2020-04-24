#include "precomp_dboclient.h"
#include "ESCMenuGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "DboTLDefine.h"
#include "NtlWorldConcept.h"
#include "NtlSLLogic.h"

// client
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "DboLogic.h"

CESCMenuGui::CESCMenuGui( const RwChar* pName )
:CNtlPLGui( pName )
{

}

CESCMenuGui::~CESCMenuGui(VOID)
{

}

RwBool CESCMenuGui::Create(VOID)
{
	NTL_FUNCTION( "CESCMenuGui::Create" );

	if(! CNtlPLGui::Create( "", "gui\\ESCMenu.srf", "gui\\ESCMenu.frm" ) )
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );	
	m_pbtnHelp = reinterpret_cast<gui::CButton*>( GetComponent( "btnHelp" ) );
	m_pbtnOption = reinterpret_cast<gui::CButton*>( GetComponent( "btnSystem" ) );
	m_pbtnChangeChannel = reinterpret_cast<gui::CButton*>( GetComponent( "btnChannelChange" ) );
	m_pbtnLogOut = reinterpret_cast<gui::CButton*>( GetComponent( "btnLogin" ) );
	m_pbtnGameExit = reinterpret_cast<gui::CButton*>( GetComponent( "btnExit" ) );	

	m_pThis->SetPriority( dDIALOGPRIORITY_ESCMENU );

	m_pbtnHelp->SetText( std::wstring( GetDisplayStringManager()->GetString( "DST_MAINMENU_HELP" ) ) );
	m_pbtnOption->SetText( std::wstring( GetDisplayStringManager()->GetString( "DST_MAINMENU_OPTION" ) ) );
	m_pbtnChangeChannel->SetText( std::wstring( GetDisplayStringManager()->GetString( "DST_MAINMENU_CHANNEL_CHANGE" ) ) );
	m_pbtnLogOut->SetText( std::wstring( GetDisplayStringManager()->GetString( "DST_MAINMENU_END_SESSION" ) ) );
	m_pbtnGameExit->SetText( std::wstring( GetDisplayStringManager()->GetString( "DST_MAINMENU_END_GAME" ) ) );

	m_slotClickHelp = m_pbtnHelp->SigClicked().Connect( this, &CESCMenuGui::OnClickedHelp );
	m_slotClickOption = m_pbtnOption->SigClicked().Connect( this, &CESCMenuGui::OnClickedOption );
	m_slotClickChangeChannel = m_pbtnChangeChannel->SigClicked().Connect( this, &CESCMenuGui::OnClickedChangeChannel );
	m_slotClickLogOut = m_pbtnLogOut->SigClicked().Connect( this, &CESCMenuGui::OnClickedLogOut );
	m_slotClickGameExit = m_pbtnGameExit->SigClicked().Connect( this, &CESCMenuGui::OnClickedGameExit );

	Show( false );

	LinkMsg( g_EventResize, 0, 0x7000 );

	NTL_RETURN(TRUE);
}

VOID CESCMenuGui::Destroy(VOID)
{
	NTL_FUNCTION("CESCMenuGui::Destroy");

	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CESCMenuGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
		CenterAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
		
	Show( bOpen );
	return 1;
}

VOID CESCMenuGui::HandleEvents( RWS::CMsg &msg )
{
	 if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize *pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );

		CenterAlign( pData->iWidht, pData->iHeight );	
	}
}

VOID CESCMenuGui::OnClickedHelp( gui::CComponent* pComponent )
{
	if( !GetDialogManager()->IsOpenDialog( DIALOG_HELPWND ) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_HELP_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_HELPWND );
	}	

	GetDialogManager()->CloseDialog( DIALOG_ESCMENU );
}

VOID CESCMenuGui::OnClickedOption( gui::CComponent* pComponent )
{
	if( !GetDialogManager()->IsOpenDialog(DIALOG_OPTIONWND) )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_MAINMENU_OPTION_OPEN ) )
			return;

		GetDialogManager()->OpenDialog( DIALOG_OPTIONWND );
	}

	GetDialogManager()->CloseDialog( DIALOG_ESCMENU );
}

VOID CESCMenuGui::OnClickedChangeChannel( gui::CComponent* pComponent )
{
	if (GetNtlWorldConcept()->IsActivePlayGrade(WORLD_CONCEPT_FIRST_GRADE))
	{
		Logic_WarnningWorldConceptActionMessage(WORLD_CONCEPT_FIRST_GRADE);
		return;
	}

	GetDialogManager()->OpenDialog( DIALOG_CHANNGEL_CHANGE );
	GetDialogManager()->CloseDialog( DIALOG_ESCMENU );
}

VOID CESCMenuGui::OnClickedLogOut( gui::CComponent* pComponent )
{
	GetAlarmManager()->AlarmMessage("DST_CHAR_SERVER_EXIT");

	if (Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE))
	{
		GetDialogManager()->CloseDialog(DIALOG_ESCMENU, FALSE);
	}
}

VOID CESCMenuGui::OnClickedGameExit( gui::CComponent* pComponent )
{
	GetAlarmManager()->AlarmMessage("DST_GAME_EXIT");

	if (Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_MAINMENU_CLOSE))
	{
		GetDialogManager()->CloseDialog(DIALOG_ESCMENU, FALSE);
	}
}
