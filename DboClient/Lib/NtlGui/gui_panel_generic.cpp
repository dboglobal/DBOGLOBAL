#include "gui_precomp.h"
#include "gui_panel_generic.h"
#include "gui_componentoption.h"

START_GUI

CPanel_Generic::CPanel_Generic(CPanel *pSelf)
: m_bFill(false), m_pPanel(pSelf)
{
	m_slotSetAlpha = pSelf->SigSetAlpha().Connect( this, &CPanel_Generic::OnSetAlpha );
	m_slotSetColor = pSelf->SigSetColor().Connect( this, &CPanel_Generic::OnSetColor );
}

VOID CPanel_Generic::OnSetAlpha( BYTE ucAlpha )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;

	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}
}

VOID CPanel_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;

	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}
}



END_GUI