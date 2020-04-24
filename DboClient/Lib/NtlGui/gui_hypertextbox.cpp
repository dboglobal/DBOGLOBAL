#include "gui_precomp.h"
#include "gui_hypertextbox.h"
#include "gui_hypertextbox_generic.h"

START_GUI

/////////////////////////////////////////////////////////////////////////////
// Construction:

CHyperTextBox::CHyperTextBox(
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	DWORD dwStyle/*=0*/)
: CComponent(pParent,pSurfaceManager), m_pImpl(NULL),m_pSBVer(NULL)
{
	m_pSBVer = NTL_NEW CScrollBar(this,pSurfaceManager);
	m_pSBVer->Show(false);

	m_pImpl = NTL_NEW CHyperTextBox_Generic(this,dwStyle);

	m_SlotSetOptions = SigSetOptions().Connect(
		this, &CHyperTextBox::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CHyperTextBox::OnPaint);
	m_SlotMove  = SigMove().Connect(
		this, &CHyperTextBox::OnMove);
}


CHyperTextBox::CHyperTextBox(
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	int nStartRow,   
	int nStartColumn,
	int nInterval,
	DWORD dwStyle)
: CComponent(pParent,pSurfaceManager), m_pImpl(NULL),m_pSBVer(NULL)
{
	m_pSBVer = NTL_NEW CScrollBar(this,pSurfaceManager);
	m_pSBVer->Show(false);

	m_pImpl = NTL_NEW CHyperTextBox_Generic(this,nStartRow,nStartColumn,nInterval,dwStyle);

	m_SlotSetOptions = SigSetOptions().Connect(
		this, &CHyperTextBox::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CHyperTextBox::OnPaint);
	m_SlotMove  = SigMove().Connect(
		this, &CHyperTextBox::OnMove);
}

CHyperTextBox::CHyperTextBox(
	const CRectangle &ptRect,
	CComponent *pParent,
	CSurfaceManager *pSurfaceManager,
	int nStartRow,   
	int nStartColumn,
	int nInterval,
	DWORD dwStyle)
: CComponent(ptRect, pParent,pSurfaceManager), m_pImpl(NULL),m_pSBVer(NULL)
{
	//-------------------------------------------------------------------------

	CRectangle rect = GetClientRect();
	rect.left = rect.right-HYPERTEXTBOX_SCROLLBAR_SIZE;
	m_pSBVer = NTL_NEW CScrollBar(rect,0,0,0,this,pSurfaceManager);
	m_pSBVer->Show(false);

	//-------------------------------------------------------------------------

	m_pImpl = NTL_NEW CHyperTextBox_Generic(this,nStartRow,nStartColumn,nInterval,dwStyle);
	
	m_SlotSetOptions = SigSetOptions().Connect(
		this, &CHyperTextBox::OnSetOptions);
	m_SlotPaint = SigPaint().Connect(
		this, &CHyperTextBox::OnPaint);
	m_SlotMove  = SigMove().Connect(
		this, &CHyperTextBox::OnMove);
}


