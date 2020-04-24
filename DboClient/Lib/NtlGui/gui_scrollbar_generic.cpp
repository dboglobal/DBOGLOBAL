#include "gui_precomp.h"
#include "gui_scrollbar_generic.h"
#include "gui_scrollbar.h"
#include "gui_component.h"
#include "gui_componentoption.h"
#include "gui_button.h"
#include "mouse.h"
#include "rectangle.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CScrollBar_Generic::CScrollBar_Generic( CScrollBar *pSelf, int nMin,int nMax,int nValue,bool bOrientation,bool bTracking)
:m_bTracking(bTracking),m_bFixedLength(true),m_bDragging(false),m_pScrollbar(pSelf),m_nCaptureLaseOffset(0),m_bInitialized(false),
 m_nButtonWidth( 0 ), m_nButtonHeight( 0 )
//	m_TimerScroll(250),
{
	m_bVertical = !bOrientation;
	SetMinValue(nMin);
	SetMaxValue(nMax);
	SetValue(nValue);

	//----------------------------------------------------------------------
	// signal callback function

	m_SlotSetOptions = m_pScrollbar->SigSetOptions().Connect(this, &CScrollBar_Generic::OnSetOptions);
	m_SlotMouseMove = m_pScrollbar->SigMouseMove().Connect(this, &CScrollBar_Generic::OnMouseMove);
	m_SlotMouseDown = m_pScrollbar->SigMouseDown().Connect(this, &CScrollBar_Generic::OnMouseDown);
	m_SlotMouseUp = m_pScrollbar->SigMouseUp().Connect(this, &CScrollBar_Generic::OnMouseUp);
	m_SlotChildAdd = m_pScrollbar->SigChildAdd().Connect(this, &CScrollBar_Generic::OnChildAdd);
	m_SlotChildRemove = m_pScrollbar->SigChildRemove().Connect(this, &CScrollBar_Generic::OnChildRemove);
	m_SlotResize = m_pScrollbar->SigResize().Connect(this, &CScrollBar_Generic::OnResize);
	//m_SlotTimer = m_TimerScroll.SigTimer().Connect(this, &CScrollBar_Generic::OnTimerScroll);
	m_SlotAlpha = m_pScrollbar->SigSetAlpha().Connect(this, &CScrollBar_Generic::OnSetAlpha);

	m_bInitialized = true;
}

