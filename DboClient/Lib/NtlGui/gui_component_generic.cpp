#include "gui_precomp.h"
#include "gui_component_generic.h"
#include "gui_component.h"


START_GUI

CGUIManager* CComponent_Generic::m_pGuiMgr = NULL;

CComponent_Generic::CComponent_Generic( CComponent *pOwner, CComponent *pParent, CSurfaceManager *pSurfaceManager )
:
	m_pOwner(pOwner),
	m_pParent(pParent),
	m_pSurfaceMgr(pSurfaceManager),
	m_bCaptured(false),
	m_bVisible(true),
	m_bEnabled(true),
	m_bPopup(false),
	m_bRemoveflag(false),
	m_bRenderTop(false),
	m_nTabID(-1),
	m_wPriority( 32767 ),
	m_bClipping(false),
	m_ucAlpha( 255 ),
	m_uiColor( RGB( 255, 255, 255 ) )
{
	m_SlotSetOptions = m_SigSetOptions.Connect( this, &CComponent_Generic::SetOptions );
	m_SlotPaintChildren = m_SigChildPaint.Connect( this, &CComponent_Generic::PaintChildren );
	m_SlotMouseEnter = m_SigMouseEnter.Connect( this, &CComponent_Generic::OnMouseEnter );
	m_SlotMouseLeave = m_SigMouseLeave.Connect( this, &CComponent_Generic::OnMouseLeave );
}

CComponent_Generic::~CComponent_Generic()
{
	HideToolTip();

	for ( std::list<CComponent *>::iterator it = m_stlDeleteChildren.begin(); it != m_stlDeleteChildren.end(); it++ )
	{
		NTL_DELETE( *it );
	}
}

VOID CComponent_Generic::SetOptions(const CComponentOptions &options)
{
	int x = 0, y = 0, width = 0, height = 0;

	if (options.Exists("x"))      x = options.GetValueAsInt("x");
	if (options.Exists("y"))      y = options.GetValueAsInt("y");
	if (options.Exists("width"))  width = options.GetValueAsInt("width");
	if (options.Exists("height")) height = options.GetValueAsInt("height");
	if (options.Exists("clipping")) m_pOwner->SetClippingMode( options.GetValueAsBool("clipping") );
	
	m_pOwner->SetPosition(CRectangle(x, y, x + width, y + height));

	if (options.Exists("visible")) m_pOwner->Show(options.GetValueAsBool("visible"));
	if (options.Exists("enabled")) m_pOwner->Enable(options.GetValueAsBool("enabled"));

	if (options.Exists("tooltip")) m_pOwner->SetToolTip( options.GetValue("tooltip") );
	if (options.Exists("description"))	m_strDescription = options.GetValue("description");

	if (options.Exists("tab_id")) m_pOwner->SetTabID(options.GetValueAsInt("tab_id"));
	if (options.Exists("render_top")) m_pOwner->SetRenderTop(options.GetValueAsBool("render_top"));
	
	if( options.Exists( "priority" ) )
		m_pOwner->SetPriority( (WORD)options.GetValueAsInt( "priority" ) );
}

VOID CComponent_Generic::FindPreferredSize(VOID)
{
//	CPos size;
//	m_pOwner->sig_get_preferred_size()(size);
//	m_pOwner->set_size(size.x, size.y);
}

VOID CComponent_Generic::PaintChildren(VOID)
{	
	// we draw the children back to front because then all non-focus input
	// will be routed to the topmost window.
		
	for( std::list<CComponent *>::iterator it = m_stlChildren.end(); it != m_stlChildren.begin() ; )
	{
		--it;

		if( (*it)->IsVisible() )
			(*it)->Paint();
	}
}

VOID CComponent_Generic::CalcClippingRect(VOID)
{
	m_rtClipping = m_pOwner->GetScreenRect();

	if( !m_pParent || !m_bClipping )
		return;

	CRectangle rtParentScreen = m_pParent->GetClippingRect();
	CRectangle rtSelfScreen = m_pOwner->GetScreenRect();	

	if( rtParentScreen.left > rtSelfScreen.left )
		m_rtClipping.left = rtParentScreen.left;
	else
		m_rtClipping.left = rtSelfScreen.left;

	if( rtParentScreen.top > rtSelfScreen.top )
		m_rtClipping.top = rtParentScreen.top;
	else
		m_rtClipping.top = rtSelfScreen.top;

	if( rtParentScreen.right < rtSelfScreen.right )
		m_rtClipping.right = rtParentScreen.right;
	else
		m_rtClipping.right = rtSelfScreen.right;

	if( rtParentScreen.bottom < rtSelfScreen.bottom )
		m_rtClipping.bottom = rtParentScreen.bottom;
	else
		m_rtClipping.bottom = rtSelfScreen.bottom;
}

VOID CComponent_Generic::OnMouseEnter( CComponent* pComponent )
{
	if( m_pOwner->GetToolTip().empty() )
		return;

	//m_hTimer = CEventTimer::AddTimer( 500, this, &CComponent_Generic::ShowToolTip );
	ShowToolTip();
}

VOID CComponent_Generic::OnMouseLeave( CComponent* pComponent )
{
	HideToolTip();
}

VOID CComponent_Generic::SetToolTip( const std::wstring& wstrToolTip )
{
	m_wstrToolTip = wstrToolTip;

	CToolTip* pToolTip = GetGlobalComponent()->GetToolTip();

	if( pToolTip )
	{
		if( pToolTip->IsOwnerComponent( m_pOwner ) )
		{
			pToolTip->ShowToolTip( m_wstrToolTip.c_str(), m_pOwner );			
		}
	}
}

VOID CComponent_Generic::ShowToolTip(VOID)
{
	//CEventTimer::RemoveTimer( m_hTimer );

	CToolTip* pToolTip = GetGlobalComponent()->GetToolTip();

	if( pToolTip )
	{
		pToolTip->ShowToolTip( m_wstrToolTip.c_str(), m_pOwner );		
	}
}

VOID CComponent_Generic::HideToolTip(VOID)
{
	//CEventTimer::RemoveTimer( m_hTimer );

	CToolTip* pToolTip = GetGlobalComponent()->GetToolTip();

	if( pToolTip )
	{
		if( pToolTip->IsVisible() )
			pToolTip->HideToolTip( m_pOwner );		
	}
}

VOID CComponent_Generic::CheckToolTipHide(VOID)
{
	CToolTip* pToolTip = GetGlobalComponent()->GetToolTip();
	
	if( pToolTip )
	{
		if( pToolTip->IsVisible() )
			pToolTip->CheckOwnerCompHide();
	}
}

END_GUI