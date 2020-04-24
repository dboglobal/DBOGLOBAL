#include "gui_precomp.h"
#include "gui_dialog_generic.h"

#include "gui_componentoption.h"
#include "gui_renderer.h"

START_GUI

CDialog_Generic::CDialog_Generic(CDialog *pSelf, BOOL bDynamic)
: m_pDialog(pSelf), m_pOutline(NULL)
{
	m_bTracking = FALSE;
	m_bTitleBar = FALSE;
	m_bMovable = TRUE;
	m_dwTrackingTime = 0;
	m_rtFrame = CRectangle( 0, 0, 0, 0 );
	
	if( bDynamic )
		CreateDynamicOutline();

	m_slotResize = m_pDialog->SigResize().Connect( this, &CDialog_Generic::OnResize );
	m_SlotSetOption = m_pDialog->SigSetOptions().Connect(this, &CDialog_Generic::OnSetOptions);
	m_SlotPaint = m_pDialog->SigPaint().Connect(this, &CDialog_Generic::OnPaint);
	m_SlotMove = m_pDialog->SigMove().Connect(this, &CDialog_Generic::OnMove);
	m_SlotMouseDown = m_pDialog->SigMouseDown().Connect(this, &CDialog_Generic::OnMouseDown);
	m_SlotMouseUp = m_pDialog->SigMouseUp().Connect(this, &CDialog_Generic::OnMouseUp);
	m_SlotMouseMove = m_pDialog->SigMouseMove().Connect(this, &CDialog_Generic::OnMouseMove);
	m_SlotSetAlpha = m_pDialog->SigSetAlpha().Connect( this, &CDialog_Generic::OnSetAlpha );
	m_SlotSetColor = m_pDialog->SigSetColor().Connect( this, &CDialog_Generic::OnSetColor );
}

CDialog_Generic::~CDialog_Generic(VOID)
{
	DeleteDynamicOutline();
}

VOID CDialog_Generic::CreateDynamicOutline(VOID)
{
	if( !m_pOutline )
		m_pOutline = NTL_NEW CDynamicOutline( m_pDialog, FALSE );
}

VOID CDialog_Generic::DeleteDynamicOutline(VOID)
{
	NTL_DELETE( m_pOutline );
}

VOID CDialog_Generic::AddSurface( CSurface surface )
{
	CRectangle rtSnap = surface.m_SnapShot.rtRect;
	CRectangle rtScreen = m_pDialog->GetScreenRect(); 

	surface.m_SnapShot.rtRect.left = rtScreen.left + rtSnap.left;
	surface.m_SnapShot.rtRect.top = rtScreen.top + rtSnap.top;
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + rtSnap.GetWidth();
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + rtSnap.GetHeight();

	m_stlSurface.push_back(surface);
}

VOID CDialog_Generic::ResizeSurface(VOID)
{
	CRectangle rtScreen,rtSnap;

	std::list<CSurface>::iterator the;
	CSurface* pSurface = NULL;

	rtScreen = m_pDialog->GetScreenRect();

	for( the = m_stlSurface.begin() ; the!=m_stlSurface.end() ; ++the )
	{
		pSurface = &(*the);

		rtSnap = pSurface->m_Original.rtRect;
		pSurface->m_SnapShot.rtRect.left = rtScreen.left+rtSnap.left;
		pSurface->m_SnapShot.rtRect.top = rtScreen.top+rtSnap.top;
		pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left+rtSnap.GetWidth();
		pSurface->m_SnapShot.rtRect.bottom = pSurface->m_SnapShot.rtRect.top+rtSnap.GetHeight();
	}
}

