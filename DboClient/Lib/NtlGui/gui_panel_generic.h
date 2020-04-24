#ifndef __GUI_PANEL_GENERIC_H__
#define __GUI_PANEL_GENERIC_H__

#include "gui_panel.h"
#include "gui_surface.h"

START_GUI

class CPanel_Generic
{
// Construction:
public:
	CPanel_Generic(CPanel *pSelf);
	~CPanel_Generic() {};

// Attributes:
public:
	bool m_bFill;

// Implementation:
public:

	std::list<CSurface> m_stlSurface;

private:
	//! CallBack
	VOID	OnSetAlpha( BYTE ucAlpha );
	VOID	OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	CSlot	m_slotSetColor;
	CSlot	m_slotSetAlpha;

	CPanel *m_pPanel;
};  

END_GUI

#endif
