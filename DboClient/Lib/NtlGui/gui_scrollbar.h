////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CScrollBar
// Desc: Text를 표현한다.
//
// Created by jhssugi@ntl-inc.com	
//
// To Be Desired :	
//	1. Code cleanup.
////////////////////////////////////////////////////////////////////////////////
#ifndef __GUI_SCROLLBAR_H__
#define __GUI_SCROLLBAR_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CButton;
class CComponentOptions;
class CScrollBar_Generic;

//: ScrollBar component
class CScrollBar : public CComponent
{
public:
//! Construction:
	
	//: ScrollBar Constructor
	CScrollBar(CComponent *parent,
						CSurfaceManager *pSurfaceManager);

	CScrollBar(const CRectangle &rtPos,
						int nMin,
						int nMax,
						bool bOrientation,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	CScrollBar(const CRectangle &rtPos,
						bool bOrientation,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	//: ScrollBar Destructor
	virtual ~CScrollBar();

//! Attributes:

	//: Returns the orientation of the scroll bar.
	//- Return true if scrollbar is vertical, false otherwise.
	bool IsVertical(VOID) const;

	//: Returns true if tracking is enabled, or false if tracking is disabled. 
	//: Tracking is initially enabled. See set_tracking for more info on tracking.
	bool IsTracking(VOID) const;

	//: Returns the current slider position
	CRectangle GetSliderPosition(VOID);

	//: Returns the current layout position

	CRectangle GetLayoutPosition(VOID);

	//: Returns the current min value.
	int GetMinValue(VOID) const;

	//: Returns the current max value.
	int GetMaxValue(VOID) const;

	//: Returns the range (delta max-min).
	int GetRange(VOID) const;

	//: Returns the current range control value. 
	int GetValue(VOID) const;

	//: Returns true if the user has clicked the mouse on the slider and is currently dragging it, or false if not. 
	bool IsDraggingSlider(VOID) const;

	//: Returns true if this is a fixed length slider, false otherwise.
	bool IsFixedLengthSlider(VOID) const;

	//: Returns the scroll bar slider rectangle. 
	CRectangle& GetSliderRect(VOID) const;

	//: Returns the minimum size of the slider.
	//: Only usable in variable-length sliders.
	int GetMinSliderLength(VOID) const;

	//: Returns increase button pointer
	CButton* GetIncreaseButton(VOID);

	//: Returns decrease button pointer
	CButton* GetDecreaseButton(VOID);

	//: Returns slider surface 
	std::list<CSurface>* GetSurfaceSlider(VOID);

	//: Returns layout surface
	std::list<CSurface>* GetSurfaceLayout(VOID);


//! Operations:

	//: Sets the length-mode of the slider.
	//- If enable is true, the slider is constant in size. (See set_slider_length()).
	//- If enable is false, the slider is resized automatically based on the range.
	VOID SetFixedLengthMode(bool bEnable = true);

	//: Sets the current slider size
	VOID SetSliderSize(int nWidth,int nHeight);

	//: Sets the Button size
	VOID SetButtonSize(int nWidth,int nHeight);

	//: Sets the current size of the slider.
	//: Only usable in fixed-length sliders.
	VOID SetSliderLength(int nLength);

	//: Sets the minimum size of the slider.
	//: Only usable in variable-length sliders.
	VOID SetMinSliderLength(int nLength);

	//: Sets the scroll bar orientation to vertical if enable is true, horizontal otherwise.
	VOID SetVertical(bool bEnable = true);

	//: Enables scroll bar tracking if enable is true, or disables tracking if enable is false. 
	//: <p>If tracking is enabled (the default), the scroll bar emits the
	//: sig_value_changed() signal while the slider is being dragged. If tracking is
	//: disabled, the scroll bar emits the sig_value_changed() only when the user
	//: releases the mouse button after moving the slider.</p>
	VOID SetTracking(bool bEnable = true);

	//: Sets the range between min_value and max_value. 
	//: If max_value is smaller than min_value, max_value is set to min_value.
	VOID SetRange(int nMinValue, int nMaxValue);

	//: Sets the range to start from value. 
	//: If current maximum value is less than the minvalue, maxvalue is set to minvalue.
	VOID SetMinValue(int nValue);

	//: Sets the range to end at value. 
	//: If current maximum value is less than the minvalue, maxvalue is set to minvalue.
	VOID SetMaxValue(int nValue);

	//: Sets the range controls value to value and forces it to be within the legal range. 
	VOID SetValue(int nValue, BOOL bDoCallBack = TRUE);

	//: Increases current value with step.
	VOID Increase(int nStep = 1);

	//: Decreases current value with step.
	VOID Decrease(int nStep = 1);

	//: Slider surface 
	VOID AddSurfaceSlider(CSurface surface);
	
	//: Layout surface
	VOID AddSurfaceLayout(CSurface surface);

//! Signals:

	//: This signal is emitted when the scroll bar value has changed, with the new scroll bar value as an argument. 
	//: See set_tracking() for specifics on this signal.
	CSignal_v1<int>& SigValueChanged(VOID);

	//: This signal is emitted when the user presses the slider with the mouse. 
	CSignal_v0& SigSliderPressed(VOID);

	//: This signal is emitted when the slider is moved by the user, with the new scroll bar value as an argument. 
	//: This signal is emitted even when tracking is turned off. 
	CSignal_v1<int>& SigSliderMoved(VOID);

	//: This signal is emitted when the user releases the slider with the mouse. 
	CSignal_v0& SigSliderReleased(VOID);

	//: 
	VOID ResizeSurfaceSlider(VOID);
	VOID ResizeSurfaceLayout(VOID);
	VOID ResizeButton(VOID);

private:
//! Implementation:

	//: thumb track or slider bar를 잡고 이동했을 경우 호출되는 function
	VOID SliderMovePaint(VOID);
	

private:
//! callback function:
	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnStartScroll( CComponent* pComponent, int nDelta);
	VOID OnStopScroll( CComponent* pComponent );
	VOID OnResize(int nOldWidth, int nOldHeight);
	VOID OnMove(int nX,int nY);
	
	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotResize;
	CSlot m_SlotIncPressed;
	CSlot m_SlotDecPressed;
	CSlot m_SlotIncReleased;
	CSlot m_SlotDecReleased;
	CSlot m_SlotMove;
	
	int m_nScrollDelta;
	CButton *m_pBtnInc;
	CButton *m_pBtnDec;

	//: slider bar rendering position 계산.
	CRectangle CalculateSliderPaint(VOID);
	
	CScrollBar(const CScrollBar &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CScrollBar_Generic *m_pImpl;
	
	friend class CScrollBar_Generic;
};

END_GUI

#endif