CScrollBar_Generic::~CScrollBar_Generic()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CScrollBar_Generic::GetRange(VOID) const
{
	return m_nMaxValue - m_nMinValue + 1;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CScrollBar_Generic::SetSliderPosition(CRectangle rect)
{
	m_rtRectSlider = rect;
}

VOID CScrollBar_Generic::SetSliderSize(int nWidth,int nHeight)
{
	CRectangle rect = m_pScrollbar->GetPosition();
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


VOID CScrollBar_Generic::SetVertical(bool bEnable)
{
	// throw CL_Error("CL_ScrollBar_Generic::set_vertical() is not implemented");
}

VOID CScrollBar_Generic::SetRange(int nNewMinValue, int nNewMaxValue)
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

VOID CScrollBar_Generic::SetMinValue(int nValue)
{
	m_nMinValue = nValue;
	if(m_nMaxValue < m_nMinValue)
		m_nMaxValue = m_nMinValue;
	
	if(m_nCurValue < m_nMinValue)
		m_nCurValue = m_nMinValue;

	CalculateSlider();
}

VOID CScrollBar_Generic::SetMaxValue(int nValue)
{
	m_nMaxValue = nValue;
	if(m_nMaxValue < m_nMinValue)
		m_nMaxValue = m_nMinValue;
	
	if(m_nCurValue > m_nMaxValue)
		m_nCurValue = m_nMaxValue;

	CalculateSlider();
}

VOID CScrollBar_Generic::SetValue(int nValue, bool bUsingSlider /* = false */, BOOL bDoCallBack /* = TRUE  */)
{
	if(nValue < m_nMinValue) nValue = m_nMinValue;
	if(nValue >= m_nMaxValue) nValue = m_nMaxValue;

	if(m_nCurValue != nValue)
	{
		m_nCurValue = nValue;

		if( bDoCallBack )
		{
			if (bUsingSlider)
			{
				m_SigSliderMoved(m_nCurValue);
			}

			if ((bUsingSlider && m_bTracking) || bUsingSlider == false)
			{
				m_SigValueChanged(m_nCurValue);
			}
		}		
	}

	CalculateSlider();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

VOID CScrollBar_Generic::OnSetOptions(const CComponentOptions &options)
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
	
	//-------------------------------------------------------------------

	if(options.Exists("min"))
		SetMinValue(options.GetValueAsInt("min"));
	else
		SetMinValue(0);

	if(options.Exists("max"))
		SetMaxValue(options.GetValueAsInt("max"));
	else
		SetMaxValue(100);

	if(options.Exists("value"))
		SetValue(options.GetValueAsInt("value"));
	else
		SetValue(m_nMinValue);

	//-------------------------------------------------------------------

	CRectangle rect = m_pScrollbar->GetPosition();
	int nWidth;
	int nHeight;

	if(options.Exists("slider_width"))
		nWidth = options.GetValueAsInt("slider_width");
	else if( m_bVertical )
		nWidth = rect.GetWidth();
	else
		nWidth = rect.GetHeight();

	if(options.Exists("slider_height"))
		nHeight = options.GetValueAsInt("slider_height");
	else if( !m_bVertical )
		nHeight = rect.GetHeight();
	else
		nHeight = rect.GetWidth();

	SetSliderSize(nWidth,nHeight);

	if(options.Exists("button_width"))
		m_nButtonWidth = options.GetValueAsInt("button_width");
	else if( m_bVertical )
		m_nButtonWidth = rect.GetWidth();
	else
		m_nButtonWidth = rect.GetHeight();

	if(options.Exists("button_height"))
		m_nButtonHeight = options.GetValueAsInt("button_height");
	else if( !m_bVertical )
		m_nButtonHeight = rect.GetHeight();
	else
		m_nButtonHeight = rect.GetWidth();
}

VOID CScrollBar_Generic::OnChildAdd(CComponent *pChild)
{
}

VOID CScrollBar_Generic::OnChildRemove(CComponent *pChild)
{
}

VOID CScrollBar_Generic::OnResize(int nOldWidth, int nOldHeight)
{
	CalculateSlider();
}

VOID CScrollBar_Generic::OnMouseDown(const CKey &key)
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
		CRectangle rtPosition = m_pScrollbar->GetPosition();
		CRectangle rtLayout = m_pScrollbar->GetLayoutPosition();
		CRectangle rect;
		rect.left = rtLayout.left-rtPosition.left+m_rtRectSlider.left;
		rect.top = rtLayout.top-rtPosition.top+m_rtRectSlider.top,
		rect.right = rect.left+m_rtRectSlider.GetWidth();
		rect.bottom = rect.top+m_rtRectSlider.GetHeight();
		
		if(rect.PtInRect((const int)key.m_fX,(const int)key.m_fY))
		{
			m_bDragging = true;
			m_pScrollbar->CaptureMouse();
			if(m_bVertical) 
			{
				m_nCaptureLaseOffset = (int)key.m_fY - (m_rtRectSlider.top+rtPosition.GetWidth()); 
			}
			else
			{
				m_nCaptureLaseOffset = (int)key.m_fX - (m_rtRectSlider.left+rtPosition.GetHeight()); 
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
				{
					// MAX 값이 3 이하일 경우 슬라이더 값을 1 씩 움직여준다.
					// by Kell
					/*nThumb = m_nMaxValue;*/
					nThumb = 1;
				}

				CRectangle rtTemp;
				if(m_bVertical)
				{
					rtTemp = rect;
					rtTemp.bottom = m_rtRectSlider.top+rtPosition.GetWidth();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue -= nThumb;
						SetValue((int)nValue, false);
						m_pScrollbar->SliderMovePaint();
						return;
					}

					rtTemp = rect;
					rtTemp.top = m_rtRectSlider.bottom+rtPosition.GetWidth();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue += nThumb;
						SetValue((int)nValue, false);
						m_pScrollbar->SliderMovePaint();
					}
				}
				else
				{
					rtTemp = rect;
					rtTemp.right = m_rtRectSlider.left+rtPosition.GetHeight();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue -= nThumb;
						SetValue((int)nValue, false);
						m_pScrollbar->SliderMovePaint();
						return;
					}

					rtTemp = rect;
					rtTemp.left = m_rtRectSlider.right+rtPosition.GetHeight();
					if(rtTemp.PtInRect((const int)key.m_fX,(const int)key.m_fY))
					{
						nValue += nThumb;
						SetValue((int)nValue, false);
						m_pScrollbar->SliderMovePaint();
					}
				}
			}
		}
	}
}


