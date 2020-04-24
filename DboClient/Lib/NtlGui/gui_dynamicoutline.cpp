#include "gui_precomp.h"
#include "gui_DynamicOutline.h"
#include "gui_renderer.h"

START_GUI

////////////////////////////////////////////////////////////////////////////////
// Constructor & Destructor

CDynamicOutline::CDynamicOutline( CComponent* pComponent, BOOL bBalloon )
:m_pComponent( pComponent ), m_bInitialized( FALSE ), m_bBalloon( bBalloon ),
 m_nTailX( 0 ), m_nLeftCX( 0 ), m_nRightCX( 0 ), m_nTopCY( 0 ), m_nBottomCY( 0 )
{
}

CDynamicOutline::~CDynamicOutline(VOID)
{
	
}

////////////////////////////////////////////////////////////////////////////////
// Operations

INT CDynamicOutline::GetLeftCornerCX(VOID)
{
	INT nLTCX = m_Surfaces[LTC].m_Original.rtRect.GetWidth();
	INT nLBCX = m_Surfaces[LBC].m_Original.rtRect.GetWidth();

	return ( nLTCX > nLBCX ) ? nLTCX : nLBCX;
}

INT CDynamicOutline::GetRightCornerCX(VOID)
{
	INT nRTCX = m_Surfaces[RTC].m_Original.rtRect.GetWidth();
	INT nRBCX = m_Surfaces[RBC].m_Original.rtRect.GetWidth();

	return ( nRTCX > nRBCX ) ? nRTCX : nRBCX;	
}

VOID CDynamicOutline::SetSurfaces( const CComponentOptions& options )
{
	if( m_pComponent == NULL )
		return;

	if( options.Exists( "balloontail" ) )
		m_bBalloon = options.GetValueAsBool( "balloontail" );

	if( options.Exists( "surface_file" ) )
	{
		std::string file = options.GetValue( "surface_file" ); 
		CSurfaceManager* pSurfaceManager = m_pComponent->GetSurfaceManager();
				
		if( pSurfaceManager )
		{
			AddSurface( LTC, pSurfaceManager->GetSurface( file,options.GetValue("surface_corner_lefttop",0) ) ); 
			AddSurface( RTC, pSurfaceManager->GetSurface( file,options.GetValue("surface_corner_righttop",0) ) ); 
			AddSurface( LBC, pSurfaceManager->GetSurface( file,options.GetValue("surface_corner_leftbottom",0) ) ); 
			AddSurface( RBC, pSurfaceManager->GetSurface( file,options.GetValue("surface_corner_rightbottom",0) ) ); 
			AddSurface( LL, pSurfaceManager->GetSurface( file,options.GetValue("surface_line_left",0) ) ); 
			AddSurface( TL, pSurfaceManager->GetSurface( file,options.GetValue("surface_line_top",0) ) ); 
			AddSurface( RL, pSurfaceManager->GetSurface( file,options.GetValue("surface_line_right",0) ) ); 
			AddSurface( BL1, pSurfaceManager->GetSurface( file,options.GetValue("surface_line_bottom",0) ) ); 
			
			if( m_bBalloon )
				AddSurface( BL2, pSurfaceManager->GetSurface( file,options.GetValue("surface_line_bottom",0) ) ); 
			
			AddSurface( CENTER, pSurfaceManager->GetSurface( file, options.GetValue("surface_center", 0) ) );

			m_nLeftCX = m_Surfaces[LL].m_Original.rtRect.GetWidth();
			m_nRightCX = m_Surfaces[RL].m_Original.rtRect.GetWidth();
			m_nTopCY = m_Surfaces[TL].m_Original.rtRect.GetHeight();
			m_nBottomCY = m_Surfaces[BL1].m_Original.rtRect.GetHeight();
			
			if( m_bBalloon )
			{
				AddSurface( TAIL, pSurfaceManager->GetSurface( file,options.GetValue("surface_balloontail",0) ) ); 
			
				m_cxTail = m_Surfaces[TAIL].m_Original.rtRect.GetWidth();
				m_cyTail = m_Surfaces[TAIL].m_Original.rtRect.GetHeight();
				m_nBottomCY = m_cyTail;
			}

			m_bInitialized = TRUE;
		}
	}
	
	if( options.Exists( "left_cx" ) )
		m_nLeftCX = options.GetValueAsInt( "left_cx" ); 

	if( options.Exists( "right_cx" ) )
		m_nRightCX = options.GetValueAsInt( "right_cx" ); 

	if( options.Exists( "top_cy" ) )
		m_nTopCY = options.GetValueAsInt( "top_cy" ); 

	if( options.Exists( "bottom_cy" ) )
		m_nBottomCY = options.GetValueAsInt( "bottom_cy" ); 	
}

VOID CDynamicOutline::AddSurface( SURFACEPART ePart, CSurface& surface )
{
	m_Surfaces[ePart] = surface;
}

