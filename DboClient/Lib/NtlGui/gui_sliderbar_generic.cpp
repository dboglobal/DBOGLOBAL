#include "gui_precomp.h"
#include "gui_sliderbar_generic.h"
#include "gui_sliderbar.h"
#include "gui_component.h"
#include "gui_componentoption.h"
#include "mouse.h"
#include "rectangle.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CSliderBar_Generic::CSliderBar_Generic(
	CSliderBar *pSelf,
	int nMin,
	int nMax,
	int nValue,
	bool bOrientation,
	bool bTracking)
:
	m_bTracking(bTracking),
	m_bFixedLength(false),
	m_bDragging(false),
	m_pSliderBar(pSelf),
	m_nCaptureLaseOffset(0),
//	m_TimerScroll(250),
	m_bInitialized(false)
{
	m_bVertical = !bOrientation;
	SetMinValue(nMin);
	SetMaxValue(nMax);
	SetValue(nValue);

	//----------------------------------------------------------------------
	// slider bar size °è»ê 
	if(m_bVertical)
		SetSliderSize(m_pSliderBar->GetWidth(),m_pSliderBar->GetWidth());
	else
		SetSliderSize(m_pSliderBar->GetHeight(),m_pSliderBar->GetHeight());

	//----------------------------------------------------------------------
	// signal callback function

	m_SlotSetOptions = m_pSliderBar->SigSetOptions().Connect(
		this, &CSliderBar_Generic::OnSetOptions);
	
//	m_SlotTimer = m_TimerScroll.SigTimer().Connect(
//		this, &CSliderBar_Generic::OnTimerScroll);

	m_SlotMouseMove = m_pSliderBar->SigMouseMove().Connect(
		this, &CSliderBar_Generic::OnMouseMove);
	
	m_SlotMouseDown = m_pSliderBar->SigMouseDown().Connect(
		this, &CSliderBar_Generic::OnMouseDown);
	m_SlotMouseUp = m_pSliderBar->SigMouseUp().Connect(
		this, &CSliderBar_Generic::OnMouseUp);
	m_SlotChildAdd = m_pSliderBar->SigChildAdd().Connect(
		this, &CSliderBar_Generic::OnChildAdd);
	m_SlotChildRemove = m_pSliderBar->SigChildRemove().Connect(
		this, &CSliderBar_Generic::OnChildRemove);
	m_SlotResize = m_pSliderBar->SigResize().Connect(
		this, &CSliderBar_Generic::OnResize);

	m_bInitialized = true;
}

CSliderBar_Generic::~CSliderBar_Generic()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CSliderBar_Generic::GetRange(VOID) const
{
	return m_nMaxValue - m_nMinValue + 1;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CSliderBar_Generic::SetSliderPosition(CRectangle rect)
{
	m_rtRectSlider = rect;
}

VOID CSliderBar_Generic::SetSliderSize(int nWidth,int nHeight)
{
	CRectangle rect = m_pSliderBar->GetPosition();
	int nSliderWidth=nWidth,nSliderHeight=nHeight;

	if(m_bVertical)
	{
		if(nSliderWidth > rect.GetWidth())
			nSliderWidth = rect.GetWidth();

		m_rtRectSlider.left = (rect.GetWidth()-nSliderWidth)/2;
		m_rtRectSlider.right = m_rtRectSlider.left+nSliderWidth;
		m_rtRectSlider.top = 0;
		m_rtRectSlider.bottom = nSliderHeight; 
	}
	else
	{
		if(nSliderHeight > rect.GetHeight())
			nSliderHeight = rect.GetHeight();

		m_rtRectSlider.left = 0;
		m_rtRectSlider.right = nSliderWidth; 
		m_rtRectSlider.top = (rect.GetHeight()-nSliderHeight)/2;
		m_rtRectSlider.bottom = m_rtRectSlider.top+nSliderHeight;
	}

	CalculateSlider();
}


VOID CSliderBar_Generic::SetVertical(bool bEnable)
{
	// throw CL_Error("CL_ScrollBar_Generic::set_vertical() is not implemented");
}

VOID CSliderBar_Generic::SetRange(int nNewMinValue, int nNewMaxValue)
{
	m_nMinValue = nNewMinValue;
	m_nMaxValue = nNewMaxValue;

	if(m_nMaxValue < m_nMinValue)
		m_nMaxValue = m_nMinValue;
	
	if(m_nCurValue < m_nMinValue)
		m_nCurValue = m_nMinValue;
	if(m_nCurValue > m_nMaxValue)
		m_nCurValue = m_nMaxValue;

	CalculateSlider();
}

VOID CSliderBar_Generic::SetMinValue(int nValue)
{
	m_nMinValue = nValue;
	if(m_nMaxValue < m_nMinValue)
		m_nMaxValue = m_nMinValue;
	
	if(m_nCurValue < m_nMinValue)
		m_nCurValue = m_nMinValue;

	CalculateSlider();
}

VOID CSliderBar_Generic::SetMaxValue(int nValue)
{
	m_nMaxValue = nValue;
	if(m_nMaxValue < m_nMinValue)
		m_nMaxValue = m_nMinValue;
	
	if(m_nCurValue > m_nMaxValue)
		m_nCurValue = m_nMaxValue;

	CalculateSlider();
}

VOID CSliderBar_Generic::SetValue(int nValue, bool bUsingSlider)
{
	if(nValue < m_nMinValue) nValue = m_nMinValue;
	if(nValue >= m_nMaxValue) nValue = m_nMaxValue;

	if(m_nCurValue != nValue)
	{
		m_nCurValue = nValue;

		if(bUsingSlider)
			m_SigSliderMoved(m_nCurValue);

		if((bUsingSlider && m_bTracking) || bUsingSlider == false)
			m_SigValueChanged(m_nCurValue);
	}

	CalculateSlider();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CSliderBar_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("orientation"))
	{
		std::string str = options.GetValue("orientation");
		if(str == "horizontal" || str == "1")
			m_bVertical = false;
		else if(str == "vertical" || str == "0")
			m_bVertical = true;
		else
			assert(0 && "scroll bar orientation value error");
	}
	else
		m_bVertical = false;

	//-------------------------------------------------------------------

	if(options.Exists("min"))
		SetMinValue(options.GetValueAsInt("min"));
	else
		SetMinValue(0);

	if(options.Exists("max"))
		SetMaxValue(options.GetValueAsInt("max"));
	else
		SetMaxValue(m_nMinValue);

	if(options.Exists("value"))
		SetValue(options.GetValueAsInt("value"));
	else
		SetValue(m_nMinValue);

	//-------------------------------------------------------------------

	CRectangle rect = m_pSliderBar->GetPosition();
	int nSliderWidth=0,nSliderHeight=0;

	if(options.Exists("slider_width"))
		nSliderWidth = options.GetValueAsInt("slider_width");
	if(options.Exists("slider_height"))
		nSliderHeight = options.GetValueAsInt("slider_height");

	SetSliderSize(nSliderWidth,nSliderHeight);
}

