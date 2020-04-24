#include "gui_precomp.h"
#include "gui_effect_radar.h"
#include "gui_renderer.h"

#define RADAREFFECT_UPDATETIME 0

START_GUI

CRadarEffect::CRadarEffect(VOID)
{
	memset( m_triOriginal, 0, sizeof( TriSnapShot ) * 8 );
	memset( m_triSnapShot, 0, sizeof( TriSnapShot ) * 8 );
	
	m_pTexture = NULL;

	m_fUpdateElapsedTime = 0;
	m_fEffectTime = 0;
	m_nState = 0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_fUVWidth = 0.0f;
	m_fUVHeight = 0.0f;

	m_bShow = FALSE;
	m_bClockDirFill = FALSE;
	m_bRotDirClock = TRUE;
	m_bClipping = FALSE;

	m_fpCalcProc[0] = &CRadarEffect::CalcProc0;
	m_fpCalcProc[1] = &CRadarEffect::CalcProc1;
	m_fpCalcProc[2] = &CRadarEffect::CalcProc2;
	m_fpCalcProc[3] = &CRadarEffect::CalcProc3;
	m_fpCalcProc[4] = &CRadarEffect::CalcProc4;
	m_fpCalcProc[5] = &CRadarEffect::CalcProc5;
	m_fpCalcProc[6] = &CRadarEffect::CalcProc6;
	m_fpCalcProc[7] = &CRadarEffect::CalcProc7;	
}

CRadarEffect::~CRadarEffect(VOID)
{

}

