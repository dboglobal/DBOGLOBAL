#include "gui_precomp.h"
#include "gui_panel.h"
#include "gui_panel_generic.h"
#include "gui_componentoption.h"
#include "gui_texturemanager.h"
#include "gui_renderer.h"



/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CPanel::CPanel(CComponent *pParent,
							 CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CPanel_Generic(this);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CPanel::OnPaint);
	m_SlotMove	= SigMove().Connect(
		this, &CPanel::OnMove);
}

CPanel::CPanel(
	const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager)
: CComponent(rtPos,pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CPanel_Generic(this);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CPanel::OnPaint);
	m_SlotMove	= SigMove().Connect(
		this, &CPanel::OnMove);
}

CPanel::CPanel(const CRectangle &rtPos,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	CSurface surface)
: CComponent(rtPos,pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CPanel_Generic(this);

	AddSurface(surface);

	m_SlotSetOption = SigSetOptions().Connect(
		this, &CPanel::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CPanel::OnPaint);
	m_SlotMove	= SigMove().Connect(
		this, &CPanel::OnMove);
}

CPanel::~CPanel()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

//: panel surface
VOID CPanel::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}

VOID CPanel::ClearSurface()
{
    m_pImpl->m_stlSurface.clear();
}

VOID CPanel::ChangeTexture(int idx, const char* filename)
{
	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	if (idx < 0 || idx >= (int)stlPSurface->size())
		return;

	int i = 0;
	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);

		if (i == idx)
		{
			g_TextureMgr.DeleteTexture(pSurface->m_pTexture);
			pSurface->m_pTexture = g_TextureMgr.CreateTexture(filename);
			break;
		}
		++i;
	}
}


VOID CPanel::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
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

VOID CPanel::ResizeSurface( int nWidth, int nHeight )
{
	CRectangle rtScreen,rtSnap,rtOrigin;

	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);
		rtScreen = GetScreenRect();

		rtOrigin	= pSurface->m_Original.rtRect;
		rtSnap		= pSurface->m_SnapShot.rtRect;

		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtOrigin.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtOrigin.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth() + nWidth;
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight() + nHeight;
	}
}

//VOID CPanel::SetColor (BYTE byRed,BYTE byGreen,BYTE byBlue)
//{
//	CSurface *pSurface;
//	std::list<CSurface> *stlPSurface = GetSurface();
//	std::list<CSurface>::iterator the;
//
//	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
//	{
//		pSurface = &(*the);
//		pSurface->m_SnapShot.uRed = byRed;
//		pSurface->m_SnapShot.uGreen = byGreen;
//		pSurface->m_SnapShot.uBlue = byBlue;
//	}
//}
//
//VOID CPanel::SetAlpha (BYTE	byAlpha)
//{
//	CSurface *pSurface;
//	std::list<CSurface> *stlPSurface = GetSurface();
//	std::list<CSurface>::iterator the;
//
//	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
//	{
//		pSurface = &(*the);
//		pSurface->m_SnapShot.uAlpha = byAlpha; 
//	}
//}


//: panel surface angle
VOID CPanel::SetAngle(float fAngle)
{
	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);
		pSurface->m_fAngle = fAngle;
	}
}


VOID CPanel::SetBlending(DWORD dwBlend)
{
	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);
		pSurface->m_SnapShot.uBlend = dwBlend;
	}
}

VOID CPanel::RestoreAlpha(VOID)
{
	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);
		pSurface->m_SnapShot.uAlpha = pSurface->m_Original.uAlpha;
	}
}

VOID CPanel::EnableFill(bool bEnabled)
{
	m_pImpl->m_bFill = bEnabled;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CPanel::IsFillEnabled(VOID) const 
{
	return m_pImpl->m_bFill;
}

BYTE CPanel::GetAlpha(VOID)
{
	CSurface *pSurface;
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		pSurface = &(*the);
		return pSurface->m_SnapShot.uAlpha;
	}

	return 0;
}

std::list<CSurface>* CPanel::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

VOID CPanel::OnSetOptions(const CComponentOptions& options)
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


VOID CPanel::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;
	if( IsClipped() )
	{
		pClippingRect = GetClippingRect();
	}

	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;
	for(the=stlPSurface->begin(); the!=stlPSurface->end(); the++)
	{
		g_GuiRenderer.RenderQueue(&(*the).m_SnapShot, (*the).m_pTexture, GetRenderTop(), (*the).m_fAngle, pClippingRect  );
	}
}

VOID CPanel::OnMove(int nX,int nY)
{
	ResizeSurface();
}

END_GUI