CHyperTextBox::~CHyperTextBox()
{
	NTL_DELETE( m_pSBVer );
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:


std::list<CSurface>* CHyperTextBox::GetSurface(VOID) const
{
	return &m_pImpl->m_stlSurface;
}

CScrollBar* CHyperTextBox::GetVerticalScrollBar(VOID) const
{
	return m_pSBVer;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CHyperTextBox::CreateFont(TCHAR * szFaceName, int nHeight, int nAttributes /*= 0*/)
{
	m_pImpl->CreateFont(szFaceName,nHeight,nAttributes);
}


VOID CHyperTextBox::SetTextKey(const std::string& key)
{
	m_pImpl->SetTextKey(key);
}

VOID CHyperTextBox::SetRow (int nRow)
{
	m_pImpl->m_nRow = nRow;
}

VOID CHyperTextBox::SetColumn (int nColumn)
{
	m_pImpl->m_nColumn = nColumn;
}

VOID CHyperTextBox::SetInterval (int nInterval)
{
	m_pImpl->m_nInterval = nInterval;
}

VOID CHyperTextBox::Clear(VOID)
{
	m_pImpl->Clear();
}


VOID CHyperTextBox::AddSurface(CSurface surface)
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;

	CRectangle rtScreen = GetScreenRect(); 
	
	surface.m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left+rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top+rtSnap.GetHeight();

	m_pImpl->m_stlSurface.push_back(surface);
}

VOID CHyperTextBox::ResizeSurface(VOID)
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
// signal function:

CSignal_v1<CNode*> &CHyperTextBox::SigNodeChanged(VOID)
{
	return m_pImpl->m_SigNode; 
}

/////////////////////////////////////////////////////////////////////////////
// callback functions:

VOID CHyperTextBox::OnSetOptions(const CComponentOptions &options)
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

	CRectangle rect = GetClientRect();
	rect.left = rect.right-HYPERTEXTBOX_SCROLLBAR_SIZE;
	m_pSBVer->SetPosition(rect);
	if(m_pSBVer->IsVertical()) 
		m_pSBVer->SetSliderSize(rect.GetWidth(),rect.GetWidth()); 
	else
		m_pSBVer->SetSliderSize(rect.GetHeight(),rect.GetHeight()); 
	
	if(options.Exists("surface_file"))
	{
		std::string file = options.GetValue("surface_file"); 
		CSurfaceManager *pSurfaceManager = GetSurfaceManager();
		if(pSurfaceManager)
		{
			int i,nCount;
			CButton *pDecBtn = m_pSBVer->GetDecreaseButton(); 
			CButton *pIncBtn = m_pSBVer->GetIncreaseButton(); 

			if(options.Exists("decrease_surface_up"))
			{
				nCount = options.Count("decrease_surface_up");
				for(i=0 ; i<nCount ; i++)
					pDecBtn->AddSurfaceUp(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_up",i))); 
			}
			if(options.Exists("decrease_surface_down"))
			{
				nCount = options.Count("decrease_surface_down");
				for(i=0 ; i<nCount ; i++)
					pDecBtn->AddSurfaceDown(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_down",i))); 
			}
			if(options.Exists("decrease_surface_focus"))
			{
				nCount = options.Count("decrease_surface_focus");
				for(i=0 ; i<nCount ; i++)
					pDecBtn->AddSurfaceFocus(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_focus",i))); 
			}
			if(options.Exists("decrease_surface_disable"))
			{
				nCount = options.Count("decrease_surface_disable");
				for(i=0 ; i<nCount ; i++)
					pDecBtn->AddSurfaceDisabled(pSurfaceManager->GetSurface(file,options.GetValue("decrease_surface_disable",i))); 
			}
			if(options.Exists("increase_surface_up"))
			{
				nCount = options.Count("increase_surface_up");
				for(i=0 ; i<nCount ; i++)
					pIncBtn->AddSurfaceUp(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_up",i))); 
			}
			if(options.Exists("increase_surface_down"))
			{
				nCount = options.Count("increase_surface_down");
				for(i=0 ; i<nCount ; i++)
					pIncBtn->AddSurfaceDown(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_down",i))); 
			}
			if(options.Exists("increase_surface_focus"))
			{
				nCount = options.Count("increase_surface_focus");
				for(i=0 ; i<nCount ; i++)
					pIncBtn->AddSurfaceFocus(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_focus",i))); 
			}
			if(options.Exists("increase_surface_disable"))
			{
				nCount = options.Count("increase_surface_disable");
				for(i=0 ; i<nCount ; i++)
					pIncBtn->AddSurfaceDisabled(pSurfaceManager->GetSurface(file,options.GetValue("increase_surface_disable",i))); 
			}
			if(options.Exists("slider_surface"))
			{
				nCount = options.Count("slider_surface");
				for(i=0 ; i<nCount ; i++)
					m_pSBVer->AddSurfaceSlider(pSurfaceManager->GetSurface(file,options.GetValue("slider_surface",i))); 
			}
			if(options.Exists("layout_surface"))
			{
				nCount = options.Count("layout_surface");
				for(i=0 ; i<nCount ; i++)
					m_pSBVer->AddSurfaceLayout(pSurfaceManager->GetSurface(file,options.GetValue("layout_surface",i))); 
			}
			if(options.Exists("surface"))
			{
				nCount = options.Count("surface");
				for(i=0 ; i<nCount ; i++)
					AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
			}
		}
	}
		
}

VOID CHyperTextBox::OnPaint(VOID)
{
	std::list<CSurface> *stlPSurface = GetSurface();
	std::list<CSurface>::iterator the;

	for(the=stlPSurface->begin() ; 
			the!=stlPSurface->end() ; 
			the++)
	{
//		g_Renderer.RenderPlane((*the).m_hTexture ,&((*the).m_SnapShot),GetRenderTop());
	}

	m_pImpl->TextOut();
}

VOID CHyperTextBox::OnMove(int nOldX,int nOldY)
{
	ResizeSurface();
	if(m_pSBVer)
		m_pSBVer->SigMove()(nOldX,nOldY);
}

END_GUI