VOID CDynamicOutline::AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
								   CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL, CSurface& surTail )
{
	AddSurface( CENTER, surCenter );
	AddSurface( LTC, surLTC );
	AddSurface( RTC, surRTC );
	AddSurface( LBC, surLBC );
	AddSurface( RBC, surRBC );
	AddSurface( LL, surLL );
	AddSurface( TL, surTL );
	AddSurface( RL, surRL );
	AddSurface( BL1, surBL );
	AddSurface( BL2, surBL );
	
	m_nLeftCX = surLL.m_Original.rtRect.GetWidth(); 
	m_nRightCX = surRL.m_Original.rtRect.GetWidth();
	m_nTopCY = surTL.m_Original.rtRect.GetHeight(); 
	m_nBottomCY = surBL.m_Original.rtRect.GetHeight();

	if( m_bBalloon )
	{
		AddSurface( TAIL, surTail );
		m_cxTail = surTail.m_Original.rtRect.GetWidth();
		m_cyTail = surTail.m_Original.rtRect.GetHeight(); 
		m_nBottomCY = m_cyTail;
	}
	
	m_bInitialized = TRUE;
}

VOID CDynamicOutline::Paint( bool bRenderTop )
{
	INT i, nCount;
	
	if( m_bBalloon )
		nCount = NUM_SURFACE;
	else
		nCount = NUM_SURFACE - 2;
	
	for( i = 0 ; i < nCount ; ++i )
	{
		g_GuiRenderer.RenderQueue( &( m_Surfaces[i].m_SnapShot ), m_Surfaces[i].m_pTexture, bRenderTop );
	}
}

VOID CDynamicOutline::SetOutScreenRect( CRectangle& OutRect, INT nTailX /* = 0  */)
{
	if( !m_bInitialized )
		return;
	
	CSurface* ltc	= &m_Surfaces[LTC];
	CSurface* rtc	= &m_Surfaces[RTC];
	CSurface* lbc	= &m_Surfaces[LBC];
	CSurface* rbc	= &m_Surfaces[RBC];
	CSurface* ll	= &m_Surfaces[LL];
	CSurface* tl	= &m_Surfaces[TL];
	CSurface* rl	= &m_Surfaces[RL];
	CSurface* bl1	= &m_Surfaces[BL1];
	CSurface* center= &m_Surfaces[CENTER];
	CSurface* bl2	= NULL;
	CSurface* tail	= NULL;
	
	m_rcOutRect = OutRect;
	m_nTailX = nTailX;

	if( m_bBalloon )
	{
		bl2  = &m_Surfaces[BL2];
		tail = &m_Surfaces[TAIL];
		OutRect.bottom += ( bl1->m_Original.rtRect.GetHeight() - tail->m_Original.rtRect.GetHeight() );
		nTailX += OutRect.left;
	}


	// Left Top Corner
	ltc->m_SnapShot.rtRect.left		= OutRect.left;
	ltc->m_SnapShot.rtRect.top		= OutRect.top;
	ltc->m_SnapShot.rtRect.right	= OutRect.left + ltc->m_Original.rtRect.GetWidth();
	ltc->m_SnapShot.rtRect.bottom	= OutRect.top + ltc->m_Original.rtRect.GetHeight();

	// Right Top Corner
	rtc->m_SnapShot.rtRect.left		= OutRect.right - rtc->m_Original.rtRect.GetWidth();
	rtc->m_SnapShot.rtRect.top		= OutRect.top;
	rtc->m_SnapShot.rtRect.right	= OutRect.right;
	rtc->m_SnapShot.rtRect.bottom	= OutRect.top + rtc->m_Original.rtRect.GetHeight();

	// Left Bottom Corner
	
	lbc->m_SnapShot.rtRect.left		= OutRect.left;
	lbc->m_SnapShot.rtRect.top		= OutRect.bottom - lbc->m_Original.rtRect.GetHeight();
	lbc->m_SnapShot.rtRect.right	= OutRect.left + lbc->m_Original.rtRect.GetWidth();
	lbc->m_SnapShot.rtRect.bottom	= OutRect.bottom;

	// Right Bottom Corner
	rbc->m_SnapShot.rtRect.left		= OutRect.right - rbc->m_Original.rtRect.GetWidth();
	rbc->m_SnapShot.rtRect.top		= OutRect.bottom - rbc->m_Original.rtRect.GetHeight();
	rbc->m_SnapShot.rtRect.right	= OutRect.right;
	rbc->m_SnapShot.rtRect.bottom	= OutRect.bottom;

	// Left Line
	ll->m_SnapShot.rtRect.left		= OutRect.left;
	ll->m_SnapShot.rtRect.top		= OutRect.top + ltc->m_Original.rtRect.GetHeight();
	ll->m_SnapShot.rtRect.right		= OutRect.left + ll->m_Original.rtRect.GetWidth();
	ll->m_SnapShot.rtRect.bottom	= OutRect.bottom - lbc->m_Original.rtRect.GetHeight();

	// Right Line
	rl->m_SnapShot.rtRect.left		= OutRect.right - rl->m_Original.rtRect.GetWidth();
	rl->m_SnapShot.rtRect.top		= OutRect.top + rtc->m_Original.rtRect.GetHeight();
	rl->m_SnapShot.rtRect.right		= OutRect.right;
	rl->m_SnapShot.rtRect.bottom	= OutRect.bottom - rbc->m_Original.rtRect.GetHeight();

	// Top Line
	tl->m_SnapShot.rtRect.left		= OutRect.left + ltc->m_Original.rtRect.GetWidth();
	tl->m_SnapShot.rtRect.top		= OutRect.top;
	tl->m_SnapShot.rtRect.right		= OutRect.right - rtc->m_Original.rtRect.GetWidth();
	tl->m_SnapShot.rtRect.bottom	= OutRect.top + tl->m_Original.rtRect.GetHeight();

	// BottomLine
	bl1->m_SnapShot.rtRect.left		= OutRect.left + lbc->m_Original.rtRect.GetWidth();
	bl1->m_SnapShot.rtRect.top		= OutRect.bottom - bl1->m_Original.rtRect.GetHeight();
	bl1->m_SnapShot.rtRect.right	= OutRect.right - rbc->m_Original.rtRect.GetWidth();
	bl1->m_SnapShot.rtRect.bottom	= OutRect.bottom;

	// Center
	center->m_SnapShot.rtRect.left	= OutRect.left + ll->m_Original.rtRect.GetWidth();
	center->m_SnapShot.rtRect.top	= OutRect.top + tl->m_Original.rtRect.GetHeight();
	center->m_SnapShot.rtRect.right	= OutRect.right - rl->m_Original.rtRect.GetWidth();
	center->m_SnapShot.rtRect.bottom= OutRect.bottom - bl1->m_Original.rtRect.GetHeight();

	if( m_bBalloon )
	{
		bl2->m_SnapShot.rtRect.right	= bl1->m_SnapShot.rtRect.right;
		bl1->m_SnapShot.rtRect.right	= nTailX;

		tail->m_SnapShot.rtRect.left	= nTailX;
		tail->m_SnapShot.rtRect.top		= bl1->m_SnapShot.rtRect.top;
		tail->m_SnapShot.rtRect.right	= nTailX + tail->m_Original.rtRect.GetWidth();
		tail->m_SnapShot.rtRect.bottom	= OutRect.bottom + tail->m_Original.rtRect.GetHeight() - bl1->m_Original.rtRect.GetHeight();

 		bl2->m_SnapShot.rtRect.left		= tail->m_SnapShot.rtRect.right;
		bl2->m_SnapShot.rtRect.top		= tail->m_SnapShot.rtRect.top;
		bl2->m_SnapShot.rtRect.bottom	= OutRect.bottom;
	}
		
}

