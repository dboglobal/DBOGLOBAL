////////////////////////////////////////////////////////////////////////////////
// File: GaugeAnimation.h
// Desc:
//
// 2008.08.14 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GAUGE_ANIMATION_H__
#define __GAUGE_ANIMATION_H__

#include "gui_progressbar.h"

class CGaugeAnimation
{
public:
	CGaugeAnimation(VOID);
	~CGaugeAnimation(VOID);

	VOID	Init(VOID);
	VOID	Update( RwReal fElapsed, gui::CProgressBar* pGaugeComponent );

	VOID	SetValue( RwInt32 nValue, RwInt32 nMaxValue, gui::CProgressBar* pGaugeComponent, RwBool bNoAnimation = FALSE );
	VOID	SetSpeed( RwReal fSpeed ) { m_fSpeed = fSpeed; }

private:
	RwReal				m_fAnimationTime;		
	RwReal				m_fCurrentTime;		
	RwReal				m_fStartValue;
	RwReal				m_fDestValue;			
	RwReal				m_fSpeed;				// pixel per second;
};

#endif//__GAUGE_ANIMATION_H__