VOID CSliderBar_Generic::OnChildAdd(CComponent *pChild)
{
}

VOID CSliderBar_Generic::OnChildRemove(CComponent *pChild)
{
}

VOID CSliderBar_Generic::OnResize(int nOldWidth, int nOldHeight)
{
	CalculateSlider();
}

VOID CSliderBar_Generic::OnMouseDown(const CKey &key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

//	if(key.id == CL_MOUSE_WHEELUP)
//	{
//		set_value(cur_value - 5);
//		return;
//	}

//	if(key.id == CL_MOUSE_WHEELDOWN)
//	{
//		set_value(cur_value + 5);
//		return;
//	}

	// Mouse input

	if(m_bDragging == false)
	{
		CRectangle rtPosition = m_pSliderBar->GetPosition();
		CRectangle rtLayout = m_pSliderBar->GetLayoutPosition();
		CRectangle rect;
		rect.left = rtLayout.left-rtPosition.left+m_rtRectSlider.left;
		rect.top = rtLayout.top-rtPosition.top+m_rtRectSlider.top,
		rect.right = rect.left+m_rtRectSlider.GetWidth();
		rect.bottom = rect.top+m_rtRectSlider.GetHeight();
		
		if(rect.PtInRect((const int)key.m_fX,(const int)key.m_fY))
		{
			m_bDragging = true;
			m_pSliderBar->CaptureMouse();
			if(m_bVertical) 
			{
				m_nCaptureLaseOffset = (int)key.m_fY - m_rtRectSlider.top;
			}
			else
			{
				m_nCaptureLaseOffset = (int)key.m_fX - m_rtRectSlider.left;
			}
			m_SlgSliderPressed();
		}
		else
		{
			rect.left = rtLayout.left-rtPosition.left;
			rect.top = rtLayout.top-rtPosition.top;
			rect.right = rect.left+rtLayout.GetWidth();
			rect.bottom = rect.top+rtLayout.GetHeight();
			if(rect.PtInRect((const int)key.m_fX,(const int)key.m_fY))
			{
				int nThumbDiv = (3*m_nMaxValue/10);
				if(nThumbDiv == 0)
					nThumbDiv = 3;
				int nValue = m_nCurValue;
				int nThumb;
				if(m_nMaxValue>3)
					nThumb = m_nMaxValue/nThumbDiv;
				else
					nThumb = m_nMaxValue;

				CRectangle rtTemp;
				if(m_bVertical)
				{
					rtTemp = rect;
					rtTemp.bottom = m_rtRectSlider.top+rtPosition.GetWidth();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue -= nThumb;
						SetValue((int)nValue, false);
						m_pSliderBar->SliderMovePaint();
						return;
					}

					rtTemp = rect;
					rtTemp.top = m_rtRectSlider.bottom+rtPosition.GetWidth();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue += nThumb;
						SetValue((int)nValue, false);
						m_pSliderBar->SliderMovePaint();
					}
				}
				else
				{
					rtTemp = rect;
					rtTemp.right = m_rtRectSlider.left;
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue -= nThumb;
						SetValue((int)nValue, false);
						m_pSliderBar->SliderMovePaint();
						return;
					}

					rtTemp = rect;
					rtTemp.left = m_rtRectSlider.right;
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue += nThumb;
						SetValue((int)nValue, false);
						m_pSliderBar->SliderMovePaint();
					}
				}
			}
		}
	}
}


