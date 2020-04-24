#include "gui_precomp.h"
#include "gui_anipanel.h"
#include "gui_anipanel_generic.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CAniPanel::CAniPanel(CComponent *pParent,
							 CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CAniPanel_Generic(this);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CAniPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CAniPanel::OnPaint);
	m_SlotMove	= SigMove().Connect(
		this, &CAniPanel::OnMove);
}

CAniPanel::CAniPanel(
	const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager)
: CComponent(rtPos,pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CAniPanel_Generic(this);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CAniPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CAniPanel::OnPaint);
	m_SlotMove	= SigMove().Connect(
		this, &CAniPanel::OnMove);
}

CAniPanel::CAniPanel(const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	CSurface surface)
: CComponent(rtPos,pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CAniPanel_Generic(this);

	AddSurface(surface);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CAniPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CAniPanel::OnPaint);
}

CAniPanel::~CAniPanel()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CAniPanel::IsFillEnabled(VOID) const 
{
	return m_pImpl->m_bFill;
}

//: panel surface
VOID CAniPanel::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
	m_pImpl->m_iterAni = m_pImpl->m_stlSurface.begin();

}

VOID CAniPanel::ResizeSurface(VOID)
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

VOID CAniPanel::EnableFill(bool bEnabled)
{
	m_pImpl->m_bFill = bEnabled;
}

VOID CAniPanel::SetDeltaTime(DWORD dwDelta)
{
	m_pImpl->m_dwDelta = dwDelta;
}


VOID CAniPanel::SetAnimation(bool bAni)
{
	m_pImpl->m_bAni = bAni;
	m_pImpl->m_dwTime = timeGetTime();
}

std::list<CSurface>* CAniPanel::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

VOID CAniPanel::OnSetOptions(const CComponentOptions& options)
{
	int nX=0,nY=0,nWidth=0,nHeight=0;
	
	if(options.Exists("x"))
		nX = options.GetValueAsInt("x");
	if(options.Exists("y"))
		nY = options.GetValueAsInt("y");
	if(options.Exists("width"))
		nWidth = options.GetValueAsInt("width");
	if(options.Exists("height"))
		nHeight = options.GetValueAsInt("height");
	if(options.Exists("delta"))
		m_pImpl->m_dwDelta = options.GetValueAsInt("delta");

	SetPosition(CRectangle(nX,nY,nX+nWidth,nY+nHeight));

	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int nCount = options.Count("surface");
			for(int i=0 ; i<nCount ; i++)
				AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
		}
	}
}


VOID CAniPanel::OnPaint(VOID)
{
	std::list<CSurface> *stlPSurface = GetSurface();
	if(stlPSurface->empty())
		return;

	DWORD dwDelta = timeGetTime() - m_pImpl->m_dwTime;

	if(m_pImpl->m_bAni && dwDelta >= m_pImpl->m_dwDelta)
	{
		m_pImpl->m_iterAni++;
		if(m_pImpl->m_iterAni == stlPSurface->end())
			m_pImpl->m_iterAni = stlPSurface->begin();
		m_pImpl->m_dwTime = timeGetTime();
	}

	g_GuiRenderer.RenderQueue(  &((*m_pImpl->m_iterAni).m_SnapShot), (*m_pImpl->m_iterAni).m_pTexture, GetRenderTop() );
	
//	g_Renderer.RenderPlane((*m_pImpl->m_iterAni).m_hTexture ,&((*m_pImpl->m_iterAni).m_SnapShot),GetRenderTop());
}

VOID CAniPanel::OnMove(int nX,int nY)
{
	ResizeSurface();
}


END_GUI
