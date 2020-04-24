#include "precomp_dboclient.h"
#include "ActionNotifyGui.h"

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
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogPriority.h"

#define ACTION_NOTIFY_MAX_ALPHA		255
#define ACTION_NOTIFY_MIN_ALPHA		0
#define ACTION_NOTIFY_TIME_FADEIN	1.0f
#define ACTION_NOTIFY_TIME_SHOW		4.0f
#define ACTION_NOTIFY_TIME_FADEOUT	1.0f
#define ACTION_NOTIFY_TIME_MOVE		0.5f
#define ACTION_NOTIFY_YPOS_LINEDIFF	17

//#define ACTION_NOTIFY_YPOS_RATE		0.162f
#define ACTION_NOTIFY_YPOS_RATE		0.215f

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CActionNotifyItem
CActionNotifyItem::CActionNotifyItem(VOID)
{
	m_pstbActionText = NULL;

	m_fCurrentFadeTime = 0.0f;
	m_fCurrentMoveTime = 0.0f;
	m_nLine = 0;
	m_nOldYPos = 0;
	m_eFadeState = NONE;
}

CActionNotifyItem::~CActionNotifyItem(VOID)
{

}

VOID CActionNotifyItem::SetStaticBox( gui::CStaticBox* pStaticBox )
{
	m_pstbActionText = pStaticBox;
	m_pstbActionText->Show( false );
}

RwBool CActionNotifyItem::Update( RwReal fElapsedTime )
{
	if( m_eFadeState != NONE )
	{
		m_fCurrentFadeTime += fElapsedTime;

		if( m_eFadeState == FADE_IN )
		{
			if( m_fCurrentFadeTime >= ACTION_NOTIFY_TIME_FADEIN )
			{
				m_pstbActionText->SetAlpha( ACTION_NOTIFY_MAX_ALPHA );
				SetFadeState( SHOW );
			}
			else
			{
				RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fCurrentFadeTime, ACTION_NOTIFY_TIME_FADEIN, ACTION_NOTIFY_MIN_ALPHA, ACTION_NOTIFY_MAX_ALPHA ) );
				m_pstbActionText->SetAlpha( byAlpha );
			}
		}
		else if( m_eFadeState == FADE_OUT )
		{
			if( m_fCurrentFadeTime >= ACTION_NOTIFY_TIME_FADEIN )
			{
				m_pstbActionText->SetAlpha( ACTION_NOTIFY_MIN_ALPHA );
				m_pstbActionText->Show( false );
				SetFadeState( NONE );

				return FALSE;
			}
			else
			{
				RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fCurrentFadeTime, ACTION_NOTIFY_TIME_FADEOUT, ACTION_NOTIFY_MAX_ALPHA, ACTION_NOTIFY_MIN_ALPHA ) );
				m_pstbActionText->SetAlpha( byAlpha );
			}
		}
		else // SHOW
		{
			if( m_fCurrentFadeTime >= ACTION_NOTIFY_TIME_SHOW )
			{
				SetFadeState( FADE_OUT );
			}
		}

		if( m_eMoveState != STOP )
		{
			m_fCurrentMoveTime += fElapsedTime;

			if( m_fCurrentMoveTime >= ACTION_NOTIFY_TIME_MOVE )
			{
				m_nOldYPos = GetYPos( m_nLine );
				m_pstbActionText->SetPosition( 0, m_nOldYPos );
				m_eMoveState = STOP;				
			}
			else
			{
				RwInt32 nYPos = (RwInt32)( gui::GetResultLineInterpolation( m_fCurrentMoveTime, ACTION_NOTIFY_TIME_MOVE, (RwReal)m_nOldYPos, (RwReal)m_nNewYPos ) );
				m_pstbActionText->SetPosition( 0, nYPos );
			}		
		}
	}

	return TRUE;
}

VOID CActionNotifyItem::SetText( WCHAR* pText, INT nLine )
{
	m_pstbActionText->SetText( pText );
	
	// Fade Setting
	SetFadeState( FADE_IN );
	m_pstbActionText->Show( true );
	
	// Line Setting
	m_nLine = nLine;
	m_nOldYPos = m_nNewYPos = GetYPos( nLine );

	m_pstbActionText->SetPosition( 0, m_nNewYPos );
}