VOID CSliderBar_Generic::OnMouseUp(const CKey &key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

	if(m_bDragging)
	{
		m_pSliderBar->ReleaseMouse();
		m_bDragging = false;

		SetValue(m_nCurValue);

		if(m_bTracking == false)
			m_SigValueChanged(m_nCurValue);
		m_SigSliderReleased();
	}
}

VOID CSliderBar_Generic::OnMouseMove(int nKey, int nX, int nY)
{
	if(m_bDragging == false)
		return;
	
	if(m_bVertical)
	{
		CRectangle rtLayout = m_pSliderBar->GetLayoutPosition();
		CRectangle rtPosition = m_pSliderBar->GetPosition();
		CRectangle rtTemp;
		rtTemp.left = 0;
		rtTemp.top = nY-(rtLayout.top-rtPosition.top)-m_nCaptureLaseOffset;
		rtTemp.right = rtTemp.left+m_rtRectSlider.GetWidth();
		rtTemp.bottom = rtTemp.top+m_rtRectSlider.GetHeight();

		int nCurValue = CalculateSliderPosition(rtTemp); 

		if(m_nCurValue != nCurValue)
		{
			SetValue((int)nCurValue, true);
			m_pSliderBar->SliderMovePaint();
		}
	} 
	else
	{
		CRectangle rtLayout = m_pSliderBar->GetLayoutPosition();
		CRectangle rtPosition = m_pSliderBar->GetPosition();
		CRectangle rtTemp;
		rtTemp.left = nX-(rtLayout.left-rtPosition.left)-m_nCaptureLaseOffset;
		rtTemp.top = 0,
		rtTemp.right = rtTemp.left+m_rtRectSlider.GetWidth();
		rtTemp.bottom = rtTemp.top+m_rtRectSlider.GetHeight();

		int nCurValue = CalculateSliderPosition(rtTemp); 

		if(m_nCurValue != nCurValue)
		{
			SetValue((int)nCurValue, true);
			m_pSliderBar->SliderMovePaint();
		}
	}
}

VOID CSliderBar_Generic::OnTimerScroll(VOID)
{
	SetValue(m_nCurValue + m_nScrollDelta);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

VOID CSliderBar_Generic::CalculateSlider(VOID)
{
	if(m_bInitialized == false)
		return;
	if(m_pSliderBar == NULL)
		return;

	int nLength;

	if(m_bFixedLength)
	{
		assert(0 && "fixed-length sliders not implemented");
	}
	else
	{
		CRectangle rtLayout = m_pSliderBar->GetLayoutPosition();
		if(m_bVertical)
			nLength = (int)(((float)rtLayout.GetHeight()-(float)m_rtRectSlider.GetHeight())*((float)m_nCurValue/(float)m_nMaxValue));
		else
			nLength = (int)(((float)rtLayout.GetWidth()-(float)m_rtRectSlider.GetWidth())*((float)m_nCurValue/(float)m_nMaxValue));
	}

	CRectangle rtRectSlider = m_rtRectSlider;

	if(m_bVertical)
	{
		rtRectSlider.top = nLength;
		rtRectSlider.bottom = nLength + m_rtRectSlider.GetHeight();
	}
	else
	{
		rtRectSlider.left = nLength;
		rtRectSlider.right = nLength + m_rtRectSlider.GetWidth();
	}

	m_rtRectSlider = rtRectSlider;
}

int CSliderBar_Generic::CalculateSliderPosition(CRectangle rect)
{
	if(m_bInitialized == false)
		return 0;
	if(m_pSliderBar == NULL)
		return 0;

	float fLength;

	if(m_bFixedLength)
	{
		assert(0 && "fixed-length sliders not implemented");
	}
	else
	{
		CRectangle rtLayout = m_pSliderBar->GetLayoutPosition();
		
		if(m_bVertical)
		{
			fLength = ((float)rtLayout.GetHeight()-(float)m_rtRectSlider.GetHeight())/(float)m_nMaxValue;
			fLength = (float)rect.top/(float)fLength; 
		}
		else
		{
			fLength = ((float)rtLayout.GetWidth()-(float)m_rtRectSlider.GetWidth())/(float)m_nMaxValue;
			fLength = (float)rect.left/(float)fLength; 
		}
	}

	return (int)fLength;
}

END_GUI
