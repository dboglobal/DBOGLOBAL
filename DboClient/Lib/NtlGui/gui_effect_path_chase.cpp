#include "gui_precomp.h"
/*
#include "gui_effect_path_chase.h"

#include "gui_renderer.h"
#include "GuiUtil.h"

START_GUI

GUI_DEFINITION_MEMORY_POOL( CPathInfo_Pos )
GUI_DEFINITION_MEMORY_POOL( CPathInfo_PosAlpha )
GUI_DEFINITION_MEMORY_POOL( CPathParticle )

///////////////////////////////////////////////////////////////////////////////////////////
// CPathParticle 

VOID CPathInfo_Pos::Update( CPathParticle* pParticle )
{
	CPos&	posParent = pParticle->GetPos();
	CPos	posInterpolate;

	INT		nHalfWidth = pParticle->m_SnapShot.rtRect.GetWidth() / 2;
	INT		nHalfHeight= pParticle->m_SnapShot.rtRect.GetHeight() / 2;

	posInterpolate.x = (INT)( GetResultLineInterpolation( pParticle->GetTime(), m_fPathTime, (FLOAT)m_posSrcCenter.x, (FLOAT)m_posDestCenter.x ) );
	posInterpolate.y = (INT)( GetResultLineInterpolation( pParticle->GetTime(), m_fPathTime, (FLOAT)m_posSrcCenter.y, (FLOAT)m_posDestCenter.y ) );

	pParticle->m_SnapShot.rtRect.left	= posParent.x + posInterpolate.x - nHalfWidth;
	pParticle->m_SnapShot.rtRect.top	= posParent.y + posInterpolate.y - nHalfHeight;
	pParticle->m_SnapShot.rtRect.right	= posParent.x + posInterpolate.x + nHalfWidth;
	pParticle->m_SnapShot.rtRect.bottom	= posParent.y + posInterpolate.y + nHalfHeight;
}

VOID CPathInfo_PosAlpha::Update( CPathParticle* pParticle )
{
	CPos&	posParent = pParticle->GetPos();
	CPos	posInterpolate;

	INT		nHalfWidth = pParticle->m_SnapShot.rtRect.GetWidth() / 2;
	INT		nHalfHeight= pParticle->m_SnapShot.rtRect.GetHeight() / 2;

	posInterpolate.x = (INT)( GetResultLineInterpolation( pParticle->GetTime(), m_fPathTime, (FLOAT)m_posSrcCenter.x, (FLOAT)m_posDestCenter.x ) );
	posInterpolate.y = (INT)( GetResultLineInterpolation( pParticle->GetTime(), m_fPathTime, (FLOAT)m_posSrcCenter.y, (FLOAT)m_posDestCenter.y ) );
	BYTE ucAlpha = (BYTE)( GetResultLineInterpolation( pParticle->GetTime(), m_fPathTime, (FLOAT)m_ucSrcAlpha, (FLOAT)m_ucDestAlpha ) );

	pParticle->m_SnapShot.rtRect.left	= posParent.x + posInterpolate.x - nHalfWidth;
	pParticle->m_SnapShot.rtRect.top	= posParent.y + posInterpolate.y - nHalfHeight;
	pParticle->m_SnapShot.rtRect.right	= posParent.x + posInterpolate.x + nHalfWidth;
	pParticle->m_SnapShot.rtRect.bottom	= posParent.y + posInterpolate.y + nHalfHeight;
	pParticle->m_SnapShot.uAlpha = ucAlpha;
}

///////////////////////////////////////////////////////////////////////////////////////////
// CPathParticle 
CPathParticle::CPathParticle( CSurface& surface, std::vector<CPathInfo*>* pvecPath, CPos& posParent )
: m_posParent( posParent )
{
	m_Original.rtRect.left = surface.m_Original.rtRect.left;
	m_Original.rtRect.top = surface.m_Original.rtRect.top;
	m_Original.rtRect.right	= surface.m_Original.rtRect.right;
	m_Original.rtRect.bottom = surface.m_Original.rtRect.bottom;
	m_Original.uRed = surface.m_Original.uRed;
	m_Original.uGreen = surface.m_Original.uGreen;
	m_Original.uBlue = surface.m_Original.uBlue;
	m_Original.uAlpha = surface.m_Original.uAlpha;
	m_Original.UVs[0] = surface.m_Original.UVs[0];
	m_Original.UVs[1] = surface.m_Original.UVs[1];
	m_Original.UVs[2] = surface.m_Original.UVs[2];
	m_Original.UVs[3] = surface.m_Original.UVs[3];
	m_Original.uBlend = surface.m_Original.uBlend;

	m_SnapShot.rtRect.left = surface.m_SnapShot.rtRect.left;
	m_SnapShot.rtRect.top = surface.m_SnapShot.rtRect.top;
	m_SnapShot.rtRect.right	= surface.m_SnapShot.rtRect.right;
	m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.bottom;
	m_SnapShot.uRed = surface.m_SnapShot.uRed;
	m_SnapShot.uGreen = surface.m_SnapShot.uGreen;
	m_SnapShot.uBlue = surface.m_SnapShot.uBlue;
	m_SnapShot.uAlpha = surface.m_SnapShot.uAlpha;
	m_SnapShot.UVs[0] = surface.m_SnapShot.UVs[0];
	m_SnapShot.UVs[1] = surface.m_SnapShot.UVs[1];
	m_SnapShot.UVs[2] = surface.m_SnapShot.UVs[2];
	m_SnapShot.UVs[3] = surface.m_SnapShot.UVs[3];
	m_SnapShot.uBlend = surface.m_SnapShot.uBlend;

	m_pTexture = surface.m_pTexture;

	m_fAngle = surface.m_fAngle;

	m_pvecPath = pvecPath;
	
	Reset();
}

VOID CPathParticle::Reset(VOID)
{
	m_fCurrentTime = 0.0f;
	m_nPathIndex = 0;
}

BOOL CPathParticle::Update( FLOAT fElapsedTIme, BOOL bRepeat )
{
	// return FALSE : »èÁ¦.
	
	m_fCurrentTime += fElapsedTIme;

	// Time Check
	if( m_fCurrentTime >= (*m_pvecPath)[m_nPathIndex]->m_fPathTime )
	{
		m_fCurrentTime -= (*m_pvecPath)[m_nPathIndex]->m_fPathTime;
		++m_nPathIndex;
	}

	// Path Check
	if( m_nPathIndex >= (INT)m_pvecPath->size() )
	{
		if( bRepeat )
			Reset();		
		else
			return FALSE;			
	}
	
	// Apply
	(*m_pvecPath)[m_nPathIndex]->Update( this );

	return TRUE;
}

VOID CPathParticle::Render(VOID)
{
	g_GuiRenderer.RenderQueue( &m_SnapShot, m_pTexture );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CPathChaseEffect

CPathChaseEffect::CPathChaseEffect(VOID)
{
	Init();
}

CPathChaseEffect::~CPathChaseEffect(VOID)
{
	ClearPath();
	ClearParticle();
}

VOID CPathChaseEffect::SetCenterPos( INT nPosX, INT nPosY )
{
	m_posCenter.SetPos( nPosX, nPosY );

	std::list<CPathParticle*>::iterator it;

	for( it = m_listParticles.begin() ; it != m_listParticles.end() ; ++it )
	{
		(*it)->SetParentPos( m_posCenter );
	}
}	

VOID CPathChaseEffect::AddPathInfo( CPathInfo* pPathInfo )
{
	if( IsWork() )
		EndProc();
	
	m_vecPathInfo.push_back( pPathInfo );
}

VOID CPathChaseEffect::SetEmitSizer( INT nWidth, INT nHeight )
{
	if( nWidth == 0 && nHeight == 0 )
	{
		m_EmitSizer.bSizer = FALSE;
		return;
	}

	m_EmitSizer.bSizer = TRUE;
	m_EmitSizer.nWidth = nWidth;
	m_EmitSizer.nHeight= nHeight;
}

VOID CPathChaseEffect::Reset(VOID)
{
	ClearParticle();

	m_fCurrentTime = 0.0f;
	m_fCurrentEmitTime = 0.0f;
	m_bFinishEmit = FALSE;
}

VOID CPathChaseEffect::Update( FLOAT fElapsedTime )
{
	if( !m_bRun )
		return;

	// LifeTime Check
	m_fCurrentTime += fElapsedTime;

	if( m_fCurrentTime >= m_fLifeTime )
	{
		if( !m_bRepeat )
		{
			EndProc();
			return;
		}
	}		

	// EmitTime Check
	if( !m_bFinishEmit )
	{
		if( m_listParticles.size() < (DWORD)m_nEmitCount )
		{
			m_fCurrentEmitTime += fElapsedTime;

			if( m_fCurrentEmitTime >= m_fEmitTime )
			{
				EmitParticle();
				m_fCurrentEmitTime -= m_fEmitTime;
			}
		}	
		else
			m_bFinishEmit = TRUE;
	}
	
	// Particle Update
	std::list<CPathParticle*>::iterator it;

	for( it = m_listParticles.begin() ; it != m_listParticles.end() ; )
	{
		if( (*it)->Update( fElapsedTime, m_bRepeat ) )
		{
			++it;
		}
		else
		{
			delete (*it);
			it = m_listParticles.erase( it );
		}
	}
}

VOID CPathChaseEffect::Render(VOID)
{
	if( m_bRun )
	{
		std::list<CPathParticle*>::iterator it;

		for( it = m_listParticles.begin() ; it != m_listParticles.end() ; ++it )
		{
			(*it)->Render();
		}
	}	
}

VOID CPathChaseEffect::Init(VOID)
{
	m_fLifeTime = 0.0f;
	m_fCurrentEmitTime = 0.0f;
	m_fEmitTime = 0.0f;
	m_fCurrentEmitTime = 0.0f;

	m_nEmitCount = 0;

	m_bRun = FALSE;
	m_bRepeat = FALSE;
	m_bFinishEmit = FALSE;

	m_EmitSizer.bSizer = FALSE;
	m_EmitSizer.nWidth = 0;
	m_EmitSizer.nHeight= 0;
}

VOID CPathChaseEffect::ClearParticle(VOID)
{
	std::list<CPathParticle*>::iterator it;

	for( it =  m_listParticles.begin() ; it != m_listParticles.end() ; ++it )
	{
		delete (*it);
	}

	m_listParticles.clear();
}

VOID CPathChaseEffect::ClearPath(VOID)
{
	std::vector<CPathInfo*>::iterator it;

	for( it = m_vecPathInfo.begin() ; it != m_vecPathInfo.end() ; ++it )
	{
		delete (*it);
	}

	m_vecPathInfo.clear();
}

VOID CPathChaseEffect::EmitParticle(VOID)
{
	if( m_EmitSizer.bSizer )
	{
		CSurface surface = m_vecSurface[0];

		surface.m_SnapShot.rtRect.right += m_EmitSizer.nWidth * (INT)m_listParticles.size();
		surface.m_SnapShot.rtRect.bottom+= m_EmitSizer.nHeight* (INT)m_listParticles.size();

		m_listParticles.push_back( NTL_NEW CPathParticle( surface, &m_vecPathInfo, m_posCenter ) );
	}
	else
	{
		m_listParticles.push_back( NTL_NEW CPathParticle( m_vecSurface[0], &m_vecPathInfo, m_posCenter ) );
	}	
}

END_GUI
*/