// Precompile
#include "stdafx.h"
#include "PathPoint.h"

// Core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

// Editor
#include "2DParticleGlobal.h"

#include "2DParticleApp.h"
#include "PathManager.h"


CPathPoint::CPathPoint()
: m_pPanPoint( NULL )
, m_pStbNum(NULL)
, m_fTime(0.0f)
{
	memset( &m_Pos, 0, sizeof( CPos ) );
	memset( m_atcToolTip, 0, sizeof( TCHAR ) * dPATH_POINT_TOOLTIP_SIZE );
	m_nIndex = 0;
}

CPathPoint::CPathPoint( int nX, int nY )
: m_pPanPoint( NULL )
, m_pStbNum(NULL)
, m_fTime(0.0f)
{
	m_Pos.x = nX;
	m_Pos.y = nY;
	memset( m_atcToolTip, 0, sizeof( TCHAR ) * dPATH_POINT_TOOLTIP_SIZE );
	SetPosition( m_Pos );
	m_nIndex = 0;
}

CPathPoint::CPathPoint( CPos& pos )
: m_pPanPoint( NULL )
, m_pStbNum(NULL)
{
	m_Pos = pos;
	memset( m_atcToolTip, 0, sizeof( TCHAR ) * dPATH_POINT_TOOLTIP_SIZE );
	SetPosition( m_Pos );
	m_nIndex = 0;
}

CPathPoint::~CPathPoint()
{
	Destroy();
}

void CPathPoint::SetPosition( CPos pos )
{
	m_Pos = pos;

	int nNewX = m_Pos.x - ( m_pPanPoint->GetWidth()/2 );
	int nNewY = m_Pos.y - ( m_pPanPoint->GetHeight()/2 );

	if( m_pPanPoint )
		m_pPanPoint->SetPosition(  nNewX, nNewY );

	nNewX = m_Pos.x - ( m_pStbNum->GetWidth()/2 );
	nNewY = m_Pos.y - ( m_pStbNum->GetHeight()/2 );

	if( m_pStbNum )
		m_pStbNum->SetPosition( nNewX, nNewY );

	if( m_byState == STATE_FOCUS || m_byState == STATE_DOWN )
	{
		UpdateToolTip();
	}
}

void CPathPoint::SetNumber( int nNumber )
{
	m_nIndex = nNumber;
	
	if( m_pStbNum )
		m_pStbNum->SetText( m_nIndex );
}

void CPathPoint::ClearNumber()
{
	if( m_pStbNum )
		m_pStbNum->Clear();
}