VOID CScrollBar_Generic::OnMouseUp(const CKey &key)
{
	if(key.m_nID != UD_LEFT_BUTTON)
		return;

	if(m_bDragging)
	{
		m_pScrollbar->ReleaseMouse();
		m_bDragging = false;

		SetValue(m_nCurValue);

		if (m_bTracking == false)
		{
			m_SigValueChanged(m_nCurValue);
		}

		m_SigSliderReleased();
	}
}

VOID CScrollBar_Generic::OnMouseMove(int nKey, int nX, int nY)
{
	if(m_bDragging == false)
		return;
	
	if(m_bVertical)
	{
		CRectangle rtLayout = m_pScrollbar->GetLayoutPosition();
		CRectangle rtPosition = m_pScrollbar->GetPosition();
		CRectangle rtTemp;
		rtTemp.left = 0;
		rtTemp.top = nY-(rtLayout.top-rtPosition.top)-m_nCaptureLaseOffset;
		rtTemp.right = rtTemp.left+m_rtRectSlider.GetWidth();
		rtTemp.bottom = rtTemp.top+m_rtRectSlider.GetHeight();

		int nCurValue = CalculateSliderPosition(rtTemp); 

		if(m_nCurValue != nCurValue)
		{
			SetValue((int)nCurValue, true);
			m_pScrollbar->SliderMovePaint();
		}
	} 
	else
	{
		CRectangle rtLayout = m_pScrollbar->GetLayoutPosition();
		CRectangle rtPosition = m_pScrollbar->GetPosition();
		CRectangle rtTemp;
		rtTemp.left = nX-(rtLayout.left-rtPosition.left)-m_nCaptureLaseOffset;
		rtTemp.top = 0;
		rtTemp.right = rtTemp.left+m_rtRectSlider.GetWidth();
		rtTemp.bottom = rtTemp.top+m_rtRectSlider.GetHeight();

		int nCurValue = CalculateSliderPosition(rtTemp); 

		if(m_nCurValue != nCurValue)
		{
			SetValue((int)nCurValue, true);
			m_pScrollbar->SliderMovePaint();
		}
	}
}

VOID CScrollBar_Generic::OnTimerScroll(VOID)
{
	SetValue(m_nCurValue + m_nScrollDelta);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

VOID CScrollBar_Generic::CalculateSlider(VOID)
{
	if(m_bInitialized == false)
		return;
	if(m_pScrollbar == NULL)
		return;

	// peessitemp : 현재 스크롤 할 것이 없을때의 처리. 임시
	if( m_nMaxValue <= 0 )
	{
		BYTE ucAlpha = m_pScrollbar->GetAlpha();
		if( ucAlpha > 50 )
			m_pScrollbar->SetAlpha( 50, FALSE );

		m_pScrollbar->GetIncreaseButton()->Enable( false );
		m_pScrollbar->GetDecreaseButton()->Enable( false );
	}
	else
	{
		m_pScrollbar->SetAlpha( m_pScrollbar->GetAlpha(), FALSE );
		m_pScrollbar->GetIncreaseButton()->Enable( true );
		m_pScrollbar->GetDecreaseButton()->Enable( true );
	}
	//

	int nLength;

	if(!m_bFixedLength)
	{
		assert(0 && "fixed-length sliders not implemented");
	}
	else
	{
		CRectangle rtLayout = m_pScrollbar->GetLayoutPosition();
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

int CScrollBar_Generic::CalculateSliderPosition(CRectangle rect)
{
	if(m_bInitialized == false)
		return 0;
	if(m_pScrollbar == NULL)
		return 0;

	float fLength;

	if(!m_bFixedLength)
	{
		assert(0 && "fixed-length sliders not implemented");
	}
	else
	{
		CRectangle rtLayout = m_pScrollbar->GetLayoutPosition();
		
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

VOID CScrollBar_Generic::OnSetAlpha( unsigned char ucAlpha )
{
	CSurface* pSurface = NULL;

	std::list<CSurface>::iterator it;

	for( it = m_stlLayoutSurface.begin() ; it != m_stlLayoutSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	for( it = m_stlSliderSurface.begin() ; it != m_stlSliderSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}
}

END_GUI