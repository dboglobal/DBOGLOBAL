#ifndef __GUI_ANIPANEL_GENERIC_H__
#define __GUI_ANIPANEL_GENERIC_H__

#include "gui_anipanel.h"
#include "gui_surface.h"

START_GUI

class CAniPanel_Generic
{
// Construction:
public:
	CAniPanel_Generic(CAniPanel *pSelf);
	~CAniPanel_Generic() {};

// Attributes:
public:

	bool m_bFill;
	bool m_bAni;
	DWORD m_dwTime;
	DWORD m_dwDelta;
	std::list<CSurface>::iterator m_iterAni;

// Implementation:
public:

	std::list<CSurface> m_stlSurface;

private:

	CAniPanel *m_pPanel;
};  

END_GUI

#endif
