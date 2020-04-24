#include "precomp_dboclient.h"
#include "ResultNarrationGui.h"

#include "NtlDebug.h"

#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

#include "DboGlobal.h"
#include "DialogPriority.h"

CResultNarrationGui::CResultNarrationGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	
}

CResultNarrationGui::~CResultNarrationGui(VOID)
{

}

RwBool CResultNarrationGui::Create(VOID)
{
	NTL_FUNCTION( "CResultNarrationGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\ResultNarration.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority( dDIALOGPRIORITY_RESULT_NARRATION );

	m_phbxMessage = (gui::CHtmlBox*)GetComponent( "hbxMessage" );
	m_pflaDirection = (gui::CFlash*)GetComponent( "flaDirection" );

	m_slotFSCallBack = m_pflaDirection->SigFSCallBack().Connect( this, &CResultNarrationGui::OnFSCallBack );

	CNtlPLGui::Show( false );

	LinkMsg( g_EventResize );

	NTL_RETURN( TRUE );
}

VOID CResultNarrationGui::Destroy(VOID)
{
	// 혹시나...
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CResultNarrationGui::Update( RwReal fElapsed )
{
	m_pflaDirection->Update( fElapsed );
}

VOID CResultNarrationGui::Show( bool bShow )
{
	if( bShow )
	{
		if( !IsShow() )
		{
			CalcSize( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
			GetNtlGuiManager()->AddUpdateFunc( this );			
		}
	}
	else
	{
		if( IsShow() )
			GetNtlGuiManager()->RemoveUpdateFunc( this );
	}

	CNtlPLGui::Show( bShow );	
}

VOID CResultNarrationGui::SetData( const RwChar* szFileName, const WCHAR* szMessage )
{
	// Peessi: Resource문제에 따른 임시대응.
	if( strcmp( szFileName, "TB_MatchFinish_npc.swf" ) == 0 )
		m_phbxMessage->SetPosition( 170, 580 );
	else
		m_phbxMessage->SetPosition( 141, 580 );

	m_pflaDirection->Load( szFileName, TRUE );
	m_pflaDirection->PlayMovie( TRUE );

	if( szMessage )
		m_phbxMessage->SetHtmlFromMemory( szMessage, wcslen( szMessage ) );

	m_phbxMessage->Show( false );

	Show( true );
}

VOID CResultNarrationGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		CalcSize( pData->iWidht, pData->iHeight );
	}
}

VOID CResultNarrationGui::CalcSize( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	RwInt32 nX = ( nScreenWidth - GetWidth() ) / 2;
	RwInt32 nY = nScreenHeight - GetHeight();

	m_pThis->SetPosition( nX, nY );
}

VOID CResultNarrationGui::OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs )
{
	if( !strcmp( szCommand, "TextEnd1" ) )
	{
		m_phbxMessage->Show( true );
	}
}