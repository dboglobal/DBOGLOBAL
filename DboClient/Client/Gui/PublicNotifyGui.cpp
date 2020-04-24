#include "precomp_dboclient.h"
#include "PublicNotifyGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// table
#include "TableContainer.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"

// Dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DialogPriority.h"

#define PUBLIC_NOTIFY_MAX_ALPHA		255
#define PUBLIC_NOTIFY_MIN_ALPHA		0
#define PUBLIC_NOTIFY_TIME_FADEIN	1.0f
#define PUBLIC_NOTIFY_TIME_SHOW		4.0f
#define PUBLIC_NOTIFY_TIME_FADEOUT	1.0f
#define PUBLIC_NOTIFY_YPOS_RATE		0.123f

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPublicNotifyGui
CPublicNotifyGui::CPublicNotifyGui(VOID)
{
	Init();
}

CPublicNotifyGui::CPublicNotifyGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CPublicNotifyGui::~CPublicNotifyGui(VOID)
{

}

VOID CPublicNotifyGui::Init(VOID)
{
	m_fPublicCurrentTime = 0.0f;
	m_bPublicNotify = FALSE;
	
	SetPublicUpdateState( NONE );
}

RwBool CPublicNotifyGui::Create(VOID)
{
	NTL_FUNCTION( "CPublicNotifyGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\PublicNotify.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	m_pstbPublicNotifyText = (gui::CStaticBox*)GetComponent( "stbPublic" );
	
	m_pstbPublicNotifyText->Show( false );

	PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	
	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventNotify );
	LinkMsg( g_EventResize );

	NTL_RETURN(TRUE);
}

VOID CPublicNotifyGui::Destroy(VOID)
{
	NTL_FUNCTION( "PublicNotifyGui::Destroy" );

	UnLinkMsg( g_EventNotify );
	UnLinkMsg( g_EventResize );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPublicNotifyGui::Update( RwReal fElapsed )
{
	if( m_bPublicNotify )
		PublicUpdateProc( fElapsed );
}

VOID CPublicNotifyGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventNotify )
	{
		SDboEventNotify* pData = reinterpret_cast<SDboEventNotify*>( msg.pData );

		if( pData->eType == SDboEventNotify::PUBLIC )
		{			
			m_pstbPublicNotifyText->SetText( pData->wchMessage );
			m_pstbPublicNotifyText->Show( true );
			SetPublicUpdateState( FADE_IN );

			Logic_PlayGUISound(GSD_SYSTEM_PUBLIC_NOTIFY);					
		}		
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign( pPacket->iWidht, pPacket->iHeight );		
	}
}

VOID CPublicNotifyGui::PublicUpdateProc( RwReal fElapsed )
{
	m_fPublicCurrentTime += fElapsed;

	(this->*StatePublicUpdate)();	
}

VOID CPublicNotifyGui::PublicUpdate_None(VOID)
{
	
}

VOID CPublicNotifyGui::PublicUpdate_FadeIn(VOID)
{
	if( m_fPublicCurrentTime >= PUBLIC_NOTIFY_TIME_FADEIN )
	{
		m_pstbPublicNotifyText->SetAlpha( PUBLIC_NOTIFY_MAX_ALPHA );
		SetPublicUpdateState( SHOW );
	}
	else
	{
		RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fPublicCurrentTime, PUBLIC_NOTIFY_TIME_FADEIN, PUBLIC_NOTIFY_MIN_ALPHA, PUBLIC_NOTIFY_MAX_ALPHA ) );
		m_pstbPublicNotifyText->SetAlpha( byAlpha );
	}		
}

VOID CPublicNotifyGui::PublicUpdate_Show(VOID)
{
	if( m_fPublicCurrentTime >= PUBLIC_NOTIFY_TIME_SHOW )
	{
		SetPublicUpdateState( FADE_OUT );
	}
}

VOID CPublicNotifyGui::PublicUpdate_FadeOut(VOID)
{
	if( m_fPublicCurrentTime >= PUBLIC_NOTIFY_TIME_FADEOUT )
	{
		m_pstbPublicNotifyText->SetAlpha( PUBLIC_NOTIFY_MIN_ALPHA );
		m_pstbPublicNotifyText->Show( false );
		SetPublicUpdateState( NONE );		
	}
	else
	{
		RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fPublicCurrentTime, PUBLIC_NOTIFY_TIME_FADEOUT, PUBLIC_NOTIFY_MAX_ALPHA, PUBLIC_NOTIFY_MIN_ALPHA ) );
		m_pstbPublicNotifyText->SetAlpha( byAlpha );
	}	
}

VOID CPublicNotifyGui::SetPublicUpdateState( STATE eState )
{
	m_ePublicState = eState;
	m_fPublicCurrentTime = 0.0f;

	switch( eState )
	{
	case NONE:
		m_bPublicNotify = FALSE;
		StatePublicUpdate = &CPublicNotifyGui::PublicUpdate_None; break;
	case FADE_IN:
		m_bPublicNotify = TRUE;
		StatePublicUpdate = &CPublicNotifyGui::PublicUpdate_FadeIn; break;
	case SHOW:
		StatePublicUpdate = &CPublicNotifyGui::PublicUpdate_Show; break;
	case FADE_OUT:
		StatePublicUpdate = &CPublicNotifyGui::PublicUpdate_FadeOut; break;
	}
}

VOID CPublicNotifyGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	m_pThis->SetPosition( (RwInt32)( ( nScreenWidth - m_pThis->GetWidth() ) / 2 ), (RwInt32)( nScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fPublicNotify_YPosRate ) );		
}