VOID CRadarEffect::SetPosition( INT nPosX, INT nPosY )
{
	if( m_bShow )
	{
		INT nDiffX = nPosX - m_triOriginal[7].ptPos[0].x;
		INT nDiffY = nPosY - m_triOriginal[7].ptPos[2].y;

		for( RwInt32 i = 0 ; i < 8 ; ++i )
		{
			for( RwInt32 j = 0 ; j < 3 ; ++j )
			{
				m_triSnapShot[i].ptPos[j].x += nDiffX;
				m_triSnapShot[i].ptPos[j].y += nDiffY;
			}
		}
	}
	else
	{
		m_triSnapShot[0].ptPos[0].y = m_triSnapShot[0].ptPos[2].y = m_triSnapShot[1].ptPos[0].y =
		m_triSnapShot[6].ptPos[2].y = m_triSnapShot[7].ptPos[0].y = m_triSnapShot[7].ptPos[2].y = nPosY;

		m_triSnapShot[2].ptPos[2].y = m_triSnapShot[3].ptPos[0].y = m_triSnapShot[3].ptPos[2].y =
		m_triSnapShot[4].ptPos[0].y = m_triSnapShot[4].ptPos[2].y = m_triSnapShot[5].ptPos[0].y = nPosY + m_nHeight;

		m_triSnapShot[4].ptPos[2].x = m_triSnapShot[5].ptPos[0].x = m_triSnapShot[5].ptPos[2].x =
		m_triSnapShot[6].ptPos[0].x = m_triSnapShot[6].ptPos[2].x = m_triSnapShot[7].ptPos[0].x = nPosX;

		m_triSnapShot[0].ptPos[2].x = m_triSnapShot[1].ptPos[0].x = m_triSnapShot[1].ptPos[2].x =
		m_triSnapShot[2].ptPos[0].x = m_triSnapShot[2].ptPos[2].x = m_triSnapShot[3].ptPos[0].x = nPosX + m_nWidth;

		m_triSnapShot[0].ptPos[1].x = m_triSnapShot[1].ptPos[1].x = m_triSnapShot[2].ptPos[1].x =
		m_triSnapShot[3].ptPos[1].x = m_triSnapShot[4].ptPos[1].x = m_triSnapShot[5].ptPos[1].x =
		m_triSnapShot[6].ptPos[1].x = m_triSnapShot[7].ptPos[1].x = m_triSnapShot[0].ptPos[0].x =
		m_triSnapShot[7].ptPos[2].x = m_triSnapShot[3].ptPos[2].x = m_triSnapShot[4].ptPos[0].x = nPosX + m_nWidth / 2;

		m_triSnapShot[0].ptPos[1].y = m_triSnapShot[1].ptPos[1].y = m_triSnapShot[2].ptPos[1].y =
		m_triSnapShot[3].ptPos[1].y = m_triSnapShot[4].ptPos[1].y = m_triSnapShot[5].ptPos[1].y =
		m_triSnapShot[6].ptPos[1].y = m_triSnapShot[7].ptPos[1].y = m_triSnapShot[1].ptPos[2].y =
		m_triSnapShot[2].ptPos[0].y = m_triSnapShot[5].ptPos[2].y = m_triSnapShot[6].ptPos[0].y = nPosY + m_nHeight / 2;
	}	

	m_triOriginal[0].ptPos[0].y = m_triOriginal[0].ptPos[2].y = m_triOriginal[1].ptPos[0].y =
	m_triOriginal[6].ptPos[2].y = m_triOriginal[7].ptPos[0].y = m_triOriginal[7].ptPos[2].y = nPosY;

	m_triOriginal[2].ptPos[2].y = m_triOriginal[3].ptPos[0].y = m_triOriginal[3].ptPos[2].y =
	m_triOriginal[4].ptPos[0].y = m_triOriginal[4].ptPos[2].y = m_triOriginal[5].ptPos[0].y = nPosY + m_nHeight;

	m_triOriginal[4].ptPos[2].x = m_triOriginal[5].ptPos[0].x = m_triOriginal[5].ptPos[2].x =
	m_triOriginal[6].ptPos[0].x = m_triOriginal[6].ptPos[2].x = m_triOriginal[7].ptPos[0].x = nPosX;

	m_triOriginal[0].ptPos[2].x = m_triOriginal[1].ptPos[0].x = m_triOriginal[1].ptPos[2].x =
	m_triOriginal[2].ptPos[0].x = m_triOriginal[2].ptPos[2].x = m_triOriginal[3].ptPos[0].x = nPosX + m_nWidth;

	m_triOriginal[0].ptPos[1].x = m_triOriginal[1].ptPos[1].x = m_triOriginal[2].ptPos[1].x =
	m_triOriginal[3].ptPos[1].x = m_triOriginal[4].ptPos[1].x = m_triOriginal[5].ptPos[1].x =
	m_triOriginal[6].ptPos[1].x = m_triOriginal[7].ptPos[1].x = m_triOriginal[0].ptPos[0].x =
	m_triOriginal[7].ptPos[2].x = m_triOriginal[3].ptPos[2].x = m_triOriginal[4].ptPos[0].x = nPosX + m_nWidth / 2;

	m_triOriginal[0].ptPos[1].y = m_triOriginal[1].ptPos[1].y = m_triOriginal[2].ptPos[1].y =
	m_triOriginal[3].ptPos[1].y = m_triOriginal[4].ptPos[1].y = m_triOriginal[5].ptPos[1].y =
	m_triOriginal[6].ptPos[1].y = m_triOriginal[7].ptPos[1].y = m_triOriginal[1].ptPos[2].y =
	m_triOriginal[2].ptPos[0].y = m_triOriginal[5].ptPos[2].y = m_triOriginal[6].ptPos[0].y = nPosY + m_nHeight / 2;
}

