#include "gui_precomp.h"
#include "gui_scrollbar.h"
#include "gui_componentoption.h"
#include "gui_scrollbar_generic.h"
#include "gui_button.h"
#include "gui_renderer.h"

START_GUI

CScrollBar::CScrollBar(CComponent *pParent, CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager), m_pImpl(0),m_pBtnInc(0),m_pBtnDec(0)
{
	m_pImpl = NTL_NEW CScrollBar_Generic(this, 0, 100, 0, false, false);

	//------------------------------------------
	// decrease and increase button을 생성한다.

	m_pBtnInc = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);
	m_pBtnDec = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(this, &CScrollBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(this, &CScrollBar::OnPaint);
	m_SlotResize = SigResize().Connect(	this,&CScrollBar::OnResize);
	m_SlotMove = SigMove().Connect(this,&CScrollBar::OnMove);
	m_SlotIncPressed = m_pBtnInc->SigPressed().Connect(this, &CScrollBar::OnStartScroll,1);
	m_SlotDecPressed = m_pBtnDec->SigPressed().Connect(this, &CScrollBar::OnStartScroll,-1);
	m_SlotIncReleased = m_pBtnInc->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
	m_SlotDecReleased = m_pBtnDec->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
}

CScrollBar::CScrollBar(const CRectangle &rtPos, int nMin, int nMax, bool bOrientation, CComponent *pParent,										CSurfaceManager *pSurfaceManager)
: CComponent(rtPos, pParent,pSurfaceManager), m_pImpl(0),m_pBtnInc(0),m_pBtnDec(0)
{
	m_pImpl = NTL_NEW CScrollBar_Generic(this, nMin, nMax, 0, bOrientation, false);

	//------------------------------------------
	// decrease and increase button을 생성한다.

	m_pBtnInc = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);
	m_pBtnDec = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(this, &CScrollBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(this, &CScrollBar::OnPaint);
	m_SlotResize = SigResize().Connect(	this,&CScrollBar::OnResize);
	m_SlotMove = SigMove().Connect(this,&CScrollBar::OnMove);
	m_SlotIncPressed = m_pBtnInc->SigPressed().Connect(this, &CScrollBar::OnStartScroll,1);
	m_SlotDecPressed = m_pBtnDec->SigPressed().Connect(this, &CScrollBar::OnStartScroll,-1);
	m_SlotIncReleased = m_pBtnInc->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
	m_SlotDecReleased = m_pBtnDec->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
		
}

CScrollBar::CScrollBar(const CRectangle &ptPos,bool bOrientation,CComponent *pParent,CSurfaceManager *pSurfaceManager)
: CComponent(ptPos, pParent,pSurfaceManager), m_pImpl(0),m_pBtnInc(0),m_pBtnDec(0)
{
	m_pImpl = NTL_NEW CScrollBar_Generic(this, 0, 100, 0, bOrientation, false);

	//------------------------------------------
	// decrease and increase button을 생성한다.

	m_pBtnInc = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);
	m_pBtnDec = NTL_NEW CButton(CRectangle(0,0,0,0),"",this,pSurfaceManager);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(this, &CScrollBar::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(this, &CScrollBar::OnPaint);
	m_SlotResize = SigResize().Connect(	this,&CScrollBar::OnResize);
	m_SlotMove = SigMove().Connect(this,&CScrollBar::OnMove);
	m_SlotIncPressed = m_pBtnInc->SigPressed().Connect(this, &CScrollBar::OnStartScroll,1);
	m_SlotDecPressed = m_pBtnDec->SigPressed().Connect(this, &CScrollBar::OnStartScroll,-1);
	m_SlotIncReleased = m_pBtnInc->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
	m_SlotDecReleased = m_pBtnDec->SigReleased().Connect(this, &CScrollBar::OnStopScroll);
}

