#include "gui_precomp.h"
#include "gui_effect_flicker.h"

#include "gui_renderer.h"
#include "GuiUtil.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CFlickerEffect::CFlickerEffect(VOID)
{
	Init();
}

CFlickerEffect::~CFlickerEffect(VOID)
{

}

////////////////////////////////////////////////////////////////////////////////////////////
// Attribute

VOID CFlickerEffect::SetSurface( const CSurface& surface )
{
	m_surSurface = surface;
	m_nVaryWidth = surface.m_Original.rtRect.GetWidth() / 2;
	m_nVaryHeight = surface.m_Original.rtRect.GetHeight() / 2;
}

VOID CFlickerEffect::SetCenterPos( INT nPosX, INT nPosY )
{
	m_nInitPosX = nPosX;
	m_nInitPosY = nPosY;

	m_surSurface.m_SnapShot.rtRect.left		= m_nInitPosX - m_nVaryWidth;
	m_surSurface.m_SnapShot.rtRect.top		= m_nInitPosY - m_nVaryHeight;
	m_surSurface.m_SnapShot.rtRect.right	= m_nInitPosX + m_nVaryWidth;
	m_surSurface.m_SnapShot.rtRect.bottom	= m_nInitPosY + m_nVaryHeight;	
}

////////////////////////////////////////////////////////////////////////////////////////////
// Operation

VOID CFlickerEffect::Update( FLOAT fElapsedTime )
{
	if( !m_bRun )
		return;

	// Lifetime Check
	m_fCurrentTime += fElapsedTime;

	if( m_fLifeTime != 0.0f )
	{
		if( m_fCurrentTime >= m_fLifeTime )
		{
			if( m_bRepeat )
				Reset();
			else
			{
				EndProc();
				return;
			}
		}
	}	

	// Pulsetime Check
	m_fCurrentPulseTime += fElapsedTime;

	if( m_fCurrentPulseTime >= m_fPulseTime )
	{
		m_bBeginState = !m_bBeginState;
		m_fCurrentPulseTime -= m_fPulseTime;
	}	

	// Apply 
	if( m_dwTypeFlag & TYPE_ALPHA )
		ApplyAlpha();

	if( m_dwTypeFlag & TYPE_COLOR )
		ApplyColor();

	if( m_dwTypeFlag & TYPE_SIZE )
		ApplySize();
}

VOID CFlickerEffect::Render(VOID)
{
	if( m_bRun )
	{
		g_GuiRenderer.RenderQueue( &m_surSurface.m_SnapShot, m_surSurface.m_pTexture, m_bRenderTop );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Implementation

VOID CFlickerEffect::Init(VOID)
{
	m_fLifeTime = m_fPulseTime = m_fCurrentTime = m_fCurrentPulseTime = 0.0f;

	m_ucBeginAlpha = m_ucEndAlpha = 0;
	m_uiBeginColor = m_uiEndColor = 0;
	m_nBeginWidth = m_nEndWidth = m_nBeginHeight = m_nEndHeight = 0;

	m_nInitPosX = m_nInitPosY = m_nVaryWidth = m_nVaryHeight = 0;

	m_dwTypeFlag = 0;
	
	m_bRun = FALSE;
	m_bRepeat = FALSE;
	m_bBeginState = TRUE;

	m_bRenderTop = FALSE;
}

VOID CFlickerEffect::ApplyAlpha(VOID)
{
	BYTE ucAlpha;

	if( m_bBeginState )
		ucAlpha = (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_ucBeginAlpha, (FLOAT)m_ucEndAlpha ) + 0.5f );
	else
		ucAlpha = (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_ucEndAlpha, (FLOAT)m_ucBeginAlpha ) + 0.5f );

	m_surSurface.m_SnapShot.uAlpha = ucAlpha;
}

VOID CFlickerEffect::ApplyColor(VOID)
{
	BYTE ucRed, ucGreen, ucBlue;

	if( m_bBeginState )
	{
		ucRed	= (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetRValue( m_uiBeginColor ) ), (FLOAT)( GetRValue( m_uiEndColor ) ) ) + 0.5f);
		ucGreen = (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetGValue( m_uiBeginColor ) ), (FLOAT)( GetGValue( m_uiEndColor ) ) ) + 0.5f);
		ucBlue	= (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetBValue( m_uiBeginColor ) ), (FLOAT)( GetBValue( m_uiEndColor ) ) ) + 0.5f);
	}
	else
	{
		ucRed	= (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetRValue( m_uiEndColor ) ), (FLOAT)( GetRValue( m_uiBeginColor ) ) ) + 0.5f );
		ucGreen = (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetGValue( m_uiEndColor ) ), (FLOAT)( GetGValue( m_uiBeginColor ) ) ) + 0.5f );
		ucBlue	= (BYTE)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)( GetBValue( m_uiEndColor ) ), (FLOAT)( GetBValue( m_uiBeginColor ) ) ) + 0.5f );
	}

	m_surSurface.m_SnapShot.uRed	= ucRed;
	m_surSurface.m_SnapShot.uGreen	= ucGreen;
	m_surSurface.m_SnapShot.uBlend	= ucBlue;
}

VOID CFlickerEffect::ApplySize(VOID)
{
	INT nWidth, nHeight;

	if( m_bBeginState )
	{
		nWidth	= (INT)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_nBeginWidth, (FLOAT) m_nEndWidth ) + 0.5f );
		nHeight	= (INT)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_nBeginHeight, (FLOAT) m_nEndHeight ) + 0.5f );
	}
	else
	{
		nWidth	= (INT)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_nEndWidth, (FLOAT) m_nBeginWidth ) + 0.5f );
		nHeight	= (INT)( GetResultLineInterpolation( m_fCurrentPulseTime, m_fPulseTime, (FLOAT)m_nEndHeight, (FLOAT) m_nBeginHeight ) + 0.5f );
	}

	m_nVaryWidth	= nWidth / 2;
	m_nVaryHeight	= nHeight / 2;

	m_surSurface.m_SnapShot.rtRect.left		= m_nInitPosX - m_nVaryWidth;
	m_surSurface.m_SnapShot.rtRect.top		= m_nInitPosY - m_nVaryHeight;
	m_surSurface.m_SnapShot.rtRect.right	= m_nInitPosX + m_nVaryWidth;
	m_surSurface.m_SnapShot.rtRect.bottom	= m_nInitPosY + m_nVaryHeight;
}

END_GUI