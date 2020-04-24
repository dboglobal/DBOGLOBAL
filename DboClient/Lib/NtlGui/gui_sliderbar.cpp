#include "gui_precomp.h"
#include "gui_sliderbar.h"
#include "gui_componentoption.h"
#include "gui_sliderbar_generic.h"

START_GUI

CSliderBar::CSliderBar(CComponent *pParent,
											 CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager), m_pImpl(0)
{
	
	m_pImpl = NTL_NEW CSliderBar_Generic(this, 0, 100, 0, false, false);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CSliderBar::OnSetOptions);

	m_SlotPaint = SigPaint().Connect(
		this, &CSliderBar::OnPaint);

	m_SlotResize = SigResize().Connect(
		this,&CSliderBar::OnResize);

	m_SlotMove = SigMove().Connect(
		this,&CSliderBar::OnMove);
}


CSliderBar::CSliderBar(const CRectangle &rtPos,
											int nMin,
											int nMax,
											bool bOrientation,
											CComponent *pParent,
											CSurfaceManager *pSurfaceManager)
: CComponent(rtPos, pParent,pSurfaceManager), m_pImpl(0)
{
	m_pImpl = NTL_NEW CSliderBar_Generic(this, nMin, nMax, 0, bOrientation, false);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CSliderBar::OnSetOptions);

	m_SlotPaint = SigPaint().Connect(
		this, &CSliderBar::OnPaint);

	m_SlotResize = SigResize().Connect(
		this,&CSliderBar::OnResize);

	m_SlotMove = SigMove().Connect(
		this,&CSliderBar::OnMove);
}

CSliderBar::CSliderBar(const CRectangle &ptPos,
											bool bOrientation,
											CComponent *pParent,
											CSurfaceManager *pSurfaceManager)
: CComponent(ptPos, pParent,pSurfaceManager), m_pImpl(0)
{
	m_pImpl = NTL_NEW CSliderBar_Generic(this, 0, 100, 0, bOrientation, false);

	//----------------------
	// signal call function

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CSliderBar::OnSetOptions);

	m_SlotPaint = SigPaint().Connect(
		this, &CSliderBar::OnPaint);

	m_SlotResize = SigResize().Connect(
		this,&CSliderBar::OnResize);

	m_SlotMove = SigMove().Connect(
		this,&CSliderBar::OnMove);
}

CSliderBar::~CSliderBar()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


bool CSliderBar::IsVertical(VOID) const
{
	return m_pImpl->m_bVertical;
}

bool CSliderBar::IsTracking(VOID) const
{
	return m_pImpl->m_bTracking;
}

CRectangle CSliderBar::GetSliderPosition(VOID)
{
	return m_pImpl->m_rtRectSlider;
}

CRectangle CSliderBar::GetLayoutPosition(VOID)
{
	if(m_pImpl == NULL)
		return CRectangle(0,0,0,0);

	return GetPosition();
}


int CSliderBar::GetMinValue(VOID) const
{
	return m_pImpl->m_nMinValue;
}

int CSliderBar::GetMaxValue(VOID) const
{
	return m_pImpl->m_nMaxValue;
}

int CSliderBar::GetValue(VOID) const
{
	return m_pImpl->m_nCurValue;
}

int CSliderBar::GetRange(VOID) const
{
	return m_pImpl->GetRange();
}

bool CSliderBar::IsDraggingSlider(VOID) const
{
	return m_pImpl->m_bDragging;
}

CRectangle& CSliderBar::GetSliderRect(VOID) const
{
	return m_pImpl->m_rtRectSlider;
}

bool CSliderBar::IsFixedLengthSlider(VOID) const
{
	return m_pImpl->m_bFixedLength;
}

int CSliderBar::GetMinSliderLength(VOID) const
{
	return m_pImpl->m_nMinSliderLength;
}

//: Returns slider surface 
std::list<CSurface>* CSliderBar::GetSurfaceSlider(VOID)
{
	return &m_pImpl->m_stlSliderSurface;
}

//: Returns layout surface
std::list<CSurface>* CSliderBar::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CSliderBar::SetVertical(bool bEnable)
{
	m_pImpl->SetVertical(bEnable);
}

VOID CSliderBar::SetTracking(bool bEnable)
{
	m_pImpl->m_bTracking = bEnable;
}

VOID CSliderBar::SetRange(int nMinValue, int nMaxValue)
{
	m_pImpl->SetRange(nMinValue, nMaxValue);
}

VOID CSliderBar::SetMinValue(int nValue)
{
	m_pImpl->SetMinValue(nValue);
}

VOID CSliderBar::SetMaxValue(int nValue)
{
	m_pImpl->SetMaxValue(nValue);
}

VOID CSliderBar::SetValue(int nValue)
{
	m_pImpl->SetValue(nValue);

	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
				(*the).m_SnapShot.rtRect = rtSlider;
			}
}

VOID CSliderBar::Increase(int nStep)
{
	m_pImpl->SetValue(m_pImpl->m_nCurValue + nStep);

	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
				(*the).m_SnapShot.rtRect = rtSlider;
			}
}

VOID CSliderBar::Decrease(int nStep)
{
	m_pImpl->SetValue(m_pImpl->m_nCurValue - nStep);

	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
				(*the).m_SnapShot.rtRect = rtSlider;
			}
}