CScrollBar::~CScrollBar()
{
	NTL_DELETE( m_pBtnInc );
	NTL_DELETE( m_pBtnDec );

	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


bool CScrollBar::IsVertical(VOID) const
{
	return m_pImpl->m_bVertical;
}

bool CScrollBar::IsTracking(VOID) const
{
	return m_pImpl->m_bTracking;
}

CRectangle CScrollBar::GetSliderPosition(VOID)
{
	return m_pImpl->m_rtRectSlider;
}

CRectangle CScrollBar::GetLayoutPosition(VOID)
{
	if(m_pImpl == NULL)
		return CRectangle(0,0,0,0);

	CRectangle rect = GetPosition();
	CRectangle rtInc,rtDec;
	if(m_pBtnDec == NULL || m_pBtnInc == NULL)
		return rect;

	rtInc = m_pBtnInc->GetPosition();
	rtDec = m_pBtnDec->GetPosition();
	
	if(IsVertical())
	{
		rect.top += rtDec.bottom;
		rect.bottom -= rtInc.GetHeight();
	}
	else
	{
		rect.left += rtDec.right;
		rect.right -= rtInc.GetWidth(); 
	}

	return rect;
}


int CScrollBar::GetMinValue(VOID) const
{
	return m_pImpl->m_nMinValue;
}

int CScrollBar::GetMaxValue(VOID) const
{
	return m_pImpl->m_nMaxValue;
}

int CScrollBar::GetValue(VOID) const
{
	return m_pImpl->m_nCurValue;
}

int CScrollBar::GetRange(VOID) const
{
	return m_pImpl->GetRange();
}

bool CScrollBar::IsDraggingSlider(VOID) const
{
	return m_pImpl->m_bDragging;
}

CRectangle& CScrollBar::GetSliderRect(VOID) const
{
	return m_pImpl->m_rtRectSlider;
}

bool CScrollBar::IsFixedLengthSlider(VOID) const
{
	return m_pImpl->m_bFixedLength;
}

int CScrollBar::GetMinSliderLength(VOID) const
{
	return m_pImpl->m_nMinSliderLength;
}

//: Returns increase button pointer
CButton* CScrollBar::GetIncreaseButton(VOID)
{
	return m_pBtnInc;
}

//: Returns decrease button pointer
CButton* CScrollBar::GetDecreaseButton(VOID)
{
	return m_pBtnDec;
}

//: Returns slider surface 
std::list<CSurface>* CScrollBar::GetSurfaceSlider(VOID)
{
	return &m_pImpl->m_stlSliderSurface;
}

//: Returns layout surface
std::list<CSurface>* CScrollBar::GetSurfaceLayout(VOID)
{
	return &m_pImpl->m_stlLayoutSurface;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CScrollBar::SetVertical(bool bEnable)
{
	m_pImpl->SetVertical(bEnable);
}

VOID CScrollBar::SetTracking(bool bEnable)
{
	m_pImpl->m_bTracking = bEnable;
}

VOID CScrollBar::SetRange(int nMinValue, int nMaxValue)
{
	m_pImpl->SetRange(nMinValue, nMaxValue);
}

VOID CScrollBar::SetMinValue(int nValue)
{
	m_pImpl->SetMinValue(nValue);
}

VOID CScrollBar::SetMaxValue(int nValue)
{
	m_pImpl->SetMaxValue(nValue);
}

VOID CScrollBar::SetValue(int nValue, BOOL bDoCallBack /* = TRUE */)
{
	m_pImpl->SetValue(nValue, false, bDoCallBack);

	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; the!=stlPSurface->end() ; the++)
	{
		(*the).m_SnapShot.rtRect = rtSlider;
	}
}

VOID CScrollBar::Increase(int nStep)
{
	m_pImpl->SetValue(m_pImpl->m_nCurValue + nStep);
}

VOID CScrollBar::Decrease(int nStep)
{
	m_pImpl->SetValue(m_pImpl->m_nCurValue - nStep);
}

VOID CScrollBar::SetFixedLengthMode(bool bEnable)
{
	m_pImpl->m_bFixedLength = bEnable;
}

VOID CScrollBar::SetSliderSize(int nWidth,int nHeight)
{
	m_pImpl->SetSliderSize(nWidth,nHeight);
}

VOID CScrollBar::SetButtonSize(int nWidth, int nHeight)
{
	m_pImpl->m_nButtonWidth = nWidth;
	m_pImpl->m_nButtonHeight = nHeight;

	ResizeButton();
	ResizeSurfaceLayout();
	ResizeSurfaceSlider();
}

VOID CScrollBar::SetSliderLength(int nLength)
{
	m_pImpl->m_nFixedSliderLength = nLength;
}

VOID CScrollBar::SetMinSliderLength(int nLength)
{
	m_pImpl->m_nMinSliderLength = nLength;
}

/////////////////////////////////////////////////////////////////
// slider surface를 추가한다.
/////////////////////////////////////////////////////////////////

VOID CScrollBar::AddSurfaceSlider(CSurface surface)
{
	CRectangle rtSlider = CalculateSliderPaint();

	surface.m_SnapShot.rtRect = rtSlider;

	m_pImpl->m_stlSliderSurface.push_back(surface);
}



/////////////////////////////////////////////////////////////////
// layout surface를 추가한다.
/////////////////////////////////////////////////////////////////

VOID CScrollBar::AddSurfaceLayout(CSurface surface)
{
	CRectangle rect = GetScreenRect();
	CRectangle rtInc = m_pBtnInc->GetScreenRect();
	CRectangle rtDec = m_pBtnDec->GetScreenRect();

	if(!IsVertical())
	{
		surface.m_SnapShot.rtRect.left = rtDec.right;
		surface.m_SnapShot.rtRect.top = rect.top;
		surface.m_SnapShot.rtRect.right = rtInc.left;
		surface.m_SnapShot.rtRect.bottom = rect.bottom;
	}
	else
	{
		surface.m_SnapShot.rtRect.left = rect.left;
		surface.m_SnapShot.rtRect.top = rtDec.bottom;
		surface.m_SnapShot.rtRect.right = rect.right;
		surface.m_SnapShot.rtRect.bottom = rtInc.top;
	}

	m_pImpl->m_stlLayoutSurface.push_back(surface);
}



VOID CScrollBar::ResizeSurfaceSlider(VOID)
{
	CRectangle rtSlider = CalculateSliderPaint();

	CSurface *pSurface;
	std::list<CSurface>::iterator the;

	for(the=m_pImpl->m_stlSliderSurface.begin() ; the!=m_pImpl->m_stlSliderSurface.end() ; the++)
	{
		pSurface = &(*the);
		
		pSurface->m_SnapShot.rtRect.left = pSurface->m_Original.rtRect.left + rtSlider.left;
		pSurface->m_SnapShot.rtRect.top = pSurface->m_Original.rtRect.top + rtSlider.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_Original.rtRect.left + rtSlider.right;
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_Original.rtRect.top + rtSlider.bottom;
	}
}

VOID CScrollBar::ResizeSurfaceLayout(VOID)
{
	CRectangle rect = GetScreenRect();
	CRectangle rtInc = m_pBtnInc->GetScreenRect();
	CRectangle rtDec = m_pBtnDec->GetScreenRect();

	CSurface *pSurface;
	std::list<CSurface>::iterator the;

	for(the=m_pImpl->m_stlLayoutSurface.begin() ; the!=m_pImpl->m_stlLayoutSurface.end() ; the++)
	{
		pSurface = &(*the);

		if(!IsVertical())
		{
			pSurface->m_SnapShot.rtRect.left = pSurface->m_Original.rtRect.left + rtDec.right;
			pSurface->m_SnapShot.rtRect.top = pSurface->m_Original.rtRect.top + rect.top;
			pSurface->m_SnapShot.rtRect.right = pSurface->m_Original.rtRect.left + rtInc.left;
			pSurface->m_SnapShot.rtRect.bottom = pSurface->m_Original.rtRect.top + rect.bottom;
		}
		else
		{
			pSurface->m_SnapShot.rtRect.left = pSurface->m_Original.rtRect.left + rect.left;
			pSurface->m_SnapShot.rtRect.top = pSurface->m_Original.rtRect.top + rtDec.bottom;
			pSurface->m_SnapShot.rtRect.right = pSurface->m_Original.rtRect.left + rect.right;
			pSurface->m_SnapShot.rtRect.bottom = pSurface->m_Original.rtRect.top + rtInc.top;
		}
	}
}

VOID CScrollBar::ResizeButton(VOID)
{
	CRectangle rtScrollBar = GetPosition();
	CRectangle rect;
	int nButtonWidth = m_pImpl->m_nButtonWidth;
	int nButtonHeight = m_pImpl->m_nButtonHeight;
	
	if(m_pImpl->m_bVertical)
	{
		if( nButtonWidth > rtScrollBar.GetWidth() )
			nButtonWidth = rtScrollBar.GetWidth();

		rect.left	= ( rtScrollBar.GetWidth() - nButtonWidth ) / 2;
		rect.top	= rtScrollBar.GetHeight() - nButtonHeight;
		rect.right	= rect.left + nButtonWidth;
		rect.bottom	= rect.top + nButtonHeight;

		m_pBtnInc->SetPosition( rect );

		rect.top	= 0;
		rect.bottom	= nButtonHeight;

		m_pBtnDec->SetPosition( rect );		
	}
	else
	{
		if( nButtonHeight > rtScrollBar.GetHeight() )
			nButtonHeight = rtScrollBar.GetHeight();

		rect.left	= rtScrollBar.GetWidth() - nButtonWidth;
		rect.top	= ( rtScrollBar.GetHeight() - nButtonHeight ) / 2;
		rect.right	= rect.left + nButtonWidth;
		rect.bottom	= rect.top + nButtonHeight;

		m_pBtnInc->SetPosition( rect );

		rect.left	= 0;
		rect.right	= nButtonWidth;

		m_pBtnDec->SetPosition ( rect );
	}	
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<int> &CScrollBar::SigValueChanged(VOID)
{
	return m_pImpl->m_SigValueChanged;
}

CSignal_v0 &CScrollBar::SigSliderPressed(VOID)
{
	return m_pImpl->m_SlgSliderPressed;
}

CSignal_v1<int> &CScrollBar::SigSliderMoved(VOID)
{
	return m_pImpl->m_SigSliderMoved;
}

CSignal_v0 &CScrollBar::SigSliderReleased(VOID)
{
	return m_pImpl->m_SigSliderReleased;
}

VOID CScrollBar::OnSetOptions(const CComponentOptions& options)
{
	// Button size
	ResizeButton();	

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int i,nCount;
			if(options.Exists("decrease_surface_up"))
			{
				nCount = options.Count("decrease_surface_up");
				for(i=0 ; i<nCount ; i++)
					m_pBtnDec->AddSurfaceUp(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_up",i))); 
			}
			if(options.Exists("decrease_surface_down"))
			{
				nCount = options.Count("decrease_surface_down");
				for(i=0 ; i<nCount ; i++)
					m_pBtnDec->AddSurfaceDown(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_down",i))); 
			}
			if(options.Exists("decrease_surface_focus"))
			{
				nCount = options.Count("decrease_surface_focus");
				for(i=0 ; i<nCount ; i++)
					m_pBtnDec->AddSurfaceFocus(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_focus",i))); 
			}
			if(options.Exists("decrease_surface_disable"))
			{
				nCount = options.Count("decrease_surface_disable");
				for(i=0 ; i<nCount ; i++)
					m_pBtnDec->AddSurfaceDisabled(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_disable",i))); 
			}
			if(options.Exists("increase_surface_up"))
			{
				nCount = options.Count("increase_surface_up");
				for(i=0 ; i<nCount ; i++)
					m_pBtnInc->AddSurfaceUp(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_up",i))); 
			}
			if(options.Exists("increase_surface_down"))
			{
				nCount = options.Count("increase_surface_down");
				for(i=0 ; i<nCount ; i++)
					m_pBtnInc->AddSurfaceDown(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_down",i))); 
			}
			if(options.Exists("increase_surface_focus"))
			{
				nCount = options.Count("increase_surface_focus");
				for(i=0 ; i<nCount ; i++)
					m_pBtnInc->AddSurfaceFocus(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_focus",i))); 
			}
			if(options.Exists("increase_surface_disable"))
			{
				nCount = options.Count("increase_surface_disable");
				for(i=0 ; i<nCount ; i++)
					m_pBtnInc->AddSurfaceDisabled(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_disable",i))); 
			}
			if(options.Exists("layout_surface"))
			{
				nCount = options.Count("layout_surface");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceLayout(pSurfaceManager->GetSurface(file,options.GetValue("layout_surface",i))); 
			}
			if(options.Exists("slider_surface"))
			{
				nCount = options.Count("slider_surface");
				for(i=0 ; i<nCount ; i++)
					AddSurfaceSlider(pSurfaceManager->GetSurface(file,options.GetValue("slider_surface",i))); 
			}			
		}
	}
}

CRectangle CScrollBar::CalculateSliderPaint(VOID)
{
	CRectangle rtScreen = GetScreenRect();
	CRectangle rect;
	rect.left = rtScreen.left + m_pImpl->m_rtRectSlider.left;
	rect.top = rtScreen.top + m_pImpl->m_rtRectSlider.top;
	rect.right = rect.left + m_pImpl->m_rtRectSlider.GetWidth();
	rect.bottom = rect.top + m_pImpl->m_rtRectSlider.GetHeight();

	CRectangle rtInc,rtDec;
	rtInc = m_pBtnInc->GetPosition();
	rtDec = m_pBtnDec->GetPosition();

	if (IsVertical())
	{
		rect.top += rtDec.GetHeight();
		rect.bottom = rect.top+m_pImpl->m_rtRectSlider.GetHeight();
	}
	else
	{
		rect.left += rtDec.GetWidth();
		rect.right = rect.left+m_pImpl->m_rtRectSlider.GetWidth();
	}

	return rect;
}


VOID CScrollBar::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( IsClipped() )
	{
		pClippingRect = GetClippingRect();
	}

	CSurface surface;
	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface;
	
	stlPSurface = GetSurfaceLayout();

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture, GetRenderTop(), (*the).m_fAngle, pClippingRect );
	}

	stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		surface = *the;
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture, GetRenderTop(), (*the).m_fAngle, pClippingRect );
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//: thumb track or slider bar를 잡고 이동했을 경우 호출되는 function

VOID CScrollBar::SliderMovePaint(VOID)
{
	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; the!=stlPSurface->end() ; the++)
	{
		(*the).m_SnapShot.rtRect = rtSlider;
	}
}


VOID CScrollBar::OnStartScroll( CComponent* pComponent, int nDelta )
{
	m_nScrollDelta = nDelta;
	SetValue(GetValue() + m_nScrollDelta);
}


VOID CScrollBar::OnStopScroll( CComponent* pComponent )
{
}


VOID CScrollBar::OnResize(int nOldWidth, int nOldHeight)
{
	ResizeButton();
	ResizeSurfaceLayout();
}


VOID CScrollBar::OnMove(int nX,int nY)
{
	ResizeSurfaceSlider();
	ResizeSurfaceLayout();
}

END_GUI
