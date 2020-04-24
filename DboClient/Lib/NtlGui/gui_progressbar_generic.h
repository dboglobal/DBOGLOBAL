#ifndef __GUI_PROGRESSBAR_GENERIC_H__
#define __GUI_PROGRESSBAR_GENERIC_H__

#include "gui_progressbar.h"
#include "gui_surface.h"

START_GUI

class CProgressBar_Generic
{
// Construction:
public:
	CProgressBar_Generic(
		CProgressBar *pSelf,
		int nMin,
		int nMax,
		int nPos);

	~CProgressBar_Generic() {};

// Attributes:
public:

	int m_nMin;
	int m_nMax;
	int m_nPos;
	bool m_bVertical;
	bool m_bReverseOrder;

// Implementation:
public:

	std::list<CSurface> m_stlSurface;
	std::list<CSurface> m_stlBUSurface;

//: Signals
private:

	VOID OnSetOptions(const CComponentOptions& options);
	
	CSlot m_SlotSetOption;

private:

	CProgressBar *m_pProgressBar;
};  

END_GUI

#endif