VOID CDialog_Generic::OnSetOptions( const CComponentOptions& options )
{
	if( options.Exists( "titlebar" ) )
		m_bTitleBar = options.GetValueAsBool( "titlebar" );

	if(options.Exists( "surface_file" ) )  
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager *pSurfaceManager = m_pDialog->GetSurfaceManager();
		if( pSurfaceManager ) 
		{
			INT nCount = options.Count( "surface" );
			for( INT i = 0 ; i < nCount ; ++i )
				AddSurface(pSurfaceManager->GetSurface(file,options.GetValue("surface",i))); 
		}
	}

	if( options.Exists( "titlebar_x" ) )
		m_rtFrame.left = options.GetValueAsInt( "titlebar_x" );
	if( options.Exists( "titlebar_y" ) )
		m_rtFrame.top = options.GetValueAsInt( "titlebar_y" );
	if( options.Exists( "titlebar_width" ) )
		m_rtFrame.right = options.GetValueAsInt( "titlebar_width" ) + m_rtFrame.left;
	if( options.Exists( "titlebar_height" ) )
		m_rtFrame.bottom = options.GetValueAsInt( "titlebar_height" ) + m_rtFrame.top;

	// 
	if( options.Exists( "dynamic" ) )
	{
		if( options.GetValueAsBool( "dynamic" ) )
		{
			CreateDynamicOutline();
			m_pOutline->SetSurfaces( options );		
			m_pOutline->SetOutScreenRect( m_pDialog->GetScreenRect() );
		}
	}	
}

VOID CDialog_Generic::OnPaint(VOID)
{
	CRectangle* pClippingRect = NULL;

	if( m_pDialog->IsClipped() )
		pClippingRect = m_pDialog->GetClippingRect();

	std::list<CSurface>::iterator the;
	for( the = m_stlSurface.begin(); the != m_stlSurface.end() ; ++the )
	{
		g_GuiRenderer.RenderQueue( &(*the).m_SnapShot, (*the).m_pTexture, m_pDialog->GetRenderTop(), (*the).m_fAngle, pClippingRect );
	}

	if( m_pOutline )
		m_pOutline->Paint( m_pDialog->GetRenderTop() );
}

VOID CDialog_Generic::OnMove( INT nX, INT nY )
{	
	ResizeSurface();

	CRectangle rtScreen = m_pDialog->GetScreenRect();	
	
	if( m_pOutline )
		m_pOutline->SetMove( rtScreen.left, rtScreen.top );
}

VOID CDialog_Generic::OnResize( INT nOldCX, INT nOldCY )
{
	if( m_pOutline )
	{
		ResizeSurface();

		CRectangle rtScreen = m_pDialog->GetScreenRect();
		m_pOutline->SetOutScreenRect( rtScreen );
	}	
}

VOID CDialog_Generic::OnMouseDown( const CKey& key )
{
	m_ptCapturePos.SetPos( (INT)key.m_fX, (INT)key.m_fY );

	if( key.m_nID == UD_LEFT_BUTTON && m_bMovable )
	{
		if( m_rtFrame.PtInRect( m_ptCapturePos ) )
		{
			m_bTracking = TRUE;
			m_pDialog->CaptureMouse();
			m_dwTrackingTime = GetTickCount();
		}
	}
}

VOID CDialog_Generic::OnMouseUp( const CKey& key )
{
	if( key.m_nID == UD_LEFT_BUTTON )
	{
		m_pDialog->ReleaseMouse();
	}

	m_dwTrackingTime = 0;
	m_bTracking = FALSE;
}

VOID CDialog_Generic::OnMouseMove( INT nFlags, INT nX, INT nY )
{
	if( m_bTracking )
	{
		DWORD dwCurrentTime = GetTickCount();

		if( dwCurrentTime - m_dwTrackingTime > 20 )
		{
			CRectangle rtRect = m_pDialog->GetPosition();
			INT nPosX = rtRect.left + nX - m_ptCapturePos.x;
			INT nPosY = rtRect.top + nY - m_ptCapturePos.y;
			m_pDialog->SetPosition( nPosX, nPosY );

			m_pDialog->SigTracking()( nPosX, nPosY );

			m_dwTrackingTime = dwCurrentTime;
		}		
	}
}

VOID CDialog_Generic::OnSetAlpha( BYTE ucAlpha )
{
	std::list<CSurface>::iterator it;
	CSurface* pSurface = NULL;

	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}

	if( m_pOutline )
		m_pOutline->SetSurfaceAlpha( ucAlpha );
}

VOID CDialog_Generic::OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	std::list<CSurface>::iterator it;
	CSurface* pSurface = NULL;

	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		pSurface = &(*it);
		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}

	if( m_pOutline )
		m_pOutline->SetSurfaceColor( ucRed, ucGreen, ucBlue );
}

END_GUI