VOID CRadarEffect::SetSurface( const CSurface& surface )
{
	m_nWidth = surface.m_Original.rtRect.GetWidth();
	m_nHeight= surface.m_Original.rtRect.GetHeight();
	m_fUVWidth = surface.m_Original.UVs[2] - surface.m_Original.UVs[0];
	m_fUVHeight = surface.m_Original.UVs[3] - surface.m_Original.UVs[1];

	// Position
	m_triOriginal[0].ptPos[0].y = m_triOriginal[0].ptPos[2].y = m_triOriginal[1].ptPos[0].y =
	m_triOriginal[6].ptPos[2].y = m_triOriginal[7].ptPos[0].y = m_triOriginal[7].ptPos[2].y = surface.m_Original.rtRect.top;

	m_triOriginal[2].ptPos[2].y = m_triOriginal[3].ptPos[0].y = m_triOriginal[3].ptPos[2].y =
	m_triOriginal[4].ptPos[0].y = m_triOriginal[4].ptPos[2].y = m_triOriginal[5].ptPos[0].y = surface.m_Original.rtRect.bottom;

	m_triOriginal[4].ptPos[2].x = m_triOriginal[5].ptPos[0].x = m_triOriginal[5].ptPos[2].x =
	m_triOriginal[6].ptPos[0].x = m_triOriginal[6].ptPos[2].x = m_triOriginal[7].ptPos[0].x = surface.m_Original.rtRect.left;

	m_triOriginal[0].ptPos[2].x = m_triOriginal[1].ptPos[0].x = m_triOriginal[1].ptPos[2].x =
	m_triOriginal[2].ptPos[0].x = m_triOriginal[2].ptPos[2].x = m_triOriginal[3].ptPos[0].x = surface.m_Original.rtRect.right;

	m_triOriginal[0].ptPos[1].x = m_triOriginal[1].ptPos[1].x = m_triOriginal[2].ptPos[1].x =
	m_triOriginal[3].ptPos[1].x = m_triOriginal[4].ptPos[1].x = m_triOriginal[5].ptPos[1].x =
	m_triOriginal[6].ptPos[1].x = m_triOriginal[7].ptPos[1].x = m_triOriginal[0].ptPos[0].x =
	m_triOriginal[7].ptPos[2].x = m_triOriginal[3].ptPos[2].x = m_triOriginal[4].ptPos[0].x = surface.m_Original.rtRect.left + m_nWidth / 2;

	m_triOriginal[0].ptPos[1].y = m_triOriginal[1].ptPos[1].y = m_triOriginal[2].ptPos[1].y =
	m_triOriginal[3].ptPos[1].y = m_triOriginal[4].ptPos[1].y = m_triOriginal[5].ptPos[1].y =
	m_triOriginal[6].ptPos[1].y = m_triOriginal[7].ptPos[1].y = m_triOriginal[1].ptPos[2].y =
	m_triOriginal[2].ptPos[0].y = m_triOriginal[5].ptPos[2].y = m_triOriginal[6].ptPos[0].y = surface.m_Original.rtRect.top + m_nHeight / 2;

	// UVs
	m_triOriginal[0].UVs[1] = m_triOriginal[0].UVs[5] = m_triOriginal[1].UVs[1] =
	m_triOriginal[6].UVs[5] = m_triOriginal[7].UVs[1] = m_triOriginal[7].UVs[5] = surface.m_Original.UVs[1];

	m_triOriginal[2].UVs[5] = m_triOriginal[3].UVs[1] = m_triOriginal[3].UVs[5] =
	m_triOriginal[4].UVs[1] = m_triOriginal[4].UVs[5] = m_triOriginal[5].UVs[1] = surface.m_Original.UVs[3];

	m_triOriginal[4].UVs[4] = m_triOriginal[5].UVs[0] = m_triOriginal[5].UVs[4] =
	m_triOriginal[6].UVs[0] = m_triOriginal[6].UVs[4] = m_triOriginal[7].UVs[0] = surface.m_Original.UVs[0];

	m_triOriginal[0].UVs[4] = m_triOriginal[1].UVs[0] = m_triOriginal[1].UVs[4] =
	m_triOriginal[2].UVs[0] = m_triOriginal[2].UVs[4] = m_triOriginal[3].UVs[0] = surface.m_Original.UVs[2];

	m_triOriginal[0].UVs[2] = m_triOriginal[1].UVs[2] = m_triOriginal[2].UVs[2] =
	m_triOriginal[3].UVs[2] = m_triOriginal[4].UVs[2] = m_triOriginal[5].UVs[2] =
	m_triOriginal[6].UVs[2] = m_triOriginal[7].UVs[2] = m_triOriginal[0].UVs[0] =
	m_triOriginal[7].UVs[4] = m_triOriginal[3].UVs[4] = m_triOriginal[4].UVs[0] = surface.m_Original.UVs[0] + m_fUVWidth / 2;

	m_triOriginal[0].UVs[3] = m_triOriginal[1].UVs[3] = m_triOriginal[2].UVs[3] =
	m_triOriginal[3].UVs[3] = m_triOriginal[4].UVs[3] = m_triOriginal[5].UVs[3] =
	m_triOriginal[6].UVs[3] = m_triOriginal[7].UVs[3] = m_triOriginal[1].UVs[5] =
	m_triOriginal[2].UVs[1] = m_triOriginal[5].UVs[5] = m_triOriginal[6].UVs[1] = surface.m_Original.UVs[1] + m_fUVHeight / 2;
	
	// Etc
	for( INT i = 0 ; i < NUM_TRIANGLE ; ++i )
	{
		m_triOriginal[i].uRed	= surface.m_Original.uRed;
		m_triOriginal[i].uGreen = surface.m_Original.uGreen;
		m_triOriginal[i].uBlue	= surface.m_Original.uBlue;
		m_triOriginal[i].uAlpha = surface.m_Original.uAlpha;
		m_triOriginal[i].uBlend = surface.m_Original.uBlend;
	}

	m_pTexture = surface.m_pTexture;
}