void CPathPoint::Create(int nX, int nY)
{
	Destroy();

	m_Pos.x = nX;
	m_Pos.y = nY;

	CRectangle rect;
	rect.SetRectWH( nX, nY, dPATH_POINT_WIDTH, dPATH_POINT_HEIGHT );
	m_pPanPoint = new gui::CPanel( &rect, g_pGuiMgr, NULL );

	rect.SetRectWH( nX, nY, dPATH_POINT_STB_WIDTH, dPATH_POINT_STB_HEIGHT );
	m_pStbNum = new gui::CStaticBox( &rect, g_pGuiMgr, NULL );
	m_pStbNum->CreateFontStd( DETAIL_FONT, dPATH_POINT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pStbNum->SetTextStyle( COMP_TEXT_VERTICAL_CENTER | COMP_TEXT_CENTER );
	m_pStbNum->SetEffectMode(TE_SHADOW);
	m_pStbNum->SetEffectColor(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pStbNum->Enable( false );

	gui::CSurface surface;
	::ZeroMemory( &surface, sizeof( gui::CSurface ) );

	surface.m_Original.rtRect.SetRectWH( 0, 0, dPATH_POINT_WIDTH, dPATH_POINT_HEIGHT );
	surface.m_Original.uRed = 255;
	surface.m_Original.uGreen = 255;
	surface.m_Original.uBlue = 255;
	surface.m_Original.uAlpha = 255;
	surface.m_Original.UVs[0] = 0.0f;
	surface.m_Original.UVs[1] = 0.0f;
	surface.m_Original.UVs[2] = 1.0f;
	surface.m_Original.UVs[3] = 1.0f;
	surface.m_Original.uBlend = 0;

	surface.m_SnapShot.rtRect.SetRectWH( 0, 0, dPATH_POINT_WIDTH, dPATH_POINT_HEIGHT );
	surface.m_SnapShot.uRed = 255;
	surface.m_SnapShot.uGreen = 0;
	surface.m_SnapShot.uBlue = 0;
	surface.m_SnapShot.uAlpha = 128;
	surface.m_SnapShot.UVs[0] = 0.0f;
	surface.m_SnapShot.UVs[1] = 0.0f;
	surface.m_SnapShot.UVs[2] = 1.0f;
	surface.m_SnapShot.UVs[3] = 1.0f;
	surface.m_SnapShot.uBlend = 0;

	m_pPanPoint->AddSurface( surface );

	m_slotMouseEnter = m_pPanPoint->SigMouseEnter().Connect( this, &CPathPoint::OnMouseEnter );
	m_slotMouseLeave = m_pPanPoint->SigMouseLeave().Connect( this, &CPathPoint::OnMouseLeave );
	m_slotMouseMove = m_pPanPoint->SigMouseMove().Connect( this, &CPathPoint::OnMouseMove );
	m_slotMouseUp	= m_pPanPoint->SigMouseUp().Connect( this, &CPathPoint::OnMouseUp );
	m_slotMouseDown	= m_pPanPoint->SigMouseDown().Connect( this, &CPathPoint::OnMouseDown );

	m_pPanPoint->Show( true );
	m_pStbNum->Show( true );
}

void CPathPoint::Destroy()
{
	if( m_pPanPoint )
	{
		delete m_pPanPoint;
		m_pPanPoint = NULL;
	}

	if( m_pStbNum )
	{
		delete m_pStbNum;
		m_pStbNum = NULL;
	}
}

void CPathPoint::SetColor( unsigned char uRed, unsigned char uGreen, unsigned char uBlue )
{
	m_pPanPoint->SetColor( uRed, uGreen, uBlue );
}

void CPathPoint::SetAlpha( unsigned char uAlpha )
{
	m_pPanPoint->SetAlpha( uAlpha );
}

void CPathPoint::OnMouseEnter( gui::CComponent* pComponent )
{
	if( pComponent == m_pPanPoint )
	{
		if( m_byState != STATE_DOWN )
			SetState( STATE_FOCUS );
	}
}

void CPathPoint::OnMouseMove( int nFlags, int nX, int nY )
{
	if( m_byState == STATE_DOWN )
	{
		CRectangle rect = m_pPanPoint->GetScreenRect();

		SetPosition( CPos( rect.left + nX, rect.top + nY ) );
	}
}

void CPathPoint::OnMouseUp( const CKey& key )
{
	if( m_byState == STATE_DOWN )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			CPathManager::GetInstance()->UpdateCurrentPathData();
			CPathManager::GetInstance()->SaveCurrentPathData();
		
			SetState( STATE_FOCUS );
			m_pPanPoint->ReleaseMouse();
		}
	}
}

void CPathPoint::OnMouseDown( const CKey& key )
{
	if( m_byState == STATE_FOCUS )
	{
		if( key.m_nID == UD_LEFT_BUTTON )
		{
			SetState( STATE_DOWN );
			m_pPanPoint->CaptureMouse();
		}
	}
}

void CPathPoint::OnMouseLeave( gui::CComponent* pComponent )
{
	if( pComponent == m_pPanPoint )
	{
		if( m_byState != STATE_DOWN )
			SetState( STATE_NORMAL );
	}
}

void CPathPoint::SetState( unsigned char byState )
{
	m_byState = byState;

	switch( byState )
	{
	case STATE_NORMAL:
		SetColor( 255, 0, 0 );
		UpdateToolTip( FALSE );
		break;
	case STATE_FOCUS:
		SetColor( 255, 128, 128 );
		UpdateToolTip();
		break;
	case STATE_DOWN:
		SetColor( 255, 255, 255 );
		break;
	}
}

CPos CPathPoint::GetPosition()
{
	return m_Pos;
}

void CPathPoint::SetTime( float fTime )
{
	m_fTime = fTime;
}

float CPathPoint::GetTime()
{
	return m_fTime;
}

void CPathPoint::UpdateToolTip( BOOL bShow /* = TRUE */ )
{
	_stprintf_s( m_atcToolTip, dPATH_POINT_TOOLTIP_SIZE, _T("I:%d\\nX:%d Y:%d\\nT:%0.3f"), m_nIndex,
		m_Pos.x - GetParticleApp()->GetPBOffsetX(), 
		m_Pos.y - GetParticleApp()->GetPBOffsetY(), m_fTime );

	CPathManager::GetInstance()->ShowToolTip( m_Pos.x,
		m_Pos.y, m_atcToolTip, bShow );
}