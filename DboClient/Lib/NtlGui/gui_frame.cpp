#include "gui_precomp.h"
#include "gui_frame.h"
#include "gui_frame_generic.h"
#include "gui_renderer.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CFrame::CFrame(CComponent *pParent,CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CFrame_Generic(this);

	m_SlotPaint = SigPaint().Connect(
		this, &CFrame::OnPaint);
	m_SlotMove = SigMove().Connect(
		this, &CFrame::OnMove);
}

CFrame::CFrame(
	const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager)
: CComponent(rtPos,pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CFrame_Generic(this);

	m_SlotPaint = SigPaint().Connect(
		this, &CFrame::OnPaint);
	m_SlotMove = SigMove().Connect(
		this, &CFrame::OnMove);
}

CFrame::~CFrame()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CFrame::IsFillEnabled(VOID) const 
{
	return m_pImpl->m_bFill;
}

//: frame surface
VOID CFrame::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}

VOID CFrame::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
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
// Operations:

VOID CFrame::EnableFill(bool bEnabled)
{
	m_pImpl->m_bFill = bEnabled;
}

//: Returns surface list
std::list<CSurface>* CFrame::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}


/////////////////////////////////////////////////////////////////////////////
// Signals:

VOID CFrame::OnPaint(VOID)
{
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;
	for( the=stlPSurface->begin() ;  the!=stlPSurface->end() ; the++)
	{
		CSurface* pSurface = &(*the);
		g_GuiRenderer.RenderQueue( &pSurface->m_SnapShot, pSurface->m_pTexture, GetRenderTop() );
	}
}

VOID CFrame::OnMove(int nX,int nY)
{
	ResizeSurface();
}


END_GUI