BOOL CRadarEffect::StartProc( FLOAT fEffectTime, RwBool bDirClock /* = TRUE */, RwBool bFill /* = FALSE  */ )
{
	if( m_nWidth <= 0 || m_nHeight <= 0 )
		return FALSE;

	m_bShow = TRUE;
	m_nState = 0;
	m_fUpdateElapsedTime = 0.0f;
	m_fEffectTime = fEffectTime;
	SetAttribute( bDirClock, bFill );

	for( INT i = 0 ; i < NUM_TRIANGLE ; ++i )
	{
		m_triSnapShot[i] = m_triOriginal[i];
	}

	return TRUE;
}

void CRadarEffect::EndProc()
{
	m_bShow = FALSE;
}

VOID CRadarEffect::Update( FLOAT fCurrentTime )
{
	if( !m_bShow )
		return;
		
	if( fCurrentTime - m_fUpdateElapsedTime < RADAREFFECT_UPDATETIME )
		return;

	if( fCurrentTime >= m_fEffectTime )
		return;
	
	FLOAT fTempTime = m_bRotDirClock ? fCurrentTime : ( m_fEffectTime - fCurrentTime );
	FLOAT tValue = fTempTime / m_fEffectTime;
	FLOAT fRadian = ( tValue * 360.0f ) * ( 3.141592654f / 180.0f );

	D3DXVECTOR3 vecInitial( 0, 2, 0 );
	D3DXVECTOR4 vecResult;
	D3DXMATRIXA16 matRot;
		
	D3DXMatrixRotationZ( &matRot, -fRadian );
	D3DXVec3Transform( &vecResult, &vecInitial, &matRot );	

	INT nState = (INT)( tValue * 8 );
	BOOL bFirst = FALSE;

	if( nState != bFirst )
	{
		bFirst = TRUE;
		m_nState = nState;
	}	

	(this->*m_fpCalcProc[m_nState])( vecResult.y / vecResult.x, bFirst );

	m_fUpdateElapsedTime = fCurrentTime;
}

VOID CRadarEffect::Render(VOID)
{
	if( m_bShow )
	{
		CRectangle* pClippingRect = NULL;

		if( m_bClipping )
			pClippingRect = &m_rtClipping;

		if( m_bClockDirFill )
		{
			for( INT i = 0 ; i <= m_nState ; ++i )
			{
				//if( m_bClipping )
				//{
				//	if( !m_rtClipping.PtInRect( m_triSnapShot[i].ptPos[1] ) )
				//		continue;
				//}
				g_GuiRenderer.RenderBuffer( &m_triSnapShot[i], m_pTexture, pClippingRect );
			}			
		}
		else
		{
			for( INT i = NUM_TRIANGLE - 1 ; i >= m_nState ; --i )
			{
				//if( m_bClipping )
				//{
				//	if( !m_rtClipping.PtInRect( m_triSnapShot[i].ptPos[1] ) )
				//		continue;
				//}

				g_GuiRenderer.RenderBuffer( &m_triSnapShot[i], m_pTexture, pClippingRect );
			}
		}		
	}
}

VOID CRadarEffect::SetClippingMode( BOOL bClipping )
{
	m_bClipping = bClipping;
}

VOID CRadarEffect::SetClippingRect( CRectangle& rtRect )
{
	m_rtClipping = rtRect;
}