VOID CActionNotifyItem::Move( RwUInt8 ucLine )
{
	if( m_eMoveState == MOVE )
		m_nOldYPos = m_pstbActionText->GetPosition().top;		
	else
		m_nOldYPos = GetYPos( m_nLine );
	
	m_nNewYPos = GetYPos( ucLine );
	m_nLine = ucLine;	
	m_eMoveState = MOVE;

	m_fCurrentMoveTime = 0.0f;
}

VOID CActionNotifyItem::SetFadeState( RwInt32 nFadeState )
{
	m_eFadeState = nFadeState;
	m_fCurrentFadeTime = 0.0f;
}

RwInt32 CActionNotifyItem::GetYPos( RwInt32 nLine )
{
	return ACTION_NOTIFY_YPOS_LINEDIFF * nLine;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CActionNotifyGui

CActionNotifyGui::CActionNotifyGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	
}

CActionNotifyGui::~CActionNotifyGui(VOID)
{

}

RwBool CActionNotifyGui::Create(VOID)
{
	NTL_FUNCTION( "CActionNotifyGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\ActionNotify.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_ActionNotifyItem[0].SetStaticBox( (gui::CStaticBox*)GetComponent( "stbText1" ) );
	m_ActionNotifyItem[1].SetStaticBox( (gui::CStaticBox*)GetComponent( "stbText2" ) );
	m_ActionNotifyItem[2].SetStaticBox( (gui::CStaticBox*)GetComponent( "stbText3" ) );

	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );	

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg( g_EventNotify );
	LinkMsg( g_EventResize );

	NTL_RETURN(TRUE);
}

VOID CActionNotifyGui::Destroy(VOID)
{
	NTL_FUNCTION( "ActionNotifyGui::Destroy" );

	UnLinkMsg( g_EventNotify );
	UnLinkMsg( g_EventResize );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 
}

VOID CActionNotifyGui::Update( RwReal fElapsed )
{
	for( RwInt32 i = 0 ; i < MAXLINE ; ++i )
	{
		if( !m_ActionNotifyItem[i].Update( fElapsed ) )
		{
			RwInt32 nDeletedLine = m_ActionNotifyItem[i].GetLine();
			for( RwInt32 j = 0 ; j < MAXLINE ; ++j )
			{
				RwInt32 nCurrentLine = m_ActionNotifyItem[j].GetLine();
				if( nCurrentLine > nDeletedLine )
					m_ActionNotifyItem[j].Move((RwUInt8)(nCurrentLine - 1) );
			}
		}
	}
}

VOID CActionNotifyGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventNotify )
	{
		SDboEventNotify* pData = reinterpret_cast<SDboEventNotify*>( msg.pData );

		if( pData->eType == SDboEventNotify::ACTION )
		{
			RwInt32 nEmptyLine = 0;
			RwInt32 nLineCount = 0;

			for( RwInt32 i = 0 ; i < MAXLINE ; ++i )
			{
				if( m_ActionNotifyItem[i].GetFadeState() == CActionNotifyItem::NONE )
					nEmptyLine = i;							
				else
					++nLineCount;
			}

			if( nLineCount == MAXLINE )	// 꽉차있는 경우
			{
				for( RwInt32 i = 0 ; i < MAXLINE ; ++i )
				{
					RwInt32 nLine = m_ActionNotifyItem[i].GetLine();

					if( nLine == 0 )
						m_ActionNotifyItem[i].SetText( pData->wchMessage, nLineCount - 1 );
					else
						m_ActionNotifyItem[i].Move((RwUInt8)( nLine - 1) );				
				}
			}
			else
			{
				m_ActionNotifyItem[nEmptyLine].SetText( pData->wchMessage, nLineCount );
			}

			Logic_PlayGUISound(GSD_SYSTEM_ACTION_NOTIFY);		

		}		
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign( pPacket->iWidht, pPacket->iHeight );
	}
}

VOID CActionNotifyGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	m_pThis->SetPosition( (RwInt32)( ( nScreenWidth - m_pThis->GetWidth() ) / 2 ), (RwInt32)( nScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fActionNotify_YPosRate ) );		
}
