#ifndef __GUI_SLIDERBAR_H__
#define __GUI_SLIDERBAR_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CComponentOptions;
class CSliderBar_Generic;

//: SliderBar component
class CSliderBar : public CComponent
{

//! Construction:
public:

	//: ScrollBar Constructor
	CSliderBar(CComponent *parent,
						CSurfaceManager *pSurfaceManager);

	CSliderBar(const CRectangle &rtPos,
						int nMin,
						int nMax,
						bool bOrientation,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	CSliderBar(const CRectangle &rtPos,
						bool bOrientation,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	//: SliderBar Destructor
	virtual ~CSliderBar();

//! Attributes:
public:

	//: Returns the orientation of the scroll bar.
	//- Return true if sliderbar is vertical, false otherwise.
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

	//: Returns layout surface
	std::list<CSurface>* GetSurface(VOID);

	//: Returns slider surface 
	std::list<CSurface>* GetSurfaceSlider(VOID);

//! Operations:
public:
	//: Sets the length-mode of the slider.
	//- If enable is true, the slider is constant in size. (See set_slider_length()).
	//- If enable is false, the slider is resized automatically based on the range.
	VOID SetFixedLengthMode(bool bEnable = true);

	//: Sets the current slider size
	VOID SetSliderSize(int nWidth,int nHeight);

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
	VOID SetValue(int nValue);

	//: Increases current value with step.
	VOID Increase(int nStep = 1);

	//: Decreases current value with step.
	VOID Decrease(int nStep = 1);

	//: Layout surface
	VOID AddSurface(CSurface surface);

	//: Slider surface 
	VOID AddSurfaceSlider(CSurface surface);

//! Signals:
public:
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

//! Implementation:

public:

	//: thumb track or slider bar를 잡고 이동했을 경우 호출되는 function
	VOID SliderMovePaint(VOID);

private:

	VOID ResizeSurface(VOID);
	VOID ResizeSurfaceSlider(VOID);
	
private:
//! callback function:
	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnResize(int nOldWidth, int nOldHeight);
	VOID OnMove(int nX,int nY);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotResize;
	CSlot m_SlotMove;

	int m_nScrollDelta;

	//: slider bar rendering position 계산.
	CRectangle CalculateSliderPaint(VOID);
	
	CSliderBar(const CSliderBar &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CSliderBar_Generic *m_pImpl;
};

END_GUI

#endif