CRectangle CDynamicOutline::SetInScreenRect( CRectangle& InRect, INT nTailX /* = 0  */)
{
	CRectangle rtOutRect;

	if( !m_bInitialized )
		return rtOutRect;

	rtOutRect.left	= InRect.left - m_nLeftCX;
	rtOutRect.top	= InRect.top - m_nTopCY;
	rtOutRect.right	= InRect.right + m_nRightCX;
	rtOutRect.bottom = InRect.bottom + m_nBottomCY;

	SetOutScreenRect( rtOutRect, nTailX );

	return rtOutRect;
}

VOID CDynamicOutline::SetMove( INT nScreenX, INT nScreenY )
{
	INT cx = m_rcOutRect.GetWidth();
	INT cy = m_rcOutRect.GetHeight();

	if( cx <= 0 || cy <= 0 )
		return;

	if( nScreenX == m_rcOutRect.left && nScreenY == m_rcOutRect.top )
		return;

	CRectangle rcOut( nScreenX, nScreenY, nScreenX + cx, nScreenY + cy );

	SetOutScreenRect( rcOut, m_nTailX );
}

VOID CDynamicOutline::SetSurfaceAlpha( BYTE ucAlpha )
{
	for( INT i = 0 ; i < NUM_SURFACE ; ++i )
	{
		CSurface* pSurface = &m_Surfaces[i];

		pSurface->m_SnapShot.uAlpha = ucAlpha;
	}
}

VOID CDynamicOutline::SetSurfaceColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
	for( INT i = 0 ; i < NUM_SURFACE ; ++i )
	{
		CSurface* pSurface = &m_Surfaces[i];

		pSurface->m_SnapShot.uRed = ucRed;
		pSurface->m_SnapShot.uGreen = ucGreen;
		pSurface->m_SnapShot.uBlue = ucBlue;
	}
}

END_GUI