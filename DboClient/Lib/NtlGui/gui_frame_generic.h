#ifndef __GUI_FRAME_GENERIC_H__
#define __GUI_FRAME_GENERIC_H__

#include "gui_frame.h"

START_GUI

class CFrame_Generic
{
// Construction:
public:
	CFrame_Generic(CFrame *pSelf);
	~CFrame_Generic() {};

// Attributes:
public:
	bool m_bFill;

public:

	std::list<CSurface> m_stlSurface;

// Implementation:
	VOID AddSurface(CSurface surface);
private:

	CSlot m_SlotSetOptions;
	
	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnPaint(VOID);

	CFrame *m_pFrame;
};  

END_GUI

#endif
