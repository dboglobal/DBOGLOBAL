#include "precomp_dboclient.h"
#include "GaugeAnimation.h"

#include "gui_progressbar.h"
#include "GuiUtil.h"

#define GAUGE_SPEED				30.0f		// pixel per second

CGaugeAnimation::CGaugeAnimation(VOID)
{
	Init();
}

CGaugeAnimation::~CGaugeAnimation(VOID)
{

}

VOID CGaugeAnimation::Init(VOID)
{
	m_fAnimationTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_fStartValue = 0.0f;
	m_fDestValue = 0.0f;	
	m_fSpeed = GAUGE_SPEED;
}

VOID CGaugeAnimation::Update( RwReal fElapsed, gui::CProgressBar* pGaugeComponent )
{
	if( m_fAnimationTime != 0.0f )
	{
		m_fCurrentTime += fElapsed;

		RwReal fValue = gui::GetResultLineInterpolation( m_fCurrentTime, m_fAnimationTime, m_fStartValue, m_fDestValue );
		pGaugeComponent->SetPos( (RwInt32)fValue );

		if( m_fCurrentTime >= m_fAnimationTime )
			m_fAnimationTime = 0.0f;
	}
}

VOID CGaugeAnimation::SetValue( RwInt32 nValue, RwInt32 nMaxValue, gui::CProgressBar* pGaugeComponent, RwBool bNoAnimation /* = FALSE  */)
{
	if( bNoAnimation )
	{
		pGaugeComponent->SetPos( nValue );
		m_fAnimationTime = 0.0f;
		return;
	}

	RwInt32 nCurrentValue = pGaugeComponent->GetPos();

	if( nCurrentValue == nValue )
		return;

	m_fStartValue = (RwReal)nCurrentValue;
	m_fDestValue = (RwReal)nValue;
	m_fCurrentTime = 0.0f;
	RwReal fPixelPerValue = (RwReal)pGaugeComponent->GetWidth() / (RwReal)nMaxValue;
	m_fAnimationTime = (RwReal)abs( m_fDestValue - m_fStartValue ) / m_fSpeed * fPixelPerValue;
}