VOID CRadarEffect::CalcProc0( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fIntersectX = 1 / fGradient;
	
	INT		nRelX = (INT)( GetRelativeValue( fIntersectX, (FLOAT)m_nWidth ) );
	FLOAT	fRelU = GetRelativeValue( fIntersectX, m_fUVWidth );

	if( m_bClockDirFill )
	{
		m_triSnapShot[0].ptPos[2].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[0].UVs[4] = m_triOriginal[7].UVs[0] + fRelU;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[1].ptPos[0].y = m_triOriginal[1].ptPos[0].y;
			m_triSnapShot[1].UVs[1] = m_triOriginal[1].UVs[1];
		}

		m_triSnapShot[0].ptPos[0].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[0].UVs[0] = m_triOriginal[7].UVs[0] + fRelU;
	}
}

VOID CRadarEffect::CalcProc1( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectY = -fGradient;

	INT		nRelY = (INT)( GetRelativeValue( fInterSectY, (FLOAT)m_nHeight ) );
	FLOAT	fRelV = GetRelativeValue( fInterSectY, m_fUVHeight);

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[0].ptPos[2].x = m_triOriginal[0].ptPos[2].x;
			m_triSnapShot[0].UVs[4] = m_triOriginal[0].UVs[4];
		}
		
		m_triSnapShot[1].ptPos[2].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[1].UVs[5] = m_triOriginal[7].UVs[1] + fRelV;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[2].ptPos[0].y = m_triOriginal[2].ptPos[0].y;
			m_triSnapShot[2].UVs[1] = m_triOriginal[2].UVs[1];
		}

		m_triSnapShot[1].ptPos[0].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[1].UVs[1] = m_triOriginal[7].UVs[1] + fRelV;		
	}	
}

VOID CRadarEffect::CalcProc2( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectY = -fGradient;

	INT		nRelY = (INT)( GetRelativeValue( fInterSectY, (FLOAT)m_nHeight ) );
	FLOAT	fRelV = GetRelativeValue( fInterSectY, m_fUVHeight );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[1].ptPos[2].y = m_triOriginal[1].ptPos[2].y;
			m_triSnapShot[1].UVs[5] = m_triOriginal[1].UVs[5];
		}		

		m_triSnapShot[2].ptPos[2].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[2].UVs[5] = m_triOriginal[7].UVs[1] + fRelV;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[3].ptPos[0].x = m_triOriginal[3].ptPos[0].x;
			m_triSnapShot[3].UVs[0] = m_triOriginal[3].UVs[0];
		}

		m_triSnapShot[2].ptPos[0].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[2].UVs[1] = m_triOriginal[7].UVs[1] + fRelV;
	}	
}

VOID CRadarEffect::CalcProc3( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectX = -1 / fGradient;
	
	INT		nRelX = (INT)( GetRelativeValue( fInterSectX, (FLOAT)m_nWidth ) );
	FLOAT	fRelU = GetRelativeValue( fInterSectX, m_fUVWidth );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[2].ptPos[2].y = m_triOriginal[2].ptPos[2].y;
			m_triSnapShot[2].UVs[5] = m_triOriginal[2].UVs[5];
		}
		
		m_triSnapShot[3].ptPos[2].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[3].UVs[4] = m_triOriginal[7].UVs[0] + fRelU;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[4].ptPos[0].x = m_triOriginal[4].ptPos[0].x;
			m_triSnapShot[4].UVs[0] = m_triOriginal[4].UVs[0];
		}
		
		m_triSnapShot[3].ptPos[0].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[3].UVs[0] = m_triOriginal[7].UVs[0] + fRelU;
	}	
}

VOID CRadarEffect::CalcProc4( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectX = -1 / fGradient;
	
	INT		nRelX = (INT)( GetRelativeValue( fInterSectX, (FLOAT)m_nWidth ) );
	FLOAT	fRelU = GetRelativeValue( fInterSectX, m_fUVWidth );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[3].ptPos[2].x = m_triOriginal[3].ptPos[2].x;
			m_triSnapShot[3].UVs[4] = m_triOriginal[3].UVs[4];
		}		

		m_triSnapShot[4].ptPos[2].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[4].UVs[4] = m_triOriginal[7].UVs[0] + fRelU;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[5].ptPos[0].y = m_triOriginal[5].ptPos[0].y;
			m_triSnapShot[5].UVs[1] = m_triOriginal[5].UVs[1];
		}
		
		m_triSnapShot[4].ptPos[0].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[4].UVs[0] = m_triOriginal[7].UVs[0] + fRelU;
	}	
}

