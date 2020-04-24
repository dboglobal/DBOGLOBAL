#include "gui_precomp.h"
#include "gui_dialog.h"
#include "gui_dialog_generic.h"
#include "gui_componentoption.h"
#include "gui_renderer.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CDialog::CDialog(CComponent *pParent,CSurfaceManager *pSurfaceManager)
: CComponent(pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CDialog_Generic(this,FALSE);
}

CDialog::CDialog(const CRectangle &rtPos, CComponent *pParent, CSurfaceManager *pSurfaceManager, bool bDynamic /* = FALSE */)
: CComponent(rtPos,pParent,pSurfaceManager), m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CDialog_Generic(this,bDynamic);
}

CDialog::CDialog(const CRectangle &rtPos, CComponent *pParent, CSurfaceManager *pSurfaceManager, CSurface surface, bool bDynamic /* = FALSE */)
: CComponent(rtPos,pParent,pSurfaceManager),m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CDialog_Generic(this,bDynamic);

	AddSurface(surface);
}

CDialog::~CDialog()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Operations:
VOID CDialog::SetFrameSize(const CRectangle& rtRect)
{
	m_pImpl->m_rtFrame = rtRect;
}

//: panel surface
VOID CDialog::AddSurface(CSurface surface)
{
	m_pImpl->AddSurface( surface );
}

VOID CDialog::AddDynamicSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
								  CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL )
{
	m_pImpl->CreateDynamicOutline();
	m_pImpl->m_pOutline->AddSurfaces( surCenter, surLTC, surRTC, surLBC, surRBC, surLL, surTL, surRL, surBL, CSurface() );
	m_pImpl->m_pOutline->SetOutScreenRect( GetScreenRect() );
}

VOID CDialog::SetMovable(BOOL bMovable)
{
	m_pImpl->m_bMovable = bMovable;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:
std::list<CSurface>* CDialog::GetSurface(VOID)
{
	return &m_pImpl->m_stlSurface;
}

/////////////////////////////////////////////////////////////////////////////
// virtual operator
VOID CDialog::TopTabOrder(VOID)
{
	CComponent::TopTabOrder();
	CComponent *pParent = GetParent();
	if(pParent && m_pImpl->m_bTitleBar)
		pParent->TopTabOrder(this);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:


END_GUI