VOID CSliderBar::SetFixedLengthMode(bool bEnable)
{
	m_pImpl->m_bFixedLength = bEnable;
}

VOID CSliderBar::SetSliderSize(int nWidth,int nHeight)
{
	m_pImpl->SetSliderSize(nWidth,nHeight);
}


VOID CSliderBar::SetSliderLength(int nLength)
{
	m_pImpl->m_nFixedSliderLength = nLength;
}

VOID CSliderBar::SetMinSliderLength(int nLength)
{
	m_pImpl->m_nMinSliderLength = nLength;
}

/////////////////////////////////////////////////////////////////
// slider surface를 추가한다.
/////////////////////////////////////////////////////////////////

VOID CSliderBar::AddSurfaceSlider(CSurface surface)
{
	CRectangle rtSlider = CalculateSliderPaint();

	surface.m_SnapShot.rtRect = rtSlider;
	
	m_pImpl->m_stlSliderSurface.push_back(surface);
}



/////////////////////////////////////////////////////////////////
// layout surface를 추가한다.
/////////////////////////////////////////////////////////////////

VOID CSliderBar::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;

	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}



VOID CSliderBar::ResizeSurfaceSlider(VOID)
{
	CRectangle rtSlider = CalculateSliderPaint();

	CSurface *pSurface;
	std::list<CSurface>::iterator the;

	for(the=m_pImpl->m_stlSliderSurface.begin() ; 
			the!=m_pImpl->m_stlSliderSurface.end() ; 
			the++)
			{
				pSurface = &(*the);
				pSurface->m_SnapShot.rtRect = rtSlider;
			}
}

VOID CSliderBar::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;
	
	CSurface *pSurface;
	std::list<CSurface>::iterator the;

	for(the=m_pImpl->m_stlSurface.begin() ; 
			the!=m_pImpl->m_stlSurface.end() ; 
			the++)
			{
				pSurface = &(*the);

				rtScreen = GetScreenRect();

				rtSnap = pSurface->m_Original.rtRect;

				pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
				pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
				pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
				pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
			}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<int> &CSliderBar::SigValueChanged(VOID)
{
	return m_pImpl->m_SigValueChanged;
}

CSignal_v0 &CSliderBar::SigSliderPressed(VOID)
{
	return m_pImpl->m_SlgSliderPressed;
}

CSignal_v1<int> &CSliderBar::SigSliderMoved(VOID)
{
	return m_pImpl->m_SigSliderMoved;
}

CSignal_v0 &CSliderBar::SigSliderReleased(VOID)
{
	return m_pImpl->m_SigSliderReleased;
}

VOID CSliderBar::OnSetOptions(const CComponentOptions& options)
{
	
	//-------------------------------------------------------------------
	// scrollbar position

	int nX=0,nY=0,nWidth=0,nHeight=0;
	
	if(options.Exists("x"))
		nX = options.GetValueAsInt("x");
	if(options.Exists("y"))
		nY = options.GetValueAsInt("y");
	if(options.Exists("width"))
		nWidth = options.GetValueAsInt("width");
	if(options.Exists("height"))
		nHeight = options.GetValueAsInt("height");

	SetPosition(CRectangle(nX,nY,nX+nWidth,nY+nHeight));

	//-------------------------------------------------------------------
	// surface position

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int i,nCount;

			if(options.Exists("surface"))
			{
				nCount = options.Count("surface");
				for(i=0 ; i<nCount ; i++)
					AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
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

CRectangle CSliderBar::CalculateSliderPaint(VOID)
{
	CRectangle rtScreen = GetScreenRect();
	CRectangle rect;
	rect.left = rtScreen.left + m_pImpl->m_rtRectSlider.left;
	rect.top = rtScreen.top + m_pImpl->m_rtRectSlider.top;
	rect.right = rect.left + m_pImpl->m_rtRectSlider.GetWidth();
	rect.bottom = rect.top + m_pImpl->m_rtRectSlider.GetHeight();

	return rect;
}


VOID CSliderBar::OnPaint(VOID)
{
	CSurface surface;
	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface;
	
	stlPSurface = GetSurface();

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
//				g_Renderer.RenderPlane((*the).m_hTexture ,&((*the).m_SnapShot),GetRenderTop());
			}

	stlPSurface = GetSurfaceSlider();
	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
				surface = *the;
//				g_Renderer.RenderPlane((*the).m_hTexture ,&((*the).m_SnapShot),GetRenderTop());
			}
}

/////////////////////////////////////////////////////////////////////////////////////
//: thumb track or slider bar를 잡고 이동했을 경우 호출되는 function

VOID CSliderBar::SliderMovePaint(VOID)
{
	CRectangle rtSlider = CalculateSliderPaint();

	std::list<CSurface>::iterator the;
	std::list<CSurface> *stlPSurface = GetSurfaceSlider();

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
			{
				(*the).m_SnapShot.rtRect = rtSlider;
			}
}


VOID CSliderBar::OnResize(int nOldWidth, int nOldHeight)
{
}


VOID CSliderBar::OnMove(int nX,int nY)
{
	ResizeSurface();
	ResizeSurfaceSlider();
}

END_GUI