VOID CRadarEffect::CalcProc5( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectY = fGradient;

	INT		nRelY = (INT)( GetRelativeValue( fInterSectY, (FLOAT)m_nHeight ) );
	FLOAT	fRelV = GetRelativeValue( fInterSectY, m_fUVHeight );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[4].ptPos[2].x = m_triOriginal[4].ptPos[2].x;
			m_triSnapShot[4].UVs[4] = m_triOriginal[4].UVs[4];
		}
		
		m_triSnapShot[5].ptPos[2].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[5].UVs[5] = m_triOriginal[7].UVs[1] + fRelV;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[6].ptPos[0].y = m_triOriginal[6].ptPos[0].y;
			m_triSnapShot[6].UVs[1] = m_triOriginal[6].UVs[1];
		}

		m_triSnapShot[5].ptPos[0].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[5].UVs[1] = m_triOriginal[7].UVs[1] + fRelV;
	}
}

VOID CRadarEffect::CalcProc6( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fInterSectY = fGradient;

	INT		nRelY = (INT)( GetRelativeValue( fInterSectY, (FLOAT)m_nHeight ) );
	FLOAT	fRelV = GetRelativeValue( fInterSectY, m_fUVHeight );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[5].ptPos[2].y = m_triOriginal[5].ptPos[2].y;
			m_triSnapShot[5].UVs[5] = m_triOriginal[5].UVs[5];
		}

		m_triSnapShot[6].ptPos[2].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[6].UVs[5] = m_triOriginal[7].UVs[1] + fRelV;
	}
	else
	{
		if( bFirst )
		{
			m_triSnapShot[7].ptPos[0].x = m_triOriginal[7].ptPos[0].x;
			m_triSnapShot[7].UVs[0] = m_triOriginal[7].UVs[0];
		}

		m_triSnapShot[6].ptPos[0].y = m_triOriginal[7].ptPos[0].y + nRelY;
		m_triSnapShot[6].UVs[1] = m_triOriginal[7].UVs[1] + fRelV;
	}
}

VOID CRadarEffect::CalcProc7( FLOAT fGradient, BOOL bFirst )
{
	FLOAT fIntersectX = 1 / fGradient;
	
	INT		nRelX = (INT)( GetRelativeValue( fIntersectX, (FLOAT)m_nWidth ) );
	FLOAT	fRelU = GetRelativeValue( fIntersectX, m_fUVWidth );

	if( m_bClockDirFill )
	{
		if( bFirst )
		{
			m_triSnapShot[6].ptPos[2].y = m_triOriginal[6].ptPos[2].y;
			m_triSnapShot[6].UVs[5] = m_triOriginal[6].UVs[5];
		}

		m_triSnapShot[7].ptPos[2].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[7].UVs[4] = m_triOriginal[7].UVs[0] + fRelU;
	}
	else
	{
		m_triSnapShot[7].ptPos[0].x = m_triOriginal[7].ptPos[0].x + nRelX;
		m_triSnapShot[7].UVs[0] = m_triOriginal[7].UVs[0] + fRelU;
	}
}

FLOAT CRadarEffect::GetRelativeValue( FLOAT fValue, FLOAT fComparison )
{
	fValue = ( fValue + 1.0f ) / 2;
	return fValue * fComparison;
}

VOID CRadarEffect::SetAttribute( RwBool bDirClock, RwBool bFill )
{
	m_bRotDirClock = bDirClock;

	if( bDirClock )
	{
		if( bFill )
			m_bClockDirFill = TRUE;
		else
			m_bClockDirFill = FALSE;		
	}
	else
	{
		if( bFill )
			m_bClockDirFill = FALSE;
		else
			m_bClockDirFill = TRUE;
	}
}

END_GUI