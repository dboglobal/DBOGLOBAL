#ifndef __GUI_SCROLLBAR_GENERIC_H__
#define __GUI_SCROLLBAR_GENERIC_H__

#include "gui_signals.h"
#include "gui_surface.h"
#include "rectangle.h"
#include "position.h"
#include "key.h"

START_GUI

class CScrollBar;
class CButton;
class CInputDevice;
class CComponent;
class CComponentOptions;

class CScrollBar_Generic
{
// Construction:
public:
	CScrollBar_Generic(	CScrollBar *self, int nMin, int nMax, int nValue, bool bOrientation, bool bTracking);
	~CScrollBar_Generic();

// Attributes:
public:

	int GetRange(VOID) const;

	int m_nMinValue;					  // slider bar min value    
	int m_nMaxValue;					  // slider bar max value
	int m_nCurValue;					  // slider bar current value

	bool m_bTracking;
	bool m_bVertical;						// ver or hor
	bool m_bFixedLength;

	bool m_bDragging;

	CRectangle m_rtRectSlider;  // slider bar rectangle

	int m_nButtonWidth;
	int	m_nButtonHeight;

	int m_nMinSliderLength;
	int m_nFixedSliderLength;
			
// Operations:
public:

	VOID SetSliderPosition(CRectangle rect);
	VOID SetSliderSize(int nWidth,int nHeight);
	VOID SetVertical(bool bEnable);
	VOID SetRange(int nMinValue, int nMaxValue);
	VOID SetMinValue(int nValue);
	VOID SetMaxValue(int nValue);
	VOID SetValue(int nValue, bool bUsingSlider = false, BOOL bDoCallBack = TRUE );

// Signals:
public:

	CSignal_v1<int> m_SigValueChanged;
	CSignal_v0 m_SlgSliderPressed;
	CSignal_v1<int> m_SigSliderMoved;
	CSignal_v0 m_SigSliderReleased;
	
// Slots:
private:

	CSlot m_SlotSetOptions;
//	CSlot m_SlotTimer;
	CSlot m_SlotMouseDown,m_SlotMouseUp,m_SlotMouseMove;
	CSlot m_SlotButtonDecrease, m_SlotButtonIncrease;
	CSlot m_SlotChildAdd, m_SlotChildRemove;
	CSlot m_SlotResize;
	CSlot m_SlotAlpha;


// Callbacks:
private:

	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnChildAdd(CComponent *pChild);
	VOID OnChildRemove(CComponent *child);
	VOID OnMouseDown(const CKey &key);
	VOID OnMouseUp(const CKey &key);
	VOID OnResize(int nOldWidth, int nOldHeight);
	VOID OnMouseMove(int nKey, int nX, int nY );
	VOID OnTimerScroll(VOID);
	VOID OnSetAlpha( unsigned char ucAlpha );

// Implementation:
private:

	VOID CalculateSlider(VOID);
	int CalculateSliderPosition(CRectangle rect);

//	CL_LayoutManager layout;
	CScrollBar *m_pScrollbar;

	bool m_bMouseCaptured;
	int m_nCaptureLaseOffset;

//	CTimer m_TimerScroll;
	int m_nScrollDelta;

	bool m_bInitialized;

public:

	std::list<CSurface> m_stlSliderSurface;
	std::list<CSurface> m_stlLayoutSurface;
};

END_GUI

#endif
