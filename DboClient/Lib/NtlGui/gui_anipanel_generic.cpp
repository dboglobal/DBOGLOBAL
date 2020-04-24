#include "gui_precomp.h"
#include "gui_anipanel_generic.h"
#include "gui_componentoption.h"

START_GUI

CAniPanel_Generic::CAniPanel_Generic(CAniPanel *pSelf)
: m_bFill(false),m_bAni(true), m_pPanel(pSelf),m_dwDelta(0)
{
	m_dwTime = timeGetTime();
	m_iterAni = m_stlSurface.begin();
}


END_GUI